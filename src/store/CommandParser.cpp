/*============================================================================*/
/* CommandParser.cpp - Parses all external SE commands & produces tasks       */
/*============================================================================*/

#include "BASE.h"
#include "CommandParser.hpp"

#include "sqlLexer.h"
#include "sqlParser.h"
#include "sqlBaseVisitor.h"

#include "parser/common/ErrorListener.hpp"
#include "SqlException.hpp"

#include "Text.hpp"
#include "AttachMap.hpp"

extern AttachMap*  gpAttachMap;

using namespace STORE;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(STORE,CommandParser,CN_COMMANDPARSER)
__aNewFunction(CommandParser,CN_COMMANDPARSER)
__vDeleteFunction(CommandParser,CN_COMMANDPARSER)
__aFooterFunction(CommandParser)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CommandParser::CommandParser(TagSet* pCommonTagSet, Task* pParentTask)
: Object(CN_COMMANDPARSER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPARSER,_FOOTER_);

  ASSERT(pCommonTagSet);
  ASSERT(pParentTask);

  m_pCommonTagSet = pCommonTagSet;
  m_pParentTask = pParentTask;

  m_pTaskList = new TaskList(QK_NONE);
  _checkInitialised_(CN_COMMANDPARSER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPARSER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CommandParser::~CommandParser(void) noexcept
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPARSER,_FOOTER_);

  delete m_pCommonTagSet;
  m_pParentTask = nullptr;

  if (m_pTaskList) {
    Task* pTask;

    m_pTaskList->vGrabLock();
      while ((pTask = m_pTaskList->pPopTask()) != nullptr) {
        delete pTask;
      }
    m_pTaskList->vDropLock();
    delete m_pTaskList;
  }
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Parse and create an initial plan for a SQL statement                       */
/*----------------------------------------------------------------------------*/
TaskList* CommandParser::pParseCommand(Stz* szCommand)
{
  // We need to give the Lexer a CharStream, but that requires an STL-string...
  std::string sqlStr(szCommand->azChar());
  antlr4::ANTLRInputStream stream(sqlStr);
  
  // Create the lexer and get the stream of tokens representing the input command.
  Parser::sqlLexer lexer(&stream);
  antlr4::CommonTokenStream tokens(&lexer);
  
  // The thing that gets told of parsing errors.
  Parser::ErrorListener errorListener;
  
  // Create a parser from the stream of tokens.
  Parser::sqlParser parser(&tokens);
  
  // Add our own error listener so that we can be notified if there is a parsing error.
  parser.removeErrorListeners();
  parser.addErrorListener(&errorListener);
  
  // Parse the token stream.
  Parser::sqlParser::StatementsContext* pStatementsCtx = parser.statements();
  
  // Check for any syntax errors
  if (errorListener.bGotError()) {
    //vTrace("%s\n"_Stz, errorListener.szErrorMsg()->azChar());
    Parser::SyntaxException::vThrowAntlr(errorListener.iLine(), errorListener.iColumn(), errorListener.szErrorMsg());
  }


  // Visit the parse tree that resulted from the above parsing.
  // May throw a SyntaxException or SemanticException.
  this->visitStatements(pStatementsCtx);

  // TaskList is owned by the caller.
  TaskList* pTaskList = m_pTaskList;
  m_pTaskList = nullptr;

  return pTaskList;
}


