/*============================================================================*/
/* Card_StreamSet.hpp -- All Join Permutations                              */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_StreamSet : public Card_symbol<Card_StreamSet>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamSet(void);
   ~Card_StreamSet(void) noexcept(false);

    static Card_StreamSet* pComputeAllPermutations(Card_TableList* p_tableList,
                                                   Card_JoinPredList* p_joinPredList);

    void vEstCostsForAllPermutations(CardIter* pCI,
                                     Card_TableList*    p_tableList,
                                     Card_FiltPredList* p_filtPredList);
    Card_StreamPer* pHighestCost(void);
    Card_StreamPer* pJoinOrder(i8 iJoinOrder);
    Card_StreamPer* pLowestCost(void);
    void vRemoveInvalid(void);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_StreamSet* _pCard_StreamSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
