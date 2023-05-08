/*============================================================================*/
/* OptimiserTool.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Bench.hpp"

namespace STORE
{
  class OptimiserTool : public Object
  {
  protected:
    Optimiser* m_pOptimiser;
    Bench*     m_pBench;
    Board*     m_pBoard;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    OptimiserTool(eCN eClass);
   ~OptimiserTool(void) noexcept(false);

  private:

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
