/*============================================================================*/
/* XF_Optimise_Duplicate_filter.cpp -- Optimise Onetime Calculations              */
/*============================================================================*/

#include "XF_Optimise_Duplicate_filter.hpp"

#include "eOT.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_Duplicate_filter, CN_XF_OPTIMISE_DUPLICATE_FILTER)
__aNewFunction(XF_Optimise_Duplicate_filter, CN_XF_OPTIMISE_DUPLICATE_FILTER)
__vDeleteFunction(XF_Optimise_Duplicate_filter, CN_XF_OPTIMISE_DUPLICATE_FILTER)
__aFooterFunction(XF_Optimise_Duplicate_filter)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_Duplicate_filter::XF_Optimise_Duplicate_filter(CardIter* pCI)
: XF_TRANSFORM(CN_XF_OPTIMISE_DUPLICATE_FILTER, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_DUPLICATE_FILTER, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_DUPLICATE_FILTER);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_DUPLICATE_FILTER, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_Duplicate_filter::~XF_Optimise_Duplicate_filter(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_DUPLICATE_FILTER, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Duplicate_filter::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_Duplicate_filter* pXF_Optimise_Duplicate_filter = new XF_Optimise_Duplicate_filter(pCI);

  pXF_Optimise_Duplicate_filter->vOptimise();

  delete pXF_Optimise_Duplicate_filter;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Duplicate_filter::vOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_DUPLICATE_FILTER, _FOOTER_);

  m_pCI->vTransform(__func__, XF_OPTIMISE_ONETIME_CALC, "optimise one time calcs"_Stz);

  m_pCI->vClearRestart();

  m_pCI->pBoard()->pState()->vInsert(aOT[OT_DUPLICATE_FILTER]);

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
