/*============================================================================*/
/* OpSpec.hpp -- Op Specification                                             */
/*============================================================================*/
#pragma once

#include "eXK.h"

#include "DeleteHelper.hpp"

namespace STORE { class PipelineSpec; class StageSpec; }

namespace STORE
{
  class OpSpec : public Object,
                 public DeleteHelper
  {
  private:
    s32  m_sOpNm;       // op name
  
    StageSpec*    m_pStageSpec;    // which stage spec
    i8            m_i_OpSpec;      // which op in stage
  
    TagSet m_oArgTags;
    
    ePX m_eCrossTopo;
    i8  m_i_NextStage;
    i8  m_i_NextOp;
    i8  m_i_OtherStage;    // other stage in the cross topology
    i8  m_i_OtherOp;       // next Op in the other stage

    bool   m_bAppendRight; // append to right queue of next operator
    eXK    m_eSendKind;    // {normal, broadcast, hashcast, robin, local robin} if send op

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    OpSpec(Stz* szOpNm, i8 i_OpSpec, StageSpec* pStageSpec);
   ~OpSpec(void) noexcept(false);
  
    bool bAppendRight(void);
    void vAppendRight(bool bRight);
    void vArgTagAdd(Stz* sName, i8 iValue, eTN eToken);
    void vArgTagAdd(Stz* sName, Stz* sValue, eTN eToken);
    TagSet* pArgTags(void);
    Stz* szOpNm(void);
    ePX  eCrossTopo(void);
    void vCrossTopo(ePX eCrossTopo);
    void vCrossTopo(ePX eCrossTopo, i8 i_NextStage, i8 i_NextOp);
    i8   iNextOp(void);
    void vNextOp(i8 i_NextOp);
    i8   iNextStage(void);
    void vNextStage(i8 i_NextStage);
    void vOpNm(Stz* szOpNm);
    i8   iOpSpec(void);
    void vOpSpec(i8 i_OpSpec);
    i8   iOtherOp(void);
    void vOtherOp(i8 i_OtherOp);
    i8   iOtherStage(void);
    void vOtherStage(i8 i_OtherStage);
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
