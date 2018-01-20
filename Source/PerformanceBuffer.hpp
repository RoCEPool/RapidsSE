/*============================================================================*/
/* PerformanceBuffer.hpp -- Performance Buffer                                */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class PerformanceBuffer : public ItemBuffer
  {
  protected:

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    PerformanceBuffer(void);
   ~PerformanceBuffer(void);
  
    Footer* aFooter(void);
  };
  
  PerformanceBuffer* _pPerformanceBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
