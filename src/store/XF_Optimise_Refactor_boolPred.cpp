/*============================================================================*/
/* XF_Optimise_Refactor_boolPred.cpp -- Refactor boolPred                     */
/*============================================================================*/

#include "XF_Optimise_Refactor_boolPred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_Refactor_boolPred, CN_XF_OPTIMISE_REFACTOR_BOOLPRED)
__aNewFunction(XF_Optimise_Refactor_boolPred, CN_XF_OPTIMISE_REFACTOR_BOOLPRED)
__vDeleteFunction(XF_Optimise_Refactor_boolPred, CN_XF_OPTIMISE_REFACTOR_BOOLPRED)
__aFooterFunction(XF_Optimise_Refactor_boolPred)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_Refactor_boolPred::XF_Optimise_Refactor_boolPred(CardIter* pCI)
: XF_OPTIMISE(CN_XF_OPTIMISE_REFACTOR_BOOLPRED, OT_STATIC_CALC, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_REFACTOR_BOOLPRED, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_REFACTOR_BOOLPRED);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_REFACTOR_BOOLPRED, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_Refactor_boolPred::~XF_Optimise_Refactor_boolPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_REFACTOR_BOOLPRED, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Refactor_boolPred::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_Refactor_boolPred* pRefactor_boolPred = new XF_Optimise_Refactor_boolPred(pCI);

  bool b1 = pRefactor_boolPred->bOptimise();

  pRefactor_boolPred->vOptimised(b1);

  delete pRefactor_boolPred;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Refactor_boolPred::bOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_REFACTOR_BOOLPRED, _FOOTER_);

  bool bOptRun = false;

  m_pCI->vTransform(__func__, XF_OPTIMISE_REFACTOR_BOOLPRED, "refactor boolPred"_Stz);

  m_pCI->vClearRestart();

  m_pCI->vEndTransform(__LINE__);

  return bOptRun;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
