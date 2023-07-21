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

#include "ExtendedText.hpp"
#include "AttachMap.hpp"
#include "TagConstants.h"
#include "Procedure.hpp"
#include "Api.hpp"
#include "Job.hpp"

extern AttachMap*  gpAttachMap;

using namespace STORE;

static Stz* C_EMPTY_CMD = "emptyCommand"_Stz;

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
  m_szProcName  = nullptr;
  m_pArgValues  = nullptr;
  m_pChildTasks = nullptr;

  _checkInitialised_(CN_COMMANDPARSER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPARSER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CommandParser::~CommandParser(void) noexcept
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMMANDPARSER,_FOOTER_);

  m_pCommonTagSet = nullptr; // not owned
  m_pParentTask = nullptr;
  delete m_pTaskList;

  delete m_pCurStmtTagSet;
  m_pCurStmtTagSet = nullptr;
  m_szProcName = nullptr;

  delete m_pArgValues;
  delete m_pChildTasks;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Parse any SE system command                                                */
/*----------------------------------------------------------------------------*/
TaskList* CommandParser::pParseCommand(Stz* szCommand)
{
  // The thing that gets told of parsing errors.
  Parser::ErrorListener errorListener;

  // We need to give the Lexer a CharStream, but that requires an STL-string...
  std::string sqlStr(szCommand->azChar());
  antlr4::ANTLRInputStream stream(sqlStr);

  // Create the lexer and get the stream of tokens representing the input command.
  Parser::sqlLexer lexer(&stream);
  lexer.removeErrorListeners();
  lexer.addErrorListener(&errorListener);

  // Tokenise the input stream
  antlr4::CommonTokenStream tokens(&lexer);

  // Check for any syntax errors
  if (errorListener.bGotError()) {
    //vTrace("%s\n"_Stz, errorListener.szErrorMsg()->azChar());
    Parser::SyntaxException::vThrowAntlr(errorListener.iLine(), errorListener.iColumn(), errorListener.szErrorMsg());
  }

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

  ASSERT(!m_pChildTasks);

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
  if (pCtx->start == nullptr || pCtx->stop == nullptr) {
    // Handle a file with no statements, just comments
    m_sQueryStr.vReset();
    return visitChildren(pCtx);
  }

  // Save the whole query text which may contain multiple statements.
  antlr4::misc::Interval interval(pCtx->start->getStartIndex(), pCtx->stop->getStopIndex());
  std::string queryStlStr = pCtx->start->getInputStream()->getText(interval);

  m_sQueryStr.vCopy((Stz*) queryStlStr.c_str());

  visitChildren(pCtx);

  // If there aren't any tasks created then we will insert a noop task.
  // Reason being that the client has sent a request and that request is not
  // really invalid, it's just empty. The server shouldn't respond with an error
  // so the easiest thing is to just execute a noop and let it respond affirmatively.
  // Empty commands that occur in the presence of a batched query are otherwise
  // ignored.

  if (m_pTaskList->nTasks() == 0) {

    Task* pNoopTask = pCreateNoopTask((Stz*) queryStlStr.c_str());

    m_pTaskList->vGrabLock();
    m_pTaskList->vAddTask(pNoopTask);
    m_pTaskList->vDropLock();
  }

  return nullptr;
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

  JobId tParentId;
  tParentId = m_pParentTask->pOuterJobOfTask()->tJobId();

  // Except for raw pipeline statements being specified, all commands should be resolvable
  // to a procedure or pipeline statement in the dictionary.
  // Some commands are virtual and are replaced by their children. e.g. "run myfile.sql;"
  // will be replaced by the commands found in myfile.sql.
  sSTMT sStmt;
  if (m_szProcName) {
    ASSERT(!m_pChildTasks);

    if (m_szProcName == C_EMPTY_CMD) {
      // It's an empty command. We don't want to create a task for this, unless
      // it is the only statement in the query (but we will do that in visitStatements())

      // cleanup
      delete m_pCurStmtTagSet;
      m_pCurStmtTagSet = nullptr;
      m_sCurStmtStr.vReset();
      m_szProcName = nullptr;
      delete m_pArgValues;
      m_pArgValues = nullptr;

      return nullptr;
    }

    m_pCurStmtTagSet->vReplace(C_TAG_JOBNM, m_szProcName);
    // TODO: raw pipeline specified?

    if (!Procedure::bTranslateToPipeline(m_szProcName, m_pArgValues, m_sCurStmtStr.szStz(), sStmt.pString())) {
      // All external commands should be resolvable.
      NEVER_REACH
    }

    Task* pTask = Api::pCreateTask(sStmt.szStz(), m_pCurStmtTagSet, tParentId, m_sCurStmtStr.szStz());

    // cleanup
    delete m_pCurStmtTagSet;
    m_pCurStmtTagSet = nullptr;
    m_sCurStmtStr.vReset();
    m_szProcName = nullptr;
    delete m_pArgValues;
    m_pArgValues = nullptr;

    if (!pTask) {
      i8 iLine = pCtx->start->getLine();
      i8 iColumn = pCtx->start->getCharPositionInLine();
      Parser::SyntaxException::vThrow(iLine, iColumn, EC_INVALID_COMMAND);
    }

    m_pTaskList->vGrabLock();
    m_pTaskList->vAddTask(pTask);
    m_pTaskList->vDropLock();

    return nullptr;

  } else if (m_pChildTasks) {
    ASSERT(m_pChildTasks->nTasks() > 0);

    m_pChildTasks->vGrabLock();
    m_pTaskList->vGrabLock();

    Task* pTask;
    while ( (pTask = m_pChildTasks->pPopTask()) != nullptr) {
      m_pTaskList->vAddTask(pTask);
    }

    m_pTaskList->vDropLock();
    m_pChildTasks->vDropLock();

    delete m_pCurStmtTagSet;
    m_pCurStmtTagSet = nullptr;
    m_sCurStmtStr.vReset();
    m_szProcName = nullptr;
    delete m_pChildTasks;
    m_pChildTasks = nullptr;
    delete m_pArgValues;
    m_pArgValues = nullptr;

    return nullptr;

  } else {
    NEVER_REACH
  }
}


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* StatementChoice node                                                       */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitStatementChoice(Parser::sqlParser::StatementChoiceContext *pCtx)
{
  // Determine which procedure/pipeline needs to be invoked for a given command.
  // Note that we partially descend the parse tree for any commands that have parameters.
  // This if statement is ordered in roughly relative order of statement commonality.

  if (pCtx->queryStatement() || pCtx->explainStatement())
                                  { m_szProcName = "sqlCompiler"_Stz;     return visitChildren(pCtx); }
  else if (pCtx->createQuery() && pCtx->createQuery()->createTableQuery())
                                  { m_szProcName = "create__table"_Stz;   return visitChildren(pCtx); }
  else if (pCtx->createQuery() && pCtx->createQuery()->createIndexQuery())
                                  { m_szProcName = "create__index"_Stz;   return visitChildren(pCtx); }
  else if (pCtx->dropQuery() && pCtx->dropQuery()->dropTableQuery())
                                  { m_szProcName = "drop__table"_Stz;     return visitChildren(pCtx); }
  else if (pCtx->dropQuery() && pCtx->dropQuery()->dropIndexQuery())
                                  { m_szProcName = "drop__index"_Stz;     return visitChildren(pCtx); }
  else if (pCtx->load())          { m_szProcName = "load"_Stz;            return visitChildren(pCtx); }
  else if (pCtx->dload())         { m_szProcName = "dload"_Stz;           return visitChildren(pCtx); }
  else if (pCtx->truncateQuery()) { m_szProcName = "truncate"_Stz;        return visitChildren(pCtx); }

  else if (pCtx->log())           { m_szProcName = "logMsg"_Stz;          return visitChildren(pCtx); }
  else if (pCtx->lscol())         { m_szProcName = "lscol"_Stz;           return visitChildren(pCtx); }
  else if (pCtx->lsidx())         { m_szProcName = "lsidx"_Stz;           return visitChildren(pCtx); }
  else if (pCtx->lsidxcol())      { m_szProcName = "lsidxcol"_Stz;        return visitChildren(pCtx); }
  else if (pCtx->lstbl())         { m_szProcName = "lstbl"_Stz;           return nullptr; }
  else if (pCtx->count())         { m_szProcName = "count"_Stz;           return visitChildren(pCtx); }
  else if (pCtx->sleep())         { m_szProcName = "sleepCmd"_Stz;        return visitChildren(pCtx); }
  else if (pCtx->space())         { m_szProcName = "space"_Stz;           return nullptr; }
  else if (pCtx->status())        { m_szProcName = "status"_Stz;          return nullptr; }
  else if (pCtx->szidx())         { m_szProcName = "szidx"_Stz;           return visitChildren(pCtx); }
  else if (pCtx->sztbl())         { m_szProcName = "sztbl"_Stz;           return visitChildren(pCtx); }
  else if (pCtx->trackmemory())   { m_szProcName = "trackmemory"_Stz;     return visitChildren(pCtx); }
  else if (pCtx->memoryleaks())   { m_szProcName = "memoryleaks"_Stz;     return nullptr; }
  else if (pCtx->bpa())           { m_szProcName = "bpa"_Stz;             return visitChildren(pCtx); }
  else if (pCtx->file())          { m_szProcName = nullptr;               return visitChildren(pCtx); }
  else if (pCtx->noop())          { m_szProcName = "noop"_Stz;            return nullptr; }
  else if (pCtx->emptyCommand())  { m_szProcName = C_EMPTY_CMD;           return nullptr; }

  else                            { NOT_IMPLEMENTED_YET }

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
  s256 sTagName;

  ASSERT(pCtx->PLAIN_IDENTIFIER());

  vCleanTag(pCtx->PLAIN_IDENTIFIER()->getSymbol(), sTagName);
  m_pCurStmtTagSet->vAppend((Stz*) sTagName.szStz());

  return visitChildren(pCtx);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Extract a key-value tag and add it to the current statement's tag set      */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitKeyValueTag(Parser::sqlParser::KeyValueTagContext *pCtx)
{
  s256 sTagName;

  ASSERT(pCtx->keyName);
  vCleanTag(pCtx->keyName, sTagName);

  if (pCtx->stringLiteral()) {
    // Get value without quotes
    std::string sValue = pCtx->stringLiteral()->getText().substr(1, pCtx->stringLiteral()->getText().length() - 2);
    m_pCurStmtTagSet->vAppend((Stz*) sTagName.szStz(), (Stz*) sValue.c_str());

  } else {
    ASSERT(pCtx->INTEGER_LITERAL());
    i8 iValue = iCleanInt(pCtx->INTEGER_LITERAL()->getSymbol());
    m_pCurStmtTagSet->vAppend((Stz*) sTagName.szStz(), iValue);
  }

  return visitChildren(pCtx);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Add command parameters for a query statement                               */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitQueryStatement(Parser::sqlParser::QueryStatementContext *pCtx)
{
  UNUSED(pCtx);

  // Add command parameters
  m_pArgValues->vAppendToken(m_sCurStmtStr, TN_STRING);
  return nullptr; // No need to visit any further down the tree
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Add command parameters for an explain statement                            */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitExplainStatement(Parser::sqlParser::ExplainStatementContext *pCtx)
{
  UNUSED(pCtx);

  // Add command parameters
  m_pArgValues->vAppendToken(m_sCurStmtStr, TN_STRING);
  return nullptr; // No need to visit any further down the tree
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Add command parameters for a log statement                                 */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitLog(Parser::sqlParser::LogContext *pCtx)
{
  // Add command parameters
  Text* pLogMsg = pCleanString(pCtx->msg);

  bool bEcho = true;
  if (pCtx->SQL_FALSE() || pCtx->OFF()) {
    bEcho = false;
  }

  m_pArgValues->vAppendToken(pLogMsg->szGet(), TN_STRING);
  m_pArgValues->vAppendToken(bEcho ? "true"_Stz : "false"_Stz, TN_BOOLEAN);

  delete pLogMsg;
  return nullptr; // No need to visit any further down the tree
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Add command parameters for an lscol statement                              */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitLscol(Parser::sqlParser::LscolContext *pCtx)
{
  // Add command parameters
  s256 sTableName;
  vCleanString(pCtx->tableName()->identifier(), sTableName);
  m_pArgValues->vAppendToken(sTableName.szStz(), TN_STRING);

  return nullptr; // No need to visit any further down the tree
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Add command parameters for an ldidx statement                              */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitLsidx(Parser::sqlParser::LsidxContext *pCtx)
{
  // Add command parameters
  s256 sIdentifier;
  vCleanString(pCtx->tableName()->identifier(), sIdentifier);
  m_pArgValues->vAppendToken(sIdentifier.szStz(), TN_STRING);

  return nullptr; // No need to visit any further down the tree
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Add command parameters for an ldidx statement                              */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitLsidxcol(Parser::sqlParser::LsidxcolContext *pCtx)
{
  // Add command parameters
  s256 sTableName;
  s256 sIndexName;
  vCleanString(pCtx->tableName()->identifier(), sTableName);
  vCleanString(pCtx->indexName()->identifier(), sIndexName);

  m_pArgValues->vAppendToken(sTableName.szStz(), TN_STRING);
  m_pArgValues->vAppendToken(sIndexName.szStz(), TN_STRING);

  return nullptr; // No need to visit any further down the tree
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Add command parameters for an CREATE TABLE statement                       */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitCreateTableQuery(Parser::sqlParser::CreateTableQueryContext *pCtx)
{
  // The first parameter must be the table name.

  // After that is $etc, which is 1 or more parameters. It represents the rest
  // of the CREATE TABLE statement after the initial "CREATE TABLE <tablename>"
  // prefix. This must be represented as tokens, but not just any old tokens.
  //
  // Indeed it is here that downstream complexity reaches upstream and dirties
  // the waters, for $etc must be represented as tokens that have been tokenised
  // as C++ code and not tokenised as SEN code. So we can't just call
  // AM::TOKENISE::vTokeniseSen(), pass it the string and get back a token vector.
  // Instead we need to emulate parts of the SE Prompt to ensure that the command
  // is tokenised properly.
  //
  // Specifically, doing it this way ensures that characters like open/close
  // parentheses and commas are not considered part of the token that follows them,
  // particularly when they are NOT separated by whitespace.
  // e.g. this string doesn't get parsed correctly for downstream code because there
  // is no whitespace between the parentheses and commas:
  //   (colA INTEGER,colB VARCHAR)
  //
  // The token vector that represents this could be built manually, but then this
  // code will always break whenever the grammar for CREATE TABLE is changed.
  // So instead we will try to reuse code from the SE Prompt to parse this string
  // and produce a token vector.
  //
  // Of course, all this is really unnecessary because the statement has already
  // been parsed. It's really the limitations of the underlying pipeline/operator
  // (pp_create.sen and OP_syntaxTable.sen) but we cannot change that just yet.
  //
  // The text of the create table statement currently gets parsed again as
  // part of OP_syntaxTable.sen, in the class StreamDefBase.
  //
  // PS: The reason why the create table can be parsed as sen code later on is
  // because it has been reconstituted back to a string form where there is
  // whitespace around special characters such as parentheses and commas. This
  // ensures that they are treated as separate tokens when parsed as sen code.

  // First append a token for the table name
  s256 sTableName;
  vCleanString(pCtx->tableName()->identifier(), sTableName);
  m_pArgValues->vAppendToken(sTableName, TN_STRING);

  // Now reconstitute a string for the rest of the statement.
  // We also need to add a ';' to make the parsing of this string work. :-(
  antlr4::misc::Interval interval(pCtx->tableDefinition()->start->getStartIndex(), pCtx->stop->getStopIndex());
  std::string sCreateTableCmd = pCtx->start->getInputStream()->getText(interval);
  sCreateTableCmd.append(";");  // Needed to be tokenised as C++ tokens...

  // Now parse this reconstituted string into a token vector
  Input* pInput = new Input(nullptr);
  pInput->vUseString((Stz*) sCreateTableCmd.c_str());
  eEC eInputErr = EC_NONE;
  pInput->vFormCppTokens();
  pInput->vUseBegin();
  bool bResult = pInput->bReadCommand(eInputErr);
  ASSERT(bResult);

  // Append these tokens and clean up
  m_pArgValues->vAppendVector(pInput->pInpTknVec());
  pInput->vUseEnd();
  delete pInput;

  return nullptr; // No need to visit any further down the tree
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Add command parameters for an CREATE INDEX statement                       */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitCreateIndexQuery(Parser::sqlParser::CreateIndexQueryContext *pCtx)
{
  // See above comments in visitCreateTableQuery regarding why this is done the
  // way it is.

  // First append a token for the table name
  s256 sIndexName;
  vCleanString(pCtx->indexName()->identifier(), sIndexName);
  m_pArgValues->vAppendToken(sIndexName, TN_STRING);

  // Now reconstitute a string for the rest of the statement.
  // We also need to add a ';' to make the parsing of this string work. :-(
  antlr4::misc::Interval interval(pCtx->ON()->getSymbol()->getStartIndex(), pCtx->stop->getStopIndex());
  std::string sCreateIndexCmd = pCtx->start->getInputStream()->getText(interval);
  sCreateIndexCmd.append(";");  // Needed to be tokenised as C++ tokens...

  // Now parse this reconstituted string into a token vector
  Input* pInput = new Input(nullptr);
  pInput->vUseString((Stz*) sCreateIndexCmd.c_str());
  eEC eInputErr = EC_NONE;
  pInput->vFormCppTokens();
  pInput->vUseBegin();
  bool bResult = pInput->bReadCommand(eInputErr);
  ASSERT(bResult);

  // Append these tokens and clean up
  m_pArgValues->vAppendVector(pInput->pInpTknVec());
  pInput->vUseEnd();
  delete pInput;

  // If it is a CREATE UNIQUE INDEX then we need to set a <unique> tag.
  if (pCtx->UNIQUE()) {
    m_pCommonTagSet->vAppend("<unique>"_Stz);
  }

  return nullptr; // No need to visit any further down the tree
}


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Add command parameters for an DROP TABLE statement                         */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitDropTableQuery(Parser::sqlParser::DropTableQueryContext *pCtx)
{
  // Add command parameters
  s256 sTableName;
  vCleanString(pCtx->tableName()->identifier(), sTableName);

  m_pArgValues->vAppendToken(sTableName.szStz(), TN_STRING);

  return nullptr; // No need to visit any further down the tree
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Add command parameters for an DROP INDEX statement                         */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitDropIndexQuery(Parser::sqlParser::DropIndexQueryContext *pCtx)
{
  // Add command parameters
  s256 sTableName;
  s256 sIndexName;

  vCleanString(pCtx->tableName()->identifier(), sTableName);
  vCleanString(pCtx->indexName()->identifier(), sIndexName);

  m_pArgValues->vAppendToken(sTableName.szStz(), TN_STRING);
  m_pArgValues->vAppendToken(sIndexName.szStz(), TN_STRING);

  return nullptr; // No need to visit any further down the tree
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Add command parameters for an TRUNCATE statement                           */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitTruncateQuery(Parser::sqlParser::TruncateQueryContext *pCtx)
{
  // Add command parameters
  s256 sTableName;
  vCleanString(pCtx->tableName()->identifier(), sTableName);

  m_pArgValues->vAppendToken(sTableName.szStz(), TN_STRING);

  return nullptr; // No need to visit any further down the tree
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Add command parameters for an LOAD statement                               */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitLoad(Parser::sqlParser::LoadContext *pCtx)
{
  // Add command parameters
  s256 sTableName;
  s256 sFilePath;

  vCleanString(pCtx->tableName()->identifier(), sTableName);
  vCleanString(pCtx->filePath, sFilePath);

  m_pArgValues->vAppendToken(sTableName.szStz(), TN_STRING);
  m_pArgValues->vAppendToken(sFilePath.szStz(), TN_STRING);

  // We need to visit children here to extract out any load options
  // and add them to the tag vector
  visitChildren(pCtx);

  return nullptr; // No need to visit any further down the tree
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Add command parameters for an DLOAD statement                               */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitDload(Parser::sqlParser::DloadContext *pCtx)
{
  // Add command parameters
  s256 sTableName;
  s256 sFilePath;
  s256 sPathTag;

  vCleanString(pCtx->tableName()->identifier(), sTableName);
  m_pArgValues->vAppendToken(sTableName.szStz(), TN_STRING);

  // There can be one or more file paths. These need to be added as tags
  // to the tagSet instead of as parameters to the command.
  i8 i = 0;
  for (auto pStrLitCtx : pCtx->stringLiteral()) {
    sPathTag.vSprint("<path%lld>"_Stz, i);
    vCleanString(pStrLitCtx, sFilePath);
    m_pCurStmtTagSet->vAppend(sPathTag.szStz(), sFilePath.szStz());
    ++i;
  }

  // Add a tag with the total number of file paths
  m_pCurStmtTagSet->vAppend("<paths>"_Stz, i);

  // We need to visit children here to extract out any load options
  // and add them to the tag vector
  visitChildren(pCtx);

  return nullptr; // No need to visit any further down the tree
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Extract any load options and values and add them as tags                   */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitLoadOption(Parser::sqlParser::LoadOptionContext *pCtx)
{
  s256 sOptionName;
  s256 sOptionValue;

  vCleanTag(pCtx->loadOptionKeys()->start, sOptionName);
  vCleanString(pCtx->loadValue, sOptionValue);

  // load option should be lower case
  sOptionName.vToLower();

  m_pCurStmtTagSet->vAppend(sOptionName.szStz(), sOptionValue.szStz());

  return nullptr; // No need to visit any further down the tree
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Add command parameters for a COUNT statement                               */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitCount(Parser::sqlParser::CountContext *pCtx)
{
  // Add command parameters
  s256 sTableName;
  vCleanString(pCtx->tableName()->identifier(), sTableName);
  m_pArgValues->vAppendToken(sTableName.szStz(), TN_STRING);

  return nullptr; // No need to visit any further down the tree
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Add command parameters for a SZIDX statement                               */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitTrackmemory(Parser::sqlParser::TrackmemoryContext* pCtx)
{
  // Add command parameters
  m_pArgValues->vAppendToken(pCtx->FORCE() ? "true"_Stz : "false"_Stz, TN_BOOLEAN);

  return nullptr; // No need to visit any further down the tree
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Add command parameters for a SLEEP statement                               */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitSleep(Parser::sqlParser::SleepContext *pCtx)
{
  // Add command parameters
  s256 sBuf;

  bool bUnitsAreSec = true;

  if (pCtx->sleepUnits() && pCtx->sleepUnits()->millis) {
    bUnitsAreSec = false;
  }

  i8 iVal = iCleanInt(pCtx->integerLiteral());

  if (bUnitsAreSec) {
    // pipeline expects milliseconds
    iVal *= C_1K;
  }

  sBuf.vSprint("%lld"_Stz, iVal);

  m_pArgValues->vAppendToken(sBuf.szStz(), TN_STRING);

  return nullptr; // No need to visit any further down the tree
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Add command parameters for a SZIDX statement                               */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitSzidx(Parser::sqlParser::SzidxContext *pCtx)
{
  // Add command parameters
  s256 sTableName;
  s256 sIndexName;

  vCleanString(pCtx->tableName()->identifier(), sTableName);
  vCleanString(pCtx->indexName()->identifier(), sIndexName);

  m_pArgValues->vAppendToken(sTableName.szStz(), TN_STRING);
  m_pArgValues->vAppendToken(sIndexName.szStz(), TN_STRING);

  return nullptr; // No need to visit any further down the tree
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Add command parameters for a SZTBL statement                               */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitSztbl(Parser::sqlParser::SztblContext *pCtx)
{
  // Add command parameters
  s256 sTableName;
  vCleanString(pCtx->tableName()->identifier(), sTableName);
  m_pArgValues->vAppendToken(sTableName.szStz(), TN_STRING);

  return nullptr; // No need to visit any further down the tree
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Add command parameters for a BPA statement                                 */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitBpa(Parser::sqlParser::BpaContext *pCtx)
{
  // Add command parameters

  if (pCtx->type && !pCtx->ALL()) {
    Stz* szAnalysisType;
    s256 sBuf;

    vCleanString(pCtx->type, sBuf);
    szAnalysisType = sBuf.szStz();

    m_pArgValues->vAppendToken(szAnalysisType, TN_STRING);

  } else {
    // The default if no bpa parameter is given is to run both bpa sub-commands

    // Create a new parser using the current statement's tag set as the base.
    ASSERT(!m_pChildTasks);

    CommandParser* nestedParser = new CommandParser(m_pCurStmtTagSet, m_pParentTask);

    try {
      Stz* szBpaAllCmd = "bpa regions; bpa objects;"_Stz;
      m_pChildTasks = nestedParser->pParseCommand(szBpaAllCmd);

    } catch (Parser::SyntaxException& e) {
      delete nestedParser;
      throw e;

    } catch (Parser::SemanticException& e) {
      delete nestedParser;
      throw e;
    }

    delete nestedParser;

    ASSERT(m_pChildTasks);
    ASSERT(m_pChildTasks->nTasks() > 0);

    // This ensures that the two nested sub-commands will get picked.
    m_szProcName = nullptr;
  }

  return nullptr; // No need to visit any further down the tree
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Parse a RUN statement                                                      */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any CommandParser::visitFile(Parser::sqlParser::FileContext *pCtx)
{
  s256 sFilePath;
  if (pCtx->identifier()) {
    vCleanString(pCtx->identifier(), sFilePath);
  } else {
    ASSERT(pCtx->stringLiteral());
    vCleanString(pCtx->stringLiteral(), sFilePath);
  }

  // Handle an empty path
  if (sFilePath.bIsEmpty()) {
    Parser::SemanticException::vThrow(EC_INVALID_VALUE,
                                      ": file path cannot be an empty string"_Stz);
  }

  // Validate that the file exists
  if (!File::bExists(sFilePath.szStz(), false)) {
    Parser::SemanticException::vThrow(EC_NO_SUCH_FILE,
                                      ": %s"_Stz,
                                      sFilePath.szStz());
  }

  // Validate that the size of the file is < max length of string
  s48K sFileContents;
  i8 iFileSize = File::nSize(sFilePath.szStz());
  if (iFileSize >= sFileContents.nSpace()) {
    Parser::SemanticException::vThrow(EC_FILE_TOO_LARGE,
                                      ": File size (%lld) is too large for buffer (%lld) for file %s"_Stz,
                                      iFileSize,
                                      sFileContents.nSize(),
                                      sFilePath.szStz());
  }

  // Validate that the file is not empty
  if (iFileSize == 0) {
    Parser::SemanticException::vThrow(EC_FILE_IS_EMPTY,
                                      ": %s"_Stz,
                                      sFilePath.szStz());
  }

  // Read the file contents
  File* pFile = new File(this);
  bool bResult;

  bResult = pFile->bOpenRead(sFilePath.szStz());
  if (!bResult) {
    delete pFile;
    Parser::SemanticException::vThrow(EC_IO_ERROR,
                                      ": Unable to open file: %s"_Stz,
                                      sFilePath.szStz());
  }

  i8 iBytesRead = pFile->nReadFile(sFileContents);
  if (iBytesRead <= 0) {
    pFile->vClose();
    delete pFile;
    Parser::SemanticException::vThrow(EC_IO_ERROR,
                                      ": Read %lld bytes from file: %s"_Stz,
                                      iBytesRead,
                                      sFilePath.szStz());
  }

  pFile->vClose();
  delete pFile;
  pFile = nullptr;

  // Parse the file contents.
  // Create a new parser using the current statement's tag set as the base.
  ASSERT(!m_pChildTasks);
  ASSERT(sFileContents.nLength() > 0);

  CommandParser* nestedParser = new CommandParser(m_pCurStmtTagSet, m_pParentTask);

  try {
    m_pChildTasks = nestedParser->pParseCommand(sFileContents.szStz());

  } catch (Parser::SyntaxException& e) {
    delete nestedParser;
    throw e;

  } catch (Parser::SemanticException& e) {
    delete nestedParser;
    throw e;
  }

  delete nestedParser;

  // Check for no commands. If so, substitute an empty command instead
  ASSERT(m_pChildTasks);
  if (m_pChildTasks->nTasks() == 0) {
    delete m_pChildTasks;
    m_pChildTasks = nullptr;

    // Nulling the m_pChildTasks and setting this procname will insert a noop
    // into the current task list.
    m_szProcName = "noop"_Stz;
  }

  return nullptr; // No need to visit any further down the tree
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates a noop task specifically                                           */
/*----------------------------------------------------------------------------*/
Task* CommandParser::pCreateNoopTask(Stz* szStmt)
{
  ASSERT(m_pCommonTagSet);
  Stz* szProcName = "noop"_Stz;
  sSTMT sStmt;

  TagSet* pCurStmtTagSet = new TagSet();
  pCurStmtTagSet->vCopy(m_pCommonTagSet);

  pCurStmtTagSet->vReplace(C_TAG_JOBNM, szProcName);

  if (!Procedure::bTranslateToPipeline(szProcName, nullptr, szProcName, sStmt.pString())) {
    // All external commands should be resolvable.
    NEVER_REACH
  }

  JobId tParentId;
  tParentId = m_pParentTask->pOuterJobOfTask()->tJobId();

  Task* pTask = Api::pCreateTask(sStmt.szStz(), pCurStmtTagSet, tParentId, szStmt);
  ASSERT(pTask);

  // cleanup
  delete pCurStmtTagSet;

  return pTask;
}

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
/* Clean up a string literal. Returned string is written to the given buffer  */
/*----------------------------------------------------------------------------*/
void CommandParser::vCleanTag(antlr4::Token* pToken, s256& sBuf)
{
  if (!pToken) {
    return;
  }

  std::string sStr = pToken->getText();

  sBuf.vReset();

  // Tags are always specified in angle brackets
  sBuf.vAppend('<');
  sBuf.vConcat((Stz*) sStr.c_str());
  sBuf.vAppend('>');

  sBuf.vToLower();
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
