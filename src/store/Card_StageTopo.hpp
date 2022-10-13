/*============================================================================*/
/* Card_StageTopo.hpp -- Stage Topology                                       */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_StageTopo : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StageTopo(eYG eStageTopo);
   ~Card_StageTopo(void) noexcept(false);
  };

  Card_StageTopo* _pCard_StageTopo(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
