/*============================================================================*/
/* XF_Flatten_53_W_L_NC_NI_join.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_53_W_L_NC_NI_join.hpp"

#include "Fold.hpp"
#include "Smooth.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_53_W_L_NC_NI_join, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN)
__aNewFunction(XF_Flatten_53_W_L_NC_NI_join, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN)
__vDeleteFunction(XF_Flatten_53_W_L_NC_NI_join, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN)
__aFooterFunction(XF_Flatten_53_W_L_NC_NI_join)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_53_W_L_NC_NI_join::XF_Flatten_53_W_L_NC_NI_join(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FLATTEN_53_W_L_NC_NI_JOIN, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_53_W_L_NC_NI_JOIN);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_53_W_L_NC_NI_join::~XF_Flatten_53_W_L_NC_NI_join(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_53_W_L_NC_NI_join::vXF_Flatten_53_W_L_NC_NI_join(CardIter* pCI)
{
  _static_

  XF_Flatten_53_W_L_NC_NI_join* pXF_Flatten_53_W_L_NC_NI_join = new XF_Flatten_53_W_L_NC_NI_join(pCI);

  pXF_Flatten_53_W_L_NC_NI_join->vXF_Flatten_53_W_L_NC_NI_join();

  delete pXF_Flatten_53_W_L_NC_NI_join;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_53_W_L_NC_NI_join::vXF_Flatten_53_W_L_NC_NI_join(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN, _FOOTER_);

  m_pCI->vTransform(__func__, XF_NONE, "add level to op"_Stz);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain())
    ((Card_OP*)m_pCI->G.p_MainCard)->vAddLevel();

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
