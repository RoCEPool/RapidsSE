/*============================================================================*/
/* Card_BoolPred.hpp -- Boolean Predicate                                     */
/*============================================================================*/
#pragma once

#include "Card_CONJSUBROOT.hpp"
#include "Card_Correlation.hpp"

namespace STORE
{
  class Card_BoolPred : public Card_CONJSUBROOT
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_BoolPred(void);
   ~Card_BoolPred(void) noexcept(false);

    static Card_BoolPred* pTo_boolPred(CardIter* pCI, Card_RpnCalc* p_rpnCalc_arg);

    void vAdd_rlnConj(Card_RlnConj *p_rlnConj);
    void vBump_expCol(i8 p_conj, bool bSkipFoldExpr=false);
    TypeId xColTypeId(void);
    bool bContains(Symbol* pGivenSymbol);
    void vCopyMarked_rlnConj_To_table_filter(CardIter* pCI, Stz* szWithMark);
    void vCorrect_expCol(Card_Emit* p_arriving);
    bool bCullMarked_rlnConj(Card_CONJUNCTION* p_conj, Stz* szWithMark);
    void vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn);
    void vEnsureTypesFor_expCol(CardIter* pCI);
    Card_RlnConj * pFind_rlnConj(Stz* szWithPpy);
    Card_RlnConj* pFind_rlnConj_With_sqyCall(CardIter* pCI);
    bool bHas_term(Symbol* pWithSymbol);
    bool bIsBraced(void);
    bool bIsCompound(void);
    bool bIsLiterals(void);
    bool bIsScalarCol(void);
    bool bIsScalarKey(void);
    i8   nMarked_conj(Stz* szWithMark);
    void vMark_conj_Wrt_table(CardIter* pCI,Stz* szWithMark);
    void vOntoRpnExpr(String* psRpnExpr);
    void vRemove_rlnConj(Card_RlnConj *p_rlnConj);
    Card_CONJUNCTION* pRoot_conj(void);
    void vShortForm(String* pString);

    Card_RlnConj * pYank_rlnConj(Stz* szWithMark);
    Card_RlnConj * pYank_rlnConj(Stz* szWithMark, Card* p_test);

    void vX_expCol_To_tblCol(CardIter* pCI);
    void vX_tblCol_To_expCol(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_BoolPred* _pCard_BoolPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
