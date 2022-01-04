/*============================================================================*/
/* SysMetrics.hpp -- SysMetrics                                                 */
/*============================================================================*/
#pragma once

#include "Metrics.hpp"
#include "PipelineMetrics.hpp"

namespace STORE
{
  class SysMetrics : public Operand
  {
  private:
  
    i8 m_iPctFree;
    i8 m_nRefTimer;
    i8 m_nSince;
    i8 m_nThreadsIdle;

    i8 m_nLargeHeap;
    i8 m_nMediumHeap;
    i8 m_nSmallHeap;
    i8 m_nTinyHeap;

    SysMetrics* m_pNext;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pSysMetricsAddress);
  
    SysMetrics(void);
   ~SysMetrics(void) noexcept(false);
  
    static void vRepair(Object* pObject);

    void vCollect(void);
  
    SysMetrics* pNext(void);
    void        vNext(SysMetrics* pNext);
  
    i8  iPctFree(void);
    i8  nRefTimer(void);
    i8  nSince(void);
    i8  nThreadsIdle(void);

    i8  nLargeHeap(void);
    i8  nMediumHeap(void);
    i8  nSmallHeap(void);
    i8  nTinyHeap(void);
  
  private:
 
    Footer* aFooter(void);
  };
  
   SysMetrics* _pSysMetrics(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
