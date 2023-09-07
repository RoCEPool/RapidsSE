/*============================================================================*/
/* XF_Prolog_rpnCalc_To_boolCalc.cpp -- RpnCalc to BoolCalc                          */
/*============================================================================*/

#include "XF_Prolog_rpnCalc_To_boolCalc.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_rpnCalc_To_boolCalc, CN_XF_RPNCALC_TO_BOOLCALC)
__aNewFunction(XF_Prolog_rpnCalc_To_boolCalc, CN_XF_RPNCALC_TO_BOOLCALC)
__vDeleteFunction(XF_Prolog_rpnCalc_To_boolCalc, CN_XF_RPNCALC_TO_BOOLCALC)
__aFooterFunction(XF_Prolog_rpnCalc_To_boolCalc)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_rpnCalc_To_boolCalc::XF_Prolog_rpnCalc_To_boolCalc(CardIter* pCI)
: XF_PROLOG(CN_XF_RPNCALC_TO_BOOLCALC, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_RPNCALC_TO_BOOLCALC, _FOOTER_);

  _checkInitialised_(CN_XF_RPNCALC_TO_BOOLCALC);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_RPNCALC_TO_BOOLCALC, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_rpnCalc_To_boolCalc::~XF_Prolog_rpnCalc_To_boolCalc(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_RPNCALC_TO_BOOLCALC, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_rpnCalc_To_boolCalc::vProlog(CardIter* pCI)
{
  _static_

  XF_Prolog_rpnCalc_To_boolCalc* pXF_rpnCalc_To_boolCalc = new XF_Prolog_rpnCalc_To_boolCalc(pCI);

  pXF_rpnCalc_To_boolCalc->vXF_rpnCalc_To_boolCalc();

  delete pXF_rpnCalc_To_boolCalc;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* all $n become $c.n                                                         */
/*----------------------------------------------------------------------------*/
void XF_Prolog_rpnCalc_To_boolCalc::vXF_rpnCalc_To_boolCalc(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_RPNCALC_TO_BOOLCALC, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_RPNCALC_TO_BOOLCALC, "all $n become $c.n"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMainWith2Sub(CK_FROM, CK_CONSTRAINT, CK_RPNCALC)) {
    m_pCI->G.p_boolCalc = m_pCI->G.p_rpnCalc->pTo_boolCalc(m_pCI);
    m_pCI->G.p_from->vReplaceSubCard(m_pCI->G.p_rpnCalc,m_pCI->G.p_boolCalc);
    m_pCI->G_vDeleteMember(m_pCI->G.p_rpnCalc);
  }

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_RPNCALC)) {
    m_pCI->G.p_boolCalc = m_pCI->G.p_rpnCalc->pTo_boolCalc(m_pCI);
    m_pCI->G.p_where->vReplaceSubCard(m_pCI->G.p_rpnCalc,m_pCI->G.p_boolCalc);
    m_pCI->G_vDeleteMember(m_pCI->G.p_rpnCalc);
  }

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMainWithSub(CK_HAVING, CK_RPNCALC)) {
    m_pCI->G.p_boolCalc = m_pCI->G.p_rpnCalc->pTo_boolCalc(m_pCI);
    m_pCI->G.p_having->vReplaceSubCard(m_pCI->G.p_rpnCalc,m_pCI->G.p_boolCalc);
    m_pCI->G_vDeleteMember(m_pCI->G.p_rpnCalc);
  }

//  m_pCI->vClearRestart();
//  while (m_pCI->G_bEachMainWith2Sub(CK_INDEXJOIN, CK_CONSTRAINT, CK_RPNCALC)) {
//    m_pCI->G.p_boolCalc = m_pCI->G.p_rpnCalc->pTo_boolCalc(m_pCI);
//    m_pCI->G.p_indexScan->vReplaceSubCard(m_pCI->G.p_rpnCalc,m_pCI->G.p_boolCalc);
//    m_pCI->G_vDeleteMember(m_pCI->G.p_rpnCalc);
//  }

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
