/*============================================================================*/
/* XF_Flatten_Smooth.cpp -- XF_Flatten_Smooth                                                       */
/*============================================================================*/

#include "XF_Flatten_Smooth.hpp"

#include "CardIter.hpp"
#include "CarryForward.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_Smooth, CN_XF_FLATTEN_SMOOTH)
__aNewFunction(XF_Flatten_Smooth, CN_XF_FLATTEN_SMOOTH)
__vDeleteFunction(XF_Flatten_Smooth, CN_XF_FLATTEN_SMOOTH)
__aFooterFunction(XF_Flatten_Smooth)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_Smooth::XF_Flatten_Smooth(void)
: Object(CN_XF_FLATTEN_SMOOTH)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SMOOTH, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_SMOOTH);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SMOOTH, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_Smooth::~XF_Flatten_Smooth(void) noexcept(false)
{
_precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SMOOTH, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
