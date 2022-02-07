 /*============================================================================*/
/* Bench.hpp -- Bench                                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Board.hpp"
#include "Tools.hpp"

namespace STORE
{
  class Bench : public Object
              , public ListExo2_t<Board>
  {
  private:
  
    Tools* m_pTools;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Bench(void);
   ~Bench(void) noexcept(false);
  
    void vWalkUnits(std::function<void(Card* pCard)> f_vClosure);
    void vWalkAllCards(std::function<void(Card* pCard)> f_vClosure);

  private:
    Footer* aFooter(void);
  };

  Bench* _pBench(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
