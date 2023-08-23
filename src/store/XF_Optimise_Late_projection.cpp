/*============================================================================*/
/* XF_Optimise_Late_projection.cpp -- Optimise Onetime Calculations              */
/*============================================================================*/

#include "XF_Optimise_Late_projection.hpp"

#include "eOT.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_Late_projection, CN_XF_OPTIMISE_LATE_PROJECTION)
__aNewFunction(XF_Optimise_Late_projection, CN_XF_OPTIMISE_LATE_PROJECTION)
__vDeleteFunction(XF_Optimise_Late_projection, CN_XF_OPTIMISE_LATE_PROJECTION)
__aFooterFunction(XF_Optimise_Late_projection)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_Late_projection::XF_Optimise_Late_projection(CardIter* pCI)
: XF_TRANSFORM(CN_XF_OPTIMISE_LATE_PROJECTION, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_LATE_PROJECTION, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_LATE_PROJECTION);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_LATE_PROJECTION, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_Late_projection::~XF_Optimise_Late_projection(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_LATE_PROJECTION, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Late_projection::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_Late_projection* pXF_Optimise_Late_projection = new XF_Optimise_Late_projection(pCI);

  pXF_Optimise_Late_projection->bOptimise();

  delete pXF_Optimise_Late_projection;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Late_projection::bOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_LATE_PROJECTION, _FOOTER_);

  m_pCI->vTransform(__func__, XF_OPTIMISE_ONETIME_CALC, "optimise one time calcs"_Stz);

  m_pCI->vClearRestart();

  m_pCI->pBoard()->pState()->vInsert(aOT[OT_LATE_PROJECTION]);

  m_pCI->vEndTransform();

  vValidate();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
