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

    Card_SubSet* m_pCard_SubSet;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    OptimiserTool(eCN eClass, Card_SubSet* pCard_SubSet, Bench* pBench);
   ~OptimiserTool(void) noexcept(false);

  private:

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
