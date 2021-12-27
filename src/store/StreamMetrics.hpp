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
  
    i8 m_nOps;

    i8 m_d_OpMetrics;

    // must be last
    Footer m_xFooter;
    
  public:  
    void* operator new(size_t nSize, MetricsWsp* pMetricsWsp);
    void operator delete(void* aAddr);
  
    StreamMetrics(i8 i_Stream);
   ~StreamMetrics(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };
  
   StreamMetrics* _pStreamMetrics(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
