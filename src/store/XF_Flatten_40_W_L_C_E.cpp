/*============================================================================*/
/* XF_Flatten_40_W_L_C_E.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_40_W_L_C_E.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_40_W_L_C_E, CN_XF_ADD_LEVEL)
__aNewFunction(XF_Flatten_40_W_L_C_E, CN_XF_ADD_LEVEL)
__vDeleteFunction(XF_Flatten_40_W_L_C_E, CN_XF_ADD_LEVEL)
__aFooterFunction(XF_Flatten_40_W_L_C_E)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_40_W_L_C_E::XF_Flatten_40_W_L_C_E(CardIter* pCI)
: XF_TRANSFORM(CN_XF_ADD_LEVEL, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);

  _checkInitialised_(CN_XF_ADD_LEVEL);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_40_W_L_C_E::~XF_Flatten_40_W_L_C_E(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_40_W_L_C_E::vXF_Flatten_40_W_L_C_E(CardIter* pCI)
{
  _static_

  XF_Flatten_40_W_L_C_E* pXF_Flatten_40_W_L_C_E = new XF_Flatten_40_W_L_C_E(pCI);

  pXF_Flatten_40_W_L_C_E->vXF_Flatten_40_W_L_C_E();

  delete pXF_Flatten_40_W_L_C_E;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_40_W_L_C_E::vXF_Flatten_40_W_L_C_E(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);

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
