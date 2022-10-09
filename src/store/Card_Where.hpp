/*============================================================================*/
/* Card_Where.hpp -- Where                                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Where : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Where(void);
   ~Card_Where(void) noexcept(false);

    bool bIsPredInScope(Card_PRED* pCard_PRED);
  };

  Card_Where* _pCard_Where(void* aAddr);
  bool _bIsCard_Where(void* aAddr);
  Card_Where* _pGetSub_Where(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
