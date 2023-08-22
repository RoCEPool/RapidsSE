/*============================================================================*/
/* XF_Optimise_aggreg_To_sort_ordAgg.cpp -- Optimise Onetime Calculations              */
/*============================================================================*/

#include "XF_Optimise_aggreg_To_sort_ordAgg.hpp"

#include "eOT.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_aggreg_To_sort_ordAgg, CN_XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG)
__aNewFunction(XF_Optimise_aggreg_To_sort_ordAgg, CN_XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG)
__vDeleteFunction(XF_Optimise_aggreg_To_sort_ordAgg, CN_XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG)
__aFooterFunction(XF_Optimise_aggreg_To_sort_ordAgg)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_aggreg_To_sort_ordAgg::XF_Optimise_aggreg_To_sort_ordAgg(CardIter* pCI)
: XF_TRANSFORM(CN_XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_aggreg_To_sort_ordAgg::~XF_Optimise_aggreg_To_sort_ordAgg(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_aggreg_To_sort_ordAgg::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_aggreg_To_sort_ordAgg* pXF_Optimise_aggreg_To_sort_ordAgg = new XF_Optimise_aggreg_To_sort_ordAgg(pCI);

  pXF_Optimise_aggreg_To_sort_ordAgg->vOptimise();

  delete pXF_Optimise_aggreg_To_sort_ordAgg;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_aggreg_To_sort_ordAgg::vOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG, _FOOTER_);

  m_pCI->vTransform(__func__, XF_OPTIMISE_ONETIME_CALC, "optimise one time calcs"_Stz);

  m_pCI->vClearRestart();

  m_pCI->pBoard()->pState()->vInsert(aOT[OT_AGGREG_TO_SORT_ORDAGG]);

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
