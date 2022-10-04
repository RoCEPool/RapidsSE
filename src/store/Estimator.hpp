/*============================================================================*/
/* Estimator.hpp -- Estimator tool                                            */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Estimator : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Estimator(Bench* pBench);
   ~Estimator(void) noexcept(false);

    void vXyz(void);

  private:

    Footer* aFooter(void);
  };

  Estimator* _pEstimator(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
