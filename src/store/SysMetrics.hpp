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
  
    SysMetrics* m_pNext;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pSysMetricsAddress);
  
    SysMetrics(void);
   ~SysMetrics(void) noexcept(false);
  
    static void vRepair(Object* pObject);

    bool bCollect(void);
  
    SysMetrics* pNext(void);
    void        vNext(SysMetrics* pNext);

    bool bUpdate(bool bAppendList);

    void vPrint(void);

  private:
 
    Footer* aFooter(void);
  };
  
   SysMetrics* _pSysMetrics(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
