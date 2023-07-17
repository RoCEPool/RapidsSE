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

    eXK eCastKind(char cTblSyn_l, Card_TablePtn* p_tablePtn_l,
                  char cTblSyn_r, Card_TablePtn* p_tablePtn_r);
    Card_CmpPred* pClone(void);
    Card_CmpPred* pCmpPredFindBound(Index* pIndex, ePB eBound);
    bool bCmpPredHasBounds(Index* pIndex);
    bool bCoPtnWith(Card_CmpPred* p_cmpPred_arg);
    Card_Term* pGetCorrCol(void);
    Card_Term* pExtractParamTerm(void);
    char cGetTable_a(void);
    char cGetTable_b(void);
    void vGetTables_a_b(char& rcTblSyn_a, char& rcTblSyn_b);
    Card_Term* pGetTerm_0(void);
    Card_Term* pGetTerm_1(void);
    bool bIsEq(Card_CmpPred* p_cmpPred_arg);
    bool bIsEqOp(void);
    char cOtherTable(char cTblSyn);
    bool bOverlapsWith(Card_CmpPred* p_cmpPred_arg);
    bool bSubsetOf_bounds(Card_Bounds* p_bounds);
    bool bUsesTable(char cTblSyn);
    bool bUsesTables(char cTblSyn_a, char cTblSyn_b);

  };

  Card_CmpPred* _pCard_CmpPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
