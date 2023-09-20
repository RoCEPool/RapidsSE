/*============================================================================*/
/* XF_projection_Analysis.cpp -- Projection Analysis                          */
/*============================================================================*/

#include "XF_Prolog_Analyse_projection.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Analyse_projection, CN_XF_PROLOG_ANALYSE_PROJECTION)
__aNewFunction(XF_Prolog_Analyse_projection, CN_XF_PROLOG_ANALYSE_PROJECTION)
__vDeleteFunction(XF_Prolog_Analyse_projection, CN_XF_PROLOG_ANALYSE_PROJECTION)
__aFooterFunction(XF_Prolog_Analyse_projection)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_projection::XF_Prolog_Analyse_projection(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_ANALYSE_PROJECTION, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_PROJECTION, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_ANALYSE_PROJECTION);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_PROJECTION, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_projection::~XF_Prolog_Analyse_projection(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_PROJECTION, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_projection::vProlog(CardIter* pCI)
{
  _static_

  XF_Prolog_Analyse_projection* pXF_projection_Analysis = new XF_Prolog_Analyse_projection(pCI);

  pXF_projection_Analysis->vXF_projection_Analysis();

  delete pXF_projection_Analysis;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_projection::vXF_projection_Analysis_passThru(CardIter* pCI)
{
  _static_

  XF_Prolog_Analyse_projection* pXF_projection_Analysis = new XF_Prolog_Analyse_projection(pCI);

  pXF_projection_Analysis->vXF_projection_Analysis_passThru();

  delete pXF_projection_Analysis;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* projection analysis                                                        */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_projection::vXF_projection_Analysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_PROJECTION, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_ANALYSE_PROJECTION, "projection analysis"_Stz);

  m_pCI->G.pDeck->vProjectionAnalysis(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* projection analysis                                                        */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_projection::vXF_projection_Analysis_passThru(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_PROJECTION, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_ANALYSE_PROJECTION, "vProjectionAnalysis_passThru"_Stz);

  m_pCI->G.pDeck->vProjectionAnalysis_passThru(m_pCI);

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
