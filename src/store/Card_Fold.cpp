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
Card_Fold* Card_Fold::pCraft_fold(Stz* szFoldNm, i8 i_SqyNo)
{
  _static_

  Card_Fold* p_fold = new Card_Fold;

  p_fold->pSymbol()->vApplyFacet(SF_FLD_NM, szFoldNm);
  p_fold->pSymbol()->vApplyFacet(SF_SQY_NO, i_SqyNo);

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
  while (pCI->bEachOp(CK_c_FOLD))
    pCI->E.p_fold->vCard(CK_c_FLAT);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Fold::vPush_foldExpr_Into_emit(CardIter* pCI, Card_Expr* p_expr_fold)
{
  _static_

  //pCI->vNewMember(CK_EXPR);

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

  pCI->E.p_emit->vPushChild(p_expr_new);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Fold::vPush_foldExpr_Into_key(CardIter* pCI, Card_Expr* p_expr_fold)
{
  _static_

/**Z_KEY_DIRECTION push fold*/
  pCI->E.p_direction = (new Card_Direction) -> SYM(SF_DIRECTION, C_ASC);

  pCI->E.p_expr = new Card_Expr;
  pCI->E.p_direction->vAppendChild(pCI->E.p_expr);

  pCI->E.p_expr->pSymbol()->Y_vSet(P_Injected);

  pCI->E.p_expr->vAppendChild(p_expr_fold->pSub_rpnCalc(C_NOT_NULL)->pDupCardRecursive());
  pCI->E.p_expr->vApplyCast_p(p_expr_fold->pSub_rpnCalc(C_NOT_NULL));

  pCI->E.p_key->vPushChild(pCI->E.p_direction);

  //pCI->G.p_key->vDump(__func__);
  vDebugPoint();
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* derive the fold emit for a (non-correlated?) subquery                      */
/*----------------------------------------------------------------------------*/
void Card_Fold::vDerive_emit_fold(CardIter *pCI, SubQuery *pSubQuery_leftmost)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bCorrelated = pSubQuery_leftmost->nCorrCols();
  UNUSED(bCorrelated);


  pCI->E.p_emit = nullptr;
  for (i8 i_Main = pCI->Flat.m_i_fold - 1; i_Main >= 0; i_Main--) {
    Card *pMain = pCI->E.p_deck->pGetChild(i_Main);
    if ((pCI->E.p_emit = pMain->pSub_emit()))
      break;
  }
  if (! pCI->E.p_emit)
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Expr* p_expr_result = pCI->Dic.m_p_expr->pGet_expr((i8) 0);

  pCI->vAppendNewChild(pCI->E.p_fold, CK_s_STREAM);
  pCI->vAppendNewChild(pCI->E.p_stream, CK_t_EMIT);
  pCI->vAppendNewChild(pCI->E.p_emit, CK_EXPR);

  pCI->vAppendNewChild(pCI->E.p_expr, CK_RPNCALC);
  pCI->vAppendNewChild(pCI->E.p_rpnCalc, CK_TERM, Symbol(SF_EXP_COL, "$1"_Stz));

  pCI->E.p_term->vApplyCast_x(p_expr_result->xCast());

  pCI->E.p_rpnCalc->vApplyCast_x(p_expr_result->xCast());

  pCI->E.p_expr->pSymbol()->vApplyFacet(SF_SQY_RSL, pSubQuery_leftmost->pBoard()->i_Board());
  pCI->E.p_expr->pSymbol()->vApplyFacet(SF_EXP_COL, (i8) 1);
  if (p_expr_result->pSymbol()->bHasFacet(SF_NMD_COL))
    pCI->E.p_expr->pSymbol()->vApplyFacet(SF_NMD_COL, p_expr_result->pSymbol());
  pCI->E.p_expr->vApplyCast_x(p_expr_result->xCast());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Fold::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pParent);

//#ifdef WITH_DEBUG_VERSION
//  if ((pCI->G.p_expr = pSub_expr())) {
//    pCI->PRINTBOARD_dbg();
//    vDump(__func__);
//    vDebugPoint();
//  }
//#endif

  Card::vValidate(pCI,iDepth,pParent);
  Card_CONSTRUCTION::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
