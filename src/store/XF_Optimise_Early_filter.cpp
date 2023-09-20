/*============================================================================*/
/* XF_Optimise_Early_filter.cpp -- Optimise early filter                      */
/*============================================================================*/

#include "XF_Optimise_Early_filter.hpp"

#include "eOT.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_Early_filter, CN_XF_OPTIMISE_EARLY_FILTER)
__aNewFunction(XF_Optimise_Early_filter, CN_XF_OPTIMISE_EARLY_FILTER)
__vDeleteFunction(XF_Optimise_Early_filter, CN_XF_OPTIMISE_EARLY_FILTER)
__aFooterFunction(XF_Optimise_Early_filter)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_Early_filter::XF_Optimise_Early_filter(CardIter* pCI)
: XF_OPTIMISE(CN_XF_OPTIMISE_EARLY_FILTER, OT_EARLY_FILTER, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_FILTER, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_EARLY_FILTER);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_FILTER, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_Early_filter::~XF_Optimise_Early_filter(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_FILTER, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Early_filter::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_Early_filter* pXF_Optimise_Early_filter = new XF_Optimise_Early_filter(pCI);

  bool b1 = pXF_Optimise_Early_filter->bOptimise();

  pXF_Optimise_Early_filter->vOptimised(b1);

  delete pXF_Optimise_Early_filter;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Early_filter::bOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_EARLY_FILTER, _FOOTER_);

  bool bOptRun = false;

  m_pCI->vTransform(__func__, XF_OPTIMISE_EARLY_FILTER, "optimise early filter"_Stz);

  m_pCI->vClearRestart();

  //m_pCI->pBoard()->pState()->vInsert(aOT[OT_EARLY_FILTER]);

  m_pCI->vEndTransform();
//vValidateBench();

  return bOptRun;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
