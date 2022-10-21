/*============================================================================*/
/* Card_Topology.hpp -- Stage Topology                                        */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Topology : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Topology(eYG eStageTopo);
   ~Card_Topology(void) noexcept(false);
  };

  Card_Topology* _pCard_Topology(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
