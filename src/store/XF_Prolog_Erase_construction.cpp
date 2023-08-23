/*============================================================================*/
/* XF_Prolog_Erase_construction.cpp -- Delete Unneeded                                  */
/*============================================================================*/

#include "XF_Prolog_Erase_construction.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Erase_construction, CN_XF_ERASE_CONSTRUCTION)
__aNewFunction(XF_Prolog_Erase_construction, CN_XF_ERASE_CONSTRUCTION)
__vDeleteFunction(XF_Prolog_Erase_construction, CN_XF_ERASE_CONSTRUCTION)
__aFooterFunction(XF_Prolog_Erase_construction)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Erase_construction::XF_Prolog_Erase_construction(CardIter* pCI)
: XF_PROLOG(CN_XF_ERASE_CONSTRUCTION, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ERASE_CONSTRUCTION, _FOOTER_);

  _checkInitialised_(CN_XF_ERASE_CONSTRUCTION);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_ERASE_CONSTRUCTION, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Erase_construction::~XF_Prolog_Erase_construction(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ERASE_CONSTRUCTION, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Erase_construction::vXF_Delete_Unneeded(CardIter* pCI)
{
  _static_

  XF_Prolog_Erase_construction* pXF_Delete_Unneeded = new XF_Prolog_Erase_construction(pCI);

  pXF_Delete_Unneeded->vProlog();

  delete pXF_Delete_Unneeded;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete superfluous <absJoin>'s                                             */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Erase_construction::vProlog(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ERASE_CONSTRUCTION, _FOOTER_);

  m_pCI->vTransform(__func__, XF_ERASE_CONSTRUCTION, "erase constructions"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All AbsJoin
  m_pCI->vBeginEach();
  Card_AbsJoin::vDeleteAllAbsJoin(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All Combined
  m_pCI->vBeginEach();
  Card_Combined::vDeleteAllCombined(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All Send
  m_pCI->vClearRestart();
  Card_Send::vDeleteAllSend(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All Stage
  m_pCI->vClearRestart();
  Card_Stage::vDeleteAllStage(m_pCI);

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
