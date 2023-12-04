/*============================================================================*/
/* XF_Flatten_52_W_L_NC_I_join.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_52_W_L_NC_I_join.hpp"

#include "XF_Flatten_53_W_L_NC_NI_exists.hpp"
#include "XF_Flatten_53_W_L_NC_NI_join.hpp"

#include "XF_Fold.hpp"
#include "XF_Flatten_Smooth.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_52_W_L_NC_I_join, CN_XF_FLATTEN_52_W_L_NC_I_JOIN)
__aNewFunction(XF_Flatten_52_W_L_NC_I_join, CN_XF_FLATTEN_52_W_L_NC_I_JOIN)
__vDeleteFunction(XF_Flatten_52_W_L_NC_I_join, CN_XF_FLATTEN_52_W_L_NC_I_JOIN)
__aFooterFunction(XF_Flatten_52_W_L_NC_I_join)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_52_W_L_NC_I_join::XF_Flatten_52_W_L_NC_I_join(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FLATTEN_52_W_L_NC_I_JOIN, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_52_W_L_NC_I_JOIN, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_52_W_L_NC_I_JOIN);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_52_W_L_NC_I_JOIN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_52_W_L_NC_I_join::~XF_Flatten_52_W_L_NC_I_join(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_52_W_L_NC_I_JOIN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_52_W_L_NC_I_join::vXF_Flatten_52_W_L_NC_I_join(CardIter* pCI)
{
  _static_

  XF_Fold* pFold = new XF_Fold(0, pCI->G.pDeck);
  i8 nTables = pFold->nTables_In_SubQuery(pCI, "52_W_L_NC_I_join"_Stz, CK_o_WHERE);

  delete pFold;

  if (1 == nTables)
    XF_Flatten_53_W_L_NC_NI_join::vXF_Flatten_53_W_L_NC_NI_join(pCI);
  else
    XF_Flatten_53_W_L_NC_NI_exists::vXF_Flatten_53_W_L_NC_NI_exists(pCI);
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
