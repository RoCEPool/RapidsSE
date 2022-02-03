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
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Optimiser(void);
   ~Optimiser(void) noexcept(false);

    static void vRepair(Object* pObject);

    void vRun(PipelineSpec *pPipelineSpec);

  private:

    Footer* aFooter(void);
  };

  Optimiser* _pOptimiser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
