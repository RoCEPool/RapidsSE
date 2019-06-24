/*============================================================================*/
/* RowCoder.hpp -- Row Coder Mark IV                                          */
/*============================================================================*/
#pragma once

namespace Data
{
  static const i8 C_C_1B  = 1;  // <C> 1B number of compressed bytes
  static const i8 C_L_1B  = 1;  // <L> 1B string length in bytes
  static const i8 C_M_1B  = 1;  // <M> 1B overflowed metacode
  static const i8 C_U_1B  = 1;  // <U> 1B over/tail line size in units
  static const i8 C_W_1B  = 1;  // <W> 1B head line size in words

  static const i8 C_FT_2B = 2;  // |FT| 2B absolute from-tail i_Line
  static const i8 C_IT_2B = 2;  // [IT] 2B relative into-tail i_Line
  static const i8 C_OT_2B = 2;  // [OT] 2B relative over-tail i_Line
  
  static const i8 C_DSR_3B = 3; // [DSR] 3B relative distance to shoco

  static const i8 C_METABITS = 8;
  static const i8 C_HEAD_MIN_BYTES = 24;
  static const i8 C_COMPRESSED_OFFSET = C_U_1B;
  
  static const i8 C_ERA_0 =  0; // 1900
  static const i8 C_ERA_1 = 92; // 1992
  static const i8 C_ERA_2 = 99; // 1999

  /*--------------------------------------------------------------------------*/

  class RowCoder
  {
  public:
    static i8   iBaseOffset(ColumnVec* pColVec);
    static i8   nDataSize(eMC eMetaCode);
    static void vDteFromEdt(Dtm* aDte, i8 iEra);
    static void vDteIntoEdt(Dtm wDte, Dtm& wEdt_, i8& iEra_);
    static i8   nHeadSize(ColumnVec* pColVec);
    static bool bIsFit(eMC eMetaCode);
    static i8   nMetaSize(ColumnVec* pColVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
