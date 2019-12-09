/*============================================================================*/
/* RowDecoder.hpp -- Row Decoder Mark V                                       */
/*============================================================================*/
#pragma once

namespace Buffers
{
  class RowDecoder : private RowCoder
  {
  public:
  
    static void vDecode(Datum* pDatum, HeadLine* aHeadLine,
                        i8 nColumns, Column* pCol,
                        i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeBcd(Datum* pDatum, HeadLine* aHeadLine,
                           i8 nColumns, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeBol(Datum* pDatum, HeadLine* aHeadLine,Column* pCol);
    static void vDecodeDte(Datum* pDatum, HeadLine* aHeadLine,
                           i8 nColumns, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeFlt(Datum* pDatum, HeadLine* aHeadLine,
                           i8 nColumns, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeInt(Datum* pDatum, HeadLine* aHeadLine,
                           i8 nColumns, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeIvl(Datum* pDatum, HeadLine* aHeadLine,
                           i8 nColumns, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeTme(Datum* pDatum, HeadLine* aHeadLine,
                           i8 nColumns, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeTsp(Datum* pDatum, HeadLine* aHeadLine,
                           i8 nColumns, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeVch(Datum* pDatum, HeadLine* aHeadLine,
                           i8 nColumns, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeVchLong(Datum* pDatum, HeadLine* aHeadLine,
                                Column* pCol, i8 i_Offset);
    static void vDecodeVchShocoFit(Datum* pDatum, HeadLine* aHeadLine,
                                   Column* pCol, i8 i_Offset);
    static void vDecodeVchShocoSplit(Datum* pDatum, HeadLine* aHeadLine,
                                     Column* pCol, i8 i_Offset);
    static void vDecodeVchShocoTail(Datum* pDatum, HeadLine* aHeadLine,
                                    Column* pCol, i8 i_Offset);
    static void vDecodeVchStringTail(Datum* pDatum, HeadLine* aHeadLine,
                                     Column* pCol, i8 i_Offset);
    static void vDecodeVchSmall(Datum* pDatum, HeadLine* aHeadLine,
                                Column* pCol, i8 i_Offset);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
