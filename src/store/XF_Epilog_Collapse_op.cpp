/*============================================================================*/
/* XF_Epilog_Collapse_op.cpp -- Column Mapping                                    */
/*============================================================================*/

#include "XF_Epilog_Collapse_op.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Collapse_op, CN_XF_COLLAPSE_OP)
__aNewFunction(XF_Epilog_Collapse_op, CN_XF_COLLAPSE_OP)
__vDeleteFunction(XF_Epilog_Collapse_op, CN_XF_COLLAPSE_OP)
__aFooterFunction(XF_Epilog_Collapse_op)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Collapse_op::XF_Epilog_Collapse_op(CardIter* pCI)
: XF_EPILOG(CN_XF_COLLAPSE_OP, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLLAPSE_OP, _FOOTER_);

  _checkInitialised_(CN_XF_COLLAPSE_OP);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_COLLAPSE_OP, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Collapse_op::~XF_Epilog_Collapse_op(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLLAPSE_OP, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Collapse_op::vEpilog(CardIter* pCI)
{
  _static_

  XF_Epilog_Collapse_op* pXF_Column_Mapping = new XF_Epilog_Collapse_op(pCI);

  pXF_Column_Mapping->vXF_Columns_required();

  delete pXF_Column_Mapping;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* columns required                                                           */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Collapse_op::vXF_Columns_required(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLLAPSE_OP, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_COLUMN_MAPPING, "column mapping"_Stz);

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
