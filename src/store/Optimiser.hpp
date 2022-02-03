/*============================================================================*/
/* Optimiser.hpp --                                                               */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Dissect.hpp"

namespace STORE
{
  class Optimiser : public Object
  {
  private:
    PipelineSpec* m_pPipelineSpec_original;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Optimiser(PipelineSpec* pPipelineSpec_original);
   ~Optimiser(void) noexcept(false);

    static void vRepair(Object* pObject);

    PipelineSpec* pPipelineSpec_original(void);
    void vRun(void);

  private:

    Footer* aFooter(void);
  };

  Optimiser* _pOptimiser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
