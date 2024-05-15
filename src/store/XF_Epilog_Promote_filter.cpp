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

  m_pCI->vTransform(__FILE__, __func__, XF_EPILOG_PROMOTE_FILTER);

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();
  while (m_pCI->bEachOp(CK_o_SCAN, CK_o_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();
  while (m_pCI->bEachOp(CK_o_INDEXSCAN, CK_o_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();
  while (m_pCI->bEachOp(CK_o_INDEXJOIN, CK_o_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();
  while (m_pCI->bEachOp(CK_o_CROSSJOIN, CK_o_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
