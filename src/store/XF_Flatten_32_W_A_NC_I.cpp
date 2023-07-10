/*============================================================================*/
/* XF_Flatten_32_W_A_NC_I.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_32_W_A_NC_I.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_32_W_A_NC_I, CN_XF_FLATTEN_32_W_A_NC_I)
__aNewFunction(XF_Flatten_32_W_A_NC_I, CN_XF_FLATTEN_32_W_A_NC_I)
__vDeleteFunction(XF_Flatten_32_W_A_NC_I, CN_XF_FLATTEN_32_W_A_NC_I)
__aFooterFunction(XF_Flatten_32_W_A_NC_I)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_32_W_A_NC_I::XF_Flatten_32_W_A_NC_I(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FLATTEN_32_W_A_NC_I, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_32_W_A_NC_I, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_32_W_A_NC_I);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_32_W_A_NC_I, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_32_W_A_NC_I::~XF_Flatten_32_W_A_NC_I(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_32_W_A_NC_I, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_32_W_A_NC_I::vXF_Flatten_32_W_A_NC_I(CardIter* pCI)
{
  _static_

  XF_Flatten_32_W_A_NC_I* pXF_Flatten_32_W_A_NC_I = new XF_Flatten_32_W_A_NC_I(pCI);

  pXF_Flatten_32_W_A_NC_I->vXF_Flatten_32_W_A_NC_I();

  delete pXF_Flatten_32_W_A_NC_I;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_32_W_A_NC_I::vXF_Flatten_32_W_A_NC_I(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_32_W_A_NC_I, _FOOTER_);

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
