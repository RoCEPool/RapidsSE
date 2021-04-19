/*============================================================================*/
/* Backend.hpp -- Backend (plan to pipeline plan transliterate)               */
/*============================================================================*/
#pragma once

#include "RootNode.hpp"
#include "GraphNav.hpp"
#include "QueryNode.hpp"
#include "QueryNodeList.hpp"

using namespace Parser;

namespace STORE
{
  class Backend : public Object
  {
  private:

    PlanNode*      m_pPlanRoot;
    QueryNodeList* m_pColExprList;
    String*        m_sPipeline;

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
    static void vAppendAggregateOp(Backend* pBackend, QueryNode* pNode);
    static void vAppendExpr(Backend* pBackend, QueryNode* pNode, Stz* szAliasName);
    static void vAppendFilterOp(Backend* pBackend, QueryNode* pNode);
    static void vAppendLimitOp(Backend* pBackend, QueryNode* pNode);
    static void vAppendProjectOp(Backend* pBackend,  QueryNode* pNode);
    static void vAppendScanOp(Backend* pBackend,  QueryNode* pNode);
    static void vAppendUnionOp(Backend* pBackend,  QueryNode* pNode);

    static void vPrintPlanGraph(Parser::PlanNode* pPlanRoot);

    //
    // visitor functions
    //
    static Parser::GraphNav::eWalkCtrl vVisitorExpr(PlanNode* pNode,
                                                    s256& sPath, i8 iDepth, void* pvArg);
    static Parser::GraphNav::eWalkCtrl vVisitorPlan(PlanNode* pNode,
                                                    s256& sPath, i8 iDepth, void* pvArg);
    static Parser::GraphNav::eWalkCtrl vVisitorStatement(PlanNode* pNode,
                                                    s256& sPath, i8 iDepth, void* pvArg);

    //
    // pipeline stages
    //
    void vAppendLastStage(void);
    void vAppendNextStage(Stz* szStage);
    void vAppendNextOp(Stz* szStage);
    void vAppendPipe(void);
    void vAppendStartStage(void);

    Footer* aFooter(void);
  };

  Backend* _pBackend(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
