/*============================================================================*/
/* OptimiserTool.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Dissector.hpp"

#include "Bench.hpp"

namespace STORE
{
  class OptimiserTool : public Object
  {
  protected:

    Bench*       m_pBench;
    Board*       m_pBoard;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    OptimiserTool(eCN eClass, Bench* pBench);
   ~OptimiserTool(void) noexcept(false);

  private:

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
