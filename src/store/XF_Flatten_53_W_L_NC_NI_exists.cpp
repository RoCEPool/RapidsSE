/*============================================================================*/
/* XF_Flatten_53_W_L_NC_NI_exists.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_53_W_L_NC_NI_exists.hpp"

#include "Fold.hpp"
#include "Smooth.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_53_W_L_NC_NI_exists, CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS)
__aNewFunction(XF_Flatten_53_W_L_NC_NI_exists, CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS)
__vDeleteFunction(XF_Flatten_53_W_L_NC_NI_exists, CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS)
__aFooterFunction(XF_Flatten_53_W_L_NC_NI_exists)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_53_W_L_NC_NI_exists::XF_Flatten_53_W_L_NC_NI_exists(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_53_W_L_NC_NI_exists::~XF_Flatten_53_W_L_NC_NI_exists(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_53_W_L_NC_NI_exists::vXF_Flatten_53_W_L_NC_NI_exists(CardIter* pCI)
{
  _static_

  XF_Flatten_53_W_L_NC_NI_exists* pXF_Flatten_53_W_L_NC_NI_exists = new XF_Flatten_53_W_L_NC_NI_exists(pCI);

  pXF_Flatten_53_W_L_NC_NI_exists->vXF_Flatten_53_W_L_NC_NI_exists();

  delete pXF_Flatten_53_W_L_NC_NI_exists;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_53_W_L_NC_NI_exists::vXF_Flatten_53_W_L_NC_NI_exists(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS, _FOOTER_);

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
