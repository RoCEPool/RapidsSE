/*============================================================================*/
/* XF_Epilog_Promote_filter.cpp -- Individuate filter                            */
/*============================================================================*/

#include "XF_Epilog_Promote_filter.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Promote_filter, CN_XF_EPILOG_PROMOTE_FILTER)
__aNewFunction(XF_Epilog_Promote_filter, CN_XF_EPILOG_PROMOTE_FILTER)
__vDeleteFunction(XF_Epilog_Promote_filter, CN_XF_EPILOG_PROMOTE_FILTER)
__aFooterFunction(XF_Epilog_Promote_filter)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Promote_filter::XF_Epilog_Promote_filter(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_PROMOTE_FILTER, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_PROMOTE_FILTER, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_PROMOTE_FILTER);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_PROMOTE_FILTER, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Promote_filter::~XF_Epilog_Promote_filter(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_PROMOTE_FILTER, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Promote_filter::vPromote(CardIter* pCI)
{
  _static_

  XF_Epilog_Promote_filter* pXF_Individuate_filter = new XF_Epilog_Promote_filter(pCI);

  pXF_Individuate_filter->vPromote_filter();

  delete pXF_Individuate_filter;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Promote_filter::vPromote_filter(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_PROMOTE_FILTER, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_PROMOTE_FILTER, "individuate <scan|indexJoin>/<filter> as main <filter>"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMainWithSub(CK_m_SCAN, CK_m_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMainWithSub(CK_m_INDEXSCAN, CK_m_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMainWithSub(CK_m_INDEXJOIN, CK_m_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMainWithSub(CK_m_CROSSJOIN, CK_m_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->vEndTransform(__LINE__);
//vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
