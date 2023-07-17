/*============================================================================*/
/* XF_tblCol_To_stmCol.cpp -- Table-Column to Stream-Column                   */
/*============================================================================*/

#include "XF_tblCol_To_stmCol.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_tblCol_To_stmCol, CN_XF_TBLCOL_TO_STMCOL)
__aNewFunction(XF_tblCol_To_stmCol, CN_XF_TBLCOL_TO_STMCOL)
__vDeleteFunction(XF_tblCol_To_stmCol, CN_XF_TBLCOL_TO_STMCOL)
__aFooterFunction(XF_tblCol_To_stmCol)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_tblCol_To_stmCol::XF_tblCol_To_stmCol(CardIter* pCI)
: XF_TRANSFORM(CN_XF_TBLCOL_TO_STMCOL, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TBLCOL_TO_STMCOL, _FOOTER_);

  _checkInitialised_(CN_XF_TBLCOL_TO_STMCOL);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_TBLCOL_TO_STMCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_tblCol_To_stmCol::~XF_tblCol_To_stmCol(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TBLCOL_TO_STMCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_tblCol_To_stmCol::vXF_tblCol_To_stmCol(CardIter* pCI)
{
  _static_

  XF_tblCol_To_stmCol* pXF_tblCol_To_stmCol = new XF_tblCol_To_stmCol(pCI);

  pXF_tblCol_To_stmCol->vXF_tblCol_To_stmCol();

  delete pXF_tblCol_To_stmCol;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* transform table-columns to stream-columns                                  */
/*----------------------------------------------------------------------------*/
void XF_tblCol_To_stmCol::vXF_tblCol_To_stmCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TBLCOL_TO_STMCOL, _FOOTER_);

  m_pCI->vTransform(__func__, XF_TBLCOL_TO_STMCOL, "transform table-columns to stream-columns"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain_reverse()) {

    m_pCI->Analysis.m_pProjection_prior = Card_Projection::pResolvePriorProjection(m_pCI);

    ((Card_OP *) m_pCI->G.p_MainCard)->vConvert_tblColOdn_To_stmColOdn(m_pCI);

//    if (m_pCI->G.p_OP->pSub_projection())
//      m_pCI->Analysis.m_pProjection_prior =
//          m_pCI->G.p_OP->pSub_projection();
  }
  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
