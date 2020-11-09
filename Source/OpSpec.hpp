/*============================================================================*/
/* OpSpec.hpp -- Op Specification                                             */
/*============================================================================*/
#pragma once

namespace PipelineSpecs
{
  class OpSpec : public Object
  {
  private:
    StageSpec*    m_pStageSpec;     // to which stage spec
    PipelineSpec* m_pPipelineSpec;  // of which pipeline spec
  
    s32    m_sOpNm;
    i8     m_i_OpSpec;
    i8     m_i_StageSpec;

    bool   m_bAppendRight;
    eXK    m_eXmitKind;
    
    TagSet m_oOpSpecTagSet;
    TagSet m_oTopologyTagSet;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    OpSpec(Stz* szOpNm, i8 i_OpSpec, StageSpec* pStageSpec);
   ~OpSpec(void) noexcept(false);
  
    void vAddEnamePair(Stz* sName, Stz* sValue, eTN eToken);
    void vAddEnamePair(Stz* sName, i8 iValue, eTN eToken);
    void vAddTopologyEnamePair(Stz* sName, i8 iValue, eTN eToken);
    bool bAppendRight(void);
    Stz* szOpNm(void);
    void vOpNm(Stz* szOpNm);
    i8   i_OpSpec(void);
    void v_OpSpec(i8 i_OpSpec);
    TagSet* pOpSpecTagSet(void);
    void vReplaceTopologyEnamePair(Stz* szName, Stz* szValue, eTN eToken);
    void vReplaceTopologyEnamePair(Stz* sName, i8 iValue, eTN eToken);
    TagSet* pTopologyTagSet(void);
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
