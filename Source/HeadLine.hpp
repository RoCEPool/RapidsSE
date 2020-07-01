/*============================================================================*/
/* HeadLine.hpp -- a head line                                                */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  static const i8 C_HEADLINE_OFS_WORDS = 0;
  static const i8 C_HEADLINE_OFS_PERSPECTIVE = 1;
  static const i8 C_HEADLINE_DELETED = 127;
  
  class HeadLine : public Line
  {
  public:
    byte m_yaByte[sizeof(SingleWord)];

  public:
    HeadLine(void);
    ~HeadLine(void) noexcept(false);
  
    byte* ayAdrLogical(i8 i_Offset);
    i8   nBytes(void);
    void vDelete(void);
    bool bIsDeleted(void);
    void vDump(void);
    i8   nFreeSpace(StreamDef* pStreamDef);
    eMC  eGetEncoding(Column* pCol,  i8& i_ColCrs_, i8& i_OfsCrs_,
                      StreamDef* pStreamDef);
    w8   wGetLogical(i8 i_Offset, i8 nDataSize);
    OverLine* aGetOver(void);
    i8   iGetTailRel(TailLine* TailLine);
    TailLine* aGetTailRel(i8 i_Tail_rel);
    bool bHasOverflow(void);
    void vHasOverflow(void);
    void vMetaCode(i8 i_Col, eMC eMetaCode);
    i8   iOverflow2B(void);
    void vOverflow2B(i8 iOver2B);
    t8   tPerspective(void);
    void vPerspective(t8 tPerspective);
    void vPutLogical(i8 i_Offset, w8 wDataCode, i8 nDataSize);
    void vPutLogical(i8 i_Offset, Stz* szString, i8 nLength);
    void vSetOver(OverLine* aOverLine);
    i8   nUnits(void);
    i8   nW(void);
    void vW(i8 nWords);
    i8   nWords(void);

    // inline functions
  
    /*------------------------------------------------------------------------*/
    /* calculate address of meta area of head line                            */
    /*------------------------------------------------------------------------*/
    inline eMC* aeMetaArea(void)
    {
      i8 i_BaseOffsetMeta = RowCoder::iBaseOffsetMeta();

      eMC* aeCode = aAdd<eMC*>(this,i_BaseOffsetMeta);
  
      return aeCode;
    }
  
    /*------------------------------------------------------------------------*/
    /* return meta code                                                       */
    /*------------------------------------------------------------------------*/
    inline eMC eMetaCode(i8 i_Col)
    {
      i8 i_BaseOffsetMeta = RowCoder::iBaseOffsetMeta();
    
      eMC eCode = *aAdd<eMC*>(this,i_BaseOffsetMeta + i_Col);
  
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
