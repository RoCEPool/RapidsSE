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

    eXK eCastKind(Card_TablePtn* p_tablePtn_l, Card_TablePtn* p_tablePtn_r);
    Card_CmpPred* pCmpPredFindBound(Index* pIndex, ePB eBound);
    bool bCmpPredHasBounds(Index* pIndex);
    bool bContains_Sqy_Call(void);
    bool bCoPtnWith(Card_CmpPred* p_cmpPred_arg);
    Card_Term* pExtractArgTerm(void);
    Card_Term* pExtractParamTerm(void);
    bool bIsEq(Card_CmpPred* p_cmpPred_arg);
    bool bIsEqOp(void);
    bool bIsJoinPredBetween(char cTblSyn_l, char cTblSyn_r);
    void vMarkBound(void);
    bool bMaybeBounds_v2(Card_Table* p_table, Card_Index* p_index, bool bParamAllowedInBounds);
    bool bMaybeBoundsOf_index(Card_Index* p_index, bool bParamAllowedInBounds);
    bool bMaybeJoinPred(void);
    char cOtherTable(char cTblSyn);
    bool bOverlapsWith(Card_CmpPred* p_cmpPred_arg);
    bool bSubsetOf_bounds(Card_Bounds* p_bounds);
    bool bUsesTable(char cTblSyn);

  };

  Card_CmpPred* _pCard_CmpPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
