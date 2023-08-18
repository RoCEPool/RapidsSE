/*============================================================================*/
/* XF_OT_Reduce_gbyAgg_key.cpp -- Reduce key of gbyAgg                           */
/*============================================================================*/

#include "XF_OT_Reduce_gbyAgg_key.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_OT_Reduce_gbyAgg_key, CN_XF_REDUCE_KEY_GBYAGG)
__aNewFunction(XF_OT_Reduce_gbyAgg_key, CN_XF_REDUCE_KEY_GBYAGG)
__vDeleteFunction(XF_OT_Reduce_gbyAgg_key, CN_XF_REDUCE_KEY_GBYAGG)
__aFooterFunction(XF_OT_Reduce_gbyAgg_key)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_OT_Reduce_gbyAgg_key::XF_OT_Reduce_gbyAgg_key(CardIter* pCI)
: XF_TRANSFORM(CN_XF_REDUCE_KEY_GBYAGG,pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XF_REDUCE_KEY_GBYAGG,_FOOTER_);

  _checkInitialised_(CN_XF_REDUCE_KEY_GBYAGG);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_XF_REDUCE_KEY_GBYAGG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_OT_Reduce_gbyAgg_key::~XF_OT_Reduce_gbyAgg_key(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XF_REDUCE_KEY_GBYAGG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_OT_Reduce_gbyAgg_key::vXF_Reduce_key_gbyAgg(CardIter* pCI)
{
  _static_

  XF_OT_Reduce_gbyAgg_key* pXF_Reduce_key_gbyAgg = new XF_OT_Reduce_gbyAgg_key(pCI);

  pXF_Reduce_key_gbyAgg->vXF_Reduce_key_gbyAgg();

  delete pXF_Reduce_key_gbyAgg;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_OT_Reduce_gbyAgg_key::vXF_Reduce_key_gbyAgg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XF_REDUCE_KEY_GBYAGG,_FOOTER_);

  vValidate();

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
