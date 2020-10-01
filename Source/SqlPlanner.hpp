/*============================================================================*/
/* SqlPlanner.hpp -                                                           */
/*============================================================================*/
#pragma once

#include "sqlBaseVisitor.h"

namespace Parser
{  
  class SqlPlanner : public Object, public sqlBaseVisitor
  {
  private:
  
    Stack* m_pTableRefStack;
    Stack* m_pExprStack;
    ExprList* m_pSelectList;
    Footer m_xFooter;
    
   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    SqlPlanner(void);
    
    void plan(Stz* pStr);

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
