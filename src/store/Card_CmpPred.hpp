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

    void vMarkBound(void);


    ePB eBound(void);
    eRO eRelOp(void);

    bool bCoPtnWith(Card_CmpPred* p_PredCmp_arg);
    Card_CmpPred* pDup(void);
    Card_CmpPred* pFindBound_compare(Index* pIndex, ePB eBound);
    bool bHasBounds_compare(Index* pIndex);

    bool bIsEq(Card_CmpPred* p_PredCmp_arg);

    bool bOverlapsWith(Card_CmpPred* p_PredCmp_arg);

    bool bIsJoinPred(void);
    bool bUsesTable(char cTableSyn);
  };

  Card_CmpPred* _pCard_CmpPred(void* aAddr);
  bool _bIsCard_CmpPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
