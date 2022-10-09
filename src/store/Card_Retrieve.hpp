/*============================================================================*/
/* Card_Retrieve.hpp -- Scan                                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Retrieve : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Retrieve(void);
   ~Card_Retrieve(void) noexcept(false);

    bool bIsPredInScope(Card_PRED* pCard_PRED);
  };

  Card_Retrieve* _pCard_Retrieve(void* aAddr);
  bool _bIsCard_Retrieve(void* aAddr);
  Card_Retrieve* _pGetSub_Retrieve(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
