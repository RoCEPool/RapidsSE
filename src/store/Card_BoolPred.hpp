/*============================================================================*/
/* Card_BoolPred.hpp -- Boolean Predicate                                     */
/*============================================================================*/
#pragma once

#include "Card_CONJUNCTION.hpp"
#include "Card_TERMLIST.hpp"
#include "Card_Correlation.hpp"

namespace STORE
{
  class Card_BoolPred : public Card_TERMLIST
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_BoolPred(void);
   ~Card_BoolPred(void) noexcept(false);

    Card_RlnConj* pCmpConj_whichCallsSubQuery(CardIter* pCI);
    void vBump_stmCol(i8 p_conj, bool bSkipFoldExpr=false);
    eCT  eColumnType(void);
    Card_CONJUNCTION* pConj_root(void);
    void vCopyMarkedConjToTable(CardIter* pCI);
    void vCorrectStmCol(Card_Emit* p_emitted_prior);
    i8 nCullConj(Stz* szWithMark);
    bool bCullConj(Card_CONJUNCTION* p_conj, Stz* szWithMark);
    void vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn);
    void vEnsureTypesFor_stmCol(CardIter* pCI);
    Card_CONJUNCTION* pFindRemove_conj_corr(void);
    bool bIsBraced(void);
    bool bIsCompound(void);
    bool bIsLiterals(void);
    bool bIsScalarCol(void);
    bool bIsScalarKey(void);
    bool bHasTerm(Symbol* pSymbol);
    void vMarkWrt_table(CardIter* pCI);
    void vPush_CrlKeyConj_As_CrlKeyTest(Card_CONJUNCTION* p_conj);
    void vPushConj(Card_CONJUNCTION* p_conj_toBeAdded);
    void vRemove_conj(Card_CONJUNCTION* p_conj);
    void vShortForm(String* pString);

    bool bYank_conj(Stz* szWithMark, Card_CONJUNCTION*& p_conj_yanked);
    bool bYank_conj(Stz* szWithMark, Card* p_test, Card_CONJUNCTION*& p_conj_yanked);

    void vX_stmCol_To_tblCol(CardIter* p_conj);
    void vX_tblCol_To_stmCol(CardIter* p_conj);


    eCT  eColType(void);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_BoolPred* _pCard_BoolPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
