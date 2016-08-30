/*============================================================================*/
/* HeapBuffer.hpp - segment of objects                                        */
/*============================================================================*/

#pragma once

#include "Pool.h"

namespace Pool
{
  class HeapBuffer : public Buffer
  {
  private:
    HeapBuffer* m_pNextBuffer;  // next buffer in list
    HeapBuffer* m_pPrevBuffer;  // prev buffer in list
    // ..space..
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    HeapBuffer(void);
   ~HeapBuffer(void);
    
    static void vDump(Object* pObject, const char* pTab);
    
    void vAnalyse(i8 nBytes, i8 xCounts[]);
    HeapBuffer* pNextBuffer(void);
    void        vNextBuffer(HeapBuffer* pNextBuffer);
    HeapBuffer* pPrevBuffer(void);
    void        vPrevBuffer(HeapBuffer* pPrevSegment);
    
    FreeChunk*  pFirstChunk(void);
    FreeChunk*  pNextChunk(FreeChunk* pChunk, i8 nBytes);
    
  private:
    char* pChunkEnd(FreeChunk* pChunk, i8 nBytes);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
