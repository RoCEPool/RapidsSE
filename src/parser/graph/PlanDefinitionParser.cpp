/*============================================================================*/
/* PlanDefinitionParser.cpp - Parses SQL statements & produces an initial knowledgebase */
/*============================================================================*/

#include "../Parser.h"
#include "json5Lexer.h"
#include "json5Parser.h"
#include "json5BaseVisitor.h"
#include "ErrorListener.hpp"

//using namespace antlr4;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,PlanDefinitionParser,CN_PLANDEFINITIONPARSER)
//__aNewFunction(PlanDefinitionParser,CN_PLANDEFINITIONPARSER)
//__vDeleteFunction(PlanDefinitionParser,CN_PLANDEFINITIONPARSER)
__aFooterFunction(PlanDefinitionParser)


void* PlanDefinitionParser::operator new(size_t nSize)
{
  vTrace("PlanDefinitionParser::operator : nSize %lld ; sizeof() %lld\n"_Stz,
         nSize,sizeof(PlanDefinitionParser));
  
  void* pObject = HeapMem::pAllocate(CN_PLANDEFINITIONPARSER);
  
  return pObject;
}

void PlanDefinitionParser::operator delete(void* aAddr)
{
  PlanDefinitionParser*
    pObject = reinterpret_cast<PlanDefinitionParser*>(aAddr);

  HeapMem::vFree(pObject);
}

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
: Object(CN_PLANDEFINITIONPARSER)
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
   

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

b1 PlanDefinitionParser::findDefinitionFile(s2K& sBuf)
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
      sBuf = sPath;
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
PlanNode* PlanDefinitionParser::parse()
{
  // Find the PlannerDef.json5 file
  s2K sFilePath;
  if (!findDefinitionFile(sFilePath)) {
    return nullptr;
  }

//  // We need to give the Lexer a CharStream, but that requires an STL-string...
  //std::string filePath(pStr->azChar());
  antlr4::ANTLRFileStream stream(sFilePath.azChar());

  // Create the lexer and get the stream of tokens representing the input command.
//  sqlLexer lexer(&stream);
//  antlr4::CommonTokenStream tokens(&lexer);
//
//  // The thing that gets told of parsing errors.
//  ErrorListener errorListener;
//
//  // Create a parser from the stream of tokens.
//  sqlParser parser(&tokens);
//
//  // Add our own error listener so that we can be notified if there is a parsing error.
//  parser.removeErrorListeners();
//  parser.addErrorListener(&errorListener);
//
//  // Parse the token stream.
//  sqlParser::StatementContext* pStatementCtx = parser.statement();
//
//  // Check for any syntax errors
//  if (errorListener.bGotError()) {
//    vTrace("%s\n"_Stz, errorListener.szErrorMsg()->azChar());
//    return;
//  }
//
//  // Visit the parse tree that resulted from the above parsing.
//  this->visitStatement(pStatementCtx);
//  //vTrace("Finished visiting the parse tree.\n"_Stz);

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* Visit functions for parse tree nodes                                       */
/*----------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Statement node (top-most level of the parse tree)                          */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
//antlrcpp::Any PlanDefinitionParser::visitStatement(sqlParser::StatementContext *ctx)
//{
//  return visitChildren(ctx);
//}



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Clean up a name of an object. Returned string is dynamically allocated.    */
/*----------------------------------------------------------------------------*/
//Text* PlanDefinitionParser::cleanId(json5Parser::IdentifierContext *ctx)
//{
//  std::string idStr = ctx->getText();
//
//  i8 idLen = idStr.length();
//
//  if (idLen == 0) {
//    return new Text(""_Stz);
//  }
//
//  // Remove double quotes
//  if (idStr.at(0) == '"' && idStr.at(idLen - 1) == '"') {
//    // Not great, substr() makes a another copy of the string.
//    return new Text((Stz*) idStr.substr(1, idLen - 2).c_str());
//  }
//
//  // Remove backticks
//  if (idStr.at(0) == '`' && idStr.at(idLen - 1) == '`') {
//    // Not great, substr() makes a another copy of the string.
//    return new Text((Stz*) idStr.substr(1, idLen - 2).c_str());
//  }
//
//  return new Text((Stz*) idStr.c_str());
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
