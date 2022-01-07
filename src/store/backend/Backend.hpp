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
    PlanNode*      m_pWalkNode;
    String*        m_sPipeline;
    bool           m_bHasUnionOp;
    bool           m_bNewStage;
    QueryNode*     m_pPrevStageMarker;
    bool           m_bJoinPredicate;
    bool           m_bIndexJoin;
    s256           m_sIndexJoinTable;
    bool           m_bInSubquery;
    s32            m_sIndent;
    i8             m_iColumnsOfLeftChild;

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
    static void vAppendCallSubQry(Backend* pBackend, QueryNode* pNode);
    static void vAppendExpr(Backend* pBackend, QueryNodeList* pColExprList, QueryNode* pCorrelatedExprs, QueryNode* pNode, Stz* szAliasName);
    static void vAppendExprRelativeToGroupBy(Backend* pBackend, QueryNode* pExprNode, QueryNode* pOrdinalExprNode, Stz* szAliasName);
    static void vAppendFilterOp(Backend* pBackend, QueryNode* pNode);
    static void vAppendIndexJoinOp(Backend* pBackend, QueryNode* pNode);
    static void vAppendLimitOp(Backend* pBackend, QueryNode* pNode);
    static void vAppendLoopJoinOp(Backend* pBackend, QueryNode* pNode);
    static void vAppendOrderByOp(Backend* pBackend,  QueryNode* pNode);
    static void vAppendProjectOp(Backend* pBackend,  QueryNode* pNode);
    static void vAppendScanOp(Backend* pBackend,  QueryNode* pNode);
    static void vAppendScanRightOp(Backend* pBackend,  QueryNode* pNode);
    static void vAppendUnionOp(Backend* pBackend,  QueryNode* pNode);
    static Stz* szGetRouting(QueryNode* pStageMarker);
    static void vPrintPlanGraph(Parser::PlanNode* pPlanRoot);

    //
    // visitor functions
    //
    static Parser::GraphNav::eWalkCtrl vVisitorExpr(PlanNode* pNode,
                                                    s2K& sPath, i8 iDepth,
                                                    GraphNav::eWalkEvent eEventType, void* pvArg);
    static Parser::GraphNav::eWalkCtrl vVisitorPlan(PlanNode* pNode,
                                                    s2K& sPath, i8 iDepth, void* pvArg);
    static Parser::GraphNav::eWalkCtrl vVisitorStage(PlanNode* pNode,
                                                    s2K& sPath, i8 iDepth, void* pvArg);
    static Parser::GraphNav::eWalkCtrl vVisitorStatement(PlanNode* pNode,
                                                    s2K& sPath, i8 iDepth, void* pvArg);

    //
    // pipeline stages
    //
    void vAppendLastStage(Stz* szLocation);
    void vAppendNewStage(Stz* szStage, Stz* szLocation);
    void vAppendNextOp(Stz* szStage, Stz* szRouting = nullptr);
    void vAppendPipeline(Stz* szPipeline);
    void vAppendStartStage(void);

    //
    // helper functions
    //
    bool bInTableSubquery(QueryNode* pNode);

    Footer* aFooter(void);
  };

  Backend* _pBackend(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
