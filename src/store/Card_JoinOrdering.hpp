/*============================================================================*/
/* Card_JoinOrdering.hpp -- Join Order Sequence                               */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_JoinOrdering : public Card_symbol<Card_JoinOrdering>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinOrdering(void);
   ~Card_JoinOrdering(void) noexcept(false);

    static Card_JoinOrdering* pDerive(Card_StreamPer* p_streamPer);

    static Card_JoinOrdering* pPick(Card_StreamSet* p_joinPermAll, i8 iJoinOrder);
    static Card_JoinOrdering* pPick(Card_StreamSet* p_joinPermAll, bool bLowest=true);

    void vJoinOrderString(String* pString, Card_TableList* p_tableList,
                          bool bNumbers=false);
    void vReorder(Card_Deck* pDeck);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_JoinOrdering* _pCard_JoinOrdering(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
