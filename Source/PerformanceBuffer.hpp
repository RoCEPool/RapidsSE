/*============================================================================*/
/* PerformanceBuffer.hpp -- Performance Buffer                                */
/*============================================================================*/
#pragma once

namespace Measurement
{
  class PerformanceBuffer : public ItemBuffer
  {
  protected:
  
    PerformanceBuffer* m_pNextBuffer;  // next buffer
    PerformanceBuffer* m_pPrevBuffer;  // prev buffer

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    PerformanceBuffer(void);
   ~PerformanceBuffer(void);
  
    PerformanceEntry*  pAllotEntry(void);
  
    PerformanceBuffer* pNextBuffer(void);
    void               vNextBuffer(PerformanceBuffer* pNextBuffer);
    
    PerformanceBuffer* pPrevBuffer(void);
    void               vPrevBuffer(PerformanceBuffer* pPrevBuffer);
  
    PerformanceEntry*  pFirstEntry(void);
    PerformanceEntry*  pNextEntry(PerformanceEntry* pCurrentEntry);
  
    Footer* aFooter(void);
  };
  
  PerformanceBuffer* _pPerformanceBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
