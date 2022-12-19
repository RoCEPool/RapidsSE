/*============================================================================*/
/* Card_JoinPredList.hpp -- Join Predicate List                               */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_JoinPredList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinPredList(void);
   ~Card_JoinPredList(void) noexcept(false);

    static Card_JoinPredList* pExtract(Card_TableList* p_tableList);

  };

  Card_JoinPredList* _pCard_JoinPredList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
