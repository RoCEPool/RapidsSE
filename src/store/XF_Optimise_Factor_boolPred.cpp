/*============================================================================*/
/* XF_Optimise_Factor_boolPred.cpp -- Refactor boolPred                     */
/*============================================================================*/

#include "XF_Optimise_Factor_boolPred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_Factor_boolPred, CN_XF_OPTIMISE_FACTOR_BOOLPRED)
__aNewFunction(XF_Optimise_Factor_boolPred, CN_XF_OPTIMISE_FACTOR_BOOLPRED)
__vDeleteFunction(XF_Optimise_Factor_boolPred, CN_XF_OPTIMISE_FACTOR_BOOLPRED)
__aFooterFunction(XF_Optimise_Factor_boolPred)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_Factor_boolPred::XF_Optimise_Factor_boolPred(CardIter* pCI)
: XF_OPTIMISE(CN_XF_OPTIMISE_FACTOR_BOOLPRED, OT_STATIC_CALC, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_FACTOR_BOOLPRED);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_Factor_boolPred::~XF_Optimise_Factor_boolPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Factor_boolPred::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_Factor_boolPred* pFactor_boolPred = new XF_Optimise_Factor_boolPred(pCI);

  bool b1 = pFactor_boolPred->bOptimise();

  pFactor_boolPred->vOptimised(b1);

  delete pFactor_boolPred;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Factor_boolPred::bOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  bool bOptRun = false;

  m_pCI->vTransform(__func__, XF_OPTIMISE_FACTOR_BOOLPRED, "refactor boolPred"_Stz);

  m_pCI->vClearRestart();

  while (m_pCI->G_bEachMain_NONE()) {
  }

  m_pCI->vEndTransform(__LINE__);

  return bOptRun;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
