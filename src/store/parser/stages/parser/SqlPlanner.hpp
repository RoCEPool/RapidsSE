/*============================================================================*/
/* SqlPlanner.hpp - Parses SQL statements & produces an initial knowledgebase */
/*============================================================================*/
#pragma once

#include "sqlBaseVisitor.h"
#include "QueryNodeStack.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class PlanNode;

  class SqlPlanner : public Object, public sqlBaseVisitor
  {
  private:
    // The query text that was entered by the user
    s48K m_sQueryStr;
    QueryNodeStack* m_pStatementStack;
    QueryNodeStack* m_pTableRefStack;
    QueryNodeStack* m_pExprStack;
    QueryNodeList* m_pSelectItems;
    Footer m_xFooter;
    
   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    SqlPlanner(void);
    
    PlanNode* plan(Stz* pStr, const PlanNode* pTemplate);

    ~SqlPlanner(void) noexcept;
  
    Footer* aFooter(void);
    
    virtual antlrcpp::Any visitStatement(sqlParser::StatementContext *pCtx)           override;
    virtual antlrcpp::Any visitQuery(sqlParser::QueryContext *pCtx)                   override;
    virtual antlrcpp::Any visitSelectQuery(sqlParser::SelectQueryContext *pCtx)       override;
    virtual antlrcpp::Any visitSelectExpr(sqlParser::SelectExprContext *pCtx)         override;
    virtual antlrcpp::Any visitFetch(sqlParser::FetchContext *pCtx)                   override;
    virtual antlrcpp::Any visitTableReference(sqlParser::TableReferenceContext *pCtx) override;
    virtual antlrcpp::Any visitSelectList(sqlParser::SelectListContext *pCtx)         override;
    virtual antlrcpp::Any visitSelectListItem(sqlParser::SelectListItemContext* pCtx) override;
    virtual antlrcpp::Any visitColumnRef(sqlParser::ColumnRefContext* pCtx)           override;
    virtual antlrcpp::Any visitLiteral(sqlParser::LiteralContext* pCtx)               override;
    virtual antlrcpp::Any visitUnaryOp(sqlParser::UnaryOpContext *pCtx)               override;
    virtual antlrcpp::Any visitBinaryOp(sqlParser::BinaryOpContext *pCtx)             override;
    virtual antlrcpp::Any visitLogicOp(sqlParser::LogicOpContext *pCtx)               override;
    virtual antlrcpp::Any visitIsNullOp(sqlParser::IsNullOpContext *pCtx)             override;
    virtual antlrcpp::Any visitCountAll(sqlParser::CountAllContext *pCtx)             override;
    virtual antlrcpp::Any visitAggFunc(sqlParser::AggFuncContext *pCtx)               override;

    
  private:
    Text* pCleanId(sqlParser::IdentifierContext* pCtx);
    Text* pCleanString(sqlParser::StringLiteralContext* pCtx);
    void  vCleanString(sqlParser::StringLiteralContext* pCtx, s256& sBuf);
    void  vCleanString(sqlParser::IdentifierContext* pCtx, s256& sBuf);
    void  vCleanString(antlr4::Token* pToken, s256& sBuf);
    void  vCleanString(std::string& sStr, s256& sBuf);
    i8    iCleanInt(sqlParser::IntegerLiteralContext* pCtx);
    i8    iCleanInt(antlr4::Token* pToken);
    i8    iCleanInt(std::string& sStr);
    d8    dCleanFloat(sqlParser::FloatLiteralContext* pCtx);
//  ??    cleanDecimal(sqlParser::DecimalLiteralContext* pCtx);
    Dte   xCleanDate(sqlParser::StringLiteralContext* pCtx);
    Tme   xCleanTime(sqlParser::StringLiteralContext* pCtx);
    Tsp   xCleanTimestamp(sqlParser::StringLiteralContext* pCtx);
//  Ivl   xCleanInterval(sqlParser::StringLiteralContext* pCtx);

  };
  
  SqlPlanner* _pSqlPlanner(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
