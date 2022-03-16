/*============================================================================*/
/* Stage.hpp -- Stage of Pipeline (being a vector of Stbream)                  */
/*============================================================================*/
#pragma once

#include "eYS.h"

#include "LifeGuard_t.hpp"

namespace STORE { class Formwork; class Stream; class StageSpec; }

#ifdef WITH_FORMWORK

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
    Formwork*   m_pFormwork;  // which pipeline
  
    // -- -- -- -- -- --

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Stage(StageSpec* pStageSpec, Formwork* pFormwork);
   ~Stage(void) noexcept(false);
      
    void vCancelPipeline(Task* pTriggeringTask);
    Stream* pGetStream_byNodeNo(i8 i_Node);
    Stream* pGetStream_byPtnNo(i8 i_Ptn);
    Stream* pGetStream_byStreamNo(i8 i_Stream);
    i8   nOpsOnNode(i8 iNode, i8 iOrigin);
    Formwork* pFormwork(void);
    Op*  pPipeDstOp(PtnId tDstPtnId, i8 i_DstOp);
    i8   i_Stage(void);
    Stz* szStageNm(void);
    StageSpec* pStageSpec(void);
    bool bStartPipeline(void);
    void vWalk(void* aArg);
    void vWalkLambda(
           i8 iDepth,
           std::function<void(Object* pObject, i8 iDepth)> f_vClosure);

  void    vCreateStreams(void);

  private:

    Footer* aFooter(void);
  };
  
  Stage* _pStage(void* aAddr);
}

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#else

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
    Pipeline*   m_pPipeline;  // which pipeline
  
    // -- -- -- -- -- --

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Stage(StageSpec* pStageSpec, Pipeline* pPipeline);
   ~Stage(void) noexcept(false);
      
    void vCancelPipeline(Task* pTriggeringTask);
    Stream* pGetStream_byNodeNo(i8 i_Node);
    Stream* pGetStream_byPtnNo(i8 i_Ptn);
    Stream* pGetStream_byStreamNo(i8 i_Stream);
    i8   nOpsOnNode(i8 iNode, i8 iOrigin);
    Pipeline* pPipeline(void);
    Op*  pPipeDstOp(PtnId tDstPtnId, i8 i_DstOp);
    i8   i_Stage(void);
    Stz* szStageNm(void);
    StageSpec* pStageSpec(void);
    bool bStartPipeline(void);
    void vWalk(void* aArg);
    void vWalkLambda(
           i8 iDepth,
           std::function<void(Object* pObject, i8 iDepth)> f_vClosure);

  void    vCreateStreams(void);

  private:

    Footer* aFooter(void);
  };
  
  Stage* _pStage(void* aAddr);
}

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