/*----------------------------------------------------------------------------*/
/* Visit functions for parse tree nodes                                       */
/*----------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Statement node (top-most level of the parse tree)                          */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitStatements(Parser::sqlParser::StatementsContext *pCtx)
{
  // Save the whole statement text
  antlr4::misc::Interval interval(pCtx->start->getStartIndex(), pCtx->stop->getStopIndex());
  std::string queryStlStr = pCtx->start->getInputStream()->getText(interval);

  m_sQueryStr.vCopy((Stz*) queryStlStr.c_str());

  return visitChildren(pCtx);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Clean up a name of an object. Returned string is dynamically allocated.    */
/*----------------------------------------------------------------------------*/
Text* CommandParser::pCleanId(Parser::sqlParser::IdentifierContext *pCtx)
{
  if (!pCtx) {
    return nullptr;
  }

  std::string idStr = pCtx->getText();
  
  i8 idLen = idStr.length();
  
  if (idLen == 0) {
    return new Text(""_Stz);
  }
      
  // Remove double quotes
  if (idStr.at(0) == '"' && idStr.at(idLen - 1) == '"') {
    // Not great, substr() makes a another copy of the string.
    return new Text((Stz*) idStr.substr(1, idLen - 2).c_str());
  }
  
  // Remove backticks
  if (idStr.at(0) == '`' && idStr.at(idLen - 1) == '`') {
    // Not great, substr() makes a another copy of the string.
    return new Text((Stz*) idStr.substr(1, idLen - 2).c_str());
  }
  
  return new Text((Stz*) idStr.c_str());
}

/*----------------------------------------------------------------------------*/
/* Clean up a a string literal. Returned string is dynamically allocated.     */
/*----------------------------------------------------------------------------*/
Text* CommandParser::pCleanString(Parser::sqlParser::StringLiteralContext *pCtx)
{
  if (!pCtx) {
    return nullptr;
  }

  std::string idStr = pCtx->getText();

  i8 idLen = idStr.length();

  if (idLen == 0) {
    return new Text(""_Stz);
  }

  // Remove single quotes
  if (idStr.at(0) == '\'' && idStr.at(idLen - 1) == '\'') {
    // Not great, substr() makes a another copy of the string.
    Text* pText = new Text((Stz*) idStr.substr(1, idLen - 2).c_str());

    // Unescape any two consecutive single quotes
    while (StrLib::bReplace(pText->szGet()->azChar(), "''", "'")) { /*noop*/ }
    pText->vSizeText();

    return pText;
  }

  // Remove double quotes
  if (idStr.at(0) == '"' && idStr.at(idLen - 1) == '"') {
    // Not great, substr() makes a another copy of the string.
    Text* pText = new Text((Stz*) idStr.substr(1, idLen - 2).c_str());

    // Unescape any two consecutive double quotes
    while (StrLib::bReplace(pText->szGet()->azChar(), "\"\"", "\"")) { /*noop*/ }
    pText->vSizeText();

    return pText;
  }

  return new Text((Stz*) idStr.c_str());
}

/*----------------------------------------------------------------------------*/
/* Clean up a string token. Returned string is dynamically allocated.         */
/*----------------------------------------------------------------------------*/
Text* CommandParser::pCleanString(antlr4::Token* pToken)
{
  if (!pToken) {
    return nullptr;
  }

  return new Text((Stz*) pToken->getText().c_str());
}


/*----------------------------------------------------------------------------*/
/* Clean up a string literal. Returned string is written to the given buffer  */
/*----------------------------------------------------------------------------*/
void CommandParser::vCleanString(Parser::sqlParser::StringLiteralContext *pCtx, s256& sBuf)
{
  if (!pCtx) {
    return;
  }

  std::string idStr = pCtx->getText();
  vCleanString(idStr, sBuf);
}

/*----------------------------------------------------------------------------*/
/* Clean up a string literal. Returned string is written to the given buffer  */
/*----------------------------------------------------------------------------*/
void CommandParser::vCleanString(Parser::sqlParser::IdentifierContext *pCtx, s256& sBuf)
{
  if (!pCtx) {
    return;
  }

  std::string idStr = pCtx->getText();
  vCleanString(idStr, sBuf);
}

/*----------------------------------------------------------------------------*/
/* Clean up a string literal. Returned string is written to the given buffer  */
/*----------------------------------------------------------------------------*/
void CommandParser::vCleanString(antlr4::Token* pToken, s256& sBuf)
{
  if (!pToken) {
    return;
  }

  std::string sStr = pToken->getText();
  vCleanString(sStr, sBuf);
}

/*----------------------------------------------------------------------------*/
/* Clean up a string literal. Returned string is written to the given buffer  */
/*----------------------------------------------------------------------------*/
void CommandParser::vCleanString(std::string& sStr, s256& sBuf)
{
  i8 len = sStr.length();

  if (len == 0) {
    return;
  }

  // Ensure that the string is short enough to fit in the buffer
  RASSERT(len < sBuf.nSpace());

  // Remove single quotes
  if (sStr.at(0) == '\'' && sStr.at(len - 1) == '\'') {
    // Not great, substr() makes a another copy of the string.
    sBuf.vCopy((Stz*) sStr.substr(1, len - 2).c_str());

    // Unescape any two consecutive single quotes
    while (StrLib::bReplace(sBuf.azChar(), "''", "'")) { /*noop*/ }
    return;
  }

  // Remove double quotes
  else if (sStr.at(0) == '"' && sStr.at(len - 1) == '"') {
    // Not great, substr() makes a another copy of the string.
    sBuf.vCopy((Stz*) sStr.substr(1, len - 2).c_str());

    // Unescape any two consecutive double quotes
    while (StrLib::bReplace(sBuf.azChar(), "\"\"", "\"")) { /*noop*/ }
    return;


  } else {
    // No quotes. Just copy the string
    sBuf.vCopy((Stz*) sStr.c_str());
  }
}

/*----------------------------------------------------------------------------*/
/* Return an integer value from an integer literal token                      */
/*----------------------------------------------------------------------------*/
i8 CommandParser::iCleanInt(Parser::sqlParser::SignedIntegerLiteralContext *pCtx)
{
  ASSERT(pCtx);

  std::string sStr = pCtx->getText();
  return iCleanInt(sStr);
}

/*----------------------------------------------------------------------------*/
/* Return an integer value from an integer literal token                      */
/*----------------------------------------------------------------------------*/
i8 CommandParser::iCleanInt(Parser::sqlParser::IntegerLiteralContext *pCtx)
{
  ASSERT(pCtx);

  std::string sStr = pCtx->getText();
  return iCleanInt(sStr);
}

/*----------------------------------------------------------------------------*/
/* Return an integer value from an integer literal token                      */
/*----------------------------------------------------------------------------*/
i8 CommandParser::iCleanInt(antlr4::Token* pToken)
{
  ASSERT(pToken);

  std::string sStr = pToken->getText();
  return iCleanInt(sStr);
}

/*----------------------------------------------------------------------------*/
/* Return an integer value from an integer literal token                      */
/*----------------------------------------------------------------------------*/
i8 CommandParser::iCleanInt(std::string& sStr)
{
  i8 iVal;

  try {
    iVal = std::stoll(sStr);

  } catch (std::invalid_argument &e) {
    Parser::SemanticException::vThrow(EC_INVALID_INTEGER_FORMAT,
                              ": \"%s\""_Stz,
                              sStr.c_str());

  } catch (std::out_of_range &e) {
    Parser::SemanticException::vThrow(EC_INVALID_INTEGER_FORMAT,
                              ": Value of out range: \"%s\". Integer range is %lld to %lld"_Stz,
                              sStr.c_str(), C_INT_MIN, C_INT_MAX);
  }

  // Because NULL is represented as a bit pattern, we need to restrict the
  // literal value range a little bit further. Refer to the definitions in
  // the file Fragments.cpp (specifically C_DATUM_NULL).

  if (iVal > 9223372036854775806 || iVal < -9223372036854775806) {
    Parser::SemanticException::vThrow(EC_INVALID_INTEGER_FORMAT,
                              ": Value of out range: \"%s\". Integer range is %lld to %lld"_Stz,
                              sStr.c_str(), C_INT_MIN, C_INT_MAX);
  }

  return iVal;
}

/*----------------------------------------------------------------------------*/
/* Return an unsigned integer value from an unsigned integer literal token    */
/*----------------------------------------------------------------------------*/
u8 CommandParser::uCleanInt(Parser::sqlParser::SignedIntegerLiteralContext *pCtx)
{
  ASSERT(pCtx);

  std::string sStr = pCtx->getText();
  return uCleanInt(sStr);
}

/*----------------------------------------------------------------------------*/
/* Return an unsigned integer value from an unsigned integer literal token    */
/*----------------------------------------------------------------------------*/
u8 CommandParser::uCleanInt(Parser::sqlParser::IntegerLiteralContext *pCtx)
{
  ASSERT(pCtx);

  std::string sStr = pCtx->getText();
  return uCleanInt(sStr);
}

/*----------------------------------------------------------------------------*/
/* Return an unsigned integer value from an unsigned integer literal token    */
/*----------------------------------------------------------------------------*/
u8 CommandParser::uCleanInt(antlr4::Token* pToken)
{
  ASSERT(pToken);

  std::string sStr = pToken->getText();
  return uCleanInt(sStr);
}

/*----------------------------------------------------------------------------*/
/* Return an unsigned integer value from an unsigned integer literal token    */
/*----------------------------------------------------------------------------*/
u8 CommandParser::uCleanInt(std::string& sStr)
{
  u8 uVal;

  try {
    uVal = std::stoull(sStr);

  } catch (std::invalid_argument &e) {
    Parser::SemanticException::vThrow(EC_INVALID_INTEGER_FORMAT,
                              ": \"%s\""_Stz,
                              sStr.c_str());

  } catch (std::out_of_range &e) {
    Parser::SemanticException::vThrow(EC_INVALID_INTEGER_FORMAT,
                              ": Value of out range: \"%s\". Valid range is %lld to %lld"_Stz,
                              sStr.c_str(), 0, C_INT_MAX);
  }

  // Because NULL is represented as a bit pattern, we need to restrict the
  // literal value range a little bit further. Refer to the definitions in
  // the file Fragments.cpp (specifically C_DATUM_NULL).

  if (uVal > 9223372036854775806) {
    Parser::SemanticException::vThrow(EC_INVALID_INTEGER_FORMAT,
                              ": Value of out range: \"%s\". Valid range is %lld to %lld"_Stz,
                              sStr.c_str(), 0, C_INT_MAX);
  }

  return uVal;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
