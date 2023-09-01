/*============================================================================*/
/* XF_Epilog_Itemise_colNm.cpp -- De-Duplicate As (column name)                          */
/*============================================================================*/

#include "XF_Epilog_Itemise_colNm.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Itemise_colNm, CN_XF_EPILOG_ITEMISE_COLNM)
__aNewFunction(XF_Epilog_Itemise_colNm, CN_XF_EPILOG_ITEMISE_COLNM)
__vDeleteFunction(XF_Epilog_Itemise_colNm, CN_XF_EPILOG_ITEMISE_COLNM)
__aFooterFunction(XF_Epilog_Itemise_colNm)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Itemise_colNm::XF_Epilog_Itemise_colNm(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_ITEMISE_COLNM, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMISE_COLNM, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_ITEMISE_COLNM);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMISE_COLNM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Itemise_colNm::~XF_Epilog_Itemise_colNm(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMISE_COLNM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* de-duplicate as (column names)                                             */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Itemise_colNm::vEpilog(CardIter* pCI)
{
  _static_

  XF_Epilog_Itemise_colNm* pXF_Dedupe_colNm = new XF_Epilog_Itemise_colNm(pCI);

  pXF_Dedupe_colNm->vEpilog();

  delete pXF_Dedupe_colNm;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* de-duplicate as (column names)                                             */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Itemise_colNm::vEpilog(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMISE_COLNM, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_ITEMISE_COLNM, "de-duplicate as (column names)"_Stz);

  while (m_pCI->G_bEachMainWith2Sub(CK_INDEXJOIN,CK_PROJECTION,CK_CONSTRAINT)) {
    m_pCI->G.p_constraint->vDisambiguate(m_pCI);
  }

#ifdef Z_ITEMISED_NAMES
  Card_As::vItemise_colNm(m_pCI);
  Card_Term::vItemise_colNm(m_pCI);
#endif

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
