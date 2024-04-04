/*============================================================================*/
/* XF_Prolog_Reduce_not.cpp -- Reduce Not expression ops                     */
/*============================================================================*/

#include "XF_Prolog_Reduce_not.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Reduce_not, CN_XF_PROLOG_REDUCE_NOT)
__aNewFunction(XF_Prolog_Reduce_not, CN_XF_PROLOG_REDUCE_NOT)
__vDeleteFunction(XF_Prolog_Reduce_not, CN_XF_PROLOG_REDUCE_NOT)
__aFooterFunction(XF_Prolog_Reduce_not)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Reduce_not::XF_Prolog_Reduce_not(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_REDUCE_NOT, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_REDUCE_NOT, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_REDUCE_NOT);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_REDUCE_NOT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Reduce_not::~XF_Prolog_Reduce_not(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_REDUCE_NOT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Reduce_not::vReduce(CardIter* pCI)
{
  _static_

  XF_Prolog_Reduce_not* pXF_not_Ops = new XF_Prolog_Reduce_not(pCI);

  pXF_not_Ops->vXF_not_Ops();

  delete pXF_not_Ops;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* not ops                                                                    */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Reduce_not::vXF_not_Ops(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_REDUCE_NOT, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_PROLOG_REDUCE_NOT);

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();
  while (m_pCI->bEachOp_by2Sub(CK_EXPR,CK_RPNCALC)) {

//    Kid xRpnCalc(m_pCI->E.p_expr->pSub_rpnCalc());
//    while (xRpnCalc.bEach(CK_TERM)) {
    for (m_pCI->vBeginTarget(m_pCI->E.p_expr,CK_TERM); m_pCI->bEachTarget(); /**/) {

      Card_Term* p_term = m_pCI->E.xTarget.p_term();

      if (! m_pCI->bMoreTarget()) {
        m_pCI->vEndTarget();
        break;
      }

      if (p_term->pSymbol()->bHasFacet("exists"_Stz)) {

        RASSERT(m_pCI->bEachTarget());
        Card_Term* p_term_next = m_pCI->E.xTarget.p_term();
        if (p_term_next->pSymbol()->bHasFacet("not"_Stz)) {
          m_pCI->E.xTarget.vBackstep();
          m_pCI->E.p_rpnCalc->vRemoveChild(p_term_next);
          p_term->SYM(SF_SQY_OP,"notExists"_Stz);
          continue;
        }
        continue;
      }

      if (m_pCI->E.xTarget.p_term()->pSymbol()->bHasFacet("in"_Stz)) {

        RASSERT(m_pCI->bEachTarget());
        Card_Term* p_term_next = m_pCI->E.xTarget.p_term();
        if (p_term_next->pSymbol()->bHasFacet("not"_Stz)) {
          m_pCI->E.xTarget.vBackstep();
          m_pCI->E.p_rpnCalc->vRemoveChild(p_term_next);
          p_term->SYM(SF_SQY_OP,"notIn"_Stz);
          DELETE_CARD(p_term_next);
          continue;
        }
        continue;
      }

    }

  }

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
