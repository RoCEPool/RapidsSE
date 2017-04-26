/*============================================================================*/
/* Stack.hpp -- Push/Pop Stack of Values                                      */
/*============================================================================*/
#pragma once

namespace Dictionaries
{
  class Cell;
}

namespace Structures
{
  static const i8 C_STACK_LENGTH = C_SIZE_POOL_BUFFER - C_1KB;
  
  class Stack : public Buffer
  {
  private:
  
  private:
    i8     m_iValues; // number of values currently on stack
    Value* m_pTop;    // current top of stack

    Value* m_pRetryTop;    // reset to this top of stack
    i8     m_iRetryValues; // reset with this many values on stack
    
    Footer m_xHazard;
    
    WspFxd<C_STACK_LENGTH> m_xWsp;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Stack(void);
    ~Stack(void);
    
    static Stack* _pStack(void* aAddr);
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
    
    bool bGuard(i8 iCount);
    bool bGuard(eCN eClass0);
    bool bGuard(eCN eClass0, eCN eClass1);
    bool bGuard(eCN eClass0, eCN eClass1, eCN eClass2);
    
    bool bPop(Dictionaries::Cell*& pCell);
    bool bPop(eCN& eArg1);
    //bool bPop(Header*& pHeader);
    bool bPop(i8& iArg1);
    bool bPop(i8& iArg1, i8& iArg2);
    bool bPop(i8& iArg1, Object*& pObject);
    bool bPop(i8& iArg1, Value& xArg1);
    bool bPop(i8& iArg1, s48K& sArg2);
    bool bPop(i8& iArg1, i8& iArg2, Value& xArg3);
    bool bPop(Object*& pObject);
    bool bPop(Object*& pObject, i8& iArg);
    bool bPop(Object*& pObject1, Object*& pObject2);
    bool bPop(Object*& pObject, Value& xValue);
    bool bPop(Value* pArg1);
    bool bPop(Value& xArg1);
    bool bPop(Value& xArg1, Value& xArg2);
    bool bPop(Value& xArg1, Value& xArg2, Value& xArg3);
    bool bPop(s32&  sArg1);
    bool bPop(s256& sArg1);
    bool bPop(s48K& sArg1);
    bool bPop(s48K& sArg1, i8& iArg2);
    bool bPop(s48K& sArg1, i8& iArg2, i8& iArg3);
    bool bPop(s48K& sArg1, s48K& sArg2);
    bool bPopBinary(Object*& pObject);
    bool bPopParamInt(i8& iArg1);
    
    void vPushBinary(Object* pObject);
    void vPushCell(Dictionaries::Cell* pCell);
    void vPushInt(i8 iInt);
    void vPushObject(Object* pObject);
    void vPushS32(s32& s32);
    void vPushS256(s256& s256);
    void vPushS48K(s48K& s48K);
    void vPushValue(Value* pValue);
    void vPushValue(Value& xValue);
    void vPushValSet(ValSet& xValSet);
    void vPushValWsp(ValWsp& xValWsp);
  
    i8   iPop(void);
   
    //-------------------------------------------------------------------------
    
    void   vDiscard(void);
    void   vContentDump(Str* aPrefix, Str* aTitle);
    Value* pPeek(void);
    Value* pPeek(i8 iNthValue);
    void   vPeekValWsp(i8 iNthValue, ValWsp& xValWsp);
#ifndef OLD_VALWSP
    void   vPeek(i8 iNthValue, ValWsp& xValWsp);
#endif
  
    void   vPoke(i8 iNthValue, Value* pValue);
    bool   bPeekZero(void);
    void   vRetry(void);
    void   vRetryPoint(void);
    void   vReset(void);
    i8     iValues(void);
    
    Value* pTopValue(void);
    
    Value* pGetValue(i8 iValue);
    
  private:
    void   vPushBy(i8 iBytes);
    void   vPopBy(i8 iBytes);
    void   vPopVal(Value* pValue);
    void   vPushVal(Value* pValue);
    Value* pStackBase(void);
    Value* pStackLimit(void);
    
    Footer* pFooter(void);
    Footer* pHazard(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
