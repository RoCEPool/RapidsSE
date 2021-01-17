/*============================================================================*/
/* Stage.hpp -- Stage of Pipeline                                             */
/*============================================================================*/
#pragma once

namespace STORE
{
  class Stage : public VectorNL_t<CN_STAGE,Stream,C_MAX_STRANDS>
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
  
    void vDump(Pipeline* pPipeline);
    
    void    vCancelPipeline(Task* pTriggeringTask);
    Stream* pGetPartitionStream(i8 i_Ptn);
    Stream* pGetNodeStream(i8 i_Node);
    i8      nOpsOnNode(i8 iNode);
    Pipeline* pPipeline(void);
    Op*     pPipeOutOp(ePX eCrossTopo, i8 i_Node, tPN tPtnNo, i8 i_Op);
    Stz*    szStageNm(void);
    StageSpec* pStageSpec(void);
    void    vStartPipeline(Task* pTriggeringTask);
  
  private:
    void    vCreateStreams(void);

    Footer* aFooter(void);
  };
  
  Stage* _pStage(void* aAddr);
}

// declare a function pointer to any member function of Step
typedef void (Step::*fpSTEPFN)(void);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
