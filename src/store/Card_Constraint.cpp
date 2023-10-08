/*============================================================================*/
/* Card_Constraint.cpp -- Filter                                                  */
/*============================================================================*/

#include "Card_Constraint.hpp"

#include "Card_Expr.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_BoolCalc.hpp"
#include "Card_CmpConj.hpp"
#include "Card_Term.hpp"
#include "Card_SubSet.hpp"
#include "Card_AndConj.hpp"
#include "Card_OrConj.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Constraint,CK_CONSTRAINT)
__aNewFunction(Card_Constraint,CN_CARD)
__vDeleteFunction(Card_Constraint,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Constraint::Card_Constraint(void)
: Card_CONDITION(CK_CONSTRAINT, 2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Constraint::~Card_Constraint(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* cull non-used bounds                                                        */
/*----------------------------------------------------------------------------*/
void Card_Constraint::vCullUsedBounds(CardIter* pCI) {

  _static_

  while (pCI->G_bEachMain_NONE()) {
    if ((pCI->G.p_constraint = pCI->G.p_MainCard->pSub_constraint()))
      pCI->G.p_constraint->nCullPred(MARK_AT);
  }
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* acclimate for injected fold expr                                           */
/*----------------------------------------------------------------------------*/
void Card_Constraint::vAcclimate_foldExpr(CardIter* pCI, Card_Expr* p_expr_fold)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  UNUSED(pCI);
  UNUSED(p_expr_fold);

  vBump_Rightwards(1,true);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Constraint::vBump_Rightwards(i8 nBumpBy, bool bSkipFoldExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_Expr* p_expr = pSub_expr())
    p_expr->vBump_stmOdn(nBumpBy, bSkipFoldExpr);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Constraint::vConvert_stmColOdn_To_stmColNm(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_Main = pCI->G.pDeck->iFindChild(pCI->G.p_MainCard);

  Card_BoolCalc* p_boolCalc = pCI->G.p_constraint->pSub_boolCalc();

  Card_CONJUNCTION* p_pred_ROOT = static_cast<Card_CONJUNCTION*>(p_boolCalc->pRootPred());
  if (! p_pred_ROOT)
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI,i_Main](Card_CONJUNCTION* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_pred);
    if (!p_term->pSymbol()->bIs_Col_No())
      return C_CONTINUE_RECURSION;


    Stz* szColNm = nullptr;


    Card_Expr* p_expr_col;

    Symbol* pSymbol = p_term->pSymbol();
    pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection();
    p_expr_col = pCI->G.p_projection->pFind_expr_By_ordinal(pSymbol);



    UNUSED(i_Main);
    //p_expr_col = pCI->pExpr_For_col(p_term,i_Main + 1);
    if (p_expr_col) {
      Card_As* p_as = p_expr_col->pSub_as();
      szColNm = p_as->szSymbol(SF_NONE);
    }
    else {

      pCI->G.p_table = pCI->G.p_MainCard->pSub_table();
      pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection();
      i8 nExprs = pCI->G.p_projection->nChildCards();
      i8 i_Odn = p_term->iGetOdn();
      i8 i_Col_base0 = i_Odn - nExprs - 1;

      if (i_Col_base0 >= 0)
        szColNm = pCI->G.p_table->szColNm(i_Col_base0);
      
      vDebugPoint();

    }

    if (szColNm) {
      p_term->SYM(SF_COL_NM,szColNm);
      p_term->vMark1(MARK_CHANGED);
    }
    else {
      vDebugPoint();
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);
  UNUSED(bIgnore);

}
/*----------------------------------------------------------------------------*/
//void Card_Constraint::vConvert_stmColOdn_To_stmColNm(CardIter* pCI)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
//
//  i8 i_Main = pCI->G.pDeck->iFindChild(pCI->G.p_MainCard);
//
//  Card_BoolCalc* p_boolCalc = pCI->G.p_constraint->pSub_boolCalc();
//  RASSERT(1 == p_boolCalc->nChildCards());
//
//  Card_CONJUNCTION* p_pred_ROOT = static_cast<Card_CONJUNCTION*>(p_boolCalc->pGetChild(0));
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  auto bLambdaFn = [pCI,i_Main](Card_CONJUNCTION* p_pred, bool bUpwards)
//  {
//    if (bUpwards)
//      return C_CONTINUE_RECURSION;
//
//    if (! p_pred->bIsCard(CK_TERM))
//      return C_CONTINUE_RECURSION;
//
//    Card_Term* p_term = _pCard_Term(p_pred);
//    if (!p_term->pSymbol()->bIs_Col_No())
//      return C_CONTINUE_RECURSION;
//
//    Stz* szColNm = nullptr;
//
//    Card_Expr* p_expr_col = pCI->pExpr_For_col(p_term,i_Main + 1);
//    if (p_expr_col) {
//      Card_As* p_as = p_expr_col->pSub_as();
//      szColNm = p_as->szSymbol(SF_NONE);
//    }
//    else {
//
//      pCI->G.p_table = pCI->G.p_MainCard->pSub_table();
//      pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection();
//      i8 nExprs = pCI->G.p_projection->nChildCards();
//      i8 i_Odn = p_term->iGetOdn();
//      i8 i_Col_base0 = i_Odn - nExprs - 1;
//
//      if (i_Col_base0 >= 0)
//        szColNm = pCI->G.p_table->szColNm(i_Col_base0);
//
//      vDebugPoint();
//
//    }
//
//    if (szColNm) {
//      p_term->SYM(SF_COL_NM,szColNm);
//      p_term->vMark1(MARK_CHANGED);
//    }
//
//    return C_CONTINUE_RECURSION;
//  };
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);
//  UNUSED(bIgnore);
//
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Card_Constraint::nCullPred(Stz* szWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_BoolCalc* p_boolCalc = pSub_boolCalc();
  i8 nCulled = p_boolCalc->nCullPred(szWithMark);

  return nCulled;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Constraint::vDisambiguate(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(pCI->G.p_projection);
  pCI->G.p_projection->vDump(__func__);

  for (Kid xRpnCalc(pSub_rpnCalc()); xRpnCalc.bEach(CK_TERM); /**/) {
    Card_Term* p_term = xRpnCalc.p_term();
    if (! p_term->bIsColNm())
      continue;

    //vTrace("Disambiguate: %s\n"_Stz,p_term->szSymbol(SF_NONE));

    Card_Expr* p_expr = pCI->G.p_projection->pFindExprOfAs(p_term->pSymbol());
    if (p_expr) {
      //p_expr->vDump(__func__);

      i8 i_Child = p_expr->pSymbol()->iSF_ordinal() - 1;
      p_term->SYM(SF_AS_NM_syn, i_Child, p_term->pSymbol());
    }
  }

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Constraint::vEnsureTypes_For_stmColOdn(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if ((pCI->G.p_boolCalc = pSub_boolCalc()))
    pCI->G.p_boolCalc->vEnsureTypesFor_stmOdn(pCI);
  else
  if ((pCI->G.p_rpnCalc = pSub_rpnCalc()))
    pCI->G.p_rpnCalc->vEnsureTypesFor_stmOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Constraint::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  eCK eParent = pParent->eCard();

  RASSERT(CK_FROM         == eParent ||
          CK_INDEXJOIN    == eParent ||
          CK_KEYEXISTS    == eParent ||
          CK_KEYNOTEXISTS == eParent );

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
