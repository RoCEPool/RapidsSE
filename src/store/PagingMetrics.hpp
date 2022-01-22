/*============================================================================*/
/* PagingMetrics.hpp -- Paging Metrics                                        */
/*============================================================================*/
#pragma once

#include "Metrics.hpp"
#include "MetricsWsp.hpp"
#include "DerefMetrics.hpp"
#include "eDR.h"

#include "Tub.hpp"

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

    bool bAddDerefMetrics(i8 iObjectNo, eCN eClassNo, i8 iTsc, eDR eAction);
    PagingMetrics* pNext(void);
    void vReportDerefMetrics(Tub* pTub);

  private:
  
    DerefMetrics* pGetDerefMetrics(i8& d_MetricsCursor);

    Footer* aFooter(void);
  };
  
   PagingMetrics* _pPagingMetrics(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
