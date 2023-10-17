/*============================================================================*/
/* XF_Prolog_X_colNm_To_stmCol.cpp -- Table-Column to Stream-Column                   */
/*============================================================================*/

#include "XF_Prolog_X_colNm_To_stmCol.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_X_colNm_To_stmCol, CN_XF_PROLOG_X_COLNM_TO_STMCOL)
__aNewFunction(XF_Prolog_X_colNm_To_stmCol, CN_XF_PROLOG_X_COLNM_TO_STMCOL)
__vDeleteFunction(XF_Prolog_X_colNm_To_stmCol, CN_XF_PROLOG_X_COLNM_TO_STMCOL)
__aFooterFunction(XF_Prolog_X_colNm_To_stmCol)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_X_colNm_To_stmCol::XF_Prolog_X_colNm_To_stmCol(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_X_COLNM_TO_STMCOL, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_COLNM_TO_STMCOL, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_X_COLNM_TO_STMCOL);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_COLNM_TO_STMCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_X_colNm_To_stmCol::~XF_Prolog_X_colNm_To_stmCol(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_COLNM_TO_STMCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_X_colNm_To_stmCol::vXF_colNm_To_stmCol(CardIter* pCI)
{
  _static_

  XF_Prolog_X_colNm_To_stmCol* pXF_colNm_to_stmCol = new XF_Prolog_X_colNm_To_stmCol(pCI);

  pXF_colNm_to_stmCol->vXF_colNm_To_stmCol();

  delete pXF_colNm_to_stmCol;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* transform column-name to stream-column                                     */
/*----------------------------------------------------------------------------*/
void XF_Prolog_X_colNm_To_stmCol::vXF_colNm_To_stmCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_COLNM_TO_STMCOL, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_X_COLNM_TO_STMCOL, "transform column-name to stream-column"_Stz);

  for (m_pCI->vClearRestart(); m_pCI->G_bEachMain_NONE(); m_pCI->vSnapPriorProjection()) {
    if (! m_pCI->Prior.m_pEmitted)
      continue;

    //m_pCI->Prior.m_pEmitted->vDump(__func__);
    vConvert_colNm_To_stmCol();
  }

  m_pCI->vEndTransform();
//vValidateBench();
}

/*----------------------------------------------------------------------------*/
/*-- private ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert colNm to stmCol                                                    */
/*----------------------------------------------------------------------------*/
void XF_Prolog_X_colNm_To_stmCol::vConvert_colNm_To_stmCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_COLNM_TO_STMCOL, _FOOTER_);

  if (m_pCI->G.p_OP->pSymbol()->Y_bHas(P_BreakPt))
    vDebugPoint();

  RASSERT(m_pCI->Prior.m_pEmitted);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  CardIter* pCI = m_pCI;

  auto fb_colNm_To_stmCol = [pCI](Card* pCard)
  {
    Card_Term* p_term_colNm = _pCard_Term(pCard);
    if (! p_term_colNm->pSymbol()->bIs_Col_Nm())
      return C_CONTINUE_RECURSION;

    //p_term_colNm->vDump(__func__);

    if (Card_Expr* p_expr_found =
        pCI->Prior.m_pEmitted->pFind_expr_Via_as(p_term_colNm->pSymbol())) {
      //p_expr_found->vDump(__func__);
      p_term_colNm->pSymbol()->vFacetsPpy(p_expr_found->pSymbol());
      if (Symbol* pSymbol_found = p_expr_found->pFirstTblCol()) {
        //pSymbol_found->vDump();
        p_term_colNm->pSymbol()->vFacet(SF_TBL_COL, pSymbol_found->szSF_chrSeq(SF_NONE));
      }

      //p_term_colNm->vDump(__func__);
      vDebugPoint();
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_OP->vApply_lambdaFn_ToAll_term(pCI, fb_colNm_To_stmCol);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
