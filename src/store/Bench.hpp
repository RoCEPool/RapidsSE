 /*============================================================================*/
/* Bench.hpp -- Bench                                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Board.hpp"

namespace STORE
{
  class Bench : public Object
              , public ListExo2_t<Board>
  {
  private:
    Optimiser* m_pOptimiser;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Bench(Optimiser* pOptimiser);
   ~Bench(void) noexcept(false);
  
    void vPrintBench(void);
    void vRenumberCards(void);

  private:
    Footer* aFooter(void);
  };

  Bench* _pBench(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
