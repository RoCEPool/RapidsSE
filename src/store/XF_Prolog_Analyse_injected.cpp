/*============================================================================*/
/* XF_projection_Analysis.cpp -- Projection Injection                         */
/*============================================================================*/

#include "XF_Prolog_Analyse_injected.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Analyse_injected, CN_XF_PROLOG_ANALYSE_INJECTED)
__aNewFunction(XF_Prolog_Analyse_injected, CN_XF_PROLOG_ANALYSE_INJECTED)
__vDeleteFunction(XF_Prolog_Analyse_injected, CN_XF_PROLOG_ANALYSE_INJECTED)
__aFooterFunction(XF_Prolog_Analyse_injected)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_injected::XF_Prolog_Analyse_injected(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_ANALYSE_INJECTED, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_INJECTED, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_ANALYSE_INJECTED);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_INJECTED, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_injected::~XF_Prolog_Analyse_injected(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_INJECTED, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_injected::vAnalyse(CardIter* pCI)
{
  _static_

  XF_Prolog_Analyse_injected* pXF_projection_Analysis = new XF_Prolog_Analyse_injected(pCI);

  pXF_projection_Analysis->vAnalyse();

  delete pXF_projection_Analysis;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_injected::vAnalyse_passThru(CardIter* pCI)
{
  _static_

  XF_Prolog_Analyse_injected* pXF_projection_Analysis = new XF_Prolog_Analyse_injected(pCI);

  pXF_projection_Analysis->vAnalyse_passThru();

  delete pXF_projection_Analysis;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* projection analysis                                                        */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_injected::vAnalyse(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_INJECTED, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_ANALYSE_PROJECTION, "projection analysis"_Stz);


  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* projection analysis                                                        */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_injected::vAnalyse_passThru(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_INJECTED, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_ANALYSE_PROJECTION, "vProjectionAnalysis_passThru"_Stz);


  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
