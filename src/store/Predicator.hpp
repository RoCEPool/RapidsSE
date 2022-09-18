/*============================================================================*/
/* Predicator.hpp -- predicate tool                                           */
/*============================================================================*/
#pragma once

#include "OptimiserTool.hpp"

namespace STORE
{
  class Predicator : public OptimiserTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Predicator(Bench* pBench);
   ~Predicator(void) noexcept(false);

    void vDiscover(void);

  private:

    Footer* aFooter(void);
  };

  Predicator* _pPredicator(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
