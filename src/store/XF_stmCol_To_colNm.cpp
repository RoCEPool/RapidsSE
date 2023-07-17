/*============================================================================*/
/* XF_stmCol_To_colNm.cpp -- Stream-Column to Column-Name                     */
/*============================================================================*/

#include "XF_stmCol_To_colNm.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_stmCol_To_colNm, CN_XF_STMCOL_TO_COLNM)
__aNewFunction(XF_stmCol_To_colNm, CN_XF_STMCOL_TO_COLNM)
__vDeleteFunction(XF_stmCol_To_colNm, CN_XF_STMCOL_TO_COLNM)
__aFooterFunction(XF_stmCol_To_colNm)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_stmCol_To_colNm::XF_stmCol_To_colNm(CardIter* pCI)
: XF_TRANSFORM(CN_XF_STMCOL_TO_COLNM, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_STMCOL_TO_COLNM, _FOOTER_);

  _checkInitialised_(CN_XF_STMCOL_TO_COLNM);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_STMCOL_TO_COLNM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_stmCol_To_colNm::~XF_stmCol_To_colNm(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_STMCOL_TO_COLNM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_stmCol_To_colNm::vXF_stmCol_To_colNm(CardIter* pCI)
{
  _static_

  XF_stmCol_To_colNm* pXF_stmCol_to_colNm = new XF_stmCol_To_colNm(pCI);

  pXF_stmCol_to_colNm->vXF_stmCol_To_colNm();

  delete pXF_stmCol_to_colNm;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* stream-column to column-name                                               */
/*----------------------------------------------------------------------------*/
void XF_stmCol_To_colNm::vXF_stmCol_To_colNm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_STMCOL_TO_COLNM, _FOOTER_);

  m_pCI->vTransform(__func__, XF_STMCOL_TO_COLNM, "stream-column to column-name"_Stz);

  m_pCI->vBeginEach();

  while (m_pCI->G_bEachMain()) {

    if (! (   CK_KEYEXISTS    == m_pCI->G.e_MainCard
              || CK_KEYNOTEXISTS == m_pCI->G.e_MainCard))
      continue;

    m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint();

    m_pCI->G.p_constraint->vConvert_stmColOdn_To_stmColNm(m_pCI);
  }

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
