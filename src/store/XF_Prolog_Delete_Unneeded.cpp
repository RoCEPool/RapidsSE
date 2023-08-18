/*============================================================================*/
/* XF_Prolog_Delete_Unneeded.cpp -- Delete Unneeded                                  */
/*============================================================================*/

#include "XF_Prolog_Delete_Unneeded.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Delete_Unneeded, CN_XF_DELETE_UNNEEDED)
__aNewFunction(XF_Prolog_Delete_Unneeded, CN_XF_DELETE_UNNEEDED)
__vDeleteFunction(XF_Prolog_Delete_Unneeded, CN_XF_DELETE_UNNEEDED)
__aFooterFunction(XF_Prolog_Delete_Unneeded)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Delete_Unneeded::XF_Prolog_Delete_Unneeded(CardIter* pCI)
: XF_TRANSFORM(CN_XF_DELETE_UNNEEDED, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DELETE_UNNEEDED, _FOOTER_);

  _checkInitialised_(CN_XF_DELETE_UNNEEDED);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_DELETE_UNNEEDED, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Delete_Unneeded::~XF_Prolog_Delete_Unneeded(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DELETE_UNNEEDED, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Delete_Unneeded::vXF_Delete_Unneeded(CardIter* pCI)
{
  _static_

  XF_Prolog_Delete_Unneeded* pXF_Delete_Unneeded = new XF_Prolog_Delete_Unneeded(pCI);

  pXF_Delete_Unneeded->vXF_Delete_Unneeded();

  delete pXF_Delete_Unneeded;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete superfluous <absJoin>'s                                             */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Delete_Unneeded::vXF_Delete_Unneeded(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_DELETE_UNNEEDED, _FOOTER_);

  m_pCI->vTransform(__func__, XF_DELETE_UNNEEDED, "delete superfluous <absJoin>'s"_Stz);

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
