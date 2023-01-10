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

    static void vAdd_where(CardIter* pCI, Card* p_after);
  };

  Card_Where* _pCard_Where(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
