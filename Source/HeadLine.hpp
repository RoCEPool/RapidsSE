/*============================================================================*/
/* HeadLine.hpp -- a head line                                                */
/*============================================================================*/
#pragma once

namespace Data
{
  class DtmVal;
  
  class HeadLine : public Line
  {
  public:
    byte m_yaByte[sizeof(Word)];

  public:
    HeadLine(void);
    ~HeadLine(void) noexcept(false);
  
    static void vDumpHead(Stz* szTitle, HeadLine* aHeadLine, ColumnVec* pColVec);
    static void vDumpHead_IV(Stz* szTitle, HeadLine* aHeadLine, ColumnVec* pColVec);

    void vMetaCode(i8 i_Meta, w8 wArg);
    
    i8   nBytes(void);
    i8   nUnits(void);
    i8   nWords(void);

    w8   wGetLogical(i8 i_Offset, i8 nDataSize);
    bool bHasOverflow(void);
    void vHasOverflow(void);
    bool bIsLargeHead(void);
    i8   iOverflow2B(void);
    void vOverflow2B(i8 iOver2B);
    void vPutLogical(i8 i_Offset, w8 wDataCode, i8 nDataSize);

    i8   iW(void);
    void vW(i8 nWords);

    /*------------------------------------------------------------------------*/
    /* return meta code                                                       */
    /*------------------------------------------------------------------------*/
    inline eMC eMetaCode(i8 i_Meta)
    {
      eMC eCode = *(eMC*)((byte*)this + C_W_1B + i_Meta);
  
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
