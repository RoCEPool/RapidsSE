/*============================================================================*/
/* XF_Prolog_X_colNm_To_stmCol.cpp -- Table-Column to Stream-Column                   */
/*============================================================================*/

#include "XF_Prolog_X_colNm_To_stmCol.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_X_colNm_To_stmCol, CN_XF_EPILOG_X_TBLCOL_TO_STMCOL)
__aNewFunction(XF_Prolog_X_colNm_To_stmCol, CN_XF_EPILOG_X_TBLCOL_TO_STMCOL)
__vDeleteFunction(XF_Prolog_X_colNm_To_stmCol, CN_XF_EPILOG_X_TBLCOL_TO_STMCOL)
__aFooterFunction(XF_Prolog_X_colNm_To_stmCol)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_X_colNm_To_stmCol::XF_Prolog_X_colNm_To_stmCol(CardIter* pCI)
: XF_PROLOG(CN_XF_EPILOG_X_TBLCOL_TO_STMCOL, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_TBLCOL_TO_STMCOL, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_X_TBLCOL_TO_STMCOL);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_TBLCOL_TO_STMCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_X_colNm_To_stmCol::~XF_Prolog_X_colNm_To_stmCol(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_TBLCOL_TO_STMCOL, _FOOTER_);
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
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_TBLCOL_TO_STMCOL, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_COLNM_TO_STMCOL, "transform column-name to stream-column"_Stz);

  for (m_pCI->vClearRestart(); m_pCI->G_bEachMain_NONE(); m_pCI->vSnapPriorProjection()) {
    if (! m_pCI->Prior.m_pProjection)
      continue;

    //m_pCI->Prior.m_pProjection->vDump(__func__);
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
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_TBLCOL_TO_STMCOL, _FOOTER_);

  if (m_pCI->G.p_OP->pSymbol()->Y_bHas(P_BreakPt))
    vDebugPoint();

  RASSERT(m_pCI->Prior.m_pProjection);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  CardIter* pCI = m_pCI;

  auto b_colNm_To_stmCol = [pCI](Card* pCard)
  {
    Card_Term* p_term = _pCard_Term(pCard);
    if (!p_term->pSymbol()->bIs_Col_Nm())
      return C_CONTINUE_RECURSION;

    //p_term->vDump(__func__);

//    // todo: delete
//    if (Card_Expr* p_expr_col =
//        pCI->Prior.m_pProjection->pFind_expr_Of_symbol(p_term->pSymbol())) {
//      //p_expr_col->vDump();
//      p_term->pSymbol()->vSym(p_expr_col->pSymbol());
//    }
//    else
    if (Card_Expr* p_expr_col =
        pCI->Prior.m_pProjection->pFind_expr_By_as(p_term->pSymbol())) {
//      p_expr_col->vDump(__func__);
      p_term->pSymbol()->vSymPpy(p_expr_col->pSymbol());
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_OP->vApply_lambdaFn_ToAll_term(pCI, b_colNm_To_stmCol);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
