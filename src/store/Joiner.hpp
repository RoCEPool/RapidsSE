/*============================================================================*/
/* Joiner.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "OptimiserTool.hpp"

namespace STORE
{
  class Joiner : public OptimiserTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Joiner(Bench* pBench);
   ~Joiner(void) noexcept(false);

    void vJoin(void);

  private:

    Footer* aFooter(void);
  };

  Joiner* _pJoiner(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
