/*============================================================================*/
/* Card_Scan.hpp -- Scan                                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Scan : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Scan(void);
   ~Card_Scan(void) noexcept(false);

    bool bIsPredInScope(Card_PRED* pCard_PRED);
  };

  Card_Scan* _pCard_Scan(void* aAddr);
  bool _bIsCard_Scan(void* aAddr);
  Card_Scan* _pGetSub_Card_Scan(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
