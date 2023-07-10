/*============================================================================*/
/* XF_Flatten_10_F.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_10_F.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_10_F, CN_XF_ADD_LEVEL)
__aNewFunction(XF_Flatten_10_F, CN_XF_ADD_LEVEL)
__vDeleteFunction(XF_Flatten_10_F, CN_XF_ADD_LEVEL)
__aFooterFunction(XF_Flatten_10_F)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_10_F::XF_Flatten_10_F(CardIter* pCI)
: XF_TRANSFORM(CN_XF_ADD_LEVEL, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);

  _checkInitialised_(CN_XF_ADD_LEVEL);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_10_F::~XF_Flatten_10_F(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_10_F::vXF_Flatten_10_F(CardIter* pCI)
{
  _static_

  XF_Flatten_10_F* pXF_Flatten_10_F = new XF_Flatten_10_F(pCI);

  pXF_Flatten_10_F->vXF_Flatten_10_F();

  delete pXF_Flatten_10_F;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_10_F::vXF_Flatten_10_F(void)
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
