/*============================================================================*/
/* HeadLine.hpp -- a head line                                                */
/*============================================================================*/
#pragma once

namespace Data
{
  class Datum;
  
  static const i8 C_HEAD_PRE_BYTES = 1;

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
  
  /*----------------------------------------------------------------------------*/
  /* return meta code                                                           */
  /*----------------------------------------------------------------------------*/
  inline eMC eMetaCode(i8 i_Meta)
  {
    eMC eCode = *(eMC*)((byte*)this + C_HEAD_PRE_BYTES + i_Meta);
  
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
