/*============================================================================*/
/* OpMetrics.hpp -- Op Metrics                                                */
/*============================================================================*/
#pragma once

#include "Metrics.hpp"
#include "MetricsWsp.hpp"

namespace STORE { class JobMetrics; }
namespace STORE
{
  class OpMetrics : public Metrics
  {
  private:

    i8 m_i_Op;
  
    i8 m_nQueued;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t nSize, MetricsWsp* pMetricsWsp);
    void operator delete(void* aAddr);
  
    OpMetrics(i8 i_Op);
   ~OpMetrics(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };
  
   OpMetrics* _pOpMetrics(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
