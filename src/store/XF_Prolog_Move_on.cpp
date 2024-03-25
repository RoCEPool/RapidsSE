/*============================================================================*/
/* XF_Prolog_Move_on.cpp -- Move On                                                  */
/*============================================================================*/

#include "XF_Prolog_Move_on.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Move_on, CN_XF_PROLOG_MOVE_ON)
__aNewFunction(XF_Prolog_Move_on, CN_XF_PROLOG_MOVE_ON)
__vDeleteFunction(XF_Prolog_Move_on, CN_XF_PROLOG_MOVE_ON)
__aFooterFunction(XF_Prolog_Move_on)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Move_on::XF_Prolog_Move_on(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_MOVE_ON, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_MOVE_ON, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_MOVE_ON);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_MOVE_ON, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Move_on::~XF_Prolog_Move_on(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_MOVE_ON, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Move_on::vMove(CardIter* pCI)
{
  _static_

  XF_Prolog_Move_on* pXF_Move_on = new XF_Prolog_Move_on(pCI);

  pXF_Move_on->vXF_Move_on();

  delete pXF_Move_on;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* move <on> in <absJoin>'s to <where>                                        */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Move_on::vXF_Move_on(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_MOVE_ON, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_PROLOG_MOVE_ON);

  m_pCI->vBeginEach();
  if (m_pCI->bEachOp(CK_o_WHERE)) {
    m_pCI->E.p_expr = m_pCI->E.p_where->pSub_expr(C_NOT_NULL);
    m_pCI->E.p_rpnCalc = m_pCI->E.p_where->pSub_rpnCalc(C_NOT_NULL);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each <absJoin>/<on>/<rpnCalc>
  //   move/append to <where>/<rpnCalc>

  Card_Scan* p_scan_over = nullptr;

  m_pCI->vBeginEach_reverse();
  while (m_pCI->bEachOp_reverse(CK_c_ABSJOIN, CK_ON)) {

    // ignore
    if (! m_pCI->E.p_on->nChildCards())
      continue;

    // move LEFT_OUTER to scan
    if (m_pCI->E.p_on->bPopDeleteChildIf(CK_TERM,
                                         Symbol(SF_CROSS,"LEFT_OUTER"_Stz))) {
      if (m_pCI->bEachOp_reverse(CK_o_SCAN)) {
        p_scan_over = m_pCI->E.p_scan;
        m_pCI->vNewMember(CK_OUTERJOIN, Symbol(SF_CROSS,"LEFT_OUTER"_Stz));
        m_pCI->E.p_scan->vAppendChild(m_pCI->E.p_outerJoin);
      }
    }

    Card_Term *p_term_0 = _pCard_Term(m_pCI->E.p_on->pGetChild(0));
    if (p_term_0->pSymbol()->bHasFacet("CROSS"_Stz))
      continue;

    if (! m_pCI->E.p_where) {
      // create a new <where>/...
      m_pCI->E.p_where = Card_Where::pNewAssembly();
      m_pCI->E.p_deck->vInsertChildAfter(m_pCI->E.p_where, m_pCI->E.p_absJoin);
    }
    m_pCI->E.p_expr = m_pCI->E.p_where->pSub_expr();
    m_pCI->E.p_rpnCalc = m_pCI->E.p_expr->pSub_rpnCalc(C_NOT_NULL);

    // append the <absJoin>/<on>/<rpnCalc> to the <where>/<rpnCalc>
    m_pCI->E.p_expr->vTransfer(m_pCI->E.p_on, SF_RLN_OP);
    m_pCI->E.p_expr->vApplyCast_x(m_pCI->E.p_on->xCast());

    if (m_pCI->E.p_on->nChildCards()) {
      delete m_pCI->E.p_on->pPopChild_tail();
      m_pCI->E.p_constraint = Card_Constraint::pNewAssembly(m_pCI);
      p_scan_over->vAppendChild(m_pCI->E.p_constraint);
      m_pCI->E.p_expr->vApplyCast_x(m_pCI->E.p_on->xCast());
      m_pCI->E.p_rpnCalc->vTransferChildren(m_pCI->E.p_on);
    }

    // remove and delete <absJoin>/<on>
    m_pCI->E.p_absJoin->vRemoveChild(m_pCI->E.p_on);
    DELETE_CARD(m_pCI->E.p_on);
  }

  m_pCI->vEndTransform(__LINE__);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
