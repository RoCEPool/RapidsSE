/*============================================================================*/
/* Stack.hpp -- Push/Pop Stack of Values                                      */
/*============================================================================*/

#pragma once

#include "DataStructures.h"

namespace DataStructures
{
  class Stack : public Object
  {
  private:
    static const i8 C_STACK_AREA = 4096;
    
  private:
    OnStack* m_pTop;    // current top of stack
    i8       m_iValues; // number of values currently on stack

    OnStack* m_pRetryTop;    // reset to this top of stack
    i8       m_iRetryValues; // reset with this many values on stack
    
    char     m_mSpace[C_STACK_AREA];
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Stack(void);
    ~Stack(void);
    
    void   vDiscard(void);
    OnStack* pPeek(void) const;
    OnStack* pPeek(i8 iNthValue) const;
    void   vPeek(i8 iNthValue, OnStack* pValue);
    void   vPoke(i8 iNthValue, OnStack* pValue);
    void   vPop(OnStack* pValue);
    void   vPush(OnStack* pValue);
    void   vRetry(void);
    void   vRetryPoint(void);
    void   vReset(void);
    i8     iValues(void) const;
    
  private:
    OnStack* pGetValue(i8 iValue) const;
    void   vPushBy(i8 iBytes);
    void   vPopBy(i8 iBytes);
    OnStack* pStackBase(void);
    OnStack* pStackLimit(void) const;
    OnStack* pTopValue(void) const;
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/