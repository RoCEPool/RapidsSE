/*============================================================================*/
/* RowCoder.hpp -- Row Coder Mark IV                                          */
/*============================================================================*/
#pragma once

namespace Data
{
  static const i8 C_METABITS = 8;
  
  static const i8 _1B_C   = 1;  // <C>  1B no. of compressed bytes
  static const i8 _1B_L   = 1;  // <L>  1B string length in bytes
  static const i8 _1B_M   = 1;  // <M>  1B overflowed metacode
  
  static const i8 _2B_FT  = 2;  // |FT| absolute from-tail i_Line
  static const i8 _2B_IT  = 2;  // [IT] relative into-tail i_Line
  static const i8 _2B_OT  = 2;  // [OT] relative over-tail i_Line
  
  static const i8 _3B_DSR = 3;  // [DSR] relative distance to shoco
  
  static const i8 C_OFFSET_UNITS = 0;
  static const i8 C_OFFSET_COMPRESSED = C_OFFSET_UNITS + C_U_1B;

  static const i8 C_HEAD_MIN_BYTES = 24;
  
  /*--------------------------------------------------------------------------*/

  class RowCoder
  {
  public:
    static byte* ayAdrLogical(HeadLine* aHeadLine, i8 i_Offset);
  
    static i8   iBaseOffset(ColumnVec* pColVec);
  
    static i8   nDataBytes(eMC eMetaCode);
    static void vDteFromEdt(Dtm* aDte, i8 iEra);
    static void vDteIntoEdt(Dtm wDte, Dtm& wEdt_, i8& iEra_);

    static i8   nFreeSpaceHead(HeadLine* aHeadLine, ColumnVec* pColVec);
  
    static eMC  eGetEncoding(HeadLine* aHeadLine, Column* pCol,
                              ColumnVec* pColVec,
                              i8& i_PriorColumn_, i8& i_Offset_);

    static w8   wGetLogical(HeadLine* aHeadLine, i8 i_Offset, i8 nDataSize);
    static OverLine* aGetOver(HeadLine* aHeadLine);
    static OverLine* aGetOverRel(HeadLine* aHeadLine, i8 i_Over_rel);
    static TailLine* aGetTailRel(HeadLine* aHeadLine, i8 i_Tail_rel);
    static i8   nHeadSize(ColumnVec* pColVec);
    static bool bIsFit(eMC eMetaCode);
    static i8   nMetaSize(ColumnVec* pColVec);
    static byte* ayShoco(HeadLine* aHeadLine, i8 iBaseOffset,
                         ColumnVec* pColVec, Column* pCol);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
