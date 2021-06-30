/*============================================================================*/
/* PipelineSpec.hpp -- pipeline Specification                                 */
/*============================================================================*/
#pragma once

namespace STORE { class Dynamo; class StageSpec; class OpSpec; }

namespace STORE
{
  class PipelineSpec : public VectorNL_t<CN_PIPELINESPEC,StageSpec,C_MAX_STAGES>
  {
  
    friend class STORE::Dynamo;

  private:
    s32  m_sPipelineNm; // pipeline name
  
    // optional set of nested pipelines
    VectorNL_t<CN_NESTEDPIPELINESPEC,PipelineSpec,C_MAX_NESTED_PIPELINES>
      m_oNestedPipelineSpecs;
  
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    Dynamo*  m_p_Dynamo; // the dynamo compiler

  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    PipelineSpec(Stz* szPipelineNm, Dynamo* p_Dynamo);
   ~PipelineSpec(void) noexcept(false);
  
    void vAddStageSpec(StageSpec* pStageSpec);
    StageSpec* pFindStageSpec(Stz* szFindNm);
    Stz* szPipelineNm(void);
    StageSpec* pStageSpec(i8 i_Stage);

    void vPrint(void);

    Footer* aFooter(void);
  
  
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //                            PIPELINE COMPILER
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  private:
  
    //
    // Pass 1 : Collect
    //
    bool bPass1_collect(void);

    bool bCollectBegin(void);
    bool bCollectEnd(void);
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

    bool bTokenIsOp(Token* pTkn);


    //
    // Pass 2 : Staging
    //
    bool bPass2_staging(void);

    void vArgAdd_sends(OpSpec* pConsumeOpSpec, ePX eCrossTopo);
    void vArgUpdate_sends(OpSpec* pConsumeOpSpec, ePX eCrossTopo);

    void vMorphToSend(OpSpec* pOpSpec, ePX eCrossTopo, i8 i_NextStage);

    void vNameIsOp(StageSpec* pStageSpec, OpSpec* pOpSpec);
    void vNameIsPipeline(void);
    void vNameIsStage(StageSpec* pStageSpec, OpSpec* pOpSpec,
                      StageSpec* pNextStageSpec);

    void vStageConsumeOpInsert(StageSpec* pStageSpec, ePX eCrossTopo);
    void vStageFirstSend(StageSpec* pStageSpec, OpSpec* pOpSpec,
                         StageSpec* pNextStageSpec);
    void vStageSubsequentSend(StageSpec* pStageSpec, OpSpec* pOpSpec,
                              StageSpec* pNextStageSpec, OpSpec* pOpSpecSend);

    void vTopoTagsAdd(OpSpec* pOpSpec,
                      ePX eCrossTopo, i8 i_NextStage, i8 i_NextOp);
    void vTopoTagsUpdate(OpSpec* pOpSpec,
                         ePX eCrossTopo, i8 i_NextStage, i8 i_NextOp);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
