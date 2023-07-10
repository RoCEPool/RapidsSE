/*============================================================================*/
/* XF_Flatten_24_W_A_C_ro.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_24_W_A_C_ro.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_24_W_A_C_ro, CN_XF_FLATTEN_24_W_A_C_RO)
__aNewFunction(XF_Flatten_24_W_A_C_ro, CN_XF_FLATTEN_24_W_A_C_RO)
__vDeleteFunction(XF_Flatten_24_W_A_C_ro, CN_XF_FLATTEN_24_W_A_C_RO)
__aFooterFunction(XF_Flatten_24_W_A_C_ro)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_24_W_A_C_ro::XF_Flatten_24_W_A_C_ro(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FLATTEN_24_W_A_C_RO, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_24_W_A_C_RO);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_24_W_A_C_ro::~XF_Flatten_24_W_A_C_ro(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_24_W_A_C_ro::vXF_Flatten_24_W_A_C_ro(CardIter* pCI)
{
  _static_

  XF_Flatten_24_W_A_C_ro* pXF_Flatten_24_W_A_C_ro = new XF_Flatten_24_W_A_C_ro(pCI);

  pXF_Flatten_24_W_A_C_ro->vXF_Flatten_24_W_A_C_ro();

  delete pXF_Flatten_24_W_A_C_ro;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_24_W_A_C_ro::vXF_Flatten_24_W_A_C_ro(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);

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
