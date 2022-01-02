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
    JobId m_tJobId;
  
    JobMetrics* m_pNext;
    MetricsWsp  m_oMetricsWsp;
  
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
  
    PipelineMetrics* pGetPipelineMetrics(i8& d_MetricsCursor);
    StageMetrics*    pGetStageMetrics(i8& d_MetricsCursor);
    StreamMetrics*   pGetStreamMetrics(i8& d_MetricsCursor);
    OpMetrics*       pGetOpMetrics(i8& d_MetricsCursor);

    MetricsWsp*      pMetricsWsp(void);

    JobMetrics*      pNext(void);
    void             vNext(JobMetrics* pNext);

    bool bUpdate(bool bAppendList);

    void vPrint(void);

  private:
    void vCollect(Job* pJob);
    eCN  eNextClassNo(i8 d_MetricsCursor);
    Object* pObject(i8 d_MetricsCursor);
 
    Footer* aFooter(void);
  };
  
   JobMetrics* _pJobMetrics(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
