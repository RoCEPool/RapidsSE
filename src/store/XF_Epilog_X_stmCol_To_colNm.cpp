/*============================================================================*/
/* XF_Epilog_X_stmCol_To_colNm.cpp -- Stream-Column to Column-Name            */
/*============================================================================*/

#include "XF_Epilog_X_stmCol_To_colNm.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_X_stmCol_To_colNm, CN_XF_EPILOG_X_STMCOL_TO_COLNM)
__aNewFunction(XF_Epilog_X_stmCol_To_colNm, CN_XF_EPILOG_X_STMCOL_TO_COLNM)
__vDeleteFunction(XF_Epilog_X_stmCol_To_colNm, CN_XF_EPILOG_X_STMCOL_TO_COLNM)
__aFooterFunction(XF_Epilog_X_stmCol_To_colNm)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_X_stmCol_To_colNm::XF_Epilog_X_stmCol_To_colNm(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_X_STMCOL_TO_COLNM, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_STMCOL_TO_COLNM, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_X_STMCOL_TO_COLNM);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_STMCOL_TO_COLNM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_X_stmCol_To_colNm::~XF_Epilog_X_stmCol_To_colNm(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_STMCOL_TO_COLNM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_X_stmCol_To_colNm::vConvert(CardIter* pCI)
{
  _static_

  XF_Epilog_X_stmCol_To_colNm* pXF_stmCol_to_colNm = new XF_Epilog_X_stmCol_To_colNm(pCI);

  pXF_stmCol_to_colNm->vX_stmCol_to_colNm();

  delete pXF_stmCol_to_colNm;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* stream-column to column-name                                               */
/*----------------------------------------------------------------------------*/
void XF_Epilog_X_stmCol_To_colNm::vX_stmCol_to_colNm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_X_STMCOL_TO_COLNM, _FOOTER_);

  m_pCI->vTransform(__FILE__,__func__, XF_EPILOG_X_STMCOL_TO_COLNM, "stream-column to column-name"_Stz);

  m_pCI->vBeginEach();

  while (m_pCI->G_bEachMain_NONE()) {

//    if (! (   CK_o_KEYEXISTS    == m_pCI->G.e_MainCard
//              || CK_o_KEYNOTEXISTS == m_pCI->G.e_MainCard))
//      continue;

    if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint()))
      m_pCI->G.p_constraint->vResolve_colNm_From_colSym(m_pCI);
  }

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
