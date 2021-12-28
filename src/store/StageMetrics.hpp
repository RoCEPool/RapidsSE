/*============================================================================*/
/* StageMetrics.hpp -- Stage Metrics                                          */
/*============================================================================*/
#pragma once

#include "Metrics.hpp"
#include "MetricsWsp.hpp"
#include "StreamMetrics.hpp"

namespace STORE { class PipelineMetrics;
                  class StageMetrics;
                  class StreamMetrics;
                  class OpMetrics;       }

namespace STORE
{
  class StageMetrics : public Metrics
  {
  private:
  
    s32 m_sStageNm;
    i8  m_i_Stage;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t nSize, MetricsWsp* pMetricsWsp);
    void operator delete(void* aAddr);
  
    StageMetrics(Stz* szStageNm, i8 i_Stage);
   ~StageMetrics(void) noexcept(false);

    StageMetrics*    pAllotStageMetrics(void);
    StreamMetrics*   pAllotStreamMetrics(void);
    OpMetrics*       pAllotOpMetrics(void);
  
  private:
    Footer* aFooter(void);
  };
  
   StageMetrics* _pStageMetrics(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
