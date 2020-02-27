/*============================================================================*/
/* OpSpec.hpp -- Op Specification                                             */
/*============================================================================*/
#pragma once

namespace PipelineSpecs
{
  class OpSpec : public Object
  {
  private:
    s32        m_sOpNm;
    StageSpec* m_pStageSpec;
  
    bool       m_bAppendRight;
    i8         m_i_OpSpec;
    i8         m_i_Slot;
    eXK        m_eXmitKind;
    
    EnameSet   m_oEnameSet;
    EnameSet   m_oTopologyEnameSet;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    OpSpec(Stz* szOpNm, StageSpec* pStageSpec);
   ~OpSpec(void) noexcept(false);
  
    void vAddEnamePair(Stz* sName, Stz* sValue, eTN eToken);
    void vAddEnamePair(Stz* sName, i8 iValue, eTN eToken);
    void vAddTopologyEnamePair(Stz* sName, i8 iValue, eTN eToken);
    bool bAppendRight(void);
    EnameSet* pEnameSet(void);
    Stz* szOpNm(void);
    void vOpNm(Stz* szOpNm);
    i8   i_OpSpec(void);
    void v_OpSpec(i8 i_OpSpec);
    void vReplaceTopologyEnamePair(Stz* szName, Stz* szValue, eTN eToken);
    void vReplaceTopologyEnamePair(Stz* sName, i8 iValue, eTN eToken);
    EnameSet* pTopologyEnameSet(void);
    i8   i_Slot(void);
    eXK  eXmitKind(void);
    void vXmitKind(eXK eStageKind);
    StageSpec* pStageSpec(void);

    void vPrint(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
