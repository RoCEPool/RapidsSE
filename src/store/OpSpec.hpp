/*============================================================================*/
/* OpSpec.hpp -- Op Specification                                             */
/*============================================================================*/
#pragma once

#include "eXK.h"
#include "eYO.h"

#include "LifeGuard_t.hpp"

namespace STORE { class PipelineSpec; class StageSpec; }

namespace STORE
{
  class OpSpec : public Object
               , public LifeGuard_t<OpSpec>

               , public ListItem_t<OpSpec>
  {
  private:
    s32  m_sOpNm;       // op name
  
    StageSpec*    m_pStageSpec;    // which stage spec
  
    TagSet m_oArgTags;
    
    eYX m_eCrossTopo;
    eYO m_eOpTopology;

    s32 m_sNextStageNm;    // next stage name
    i8  m_i_NextStage;
    i8  m_i_NextOp;
  
    s32 m_sOtherStageNm;   // next stage name
    i8  m_i_OtherStage;    // other stage in the cross topology
    i8  m_i_OtherOp;       // next Op in the other stage

    bool   m_bAppendRight; // append to right queue of next operator
    eXK    m_eSendKind;    // {normal, broadcast, hashcast, robin, local robin} if send op

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    OpSpec(Stz* szOpNm, StageSpec* pStageSpec);
   ~OpSpec(void) noexcept(false);
  
    bool bAppendRight(void);
    void vAppendRight(bool bRight);
    void vArgTagAdd(Stz* sName, i8 iValue, eTN eToken);
    void vArgTagAdd(Stz* sName, Stz* sValue, eTN eToken);
    TagSet* pArgTags(void);
    Stz* szOpNm(void);
    eYX  eCrossTopo(void);
    void vCrossTopo(eYX eCrossTopo, eYO eOpTopology,
                    Stz* szNextStageNm, i8 i_NextStage, i8 i_NextOp);
    i8   iNextOp(void);
    i8   iNextStage(void);
    Stz* szNextStageNm(void);
    void vOpNm(Stz* szOpNm);
    i8   iOpSpec(void);
    eYO  eOpTopology(void);
    i8   iOtherOp(void);
    i8   iOtherStage(void);
    Stz* szOtherStageNm(void);
    void vOtherStageOp(Stz* szOtherStageNm, i8 i_OtherStage,i8 i_OtherOp);
    eXK  eSendKind(void);
    void vSendKind(eXK eStageKind);
    StageSpec* pStageSpec(void);
  
    Footer* aFooter(void);
  };

  OpSpec* _pOpSpec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
