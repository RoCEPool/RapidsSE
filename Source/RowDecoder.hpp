/*============================================================================*/
/* RowDecoder.hpp -- Row Decoder Mark V                                       */
/*============================================================================*/
#pragma once

namespace Rows
{
  class RowDecoder : private RowCoder
  {
  public:
  
    static void vDecode(DtmVal* pDtmVal, HeadLine* aHeadLine,
                        ColumnVec* pColVec, Column* pCol,
                        i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeBcd(DtmVal* pDtmVal, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeBol(DtmVal* pDtmVal, HeadLine* aHeadLine,Column* pCol);
    static void vDecodeDte(DtmVal* pDtmVal, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeFlt(DtmVal* pDtmVal, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeInt(DtmVal* pDtmVal, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeIvl(DtmVal* pDtmVal, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeTme(DtmVal* pDtmVal, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeTsp(DtmVal* pDtmVal, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeVch(DtmVal* pDtmVal, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_ColCrs_, i8& i_OfsCrs_);
    static void vDecodeVchLong(DtmVal* pDtmVal, HeadLine* aHeadLine,
                                Column* pCol, i8 i_Offset);
    static void vDecodeVchShocoFit(DtmVal* pDtmVal, HeadLine* aHeadLine,
                                   Column* pCol, i8 i_Offset);
    static void vDecodeVchShocoSplit(DtmVal* pDtmVal, HeadLine* aHeadLine,
                                     Column* pCol, i8 i_Offset);
    static void vDecodeVchShocoTail(DtmVal* pDtmVal, HeadLine* aHeadLine,
                                    Column* pCol, i8 i_Offset);
    static void vDecodeVchStringTail(DtmVal* pDtmVal, HeadLine* aHeadLine,
                                     Column* pCol, i8 i_Offset);
    static void vDecodeVchSmall(DtmVal* pDtmVal, HeadLine* aHeadLine,
                                Column* pCol, i8 i_Offset);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
