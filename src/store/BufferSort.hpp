/*============================================================================*/
/* BufferSort.hpp -- Buffer Sort                                              */
/*============================================================================*/
#pragma once

namespace STORE
{
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class BufferSort 
  {
  public:
    static BufferCursor* pPartitionBuffer(SrtBuffer* pSrtBuffer,
                                          BufferCursor* pLowCursor,
                                          BufferCursor* pHighCursor);
    static void vQuickSortBuffer(SrtBuffer* pSrtBuffer,
                                 BufferCursor* pLowCursor,
                                 BufferCursor* pHighCursor);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
