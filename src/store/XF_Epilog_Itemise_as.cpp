/*============================================================================*/
/* XF_Epilog_Itemise_as.cpp -- De-Duplicate As (column name)                          */
/*============================================================================*/

#include "XF_Epilog_Itemise_as.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Itemise_as, CN_XF_EPILOG_ITEMISE_AS)
__aNewFunction(XF_Epilog_Itemise_as, CN_XF_EPILOG_ITEMISE_AS)
__vDeleteFunction(XF_Epilog_Itemise_as, CN_XF_EPILOG_ITEMISE_AS)
__aFooterFunction(XF_Epilog_Itemise_as)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Itemise_as::XF_Epilog_Itemise_as(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_ITEMISE_AS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMISE_AS, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_ITEMISE_AS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMISE_AS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Itemise_as::~XF_Epilog_Itemise_as(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMISE_AS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* de-duplicate as (column names)                                             */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Itemise_as::vEpilog(CardIter* pCI)
{
  _static_

  XF_Epilog_Itemise_as* pXF_Dedupe_colNm = new XF_Epilog_Itemise_as(pCI);

  pXF_Dedupe_colNm->vEpilog();

  delete pXF_Dedupe_colNm;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* de-duplicate as (column names)                                             */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Itemise_as::vEpilog(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_ITEMISE_AS, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_DEDUPE_AS, "de-duplicate as (column names)"_Stz);

  m_pCI->vClearRestart();

  while (m_pCI->G_bEachMainWith2Sub(CK_INDEXJOIN,CK_PROJECTION,CK_CONSTRAINT)) {
    m_pCI->G.p_constraint->vDisambiguate(m_pCI);
  }

  m_pCI->vEndTransform();
//vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
