/*============================================================================*/
/* Card_Fold.cpp -- old (delineates the two halves during flattening)         */
/*============================================================================*/

#include "Card_Fold.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Fold, CK_c_FOLD)
__aNewFunction(Card_Fold,CN_CARD)
__vDeleteFunction(Card_Fold,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Fold::Card_Fold(void)
: Card_CONSTRUCTION(CK_c_FOLD, 1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Fold::~Card_Fold(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Fold* Card_Fold::pCraft_fold(Stz* szFoldNm, Stz *szCrlPpy, i8 i_SqyNo)
{
  _static_

  Card_Fold* p_fold = new Card_Fold;

  p_fold->pSymbol()->vApplyFacet(SF_FLD_NM, szFoldNm);

  p_fold->pSymbol()->Y_vSet(szCrlPpy);
  p_fold->pSymbol()->Y_vSet(P_SqyNo,i_SqyNo);

  Card_Level* p_level = new Card_Level(AL_NONE);
  p_fold->vAppendChild(p_level);

  return p_fold;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Fold::vMorph_to_flat(CardIter* pCI)
{
  _static_

  pCI->vBeginEach();
  while (pCI->G_bEachMain(CK_c_FOLD))
    pCI->G.p_fold->vCard(CK_c_FLAT);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Fold::vPush_foldExpr_Into_emit(CardIter* pCI, Card_Expr* p_expr_fold)
{
  _static_

  //pCI->G_vNewMember(CK_EXPR);

  RASSERT(CT_NONE != p_expr_fold->eCTVal());

  Card_Expr* p_expr_new = new Card_Expr;

  p_expr_new->pSymbol()->vApplyFacet(p_expr_fold->pSymbol()->aFacet(SF_SQY_RSL));
  p_expr_new->pSymbol()->Y_vSet(P_Injected);

  Card_RpnCalc* p_rpnCalc_new = new Card_RpnCalc;
  p_expr_new->vAppendChild(p_rpnCalc_new);

  Symbol xSymbol_KeyCol(SF_KEY_COL,1);

  Card_Term* p_term_new = p_expr_fold->pSub_term()->pClone(&xSymbol_KeyCol);
  p_rpnCalc_new->vApplyCast_x(p_expr_fold->xCast());

  p_rpnCalc_new->vAppendChild(p_term_new);
  p_expr_new->vApplyCast_p(p_rpnCalc_new);

  pCI->G.p_emit->vPushChild(p_expr_new);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Fold::vPush_foldExpr_Into_key(CardIter* pCI, Card_Expr* p_expr_fold)
{
  _static_

  pCI->G.p_expr = new Card_Expr;

  pCI->G.p_expr->pSymbol()->Y_vSet(P_Injected);

  pCI->G.p_expr->vAppendChild(p_expr_fold->pSub_rpnCalc()->pDupCardRecursive());
  pCI->G.p_expr->vApplyCast_p(p_expr_fold->pSub_rpnCalc());

  pCI->G.p_key->vPushChild(pCI->G.p_expr);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Fold::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pParent);

//#ifdef WITH_DEBUG_VERSION
//  if ((pCI->G.p_expr = pSub_expr())) {
//    pCI->vPrintBoard(__FILE_NAME__,__func__,__LINE__,true);
//    vDump(__func__);
//    vDebugPoint();
//  }
//#endif
  if (! pSymbol()->Y_bHas(P_CrlSqy) && ! pSymbol()->Y_bHas(P_NonCrlSqy)) {
    pCI->vPrintBoard(__FILE_NAME__,__func__,__LINE__);
    vDump(__func__);
    NEVER_REACH;
  }

  Card::vValidate(pCI,iDepth,pParent);
  Card_CONSTRUCTION::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
