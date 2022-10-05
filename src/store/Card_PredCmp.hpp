/*============================================================================*/
/* Card_PredCmp.hpp -- Compare Predicate                                      */
/*============================================================================*/
#pragma once

#include "Card_PRED.hpp"

#include "eRO.h"

namespace STORE
{
  class Card_PredCmp : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_PredCmp(void);
    Card_PredCmp(Stz* szCmp);
   ~Card_PredCmp(void) noexcept(false);

    void vMarkBound(void);


    ePB eBound(void);
    eRO eRelOp(void);

    bool bCoPtnWith(Card_PredCmp* p_PredCmp_arg);
    Card_PredCmp* pDup(void);
    Card_PredCmp* pFindBound_compare(Index* pIndex, ePB eBound);
    bool bHasBounds_compare(Index* pIndex);

    bool bIsEq(Card_PredCmp* p_PredCmp_arg);

    bool bOverlapsWith(Card_PredCmp* p_PredCmp_arg);

    bool bIsJoinPred(void);
    bool bUsesTable(char cTableSyn);
  };

  Card_PredCmp* _pCard_PredCmp(void* aAddr);
  bool _bIsCard_PredCmp(void* aAddr);
  Card_PredCmp* _pGetSub_PredCmp(Card* pParent);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
