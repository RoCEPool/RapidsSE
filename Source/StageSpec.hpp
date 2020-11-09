/*============================================================================*/
/* StageSpec.hpp -- Stage Specification                                       */
/*============================================================================*/
#pragma once

namespace PipelineSpecs
{
  class StageSpec : public VectorNL_t<CN_STAGESPEC,OpSpec,C_MAX_OPS>
  {
  private:
    PipelineSpec* m_pPipelineSpec;  // to which pipeline spec
  
    s32  m_sStageNm;       // stage name
    i8   m_i_StageSpec;    // stage no
  
    ePO  m_eTopology;  // stage topoplogy
    bool m_bNodeTopo;  // is node-level topology (or partition)?
    i8   m_i_Node;     // stage node
    tPN  m_tPtnNo;     // stage ptn
    //i8  m_nThreads;  // threads
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    StageSpec(Stz* szStageNm, i8 i_StageSpec, PipelineSpec* pPipelineSpec);
   ~StageSpec(void) noexcept(false);
   
    i8   i_StageSpec(void);
    void vAddOpSpec(OpSpec* pOpSpec);
    void vCopyOps(StageSpec* pStageSpec);
    Stz* szStageNm(void);
    void vNode(i8 i_Node);
    i8   iNode(void);
    bool bNodeTopo(void);
    PipelineSpec* pPipelineSpec(void);
    tPN  tPtnNo(void);
    void vPtnNo(tPN tPtnNo);
    OpSpec* pPrependConsumeOp(ePX eTopologyX, i8 iStage, i8 i_Op);
    //void vThreads(i8 iThreads);
    //i8   nThreads(void);
    ePO  eTopology(void);
    void vTopology(ePO eTopology);
    void vToPipeStr(Stz* szPipeStr);
  
    void vPrint(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
