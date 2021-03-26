/*============================================================================*/
/* Backend.hpp -- Backend (plan to pipeline plan transliterate)               */
/*============================================================================*/
#pragma once

#include "RootNode.hpp"
#include "GraphNav.hpp"
#include "QueryNode.hpp"

using namespace Parser;

namespace STORE
{
  class Backend : public Object
  {
  private:

    PlanNode* m_pPlanRoot;
    String*   m_sPipeline;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Backend(RootNode* pPlanRoot);
   ~Backend(void) noexcept(false);

    bool bTransliterate(String* sPipeline);

  private:

    //
    // plan transliteration
    //
    static void vAppendAggregateOp(String* sPipeline, QueryNode* pNode);
    static void vAppendExpr(String* sPipeline, QueryNode* pNode);
    static void vAppendFilterOp( String* sPipeline, QueryNode* pNode);
    static void vAppendProjectOp( String* sPipeline, QueryNode* pNode);
    static void vAppendScanOp(String* sPipeline, QueryNode* pNode);

    static void vPrintPlanGraph(Parser::PlanNode* pPlanRoot);

    static Parser::GraphNav::eWalkCtrl vVisitorExpr(PlanNode* pNode,
                                                    s256& sPath, i8 iDepth, void* pvArg);
    static Parser::GraphNav::eWalkCtrl vVisitorPlan(PlanNode* pNode,
                                                    s256& sPath, i8 iDepth, void* pvArg);

    Footer* aFooter(void);
  };

  Backend* _pBackend(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
