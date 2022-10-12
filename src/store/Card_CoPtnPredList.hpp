/*============================================================================*/
/* Card_CoPtnPredList.hpp -- Co-Partition Predicate List                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CoPtnPredList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CoPtnPredList(void);
   ~Card_CoPtnPredList(void) noexcept(false);
  };

  Card_CoPtnPredList* _pCard_CoPtnPredList(void* aAddr);
  bool _bIsCard_CoPtnPredList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
