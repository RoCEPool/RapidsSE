/*============================================================================*/
/* DerefMetrics.hpp -- De-Reference Metrics                                   */
/*============================================================================*/
#pragma once

#include "Metrics.hpp"
#include "MetricsWsp.hpp"
#include "eDR.h"

namespace STORE
{
  class DerefMetrics : public Metrics
  {
  private:

    i8   m_iObjectNo;
    eCN  m_eClassNo;
    i8   m_iTsc;
    eDR  m_eAction;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t nSize, MetricsWsp* pMetricsWsp);
    void operator delete(void* aAddr);
  
    DerefMetrics(i8 iObjectNo, eCN eClassNo, i8 iTsc, eDR eAction);
   ~DerefMetrics(void) noexcept(false);

    eDR  eAction(void);
    eCN  eClassNo(void);
    i8   iObjectNo(void);
    i8   iTsc(void);

  private:
    Footer* aFooter(void);
  };
  
   DerefMetrics* _pDerefMetrics(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
