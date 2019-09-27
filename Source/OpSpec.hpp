/*============================================================================*/
/* OpSpec.hpp -- Op Specification                                             */
/*============================================================================*/
#pragma once

namespace Dynamics
{
  class OpSpec : public Object
  {
  private:
    s32  m_sLabel;
    s32  m_sName;
    bool m_bAppendRight;
    i8   m_i_Slot;

    EnmSet  m_oEnmSet;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    OpSpec(Stz* szOpName);
    OpSpec(Stz* szOpLabel, Stz* szOpName, bool bAppR, i8 i_Slot);
   ~OpSpec(void) noexcept(false);
  
    void vAddEnmPair(Stz* sName, Stz* sValue, eTN eToken);
    bool bAppendRight(void);
    EnmSet* pEnmSet(void);
    Stz* szLabel(void);
    Stz* szName(void);
    void vName(Stz* szName);
    i8   i_Slot(void);

    void vPrint(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
