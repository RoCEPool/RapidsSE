/*============================================================================*/
/* XF_Epilog_X_tblCol_To_stmCol.cpp -- Table-Column to Stream-Column                   */
/*============================================================================*/

#include "XF_Epilog_X_tblCol_To_stmCol.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_X_tblCol_To_stmCol, CN_XF_EPILOG_X_TBLCOL_TO_STMCOL)
__aNewFunction(XF_Epilog_X_tblCol_To_stmCol, CN_XF_EPILOG_X_TBLCOL_TO_STMCOL)
__vDeleteFunction(XF_Epilog_X_tblCol_To_stmCol, CN_XF_EPILOG_X_TBLCOL_TO_STMCOL)
__aFooterFunction(XF_Epilog_X_tblCol_To_stmCol)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_X_tblCol_To_stmCol::XF_Epilog_X_tblCol_To_stmCol(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_X_TBLCOL_TO_STMCOL, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_TBLCOL_TO_STMCOL, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_X_TBLCOL_TO_STMCOL);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_TBLCOL_TO_STMCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_X_tblCol_To_stmCol::~XF_Epilog_X_tblCol_To_stmCol(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_TBLCOL_TO_STMCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_X_tblCol_To_stmCol::vConvert(CardIter* pCI)
{
  _static_

  XF_Epilog_X_tblCol_To_stmCol* pXF_tblCol_To_stmCol = new XF_Epilog_X_tblCol_To_stmCol(pCI);

  pXF_tblCol_To_stmCol->vX_tblCol_To_expCol();

  delete pXF_tblCol_To_stmCol;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* transform table-columns to stream-columns                                  */
/*----------------------------------------------------------------------------*/
void XF_Epilog_X_tblCol_To_stmCol::vX_tblCol_To_expCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_TBLCOL_TO_STMCOL, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_X_TBLCOL_TO_STMCOL, "transform table-columns to stream-columns"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain_NONE()) {

    if (m_pCI->G.p_MainCard->bIsCard(CK_m_GBYAGG)) {

      if ((m_pCI->G.p_level = m_pCI->G.p_MainCard->pSub_level())) {
        if (AL_PARTITION != m_pCI->G.p_level->pSymbol()->Y_iGet(P_Level)) {
          m_pCI->vSnapFluxArriving();
          continue;
        }
      }
    }
    vTo_stmCol();

//    m_pCI->vPrintBoard(__LINE__);
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
void XF_Epilog_X_tblCol_To_stmCol::vTo_stmCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_TBLCOL_TO_STMCOL, _FOOTER_);

  if (m_pCI->G.p_OP->pSymbol()->Y_bHas(P_BreakPt)) {
    //vDump();
    vDebugPoint();
  }

  if (m_pCI->G.p_MainCard->bIsCard(CK_m_TOP)) {
    //m_pCI->G.p_OP->vDump(__func__);
    vDebugPoint();
  }

  if (m_pCI->G.p_OP->i_Child() == 40)
    vDebugPoint();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->X.m_pExprList_i = m_pCI->G.p_OP->pSub_injectable();
  m_pCI->X.m_pExprList_k = m_pCI->G.p_OP->pSub_key();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if ((m_pCI->G.p_indexList = m_pCI->G.p_OP->pSub_indexList())) {
    m_pCI->G.p_indexList->vX_tblCol_To_expCol(m_pCI);
  }

  if ((m_pCI->G.p_key = m_pCI->G.p_OP->pSub_key())) {
    if (m_pCI->Flux.m_pArriving)
      m_pCI->G.p_key->vX_tblCol_To_expCol(m_pCI);
  }

  if (CK_m_FILTER == m_pCI->G.p_OP->eCard()) {
    m_pCI->G.p_filter->vX_tblCol_To_expCol(m_pCI);
  }
  else
  if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {
    m_pCI->G.p_filter->vX_tblCol_To_expCol(m_pCI);
  }

  if ((m_pCI->G.p_emit = m_pCI->G.p_MainCard->pSub_emit())) {
    m_pCI->G.p_emit->vX_tblCol_To_expCol(m_pCI);
  }

  if ((m_pCI->G.p_hshColList = m_pCI->G.p_MainCard->pSub_hshColList())) {
    if (m_pCI->Flux.m_pArriving)
      m_pCI->G.p_hshColList->vX_tblCol_To_expCol(m_pCI);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint())) {
    if (m_pCI->Flux.m_pArriving)
      m_pCI->G.p_constraint->vX_tblCol_To_expCol(m_pCI);
  }

  m_pCI->vSnapFluxArriving();
  if (m_pCI->G.p_OP->pSub_emit())
    m_pCI->X.m_pExprList_a = m_pCI->G.p_OP->pSub_emit();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint())) {
    if (m_pCI->Flux.m_pArriving)
      m_pCI->G.p_constraint->vX_tblCol_To_expCol(m_pCI);
  }

  if ((m_pCI->G.p_streamKey = m_pCI->G.p_MainCard->pSub_streamKey())) {
    m_pCI->G.p_streamKey->vX_tblCol_To_expCol(m_pCI);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
