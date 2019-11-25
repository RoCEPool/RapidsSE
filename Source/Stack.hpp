/*============================================================================*/
/* Stack.hpp -- Push/Pop Stack of Values                                      */
/*============================================================================*/
#pragma once

namespace SenVM
{
  static const i8 C_WSP_STACK = C_SIZE_LARGE_OBJECT - C_1KB;
  //static const i8 C_WSP_STACK = C_SIZE_SMALL_OBJECT;

  class Stack: public Object
  {
  friend class Tasks::Task;
  friend class Steps::Step;

  private:
    eKK     m_eKind;        // kind of stack
  
    StkVal* m_aTop;         // current top of stack
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
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    FipVal* aAllotFipVal(void);
    IntVal* aAllotIntVal(void);
    PtrVal* aAllotPtrVal(void);
    StrVal* aAllotStrVal(i8 nSize);

    void vContentDump(DicStack* pDicStack, Stz* szTitle);
  
    void vDiscard(void);
  
    bool bGuard(i8 nValues);
    bool bGuard(eCN eClass0);
    bool bGuard(eCN eClass0, eCN eClass1);
    bool bGuard(eCN eClass0, eCN eClass1, eCN eClass2);
    
    StkVal* aPeekVal(i8 i_Val);
    void vPeekValue(i8 i_Val, GenVar& xGenVar);
    bool bPeekZero(void);
  
    bool bPopCN(eCN& eClassNo);
    bool bPopFip(Cell*& aFip);
    bool bPopGenVar(GenVar* pGenVar);
    bool bPopGenVar(GenVar& xGenVar);
    bool bPopGenVar(GenVar& xVar1, GenVar& xVar2);
    bool bPopGenVar(GenVar& xVar1, GenVar& xVar2, GenVar& xVar3);
    bool bPopI8(i8& iArg);
    bool bPopI8(i8& iArg1, i8& iArg2);
    bool bPopI8(i8& iArg1, i8& iArg2, GenVar& xGenVar);
    bool bPopI8(i8& iArg, Object*& pObject);
    bool bPopI8(i8& iArg, GenVar& xGenVar);
    bool bPopI8(i8& iArg, s48K& sArg);
    bool bPopObject(Object*& pObject);
    bool bPopObject(Object*& pObject, i8& iArg);
    bool bPopObject(Object*& pObject1, Object*& pObject2);
    bool bPopObject(Object*& pObject, GenVar& xGenVar);
    bool bPopLocal(Local*& pLocal);
    bool bPopS32(s32&  sArg1);
    bool bPopS256(s256& sArg1);
    bool bPopS4K(s4K&  sArg1);
    bool bPopS48K(s48K& sArg1);
    bool bPopS48K(s48K& sArg1, i8& iArg2);
    bool bPopS48K(s48K& sArg1, i8& iArg2, i8& iArg3);
    bool bPopS48K(s48K& sArg1, s48K& sArg2);
    bool bPopString(String* pString);
    bool bPopAsInt(i8& iArg1);
    bool bPopAsI8_Inline(i8& iArg1);
    bool bPopAsInt_Quick(i8& iArg1);
    i8   iPop(void);
  
    void vPokeGenVar(i8 i_Value, GenVar& xGenVar);
  
    void vPushBool(bool bBool);
    void vPushFip(Cell* aCell);
    void vPushGenVar(GenVar* pGenVar);
    void vPushGenVar(GenVar& xGenVar);
    void vPushI8(i8 iInt);
    void vPushPointer(Object* pObject);
    void vPushLocal(Local* pLocal);
    void vPushS32(s32& s32);
    void vPushS256(s256& s256);
    void vPushS4K(s4K& s4K);
    void vPushS16KB(s16KB& s16KB);
    void vPushS48K(s48K& s48K);
    void vPushValue(StkVal* aStkVal);
    void vPushValue(StkVal& xStkVal);
  
    void vReset(void);
    void vRetry(void);
    void vRetryPoint(void);
  
    StkVal* aTopVal(void);
  
    i8 nValues(void);
  
  public:
    void vPopBy(i8 nBytes);
  
  private:
    StkVal* aAllotStkVal(i8 nBytes);
  
    void vPopLocal(Local* aLocal);
    void vPopVal(StkVal* aStkVal);
    void vPushBy(i8 nBytes);
    void vPushVal(StkVal* aStkVal);
  
    StkVal* aStackBase(void);
    StkVal* aStackLimit(void);
    
    Footer* aFooter(void);
    Footer* aHazard(void);
  };
  
  /*----------------------------------------------------------------------------*/
  /* pop top by given bytes                                                     */
  /*----------------------------------------------------------------------------*/
  inline void Stack::vPopBy(i8 nBytes)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
    
    char* aTop = reinterpret_cast<char*>(m_aTop);
    
    aTop += nBytes;
    
    m_aTop = reinterpret_cast<StkVal*>(aTop);
  }
  
  /*----------------------------------------------------------------------------*/
  /* pop value from data stack  as integer                                      */
  /*----------------------------------------------------------------------------*/
  inline bool Stack::bPopAsI8_Inline(i8& iArg)
  {
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    return false;
  
  eCN eClassNo = m_aTop->eClass(); // inline
  
  if (CN_INTVAL == eClassNo)
    iArg = _aIntVal(m_aTop)->iInt(); // inline
  else
    if (CN_PTRVAL == eClassNo)
      iArg = _aPtrVal(m_aTop)->pObj() ? 1 : 0; // inline
    else {
      iArg = -1;
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
