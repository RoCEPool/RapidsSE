/*============================================================================*/
/* XF_Prolog_X_colNm_To_expCol.cpp -- Table-Column to Stream-Column                   */
/*============================================================================*/

#include "XF_Prolog_X_colNm_To_expCol.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_X_colNm_To_expCol, CN_XF_PROLOG_X_COLNM_TO_EXPCOL)
__aNewFunction(XF_Prolog_X_colNm_To_expCol, CN_XF_PROLOG_X_COLNM_TO_EXPCOL)
__vDeleteFunction(XF_Prolog_X_colNm_To_expCol, CN_XF_PROLOG_X_COLNM_TO_EXPCOL)
__aFooterFunction(XF_Prolog_X_colNm_To_expCol)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_X_colNm_To_expCol::XF_Prolog_X_colNm_To_expCol(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_X_COLNM_TO_EXPCOL, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_COLNM_TO_EXPCOL, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_X_COLNM_TO_EXPCOL);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_COLNM_TO_EXPCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_X_colNm_To_expCol::~XF_Prolog_X_colNm_To_expCol(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_COLNM_TO_EXPCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_X_colNm_To_expCol::vXF_colNm_To_expCol(CardIter* pCI)
{
  _static_

  XF_Prolog_X_colNm_To_expCol* pXF_colNm_to_expCol = new XF_Prolog_X_colNm_To_expCol(pCI);

  pXF_colNm_to_expCol->vXF_colNm_To_expCol();

  delete pXF_colNm_to_expCol;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* transform column-name to expression-column                                 */
/*----------------------------------------------------------------------------*/
void XF_Prolog_X_colNm_To_expCol::vXF_colNm_To_expCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_COLNM_TO_EXPCOL, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_PROLOG_X_COLNM_TO_EXPCOL);

  for (m_pCI->vClearRestart(); m_pCI->bEachMain(); m_pCI->vSnapArriving()) {
    if (! m_pCI->Flux.m_pArriving)
      continue;

    vX_colNm_To_expCol();
  }

  m_pCI->vEndTransform(__LINE__);
//vValidateBench();
}

/*----------------------------------------------------------------------------*/
/*-- private ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert colNm to expCol                                                    */
/*----------------------------------------------------------------------------*/
void XF_Prolog_X_colNm_To_expCol::vX_colNm_To_expCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_COLNM_TO_EXPCOL, _FOOTER_);

  if (m_pCI->G.p_MainCard->pSymbol()->Y_bHas(P_BreakPt))
    vDebugPoint();

  RASSERT(m_pCI->Flux.m_pArriving);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  CardIter* pCI = m_pCI;

  auto fb_colNm_To_expCol = [pCI](Card* pCard)
  {
    Card_Term* p_term_colNm = _pCard_Term(pCard);
    if (!p_term_colNm->pSymbol()->bHasFacet(SF_NMD_COL))
      return C_CONTINUE_RECURSION;

    if (Card_Expr* p_expr_defn =
        pCI->Dic.m_p_expr->pFind_expr_Matches(p_term_colNm->pSymbol())) {
      p_term_colNm->pSymbol()->vDeleteFacet(SF_NMD_COL);
      p_term_colNm->pSymbol()->vApplyCastFacetsPpy(p_expr_defn->pSymbol());
    }

    if (Card_Expr* p_expr_ancestor = (Card_Expr*) pCI->pAncestor(CK_EXPR)) {
      if (p_term_colNm->pSymbol()->bHasFacet(SF_TBL_COL))
        p_expr_ancestor->pSymbol()->vApplyFacet(SF_TBL_COL,p_term_colNm->pSymbol()->szFacet(SF_TBL_COL));
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->bLambdaWalk(pCI->G.p_MainCard, CK_TERM, fb_colNm_To_expCol);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
