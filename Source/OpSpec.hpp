/*============================================================================*/
/* OpSpec.hpp -- Op Specification                                             */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class OpSpec : public Object
  {
  private:
    s32  m_sName;
    bool m_bAppendRight;
    StageSpec* m_pStageSpec;
    i8   m_i_OpSpec;
  
    i8   m_i_Slot;

    EnmSet  m_oEnmSet;
    OpDefn* m_aOpDefn;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    OpSpec(Stz* szOpName, StageSpec* pStageSpec);
   ~OpSpec(void) noexcept(false);
  
    void vAddEnmPair(Stz* sName, Stz* sValue, eTN eToken);
    void vAddEnmPair(Stz* sName, i8 iValue, eTN eToken);
    bool bAppendRight(void);
    EnmSet* pEnmSet(void);
    Stz* szName(void);
    void vName(Stz* szName);
    OpDefn* aOpDefn(void);
    void vOpDefn(OpDefn* aOpDefn);
    i8   i_OpSpec(void);
    void v_OpSpec(i8 i_OpSpec);
    i8   i_Slot(void);
    StageSpec* pStageSpec(void);

    void vPrint(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
