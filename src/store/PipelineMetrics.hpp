/*============================================================================*/
/* PipelineMetrics.hpp -- Pipeline Metrics                                    */
/*============================================================================*/
#pragma once

#include "Metrics.hpp"
#include "MetricsWsp.hpp"
#include "StageMetrics.hpp"

namespace STORE
{
  class PipelineMetrics : public Metrics
  {
  private:
  
    s32 m_sPipelineNm;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t nSize, MetricsWsp* pMetricsWsp);
    void operator delete(void* aAddr);
    PipelineMetrics(Stz* szPipelineNm);
   ~PipelineMetrics(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };
  
   PipelineMetrics* _pPipelineMetrics(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
