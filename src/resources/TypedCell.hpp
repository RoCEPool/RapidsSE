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
  
    eCN  eType(void);
    void vType(eCN eType);
  
    void  vInt(i8 iVal);
    char* azChar(void);
    i8    nStackUsage(void);
    Stz*  szStz(void);
    void  vStz(Stz* szStz);
    //void  vStz(char* anChar, i8 nLength);
    i8    nStzSize(void);
    void  vStzSize(i8 nSize);
    void  vToStz(Stz* szStz);
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
