/*============================================================================*/
/* XF_Optimise_Static_calc.cpp -- Optimise Onetime Calculations               */
/*============================================================================*/

#include "XF_Optimise_Static_calc.hpp"

#include "eOT.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_Static_calc, CN_XF_OPTIMISE_STATIC_CALC)
__aNewFunction(XF_Optimise_Static_calc, CN_XF_OPTIMISE_STATIC_CALC)
__vDeleteFunction(XF_Optimise_Static_calc, CN_XF_OPTIMISE_STATIC_CALC)
__aFooterFunction(XF_Optimise_Static_calc)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_Static_calc::XF_Optimise_Static_calc(CardIter* pCI)
: XF_OPTIMISE(CN_XF_OPTIMISE_STATIC_CALC, OT_STATIC_CALC, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_STATIC_CALC, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_STATIC_CALC);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_STATIC_CALC, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_Static_calc::~XF_Optimise_Static_calc(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_STATIC_CALC, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Static_calc::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_Static_calc* pXF_Optimise_OneTime_calc = new XF_Optimise_Static_calc(pCI);

  bool b1 = pXF_Optimise_OneTime_calc->bOptimise();

  pXF_Optimise_OneTime_calc->vOptimised(b1);

  delete pXF_Optimise_OneTime_calc;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Static_calc::bOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_STATIC_CALC, _FOOTER_);

  bool bOptRun = false;

  m_pCI->vTransform(__func__, XF_OPTIMISE_STATIC_CALC, "optimise one time calcs"_Stz);

  m_pCI->vClearRestart();

  //m_pCI->pBoard()->pState()->vInsert(aOT[OT_STATIC_CALC]);

  m_pCI->vEndTransform();

  vValidateBench();

  return bOptRun;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
