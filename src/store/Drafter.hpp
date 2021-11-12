/*============================================================================*/
/* Drafter.hpp --                                                               */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Dissect.hpp"

namespace STORE
{
  class Drafter : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Drafter(void);
   ~Drafter(void) noexcept(false);

    static void vRepair(Object* pObject);

    void vRun(PipelineSpec *pPipelineSpec);

  private:

    Footer* aFooter(void);
  };

  Drafter* _pDrafter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
