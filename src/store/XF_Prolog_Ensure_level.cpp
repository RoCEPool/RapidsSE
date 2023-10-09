/*============================================================================*/
/* XF_Prolog_Ensure_level.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Prolog_Ensure_level.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Ensure_level, CN_XF_PROLOG_ENSURE_LEVEL)
__aNewFunction(XF_Prolog_Ensure_level, CN_XF_PROLOG_ENSURE_LEVEL)
__vDeleteFunction(XF_Prolog_Ensure_level, CN_XF_PROLOG_ENSURE_LEVEL)
__aFooterFunction(XF_Prolog_Ensure_level)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Ensure_level::XF_Prolog_Ensure_level(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_ENSURE_LEVEL, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ENSURE_LEVEL, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_ENSURE_LEVEL);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ENSURE_LEVEL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Ensure_level::~XF_Prolog_Ensure_level(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ENSURE_LEVEL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Ensure_level::vEnsure(CardIter* pCI)
{
  _static_

  XF_Prolog_Ensure_level* pXF_Add_level = new XF_Prolog_Ensure_level(pCI);

  pXF_Add_level->vEnsure();

  delete pXF_Add_level;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* ensure each op has a level                                                 */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Ensure_level::vEnsure(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ENSURE_LEVEL, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_ENSURE_LEVEL, "ensure op has level"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain_NONE()) {
    Card_Level* p_level = m_pCI->G.p_MainCard->pSub_level();
    if (p_level)
      continue;

    p_level = new Card_Level(AL_PARTITION);
    m_pCI->G.p_MainCard->vPushChild(p_level);
  }

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
