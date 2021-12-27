/*============================================================================*/
/* JobMetrics.hpp -- JobMetrics                                                 */
/*============================================================================*/
#pragma once

#include "Metrics.hpp"

namespace STORE
{
  static const i8 C_WSP_METRICS = C_SIZE_LARGE_OBJECT - C_4KB;

  class MetricsWsp : public Object
  {
  private:
  
    Wsp_t<C_WSP_METRICS> m_xWsp;

    // must be last
    Footer m_xFooter;
    
  public:
    MetricsWsp(void);
   ~MetricsWsp(void) noexcept(false);
  
    void* aAllot(i8 nBytes);

    i8    d_Offset(void* aAllot);

  private:
    Footer* aFooter(void);
  };
  
  MetricsWsp* _pMetricsWsp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
