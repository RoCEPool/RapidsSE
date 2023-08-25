/*============================================================================*/
/* XF_Prolog_Add_level.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Prolog_Add_level.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Add_level, CN_XF_ADD_LEVEL)
__aNewFunction(XF_Prolog_Add_level, CN_XF_ADD_LEVEL)
__vDeleteFunction(XF_Prolog_Add_level, CN_XF_ADD_LEVEL)
__aFooterFunction(XF_Prolog_Add_level)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Add_level::XF_Prolog_Add_level(CardIter* pCI)
: XF_PROLOG(CN_XF_ADD_LEVEL, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);

  _checkInitialised_(CN_XF_ADD_LEVEL);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Add_level::~XF_Prolog_Add_level(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Add_level::vXF_Add_level(CardIter* pCI)
{
  _static_

  XF_Prolog_Add_level* pXF_Add_level = new XF_Prolog_Add_level(pCI);

  pXF_Add_level->vXF_Add_level();

  delete pXF_Add_level;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Add_level::vXF_Add_level(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_ADD_LEVEL, "add level to op"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain())
    ((Card_OP*)m_pCI->G.p_MainCard)->vAddLevel();

  m_pCI->vEndTransform();
//vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
