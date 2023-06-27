/*============================================================================*/
/* CommandParser.cpp - Parses all external SE commands & produces tasks       */
/*============================================================================*/
#pragma once

#include "sqlBaseVisitor.h"
#include "SqlException.hpp"
//#include "eJT.h"
//#include "TypeId.hpp"

#define __NOT_IMPL(FEATURE) \
  SyntaxException::vThrowNotImplemented(pCtx->start->getLine(), pCtx->start->getCharPositionInLine(), FEATURE##_Stz ); \
  return nullptr; \
EndDefine

#define __NOT_IMPL2(FEATURE) \
  SyntaxException::vThrowNotImplemented(pCtx->start->getLine(), pCtx->start->getCharPositionInLine(), FEATURE ); \
  return nullptr; \
EndDefine

#define __NOT_IMPL3(FEATURE) \
SyntaxException::vThrowNotImplemented(pCtx->start->getLine(), pCtx->start->getCharPositionInLine(), FEATURE##_Stz ); \
EndDefine

namespace STORE
{
class CommandParser : public Object, public Parser::sqlBaseVisitor
  {
  private:
    // The query text that was entered by the user. May contain more than 1 statement.
    s48K m_sQueryStr;

    TagSet*   m_pCommonTagSet;
    Task*     m_pParentTask;
    TaskList* m_pTaskList;

    TagSet*   m_pCurStmtTagSet;
    s48K      m_sCurStmtStr;
    Stz*      m_szProcName;
    TokenVec* m_pArgValues;

    Footer m_xFooter;

   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    CommandParser(TagSet* pCommonTagSet, Task* pParentTask);
    ~CommandParser(void) noexcept;

    TaskList* pParseCommand(Stz* szCommand);


    Footer* aFooter(void);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Visited grammar rules:
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    antlrcpp::Any visitStatements(Parser::sqlParser::StatementsContext *pCtx)             override;
    antlrcpp::Any visitStatement(Parser::sqlParser::StatementContext *pCtx)               override;
    antlrcpp::Any visitStatementChoice(Parser::sqlParser::StatementChoiceContext *pCtx)   override;
    antlrcpp::Any visitKeyOnlyTag(Parser::sqlParser::KeyOnlyTagContext *pCtx)             override;
    antlrcpp::Any visitKeyValueTag(Parser::sqlParser::KeyValueTagContext *pCtx)           override;

    antlrcpp::Any visitQueryStatement(Parser::sqlParser::QueryStatementContext *pCtx)     override;
    antlrcpp::Any visitExplainStatement(Parser::sqlParser::ExplainStatementContext *pCtx) override;
    antlrcpp::Any visitLscol(Parser::sqlParser::LscolContext *pCtx)                       override;
    antlrcpp::Any visitLsidx(Parser::sqlParser::LsidxContext *pCtx)                       override;
    antlrcpp::Any visitLsidxcol(Parser::sqlParser::LsidxcolContext *pCtx)                 override;
    antlrcpp::Any visitCreateTableQuery(Parser::sqlParser::CreateTableQueryContext *pCtx) override;
    antlrcpp::Any visitCreateIndexQuery(Parser::sqlParser::CreateIndexQueryContext *pCtx) override;
    antlrcpp::Any visitDropTableQuery(Parser::sqlParser::DropTableQueryContext *pCtx)     override;
    antlrcpp::Any visitDropIndexQuery(Parser::sqlParser::DropIndexQueryContext *pCtx)     override;
    antlrcpp::Any visitCount(Parser::sqlParser::CountContext *pCtx)                       override;
    antlrcpp::Any visitSzidx(Parser::sqlParser::SzidxContext *pCtx)                       override;
    antlrcpp::Any visitSztbl(Parser::sqlParser::SztblContext *pCtx)                       override;


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  private:
    static Text* pCleanId(Parser::sqlParser::IdentifierContext* pCtx);
    static Text* pCleanString(Parser::sqlParser::StringLiteralContext* pCtx);
    static Text* pCleanString(antlr4::Token* pToken);
    static void  vCleanString(Parser::sqlParser::StringLiteralContext* pCtx, s256& sBuf);
    static void  vCleanString(Parser::sqlParser::IdentifierContext* pCtx, s256& sBuf);
    static void  vCleanString(antlr4::Token* pToken, s256& sBuf);
    static void  vCleanString(std::string& sStr, s256& sBuf);
    static i8    iCleanInt(Parser::sqlParser::SignedIntegerLiteralContext* pCtx);
    static i8    iCleanInt(Parser::sqlParser::IntegerLiteralContext* pCtx);
    static i8    iCleanInt(antlr4::Token* pToken);
    static i8    iCleanInt(std::string& sStr);
    static u8    uCleanInt(Parser::sqlParser::SignedIntegerLiteralContext* pCtx);
    static u8    uCleanInt(Parser::sqlParser::IntegerLiteralContext* pCtx);
    static u8    uCleanInt(antlr4::Token* pToken);
    static u8    uCleanInt(std::string& sStr);

  };
  
  CommandParser* _pCommandParser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
