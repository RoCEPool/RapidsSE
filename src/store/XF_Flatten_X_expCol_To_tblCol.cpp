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

  m_pCI->vTransform(__func__, XF_FLATTEN_X_EXPCOL_TO_TBLCOL, "all $n become $c.n"_Stz);

  m_pCI->vClearRestart();

  m_pCI->vBeginEach();
  while (m_pCI->G_bEachMain_NONE()) {
    if ((m_pCI->G.p_level = m_pCI->G.p_MainCard->pSub_level())) {
      if (AL_PARTITION != m_pCI->G.p_level->pSymbol()->Y_iGet(P_Level))
        continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // conversions prior to projection

    if ((m_pCI->G.p_key = m_pCI->G.p_OP->pSub_key())) {
      if (m_pCI->Flux.m_pArriving)
        m_pCI->G.p_key->vX_expCol_To_tblCol(m_pCI);
      m_pCI->G.p_key = nullptr;
    }

    if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {
      m_pCI->G.p_filter->vX_expCol_To_tblCol(m_pCI);
      m_pCI->G.p_filter = nullptr;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // projection

    if ((m_pCI->G.p_emit = m_pCI->G.p_MainCard->pSub_emit())) {
      if (m_pCI->Flux.m_pArriving)
        m_pCI->G.p_emit->vX_expCol_To_tblCol(m_pCI);
      m_pCI->Dic.m_p_expCol = m_pCI->Flux.m_pArriving = m_pCI->G.p_emit;
      m_pCI->G.p_emit = nullptr;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // conversions after projection

    if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint())) {
      if (m_pCI->Flux.m_pArriving)
        m_pCI->G.p_constraint->vX_expCol_To_tblCol(m_pCI);
      m_pCI->G.p_constraint = nullptr;
    }

  }

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
