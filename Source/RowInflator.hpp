/*============================================================================*/
/* RowInflator.hpp -- Row Inflator Mark V                                     */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  class RowInflator : private RowCoder
  {
  public:
    //! NOTE: KEEP BOTH INFLATES UP-TO-DATE
    //! Any change in any one of the functions needs to be made in the others.
    //! Kept separate for performance reasons
    //! Highly performance-critical functions

  
    static void vInflateSet(i8** aaiDstk, HeadLine* aHeadLine,
                                StreamDef* pStreamDef);
    static void vInflateColumn(RowSingle* pRow, HeadLine* aHeadLine,
                               i8 i_ColToRetrieve);
    static void vInflateRow(RowSingle* pRow, HeadLine* aHeadLine);
    static void vInflateRow(RowSlab* pRsbSlab, HeadLine* aHeadLine);

  private:
  
    // v1 meta code
    static void vInflateSet_v1(i8** aaiDstk, HeadLine* aHeadLine,
                                StreamDef* pStreamDef);
    static void vInflateColumn_v1(RowSingle* pRow, HeadLine* aHeadLine,
                                  i8 i_ColToRetrieve);
    static void vInflateRow_v1(RowSingle* pRow, HeadLine* aHeadLine);
    static void vInflateRow_v1(RowSlab* pRsbSlab, HeadLine* aHeadLine);
  
  
    // v2 meta clue
  
    static void vInflate_v2(byte*& rayDstByte,
                            byte*& rayDataByte, HeadLine* aHeadLine,
                            u1 uHigh, u1 uLow,
                            eSI eStrInflate, char*& racString);
  
    static void vSkip_v2(u1 uHigh, u1 uLow, byte*& rayDataByte);

    static void vPrepare_v2(u1*& rauMetaClue, byte*& rayDataByte,
                            i8 nCols, HeadLine* aHeadLine);

    static void vInflateSet_v2(i8** aaiDstk, HeadLine* aHeadLine,
                               StreamDef* pStreamDef);
    static void vInflateColumn_v2(RowSingle* pRow, HeadLine* aHeadLine,
                                  i8 i_ColToRetrieve);
    static void vInflateRow_v2(RowSingle* pRow, HeadLine* aHeadLine);
    static void vInflateRow_v2(RowSlab* pRsbSlab, HeadLine* aHeadLine);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
