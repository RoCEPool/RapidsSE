/*============================================================================*/
/* PagingMetrics.hpp -- Paging Metrics                                        */
/*============================================================================*/
#pragma once

#include "Metrics.hpp"
#include "PipelineMetrics.hpp"

namespace STORE
{
  class PagingMetrics : public Operand
  {
  private:

    PagingMetrics* m_pNext;
    MetricsWsp  m_oMetricsWsp;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pPagingMetricsAddress);
  
    PagingMetrics(JobId tJobId);
   ~PagingMetrics(void) noexcept(false);


  private:
    Footer* aFooter(void);
  };
  
   PagingMetrics* _pPagingMetrics(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
