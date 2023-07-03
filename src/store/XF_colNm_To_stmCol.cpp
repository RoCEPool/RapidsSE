/*============================================================================*/
/* XF_colNm_to_stmCol.cpp -- Table-Column to Stream-Column                   */
/*============================================================================*/

#include "XF_colNm_to_stmCol.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_colNm_to_stmCol, CN_XF_TBLCOL_TO_STMCOL)
__aNewFunction(XF_colNm_to_stmCol, CN_XF_TBLCOL_TO_STMCOL)
__vDeleteFunction(XF_colNm_to_stmCol, CN_XF_TBLCOL_TO_STMCOL)
__aFooterFunction(XF_colNm_to_stmCol)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_colNm_to_stmCol::XF_colNm_to_stmCol(CardIter* pCI)
: XF_TRANSFORM(CN_XF_TBLCOL_TO_STMCOL, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TBLCOL_TO_STMCOL, _FOOTER_);

  _checkInitialised_(CN_XF_TBLCOL_TO_STMCOL);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_TBLCOL_TO_STMCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_colNm_to_stmCol::~XF_colNm_to_stmCol(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TBLCOL_TO_STMCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_colNm_to_stmCol::vXF_colNm_to_stmCol(CardIter* pCI)
{
  _static_

  XF_colNm_to_stmCol* pXF_colNm_to_stmCol = new XF_colNm_to_stmCol(pCI);

  pXF_colNm_to_stmCol->vXF_colNm_to_stmCol();

  delete pXF_colNm_to_stmCol;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* transform column-name to stream-column                                     */
/*----------------------------------------------------------------------------*/
void XF_colNm_to_stmCol::vXF_colNm_to_stmCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TBLCOL_TO_STMCOL, _FOOTER_);

  m_pCI->vTransform(__func__, "transform column-name to stream-column"_Stz);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain_reverse()) {

    m_pCI->Analysis.m_pProjection_prior = Card_Projection::pResolvePriorProjection(m_pCI);

    ((Card_OP *) m_pCI->G.p_MainCard)->vConvert_tblColOdn_To_stmColOdn(m_pCI);

  }

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
