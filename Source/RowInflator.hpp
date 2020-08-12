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
  
    static void vInflateColI(i8** aaiDstk, HeadLine* aHeadLine, i8 i_ColToRetrieve,
                             StreamDef* pStreamDef);

   static void vInflateColI_stk(i8** aaiDstk, HeadLine* aHeadLine, i8 i_ColToRetrieve);

    static void vInflateColumn(RowSingle* pRow, HeadLine* aHeadLine, i8 i_ColToRetrieve);
    static void vInflateColumns(RowSlab* pRsbSlab, HeadLine* aHeadLine);
    
    static void vInflateRow(RowSingle* pRow, HeadLine* aHeadLine);
    static void vInflateRow(RowSlab* pRsbSlab, HeadLine* aHeadLine);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
