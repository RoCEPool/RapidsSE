/*============================================================================*/
/* StageSpec.hpp -- Stage Specification                                       */
/*============================================================================*/
#pragma once

#include "ePO.h"
#include "ePX.h"

#include "OpSpec.hpp"
#include "DeleteHelper.hpp"

namespace STORE
{
  class StageSpec : public VectorNL_t<CN_STAGESPEC,OpSpec,C_MAX_OPS>,
                    public DeleteHelper
  {
  private:
    s32  m_sStageNm;    // stage name

    PipelineSpec* m_pPipelineSpec; // which pipeline spec
    i8   m_i_StageSpec; // this stage spec no

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
  
    StageSpec(i8 i_StageSpec, Stz* szStageNm, PipelineSpec* pPipelineSpec);
   ~StageSpec(void) noexcept(false);
   
    static ePX eCrossTopo(StageSpec* pFromStageSpec, StageSpec* pToStageSpec);

    void vAddOpSpec(OpSpec* pOpSpec);
    void vCopyOps(StageSpec* pStageSpec);
    OpSpec* pFindOpSpec(Stz* szFindNm);
    bool bNodeTopo(void);
    OpSpec* pOpSpec(i8 i_Op);
    PipelineSpec* pPipelineSpec(void);
    void vRenumberOpSpecs(void);
    void vSpecificNode(i8 i_SpecificNode);
    i8   iSpecificNode(void);
    tPN  tSpecificPtnNo(void);
    void vSpecificPtnNo(tPN tSpecificPtnNo);
    Stz* szStageNm(void);
    i8   i_StageSpec(void);
    //void vThreads(i8 iThreads);
    //i8   nThreads(void);
    ePO  eTopology(void);
    void vTopology(ePO eTopology);
    void vToPipeStr(Stz* szPipeStr);
  
    Footer* aFooter(void);
  };

  StageSpec* _pStageSpec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
