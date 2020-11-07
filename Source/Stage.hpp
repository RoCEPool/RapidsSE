/*============================================================================*/
/* Stage.hpp -- Pipe Stage                                                    */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class Stage : public VectorNL_t<CN_STAGE,Strand,C_MAX_STRANDS>
  {
  private:
    s32         m_sStageNm;   // stage name
    StageSpec*  m_pStageSpec; // specification of this stage
    Pipeline*   m_pPipeline;  // which pipeline
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Stage(StageSpec* pStageSpec, Pipeline* pPipeline);
   ~Stage(void) noexcept(false);
  
    void vDump(Pipeline* pPipeline);
    
    void    vCancelPipeline(Task* pTriggeringTask);
    Strand* pGetPartitionStrand(i8 i_Ptn);
    Strand* pGetNodeStrand(i8 i_Node);
    i8      nOpsOnNode(i8 iNode);
    Op*     pPipeOutOp(ePX eTopologyX, i8 i_Node, tPN tPtnNo, i8 i_Op);
    Stz*    szStageNm(void);
    StageSpec* pStageSpec(void);
    void    vStartPipeline(Task* pTriggeringTask);
  
  private:
    void    vCreateStrands(void);

    Footer* aFooter(void);
  };
  
  Stage* _pStage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
