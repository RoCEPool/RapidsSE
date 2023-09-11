/*============================================================================*/
/* Card_CmpConj.hpp -- Compare Predicate                                      */
/*============================================================================*/
#pragma once

#include "Card_CONJUNCTION.hpp"

#include "eRO.h"

namespace STORE
{
  class Card_CmpConj : public Card_CONJUNCTION
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CmpConj(void);
   ~Card_CmpConj(void) noexcept(false);

    eXK eCastKind(char cTblSyn_l, Card_TablePtn* p_tablePtn_l,
                  char cTblSyn_r, Card_TablePtn* p_tablePtn_r);
    Card_CmpConj* pClone(void);
    Card_CmpConj* pCmpPredFindBound(Index* pIndex, ePB eBound);
    bool bCmpPredHasBounds(Index* pIndex);
    bool bCoPtnWith(Card_CmpConj* p_cmpPred_arg);
    void vEnsureTypesFor_stmOdn(CardIter* pCI);

    Card_Term* pGetCorrCol(void);
    Card_Term* pExtractParamTerm(void);
    char cGetTable_a(void);
    char cGetTable_b(void);
    void vGetTables_a_b(char& rcTblSyn_a, char& rcTblSyn_b);
    Card_Term* pGetTerm_0(void);
    Card_Term* pGetTerm_1(void);
    bool bIsEq(Card_CmpConj* p_cmpPred_arg);
    bool bIsEqOp(void);
    char cOtherTable(char cTblSyn);
    bool bOverlapsWith(Card_CmpConj* p_cmpPred_arg);
    bool bSubsetOf_bounds(Card_Bounds* p_bounds);
    bool bUsesTable(char cTblSyn);
    bool bUsesTables(char cTblSyn_a, char cTblSyn_b);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_CmpConj* _pCard_CmpConj(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
