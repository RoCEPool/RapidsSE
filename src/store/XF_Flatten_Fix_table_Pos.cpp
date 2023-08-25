/*============================================================================*/
/* XF_Flatten_Fix_table_Pos.cpp -- Fix Table Position                                 */
/*============================================================================*/

#include "XF_Flatten_Fix_table_Pos.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_Fix_table_Pos, CN_XF_ADD_LEVEL)
__aNewFunction(XF_Flatten_Fix_table_Pos, CN_XF_ADD_LEVEL)
__vDeleteFunction(XF_Flatten_Fix_table_Pos, CN_XF_ADD_LEVEL)
__aFooterFunction(XF_Flatten_Fix_table_Pos)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_Fix_table_Pos::XF_Flatten_Fix_table_Pos(CardIter* pCI)
: XF_TRANSFORM(CN_XF_ADD_LEVEL, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);

  _checkInitialised_(CN_XF_ADD_LEVEL);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_Fix_table_Pos::~XF_Flatten_Fix_table_Pos(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Fix_table_Pos::vXF_Fix_table_Pos(CardIter* pCI)
{
  _static_

  XF_Flatten_Fix_table_Pos* pXF_Fix_table_Pos = new XF_Flatten_Fix_table_Pos(pCI);

  pXF_Fix_table_Pos->vXF_Fix_table_Pos();

  delete pXF_Fix_table_Pos;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* fix table position                                                         */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Fix_table_Pos::vXF_Fix_table_Pos(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_ADD_LEVEL, _FOOTER_);

  // fix position of table which uses subquery result as pos 0
  m_pCI->vTransform(__func__, XF_FLATTEN_FIX_TABLE_POS, "fix position of table which uses subquery result"_Stz);
  Card_Table::vFixTablePos(m_pCI);
  m_pCI->vEndTransform();

  vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
