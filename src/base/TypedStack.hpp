/*============================================================================*/
/* TypedStack.hpp -- Push/Pop TypedStack of Typed Cells                                 */
/*============================================================================*/
#pragma once

#include "eKK.h"

namespace BASE
{
  static const i8 C_NO_ADDITIONAL_BYTES = 0;
  
  //static const i8 C_WSP_STACK = C_SIZE_LARGE_OBJECT - C_1KB;
  static const i8 C_WSP_STACK = C_SIZE_MEDIUM_OBJECT - 256;

  class LocalVar;

  class StkTop
  {
  public:
    union {
      byte* m_ayStackPtr;
      TypeValueArea* m_aTypeValueArea;
    };
  
    i8   m_nItems; // number of items currently on stack

    void vAdvance (i8 nBytes) { m_ayStackPtr -= nBytes; m_nItems++;   }
    void vRewind  (i8 nBytes) { m_ayStackPtr += nBytes; m_nItems--;   }
    void vRewind  (void)      { vRewind(m_aTypeValueArea->nStackUsage()); }
  };

  class TypedStack: public Object
  {
  private:
    eKK     m_eKind;  // kind of stack
    StkTop  m_rTop;   // current top of stack

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

    TypeValueArea* aPeekItem(i8 i_Item);
    void vPeekTypeValueArea(i8 i_Item, TypeValueArea* aTypedCell);

    void vPokeItem(i8 i_Item, TypeValueArea* aTypedCell);
  
    i8   iPopAsInt(const char* azCallingFunNm);
    i8   iPopBcd(const char* azCallingFunNm);
    i8   iPopBol(const char* azCallingFunNm);
    CodeCell* aPopFip(const char* azCallingFunNm);
    d8   dPopFlt(const char* azCallingFunNm);
    i8   iPopInt(const char* azCallingFunNm);
    i8   iPopIntBol(const char* azCallingFunNm);
    Object* pPopPtr(const char* azCallingFunNm);
    void vPopStr(String* pStr, const char* azCallingFunNm);
    void vPopStz(Text* psText, const char* azCallingFunNm);
    void vPopSenCell(SenCell* aSenCell, const char* azCallingFunNm);
    void vPopTypeValueArea(TypeValueArea* aTypeValueArea, const char* azCallingFunNm);
    void vPopLocalVar(LocalVar* axLocalVar, const char* azCallingFunNm);

    void vPushBcd(i8 iInt);
    void vPushBol(i8 iInt);
    void vPushFip(CodeCell* aFip);
    void vPushFlt(d8 dFlt);
    void vPushInt(i8 iInt);
    void vPushPtr(Object* pObj);
    void vPushStr(String* pStr);
    void vPushStz(Stz* szStz);
    void vPushSenCell(SenCell* aSenCell);
    void vPushTypeValueArea(TypeValueArea* aSenCell);
    void vPushLocalVar(LocalVar* axLocalVar);

    void vReset(void);
  
    void vStackContents(s48K& sStackContents);

    bool bTopIsZero(void);
    TypeValueArea* aTopVal(void);
  
    void vValSprint(s48K& sFormat, s48K& sString);

    i8   nValues(void);
  
  private:
    TypeValueArea* aAllotBcdVal(void);
    TypeValueArea* aAllotBolVal(void);
    TypeValueArea* aAllotFipVal(void);
    TypeValueArea* aAllotFltVal(void);
    TypeValueArea* aAllotIntVal(void);
    TypeValueArea* aAllotPtrVal(void);
    TypeValueArea* aAllotStzVal(i8 nStrSize);

    TypeValueArea* aAllotTypedCell(eCN eType, i8 nWithAdditionalBytes);

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
