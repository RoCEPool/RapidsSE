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
    
    bool bGuard(i8 iCount);
    bool bGuard(eCN eClass0);
    bool bGuard(eCN eClass0, eCN eClass1);
    bool bGuard(eCN eClass0, eCN eClass1, eCN eClass2);
    
    bool bPop(eCN& eArg1);
    bool bPop(Header*& pHeader);
    bool bPop(i8& iArg1);
    bool bPop(i8& iArg1, i8& iArg2);
    bool bPop(i8& iArg1, Object*& pObject);
    bool bPop(i8& iArg1, Value& xArg1);
    bool bPop(i8& iArg1, s8K& sArg2);
    bool bPop(i8& iArg1, i8& iArg2, Value& xArg3);
    bool bPop(i8& iArg1, i8& iArg2, s8K& sArg3);
    bool bPop(Object*& pObject);
    bool bPop(Object*& pObject1, Object*& pObject2);
    bool bPop(Object*& pObject, Value& xValue);
    bool bPop(Value& xArg1);
    bool bPop(Value& xArg1, Value& xArg2);
    bool bPop(Value& xArg1, Value& xArg2, Value& xArg3);
    bool bPop(s32&  sArg1);
    bool bPop(s256& sArg1);
    bool bPop(s8K& sArg1);
    bool bPop(s8K& sArg1, s8K& sArg2);
    bool bPop(void*& pVoid);
    bool bPopParamInt(i8& iArg1);
    
    void vPush(Header* pHeader);
    void vPush(i8 iInt);
    void vPush(Object* pObject);
    void vPush(s32& s32);
    void vPush(s256& s256);
    void vPush(s8K& s8K);
    void vPush(Value& xValue);
    void vPush(Value* pValue);
    void vPush(void* pVoid);
    
   
    //-------------------------------------------------------------------------
    
    void   vDiscard(void);
    Value* pPeek(void);
    Value* pPeek(i8 iNthValue);
    void   vPeek(i8 iNthValue, Value* pValue);
    void   vPoke(i8 iNthValue, Value* pValue);
    bool   bPeekZero(void);
    void   vRetry(void);
    void   vRetryPoint(void);
    void   vReset(void);
    i8     iValues(void);
    
  private:
    Value* pGetValue(i8 iValue);
    void   vPushBy(i8 iBytes);
    void   vPopBy(i8 iBytes);
    void   vPopVal(Value* pValue);
    void   vPushVal(Value* pValue);
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