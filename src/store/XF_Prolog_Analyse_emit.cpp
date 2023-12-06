/*============================================================================*/
/* XF_Prolog_Analyse_emit.cpp -- Analyse Emit                                 */
/*============================================================================*/

#include "XF_Prolog_Analyse_emit.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Analyse_emit, CN_XF_PROLOG_ANALYSE_EMIT)
__aNewFunction(XF_Prolog_Analyse_emit, CN_XF_PROLOG_ANALYSE_EMIT)
__vDeleteFunction(XF_Prolog_Analyse_emit, CN_XF_PROLOG_ANALYSE_EMIT)
__aFooterFunction(XF_Prolog_Analyse_emit)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_emit::XF_Prolog_Analyse_emit(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_ANALYSE_EMIT, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_EMIT, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_ANALYSE_EMIT);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_EMIT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_emit::~XF_Prolog_Analyse_emit(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_EMIT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_emit::vAnalyse(CardIter* pCI)
{
  _static_

  XF_Prolog_Analyse_emit* pXF_Analyse_emitted = new XF_Prolog_Analyse_emit(pCI);

  pXF_Analyse_emitted->vXF_Analyse();

  delete pXF_Analyse_emitted;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* projection analysis                                                        */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_emit::vXF_Analyse(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_EMIT, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_ANALYSE_EMIT, "emit analysis"_Stz);

  m_pCI->G.pDeck->vAnalyse_emitted(m_pCI);

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
