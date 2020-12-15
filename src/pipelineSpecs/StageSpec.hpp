/*============================================================================*/
/* StageSpec.hpp -- Stage Specification                                       */
/*============================================================================*/
#pragma once

#include "ePO.h"
#include "ePX.h"

namespace PipelineSpecs
{
  class StageSpec : public VectorNL_t<CN_STAGESPEC,OpSpec,C_MAX_OPS>
  {
  private:
    s32  m_sPipelineNm; // pipeline name
    s32  m_sStageNm;    // stage name
    i8   m_i_StageSpec; // this stage spec no

    PipelineSpec* m_pPipelineSpec; // which pipeline spec
    
    Job*          m_pJob;          // which job

    ePO  m_eTopology;      // stage topoplogy
    bool m_bNodeTopo;      // is node-level topology (or partition)?
    i8   m_i_SpecificNode; // run on specific node
    tPN  m_tSpecificPtnNo; // run for specific ptn
    //i8  m_nThreads;      // threads
    
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
    bool bNodeTopo(void);
    PipelineSpec* pPipelineSpec(void);
    OpSpec* pPrependConsumeOp(ePX eCrossTopo, i8 iStage, i8 i_Op);
    void vSpecificNode(i8 i_SpecificNode);
    i8   iSpecificNode(void);
    tPN  tSpecificPtnNo(void);
    void vSpecificPtnNo(tPN tSpecificPtnNo);
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
