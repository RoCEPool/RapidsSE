/*============================================================================*/
/* Preder.hpp -- predicate tool                                               */
/*============================================================================*/
#pragma once

#include "OptimiserTool.hpp"

namespace STORE
{
  class Preder : public OptimiserTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Preder(Bench* pBench);
   ~Preder(void) noexcept(false);

    void vDiscover(void);

  private:

    Footer* aFooter(void);
  };

  Preder* _pPreder(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
