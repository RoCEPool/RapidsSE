/*============================================================================*/
/* Card_JoinEstimate.hpp -- Estimate cost for a JoinNext of a Query               */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_JoinEstimate : public Card_symbol<Card_JoinEstimate>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinEstimate(void);
   ~Card_JoinEstimate(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_JoinEstimate* _pCard_JoinEstimate(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
