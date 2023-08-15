/*============================================================================*/
/* TypeValueArea.hpp -- Self-Describing / Dynamically Typed Value + Area      */
/*============================================================================*/
#pragma once

#include "TypeValue.hpp"

namespace BASE
{
  class TypeValueArea : public TypeValue
  {
  public:
    
    char m_caStringArea[C_16KB];

  public:
    TypeValueArea(void);
   ~TypeValueArea(void) noexcept(false);
  
    i8    iAsInt(void);

    i8    iBcd(void);
    void  vBcd(i8 iBcd);
  
    i8    iBol(void);
    void  vBol(i8 iBol);

    char* azChar(void);
  
    void  vCopy(TypeValueArea* aTypedCell, i8 nBytes=0);

    CodeCell* aFip(void);
    void  vFip(CodeCell* aFip);

    d8    dFlt(void);
    void  vFlt(d8 dVal);
  
    void  vInit(void);

    i8    iInt(void);
    void  vInt(i8 iVal);
  
    void  vMagic(void);
  
    Object* pObj(void);
    void  vObj(Object* pObject);

    i8    nStackUsage(void);
  
    Stz*  szStz(void);
    void  vStz(Stz* szStz);
    i8    nStzSize(void);
    void  vStzSize(i8 nSize);
  
    void  vToStr(String* psDst);
  
    eCN   eType(void);
    void  vType(eCN eType);
  
    TypeValue* aTypeValue(void);
  };

  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline TypeValueArea* _aTypeValueArea(void* aAddr)
  {
    TypeValueArea* aTypedValueArea = reinterpret_cast<TypeValueArea*>(aAddr);

    return aTypedValueArea;
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
