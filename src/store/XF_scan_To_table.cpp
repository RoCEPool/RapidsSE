/*============================================================================*/
/* XF_scan_To_table.cpp -- Scan Becomes Table                            */
/*============================================================================*/

#include "XF_scan_To_table.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_scan_To_table, CN_XF_SCAN_BECOMES_TABLE)
__aNewFunction(XF_scan_To_table, CN_XF_SCAN_BECOMES_TABLE)
__vDeleteFunction(XF_scan_To_table, CN_XF_SCAN_BECOMES_TABLE)
__aFooterFunction(XF_scan_To_table)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_scan_To_table::XF_scan_To_table(CardIter* pCI)
: XF_TRANSFORM(CN_XF_SCAN_BECOMES_TABLE, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SCAN_BECOMES_TABLE, _FOOTER_);

  _checkInitialised_(CN_XF_SCAN_BECOMES_TABLE);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_SCAN_BECOMES_TABLE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_scan_To_table::~XF_scan_To_table(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SCAN_BECOMES_TABLE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_scan_To_table::vXF_scan_To_table(CardIter* pCI)
{
  _static_

  XF_scan_To_table* pXF_scan_Becomes_table = new XF_scan_To_table(pCI);

  pXF_scan_Becomes_table->vXF_scan_To_table();

  delete pXF_scan_Becomes_table;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* scan to table                                                              */
/*----------------------------------------------------------------------------*/
void XF_scan_To_table::vXF_scan_To_table(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SCAN_BECOMES_TABLE, _FOOTER_);

  m_pCI->vTransform(__func__, "scan to table"_Stz);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain(CK_SCAN))
    Card_Scan::vScanBecomesTable(m_pCI);

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
