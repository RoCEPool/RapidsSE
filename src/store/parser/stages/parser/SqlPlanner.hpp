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
    
    virtual antlrcpp::Any visitStatement(sqlParser::StatementContext *ctx)      override;
    virtual antlrcpp::Any visitQuery(sqlParser::QueryContext *ctx)              override;
    virtual antlrcpp::Any visitSelectQuery(sqlParser::SelectQueryContext *ctx)  override;
    virtual antlrcpp::Any visitSelectExpr(sqlParser::SelectExprContext *ctx)    override;
    virtual antlrcpp::Any visitFetch(sqlParser::FetchContext *ctx)              override;
    virtual antlrcpp::Any visitTableReference(sqlParser::TableReferenceContext *ctx) override;
    virtual antlrcpp::Any visitSelectList(sqlParser::SelectListContext *ctx)    override;
    virtual antlrcpp::Any visitSelectListItem(sqlParser::SelectListItemContext* ctx) override;
    virtual antlrcpp::Any visitColumnRef(sqlParser::ColumnRefContext* ctx) override;
    
    
  private:
    Text* cleanId(sqlParser::IdentifierContext *ctx);
  };
  
  SqlPlanner* _pSqlPlanner(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
