/*============================================================================*/
/* XF_Flatten_stmCol_To_tblCol.cpp -- StmCol to TblCol                                */
/*============================================================================*/

#include "XF_Flatten_stmCol_To_tblCol.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_stmCol_To_tblCol, CN_XF_STMCOL_TO_TBLCOL)
__aNewFunction(XF_Flatten_stmCol_To_tblCol, CN_XF_STMCOL_TO_TBLCOL)
__vDeleteFunction(XF_Flatten_stmCol_To_tblCol, CN_XF_STMCOL_TO_TBLCOL)
__aFooterFunction(XF_Flatten_stmCol_To_tblCol)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_stmCol_To_tblCol::XF_Flatten_stmCol_To_tblCol(CardIter* pCI)
: XF_TRANSFORM(CN_XF_STMCOL_TO_TBLCOL, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_STMCOL_TO_TBLCOL, _FOOTER_);

  _checkInitialised_(CN_XF_STMCOL_TO_TBLCOL);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_STMCOL_TO_TBLCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_stmCol_To_tblCol::~XF_Flatten_stmCol_To_tblCol(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_STMCOL_TO_TBLCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_stmCol_To_tblCol::vXF_stmCol_To_tblCol(CardIter* pCI)
{
  _static_

  XF_Flatten_stmCol_To_tblCol* pXF_stmCol_To_tblCol = new XF_Flatten_stmCol_To_tblCol(pCI);

  pXF_stmCol_To_tblCol->vXF_stmCol_To_tblCol();

  delete pXF_stmCol_To_tblCol;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* all $n become $c.n                                                         */
/*----------------------------------------------------------------------------*/
void XF_Flatten_stmCol_To_tblCol::vXF_stmCol_To_tblCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_STMCOL_TO_TBLCOL, _FOOTER_);

  m_pCI->vTransform(__func__, XF_STMCOL_TO_TBLCOL, "all $n become $c.n"_Stz);

  while (m_pCI->G_bEachMain())
    ((Card_OP *) m_pCI->G.p_MainCard)->vConvert_stmColOdn_To_tblColOdn(m_pCI);

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
