/*============================================================================*/
/* Stack.hpp -- Push/Pop Stack of Values                                      */
/*============================================================================*/
#pragma once

#include "eKK.h"

namespace SenVM
{
  static const i8 C_NO_ADDITIONAL_BYTES = 0;
  
  //static const i8 C_WSP_STACK = C_SIZE_LARGE_OBJECT - C_1KB;
  static const i8 C_WSP_STACK = C_SIZE_MEDIUM_OBJECT - 256;

  union StkPtr {
    
    byte*   ayByte;
    
    AnyVal* aAnyVal;
 
    TypedCell* aTypedCell;
    
    void vAdvance (i8 nBytes) { ayByte -= nBytes; }
    void vRewind  (i8 nBytes) { ayByte += nBytes; }
    void vRewind  (void)      { vRewind(aAnyVal->nStackUsage()); }
  };

  class Stack: public Object
  {
  private:
    eKK     m_eKind;        // kind of stack
  
    StkPtr  m_xTop;         // current top of stack
    i8      m_nValues;      // number of values currently on stack

    AnyVal* m_aRetryTop;    // reset to this top of stack
    i8      m_nRetryValues; // reset with this many values on stack
    
    Footer  m_xHazard;
    
    Wsp_t<C_WSP_STACK> m_xWsp;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Stack(eKK eKind);
   ~Stack(void) noexcept(false);
    
    static Stack* _pStack(void* aAddr);
  
    void vDiscardTop(void);
  
    void vDump(DicStack* pDicStack, Stz* szTitle);

    AnyVal* aPeekVal(i8 i_Val);
    void vPeekVal(i8 i_Val, AnyVal* aAnyVal);

    void vPokeVal(i8 i_Val, AnyVal* aAnyVal);
  
    i8   iPopAsInt(const char* azFunNm);
    ForthCell* aPopFip(const char* azFunNm);
    i8   iPopInt(const char* azFunNm);
    Object* pPopPtr(const char* azFunNm);
    void vPopStr(String* pStr, const char* azFunNm);
    void vPopVal(AnyVal* aAnyVal, const char* azFunNm);

    void vPushFip(ForthCell* aFip);
    void vPushInt(i8 iInt);
    void vPushPtr(Object* pObj);
    void vPushStr(String* pStr);
    void vPushStz(Stz* szStz);
    void vPushVal(AnyVal* aAnyVal);
  
    void vReset(void);
    void vRetry(void);
    void vRetryPoint(void);
  
    bool bTopIsZero(void);
    AnyVal* aTopVal(void);
  
    i8   nValues(void);
  
  private:
    AnyVal* aAllotAnyVal(i8 nWithAdditionalBytes);
    TypedCell* aAllotFipVal(void);
    TypedCell* aAllotIntVal(void);
    TypedCell* aAllotPtrVal(void);
    TypedCell* aAllotStzVal(i8 nStrSize);
  
    bool bGuard(i8 nValues);
    bool bGuard(eCN eClass0);
    bool bGuard(eCN eClass0, eCN eClass1);
    bool bGuard(eCN eClass0, eCN eClass1, eCN eClass2);
  
    AnyVal* aStackBase(void);
    AnyVal* aStackLimit(void);
    
    Footer* aFooter(void);
    Footer* aHazard(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
