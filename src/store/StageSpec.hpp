/*============================================================================*/
/* StageSpec.hpp -- Stage Specification                                       */
/*============================================================================*/
#pragma once

#include "eAL.h"
#include "eYG.h"
#include "eYX.h"
#include "eYF.h"

#include "OpSpec.hpp"
#include "LifeGuard_t.hpp"

namespace STORE { class PipeSpec; }

namespace STORE
{
  class StageSpec
          : public ListNL_t<OpSpec>
          , public LifeGuard_t<StageSpec>
          , public ListItem_t<StageSpec>

  {
  private:
    s32  m_sStageNm;    // stage name

    PipeSpec* m_pPipeWorkSpec; // which formwork spec

    eYG  m_eStageTopology;  // stage topoplogy
    eYS  m_eStreamTopology; // stream topoplogy
    eYF  m_eParcelForward;  // how parcel is forwarded

    bool m_bNodeTopo;       // is node-level topology (or partition)?
    i8   m_i_SpecificNode;  // run on specific node
    PtnId m_tSpecificPtnNo; // run for specific ptn

  
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    StageSpec(Stz* szStageNm, PipeSpec* pPipeWorkSpec);
   ~StageSpec(void) noexcept(false);
   
    static eYX eCrossTopoCalc(StageSpec* pFromStageSpec,
                              StageSpec* pToStageSpec,
                              eYF&       reParcelForward,
                              eYS&       reIntraTopology);

    static eYS eStreamTopoCalc(eYG  m_eStageTopology);

    void vAddOpSpec(OpSpec* pOpSpec);
    eAL  eAggegLevel(void);

    void vCopyOps(StageSpec* pStageSpec);
  
    OpSpec* pFindOpSpec(Stz* szFindNm);
  
    bool bNodeTopo(void);
  
    OpSpec* pOpSpec(i8 i_Op);
  
    eYF  eParcelForward(void);
    void vParcelForward(eYF eParcelForward);
    PipeSpec* pPipeSpec(void);
  
    void vSpecificNode(i8 i_SpecificNode);
    i8   iSpecificNode(void);
    PtnId tSpecificPtnNo(void);
    void vSpecificPtnNo(PtnId tSpecificPtnNo);
    Stz* szStageNm(void);
    i8   i_StageSpec(void);
    eYG  eStageTopology(void);
    void vStageTopology(eYG eStageTopology);
    eYS  eStreamTopology(void);
    void vStreamTopology(eYS eStreamTopology);
    void vToPipeStr(Stz* szPipeStr);
  
    Footer* aFooter(void);
  };

  StageSpec* _pStageSpec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
