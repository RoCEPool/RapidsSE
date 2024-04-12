/*============================================================================*/
/* XF_Flatten_X_expCol_To_tblCol.cpp -- StmCol to TblCol                                */
/*============================================================================*/

#include "XF_Flatten_X_expCol_To_tblCol.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_X_expCol_To_tblCol, CN_XF_FLATTEN_X_EXPCOL_TO_TBLCOL)
__aNewFunction(XF_Flatten_X_expCol_To_tblCol, CN_XF_FLATTEN_X_EXPCOL_TO_TBLCOL)
__vDeleteFunction(XF_Flatten_X_expCol_To_tblCol, CN_XF_FLATTEN_X_EXPCOL_TO_TBLCOL)
__aFooterFunction(XF_Flatten_X_expCol_To_tblCol)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_X_expCol_To_tblCol::XF_Flatten_X_expCol_To_tblCol(CardIter* m_pCI)
: XF_TRANSFORM(CN_XF_FLATTEN_X_EXPCOL_TO_TBLCOL, m_pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_X_EXPCOL_TO_TBLCOL, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_X_EXPCOL_TO_TBLCOL);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_X_EXPCOL_TO_TBLCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_X_expCol_To_tblCol::~XF_Flatten_X_expCol_To_tblCol(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_X_EXPCOL_TO_TBLCOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_X_expCol_To_tblCol::vFlatten(CardIter* m_pCI)
{
  _static_

  XF_Flatten_X_expCol_To_tblCol* pXF_stmCol_To_tblCol = new XF_Flatten_X_expCol_To_tblCol(m_pCI);

  pXF_stmCol_To_tblCol->vX_expCol_To_tblCol();

  delete pXF_stmCol_To_tblCol;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* all $n become $c.n                                                         */
/*----------------------------------------------------------------------------*/
void XF_Flatten_X_expCol_To_tblCol::vX_expCol_To_tblCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_X_EXPCOL_TO_TBLCOL, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_X_EXPCOL_TO_TBLCOL);

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();

  m_pCI->vBeginEach();
  while (m_pCI->bEachOp()) {
    if ((m_pCI->E.p_level = m_pCI->E.p_Op->pSub_level())) {
      if (AL_PARTITION != m_pCI->E.p_level->pSymbol()->Y_iGet(P_Level))
        continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // conversions prior to projection

    if ((m_pCI->E.p_key = m_pCI->E.p_Op->pSub_key())) {
      if (m_pCI->Arriving.m_pEmit) {
//Z_KEY_DIRECTION
        for (m_pCI->vBeginTarget(m_pCI->E.p_key,CK_EXPR); m_pCI->bEachTarget(); /**/) {
           m_pCI->E.p_expr->vX_expCol_To_tblCol(m_pCI);
        }

        m_pCI->E.p_expr = nullptr;

      }
      m_pCI->E.p_key = nullptr;
    }

    if ((m_pCI->E.p_filter = m_pCI->E.p_Op->pSub_filter())) {
      m_pCI->E.p_filter->vX_expCol_To_tblCol(m_pCI);
      m_pCI->E.p_filter = nullptr;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // projection

    if ((m_pCI->E.p_emit = m_pCI->E.p_Op->pSub_emit())) {
      if (m_pCI->Arriving.m_pEmit)
        m_pCI->E.p_emit->vX_expCol_To_tblCol(m_pCI);
      m_pCI->vSnap_emit_arriving();
      m_pCI->E.p_emit = nullptr;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // conversions after projection

    if ((m_pCI->E.p_constraint = m_pCI->E.p_Op->pSub_constraint())) {
      if (m_pCI->Arriving.m_pEmit)
        m_pCI->E.p_constraint->vX_expCol_To_tblCol(m_pCI);
      m_pCI->E.p_constraint = nullptr;
    }

  }

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
