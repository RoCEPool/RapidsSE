/*============================================================================*/
/* XF_Flatten_Anchor_table.cpp -- Fix Table Position                                 */
/*============================================================================*/

#include "XF_Flatten_Anchor_table.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_Anchor_table, CN_XF_FLATTEN_ANCHOR_TABLE)
__aNewFunction(XF_Flatten_Anchor_table, CN_XF_FLATTEN_ANCHOR_TABLE)
__vDeleteFunction(XF_Flatten_Anchor_table, CN_XF_FLATTEN_ANCHOR_TABLE)
__aFooterFunction(XF_Flatten_Anchor_table)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_Anchor_table::XF_Flatten_Anchor_table(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FLATTEN_ANCHOR_TABLE, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_ANCHOR_TABLE, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_ANCHOR_TABLE);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_ANCHOR_TABLE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_Anchor_table::~XF_Flatten_Anchor_table(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_ANCHOR_TABLE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Anchor_table::vXF_Fix_table_Pos(CardIter* pCI)
{
  _static_

  XF_Flatten_Anchor_table* pXF_Fix_table_Pos = new XF_Flatten_Anchor_table(pCI);

  pXF_Fix_table_Pos->vAnchor();

  delete pXF_Fix_table_Pos;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* fix table position                                                         */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Anchor_table::vAnchor(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_ANCHOR_TABLE, _FOOTER_);

  // fix position of table which uses subquery result as pos 0
  m_pCI->vTransform(__func__, XF_FLATTEN_ANCHOR_TABLE, "fix position of table which uses subquery result"_Stz);

  Card_Table::vFixTablePos(m_pCI);

  m_pCI->vEndTransform();
//vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
