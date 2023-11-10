/*============================================================================*/
/* Card_IndexList.hpp -- Index List                                           */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_IndexList : public Card_symbol<Card_IndexList>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IndexList(void);
   ~Card_IndexList(void) noexcept(false);

    static Card_IndexList* pDerive(Table* pTable);

    void vChoose_index_For_crossJoin(void);
    void vChoose_index_For_indexJoin(void);
    void vChoose_index_For_indexScan(void);

    void vDeleteAll_bounds(void);

    Card_Index* pFindChosenIndex(void);

    bool bHas_cmpPred_In_bounds(Card_RlnConj* p_rlnConj);

    void vTypes_For_stmCol(CardIter* pCI);

    bool bHasEquivalentBound(Card_RlnConj* p_rlnConj);
    bool bUsesSqyRsl(void);

    void vX_tblCol_To_expCol(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_IndexList* _pCard_IndexList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
