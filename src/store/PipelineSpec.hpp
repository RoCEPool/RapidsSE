/*============================================================================*/
/* PipelineSpec.hpp -- pipeline Specification                                 */
/*============================================================================*/
#pragma once

#include "LifeGuard_t.hpp"


namespace STORE { class Dynamo; class StageSpec; class OpSpec; }

namespace STORE
{
  class PipelineSpec
          : public ListNL_t<StageSpec>
          , public LifeGuard_t<PipelineSpec>
          , public ListItem_t<PipelineSpec>
  {
    friend class STORE::Dynamo;

  private:
    s32  m_sPipelineNm;    // pipeline name

    PipelineSpec* m_p_ParentPipelineSpec;
  
    // optional set of nested pipelines
    VectorNL_t<CN_NESTEDPIPELINESPEC,PipelineSpec,C_MAX_NESTED_PIPELINES>
      m_oNestedPipelineSpecs;
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    TagSet*  m_p_JobTags; // job tag set
    Scanner* m_p_Scanner; // statement token scanner

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    PipelineSpec(Stz*          szPipelineNm,
                 PipelineSpec* p_ParentPipelineSpec,
                 TagSet*       p_JobTags,
                 Scanner*      p_Scanner);
   ~PipelineSpec(void) noexcept(false);
  
    static bool bCompile(PipelineSpec* p_ParentPipelineSpec,
                         TagSet*       p_JobTags,
                         Scanner*      p_Scanner,
                         PipelineSpec*& rp_PipelineSpec);
  
    void vAddStageSpec(StageSpec* pStageSpec);
    i8   iDepth(void);
    StageSpec* pFindStageSpec(Stz* szFindNm);
    PipelineSpec* pFindNestedPipelineSpec(Stz* szFindNm);
    i8   nNestedPipelineSpecs(void);
    PipelineSpec* pNestedPipelineSpec(i8 i_NestedPipeline);
    PipelineSpec* pParentPipelineSpec(void);
    Stz* szPipelineNm(void);
    StageSpec* pStageSpec(i8 i_Stage);

    Footer* aFooter(void);
  
  
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //                            PIPELINE COMPILER
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  private:
  
    //
    // Pass 1 : Collect
    //
    bool bPass1_collect(void);

    bool bCollectTag(void);
    bool bCollectBeginEnd(void);
    bool bCollectStage(void);
    bool bCollectTopology(StageSpec* p_StageSpec);
    bool bCollectOpSequence(StageSpec* p_StageSpec);
    bool bCollectOp(StageSpec* p_StageSpec);
    bool bCollectOpArgs(StageSpec* p_StageSpec, OpSpec* pOpSpec);
    bool bCollectPipeline(void);

    bool bRecogniseOnAll(StageSpec* p_StageSpec);
    bool bRecogniseOnAllNodes(StageSpec* p_StageSpec);
    bool bRecogniseOnAllPartitions(StageSpec* p_StageSpec);
    bool bRecogniseOnNode(StageSpec* p_StageSpec);
    bool bRecogniseOnOrigin(StageSpec* p_StageSpec);
    bool bRecogniseOnPartition(StageSpec* p_StageSpec);

    bool bTokenMaybeOp(Token* pTkn);
    bool bTokenMaybeOpArg(Token* pTkn);


    //
    // Pass 2 : Staging
    //
    bool bPass2_staging(void);

    void vArgAdd_sends(OpSpec* pConsumeOpSpec, ePX eCrossTopo);
    void vArgUpdate_sends(OpSpec* pConsumeOpSpec, ePX eCrossTopo);

    void vMorphToCall(OpSpec* pOpSpec, PipelineSpec* pPipelineSpecCalled);
    void vMorphToSend(OpSpec* pOpSpec, ePX eCrossTopo, i8 i_NextStage);

    void vNameIsOp(StageSpec* pStageSpec, OpSpec* pOpSpec);
    void vNameIsPipeline(StageSpec*    pStageSpec,
                         OpSpec*       pOpSpec,
                         PipelineSpec* pPipelineSpecCalled);
    void vNameIsStage(StageSpec* pStageSpec,
                      OpSpec*    pOpSpec,
                      StageSpec* pNextStageSpec);

    void vStageConsumeOpInsert(StageSpec* pStageSpec, ePX eCrossTopo);
    void vStageFirstSend(StageSpec* pStageSpec, OpSpec* pOpSpec,
                         StageSpec* pNextStageSpec);
    void vStageSubsequentSend(StageSpec* pStageSpec, OpSpec* pOpSpec,
                              StageSpec* pNextStageSpec, OpSpec* pOpSpecSend);

    //
    // token scanning
    //
    Token*     pTkn(void);
    TokenVec*  pTknVec(void);

    void vTokenAccept(void);

  };

  PipelineSpec* _pPipelineSpec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
