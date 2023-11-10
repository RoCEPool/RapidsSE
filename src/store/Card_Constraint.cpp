/*============================================================================*/
/* Card_Constraint.cpp -- Filter                                                  */
/*============================================================================*/

#include "Card_Constraint.hpp"

#include "Card_Expr.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_BoolPred.hpp"
#include "Card_RlnConj.hpp"
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
    p_expr->vBump_expCol(nBumpBy, bSkipFoldExpr);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Card_Constraint::nCullMarked_rlnConj(Stz* szWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_BoolPred* p_boolPred = pSub_boolPred();
  i8 nCulled = p_boolPred->nCullMarked_rlnConj(szWithMark);

  return nCulled;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Constraint::vEnsureTypes_For_stmCol(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if ((pCI->G.p_boolPred = pSub_boolPred()))
    pCI->G.p_boolPred->vEnsureTypesFor_expCol(pCI);
  else
  if ((pCI->G.p_rpnCalc = pSub_rpnCalc()))
    pCI->G.p_rpnCalc->vEnsureTypesFor_stmCol(pCI);
}

/*----------------------------------------------------------------------------*/
/* resolve column name from column symbol ($n or $t.c)                        */
/*----------------------------------------------------------------------------*/
void Card_Constraint::vResolve_colNm_From_colSym(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //i8 i_Main = pCI->G.pDeck->iFindChild(pCI->G.p_MainCard);

  Card_BoolPred* p_boolPred = pCI->G.p_constraint->pSub_boolPred();

  Card_CONJUNCTION* p_conj_ROOT = static_cast<Card_CONJUNCTION*>(p_boolPred->pRoot_conj());
  if (! p_conj_ROOT)
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI](Card_CONJUNCTION* p_conj, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;
    if (! p_conj->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;
    Card_Term* p_term = _pCard_Term(p_conj);
    if (! p_term->bIsColSym())
      return C_CONTINUE_RECURSION;

    Stz* szColNm = nullptr;

    Card_Expr* p_expr_resolved;

    Symbol* pSymbol = p_term->pSymbol();
    pCI->G.p_injected = pCI->G.p_MainCard->pSub_injectable();
    p_expr_resolved = pCI->G.p_injected->pFind_expr_Via_term0(pSymbol);

    szColNm = p_expr_resolved->szFacet(SF_NMD_COL);

    p_term->SYM(SF_NMD_COL, szColNm);
    p_term->vMark1(MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_conj_ROOT->bConjRecursive(bLambdaFn);
  UNUSED(bIgnore);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Constraint::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  (void)Card::eValidate_parentEither(pCI,CK_FROM,CK_INDEXJOIN,
                                     CK_KEYEXISTS,CK_KEYNOTEXISTS,
                                     CK_NONE);

  (void)eValidate_mustEither(CK_BOOLPRED,CK_RPNCALC,CK_NONE);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
