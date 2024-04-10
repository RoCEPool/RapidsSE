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

__pCastFunction_Card(STORE, Card_Constraint, CK_s_CONSTRAINT)
__aNewFunction(Card_Constraint,CN_CARD)
__vDeleteFunction(Card_Constraint,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Constraint::Card_Constraint(void)
: Card_CONDITION(CK_s_CONSTRAINT, 2)
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
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
void Card_Constraint::vApplyCast(CardIter *pCI, Card_Emit* p_Dic_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if ((pCI->E.p_boolPred = pSub_boolPred()))
    pCI->E.p_boolPred->vApplyCast(pCI,/*??*/p_Dic_expr);
}

/*----------------------------------------------------------------------------*/
/* resolve column name from column symbol ($n or $t.c)                        */
/*----------------------------------------------------------------------------*/
void Card_Constraint::vResolve_colNm_From_colSym(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //i8 i_Main = pCI->G.p_deck->iFindChild(pCI->G.p_Op);

  Card_BoolPred* p_boolPred = pCI->E.p_constraint->pSub_boolPred();

  Card_CONJ* p_conj_ROOT = static_cast<Card_CONJ*>(p_boolPred->pRoot_conj());
  if (! p_conj_ROOT)
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI](Card_CONJ* p_conj, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;
    if (! p_conj->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;
    Card_Term* p_term = _pCard_Term(p_conj);
    if (! p_term->pSymbol()->bHasFacet(SF_EXP_COL) && !p_term->pSymbol()->bHasFacet(SF_TBL_COL))
      return C_CONTINUE_RECURSION;

    Stz* szColNm = nullptr;

    Card_Expr* p_expr_resolved;

    Symbol* pSymbol = p_term->pSymbol();
    pCI->E.p_injectable = pCI->E.p_Op->pSub_injectable();
    p_expr_resolved = pCI->E.p_injectable->pFind_expr_Matches(pSymbol);

    szColNm = p_expr_resolved->szFacet(SF_NMD_COL);

    p_term->SYM(SF_NMD_COL, szColNm);
     p_term->vMark1(MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_conj_ROOT->bLambdaWalk_conj(bLambdaFn);
  UNUSED(bIgnore);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Constraint::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  (void)Card::eValidate_parentEither(pCI,
                                     CK_o_FROM,
                                     CK_o_INDEXJOIN,
                                     CK_o_KEYEXISTS,
                                     CK_o_KEYNOTEXISTS,
                                     CK_NONE);

  eValidate_mustEither(CK_BOOLPRED, CK_NONE);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
