/*============================================================================*/
/* OpSpec.hpp -- Op Specification                                             */
/*============================================================================*/
#pragma once

#include "eXK.h"
#include "eYS.h"

#include "LifeGuard_t.hpp"

namespace STORE { class PipelineSpec; class StageSpec; }

namespace STORE
{
  class OpSpec : public Object
               , public LifeGuard_t<OpSpec>

               , public ListItem_t<OpSpec>
  {
  private:
    s32  m_sOpNm;  // op name
  
    StageSpec* m_pStageSpec;  // which stage spec
  
    TagSet m_oArgTags;

  
    // -- -- -- -- -- --
  
    eYS m_eStreamTopology_op;
    eYX m_eCrossTopology;
    eXK m_eSendCast;     // {broadcast, hashcast, localRobin, robin, unicast}
                         //   if send op

    s32 m_sNextStageNm;  // next stage name
    i8  m_i_NextStage;
    i8  m_i_NextOp;

    s32 m_sOtherStageNm; // next stage name
    i8  m_i_OtherStage;  // other stage in the cross topology
    i8  m_i_OtherOp;     // next Op in the other stage


    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    OpSpec(Stz* szOpNm, StageSpec* pStageSpec);
   ~OpSpec(void) noexcept(false);
  
    void vArgTagAdd(Stz* sName, i8 iValue, eTN eToken);
    void vArgTagAdd(Stz* sName, Stz* sValue, eTN eToken);
    TagSet* pArgTags(void);
    eYX  eCrossTopology(void);
    void vCrossTopology(eYX eCrossTopology,
                    Stz* szNextStageNm, i8 i_NextStage, i8 i_NextOp);
    i8   iNextOp(void);
    i8   iNextStage(void);
    Stz* szNextStageNm(void);
    void vNextStageOp(i8 i_NextStage, i8 i_NextOp);
    void vNextStageOp(Stz* szNextStageNm, i8 i_NextStage, i8 i_NextOp);
    Stz* szOpNm(void);
    void vOpNm(Stz* szOpNm);
    i8   iOpSpec(void);
    i8   iOtherOp(void);
    i8   iOtherStage(void);
    Stz* szOtherStageNm(void);
    void vOtherStageOp(Stz* szOtherStageNm, i8 i_OtherStage,i8 i_OtherOp);
    eXK  eSendCast(void);
    void vSendCast(eXK eSendCast);
    StageSpec* pStageSpec(void);
    eYS  eStreamTopology_op(void);
    void vStreamTopology_op(eYS eStreamTopology);

    Footer* aFooter(void);
  };

  OpSpec* _pOpSpec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
