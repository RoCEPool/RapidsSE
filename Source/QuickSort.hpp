/*============================================================================*/
/* QuickSortSort.hpp -- Quicksort Sort                                        */
/*============================================================================*/
#pragma once

namespace Rows
{
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class QuickSort
  {
  public:
    static i8 iPartitionBuffer(ColumnVec* pColumnVec,
                               SrtBuffer* pSrtBuffer,
                               i8 iLowIndex,
                               i8 iHighIndex);
    static void vQuickSortBuffer(ColumnVec* pColumnVec,
                                 SrtBuffer* pSrtBuffer,
                                 i8 iLowIndex,
                                 i8 iHighIndex);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
