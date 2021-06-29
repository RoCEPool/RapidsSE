/*============================================================================*/
/* Dynamo.hpp -- Dynamo (pipeline plan compiler)                              */
/*============================================================================*/
#pragma once

#include "RootNode.hpp"
#include "GraphNav.hpp"
#include "QueryNode.hpp"

using namespace Parser;

namespace STORE
{
  class Dynamo : public Object
  {
  private:
    Job*          m_p_PipelineJob;  // job this dynamo is associated with
    
    TokenVec*     m_pTknVec;        // tokenised for of the pipeline statement
    i8            m_i_Tkn;          // current token
    Token*        m_pTkn;           // current token

    PipelineSpec* m_p_PipelineSpec; // pipeline spec being formed
    StageSpec*    m_p_StageSpec;    // current stage spec being formed
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Dynamo(Stz* szPipelineStmt, Job* p_PipelineJob);
   ~Dynamo(void) noexcept(false);

    static bool bToPipelineStmt(String* pAsPipelineStmt, TokenVec* pFromTokenVec);
    static ePX eCrossTopo(StageSpec* pFromStageSpec, StageSpec* pToStageSpec);

    bool bCompile(void);
    PipelineSpec* pPipelineSpec(void);
    void vShow(void);

  private:

    //
    // Phase 1 : Collect
    //
    bool bPhase1_collect(void);
  
    bool bCollectBegin(void);
    bool bCollectEnd(void);
    bool bCollectName(void);
    bool bCollectOp(void);
    bool bCollectOpArgs(OpSpec* pOpSpec);
    bool bCollectOpSequence(void);
    bool bCollectStage(void);
    bool bCollectTag(void);
    bool bCollectTopology(void);
  
    bool bRecogniseOnAll(void);
    bool bRecogniseOnAllNodes(void);
    bool bRecogniseOnAllPartitions(void);
    bool bRecogniseOnNode(void);
    bool bRecogniseOnOrigin(void);
    bool bRecogniseOnPartition(void);

    void vTokenAccept(void);
    void vTokenBegin(i8 i_Tkn);
    bool bTokenIsOp(Token* pTkn);


    //
    // Phase 2 : Staging
    //
    bool bPhase2_staging(void);

    void vArgAdd_sends(OpSpec* pConsumeOpSpec, ePX eCrossTopo);
    void vArgUpdate_sends(OpSpec* pConsumeOpSpec, ePX eCrossTopo);

    void vMorphToSend(OpSpec* pOpSpec, ePX eCrossTopo, i8 i_NextStage);

    void vNameIsOp(StageSpec* pStageSpec, OpSpec* pOpSpec);
    void vNameIsPipeline(void);
    void vNameIsStage(StageSpec* pStageSpec, OpSpec* pOpSpec, i8 i_Op,
                      StageSpec* pNextStageSpec, i8 i_NextStage);
  
    void vStageConsumeOpInsert(StageSpec* pStageSpec, ePX eCrossTopo);
    void vStageFirstSend(StageSpec* pStageSpec, OpSpec* pOpSpec, i8 i_Op,
                         StageSpec* pNextStageSpec, i8 i_NextStage);
    void vStageSubsequentSend(StageSpec* pStageSpec, OpSpec* pOpSpec, i8 i_Op,
                              StageSpec* pNextStageSpec, i8 i_NextStage,
                              OpSpec* pOpSpecSend);

    void vTopoTagsAdd(OpSpec* pOpSpec,
                      ePX eCrossTopo, i8 i_NextStage, i8 i_NextOp);
    void vTopoTagsUpdate(OpSpec* pOpSpec,
                         ePX eCrossTopo, i8 i_NextStage, i8 i_NextOp);

    Footer* aFooter(void);
  };

  Dynamo* _pDynamo(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
