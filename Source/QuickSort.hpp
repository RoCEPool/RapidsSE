/*============================================================================*/
/* QuickSortSort.hpp -- Quicksort Sort                                        */
/*============================================================================*/
#pragma once

namespace SortBuffers
{
  class QuickSort
  {
  public:
    static i8 iPartitionBuffer(SrtBuffer* pSrtBuffer,
                               i8 iLowIndex,
                               i8 iHighIndex);
    static void vQuickSortBuffer(SrtBuffer* pSrtBuffer,
                                 i8 iLowIndex,
                                 i8 iHighIndex);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
