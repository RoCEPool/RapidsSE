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
    i8 m_iInt;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, Atomic* pAtomic);
    void  operator delete(void* pAtomicAddress);
    
    Atomic(void);
    ~Atomic(void);
    
    void vCheckInit(void);
    
    i8   iInc(void);      // increment
    i8   iDec(void);      // decrement
    
    void operator =(i8 iInt); // assignment
    i8 operator *(void);  // dereference
    i8 operator ++(void); // increment (prefix form)
    i8 operator --(void); // decrement (prefix form)
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/