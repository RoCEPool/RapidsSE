/*============================================================================*/
/* XF_index_Analysis.cpp -- As _review                                              */
/*============================================================================*/

#include "XF_index_Analysis.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_index_Analysis, CN_XF_INDEX_ANALYSIS)
__aNewFunction(XF_index_Analysis, CN_XF_INDEX_ANALYSIS)
__vDeleteFunction(XF_index_Analysis, CN_XF_INDEX_ANALYSIS)
__aFooterFunction(XF_index_Analysis)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_index_Analysis::XF_index_Analysis(CardIter* pCI)
: XF_TRANSFORM(CN_XF_INDEX_ANALYSIS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_INDEX_ANALYSIS, _FOOTER_);

  _checkInitialised_(CN_XF_INDEX_ANALYSIS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_INDEX_ANALYSIS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_index_Analysis::~XF_index_Analysis(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_INDEX_ANALYSIS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_index_Analysis::vXF_index_Analysis(CardIter* pCI)
{
  _static_

  XF_index_Analysis* pXF_index_Analysis = new XF_index_Analysis(pCI);

  pXF_index_Analysis->vXF_index_Analysis();

  delete pXF_index_Analysis;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_index_Analysis::vXF_index_Analysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_INDEX_ANALYSIS, _FOOTER_);


  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
