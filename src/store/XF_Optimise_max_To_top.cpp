/*============================================================================*/
/* XF_Optimise_max_To_top.cpp -- Optimise Onetime Calculations              */
/*============================================================================*/

#include "XF_Optimise_max_To_top.hpp"

#include "eOT.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_max_To_top, CN_XF_OPTIMISE_MAX_TO_TOP)
__aNewFunction(XF_Optimise_max_To_top, CN_XF_OPTIMISE_MAX_TO_TOP)
__vDeleteFunction(XF_Optimise_max_To_top, CN_XF_OPTIMISE_MAX_TO_TOP)
__aFooterFunction(XF_Optimise_max_To_top)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_max_To_top::XF_Optimise_max_To_top(CardIter* pCI)
: XF_OPTIMISE(CN_XF_OPTIMISE_MAX_TO_TOP, OT_MAX_TO_TOP, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_MAX_TO_TOP, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_MAX_TO_TOP);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_MAX_TO_TOP, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_max_To_top::~XF_Optimise_max_To_top(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_MAX_TO_TOP, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_max_To_top::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_max_To_top* pXF_Optimise_max_To_top = new XF_Optimise_max_To_top(pCI);

  bool b1 = pXF_Optimise_max_To_top->bOptimise();

  pXF_Optimise_max_To_top->vOptimised(b1);

  delete pXF_Optimise_max_To_top;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_max_To_top::bOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_MAX_TO_TOP, _FOOTER_);

  m_pCI->vTransform(__func__, XF_OPTIMISE_ONETIME_CALC, "optimise one time calcs"_Stz);

  m_pCI->vClearRestart();

  m_pCI->pBoard()->pState()->vInsert(aOT[OT_MAX_TO_TOP]);

  m_pCI->vEndTransform();

  vValidate();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
