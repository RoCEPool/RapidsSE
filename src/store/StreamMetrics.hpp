/*============================================================================*/
/* StreamMetrics.hpp -- Stream Metrics                                        */
/*============================================================================*/
#pragma once

#include "Metrics.hpp"
#include "MetricsWsp.hpp"
#include "OpMetrics.hpp"

namespace STORE
{
  class StreamMetrics : public Metrics
  {
  private:

    i8 m_i_Stream;
  
    // must be last
    Footer m_xFooter;
    
  public:  
    void* operator new(size_t nSize, MetricsWsp* pMetricsWsp);
    void operator delete(void* aAddr);
  
    StreamMetrics(i8 i_Stream);
   ~StreamMetrics(void) noexcept(false);

    i8 i_Stream(void);
  
 private:
    Footer* aFooter(void);
  };
  
   StreamMetrics* _pStreamMetrics(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
