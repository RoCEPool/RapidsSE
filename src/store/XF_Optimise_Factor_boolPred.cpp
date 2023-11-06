/*============================================================================*/
/* XF_Optimise_Factor_boolPred.cpp -- Refactor boolPred                     */
/*============================================================================*/

#include "XF_Optimise_Factor_boolPred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_Factor_boolPred, CN_XF_OPTIMISE_FACTOR_BOOLPRED)
__aNewFunction(XF_Optimise_Factor_boolPred, CN_XF_OPTIMISE_FACTOR_BOOLPRED)
__vDeleteFunction(XF_Optimise_Factor_boolPred, CN_XF_OPTIMISE_FACTOR_BOOLPRED)
__aFooterFunction(XF_Optimise_Factor_boolPred)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_Factor_boolPred::XF_Optimise_Factor_boolPred(CardIter* pCI)
: XF_OPTIMISE(CN_XF_OPTIMISE_FACTOR_BOOLPRED, OT_STATIC_CALC, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_FACTOR_BOOLPRED);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_Factor_boolPred::~XF_Optimise_Factor_boolPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Factor_boolPred::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_Factor_boolPred* pFactor_boolPred = new XF_Optimise_Factor_boolPred(pCI);

  bool b1 = pFactor_boolPred->bOptimise();

  pFactor_boolPred->vOptimised(b1);

  delete pFactor_boolPred;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Factor_boolPred::bOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  bool bOptRun = false;

  m_pCI->vTransform(__func__, XF_OPTIMISE_FACTOR_BOOLPRED, "refactor boolPred"_Stz);

  m_pCI->vClearRestart();

  while (m_pCI->G_bEachMain_NONE()) {
    switch (m_pCI->G.e_MainCard) {

      case CK_CROSSJOIN:
        RASSERT(! m_pCI->G.p_MainCard->pSub_constraint());
        if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_filter->pSub_boolPred()))
            vFactor_boolPred(m_pCI->G.p_boolPred);
        }
        break;

      case CK_FILTER:
        if ((m_pCI->G.p_boolPred = m_pCI->G.p_filter->pSub_boolPred()))
          vFactor_boolPred(m_pCI->G.p_boolPred);
        break;

      case CK_FROM:
        if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_constraint->pSub_boolPred()))
            vFactor_boolPred(m_pCI->G.p_boolPred);
        }
        if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_filter->pSub_boolPred()))
            vFactor_boolPred(m_pCI->G.p_boolPred);
        }
        break;

      case CK_HAVING:
        RASSERT(! m_pCI->G.p_MainCard->pSub_constraint());
        if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_filter->pSub_boolPred()))
            vFactor_boolPred(m_pCI->G.p_boolPred);
        }
        break;

      case CK_INDEXJOIN:
        if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_constraint->pSub_boolPred()))
            vFactor_boolPred(m_pCI->G.p_boolPred);
        }
        if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_filter->pSub_boolPred()))
            vFactor_boolPred(m_pCI->G.p_boolPred);
        }
        break;

      case CK_INDEXSCAN:
        RASSERT(! m_pCI->G.p_MainCard->pSub_constraint());
        if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_filter->pSub_boolPred()))
            vFactor_boolPred(m_pCI->G.p_boolPred);
        }
        break;

      case CK_KEYEXISTS:
        if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_constraint->pSub_boolPred()))
            vFactor_boolPred(m_pCI->G.p_boolPred);
        }
        RASSERT(! m_pCI->G.p_MainCard->pSub_filter());
        break;

      case CK_KEYNOTEXISTS:
        if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_constraint->pSub_boolPred()))
            vFactor_boolPred(m_pCI->G.p_boolPred);
        }
        RASSERT(! m_pCI->G.p_MainCard->pSub_filter());
        break;

      case CK_SCAN:
        RASSERT(! m_pCI->G.p_MainCard->pSub_constraint());
        if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_filter->pSub_boolPred()))
            vFactor_boolPred(m_pCI->G.p_boolPred);
        }
        break;

      case CK_WHERE:
        RASSERT(! m_pCI->G.p_MainCard->pSub_constraint());
        if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_filter->pSub_boolPred()))
            vFactor_boolPred(m_pCI->G.p_boolPred);
        }
        break;

      default:
        RASSERT(! m_pCI->G.p_MainCard->pSub_constraint());
        RASSERT(! m_pCI->G.p_MainCard->pSub_filter());
        break;
    }
  }

  //m_pCI->G.pDeck->vMarksClearRecursive();

  m_pCI->vEndTransform(__LINE__);

  return bOptRun;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Factor_boolPred::vFactor_boolPred(Card_BoolPred* p_boolPred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);
  
  p_boolPred->vMark(MARK_AT,MARK_PREDICATE);

  Card* pChild = p_boolPred->pPeekChild();
  if (!pChild)
    return;

  RASSERT(! m_pCI->nAncestors());
  m_pCI->vAncestorPush(p_boolPred);

  eCK eChild = pChild->eCard();
  switch (eChild) {
    case CK_ANDCONJ: vFactor_andConj(_pCard_AndConj(pChild)); break;
    case CK_NOTCONJ: vFactor_notConj(_pCard_NotConj(pChild)); break;
    case CK_ORCONJ:  vFactor_orConj (_pCard_OrConj(pChild));  break;
    case CK_RLNCONJ: vFactor_rlnConj(_pCard_RlnConj(pChild)); break;

    case CK_TERM: break;

    default:
      NEVER_REACH
  }

  m_pCI->vAncestorPop();
  RASSERT(! m_pCI->nAncestors());

  m_pCI->G.p_boolPred = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Factor_boolPred::vFactor_andConj(Card_AndConj* p_andCon)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  p_andCon->vMark(MARK_AT,MARK_AND);

  Card* pChild_0 = p_andCon->pGetChild(0);
  if (!pChild_0)
    return;

  m_pCI->vAncestorPush(p_andCon);

  eCK eChild_0 = pChild_0->eCard();
  switch (eChild_0) {
    case CK_ANDCONJ: vFactor_andConj(_pCard_AndConj(pChild_0)); break;
    case CK_NOTCONJ: vFactor_notConj(_pCard_NotConj(pChild_0)); break;
    case CK_ORCONJ:  vFactor_orConj (_pCard_OrConj(pChild_0));  break;
    case CK_RLNCONJ: vFactor_rlnConj(_pCard_RlnConj(pChild_0)); break;

    case CK_TERM: break;

    default:
      NEVER_REACH
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card* pChild_1 = p_andCon->pGetChild(1);
  if (!pChild_1) {
    m_pCI->vAncestorPop();
    return;
  }

  eCK eChild_1 = pChild_1->eCard();
  switch (eChild_1) {
    case CK_ANDCONJ: vFactor_andConj(_pCard_AndConj(pChild_1)); break;
    case CK_NOTCONJ: vFactor_notConj(_pCard_NotConj(pChild_1)); break;
    case CK_ORCONJ:  vFactor_orConj (_pCard_OrConj(pChild_1));  break;
    case CK_RLNCONJ: vFactor_rlnConj(_pCard_RlnConj(pChild_1)); break;

    case CK_TERM: break;

    default:
      NEVER_REACH
  }

  if (eChild_0 == CK_RLNCONJ && eChild_1 == CK_RLNCONJ) {
    p_andCon->vConjunction(FC_AND_RLN_DOUBLE);
    p_andCon->vMark2(MARK_ENUM);
    p_andCon->vMark1(StrLib::cToHex(FC_AND_RLN_DOUBLE));
  }
  else
  if (pChild_0->eConjunction() == FC_AND_RLN_DOUBLE && eChild_1 == CK_RLNCONJ) {
    p_andCon->vConjunction(FC_AND_RLN_TRIPLE);
    p_andCon->vMark2(MARK_ENUM);
    p_andCon->vMark1(StrLib::cToHex(FC_AND_RLN_TRIPLE));
  }
  else
  if (eChild_0 == CK_RLNCONJ &&  pChild_1->eConjunction() == FC_AND_RLN_DOUBLE) {
    p_andCon->vConjunction(FC_AND_RLN_TRIPLE);
    p_andCon->vMark2(MARK_ENUM);
    p_andCon->vMark1(StrLib::cToHex(FC_AND_RLN_TRIPLE));
  }

  m_pCI->vAncestorPop();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Factor_boolPred::vFactor_notConj(Card_NotConj* p_notConj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  p_notConj->vMark(MARK_AT,MARK_NOT);

  Card* pChild_0 = p_notConj->pGetChild(0);
  if (! pChild_0)
    return;

  m_pCI->vAncestorPush(p_notConj);

  eCK eChild_0 = pChild_0->eCard();
  switch (eChild_0) {
    case CK_ANDCONJ: vFactor_andConj(_pCard_AndConj(pChild_0)); break;
    case CK_NOTCONJ: vFactor_notConj(_pCard_NotConj(pChild_0)); break;
    case CK_ORCONJ:  vFactor_orConj (_pCard_OrConj(pChild_0));  break;
    case CK_RLNCONJ: vFactor_rlnConj(_pCard_RlnConj(pChild_0)); break;

    case CK_TERM: break;

    default:
      NEVER_REACH
  }

  m_pCI->vAncestorPop();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Factor_boolPred::vFactor_orConj(Card_OrConj* p_orConj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  p_orConj->vMark(MARK_AT,MARK_OR);

  Card* pChild_0 = p_orConj->pGetChild(0);
  if (!pChild_0)
    return;

  m_pCI->vAncestorPush(p_orConj);

  eCK eChild_0 = pChild_0->eCard();
  switch (eChild_0) {
    case CK_ANDCONJ: vFactor_andConj(_pCard_AndConj(pChild_0)); break;
    case CK_NOTCONJ: vFactor_notConj(_pCard_NotConj(pChild_0)); break;
    case CK_ORCONJ:  vFactor_orConj (_pCard_OrConj(pChild_0));  break;
    case CK_RLNCONJ: vFactor_rlnConj(_pCard_RlnConj(pChild_0)); break;

    case CK_TERM: break;

    default:
      NEVER_REACH
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card* pChild_1 = p_orConj->pGetChild(1);
  if (!pChild_1) {
    m_pCI->vAncestorPop();
    return;
  }

  eCK eChild_1 = pChild_1->eCard();
  switch (eChild_1) {
    case CK_ANDCONJ: vFactor_andConj(_pCard_AndConj(pChild_1)); break;
    case CK_NOTCONJ: vFactor_notConj(_pCard_NotConj(pChild_1)); break;
    case CK_ORCONJ:  vFactor_orConj (_pCard_OrConj(pChild_1));  break;
    case CK_RLNCONJ: vFactor_rlnConj(_pCard_RlnConj(pChild_1)); break;

    case CK_TERM: break;

    default:
      NEVER_REACH
  }

  if (pChild_0->eConjunction() == FC_AND_RLN_TRIPLE &&
      pChild_1->eConjunction() == FC_AND_RLN_TRIPLE) {
    p_orConj->vConjunction(FC_OR_2AND_RLN_TRIPLE);
    p_orConj->vMark2(MARK_ENUM);
    p_orConj->vMark1(StrLib::cToHex(FC_OR_2AND_RLN_TRIPLE));

    p_orConj->vRotationConj(RC_FACTOR_OR_2AND_RLN_TRIPLE);

  }

  m_pCI->vAncestorPop();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Factor_boolPred::vFactor_rlnConj(Card_RlnConj* p_rlnConj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  p_rlnConj->vMark(MARK_AT,MARK_COMPARE);

  Card* pChild_0 = p_rlnConj->pGetChild(0);
  if (!pChild_0)
    return;

  m_pCI->vAncestorPush(p_rlnConj);

  eCK eChild = pChild_0->eCard();
  switch (eChild) {
    case CK_ANDCONJ: vFactor_andConj(_pCard_AndConj(pChild_0)); break;
    case CK_NOTCONJ: vFactor_notConj(_pCard_NotConj(pChild_0)); break;
    case CK_ORCONJ:  vFactor_orConj (_pCard_OrConj(pChild_0));  break;
    case CK_RLNCONJ: vFactor_rlnConj(_pCard_RlnConj(pChild_0)); break;

    case CK_TERM: break;

    default:
      NEVER_REACH
  }

  m_pCI->vAncestorPop();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Factor_boolPred::vRotate_Or_2ar3(Card_OrConj* p_orConj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  UNUSED(p_orConj);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Factor_boolPred::vRotate_b(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Factor_boolPred::bTest_Rotate_Or_2ar3(Card_OrConj* p_orConj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  RASSERT(RC_FACTOR_OR_2AND_RLN_TRIPLE == p_orConj->eRotationConj());

  return false;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
