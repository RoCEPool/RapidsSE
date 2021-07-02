/*============================================================================*/
/* OpSpec.hpp -- Op Specification                                             */
/*============================================================================*/
#pragma once

#include "eXK.h"

namespace STORE { class PipelineSpec; class StageSpec; }

namespace STORE
{
  class OpSpec : public Object
  {
  private:
    s32  m_sPipelineNm; // pipeline name
    s32  m_sStageNm;    // stage name
    s32  m_sOpNm;       // op name
  
    PipelineSpec* m_pPipelineSpec; // which pipeline spec
    StageSpec*    m_pStageSpec;    // which stage spec
    i8            m_i_OpSpec;      // which op in stage
  
    TagSet m_oArgTags;
    TagSet m_oTopologyTags;
    
    bool   m_bAppendRight; // append to right queue of next operator
    eXK    m_eSendKind;    // {normal, hashed, robin, local robin} if send op

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
    void vOpNm(Stz* szOpNm);
    i8   i_OpSpec(void);
    void v_OpSpec(i8 i_OpSpec);
    eXK  eSendKind(void);
    void vSendKind(eXK eStageKind);
    StageSpec* pStageSpec(void);
    void vTopoTagAdd(Stz* sName, i8 iValue, eTN eToken);
    void vTopoTagReplace(Stz* szName, Stz* szValue, eTN eToken);
    void vTopoTagReplace(Stz* sName, i8 iValue, eTN eToken);
    TagSet* pTopoTags(void);

    void vPrint(void);
  
    Footer* aFooter(void);
  };

  OpSpec* _pOpSpec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
