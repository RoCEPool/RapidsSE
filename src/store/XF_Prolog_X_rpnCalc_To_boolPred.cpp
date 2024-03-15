/*============================================================================*/
/* XF_Prolog_X_rpnCalc_To_boolPred.cpp -- RpnCalc to BoolPred                 */
/*============================================================================*/

#include "XF_Prolog_X_rpnCalc_To_boolPred.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_X_rpnCalc_To_boolPred, CN_XF_PROLOG_X_RPNCALC_TO_BOOLPRED)
__aNewFunction(XF_Prolog_X_rpnCalc_To_boolPred, CN_XF_PROLOG_X_RPNCALC_TO_BOOLPRED)
__vDeleteFunction(XF_Prolog_X_rpnCalc_To_boolPred, CN_XF_PROLOG_X_RPNCALC_TO_BOOLPRED)
__aFooterFunction(XF_Prolog_X_rpnCalc_To_boolPred)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_X_rpnCalc_To_boolPred::XF_Prolog_X_rpnCalc_To_boolPred(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_X_RPNCALC_TO_BOOLPRED, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_RPNCALC_TO_BOOLPRED, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_X_RPNCALC_TO_BOOLPRED);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_RPNCALC_TO_BOOLPRED, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_X_rpnCalc_To_boolPred::~XF_Prolog_X_rpnCalc_To_boolPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_RPNCALC_TO_BOOLPRED, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_X_rpnCalc_To_boolPred::vConvert(CardIter* pCI)
{
  _static_

  XF_Prolog_X_rpnCalc_To_boolPred* pXF_rpnCalc_To_boolPred = new XF_Prolog_X_rpnCalc_To_boolPred(pCI);

  pXF_rpnCalc_To_boolPred->vX_rpnCalc_To_boolPred();

  delete pXF_rpnCalc_To_boolPred;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* all $n become $c.n                                                         */
/*----------------------------------------------------------------------------*/
void XF_Prolog_X_rpnCalc_To_boolPred::vX_rpnCalc_To_boolPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_X_RPNCALC_TO_BOOLPRED, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_PROLOG_X_RPNCALC_TO_BOOLPRED);

  m_pCI->vClearRestart();
  while (m_pCI->bEachOp(CK_o_FROM, CK_s_CONSTRAINT)) {
    m_pCI->G.p_rpnCalc = m_pCI->G.p_constraint->pSub_rpnCalc();
    m_pCI->G.p_boolPred = m_pCI->G.p_rpnCalc->pTo_boolPred();
    m_pCI->G.p_from->vReplaceSubCard(m_pCI->G.p_rpnCalc,m_pCI->G.p_boolPred);
    m_pCI->vDeleteMember(m_pCI->G.p_rpnCalc);
  }

  m_pCI->vClearRestart();
  while (m_pCI->bEachOp(CK_o_WHERE, CK_RPNCALC)) {
    m_pCI->G.p_filter = m_pCI->G.p_where->pSub_filter();
    m_pCI->G.p_expr = m_pCI->G.p_filter->pSub_expr();
    m_pCI->G.p_boolPred = m_pCI->G.p_expr->pTo_boolPred();
    m_pCI->G.p_where->vReplaceSubCard(m_pCI->G.p_rpnCalc,m_pCI->G.p_boolPred);
    m_pCI->vDeleteMember(m_pCI->G.p_rpnCalc);
  }

  m_pCI->vClearRestart();
  while (m_pCI->bEachOp(CK_o_HAVING, CK_RPNCALC)) {
    m_pCI->G.p_filter = m_pCI->G.p_having->pSub_filter();
    m_pCI->G.p_expr = m_pCI->G.p_filter->pSub_expr();
    m_pCI->G.p_boolPred = m_pCI->G.p_expr->pTo_boolPred();
    m_pCI->G.p_having->vReplaceSubCard(m_pCI->G.p_rpnCalc,m_pCI->G.p_boolPred);
    m_pCI->vDeleteMember(m_pCI->G.p_rpnCalc);
  }

//  m_pCI->vClearRestart();
//  while (m_pCI->bEachOp(CK_o_INDEXJOIN, CK_s_CONSTRAINT, CK_RPNCALC)) {
//    m_pCI->G.p_boolPred = m_pCI->G.p_rpnCalc->pTo_boolPred(m_pCI);
//    m_pCI->G.p_indexScan->vReplaceSubCard(m_pCI->G.p_rpnCalc,m_pCI->G.p_boolPred);
//    m_pCI->vDeleteMember(m_pCI->G.p_rpnCalc);
//  }

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
