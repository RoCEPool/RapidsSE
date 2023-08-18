/*============================================================================*/
/* XF_Optimise_Promote_filter.cpp -- Individuate filter                            */
/*============================================================================*/

#include "XF_Optimise_Promote_filter.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_Promote_filter, CN_XF_INDIVIDUATE_FILTER)
__aNewFunction(XF_Optimise_Promote_filter, CN_XF_INDIVIDUATE_FILTER)
__vDeleteFunction(XF_Optimise_Promote_filter, CN_XF_INDIVIDUATE_FILTER)
__aFooterFunction(XF_Optimise_Promote_filter)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_Promote_filter::XF_Optimise_Promote_filter(CardIter* pCI)
: XF_TRANSFORM(CN_XF_INDIVIDUATE_FILTER,pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XF_INDIVIDUATE_FILTER,_FOOTER_);

  _checkInitialised_(CN_XF_INDIVIDUATE_FILTER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_XF_INDIVIDUATE_FILTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_Promote_filter::~XF_Optimise_Promote_filter(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XF_INDIVIDUATE_FILTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Promote_filter::vXF_Individuate_filter(CardIter* pCI)
{
  _static_

  XF_Optimise_Promote_filter* pXF_Individuate_filter = new XF_Optimise_Promote_filter(pCI);

  pXF_Individuate_filter->vXF_Individuate_filter();

  delete pXF_Individuate_filter;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Promote_filter::vXF_Individuate_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XF_INDIVIDUATE_FILTER,_FOOTER_);

  m_pCI->vTransform(__func__, XF_INDIVIDUATE_FILTER, "individuate <scan|indexJoin>/<filter> as main <filter>"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMainWithSub(CK_SCAN,CK_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMainWithSub(CK_INDEXSCAN,CK_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMainWithSub(CK_INDEXJOIN,CK_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMainWithSub(CK_CROSSJOIN,CK_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
