/*============================================================================*/
/* XF_Dedupe_as.cpp -- De-Duplicate As (column name)                          */
/*============================================================================*/

#include "XF_Dedupe_as.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Dedupe_as, CN_XF_DEDUPE_AS)
__aNewFunction(XF_Dedupe_as, CN_XF_DEDUPE_AS)
__vDeleteFunction(XF_Dedupe_as, CN_XF_DEDUPE_AS)
__aFooterFunction(XF_Dedupe_as)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Dedupe_as::XF_Dedupe_as(CardIter* pCI)
: XF_TRANSFORM(CN_XF_DEDUPE_AS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DEDUPE_AS, _FOOTER_);

  _checkInitialised_(CN_XF_DEDUPE_AS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_DEDUPE_AS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Dedupe_as::~XF_Dedupe_as(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DEDUPE_AS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* de-duplicate as (column names)                                             */
/*----------------------------------------------------------------------------*/
void XF_Dedupe_as::vXF_Dedupe_as(CardIter* pCI)
{
  _static_

  XF_Dedupe_as* pXF_Dedupe_colNm = new XF_Dedupe_as(pCI);

  pXF_Dedupe_colNm->vXF_Dedupe_as();

  delete pXF_Dedupe_colNm;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* de-duplicate as (column names)                                             */
/*----------------------------------------------------------------------------*/
void XF_Dedupe_as::vXF_Dedupe_as(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DEDUPE_AS, _FOOTER_);

  m_pCI->vTransform(__func__, XF_NONE, "de-duplicate as (column names)"_Stz);

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMain(CK_PROJECT)) {
    m_pCI->G.p_project->vDedupe_as();
  }

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
