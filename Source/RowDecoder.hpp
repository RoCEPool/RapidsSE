/*============================================================================*/
/* RowDecoder.hpp -- Row Decoder Mark V                                       */
/*============================================================================*/
#pragma once

namespace Rows
{
  class RowDecoder : private RowCoder
  {
  public:
  
    static void vDecode(Datum* pDtmVal, HeadLine* aHeadLine,
                        i8 nColumns, Column* pCol,
                        i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeBcd(Datum* pDtmVal, HeadLine* aHeadLine,
                           i8 nColumns, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeBol(Datum* pDtmVal, HeadLine* aHeadLine,Column* pCol);
    static void vDecodeDte(Datum* pDtmVal, HeadLine* aHeadLine,
                           i8 nColumns, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeFlt(Datum* pDtmVal, HeadLine* aHeadLine,
                           i8 nColumns, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeInt(Datum* pDtmVal, HeadLine* aHeadLine,
                           i8 nColumns, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeIvl(Datum* pDtmVal, HeadLine* aHeadLine,
                           i8 nColumns, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeTme(Datum* pDtmVal, HeadLine* aHeadLine,
                           i8 nColumns, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeTsp(Datum* pDtmVal, HeadLine* aHeadLine,
                           i8 nColumns, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeVch(Datum* pDtmVal, HeadLine* aHeadLine,
                           i8 nColumns, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeVchLong(Datum* pDtmVal, HeadLine* aHeadLine,
                                Column* pCol, i8 i_Offset);
    static void vDecodeVchShocoFit(Datum* pDtmVal, HeadLine* aHeadLine,
                                   Column* pCol, i8 i_Offset);
    static void vDecodeVchShocoSplit(Datum* pDtmVal, HeadLine* aHeadLine,
                                     Column* pCol, i8 i_Offset);
    static void vDecodeVchShocoTail(Datum* pDtmVal, HeadLine* aHeadLine,
                                    Column* pCol, i8 i_Offset);
    static void vDecodeVchStringTail(Datum* pDtmVal, HeadLine* aHeadLine,
                                     Column* pCol, i8 i_Offset);
    static void vDecodeVchSmall(Datum* pDtmVal, HeadLine* aHeadLine,
                                Column* pCol, i8 i_Offset);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
