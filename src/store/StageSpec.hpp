/*============================================================================*/
/* StageSpec.hpp -- Stage Specification                                       */
/*============================================================================*/
#pragma once

#include "eYG.h"
#include "eYX.h"

#include "OpSpec.hpp"
#include "LifeGuard_t.hpp"

namespace STORE
{
  class StageSpec
          : public ListNL_t<OpSpec>
          , public LifeGuard_t<StageSpec>
          , public ListItem_t<StageSpec>

  {
  private:
    s32  m_sStageNm;    // stage name

    PipelineSpec* m_pPipelineSpec; // which pipeline spec

    eYG  m_eStageTopology;  // stage topoplogy
    bool m_bNodeTopo;       // is node-level topology (or partition)?
    i8   m_i_SpecificNode;  // run on specific node
    PtnId m_tSpecificPtnNo; // run for specific ptn
    //i8  m_nThreads;       // threads
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    StageSpec(Stz* szStageNm, PipelineSpec* pPipelineSpec);
   ~StageSpec(void) noexcept(false);
   
    static eYX eCrossTopo(StageSpec* pFromStageSpec,
                          StageSpec* pToStageSpec,
                          ePZ&       reOpTopology);

    void vAddOpSpec(OpSpec* pOpSpec);
    void vCopyOps(StageSpec* pStageSpec);
    OpSpec* pFindOpSpec(Stz* szFindNm);
    bool bNodeTopo(void);
    OpSpec* pOpSpec(i8 i_Op);
    PipelineSpec* pPipelineSpec(void);
    void vSpecificNode(i8 i_SpecificNode);
    i8   iSpecificNode(void);

    PtnId tSpecificPtnNo(void);
    void vSpecificPtnNo(PtnId tSpecificPtnNo);

    Stz* szStageNm(void);
    i8   i_StageSpec(void);
    //void vThreads(i8 iThreads);
    //i8   nThreads(void);
    eYG  eStageTopology(void);
    void vStageTopology(eYG eStageTopology);
    void vToPipeStr(Stz* szPipeStr);
  
    Footer* aFooter(void);
  };

  StageSpec* _pStageSpec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
