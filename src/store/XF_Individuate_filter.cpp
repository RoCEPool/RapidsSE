/*============================================================================*/
/* XF_Individuate_filter.cpp -- Individuate filter                            */
/*============================================================================*/

#include "XF_Individuate_filter.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,XF_Individuate_filter,CN_XF_INDIVIDUATE_FILTER)
__aNewFunction(XF_Individuate_filter,CN_XF_INDIVIDUATE_FILTER)
__vDeleteFunction(XF_Individuate_filter,CN_XF_INDIVIDUATE_FILTER)
__aFooterFunction(XF_Individuate_filter)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Individuate_filter::XF_Individuate_filter(CardIter* pCI)
: XF_TRANSFORM(CN_XF_INDIVIDUATE_FILTER,pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XF_INDIVIDUATE_FILTER,_FOOTER_);

  _checkInitialised_(CN_XF_INDIVIDUATE_FILTER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_XF_INDIVIDUATE_FILTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Individuate_filter::~XF_Individuate_filter(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XF_INDIVIDUATE_FILTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Individuate_filter::vXF_Individuate_filter(CardIter* pCI)
{
  _static_

  XF_Individuate_filter* pXF_Individuate_filter = new XF_Individuate_filter(pCI);

  pXF_Individuate_filter->vXF_Individuate_filter();

  delete pXF_Individuate_filter;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Individuate_filter::vXF_Individuate_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XF_INDIVIDUATE_FILTER,_FOOTER_);

  m_pCI->vTransform(__func__, XF_NONE, "individuate <scan|indexJoin>/<filter> as main <filter>"_Stz);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_SCAN,CK_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_INDEXSCAN,CK_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_INDEXJOIN,CK_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_CROSSJOIN,CK_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
