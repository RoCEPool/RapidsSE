/*============================================================================*/
/* JobMetrics.hpp -- JobMetrics                                                 */
/*============================================================================*/
#pragma once

#include "Metrics.hpp"
#include "PipelineMetrics.hpp"

namespace STORE
{
  class JobMetrics : public Metrics
  {
  private:
    JobId  m_tJobId;

    i8     m_d_PipelineMetrics;
  
    MetricsWsp m_oMetricsWsp;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pJobMetricsAddress);
  
    JobMetrics(JobId tJobId);
   ~JobMetrics(void) noexcept(false);

    PipelineMetrics* pAllotPipelineMetrics(void);
    StageMetrics*    pAllotStageMetrics(void);
    StreamMetrics*   pAllotStreamMetrics(void);
    OpMetrics*       pAllotOpMetrics(void);

    PipelineMetrics* pFirstPipelineMetrics(i8& d_MetricsCursor);
    StageMetrics*    pFirstStageMetrics(i8& d_MetricsCursor);
    StreamMetrics*   pFirstStreamMetrics(i8& d_MetricsCursor);
    OpMetrics*       pFirstOpMetrics(i8& d_MetricsCursor);

    PipelineMetrics* pNextPipelineMetrics(i8& d_MetricsCursor);
    StageMetrics*    pNextStageMetrics(i8& d_MetricsCursor);
    StreamMetrics*   pNextStreamMetrics(i8& d_MetricsCursor);
    OpMetrics*       pNextOpMetrics(i8& d_MetricsCursor);

  private:
    Footer* aFooter(void);
  };
  
   JobMetrics* _pJobMetrics(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
