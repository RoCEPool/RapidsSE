/*============================================================================*/
/* TypedCell.hpp -- Self-Describing / Dynamically Typed Cell                  */
/*============================================================================*/
#pragma once

namespace BASE
{
  class TypedCell
  {
  public:
  
    //Dtm  m_wxTypeCell;
    _Header m_wxType;
    Dtm     m_wxValueCell;
  
    char m_caStringArea[C_16KB];

  public:
    TypedCell(void);
   ~TypedCell(void) noexcept(false);
  
    i8 iAsInt(void);

    i8    iBcd(void);
    void  vBcd(i8 iBcd);
  
    i8    iBol(void);
    void  vBol(i8 iBol);

    char* azChar(void);
  
    void vCopy(TypedCell* aTypedCell, i8 nBytes=0);

    CodeCell* aFip(void);
    void vFip(CodeCell* aFip);

    d8    dFlt(void);
    void  vFlt(d8 dVal);
  
    void vInit(void);

    i8    iInt(void);
    void  vInt(i8 iVal);

    i8    iIntBol(void);
  
    void  vMagic(void);
  
    Object* pObj(void);
    void    vObj(Object* pObject);

    i8    nStackUsage(void);
    Stz*  szStz(void);
    void  vStz(Stz* szStz);
    //void  vStz(char* anChar, i8 nLength);
    i8    nStzSize(void);
    void  vStzSize(i8 nSize);
  
    void  vToStz(Stz* szStz);
  
    eCN  eType(void);
    void vType(eCN eType);
  };

  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline TypedCell* _aTypedCell(void* aAddr)
  {
  TypedCell* aTypedCell = reinterpret_cast<TypedCell*>(aAddr);

    return aTypedCell;
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
