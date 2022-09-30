/*============================================================================*/
/* Card_JoinPredList.hpp -- Join Predicate List                               */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Stream;

  class Card_JoinPredList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinPredList(void);
   ~Card_JoinPredList(void) noexcept(false);

    void vTo_joinPredList(Card_CalcBool* p_calcBool);
  };

  Card_JoinPredList* _pCard_JoinPredList(void* aAddr);
  bool _bIsCard_JoinPredList(void* aAddr);
  Card_JoinPredList* _pGetSub_Card_JoinPredList(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
