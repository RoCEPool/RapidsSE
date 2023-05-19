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
#include "TagConstants.h"
#include "Procedure.hpp"
#include "Api.hpp"
#include "Job.hpp"

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

  // m_sQueryStr;
  m_pCommonTagSet = pCommonTagSet;
  m_pParentTask = pParentTask;

  m_pTaskList = new TaskList(QK_NONE);

  // Information pertaining to the current statement
  m_pCurStmtTagSet = nullptr;
  // m_sCurStmtStr
  m_szProcName = nullptr;
  m_pArgValues = nullptr;

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
  delete m_pTaskList;

  ASSERT(!m_pCurStmtTagSet);
  ASSERT(!m_szProcName);
  delete m_pArgValues;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Parse any SE system command                                                */
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
  // This allows us to determine the statement type, extract tags and other values, etc.
  // This may throw a SyntaxException or SemanticException.
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
/* Statements node (top-most level of the parse tree)                         */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitStatements(Parser::sqlParser::StatementsContext *pCtx)
{
  // Save the whole query text which may contain multiple statements.
  antlr4::misc::Interval interval(pCtx->start->getStartIndex(), pCtx->stop->getStopIndex());
  std::string queryStlStr = pCtx->start->getInputStream()->getText(interval);

  m_sQueryStr.vCopy((Stz*) queryStlStr.c_str());

  return visitChildren(pCtx);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Statement node                                                             */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitStatement(Parser::sqlParser::StatementContext *pCtx)
{
  // Save the current statement text
  antlr4::misc::Interval interval(pCtx->start->getStartIndex(), pCtx->stop->getStopIndex());
  std::string sStmtStr = pCtx->start->getInputStream()->getText(interval);

  m_sCurStmtStr.vCopy((Stz*) sStmtStr.c_str());

  // Create a tag set for this command and populate it with any common tags
  ASSERT(!m_pCurStmtTagSet);
  m_pCurStmtTagSet = new TagSet();
  m_pCurStmtTagSet->vCopy(m_pCommonTagSet);

  ASSERT(!m_szProcName);
  ASSERT(!m_pArgValues);
  m_pArgValues = new TokenVec();

  visitChildren(pCtx);

  // Except for raw pipeline statements being specified, all commands should be resolvable
  // to a procedure or pipeline statement in the dictionary.
  sSTMT sStmt;
  if (m_szProcName) {
    m_pCurStmtTagSet->vReplace(C_TAG_JOBNM, m_szProcName);

    if (!Procedure::bTranslateToPipeline(m_szProcName, m_pArgValues, m_sCurStmtStr.szStz(), sStmt.pString())) {
      // All external commands should be resolvable.
      NEVER_REACH
    }

  } else {
    // TODO: raw pipeline specified?
    sStmt.vCopy(m_sCurStmtStr);
    m_pCurStmtTagSet->vReplace(C_TAG_JOBNM, "pipeline"_Stz);
  }

  JobId tParentId;
  tParentId = m_pParentTask->pOuterJobOfTask()->tJobId();

  Task* pTask = Api::pCreateTask(sStmt.szStz(), m_pCurStmtTagSet, tParentId, m_sCurStmtStr.szStz());

  if (!pTask) {
    i8 iLine = pCtx->start->getLine();
    i8 iColumn = pCtx->start->getCharPositionInLine();
    Parser::SyntaxException::vThrow(iLine, iColumn, EC_INVALID_COMMAND);
  }

  m_pTaskList->vGrabLock();
  m_pTaskList->vAddTask(pTask);
  m_pTaskList->vDropLock();

  // TODO: post-statement cleanup.

  m_pCurStmtTagSet = nullptr;   // TODO: check where this gets deleted.
  m_sCurStmtStr.vReset();
  m_szProcName = nullptr;
  delete m_pArgValues;
  m_pArgValues = nullptr;

  return nullptr;
}


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* StatementChoice node                                                       */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitStatementChoice(Parser::sqlParser::StatementChoiceContext *pCtx)
{
  // Determine which procedure/pipeline needs to be invoked for a given command.
  // Note that for most commands we don't need to descend the parse tree any further
  // than this. Some others we do.

  if (pCtx->queryStatement() ||
      pCtx->explainStatement()) { m_szProcName = "sqlCompiler"_Stz;
                                  m_pArgValues->vAppendToken(m_sCurStmtStr, TN_STRING);
                                  return nullptr; }

  else if (pCtx->lstbl())       { m_szProcName = "lstbl"_Stz;         return nullptr; }

  else                          { NOT_IMPLEMENTED_YET }

  // NOTE:
  // the load/dload commands that accept multiple paths need to convert these
  // arguments into tags. Hence this part of the graph needs to be descended.
  // Refer to CmdHelper::vExtractCmdPaths.

  NEVER_REACH
}


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Extract a key-only tag and add it to the current statement's tag set       */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitKeyOnlyTag(Parser::sqlParser::KeyOnlyTagContext *pCtx)
{
  std::string sTagName;

  if (pCtx->TAG_FWDSLASH()) {
    sTagName = pCtx->TAG_FWDSLASH()->getText().substr(1);

  } else {
    ASSERT(pCtx->TAG_ANGLEBRACKETS());
    std::string s = pCtx->TAG_ANGLEBRACKETS()->getText();
    sTagName = s.substr(1, s.length() - 2);
  }

  m_pCurStmtTagSet->vAppend((Stz*) sTagName.c_str());

  return visitChildren(pCtx);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Extract a key-value tag and add it to the current statement's tag set      */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitKeyValueTag(Parser::sqlParser::KeyValueTagContext *pCtx)
{
  std::string sTagName;

  ASSERT(pCtx->TAG_ANGLEBRACKETS());
  std::string s = pCtx->TAG_ANGLEBRACKETS()->getText();
  sTagName = s.substr(1, s.length() - 2);

  if (pCtx->STRING_LITERAL()) {
    // Get value without quotes
    std::string sValue = pCtx->STRING_LITERAL()->getText().substr(1, pCtx->STRING_LITERAL()->getText().length() - 2);
    m_pCurStmtTagSet->vAppend((Stz*) sTagName.c_str(), (Stz*) sValue.c_str());

  } else {
    ASSERT(pCtx->INTEGER_LITERAL());
    i8 iValue = iCleanInt(pCtx->INTEGER_LITERAL()->getSymbol());
    m_pCurStmtTagSet->vAppend((Stz*) sTagName.c_str(), iValue);
  }

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
