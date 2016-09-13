/*============================================================================*/
/* Stack.hpp -- Push/Pop Stack of Values                                      */
/*============================================================================*/

#pragma once

#include "Structures.h"

namespace Structures
{
  class Stack : public Object
  {
  private:
    static const i8 C_STACK_AREA = 4096;
    
  private:
    Value* m_pTop;    // current top of stack
    i8     m_iValues; // number of values currently on stack

    Value* m_pRetryTop;    // reset to this top of stack
    i8     m_iRetryValues; // reset with this many values on stack
    
    Footer m_xHazard;
    
    char   m_mSpace[C_STACK_AREA];
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Stack(void);
    ~Stack(void);
    
    void vPushParam(Header* pHeader);
    void vPushParam(i8 iInt);
    void vPushParam(Object* pObject);
    void vPushParam(s32& s32);
    void vPushParam(s256& s256);
    void vPushParam(s8K& s8K);
    void vPushParam(Value& xValue);
    void vPushParam(Value* pValue);
    void vPushParam(void* pVoid);
    
   
    //-------------------------------------------------------------------------
    
    void   vDiscard(void);
    Value* pPeek(void);
    Value* pPeek(i8 iNthValue);
    void   vPeek(i8 iNthValue, Value* pValue);
    void   vPoke(i8 iNthValue, Value* pValue);
    void   vPop(Value* pValue);
    void   vPush(Value* pValue);
    void   vRetry(void);
    void   vRetryPoint(void);
    void   vReset(void);
    i8     iValues(void);
    
    bool   bPeekZero(void);
    
  private:
    Value* pGetValue(i8 iValue);
    void   vPushBy(i8 iBytes);
    void   vPopBy(i8 iBytes);
    Value* pStackBase(void);
    Value* pStackLimit(void);
    Value* pTopValue(void);
    
    Footer* pFooter(void);
    Footer* pHazard(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/