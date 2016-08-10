/*============================================================================*/
/* SegmentBuffer.hpp - a segment buffer within a segmented space              */
/*============================================================================*/

#pragma once

#include "MemoryManagement.h"

namespace MemoryManagement
{
  class SegmentBuffer : public Buffer
  {
  private:
    bool     m_bHasSpace;         // this buffer has more space?
    Object*  m_pFreeSpace;        // ptr to free space within this buffer
    SegmentBuffer* m_pPrevBuffer; // prev buffer
    SegmentBuffer* m_pNextBuffer; // next buffer
    
   public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SegmentBuffer(void);
   ~SegmentBuffer(void);
    
    Object* pAllocObject(size_t nBytes);
    
    Object* pFirst(void);
    Object* pNext(Object* pCurr, i8 iBytes);

    SegmentBuffer* pNextBuffer(void);
    void           vNextBuffer(SegmentBuffer* pNextBuffer);
    
    SegmentBuffer* pPrevBuffer(void);
    void           vPrevBuffer(SegmentBuffer* pPrevBuffer);
    
  private:
    Object* pFirstObject(void);
    Footer* pFooter(void);
    i8      iFreeSpace(void);
    bool    bIsExhausted(void);
    void    vSetExhausted(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
