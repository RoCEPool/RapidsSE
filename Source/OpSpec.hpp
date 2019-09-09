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

    s4K m_sArgs;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    OpSpec(Stz* szOpName);
    OpSpec(Stz* szOpLabel, Stz* szOpName, bool bAppR, i8 i_Slot);
   ~OpSpec(void) noexcept(false);
  
    bool bAppendRight(void);
    Stz* szArgs(void);
    void vArgs(Stz* szArgs);
    Stz* szLabel(void);
    Stz* szName(void);
    i8   i_Slot(void);

    void vPrint(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
