/*============================================================================*/
/* Card_StreamPrmAll.hpp -- All Join Permutations                              */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
   class Card_StreamPrmAll : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamPrmAll(void);
   ~Card_StreamPrmAll(void) noexcept(false);

    static Card_StreamPrmAll* pComputeAllPermutations(Card_TableList* p_tableList,
                                                      Card_JoinPredList* p_joinPredList);

    void vEstCostsForAllPermutations(Card_TableList*    p_tableList,
                                     Card_FiltPredList* p_filtPredList);
    Card_StreamPer* pJoinOrder(i8 iJoinOrder);
    Card_StreamPer* pLowestCost(void);
    void vRemoveInvalid(void);
   };

  Card_StreamPrmAll* _pCard_StreamPrmAll(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
