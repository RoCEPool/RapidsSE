/*============================================================================*/
/* SqlPlanner.hpp - Parses SQL statements & produces an initial knowledgebase */
/*============================================================================*/
#pragma once

#include "sqlBaseVisitor.h"
#include "QueryNodeStack.hpp"
#include "QueryNodeList.hpp"
#include "SqlException.hpp"
#include "eJT.h"

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

namespace Parser
{
  class SqlPlanner : public Object, public sqlBaseVisitor
  {
  private:
    // The query text that was entered by the user
    s48K m_sQueryStr;
    QueryNodeStack* m_pStatementStack;
    QueryNodeStack* m_pTableExprStack;  // Could be DataSources, could be Joins, Subqueries, etc
    QueryNodeList* m_pTableSubqueryList;
    QueryNodeList* m_pExprStack;
    QueryNodeList* m_pSelectItems;
    QueryNodeList* m_pOrderItems;
    Footer m_xFooter;
    
   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    SqlPlanner(void);
    
    PlanNode* plan(Stz* pStr, const PlanNode* pTemplate);

    ~SqlPlanner(void) noexcept;
  
    Footer* aFooter(void);
    
    antlrcpp::Any visitStatement(sqlParser::StatementContext *pCtx)               override;
    antlrcpp::Any visitStatementChoice(sqlParser::StatementChoiceContext *pCtx)   override;
    antlrcpp::Any visitExplainStatement(sqlParser::ExplainStatementContext *pCtx) override;
    antlrcpp::Any visitExplainFormat(sqlParser::ExplainFormatContext *pCtx)       override;
    antlrcpp::Any visitExplainStageList(sqlParser::ExplainStageListContext *pCtx) override;
    antlrcpp::Any visitExplainRange(sqlParser::ExplainRangeContext *pCtx)         override;
    antlrcpp::Any visitExplainRangeChoice(sqlParser::ExplainRangeChoiceContext *pCtx) override;

    antlrcpp::Any visitQuery(sqlParser::QueryContext *pCtx)                       override;
    antlrcpp::Any visitSelectQuery(sqlParser::SelectQueryContext *pCtx)           override;
    antlrcpp::Any visitWithQuery(sqlParser::WithQueryContext *pCtx)               override;
    antlrcpp::Any visitSetQuery(sqlParser::SetQueryContext *pCtx)                 override;
    antlrcpp::Any visitSelectExpr(sqlParser::SelectExprContext *pCtx)             override;
    antlrcpp::Any visitSetExpr(sqlParser::SetExprContext *pCtx)                   override;
    antlrcpp::Any visitFetch(sqlParser::FetchContext *pCtx)                       override;
    antlrcpp::Any visitJoinOn(sqlParser::JoinOnContext *pCtx)                     override;
    antlrcpp::Any visitJoinUsing(sqlParser::JoinUsingContext *pCtx)               override;
    antlrcpp::Any visitCrossJoin(sqlParser::CrossJoinContext *pCtx)               override;
    antlrcpp::Any visitNaturalJoin(sqlParser::NaturalJoinContext *pCtx)           override;
    antlrcpp::Any visitTableReference(sqlParser::TableReferenceContext *pCtx)     override;
    antlrcpp::Any visitSubquery(sqlParser::SubqueryContext *pCtx)                 override;
    antlrcpp::Any visitSelectList(sqlParser::SelectListContext *pCtx)             override;
    antlrcpp::Any visitSelectListItem(sqlParser::SelectListItemContext* pCtx)     override;
    antlrcpp::Any visitColumnRef(sqlParser::ColumnRefContext* pCtx)               override;
    antlrcpp::Any visitColumnWildcard(sqlParser::ColumnWildcardContext* pCtx)     override;
    antlrcpp::Any visitLiteral(sqlParser::LiteralContext* pCtx)                   override;
    antlrcpp::Any visitUnaryOp(sqlParser::UnaryOpContext *pCtx)                   override;
    antlrcpp::Any visitBinaryOp(sqlParser::BinaryOpContext *pCtx)                 override;
    antlrcpp::Any visitLogicOp(sqlParser::LogicOpContext *pCtx)                   override;
    antlrcpp::Any visitIsNullOp(sqlParser::IsNullOpContext *pCtx)                 override;
    antlrcpp::Any visitCountAll(sqlParser::CountAllContext *pCtx)                 override;
    antlrcpp::Any visitAggFunc(sqlParser::AggFuncContext *pCtx)                   override;
    antlrcpp::Any visitOrderByList(sqlParser::OrderByListContext *pCtx)           override;
    antlrcpp::Any visitOrderByListItem(sqlParser::OrderByListItemContext *pCtx)   override;
    antlrcpp::Any visitGroupByListItem(sqlParser::GroupByListItemContext *pCtx)   override;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // What follows are grammar clauses that are not yet implemented.
    // For usability purposes we will return a syntax error indicating such.

    // Statement types not implemented
    antlrcpp::Any visitStatisticsStatement(sqlParser::StatisticsStatementContext *pCtx) override { __NOT_IMPL("STATISTICS")}
    antlrcpp::Any visitQueryHint(sqlParser::QueryHintContext *pCtx)                     override { __NOT_IMPL("query hints")}

