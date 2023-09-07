/*============================================================================*/
/* XF_Epilog_tblCol_To_stmCol.cpp -- Table-Column to Stream-Column                   */
/*============================================================================*/

#include "XF_Epilog_tblCol_To_stmCol.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_tblCol_To_stmCol, CN_XF_TBLCOL_TO_STMCOL)
__aNewFunction(XF_Epilog_tblCol_To_stmCol, CN_XF_TBLCOL_TO_STMCOL)
__vDeleteFunction(XF_Epilog_tblCol_To_stmCol, CN_XF_TBLCOL_TO_STMCOL)
__aFooterFunction(XF_Epilog_tblCol_To_stmCol)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_tblCol_To_stmCol::XF_Epilog_tblCol_To_stmCol(CardIter* pCI)
: XF_EPILOG(CN_XF_TBLCOL_TO_STMCOL, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TBLCOL_TO_STMCOL, _FOOTER_);

  _checkInitialised_(CN_XF_TBLCOL_TO_STMCOL);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_TBLCOL_TO_STMCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_tblCol_To_stmCol::~XF_Epilog_tblCol_To_stmCol(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TBLCOL_TO_STMCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_tblCol_To_stmCol::vEpilog(CardIter* pCI)
{
  _static_

  XF_Epilog_tblCol_To_stmCol* pXF_tblCol_To_stmCol = new XF_Epilog_tblCol_To_stmCol(pCI);

  pXF_tblCol_To_stmCol->vXF_tblCol_To_stmCol();

  delete pXF_tblCol_To_stmCol;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* transform table-columns to stream-columns                                  */
/*----------------------------------------------------------------------------*/
void XF_Epilog_tblCol_To_stmCol::vXF_tblCol_To_stmCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TBLCOL_TO_STMCOL, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_TBLCOL_TO_STMCOL, "transform table-columns to stream-columns"_Stz);

  for (m_pCI->vClearRestart(); m_pCI->G_bEachMain_NONE(); m_pCI->vSnapPriorProjection()) {

    if ((m_pCI->G.p_level = m_pCI->G.p_MainCard->pSub_level())) {
      if (AL_PARTITION != m_pCI->G.p_level->pSymbol()->Y_iGet(P_Level))
        continue;
    }

    vConvert_tblColOdn_To_stmColOdn();
  }

  m_pCI->vEndTransform();
//vValidateBench();
}


/*----------------------------------------------------------------------------*/
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert tblColOdn to stmColOdn                                             */
/*----------------------------------------------------------------------------*/
void XF_Epilog_tblCol_To_stmCol::vConvert_tblColOdn_To_stmColOdn2(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TBLCOL_TO_STMCOL, _FOOTER_);

  if (m_pCI->G.p_OP->pSymbol()->Y_bHas(P_BreakPt)) {
    //vDump();
    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // conversions prior to projection

  if ((m_pCI->G.p_key = m_pCI->G.p_OP->pSub_key())) {
    if (m_pCI->Prior.m_pProjection)
      m_pCI->G.p_key->vConvert_colOdn_To_stmOdn(m_pCI);
  }

  if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {
    m_pCI->G.p_filter->vConvert_colOdn_To_stmOdn(m_pCI);
  }

  if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint())) {
    if (m_pCI->Prior.m_pProjection)
      m_pCI->G.p_constraint->vConvert_colOdn_To_stmOdn(m_pCI);
  }
}
/*----------------------------------------------------------------------------*/
void XF_Epilog_tblCol_To_stmCol::vConvert_tblColOdn_To_stmColOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TBLCOL_TO_STMCOL, _FOOTER_);

  if (m_pCI->G.p_OP->pSymbol()->Y_bHas(P_BreakPt)) {
    //vDump();
    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  CardIter* pCI = m_pCI;

  auto bf_tblColOdn_To_stmColOdn = [pCI](Card* pCard)
  {
    Card_Term* p_term = _pCard_Term(pCard);

    if (pCI->G.p_OP->pSymbol()->Y_bHas(P_BreakPt)) {
      pCI->G.p_OP->vDump(__func__);
      vDebugPoint();
    }

    if (!p_term->pSymbol()->bIs_Col_Tc())
      return C_CONTINUE_RECURSION;

    if (pCI->Prior.m_pProjection) {
      if (Card_Expr* p_expr_col = pCI->Prior.m_pProjection->pFindExpr(p_term->pSymbol())) {
        // record what $c.n it was
        //p_term->pSymbol()->Y_vSet(P_TblCol,p_term->szSymbol(SF_NONE));
        p_term->pSymbol()->vSym(SF_COL_NO,p_expr_col->szSymbol(SF_NONE));
        p_term->vMark1(MARK_CHANGED);

        return C_CONTINUE_RECURSION;
      }
    }
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

    i8 nCols_injectedByThisCard = 0;
    pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection();
    if (pCI->G.p_MainCard->bIsCard(CK_CROSSJOIN) ||
        pCI->G.p_MainCard->bIsCard(CK_INDEXJOIN) ||
        pCI->G.p_MainCard->bIsCard(CK_KEYEXISTS) ||
        pCI->G.p_MainCard->bIsCard(CK_KEYNOTEXISTS)) {
      nCols_injectedByThisCard = pCI->G.p_projection->nChildCards();
    }
    else
    if (! (pCI->G.p_MainCard->bIsCard(CK_SCAN) ||
           pCI->G.p_MainCard->bIsCard(CK_INDEXSCAN))) {
      return C_CONTINUE_RECURSION;
    }

    i8 i_Col = p_term->iGetOdn();
    i8 i_ColOdn = nCols_injectedByThisCard + i_Col;

    Symbol xSymbol_stmColOdn(SF_COL_NO, i_ColOdn);
    p_term->pSymbol()->vSym(&xSymbol_stmColOdn);

    p_term->vMark1(MARK_CHANGED);

    //vDump(__func__);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_OP->vApply_lambdaFn_ToAll_term(pCI, bf_tblColOdn_To_stmColOdn);

  if (pCI->G.p_OP->pSymbol()->Y_bHas(P_BreakPt)) {
    //vDump();
    vDebugPoint();
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
