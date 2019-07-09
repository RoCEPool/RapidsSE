/*============================================================================*/
/* RowInflator.hpp -- Row Inflator Mark V                                     */
/*============================================================================*/
#pragma once

namespace Rows
{
  class RowInflator : private RowCoder
  {
  public:
    //! NOTE: KEEP ALL THREE INFLATES UP-TO-DATE
    //! Any change in any one of the functions needs to be made in the others.
    //! Kept separate for performance reasons
    //! Highly performance-critical functions
  
    static void vInflateColumn(Row* pRow, HeadLine* aHeadLine, i8 i_ColToRetrieve);
  
    static void vInflateColumns(Row* pRow, HeadLine* aHeadLine);
  
    static void vInflateRow(Row* pRow, HeadLine* aHeadLine);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
