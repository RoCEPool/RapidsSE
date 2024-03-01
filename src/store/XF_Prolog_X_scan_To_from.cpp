/*============================================================================*/
/* XF_Prolog_X_scan_To_from.cpp -- Scan Becomes Table                           */
/*============================================================================*/

#include "XF_Prolog_X_scan_To_from.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_X_scan_To_from, CN_XF_PROLOG_X_SCAN_TO_FROM)
__aNewFunction(XF_Prolog_X_scan_To_from, CN_XF_PROLOG_X_SCAN_TO_FROM)
__vDeleteFunction(XF_Prolog_X_scan_To_from, CN_XF_PROLOG_X_SCAN_TO_FROM)
__aFooterFunction(XF_Prolog_X_scan_To_from)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_X_scan_To_from::XF_Prolog_X_scan_To_from(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_X_SCAN_TO_FROM, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_SCAN_TO_FROM, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_X_SCAN_TO_FROM);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_SCAN_TO_FROM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_X_scan_To_from::~XF_Prolog_X_scan_To_from(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_SCAN_TO_FROM, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_X_scan_To_from::vConvert(CardIter* pCI)
{
  _static_

  XF_Prolog_X_scan_To_from* pXF_scan_Becomes_table = new XF_Prolog_X_scan_To_from(pCI);

  pXF_scan_Becomes_table->vX_scan_To_from();

  delete pXF_scan_Becomes_table;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* scan to table                                                              */
/*----------------------------------------------------------------------------*/
void XF_Prolog_X_scan_To_from::vX_scan_To_from(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_SCAN_TO_FROM, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_PROLOG_X_SCAN_TO_FROM);

  m_pCI->vClearRestart();
  while (m_pCI->bEachMain(CK_o_SCAN))
    m_pCI->G.p_scan->vCard(CK_o_FROM);

  m_pCI->vEndTransform(__LINE__);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
