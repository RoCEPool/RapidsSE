/*============================================================================*/
/* XF_Epilog_X_tblCol_To_expCol.cpp -- Table-Column to Stream-Column                   */
/*============================================================================*/

#include "XF_Epilog_X_tblCol_To_expCol.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_X_tblCol_To_expCol, CN_XF_EPILOG_X_TBLCOL_TO_EXPCOL)
__aNewFunction(XF_Epilog_X_tblCol_To_expCol, CN_XF_EPILOG_X_TBLCOL_TO_EXPCOL)
__vDeleteFunction(XF_Epilog_X_tblCol_To_expCol, CN_XF_EPILOG_X_TBLCOL_TO_EXPCOL)
__aFooterFunction(XF_Epilog_X_tblCol_To_expCol)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_X_tblCol_To_expCol::XF_Epilog_X_tblCol_To_expCol(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_X_TBLCOL_TO_EXPCOL, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_TBLCOL_TO_EXPCOL, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_X_TBLCOL_TO_EXPCOL);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_TBLCOL_TO_EXPCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_X_tblCol_To_expCol::~XF_Epilog_X_tblCol_To_expCol(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_TBLCOL_TO_EXPCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_X_tblCol_To_expCol::vConvert(CardIter* pCI)
{
  _static_

  XF_Epilog_X_tblCol_To_expCol* pXF_tblCol_To_stmCol = new XF_Epilog_X_tblCol_To_expCol(pCI);

  pXF_tblCol_To_stmCol->vX_tblCol_To_expCol();

  delete pXF_tblCol_To_stmCol;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* transform table-columns to stream-columns                                  */
/*----------------------------------------------------------------------------*/
void XF_Epilog_X_tblCol_To_expCol::vX_tblCol_To_expCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_TBLCOL_TO_EXPCOL, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_EPILOG_X_TBLCOL_TO_EXPCOL);

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();
  while (m_pCI->bEachOp()) {
    if (CK_o_GBYAGG == m_pCI->E.e_Op)
      vDebugPoint();
    if (CK_o_SORT == m_pCI->E.e_Op) {
      //m_pCI->G.p_Op->vDump(__func__);
      vDebugPoint();
    }

    vTo_expCol();
    //m_pCI->PRINTBOARD_dbg();
    //m_pCI->G.p_Op->vDump(__func__);
    vDebugPoint();
  }

  m_pCI->vEndTransform(__LINE__);
}


/*----------------------------------------------------------------------------*/
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* tblCol to stmCol                                                           */
/*----------------------------------------------------------------------------*/
void XF_Epilog_X_tblCol_To_expCol::vTo_expCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_TBLCOL_TO_EXPCOL, _FOOTER_);

  if (m_pCI->E.p_Op->pSymbol()->Y_bHas(P_BreakPt)) {
    //vDump();
    vDebugPoint();
  }

  if (m_pCI->E.p_Op->bIsCard(CK_o_GBYAGG) /*&& m_pCI->G.p_Op->i_Child() == 6*/) {
    //m_pCI->G.p_Op->vDump(__func__);
    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->X.m_pExprList_i = m_pCI->E.p_Op->pSub_injectable();
  m_pCI->X.m_pExprList_k = m_pCI->E.p_Op->pSub_key();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if ((m_pCI->E.p_indexList = m_pCI->E.p_Op->pSub_indexList()))
    m_pCI->E.p_indexList->vX_tblCol_To_expCol(m_pCI);

  if ((m_pCI->E.p_key = m_pCI->E.p_Op->pSub_key())) {
#ifdef Z_KEY_DIRECTION
    for (m_pCI->vBeginTarget(m_pCI->E.p_key,CK_EXPR); m_pCI->bEachTarget(); /**/) {
      m_pCI->E.p_expr->vX_tblCol_To_expCol(m_pCI);
    }
    m_pCI->E.p_expr = nullptr;

//    Kid xKey(m_pCI->E.p_key);
//    while (xKey.bEach(CK_DIRECTION)) {
//      m_pCI->E.p_expr = xKey.p_expr(C_SUB);
//      m_pCI->E.p_expr->vX_tblCol_To_expCol(m_pCI);
//    }
//    m_pCI->E.p_expr = nullptr;

#else
    m_pCI->G.p_key->vX_tblCol_To_expCol(m_pCI);
#endif
  }

  if (CK_o_FILTER == m_pCI->E.p_Op->eCard())
    m_pCI->E.p_filter->vX_tblCol_To_expCol(m_pCI);
  else
  if ((m_pCI->E.p_filter = m_pCI->E.p_Op->pSub_filter()))
    m_pCI->E.p_filter->vX_tblCol_To_expCol(m_pCI);

  if ((m_pCI->E.p_hshColList = m_pCI->E.p_Op->pSub_hshColList()))
    m_pCI->E.p_hshColList->vX_tblCol_To_expCol(m_pCI);

  if ((m_pCI->E.p_emit = m_pCI->E.p_Op->pSub_emit()))
    m_pCI->E.p_emit->vX_tblCol_To_expCol(m_pCI);

  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // aspects of operators which need the emit of the operator
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

  m_pCI->vSnapArriving();
  if (m_pCI->E.p_Op->pSub_emit())
    m_pCI->X.m_pExprList_a = m_pCI->E.p_Op->pSub_emit();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if ((m_pCI->E.p_constraint = m_pCI->E.p_Op->pSub_constraint()))
      m_pCI->E.p_constraint->vX_tblCol_To_expCol(m_pCI);

  if ((m_pCI->E.p_streamKey = m_pCI->E.p_Op->pSub_streamKey()))
    m_pCI->E.p_streamKey->vX_tblCol_To_expCol(m_pCI);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
