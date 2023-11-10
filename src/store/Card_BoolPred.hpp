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

    void vAddConj(Card_CONJUNCTION *p_CONJUNCTION);
    void vBump_stmCol(i8 p_conj, bool bSkipFoldExpr=false);
    TypeId xColTypeId(void);
    Card_CONJUNCTION* pConj_root(void);
    bool bContains(Symbol* pGivenSymbol);
    void vCopyMarked_rlnConj_ToTable(CardIter* pCI);
    void vCorrect_expCol(Card_Emit* p_emitted_prior);
    bool bCullMarked_rlnConj(Card_CONJUNCTION* p_conj, Stz* szWithMark);
    void vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn);
    void vEnsureTypesFor_expCol(CardIter* pCI);
    Card_CONJUNCTION* pFind_rlnConj(Stz* szWithPpy);
    bool bHas_term(Symbol* pWithSymbol);
    bool bIsBraced(void);
    bool bIsCompound(void);
    bool bIsLiterals(void);
    bool bIsScalarCol(void);
    bool bIsScalarKey(void);
    i8   nMarked_conj(Stz* szWithMark);
    void vMarkWrt_table(CardIter* pCI);
    void vOntoRpnExpr_boolPred(String* psRpnExpr);
    void vOntoRpnExpr_boolPred_conj(String* psRpnExpr,
                                    Card_CONJSUBROOT *p_CONJSUBROOT);
    void vRemove_conj(Card_CONJUNCTION* p_conj);
    Card_RlnConj* pRlnConj_whichCallsSubQuery(CardIter* pCI);
    void vShortForm(String* pString);

    bool bYank_conj(Stz* szWithMark, Card_CONJUNCTION*& p_conj_yanked);
    bool bYank_conj(Stz* szWithMark, Card* p_test, Card_CONJUNCTION*& p_conj_yanked);

    void vX_expCol_To_tblCol(CardIter* p_conj);
    void vX_tblCol_To_expCol(CardIter* p_conj);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_BoolPred* _pCard_BoolPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
