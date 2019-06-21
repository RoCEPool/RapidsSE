/*============================================================================*/
/* RowCoder.hpp -- Row Coder Mark IV                                          */
/*============================================================================*/
#pragma once

namespace Data
{
  class RowCoder
  {
  public:
    static i8   iBaseOffset(ColumnVec* pColVec);
  
    static i8   nDataBytes(eMC eMetaCode);
    static void vDteFromEdt(Datum* pDatum, i8 iEra);
    static void vDteFromEdt(Word* aWord, i8 iEra);
    static void vDteIntoEdt(Dtm wDte, Dtm& wEdt_, i8& iEra_);

    static byte* ayAdrLogical(HeadLine* aHeadLine, i8 i_Offset);
    static w8    wGetLogical(HeadLine* aHeadLine, i8 i_Offset, i8 nBytes);
    static void  vPutLogical(HeadLine* aHeadLine, i8 i_Offset, i8 nBytes, w8 wBytes);

    static eMC   eGetEncoding(HeadLine* aHeadLine, Column* pCol,
                              ColumnVec* pColVec,
                              i8& i_PriorColumn_, i8& i_Offset_);

   static OverLine* aGetOver(HeadLine* aHeadLine);
   static TailLine* aGetTailRel(HeadLine* aHeadLine, i8 i_Tail_rel);

    static bool bIsFit(eMC eMetaCode);
  
  static i8 nMetaData(ColumnVec* pColVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
