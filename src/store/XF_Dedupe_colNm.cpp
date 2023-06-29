/*============================================================================*/
/* XF_Dedupe_colNm.cpp -- De-Duplicate Column Name                            */
/*============================================================================*/

#include "XF_Dedupe_colNm.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Dedupe_colNm, CN_XF_DEDUPE_COLNM)
__aNewFunction(XF_Dedupe_colNm, CN_XF_DEDUPE_COLNM)
__vDeleteFunction(XF_Dedupe_colNm, CN_XF_DEDUPE_COLNM)
__aFooterFunction(XF_Dedupe_colNm)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Dedupe_colNm::XF_Dedupe_colNm(CardIter* pCI)
: XF_TRANSFORM(CN_XF_DEDUPE_COLNM, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DEDUPE_COLNM, _FOOTER_);

  _checkInitialised_(CN_XF_DEDUPE_COLNM);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_DEDUPE_COLNM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Dedupe_colNm::~XF_Dedupe_colNm(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DEDUPE_COLNM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* de-duplicate column names                                                  */
/*----------------------------------------------------------------------------*/
void XF_Dedupe_colNm::vXF_Dedupe_colNm(CardIter* pCI)
{
  _static_

  XF_Dedupe_colNm* pXF_Dedupe_colNm = new XF_Dedupe_colNm(pCI);

  pXF_Dedupe_colNm->vXF_Dedupe_colNm();

  delete pXF_Dedupe_colNm;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* de-duplicate column names                                                  */
/*----------------------------------------------------------------------------*/
void XF_Dedupe_colNm::vXF_Dedupe_colNm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DEDUPE_COLNM, _FOOTER_);

  m_pCI->vTransform(__func__, "de-duplicate column names"_Stz);

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
