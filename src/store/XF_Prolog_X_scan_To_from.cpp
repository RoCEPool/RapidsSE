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
void XF_Prolog_X_scan_To_from::vProlog(CardIter* pCI)
{
  _static_

  XF_Prolog_X_scan_To_from* pXF_scan_Becomes_table = new XF_Prolog_X_scan_To_from(pCI);

  pXF_scan_Becomes_table->vXF_scan_To_table();

  delete pXF_scan_Becomes_table;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* scan to table                                                              */
/*----------------------------------------------------------------------------*/
void XF_Prolog_X_scan_To_from::vXF_scan_To_table(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_SCAN_TO_FROM, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_SCAN_TO_FROM, "scan to table"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain(CK_SCAN))
    m_pCI->G.p_scan->vCard(CK_FROM);

  m_pCI->vEndTransform();

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
