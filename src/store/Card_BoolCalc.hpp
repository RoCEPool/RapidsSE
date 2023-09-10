/*============================================================================*/
/* Card_BoolCalc.hpp -- Boolean Calculation                                   */
/*============================================================================*/
#pragma once

#include "Card_CONJUNCTION.hpp"
#include "Card_TLIST.hpp"
#include "Card_Correlation.hpp"

namespace STORE
{
  class Card_BoolCalc : public Card_TLIST
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_BoolCalc(void);
   ~Card_BoolCalc(void) noexcept(false);

    Card_CmpPred* pCmpPred_whichCallsSubQuery(CardIter* pCI);
    void vBump_stmOdn(i8 nBumpBy, bool bSkipFoldExpr=false);
    eCT  eColumnType(void);
    void vConvert_colOdn_To_stmOdn(CardIter* pCI);
    void vConvert_stmOdn_To_colOdn(CardIter* pCI);
    void vCopyMarkedPredsToTable(CardIter* pCI);
    void vCorrectStmOdn(Card_Projection* p_projection_prior);
    i8 nCullPred(Stz* szWithMark);
    bool bCullPred(Card_CONJUNCTION* p_pred, Stz* szWithMark);
    void vEnsureTypesFor_stmOdn(CardIter* pCI);
    Card_CONJUNCTION* pFindRemove_CorrPred(void);
    bool bIsScalarCol(void);
    bool bIsScalarKey(void);
    bool bHasTerm(Symbol* pSymbol);
    void vMarkWrt_table(CardIter* pCI);
    void vPush_CrlKeyPred_As_CrlKeyTest(Card_CONJUNCTION* p_PRED);
    void vPushPred(Card_CONJUNCTION* p_pred_toBeAdded);
    void vRemovePred(Card_CONJUNCTION* p_pred);
    Card_CONJUNCTION* pRootPred(void);
    bool bYank_PRED(Stz* szWithMark, Card_CONJUNCTION*& p_pred_yanked);
    bool bYank_PRED(Stz* szWithMark, Card* p_test, Card_CONJUNCTION*& p_pred_yanked);


    bool bIsBraced(void);

    Stz* szP_Type(void);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_BoolCalc* _pCard_BoolCalc(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
