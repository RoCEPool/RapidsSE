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

  
    Footer m_xFooter;
    
   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    SqlPlanner(void);
    
    void plan(Stz* pStr);

    ~SqlPlanner(void);
  
    Footer* aFooter(void);
    
    virtual antlrcpp::Any visitStatement(sqlParser::StatementContext *ctx)      override;
    virtual antlrcpp::Any visitQuery(sqlParser::QueryContext *ctx)              override;
    virtual antlrcpp::Any visitSelectQuery(sqlParser::SelectQueryContext *ctx)  override;
    virtual antlrcpp::Any visitSelectExpr(sqlParser::SelectExprContext *ctx)    override;
    virtual antlrcpp::Any visitFetch(sqlParser::FetchContext *ctx)              override;
    virtual antlrcpp::Any visitSelectList(sqlParser::SelectListContext *ctx)    override;
  };
  
  SqlPlanner* _pSqlPlanner(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
