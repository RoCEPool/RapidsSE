/*============================================================================*/
/* OpSpec.hpp -- Op Specification                                             */
/*============================================================================*/
#pragma once

namespace Dynamics
{
  class OpSpec : public Object
  {
  private:
    s32 m_sName;
  
    s4K m_sArgs;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    OpSpec(Stz* szName);
   ~OpSpec(void) noexcept(false);
  
    Stz* szArgs(void);
    void vArgs(Stz* szArgs);
    Stz* szName(void);
  
    void vPrint(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
