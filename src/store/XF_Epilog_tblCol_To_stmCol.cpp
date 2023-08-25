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
void XF_Epilog_tblCol_To_stmCol::vXF_tblCol_To_stmCol(CardIter* pCI)
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

  for (m_pCI->vClearRestart(); m_pCI->G_bEachMain(); m_pCI->vSnapPriorProjection()) {

    m_pCI->G.p_OP->vConvert_tblColOdn_To_stmColOdn(m_pCI);
  }
  m_pCI->vEndTransform();

  vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
