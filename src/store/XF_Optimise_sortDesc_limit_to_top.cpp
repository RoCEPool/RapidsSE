/*============================================================================*/
/* XF_Optimise_sortDesc_limit_to_top.cpp -- Optimise Onetime Calculations              */
/*============================================================================*/

#include "XF_Optimise_sortDesc_limit_To_top.hpp"

#include "eOT.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_sortDesc_limit_to_top, CN_XF_OPTIMISE_SORTDESC_LIMIT_TO_TOP)
__aNewFunction(XF_Optimise_sortDesc_limit_to_top, CN_XF_OPTIMISE_SORTDESC_LIMIT_TO_TOP)
__vDeleteFunction(XF_Optimise_sortDesc_limit_to_top, CN_XF_OPTIMISE_SORTDESC_LIMIT_TO_TOP)
__aFooterFunction(XF_Optimise_sortDesc_limit_to_top)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_sortDesc_limit_to_top::XF_Optimise_sortDesc_limit_to_top(CardIter* pCI)
: XF_TRANSFORM(CN_XF_OPTIMISE_SORTDESC_LIMIT_TO_TOP, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_SORTDESC_LIMIT_TO_TOP, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_SORTDESC_LIMIT_TO_TOP);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_SORTDESC_LIMIT_TO_TOP, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_sortDesc_limit_to_top::~XF_Optimise_sortDesc_limit_to_top(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_SORTDESC_LIMIT_TO_TOP, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_sortDesc_limit_to_top::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_sortDesc_limit_to_top* pXF_Optimise_sortDesc_limit_to_top = new XF_Optimise_sortDesc_limit_to_top(pCI);

  pXF_Optimise_sortDesc_limit_to_top->bOptimise();

  delete pXF_Optimise_sortDesc_limit_to_top;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_sortDesc_limit_to_top::bOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_SORTDESC_LIMIT_TO_TOP, _FOOTER_);

  m_pCI->vTransform(__func__, XF_OPTIMISE_ONETIME_CALC, "optimise one time calcs"_Stz);

  m_pCI->vClearRestart();


  m_pCI->pBoard()->pState()->vInsert(aOT[OT_SORTDESC_LIMIT_TO_TOP]);

  m_pCI->vEndTransform();

  vValidate();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
