/*============================================================================*/
/* Stack.hpp -- Push/Pop Stack of Values                                      */
/*============================================================================*/
#pragma once

namespace SenVM
{
  static const i8 C_NO_ADDITIONAL_BYTES = 0;
  
  static const i8 C_WSP_STACK = C_SIZE_LARGE_OBJECT - C_1KB;
  //static const i8 C_WSP_STACK = C_SIZE_SMALL_OBJECT;

  union StkPtr {
    
    byte*   ayByte;
    
    AnyVal* aAnyVal;
    FipVal* aFipVal;
    IntVal* aIntVal;
    PtrVal* aPtrVal;
    StrVal* aStrVal;
    
    void vAdvance (i8 nBytes) { ayByte -= nBytes; }
    void vRewind  (i8 nBytes) { ayByte += nBytes; }
    void vRewind  (void)      { vRewind(aAnyVal->nStackUsage()); }
  };

  class Stack: public Object
  {
  friend class Tasks::Task;
  friend class Steps::Step;

  private:
    eKK     m_eKind;        // kind of stack
  
    StkPtr  m_xTop;         // current top of stack
    i8      m_nValues;      // number of values currently on stack

    AnyVal* m_aRetryTop;    // reset to this top of stack
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
    
    AnyVal* aPeekVal(i8 i_Val);
    void vPeekVal(i8 i_Val, GenVar& xGenVar);
  
    void vPokeVal(i8 i_Val, AnyVal* aAnyVal);
  
    bool bPopAsInt(i8& iInt1);
    bool bPopAsInt_Inline(i8& iInt1);
    bool bPopAsInt_Quick(i8& iInt1);
    bool bPopFip(Cell*& aFip);
    bool bPopInt(i8& iInt);
    bool bPopPtr(Object*& pObj);
    bool bPopLocal(Local*& pLocal);
    bool bPopStr(String* pStr);
    bool bPopVal(GenVar& xGenVar);
  
    void vPushFip(Cell* aFip);
    void vPushInt(i8 iInt);
    void vPushPtr(Object* pObj);
    void vPushLocal(Local* pLocal);
    void vPushStr(String* pStr);
    void vPushVal(AnyVal* aAnyVal);
  
    void vReset(void);
    void vRetry(void);
    void vRetryPoint(void);
  
    AnyVal* aTopVal(void);
  
    i8 nValues(void);
  
  private:
    AnyVal* aAllotAnyVal(i8 nWithAdditionalBytes);
    FipVal* aAllotFipVal(void);
    IntVal* aAllotIntVal(void);
    PtrVal* aAllotPtrVal(void);
    StrVal* aAllotStrVal(i8 nStrSize);
  
    void    vAnyValPop(AnyVal* aAnyVal);
    void    vAnyValPush(AnyVal* aAnyVal);
  
    Cell*   aFipValPop(void);
    i8      iIntValPop(void);
    Object* pPtrValPop(void);

    void    vPopLocal(Local* aLocal);
  
    AnyVal* aStackBase(void);
    AnyVal* aStackLimit(void);
    
    void    vTopDiscard(void);
    bool    bTopIsZero(void);
  
    Footer* aFooter(void);
    Footer* aHazard(void);
  };
  
  /*----------------------------------------------------------------------------*/
  /* pop value from data stack  as integer                                      */
  /*----------------------------------------------------------------------------*/
  inline bool Stack::bPopAsInt_Inline(i8& iInt)
  {
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    return false;
  
  eCN eClassNo = m_xTop.aAnyVal->eClass(); // inline
  
  if (CN_INTVAL == eClassNo)
    iInt = _aIntVal(m_xTop.aAnyVal)->iInt(); // inline
  else
    if (CN_PTRVAL == eClassNo)
      iInt = _aPtrVal(m_xTop.aAnyVal)->pObj() ? 1 : 0; // inline
    else {
      iInt = -1;
      NEVER_REACH;
    }
  
  m_xTop.vRewind(sizeof(IntVal));
  
  m_nValues--;
  
  return true;
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
