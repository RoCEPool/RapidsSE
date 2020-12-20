/*============================================================================*/
/* TypedCell.hpp -- Self-Describing / Dynamically Typed Cell                  */
/*============================================================================*/
#pragma once

namespace Resources
{
  class TypedCell
  {
  public:
  
    //Dtm  m_wxTypeCell;
    _Header m_wxType;
    Dtm     m_wxValueCell;
  
    char m_caStringArea[C_MAX_VCH];

  public:
    TypedCell(void);
   ~TypedCell(void) noexcept(false);
  
    i8 iAsInt(void);

    char* azChar(void);
  
    void vCopy(TypedCell* aTypedCell, i8 nBytes);

    ForthCell* aFip(void);
    void vFip(ForthCell* aFip);

    void vInit(void);

    i8    iInt(void);
    void  vInt(i8 iVal);

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
