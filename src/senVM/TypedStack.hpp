/*============================================================================*/
/* TypedStack.hpp -- Push/Pop TypedStack of Typed Cells                                 */
/*============================================================================*/
#pragma once

#include "eKK.h"

namespace SenVM
{
  static const i8 C_NO_ADDITIONAL_BYTES = 0;
  
  //static const i8 C_WSP_STACK = C_SIZE_LARGE_OBJECT - C_1KB;
  static const i8 C_WSP_STACK = C_SIZE_MEDIUM_OBJECT - 256;

  class StkTop
  {
  public:
    union {
      byte* m_ayStackPtr;
      TypedCell* m_aTypedCell;
    };
  
    i8   m_nItems; // number of items currently on stack

    void vAdvance (i8 nBytes) { m_ayStackPtr -= nBytes; m_nItems++;   }
    void vRewind  (i8 nBytes) { m_ayStackPtr += nBytes; m_nItems--;   }
    void vRewind  (void)      { vRewind(m_aTypedCell->nStackUsage()); }
  };

  class TypedStack: public Object
  {
  private:
    eKK     m_eKind;  // kind of stack
    StkTop  m_rTop;   // current top of stack
    StkTop  m_rRetry; // retry point of stack
    
    Footer  m_xHazard;
    Wsp_t<C_WSP_STACK> m_xWsp;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TypedStack(eKK eKind);
   ~TypedStack(void) noexcept(false);
    
    static TypedStack* _pStack(void* aAddr);
  
    void vDiscardTop(void);
  
    void vDump(DicStack* pDicStack, Stz* szTitle);

    TypedCell* aPeekItem(i8 i_Item);
    void vPeekItem(i8 i_Item, TypedCell* aTypedCell);

    void vPokeItem(i8 i_Item, TypedCell* aTypedCell);
  
    i8   iPopAsInt(const char* azCallingFunNm);
    ForthCell* aPopFip(const char* azCallingFunNm);
    i8   iPopInt(const char* azCallingFunNm);
    Object* pPopPtr(const char* azCallingFunNm);
    void vPopStr(String* pStr, const char* azCallingFunNm);
    void vPopItem(TypedCell* aTypedCell, const char* azCallingFunNm);

    void vPushFip(ForthCell* aFip);
    void vPushInt(i8 iInt);
    void vPushPtr(Object* pObj);
    void vPushStr(String* pStr);
    void vPushStz(Stz* szStz);
    void vPushItem(TypedCell* aTypedCell);
  
    void vReset(void);
    void vRetry(void);
    void vRetryPoint(void);
  
    bool bTopIsZero(void);
    TypedCell* aTopVal(void);
  
    i8   nValues(void);
  
  private:
    TypedCell* aAllotTypedCell(eCN eType, i8 nWithAdditionalBytes);
    TypedCell* aAllotFipVal(void);
    TypedCell* aAllotIntVal(void);
    TypedCell* aAllotPtrVal(void);
    TypedCell* aAllotStzVal(i8 nStrSize);
  
    bool bGuard(i8 nValues);
    bool bGuard(eCN eClass0);
    bool bGuard(eCN eClass0, eCN eClass1);
    bool bGuard(eCN eClass0, eCN eClass1, eCN eClass2);
  
    byte* ayStackBase(void);
    byte* ayStackLimit(void);
    
    Footer* aFooter(void);
    Footer* aHazard(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
