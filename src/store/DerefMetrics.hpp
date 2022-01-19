/*============================================================================*/
/* DerefMetrics.hpp -- Deref Metrics                                          */
/*============================================================================*/
#pragma once

#include "Metrics.hpp"
#include "MetricsWsp.hpp"

namespace STORE
{
  class DerefMetrics : public Metrics
  {
  private:

    i8  m_iObjectNo;
    i8  m_iTsc;
    i8  m_iAction;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t nSize, MetricsWsp* pMetricsWsp);
    void operator delete(void* aAddr);
  
    DerefMetrics(i8 iObjectNo, i8 iTsc, i8 iAction);
   ~DerefMetrics(void) noexcept(false);

    i8   iObjectNo(void);
    i8   iTsc(void);
    i8   iAction(void);

  private:
    Footer* aFooter(void);
  };
  
   DerefMetrics* _pDerefMetrics(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
