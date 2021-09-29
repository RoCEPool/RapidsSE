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
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    TagSet*  m_p_JobTags; // job tag set
    Scanner* m_p_Scanner; // statement token scanner

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    PipelineSpec(Stz*          szPipelineNm,
                 TagSet*       p_JobTags,
                 Scanner*      p_Scanner);
   ~PipelineSpec(void) noexcept(false);
  
    bool bCompile(void);
  
    void vAddStageSpec(StageSpec* pStageSpec);
    StageSpec* pFindStageSpec(Stz* szFindNm);
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

    bool bCollectPipeline(void);
    bool bCollectTag(void);
    bool bCollectBeginEnd(void);
    bool bCollectStage(void);
    bool bCollectTopology(StageSpec* p_StageSpec);
    bool bCollectOpSequence(StageSpec* p_StageSpec);
    bool bCollectOp(StageSpec* p_StageSpec);
    bool bCollectOpArgs(StageSpec* p_StageSpec, OpSpec* pOpSpec);

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

    void vMorphToSend(OpSpec* pOpSpec, ePX eCrossTopo, i8 i_NextStage);

    void vNameIsOp(StageSpec* pStageSpec, OpSpec* pOpSpec);
    void vNameIsStage(StageSpec* pStageSpec,
                      OpSpec*    pOpSpec,
                      StageSpec* pNextStageSpec);

    void vStageConsumeOpInsert(StageSpec* pStageSpec, ePX eCrossTopo);
    void vStageFirstSend(StageSpec* pStageSpec, OpSpec* pOpSpec,
                         StageSpec* pNextStageSpec);
    void vStageSubsequentSend(StageSpec* pStageSpec, OpSpec* pOpSpec,
                              StageSpec* pNextStageSpec, OpSpec* pOpSpecSend);

    //
    // Pass 3 : Includes
    //
    bool bPass3_includes(void);

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
