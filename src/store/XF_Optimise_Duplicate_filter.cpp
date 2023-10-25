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
: XF_OPTIMISE(CN_XF_OPTIMISE_DUPLICATE_FILTER, OT_DUPLICATE_FILTER, pCI)
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

  bool b1 = pXF_Optimise_Duplicate_filter->bOptimise();

  pXF_Optimise_Duplicate_filter->vOptimised(b1);

  delete pXF_Optimise_Duplicate_filter;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Duplicate_filter::bOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_DUPLICATE_FILTER, _FOOTER_);

  bool bOptRun = false;

  m_pCI->vTransform(__func__, XF_OPTIMISE_DUPLICATE_FILTER, "optimise one time calcs"_Stz);

  m_pCI->vClearRestart();

  //m_pCI->pBoard()->pState()->vInsert(aOT[OT_DUPLICATE_FILTER]);

  m_pCI->vEndTransform(__LINE__);
//vValidateBench();

  return bOptRun;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
