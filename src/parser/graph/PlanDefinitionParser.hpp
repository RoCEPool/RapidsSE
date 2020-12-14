/*============================================================================*/
/* PlanDefinitionParser.hpp - Parses SQL statements & produces an initial knowledgebase */
/*============================================================================*/
#pragma once

#include "json5BaseVisitor.h"

namespace Parser
{  
  class PlanDefinitionParser : public Object, public json5BaseVisitor
  {
  private:
    // The paths that the planner definition json5 file could be found
    static Stz* definitionPaths[];

    Footer m_xFooter;
    
   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    PlanDefinitionParser(void);

    b1 findDefinitionFile(s2K& pBuf);
    PlanNode* parse();

    ~PlanDefinitionParser(void) noexcept;
  
    Footer* aFooter(void);
    
//    virtual antlrcpp::Any visitStatement(sqlParser::StatementContext *ctx)      override;
//    virtual antlrcpp::Any visitQuery(sqlParser::QueryContext *ctx)              override;
//    virtual antlrcpp::Any visitSelectQuery(sqlParser::SelectQueryContext *ctx)  override;
//    virtual antlrcpp::Any visitSelectExpr(sqlParser::SelectExprContext *ctx)    override;
//    virtual antlrcpp::Any visitFetch(sqlParser::FetchContext *ctx)              override;
//    virtual antlrcpp::Any visitTableReference(sqlParser::TableReferenceContext *ctx) override;
//    virtual antlrcpp::Any visitSelectList(sqlParser::SelectListContext *ctx)    override;
//    virtual antlrcpp::Any visitSelectListItem(sqlParser::SelectListItemContext* ctx) override;
//    virtual antlrcpp::Any visitColumnRef(sqlParser::ColumnRefContext* ctx) override;
    
    
  private:
//    Text* cleanId(json5Parser::IdentifierContext *ctx);
  };
  
  PlanDefinitionParser* _pPlanDefinitionParser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
