/*============================================================================*/
/* JobMetrics.hpp -- JobMetrics                                                 */
/*============================================================================*/
#pragma once

#include "Metrics.hpp"
#include "PipelineMetrics.hpp"

#include "IntStack.hpp"

namespace STORE
{
  class JobMetrics : public Operand
  {
  private:
    JobId  m_tJobId;
  
    MetricsWsp m_oMetricsWsp;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pJobMetricsAddress);
  
    JobMetrics(JobId tJobId);
   ~JobMetrics(void) noexcept(false);
  
    static void vJobSet(IntStack* pIntStack);
    static void vRepair(Object* pObject);

    bool bCollect(void);
  
    PipelineMetrics* pFirstPipelineMetrics(i8& d_MetricsCursor);
    StageMetrics*    pFirstStageMetrics(i8& d_MetricsCursor);
    StreamMetrics*   pFirstStreamMetrics(i8& d_MetricsCursor);
    OpMetrics*       pFirstOpMetrics(i8& d_MetricsCursor);

    MetricsWsp*      pMetricsWsp(void);

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
