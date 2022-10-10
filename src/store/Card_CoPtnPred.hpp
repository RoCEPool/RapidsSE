/*============================================================================*/
/* Card_CoPtnPred.hpp -- Filter                                                 */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CoPtnPred : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CoPtnPred(void);
   ~Card_CoPtnPred(void) noexcept(false);
  };

  Card_CoPtnPred* _pCard_CoPtnPred(void* aAddr);
  bool _bIsCard_CoPtnPred(void* aAddr);
  Card_CoPtnPred* _pSub_CoPtnPred(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
