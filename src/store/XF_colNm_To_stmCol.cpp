/*============================================================================*/
/* XF_colNm_To_stmCol.cpp -- Table-Column to Stream-Column                   */
/*============================================================================*/

#include "XF_colNm_To_stmCol.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_colNm_To_stmCol, CN_XF_TBLCOL_TO_STMCOL)
__aNewFunction(XF_colNm_To_stmCol, CN_XF_TBLCOL_TO_STMCOL)
__vDeleteFunction(XF_colNm_To_stmCol, CN_XF_TBLCOL_TO_STMCOL)
__aFooterFunction(XF_colNm_To_stmCol)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_colNm_To_stmCol::XF_colNm_To_stmCol(CardIter* pCI)
: XF_TRANSFORM(CN_XF_TBLCOL_TO_STMCOL, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TBLCOL_TO_STMCOL, _FOOTER_);

  _checkInitialised_(CN_XF_TBLCOL_TO_STMCOL);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_TBLCOL_TO_STMCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_colNm_To_stmCol::~XF_colNm_To_stmCol(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TBLCOL_TO_STMCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_colNm_To_stmCol::vXF_colNm_To_stmCol(CardIter* pCI)
{
  _static_

  XF_colNm_To_stmCol* pXF_colNm_to_stmCol = new XF_colNm_To_stmCol(pCI);

  pXF_colNm_to_stmCol->vXF_colNm_To_stmCol();

  delete pXF_colNm_to_stmCol;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* transform column-name to stream-column                                     */
/*----------------------------------------------------------------------------*/
void XF_colNm_To_stmCol::vXF_colNm_To_stmCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TBLCOL_TO_STMCOL, _FOOTER_);

  m_pCI->vTransform(__func__, XF_COLNM_TO_STMCOL, "transform column-name to stream-column"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain_reverse()) {

    m_pCI->Analysis.m_pProjection_prior = Card_Projection::pResolvePriorProjection(m_pCI);
    m_pCI->Analysis.m_pProjection_prior->vDump(__func__ );
    ((Card_OP *) m_pCI->G.p_MainCard)->vConvert_colNm_To_stmCol(m_pCI);

  }

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
