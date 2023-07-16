/*============================================================================*/
/* XF_projection_Analysis.cpp -- Projection Analysis                          */
/*============================================================================*/

#include "XF_projection_Analysis.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_projection_Analysis, CN_XF_PROJECTION_ANALYSIS)
__aNewFunction(XF_projection_Analysis, CN_XF_PROJECTION_ANALYSIS)
__vDeleteFunction(XF_projection_Analysis, CN_XF_PROJECTION_ANALYSIS)
__aFooterFunction(XF_projection_Analysis)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_projection_Analysis::XF_projection_Analysis(CardIter* pCI)
: XF_TRANSFORM(CN_XF_PROJECTION_ANALYSIS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROJECTION_ANALYSIS, _FOOTER_);

  _checkInitialised_(CN_XF_PROJECTION_ANALYSIS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROJECTION_ANALYSIS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_projection_Analysis::~XF_projection_Analysis(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROJECTION_ANALYSIS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_projection_Analysis::vXF_projection_Analysis(CardIter* pCI)
{
  _static_

  XF_projection_Analysis* pXF_projection_Analysis = new XF_projection_Analysis(pCI);

  pXF_projection_Analysis->vXF_projection_Analysis();

  delete pXF_projection_Analysis;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_projection_Analysis::vXF_projection_Analysis_passThru(CardIter* pCI)
{
  _static_

  XF_projection_Analysis* pXF_projection_Analysis = new XF_projection_Analysis(pCI);

  pXF_projection_Analysis->vXF_projection_Analysis_passThru();

  delete pXF_projection_Analysis;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* projection analysis                                                        */
/*----------------------------------------------------------------------------*/
void XF_projection_Analysis::vXF_projection_Analysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROJECTION_ANALYSIS, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROJECTION_ANALYSIS, "projection analysis"_Stz);

  m_pCI->G.pDeck->vProjectionAnalysis(m_pCI);

  m_pCI->vEndTransform();


  vValidate();
}

/*----------------------------------------------------------------------------*/
/* projection analysis                                                        */
/*----------------------------------------------------------------------------*/
void XF_projection_Analysis::vXF_projection_Analysis_passThru(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROJECTION_ANALYSIS, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROJECTION_ANALYSIS, "vProjectionAnalysis_passThru"_Stz);

  m_pCI->G.pDeck->vProjectionAnalysis_passThru(m_pCI);

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
