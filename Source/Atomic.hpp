/*============================================================================*/
/* Atomic.cpp -- atomic integer                                               */
/*============================================================================*/

#pragma once

#include "Common.h"

namespace Common
{
  class Atomic : public Header
  {
  private:
    i8 m_iValue;
    
  public:
    void* operator new(size_t, Atomic* pAtomic);
    void  operator delete(void* pAtomicAddress);
    
    Atomic(void);
    ~Atomic(void);
    
    void vInit(void);
    void vCheckInit(void);
    
    i8   iInc(void);
    i8   iDec(void);
    
    i8 operator *(void);  // dereference
    i8 operator ++(void); // prefix form
    i8 operator --(void); // prefix form
    
    // must be last
    Footer m_xFooter;
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/