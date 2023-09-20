/*============================================================================*/
/* XF_Optimise_Reduce_gbyAgg_key.cpp -- Reduce key of gbyAgg                           */
/*============================================================================*/

#include "XF_Optimise_Reduce_gbyAgg_key.hpp"

#include "eOT.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_Reduce_gbyAgg_key, CN_XF_OPTIMISE_REDUCE_GBYAGG_KEY)
__aNewFunction(XF_Optimise_Reduce_gbyAgg_key, CN_XF_OPTIMISE_REDUCE_GBYAGG_KEY)
__vDeleteFunction(XF_Optimise_Reduce_gbyAgg_key, CN_XF_OPTIMISE_REDUCE_GBYAGG_KEY)
__aFooterFunction(XF_Optimise_Reduce_gbyAgg_key)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_Reduce_gbyAgg_key::XF_Optimise_Reduce_gbyAgg_key(CardIter* pCI)
: XF_OPTIMISE(CN_XF_OPTIMISE_REDUCE_GBYAGG_KEY, OT_REDUCE_GBYAGG_KEY, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_REDUCE_GBYAGG_KEY, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_REDUCE_GBYAGG_KEY);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_REDUCE_GBYAGG_KEY, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_Reduce_gbyAgg_key::~XF_Optimise_Reduce_gbyAgg_key(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_REDUCE_GBYAGG_KEY, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Reduce_gbyAgg_key::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_Reduce_gbyAgg_key* pXF_Reduce_key_gbyAgg = new XF_Optimise_Reduce_gbyAgg_key(pCI);

  bool b1 = pXF_Reduce_key_gbyAgg->bOptimise();

  pXF_Reduce_key_gbyAgg->vOptimised(b1);

  delete pXF_Reduce_key_gbyAgg;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Reduce_gbyAgg_key::bOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_REDUCE_GBYAGG_KEY, _FOOTER_);

  bool bOptRun = false;

  m_pCI->vTransform(__func__, XF_OPTIMISE_REDUCE_GBYAGG_KEY, "Reduce gbyAgg key"_Stz);

  ///m_pCI->pBoard()->pState()->vInsert(aOT[OT_REDUCE_GBYAGG_KEY]);

  m_pCI->vEndTransform();
//vValidateBench();

  return bOptRun;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
