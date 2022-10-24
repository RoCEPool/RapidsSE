/*============================================================================*/
/* Card_CmpPred.hpp -- Compare Predicate                                      */
/*============================================================================*/
#pragma once

#include "Card_PRED.hpp"

#include "eRO.h"

namespace STORE
{
  class Card_CmpPred : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CmpPred(void);
    Card_CmpPred(Stz* szCmp);
   ~Card_CmpPred(void) noexcept(false);

    void vCmpPred_MarkBound(void);

    bool bCmpPred_CoPtnWith(Card_CmpPred* p_PredCmp_arg);
    Card_CmpPred* pCmpPred_FindBound(Index* pIndex, ePB eBound);
    bool bCmpPred_HasBounds(Index* pIndex);

    bool bCmpPred_IsEq(Card_CmpPred* p_PredCmp_arg);

    bool bCmpPred_OverlapsWith(Card_CmpPred* p_PredCmp_arg);

    bool bCmpPred_UsesTable(char cTableSyn);
  };

  Card_CmpPred* _pCard_CmpPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
