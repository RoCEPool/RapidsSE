/*============================================================================*/
/* Card_CoPtnJoin.hpp -- Co-Partition                                             */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_CoPtnPred.hpp"

namespace STORE
{
  class Card_CoPtnJoin : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CoPtnJoin(void);
   ~Card_CoPtnJoin(void) noexcept(false);

   bool bCoPtnWith(Card_CoPtnPred* p_CoPtnPred);
  };

  Card_CoPtnJoin* _pCard_CoPtnJoin(void* aAddr);
  bool _bIsCard_CoPtnJoin(void* aAddr);
  Card_CoPtnJoin* _pSub_CoPtnJoin(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
