/*============================================================================*/
/* Stack.hpp -- Push/Pop Stack of Values                                      */
/*============================================================================*/
#pragma once

namespace SenVM
{
  static const i8 C_WSP_STACK = C_SIZE_LARGE_OBJECT - C_1KB;
  //static const i8 C_WSP_STACK = C_SIZE_SMALL_OBJECT;

  union StkPtr {
    
    StkVal* pStkVal;
    
    FipVal* pFipVal;
    IntVal* pIntVal;
    PtrVal* pPtrVal;
    StrVal* pStrVal;
  };

  class Stack: public Object
  {
  friend class Tasks::Task;
  friend class Steps::Step;

  private:
    eKK     m_eKind;        // kind of stack
  
    StkPtr  m_xTop;         // current top of stack
    i8      m_nValues;      // number of values currently on stack

    StkVal* m_aRetryTop;    // reset to this top of stack
    i8      m_nRetryValues; // reset with this many values on stack
    
    Footer m_xHazard;
    
    Wsp_t<C_WSP_STACK> m_xWsp;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Stack(eKK eKind);
   ~Stack(void) noexcept(false);
    
    static Stack* _pStack(void* aAddr);
    static void vDump(eCN eClass, Object* pObj, Stz* aTab);
  
    void vContentDump(DicStack* pDicStack, Stz* szTitle);
  
    bool bGuard(i8 nValues);
    bool bGuard(eCN eClass0);
    bool bGuard(eCN eClass0, eCN eClass1);
    bool bGuard(eCN eClass0, eCN eClass1, eCN eClass2);
    
    StkVal* aPeekVal(i8 i_Val);
    void vPeekVar(i8 i_Val, GenVar& xVar);
  
    void vPokeVar(i8 i_Val, GenVar& xVar);
  
    bool bPopAsInt(i8& iInt1);
    bool bPopAsInt_Inline(i8& iInt1);
    bool bPopAsInt_Quick(i8& iInt1);
    bool bPopCN(eCN& eClassNo);
    bool bPopFip(Cell*& aFip);
    bool bPopInt(i8& iInt);
    bool bPopInt(i8& iInt1, i8& iInt2);
    bool bPopInt(i8& iInt1, i8& iInt2, GenVar& xVar);
    bool bPopInt(i8& iInt, Object*& pObj);
    bool bPopInt(i8& iInt, GenVar& xVar);
    bool bPopInt(i8& iInt, s48K& sEss);
    bool bPopPtr(Object*& pObj);
    bool bPopPtr(Object*& pObj, i8& iInt);
    bool bPopPtr(Object*& pObj1, Object*& pObj2);
    bool bPopPtr(Object*& pObj, GenVar& xVar);
    bool bPopLocal(Local*& pLocal);
    bool bPopS32(s32&  sEss1);
    bool bPopS256(s256& sEss1);
    bool bPopS4K(s4K&  sEss1);
    bool bPopS48K(s48K& sEss1);
    bool bPopS48K(s48K& sEss1, i8& iInt2);
    bool bPopS48K(s48K& sEss1, i8& iInt2, i8& iInt3);
    bool bPopS48K(s48K& sEss1, s48K& sEss2);
    bool bPopStr(String* pStr);
    bool bPopVar(GenVar* pVar);
    bool bPopVar(GenVar& xVar);
    bool bPopVar(GenVar& xVar1, GenVar& xVar2);
    bool bPopVar(GenVar& xVar1, GenVar& xVar2, GenVar& xVar3);
  
    void vPushFip(Cell* aCell);
    void vPushGenVar(GenVar* pVar);
    void vPushGenVar(GenVar& xVar);
    void vPushInt(i8 iInt);
    void vPushPtr(Object* pObj);
    void vPushLocal(Local* pLocal);
    void vPushS32(s32& sEss);
    void vPushS256(s256& sEss);
    void vPushS4K(s4K& sEss);
    void vPushS16KB(s16KB& sEss);
    void vPushS48K(s48K& sEss);
    void vPushStkVal(StkVal* aStkVal);
    void vPushStkVal(StkVal& xStkVal);
  
    void vReset(void);
    void vRetry(void);
    void vRetryPoint(void);
  
    StkVal* aTopStkVal(void);
  
    i8 nValues(void);
  
  public:
    void vPopBy(i8 nBytes);
  
  private:
    FipVal* aAllotFipVal(void);
    IntVal* aAllotIntVal(void);
    PtrVal* aAllotPtrVal(void);
    StkVal* aAllotStkVal(i8 nBytes);
    StrVal* aAllotStrVal(i8 nSize);
  
    void vDiscard(void);
  
    Cell*   aPopFip(void);
    i8      iPopInt(void);
    Object* pPopObj(void);

    void vPopLocal(Local* aLocal);
    void vPopVal(StkVal* aStkVal);
    void vPushBy(i8 nBytes);
    void vPushVal(StkVal* aStkVal);
  
    StkVal* aStackBase(void);
    StkVal* aStackLimit(void);
    
    bool bTopIsZero(void);
  
    Footer* aFooter(void);
    Footer* aHazard(void);
  };
  
  /*----------------------------------------------------------------------------*/
  /* pop top by given bytes                                                     */
  /*----------------------------------------------------------------------------*/
  inline void Stack::vPopBy(i8 nBytes)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
    
    char* aTop = reinterpret_cast<char*>(m_xTop.pStkVal);
    
    aTop += nBytes;
    
    m_xTop.pStkVal = reinterpret_cast<StkVal*>(aTop);
  }
  
  /*----------------------------------------------------------------------------*/
  /* pop value from data stack  as integer                                      */
  /*----------------------------------------------------------------------------*/
  inline bool Stack::bPopAsInt_Inline(i8& iInt)
  {
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    return false;
  
  eCN eClassNo = m_xTop.pStkVal->eClass(); // inline
  
  if (CN_INTVAL == eClassNo)
    iInt = _aIntVal(m_xTop.pStkVal)->iInt(); // inline
  else
    if (CN_PTRVAL == eClassNo)
      iInt = _aPtrVal(m_xTop.pStkVal)->pObj() ? 1 : 0; // inline
    else {
      iInt = -1;
      NEVER_REACH;
    }
  
  vPopBy(sizeof(IntVal)); // inline
  m_nValues--;
  
  return true;
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
