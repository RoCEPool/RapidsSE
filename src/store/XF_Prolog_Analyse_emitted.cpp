/*============================================================================*/
/* XF_Prolog_Analyse_emitted_Analysis.cpp -- Analyse Emitted                  */
/*============================================================================*/

#include "XF_Prolog_Analyse_emitted.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Analyse_emitted, CN_XF_PROLOG_ANALYSE_EMITTED)
__aNewFunction(XF_Prolog_Analyse_emitted, CN_XF_PROLOG_ANALYSE_EMITTED)
__vDeleteFunction(XF_Prolog_Analyse_emitted, CN_XF_PROLOG_ANALYSE_EMITTED)
__aFooterFunction(XF_Prolog_Analyse_emitted)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_emitted::XF_Prolog_Analyse_emitted(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_ANALYSE_EMITTED, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_EMITTED, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_ANALYSE_EMITTED);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_EMITTED, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_emitted::~XF_Prolog_Analyse_emitted(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_EMITTED, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_emitted::vAnalyse(CardIter* pCI)
{
  _static_

  XF_Prolog_Analyse_emitted* pXF_Analyse_emitted = new XF_Prolog_Analyse_emitted(pCI);

  pXF_Analyse_emitted->vXF_Analyse_emitted();

  delete pXF_Analyse_emitted;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_emitted::vXF_Analyse_emitted_passThru(CardIter* pCI)
{
  _static_

  XF_Prolog_Analyse_emitted* pXF_projection_Analysis = new XF_Prolog_Analyse_emitted(pCI);

  pXF_projection_Analysis->vXF_projection_Analysis_passThru();

  delete pXF_projection_Analysis;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* projection analysis                                                        */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_emitted::vXF_Analyse_emitted(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_EMITTED, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_ANALYSE_EMITTED, "emitted analysis"_Stz);

  m_pCI->G.pDeck->vAnalyse_emitted(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* projection analysis                                                        */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_emitted::vXF_projection_Analysis_passThru(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_EMITTED, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_ANALYSE_EMITTED, "vAnalyse_emitted_passThru"_Stz);

  m_pCI->G.pDeck->vAnalyse_emitted_passThru(m_pCI);

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
