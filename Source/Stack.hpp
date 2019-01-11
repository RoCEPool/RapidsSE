/*============================================================================*/
/* Stack.hpp -- Push/Pop Stack of Values                                      */
/*============================================================================*/
#pragma once

// forward declarations
namespace Tasks { class Task; }
namespace Steps { class Step; }
namespace DBobjects { class Entry; }

//------------------------------------------------------------------------------

namespace Forth
{
  static const i8 C_WSP_STACK = C_SIZE_LARGE_OBJECT - C_1KB;
  //static const i8 C_WSP_STACK = C_SIZE_SMALL_OBJECT;

  class Stack: public Object
  {
  friend class Tasks::Task;
  friend class Steps::Step;

  private:
  
  private:
    Value* m_aTop;    // current top of stack
    i8     m_nValues; // number of values currently on stack

    Value* m_aRetryTop;    // reset to this top of stack
    i8     m_nRetryValues; // reset with this many values on stack
    
    Footer m_xHazard;
    
    WspFxd<C_WSP_STACK> m_xWsp;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Stack(void);
    ~Stack(void) noexcept(false);
    
    static Stack* _pStack(void* aAddr);
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vContentDump(Stz* aPrefix, Stz* aTitle);
  
    void vDiscard(void);
  
    bool bGuard(i8 iCount);
    bool bGuard(eCN eClass0);
    bool bGuard(eCN eClass0, eCN eClass1);
    bool bGuard(eCN eClass0, eCN eClass1, eCN eClass2);
    
    Value* aPeekValue(void);
    Value* aPeekValue(i8 iNthValue);
    void vPeekValue(i8 iNthValue, GenVar& xVariable);
    bool bPeekZero(void);
  
    bool bPop(Cell*& pCell);
    bool bPop(eCN& eArg1);
    bool bPop(GenVar* pArg1);
    bool bPop(GenVar& xArg1);
    bool bPop(GenVar& xArg1, GenVar& xArg2);
    bool bPop(GenVar& xArg1, GenVar& xArg2, GenVar& xArg3);
    bool bPop(i8& iArg1);
    bool bPop(i8& iArg1, i8& iArg2);
    bool bPop(i8& iArg1, Object*& pObject);
    bool bPop(i8& iArg1, GenVar& xArg1);
    bool bPop(i8& iArg1, s48K& sArg2);
    bool bPop(i8& iArg1, i8& iArg2, Value& xArg3);
    bool bPop(Object*& pObject);
    bool bPop(Object*& pObject, i8& iArg);
    bool bPop(Object*& pObject1, Object*& pObject2);
    bool bPop(Object*& pObject, GenVar& xVariable);
    bool bPop(Register*& pRegister);
    bool bPop(s32&  sArg1);
    bool bPop(s256& sArg1);
    bool bPop(s4K&  sArg1);
    bool bPop(s16KB& sArg1);
    bool bPop(s48K& sArg1);
    bool bPop(s48K& sArg1, i8& iArg2);
    bool bPop(s48K& sArg1, i8& iArg2, i8& iArg3);
    bool bPop(s48K& sArg1, s48K& sArg2);
    bool bPopAsInt(i8& iArg1);
    bool bPopAsI8_Inline(i8& iArg1);
    bool bPopAsInt_Quick(i8& iArg1);
    i8   iPop(void);
  
    void  vPokeValWrk(i8 iNthValue, GenVar& xVariable);
  
    void vPushBool(bool bBool);
    void vPushCell(Cell* pCell);
    void vPushEntry(DBobjects::Entry* aEntry);
    void vPushGenVar(GenVar* pVariable);
    void vPushGenVar(GenVar& xVariable);
    void vPushI8(i8 iInt);
    void vPushPointer(Object* pObject);
    void vPushRegister(Register* pValue);
    void vPushS32(s32& s32);
    void vPushS256(s256& s256);
    void vPushS4K(s4K& s4K);
    void vPushS16KB(s16KB& s16KB);
    void vPushS48K(s48K& s48K);
    void vPushValue(Value* aValue);
    void vPushValue(Value& xValue);
  
    void vRetry(void);
    void vRetryPoint(void);
  
    i8 nValues(void);
  
  public:
    void vPopBy(i8 nBytes);
  private:
    void vPopVal(Value* aValue);
    void vPushBy(i8 nBytes);
    void vPushVal(Value* aValue);
    Value* aStackBase(void);
    Value* aStackLimit(void);
    
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
    
    m_aTop = reinterpret_cast<Value*>(aTop);
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
    iArg = _pIntVal(m_aTop)->iInt(); // inline
  else
    if (CN_PTRVAL == eClassNo)
      iArg = _pPtrVal(m_aTop)->pPtr() ? 1 : 0; // inline
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
