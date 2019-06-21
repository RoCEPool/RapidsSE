/*============================================================================*/
/* RowInflator.hpp -- Row Inflator Mark IV                                    */
/*============================================================================*/
#pragma once

namespace Data
{
  class RowInflator : private RowCoder
  {
  public:
    static void vInflate(Row* pRow, HeadLine* aHeadLine);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    static void vDecode(DtmVal* pDtmVal, HeadLine* aHeadLine,
                        ColumnVec* pColVec, Column* pCol,
                        i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeBcd(DtmVal* pDtmVal, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeBol(DtmVal* pDtmVal, HeadLine* aHeadLine,Column* pCol);
    static void vDecodeDte(DtmVal* pDtmVal, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeFlt(DtmVal* pDtmVal, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeInt(DtmVal* pDtmVal, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeIvl(DtmVal* pDtmVal, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeTme(DtmVal* pDtmVal, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeTsp(DtmVal* pDtmVal, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeVch(DtmVal* pDtmVal, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeVchShocoDup(DtmVal* pDtmVal, HeadLine* aHeadLine,
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
