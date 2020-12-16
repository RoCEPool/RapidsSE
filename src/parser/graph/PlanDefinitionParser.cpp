/*============================================================================*/
/* PlanDefinitionParser.cpp - Parses SQL statements & produces an initial knowledgebase */
/*============================================================================*/

#include "../Parser.h"
#include "json5Lexer.h"
#include "json5Parser.h"
#include "json5BaseVisitor.h"
#include "ErrorListener.hpp"
#include "PlanNodeBuilder.hpp"

#include "Dir.hpp" //?
#include "File.hpp" //?

//using namespace antlr4;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,PlanDefinitionParser,CN_PLANDEFINITIONPARSER)
__aNewFunction(PlanDefinitionParser,CN_PLANDEFINITIONPARSER)
__vDeleteFunction(PlanDefinitionParser,CN_PLANDEFINITIONPARSER)
__aFooterFunction(PlanDefinitionParser)


/*----------------------------------------------------------------------------*/
/* constants                                                                  */
/*----------------------------------------------------------------------------*/
Stz* PlanDefinitionParser::definitionPaths[] = {
    "../src/parser/graph/PlannerDef.json5"_Stz,
    "../PlannerDef.json5"_Stz,
    "../res/PlannerDef.json5"_Stz,
    nullptr};

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PlanDefinitionParser::PlanDefinitionParser(void)
: Object(CN_PLANDEFINITIONPARSER),
  m_pNodeBuilderStack(new Stack(KK_PARSER_STACK))
{
   _precondition_(_ADDRESS_,_MAGIC_,CN_PLANDEFINITIONPARSER,_FOOTER_);
  
  _checkInitialised_(CN_PLANDEFINITIONPARSER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PLANDEFINITIONPARSER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PlanDefinitionParser::~PlanDefinitionParser(void) noexcept
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANDEFINITIONPARSER,_FOOTER_);
   
  ASSERT(m_pNodeBuilderStack->nValues() == 0);
  delete m_pNodeBuilderStack;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

b1 PlanDefinitionParser::bFindDefinitionFile(s2K& pBuf)
{
  // Look in
  //    "../src/parser/graph/PlannerDef.json5"
  //    "../PlannerDef.json5"
  //    "../res/PlannerDef.json5"

  i8 i = 0;
  Stz* sPath;

  while (true) {
    sPath = definitionPaths[i++];

    if (sPath == nullptr) {
      break;
    }

    if (File::bExists(sPath), true) {
      // We found it. Copy the path into the buffer
      pBuf = sPath;
      return true;
    }
  }

  s2K errorMsg;
  errorMsg = "Couldn't find the planner definition json file at any of the following locations:"_Stz;

  s256 sCwd;
  if (!Dir::bGetCurrent(sCwd)) {
    return false;
  }

  i = 0;
  while(true) {
    if (definitionPaths[i] == nullptr)  break;
    errorMsg.vCatSprint("\n  %s/%s"_Stz, sCwd.szStz(), definitionPaths[i++]);
  }

  vWarn(errorMsg.szStz());
  return false;
}

/*----------------------------------------------------------------------------*/
/* Parse and create an initial plan for a SQL statement                       */
/*----------------------------------------------------------------------------*/
PlanNode* PlanDefinitionParser::pParse()
{
  // Find the PlannerDef.json5 file
  s2K sFilePath;
  if (!bFindDefinitionFile(sFilePath)) {
    return nullptr;
  }

  // Read the json5 definition file from disk
  antlr4::ANTLRFileStream stream(sFilePath.azChar());

  // Create the lexer and get the stream of tokens representing the input command.
  json5Lexer lexer(&stream);
  antlr4::CommonTokenStream tokens(&lexer);

  // The thing that gets told of parsing errors.
  ErrorListener errorListener;

  // Create a parser from the stream of tokens.
  json5Parser parser(&tokens);

  // Add our own error listener so that we can be notified if there is a parsing error.
  parser.removeErrorListeners();
  parser.addErrorListener(&errorListener);

  // Parse the token stream.
  json5Parser::Json5Context* pJson5Ctx = parser.json5();

  // Check for any syntax errors
  if (errorListener.bGotError()) {
    vTrace("%s\n"_Stz, errorListener.szErrorMsg()->azChar());
    return nullptr;
  }

  // Visit the parse tree that resulted from the above parsing.
  this->visitJson5(pJson5Ctx);

  PlanNodeBuilder* rootBuilder = static_cast<PlanNodeBuilder*>(m_pNodeBuilderStack->pPopPtr(__FUNCTION__));

  PlanNode* rootPlan = rootBuilder->pBuild();
  delete rootBuilder;
  return rootPlan;
}


/*----------------------------------------------------------------------------*/
/* Visit functions for parse tree nodes                                       */
/*----------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Json5 node (top-most level of the parse tree)                              */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any PlanDefinitionParser::visitJson5(json5Parser::Json5Context *ctx)
{
  // We always start off with an object
  ASSERT(ctx->value()->obj() != nullptr);

  antlrcpp::Any any = visitObj(ctx->value()->obj());

  return any;
}

antlrcpp::Any PlanDefinitionParser::visitObj(json5Parser::ObjContext *ctx)
{
  // Create a mutable object to hold the fields of this node. Put it on a stack
  // so it can be filled in when we parse the pairs.
  PlanNodeBuilder* pNodeBuilder = new PlanNodeBuilder();
  m_pNodeBuilderStack->vPushPtr(pNodeBuilder);

  // Visit the key-value pairs of this object
  visitChildren(ctx);

  return nullptr;
}


antlrcpp::Any PlanDefinitionParser::visitPair(json5Parser::PairContext *ctx)
{
  json5Parser::KeyContext* keyCtx = ctx->key();
  json5Parser::ValueContext* valueCtx = ctx->value();

  s256 keyName;

  // Get the PlanNodeBuilder to add these properties to. Don't remove it from the stack though.
  PlanNodeBuilder* pNodeBuilder = static_cast<PlanNodeBuilder*>(m_pNodeBuilderStack->aPeekVal(0)->pObj());
  ASSERT(m_pNodeBuilderStack != nullptr);

  if (keyCtx->STRING() != nullptr) {
    vCleanString(keyCtx->getText(), keyName);
  } else {
    vCleanString(keyCtx->getText(), keyName);
  }

  // Extract the known values
  if (keyName.bEqual("name"_Stz)) {
    ASSERT(valueCtx->STRING() != nullptr);
    vCleanString(valueCtx->getText(), pNodeBuilder->m_sName);

  } else if (keyName.bEqual("type"_Stz)) {
    ASSERT(valueCtx->STRING() != nullptr);
    vCleanString(valueCtx->getText(), pNodeBuilder->m_sType);

  } else if (keyName.bEqual("value"_Stz)) {
    vExtractValue(valueCtx, pNodeBuilder);

  } else if (keyName.bEqual("properties"_Stz)) {
    // Use the visit function on this child
    ASSERT(valueCtx->arr() != nullptr);
    ASSERT(pNodeBuilder->m_pProperties == nullptr);
    pNodeBuilder->m_pProperties = visitArr(valueCtx->arr());

  } else if (keyName.bEqual("children"_Stz)) {
    ASSERT(valueCtx->arr() != nullptr);
    ASSERT(pNodeBuilder->m_pChildren == nullptr);
    pNodeBuilder->m_pChildren = visitArr(valueCtx->arr());

  } else {
    // Got a key name that we didn't recognise.
    NEVER_REACH
  }

  //return visitChildren(ctx);
  return nullptr;
}


antlrcpp::Any PlanDefinitionParser::visitArr(json5Parser::ArrContext *ctx)
{
  PlanNodeBuilderList* pBuilderList = new PlanNodeBuilderList();

  std::vector<json5Parser::ValueContext*> values = ctx->value();
  std::vector<json5Parser::ValueContext*>::iterator iterator;

  for (iterator = values.begin(); iterator < values.end(); iterator++) {
    json5Parser::ValueContext* ctx = *iterator;

    // All array elements must be objects
    ASSERT(ctx->obj() != nullptr);

    // Visit this object manually. It will leave a PlanNodeBuilder on the stack.
    visitObj(ctx->obj());

    // Put it in the array
    PlanNodeBuilder* pBuilder = static_cast<PlanNodeBuilder*>(m_pNodeBuilderStack->pPopPtr(__FUNCTION__));
    pBuilderList->vAppend(pBuilder);
  }

  return pBuilderList;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


void PlanDefinitionParser::vExtractValue(json5Parser::ValueContext* ctx, PlanNodeBuilder* pNodeBuilder)
{
  pNodeBuilder->m_xValue.bMustFree = false;

  if (ctx->STRING() != nullptr) {
    pNodeBuilder->m_xValue.m_pText = pCleanString(ctx->STRING()->getText());
    pNodeBuilder->m_xValue.bMustFree = true;

  } else if (ctx->NUMBER() != nullptr) {
    // TODO: handle floating point numbers and hexadecimal
    s256 sNum;
    vCleanString(ctx->NUMBER()->getText(), sNum);
    pNodeBuilder->m_xValue.m_iInt = sNum.iToI8();

  } else if (ctx->obj() != nullptr) {
    NEVER_REACH
    // Use the visit function on this child
//    visitObj(ctx->obj());
//    pNodeBuilder->m_xValue.m_pXref = static_cast<PlanNodeBuilder*>(m_pNodeBuilderStack->pPopPtr(__FUNCTION__));

  } else if (ctx->arr() != nullptr) {
     // Can't have an array in a value field
    NEVER_REACH

  } else if (ctx->LITERAL() != nullptr) {
    if (ctx->LITERAL()->getText().compare("true") == 0) {
      pNodeBuilder->m_xValue.m_bBoolean = true;

    } else if (ctx->LITERAL()->getText().compare("false") == 0) {
      pNodeBuilder->m_xValue.m_bBoolean = false;

    } else if (ctx->LITERAL()->getText().compare("null") == 0) {
      pNodeBuilder->m_xValue.m_pText = nullptr;

    } else {
      // Got a literal value we didn't expect
      NEVER_REACH
    }

  } else {
    // Got a parameter whose name we don't recognise
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Clean up a name of an quoted string and save it in the keyName parameter.  */
/*----------------------------------------------------------------------------*/
void PlanDefinitionParser::vCleanString(std::string idStr, s256& keyName)
{
  i8 idLen = idStr.length();

  if (idLen == 0) {
    keyName.vReset();
    return;
  }

  // Remove double quotes
  if (idStr.at(0) == '"' && idStr.at(idLen - 1) == '"') {
    // Not great, substr() makes a another copy of the string.
    keyName = (Stz*) idStr.substr(1, idLen - 2).c_str();
    return;
  }

  // Remove single quotes
  if (idStr.at(0) == '\'' && idStr.at(idLen - 1) == '\'') {
    // Not great, substr() makes a another copy of the string.
    keyName = (Stz*) idStr.substr(1, idLen - 2).c_str();
    return;
  }

  keyName = (Stz*) idStr.c_str();
}

/*----------------------------------------------------------------------------*/
/* Clean up a  quoted string and return it as a dynamically allocated Text*   */
/*----------------------------------------------------------------------------*/
Text* PlanDefinitionParser::pCleanString(std::string idStr)
{
  i8 idLen = idStr.length();

  if (idLen == 0) {
    return new Text(""_Stz);
  }

  // Remove double quotes
  if (idStr.at(0) == '"' && idStr.at(idLen - 1) == '"') {
    // Not great, substr() makes a another copy of the string.
    return new Text((Stz*) idStr.substr(1, idLen - 2).c_str());
  }

  // Remove single quotes
  if (idStr.at(0) == '\'' && idStr.at(idLen - 1) == '\'') {
    // Not great, substr() makes a another copy of the string.
    return new Text((Stz*) idStr.substr(1, idLen - 2).c_str());
  }

  return new Text((Stz*) idStr.c_str());
}

/*----------------------------------------------------------------------------*/
/* Clean up a name of an identifier and save it in the keyName parameter.     */
/*----------------------------------------------------------------------------*/
void PlanDefinitionParser::vCleanId(std::string idStr, s256& keyName)
{
  keyName = (Stz*) idStr.c_str();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
