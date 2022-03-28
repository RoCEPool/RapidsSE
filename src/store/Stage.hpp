/*============================================================================*/
/* Stage.hpp -- Stage of Pipeline (being a vector of Stbream)                  */
/*============================================================================*/
#pragma once

#include "eYS.h"

#include "LifeGuard_t.hpp"

namespace STORE { class PipeWork; class Stream; class StageSpec; }

namespace STORE
{
  class Stage : public VectorNL_t<CN_STAGE,Stream,C_MAX_STREAMS>,
                public LifeGuard_t<Stage>
  {
  private:
    s32  m_sPipelineNm; // pipeline name
    s32  m_sStageNm;    // name of this stage
    i8   m_i_Stage;     // [stage] in pipeline

    StageSpec*  m_pStageSpec; // specification of this stage
    PipeWork*   m_pPipeWork;  // which pipeline
  
    // -- -- -- -- -- --

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Stage(StageSpec* pStageSpec, PipeWork* pPipeWork);
   ~Stage(void) noexcept(false);
      
    void vCancelPipeline(Task* pTriggeringTask);
    Stream* pGetStream_byNodeNo(i8 i_Node);
    Stream* pGetStream_byPtnNo(i8 i_Ptn);
    Stream* pGetStream_byStreamNo(i8 i_Stream);
    Op*  pOp0(void);
    i8   nOpsOnNode(i8 iNode, i8 iOrigin);
    PipeWork* pPipeWork(void);
    Op*  pPipeDstOp(PtnId tDstPtnId, i8 i_DstOp);
    i8   i_Stage(void);
    Stz* szStageNm(void);
    Stream* pStream0(void);
    StageSpec* pStageSpec(void);
    bool bEnqueueInvoke(void);

  void    vCreateStreams(void);

  private:

    Footer* aFooter(void);
  };
  
  Stage* _pStage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