    // Query types not implemented
    antlrcpp::Any visitInsertQuery(sqlParser::InsertQueryContext *pCtx)            override { __NOT_IMPL("INSERT")}
    antlrcpp::Any visitDeleteQuery(sqlParser::DeleteQueryContext *pCtx)            override { __NOT_IMPL("DELETE")}
    antlrcpp::Any visitUpdateQuery(sqlParser::UpdateQueryContext *pCtx)            override { __NOT_IMPL("UPDATE")}
    antlrcpp::Any visitCreateQuery(sqlParser::CreateQueryContext *pCtx)            override { __NOT_IMPL("CREATE")}
    antlrcpp::Any visitDropTable(sqlParser::DropTableContext *pCtx)                override { __NOT_IMPL("DROP TABLE")}
    antlrcpp::Any visitDropIndex(sqlParser::DropIndexContext *pCtx)                override { __NOT_IMPL("DROP INDEX")}
    antlrcpp::Any visitTruncateQuery(sqlParser::TruncateQueryContext *pCtx)        override { __NOT_IMPL("TRUNCATE")}

    // SelectQuery clauses not implemented
    // distinct is handled in visitSelectExpr

    // tableExprs not implemented
    antlrcpp::Any visitValuesSubquery(sqlParser::ValuesSubqueryContext *pCtx)      override { __NOT_IMPL("VALUES")}

    // expressions not implemented
    antlrcpp::Any visitInlineCast(sqlParser::InlineCastContext *pCtx)              override { __NOT_IMPL("cast")}
    antlrcpp::Any visitMethodCall(sqlParser::MethodCallContext *pCtx)              override { __NOT_IMPL("method call")}
    antlrcpp::Any visitCastOp(sqlParser::CastOpContext *pCtx)                      override { __NOT_IMPL("CAST")}
    antlrcpp::Any visitTypeOfOp(sqlParser::TypeOfOpContext *pCtx)                  override { __NOT_IMPL("TYPEOF")}
    antlrcpp::Any visitCaseOp(sqlParser::CaseOpContext *pCtx)                      override { __NOT_IMPL("CASE")}
    antlrcpp::Any visitIfOp(sqlParser::IfOpContext *pCtx)                          override { __NOT_IMPL("IF")}
    antlrcpp::Any visitIfNullOp(sqlParser::IfNullOpContext *pCtx)                  override { __NOT_IMPL("IFNULL")}
    antlrcpp::Any visitNullIfOp(sqlParser::NullIfOpContext *pCtx)                  override { __NOT_IMPL("NULLIF")}
    antlrcpp::Any visitCoalesceOp(sqlParser::CoalesceOpContext *pCtx)              override { __NOT_IMPL("COALESCE")}
    antlrcpp::Any visitDecodeOp(sqlParser::DecodeOpContext *pCtx)                  override { __NOT_IMPL("DECODE")}
    antlrcpp::Any visitConcatOp(sqlParser::ConcatOpContext *pCtx)                  override { __NOT_IMPL("CONCAT")}
    antlrcpp::Any visitOverlayFunc(sqlParser::OverlayFuncContext *pCtx)            override { __NOT_IMPL("OVERLAY")}
    antlrcpp::Any visitPositionFunc(sqlParser::PositionFuncContext *pCtx)          override { __NOT_IMPL("POSITION")}
    antlrcpp::Any visitSubStringFunc(sqlParser::SubStringFuncContext *pCtx)        override { __NOT_IMPL("SUBSTR")}
    antlrcpp::Any visitTrimFunc(sqlParser::TrimFuncContext *pCtx)                  override { __NOT_IMPL("TRIM")}
    antlrcpp::Any visitExtractFunc(sqlParser::ExtractFuncContext *pCtx)            override { __NOT_IMPL("EXTRACT")}
    antlrcpp::Any visitScalarFunc(sqlParser::ScalarFuncContext *pCtx)              override { __NOT_IMPL("scalar function")}
    antlrcpp::Any visitSubqueryExpr(sqlParser::SubqueryExprContext *pCtx)          override { __NOT_IMPL("subquery expression")}
    antlrcpp::Any visitLikeOp(sqlParser::LikeOpContext *pCtx)                      override { __NOT_IMPL("LIKE")}
    antlrcpp::Any visitLikeEscapeOp(sqlParser::LikeEscapeOpContext *pCtx)          override { __NOT_IMPL("LIKE")}
    antlrcpp::Any visitSubqueryExistsOp(sqlParser::SubqueryExistsOpContext *pCtx)  override { __NOT_IMPL("EXISTS")}
    antlrcpp::Any visitSubqueryInOp(sqlParser::SubqueryInOpContext *pCtx)          override { __NOT_IMPL("IN subquery")}
    antlrcpp::Any visitInOp(sqlParser::InOpContext *pCtx)                          override { __NOT_IMPL("IN")}

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
    eJT   eExtractJoinType(sqlParser::JoinTypeContext* pCtx);
    eJT   eExtractJoinType(sqlParser::ExtendedJoinTypeContext* pCtx);

  };
  
  SqlPlanner* _pSqlPlanner(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
