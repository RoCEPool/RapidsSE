/*============================================================================*/
/* Card_Topology.hpp -- Stage Topology                                        */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Topology : public Card_symbol<Card_Topology>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Topology(eYG eStageTopo);
   ~Card_Topology(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Topology* _pCard_Topology(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
