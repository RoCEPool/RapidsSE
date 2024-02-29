/*============================================================================*/
/* XF_Flatten_Purge_where.cpp -- Delete Empty Where                           */
/*============================================================================*/

#include "XF_Flatten_Purge_where.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_Purge_where, CN_XF_FLATTEN_PURGE_WHERE)
__aNewFunction(XF_Flatten_Purge_where, CN_XF_FLATTEN_PURGE_WHERE)
__vDeleteFunction(XF_Flatten_Purge_where, CN_XF_FLATTEN_PURGE_WHERE)
__aFooterFunction(XF_Flatten_Purge_where)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_Purge_where::XF_Flatten_Purge_where(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FLATTEN_PURGE_WHERE, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_PURGE_WHERE, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_PURGE_WHERE);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_PURGE_WHERE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_Purge_where::~XF_Flatten_Purge_where(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_PURGE_WHERE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Purge_where::vXF_Delete_Empty_where(CardIter* pCI)
{
  _static_

  XF_Flatten_Purge_where* pXF_Delete_Empty_where = new XF_Flatten_Purge_where(pCI);

  pXF_Delete_Empty_where->vXF_Delete_Empty_where();

  delete pXF_Delete_Empty_where;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete empty where                                                         */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Purge_where::vXF_Delete_Empty_where(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_PURGE_WHERE, _FOOTER_);

  m_pCI->vTransform(__FILE_NAME__,__func__, XF_FLATTEN_PURGE_WHERE, "delete empty where"_Stz);

  Card_Where::vDeleteEmpyWhere(m_pCI);

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
