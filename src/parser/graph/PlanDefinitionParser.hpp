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

    Stack* m_pNodeBuilderStack; // A stack of JSON objects
    Footer m_xFooter;
    
   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    PlanDefinitionParser(void);

    b1 bFindDefinitionFile(s2K& pBuf);
    PlanNode* pParse();

    ~PlanDefinitionParser(void) noexcept;
  
    Footer* aFooter(void);
    
  private:
    virtual antlrcpp::Any visitJson5(json5Parser::Json5Context *ctx)      override;
    virtual antlrcpp::Any visitObj(json5Parser::ObjContext *ctx)          override;
    virtual antlrcpp::Any visitArr(json5Parser::ArrContext *ctx)          override;
    virtual antlrcpp::Any visitPair(json5Parser::PairContext *ctx)        override;

    void vExtractValue(json5Parser::ValueContext* ctx, PlanNodeBuilder* pNodeBuilder);
    void vCleanString(std::string idStr, s256& keyName);
    Text* pCleanString(std::string idStr);
    void vCleanId(std::string idStr, s256& keyName);
  };
  
  PlanDefinitionParser* _pPlanDefinitionParser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
