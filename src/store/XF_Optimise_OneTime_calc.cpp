/*============================================================================*/
/* XF_Optimise_OneTime_calc.cpp -- Optimise Onetime Calculations              */
/*============================================================================*/

#include "XF_Optimise_OneTime_calc.hpp"

#include "eOT.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_OneTime_calc, CN_XF_AS_REVIEW)
__aNewFunction(XF_Optimise_OneTime_calc, CN_XF_AS_REVIEW)
__vDeleteFunction(XF_Optimise_OneTime_calc, CN_XF_AS_REVIEW)
__aFooterFunction(XF_Optimise_OneTime_calc)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_OneTime_calc::XF_Optimise_OneTime_calc(CardIter* pCI)
: XF_TRANSFORM(CN_XF_AS_REVIEW, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_AS_REVIEW, _FOOTER_);

  _checkInitialised_(CN_XF_AS_REVIEW);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_AS_REVIEW, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_OneTime_calc::~XF_Optimise_OneTime_calc(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_AS_REVIEW, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_OneTime_calc::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_OneTime_calc* pXF_Optimise_OneTime_calc = new XF_Optimise_OneTime_calc(pCI);

  pXF_Optimise_OneTime_calc->vOptimise();

  delete pXF_Optimise_OneTime_calc;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_OneTime_calc::vOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_AS_REVIEW, _FOOTER_);

  m_pCI->vTransform(__func__, XF_OPTIMISE_ONETIME_CALC, "optimise one time calcs"_Stz);

  m_pCI->vClearRestart();

  m_pCI->pBoard()->pState()->vInsert(aOT[OT_ONETIME_CALCULATIONS]);

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
