/*============================================================================*/
/* OpMetrics.hpp -- Op Metrics                                                */
/*============================================================================*/
#pragma once

#include "Metrics.hpp"
#include "MetricsWsp.hpp"

namespace STORE { class JobMetrics; }
namespace STORE
{
  class OpMetrics : public Metrics
  {
  private:

    i8  m_i_Op;
    s32 m_sOpNm;
  
    i8  m_nQueued;
    i8  m_nTmpIndexSize;
    i8  m_nPotSize;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t nSize, MetricsWsp* pMetricsWsp);
    void operator delete(void* aAddr);
  
    OpMetrics(i8 i_Op, Stz* szOpNm,
              i8 nQueued, i8 nTmpIndexSize, i8 nPotSize);
   ~OpMetrics(void) noexcept(false);

    i8   i_Op(void);
    Stz* szOpNm(void);
    i8   nQueued(void);
    i8   nTmpIndexSize(void);
    i8   nPotSize(void);

  private:
    Footer* aFooter(void);
  };
  
   OpMetrics* _pOpMetrics(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
