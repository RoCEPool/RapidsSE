/*============================================================================*/
/* Segment.hpp - segmented space allocator                                    */
/*============================================================================*/

#pragma once

#include "MemoryManagement.h"

namespace MemoryManagement
{
  class Segment : public Object
  {
  protected:
    Lock     m_zLock;         // lock covers all buffers
    i8       m_iCount;        // number of buffers
    SegmentBuffer* m_pFirstBuffer; // first buffer in list
    SegmentBuffer* m_pLastBuffer;  // last buffer in list
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Segment(eCN eClass);
    ~Segment(void);
    
    Object* pAllocObject(size_t nBytes);
    void    vExtendSpace(void);

    Object* pFirstObject(void);
    Object* pNextObject(Object* pCurr, i8 nBytes);

    SegmentBuffer* pFirstBuffer(void);
    SegmentBuffer* pLastBuffer(void);
    
    i8      iBufferCount(void);
    
    void    vReleaseAll(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
