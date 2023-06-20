/*============================================================================*/
/* Card_IndexList.hpp -- Index List                                           */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_IndexList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IndexList(void);
   ~Card_IndexList(void) noexcept(false);

    static Card_IndexList* pDerive(Table* pTable);

    void vDeleteAll_bounds(void);

    Card_Index* pFindChosenIndex(void);
    void vChoose_index_For_indexJoin(void);
    void vChoose_index_For_indexScan(void);

    bool bHas_cmpPred_In_bounds(Card_CmpPred* p_cmpPred);

    void vTypes_For_stmColOdn(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

    bool bHasEquivalentBound(Card_CmpPred* p_cmpPred);
    bool bUsesSqyRsl(void);

  };

  Card_IndexList* _pCard_IndexList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
