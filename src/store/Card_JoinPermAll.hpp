/*============================================================================*/
/* Card_JoinPermAll.hpp -- All Join Permutations                              */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
   class Card_JoinPermAll : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinPermAll(void);
   ~Card_JoinPermAll(void) noexcept(false);

    static Card_JoinPermAll* pAllPermutedJoins(Card_TableList* p_tableList,
                                               Card_JoinPredList* p_joinPredList);

    void vEstCostsForAllJoinPerm(Card_TableList* p_tableList);
    Card_JoinPermSeq* pLowestLoad(void);
    void vRemoveInvalid(void);
   };

  Card_JoinPermAll* _pCard_JoinPermAll(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
