/*============================================================================*/
/* HeadLine.hpp -- a head line                                                */
/*============================================================================*/
#pragma once

namespace Data
{
  class Datum;
  
  static const i8 C_U_codehead     = 0;
  static const i8 C_UNITS_CODEHEAD = C_MAX_RECORD / sizeof(Word);

  class HeadLine : public Line
  {
  public:
    byte m_yaByte[sizeof(Word)];

  public:
    HeadLine(void);
    ~HeadLine(void) noexcept(false);
  
    void vMetaCode(i8 i_Meta, w8 wArg);
    
    i8   nBytes(void);
    i8   nUnits(void);
    i8   nWords(void);

    bool bHasOverflow(void);
    i8   iOverflow2B(void);
    void vOverflow2B(i8 iOver2B);

  /*----------------------------------------------------------------------------*/
  /* return meta code                                                           */
  /*----------------------------------------------------------------------------*/
  inline eMC eMetaCode(i8 i_Meta)
  {
    eMC eCode = *(eMC*)((byte*)this + _1B_W + i_Meta);
  
    return eCode;
  }
  
  };

  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline HeadLine* _aHeadLine(void* aAddr)
  {
    HeadLine* aHeadLine = reinterpret_cast<HeadLine*>(aAddr);
    
    return aHeadLine;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
