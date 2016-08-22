/*============================================================================*/
/* Strand.hpp - a strand of segment buffers                                   */
/*============================================================================*/

#pragma once

#include "MemoryManagement.h"

namespace MemoryManagement
{
  class Strand : public Object
  {
  private:
    Lock m_xLock;
    
    Strand* m_pPrevStrand; // prev strand
    Strand* m_pNextStrand; // next strand

    i8 m_iCountBuffer; // no of buffers in this strand
    SegmentBuffer* m_pHeadBuffer; // prev buffer
    SegmentBuffer* m_pTailBuffer; // next buffer
    
    Footer m_xFooter;
    
   public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Strand(eCN eClass);
   ~Strand(void);
    
    i8 iCountBuffer(void);
    
    void vAppendBuffer(SegmentBuffer* pBuffer);
    SegmentBuffer* pFirstBuffer(void);
    SegmentBuffer* pPopBuffer(void);
    
  private:
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
