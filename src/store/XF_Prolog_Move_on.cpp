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

  m_pCI->vTransform(__func__, XF_PROLOG_MOVE_ON, "move <on> in <absJoin>'s to <where>"_Stz);

  m_pCI->vBeginEach();
  if (m_pCI->G_bEachMain(CK_m_WHERE))
    m_pCI->G.p_rpnCalc = m_pCI->G.p_where->pSub_rpnCalc();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each <absJoin>/<on>/<rpnCalc>
  //   move/append to <where>/<rpnCalc>

  Card_Scan* p_scan_over = nullptr;

  m_pCI->vBeginEachReverse();
  while (m_pCI->G_bEachMainWithSub_reverse(CK_c_ABSJOIN, CK_ON)) {

    // ignore
    Card_RpnCalc *p_rpnCalc_on = m_pCI->G.p_on->pSub_rpnCalc();
    if (! p_rpnCalc_on->nChildCards())
      continue;

    // move LEFT_OUTER to scan
    if (p_rpnCalc_on->bPopDeleteChildIf(CK_TERM,
                                        Symbol(SF_CROSS,"LEFT_OUTER"_Stz))) {
      m_pCI->G_vPushIter();
      if (m_pCI->G_bEachMain_reverse_b(CK_m_SCAN)) {
        p_scan_over = m_pCI->G.p_scan;
        m_pCI->G_vNewMember(CK_OUTERJOIN, Symbol(SF_CROSS,"LEFT_OUTER"_Stz));
        m_pCI->G.p_scan->vAppendChild(m_pCI->G.p_outerJoin);
      }
      m_pCI->G_vPopIter();
    }

    Card_Term *p_term_0 = p_rpnCalc_on->pGetTerm(0);
    if (p_term_0->pSymbol()->bSame("CROSS"_Stz))
      continue;

    if (! m_pCI->G.p_where) {
      // create a new <where>/...
      m_pCI->G.p_where = Card_Where::pNewWhereAssembly();
      m_pCI->G.pDeck->vInsertChildAfter(m_pCI->G.p_where,m_pCI->G.p_absJoin);

      m_pCI->G.p_rpnCalc = m_pCI->G.p_where->pSub_rpnCalc();
    }

    // append the <absJoin>/<on>/<rpnCalc> to the <where>/<rpnCalc>
    m_pCI->G.p_rpnCalc->vTransfer(p_rpnCalc_on,SF_RLN_OP);

    if (p_rpnCalc_on->nChildCards()) {
      delete p_rpnCalc_on->pPopChild_tail();
      m_pCI->G_vAppendNewChild(p_scan_over, CK_s_CONSTRAINT);
      m_pCI->G_vAppendNewChild(m_pCI->G.p_constraint,CK_RPNCALC);
      m_pCI->G.p_rpnCalc->vTransferChildren(p_rpnCalc_on);
    }

    // remove and delete <absJoin>/<on>
    m_pCI->G.p_absJoin->vRemoveChild(m_pCI->G.p_on);
    DELETE_CARD(m_pCI->G.p_on);
  }

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
