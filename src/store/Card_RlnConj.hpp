/*============================================================================*/
/* Card_RlnConj.hpp -- Compare Predicate                                      */
/*============================================================================*/
#pragma once

#include "Card_CONJ.hpp"

#include "eRO.h"

namespace STORE
{
  class Card_RlnConj : public Card_CONJ
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_RlnConj(void);
   ~Card_RlnConj(void) noexcept(false);

    Card_RlnConj* pClone(void);
     void vEnsureTypeId_expCol(CardIter* pCI);

    Card_Term* pGetCorrCol(void);
    char cGetTable_a(void);
    char cGetTable_b(void);
    Card_Term* pGetTerm_0(void);
    Card_Term* pGetTerm_1(void);
    bool bIsEq(Card_RlnConj* p_rlnConj_arg);
    bool bIsEqOp(void);
    bool bIsIndexBound(Card_Index* p_index);
    bool bIsRlnOp(void);
    char cOtherTable(char cTblSyn);
    bool bSubsetOf_bounds(Card_Bounds* p_bounds);
    bool bUsesTable(char cTblSyn);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_RlnConj* _pCard_RlnConj(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
