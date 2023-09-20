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
void XF_Epilog_X_tblCol_To_stmCol::vEpilog(CardIter* pCI)
{
  _static_

  XF_Epilog_X_tblCol_To_stmCol* pXF_tblCol_To_stmCol = new XF_Epilog_X_tblCol_To_stmCol(pCI);

  pXF_tblCol_To_stmCol->vXF_tblCol_To_stmCol();

  delete pXF_tblCol_To_stmCol;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* transform table-columns to stream-columns                                  */
/*----------------------------------------------------------------------------*/
void XF_Epilog_X_tblCol_To_stmCol::vXF_tblCol_To_stmCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_TBLCOL_TO_STMCOL, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_TBLCOL_TO_STMCOL, "transform table-columns to stream-columns"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain_NONE()) {

    if ((m_pCI->G.p_level = m_pCI->G.p_MainCard->pSub_level())) {
      if (AL_PARTITION != m_pCI->G.p_level->pSymbol()->Y_iGet(P_Level))
        continue;
    }

    vConvert_tblColOdn_To_stmColOdn();

//    m_pCI->vPrintBoard(__LINE__);
    vDebugPoint();
  }

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert tblColOdn to stmColOdn                                             */
/*----------------------------------------------------------------------------*/
void XF_Epilog_X_tblCol_To_stmCol::vConvert_tblColOdn_To_stmColOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_TBLCOL_TO_STMCOL, _FOOTER_);

  if (m_pCI->G.p_OP->pSymbol()->Y_bHas(P_BreakPt)) {
    //vDump();
    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if ((m_pCI->G.p_indexList = m_pCI->G.p_OP->pSub_indexList())) {
    m_pCI->G.p_indexList->vConvert_colOdn_To_stmOdn(m_pCI);
  }

  if ((m_pCI->G.p_key = m_pCI->G.p_OP->pSub_key())) {
    if (m_pCI->Prior.m_pProjection)
      m_pCI->G.p_key->vConvert_colOdn_To_stmOdn(m_pCI);
  }

  if (CK_FILTER == m_pCI->G.p_OP->eCard()) {
    m_pCI->G.p_filter->vConvert_colOdn_To_stmOdn(m_pCI);
  }
  else
  if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {
    m_pCI->G.p_filter->vConvert_colOdn_To_stmOdn(m_pCI);
  }

  if ((m_pCI->G.p_projection = m_pCI->G.p_MainCard->pSub_projection())) {
    m_pCI->G.p_projection->vConvert_colOdn_To_stmOdn(m_pCI);
  }

  if ((m_pCI->G.p_hshColList = m_pCI->G.p_MainCard->pSub_hshColList())) {
    if (m_pCI->Prior.m_pProjection)
      m_pCI->G.p_hshColList->vConvert_colOdn_To_stmOdn(m_pCI);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->vSnapPriorProjection();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint())) {
    if (m_pCI->Prior.m_pProjection)
      m_pCI->G.p_constraint->vConvert_colOdn_To_stmOdn(m_pCI);
  }

  if ((m_pCI->G.p_streamKey = m_pCI->G.p_MainCard->pSub_streamKey())) {
    m_pCI->G.p_streamKey->vConvert_colOdn_To_stmOdn(m_pCI);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
