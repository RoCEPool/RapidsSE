/*============================================================================*/
/* Card_BoolCalc.hpp -- Boolean Calculation                                   */
/*============================================================================*/
#pragma once

#include "Card_CONJUNCTION.hpp"
#include "Card_TERMLIST.hpp"
#include "Card_Correlation.hpp"

namespace STORE
{
  class Card_BoolCalc : public Card_TERMLIST
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_BoolCalc(void);
   ~Card_BoolCalc(void) noexcept(false);

    Card_CmpConj* pCmpPred_whichCallsSubQuery(CardIter* pCI);
    void vBump_stmCol(i8 nBumpBy, bool bSkipFoldExpr=false);
    eCT  eColumnType(void);
    void vCopyMarkedPredsToTable(CardIter* pCI);
    void vCorrectStmCol(Card_Emitted* p_emitted_prior);
    i8 nCullPred(Stz* szWithMark);
    bool bCullPred(Card_CONJUNCTION* p_pred, Stz* szWithMark);
    void vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn);
    void vEnsureTypesFor_stmCol(CardIter* pCI);
    Card_CONJUNCTION* pFindRemove_CorrPred(void);
    bool bIsBraced(void);
    bool bIsCompound(void);
    bool bIsLiterals(void);
    bool bIsScalarCol(void);
    bool bIsScalarKey(void);
    bool bHasTerm(Symbol* pSymbol);
    void vMarkWrt_table(CardIter* pCI);
    void vPush_CrlKeyPred_As_CrlKeyTest(Card_CONJUNCTION* p_PRED);
    void vPushPred(Card_CONJUNCTION* p_pred_toBeAdded);
    void vRemovePred(Card_CONJUNCTION* p_pred);
    Card_CONJUNCTION* pRootPred(void);
    void vShortForm(String* pString);

    bool bYank_PRED(Stz* szWithMark, Card_CONJUNCTION*& p_pred_yanked);
    bool bYank_PRED(Stz* szWithMark, Card* p_test, Card_CONJUNCTION*& p_pred_yanked);

    void vX_stmCol_To_tblCol(CardIter* pCI);
    void vX_tblCol_To_stmCol(CardIter* pCI);


    eCT  eColType(void);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_BoolCalc* _pCard_BoolCalc(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
