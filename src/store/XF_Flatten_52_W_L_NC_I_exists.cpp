/*============================================================================*/
/* XF_Flatten_52_W_L_NC_I_exists.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_52_W_L_NC_I_exists.hpp"

#include "XF_Flatten_53_W_L_NC_NI_exists.hpp"

#include "XF_Flatten_Smooth.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_52_W_L_NC_I_exists, CN_XF_FLATTEN_52_W_L_NC_I_EXISTS)
__aNewFunction(XF_Flatten_52_W_L_NC_I_exists, CN_XF_FLATTEN_52_W_L_NC_I_EXISTS)
__vDeleteFunction(XF_Flatten_52_W_L_NC_I_exists, CN_XF_FLATTEN_52_W_L_NC_I_EXISTS)
__aFooterFunction(XF_Flatten_52_W_L_NC_I_exists)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_52_W_L_NC_I_exists::XF_Flatten_52_W_L_NC_I_exists(CardIter* pCI)
: XF_FLATTEN_ABS(CN_XF_FLATTEN_52_W_L_NC_I_EXISTS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_52_W_L_NC_I_EXISTS, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_52_W_L_NC_I_EXISTS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_52_W_L_NC_I_EXISTS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_52_W_L_NC_I_exists::~XF_Flatten_52_W_L_NC_I_exists(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_52_W_L_NC_I_EXISTS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_52_W_L_NC_I_exists::vFlatten(CardIter* pCI)
{
  _static_

  XF_Flatten_53_W_L_NC_NI_exists::vFlatten(pCI);
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
