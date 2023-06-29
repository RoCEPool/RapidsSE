/*============================================================================*/
/* XF_table_Analysis.cpp -- Table Analysis                                    */
/*============================================================================*/

#include "XF_table_Analysis.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_table_Analysis, CN_XF_TABLE_ANALYSIS)
__aNewFunction(XF_table_Analysis, CN_XF_TABLE_ANALYSIS)
__vDeleteFunction(XF_table_Analysis, CN_XF_TABLE_ANALYSIS)
__aFooterFunction(XF_table_Analysis)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_table_Analysis::XF_table_Analysis(CardIter* pCI)
: XF_TRANSFORM(CN_XF_TABLE_ANALYSIS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TABLE_ANALYSIS, _FOOTER_);

  _checkInitialised_(CN_XF_TABLE_ANALYSIS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_TABLE_ANALYSIS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_table_Analysis::~XF_table_Analysis(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TABLE_ANALYSIS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_table_Analysis::vXF_table_Analysis(CardIter* pCI)
{
  _static_

  XF_table_Analysis* pXF_table_Analysis = new XF_table_Analysis(pCI);

  pXF_table_Analysis->vXF_table_Analysis();

  delete pXF_table_Analysis;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_table_Analysis::vXF_table_Analysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_TABLE_ANALYSIS, _FOOTER_);


  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
