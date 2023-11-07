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
            nFactor_boolPred(m_pCI->G.p_boolPred);
        }
        break;

      case CK_FILTER:
        if ((m_pCI->G.p_boolPred = m_pCI->G.p_filter->pSub_boolPred()))
          nFactor_boolPred(m_pCI->G.p_boolPred);
        break;

      case CK_FROM:
        if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_constraint->pSub_boolPred()))
            nFactor_boolPred(m_pCI->G.p_boolPred);
        }
        if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_filter->pSub_boolPred()))
            nFactor_boolPred(m_pCI->G.p_boolPred);
        }
        break;

      case CK_HAVING:
        RASSERT(! m_pCI->G.p_MainCard->pSub_constraint());
        if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_filter->pSub_boolPred()))
            nFactor_boolPred(m_pCI->G.p_boolPred);
        }
        break;

      case CK_INDEXJOIN:
        if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_constraint->pSub_boolPred()))
            nFactor_boolPred(m_pCI->G.p_boolPred);
        }
        if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_filter->pSub_boolPred()))
            nFactor_boolPred(m_pCI->G.p_boolPred);
        }
        break;

      case CK_INDEXSCAN:
        RASSERT(! m_pCI->G.p_MainCard->pSub_constraint());
        if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_filter->pSub_boolPred()))
            nFactor_boolPred(m_pCI->G.p_boolPred);
        }
        break;

      case CK_KEYEXISTS:
        if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_constraint->pSub_boolPred()))
            nFactor_boolPred(m_pCI->G.p_boolPred);
        }
        RASSERT(! m_pCI->G.p_MainCard->pSub_filter());
        break;

      case CK_KEYNOTEXISTS:
        if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_constraint->pSub_boolPred()))
            nFactor_boolPred(m_pCI->G.p_boolPred);
        }
        RASSERT(! m_pCI->G.p_MainCard->pSub_filter());
        break;

      case CK_SCAN:
        RASSERT(! m_pCI->G.p_MainCard->pSub_constraint());
        if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_filter->pSub_boolPred()))
            nFactor_boolPred(m_pCI->G.p_boolPred);
        }
        break;

      case CK_WHERE:
        RASSERT(! m_pCI->G.p_MainCard->pSub_constraint());
        if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {
          if ((m_pCI->G.p_boolPred = m_pCI->G.p_filter->pSub_boolPred()))
            nFactor_boolPred(m_pCI->G.p_boolPred);
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
i8 XF_Optimise_Factor_boolPred::nFactor_boolPred(Card_BoolPred* p_boolPred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  i8 nRotations = 0;

  p_boolPred->vMark(MARK_AT,MARK_PREDICATE);

  Card* pChild = p_boolPred->pPeekChild();
  if (!pChild)
    return nRotations;

  RASSERT(! m_pCI->nAncestors());
  m_pCI->vAncestorPush(p_boolPred);

  eCK eChild = pChild->eCard();
  switch (eChild) {
    case CK_ANDCONJ:
      nRotations += nFactor_andConj(_pCard_AndConj(pChild));
      break;
    case CK_NOTCONJ:
      nRotations += nFactor_notConj(_pCard_NotConj(pChild));
      break;
    case CK_ORCONJ:
      nRotations += nFactor_orConj(_pCard_OrConj(pChild));
      break;
    case CK_RLNCONJ:
      nRotations += nFactor_rlnConj(_pCard_RlnConj(pChild));
      break;

    case CK_TERM: break;

    default:
      NEVER_REACH
  }

  if (! nRotations) {
    if (pChild->eConjForm() == FC_OR_2AND_RLN_MIX) {
      pChild->vConjForm(FC_OR_2AND_RLN_TRIPLE);
      pChild->vMark2(MARK_ENUM);
      pChild->vMark1(StrLib::cToHex(FC_OR_2AND_RLN_MIX));

      pChild->vConjRot(RC_FACTOR_OR_2AND_RLN_MIX);
    }

    if (RC_FACTOR_OR_2AND_RLN_MIX == pChild->eConjRot()) {
      if (bTest_Rotate_Or_2arm(p_boolPred)) {
        vRotate_Or_2arm(p_boolPred);
        nRotations++;

        p_boolPred->vMarksClearRecursive();
        p_boolPred->vConjFormClearRecursive();
        p_boolPred->vConjRotClearRecursive();
      }
    }
  }

  m_pCI->vAncestorPop();
  RASSERT(! m_pCI->nAncestors());

  m_pCI->G.p_boolPred = nullptr;

  return nRotations;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 XF_Optimise_Factor_boolPred::nFactor_andConj(Card_AndConj* p_andConj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  i8 nRotations = 0;

  p_andConj->vMark(MARK_AT, MARK_AND);

  Card* pChild_0 = p_andConj->pGetChild(0);
  if (!pChild_0)
    return nRotations;

  m_pCI->vAncestorPush(p_andConj);

  eCK eChild_0 = pChild_0->eCard();
  switch (eChild_0) {
    case CK_ANDCONJ:
      nFactor_andConj(_pCard_AndConj(pChild_0)); break;
    case CK_NOTCONJ:
      nFactor_notConj(_pCard_NotConj(pChild_0)); break;
    case CK_ORCONJ:
      nFactor_orConj(_pCard_OrConj(pChild_0));  break;
    case CK_RLNCONJ:
      nFactor_rlnConj(_pCard_RlnConj(pChild_0)); break;

    case CK_TERM: break;

    default:
      NEVER_REACH
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card* pChild_1 = p_andConj->pGetChild(1);
  if (!pChild_1) {
    m_pCI->vAncestorPop();
    return nRotations;
  }

  eCK eChild_1 = pChild_1->eCard();
  switch (eChild_1) {
    case CK_ANDCONJ:
      nRotations += nFactor_andConj(_pCard_AndConj(pChild_1));
      break;
    case CK_NOTCONJ:
      nRotations += nFactor_notConj(_pCard_NotConj(pChild_1));
      break;
    case CK_ORCONJ:
      nRotations += nFactor_orConj(_pCard_OrConj(pChild_1));
      break;
    case CK_RLNCONJ:
      nRotations += nFactor_rlnConj(_pCard_RlnConj(pChild_1));
      break;

    case CK_TERM: break;

    default:
      NEVER_REACH
  }

  if (eChild_0 == CK_RLNCONJ && eChild_1 == CK_RLNCONJ) {
    p_andConj->vConjForm(FC_AND_RLN_DOUBLE);
    p_andConj->vMark2(MARK_ENUM);
    p_andConj->vMark1(StrLib::cToHex(FC_AND_RLN_DOUBLE));
  }
  else
  if (pChild_0->eConjForm() == FC_AND_RLN_DOUBLE && eChild_1 == CK_RLNCONJ) {
    p_andConj->vConjForm(FC_AND_RLN_TRIPLE);
    p_andConj->vMark2(MARK_ENUM);
    p_andConj->vMark1(StrLib::cToHex(FC_AND_RLN_TRIPLE));
  }
  else
  if (eChild_0 == CK_RLNCONJ && pChild_1->eConjForm() == FC_AND_RLN_DOUBLE) {
    p_andConj->vConjForm(FC_AND_RLN_TRIPLE);
    p_andConj->vMark2(MARK_ENUM);
    p_andConj->vMark1(StrLib::cToHex(FC_AND_RLN_TRIPLE));
  }
  else
  if (eChild_0 == CK_ANDCONJ && eChild_1 == CK_ORCONJ) {
    p_andConj->vConjForm(FC_AND_RLN_OR);
    p_andConj->vMark2(MARK_ENUM);
    p_andConj->vMark1(StrLib::cToHex(FC_AND_RLN_OR));
  }

  m_pCI->vAncestorPop();

  return nRotations;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 XF_Optimise_Factor_boolPred::nFactor_notConj(Card_NotConj* p_notConj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  i8 nRotations = 0;

  p_notConj->vMark(MARK_AT,MARK_NOT);

  Card* pChild_0 = p_notConj->pGetChild(0);
  if (! pChild_0)
    return nRotations;

  m_pCI->vAncestorPush(p_notConj);

  eCK eChild_0 = pChild_0->eCard();
  switch (eChild_0) {
    case CK_ANDCONJ:
      nRotations += nFactor_andConj(_pCard_AndConj(pChild_0));
      break;
    case CK_NOTCONJ:
      nRotations += nFactor_notConj(_pCard_NotConj(pChild_0));
      break;
    case CK_ORCONJ:
      nRotations += nFactor_orConj(_pCard_OrConj(pChild_0));
      break;
    case CK_RLNCONJ:
      nRotations += nFactor_rlnConj(_pCard_RlnConj(pChild_0));
      break;

    case CK_TERM: break;

    default:
      NEVER_REACH
  }

  m_pCI->vAncestorPop();

  return nRotations;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 XF_Optimise_Factor_boolPred::nFactor_orConj(Card_OrConj* p_orConj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  i8 nRotations = 0;

  p_orConj->vMark(MARK_AT,MARK_OR);

  Card* pChild_0 = p_orConj->pGetChild(0);
  if (!pChild_0)
    return nRotations;

  m_pCI->vAncestorPush(p_orConj);

  eCK eChild_0 = pChild_0->eCard();
  switch (eChild_0) {
    case CK_ANDCONJ:
      nRotations += nFactor_andConj(_pCard_AndConj(pChild_0));
      break;
    case CK_NOTCONJ:
      nRotations += nFactor_notConj(_pCard_NotConj(pChild_0));
      break;
    case CK_ORCONJ:
      nRotations += nFactor_orConj(_pCard_OrConj(pChild_0));
      break;
    case CK_RLNCONJ:
      nRotations += nFactor_rlnConj(_pCard_RlnConj(pChild_0));
      break;

    case CK_TERM: break;

    default:
      NEVER_REACH
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card* pChild_1 = p_orConj->pGetChild(1);
  if (!pChild_1) {
    m_pCI->vAncestorPop();
    return nRotations;
  }

  eCK eChild_1 = pChild_1->eCard();
  switch (eChild_1) {
    case CK_ANDCONJ:
      nRotations += nFactor_andConj(_pCard_AndConj(pChild_1));
      break;
    case CK_NOTCONJ:
      nRotations += nFactor_notConj(_pCard_NotConj(pChild_1));
      break;
    case CK_ORCONJ:
      nRotations += nFactor_orConj(_pCard_OrConj(pChild_1));
      break;
    case CK_RLNCONJ:
      nRotations += nFactor_rlnConj(_pCard_RlnConj(pChild_1));
      break;

    case CK_TERM: break;

    default:
      NEVER_REACH
  }

  if (eChild_0 == CK_ANDCONJ && eChild_1 == CK_ANDCONJ) {
    if (pChild_0->eConjForm() == FC_AND_RLN_OR &&
        pChild_1->eConjForm() == FC_AND_RLN_TRIPLE  )
      p_orConj->vConjForm(FC_OR_2AND_RLN_MIX);
  }

  if (! nRotations) {
    if (pChild_0->eConjForm() == FC_AND_RLN_TRIPLE &&
        pChild_1->eConjForm() == FC_AND_RLN_TRIPLE) {
      p_orConj->vConjForm(FC_OR_2AND_RLN_TRIPLE);
      p_orConj->vMark2(MARK_ENUM);
      p_orConj->vMark1(StrLib::cToHex(FC_OR_2AND_RLN_TRIPLE));

      p_orConj->vConjRot(RC_FACTOR_OR_2AND_RLN_TRIPLE);
    }

    if (RC_FACTOR_OR_2AND_RLN_TRIPLE == pChild_0->eConjRot()) {
      if (bTest_Rotate_Or_2ar3(p_orConj)) {
        vRotate_Or_2ar3(p_orConj);
        nRotations++;

        pChild_0->vMarksClearRecursive();
        pChild_0->vConjFormClearRecursive();
        pChild_0->vConjRotClearRecursive();
      }
    }
  }

  m_pCI->vAncestorPop();

  return nRotations;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 XF_Optimise_Factor_boolPred::nFactor_rlnConj(Card_RlnConj* p_rlnConj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  i8 nRotations = 0;

  p_rlnConj->vMark(MARK_AT,MARK_COMPARE);

  Card* pChild_0 = p_rlnConj->pGetChild(0);
  if (!pChild_0)
    return nRotations;

  m_pCI->vAncestorPush(p_rlnConj);

  eCK eChild = pChild_0->eCard();
  switch (eChild) {
    case CK_ANDCONJ:
      nRotations += nFactor_andConj(_pCard_AndConj(pChild_0));
      break;
    case CK_NOTCONJ:
      nRotations += nFactor_notConj(_pCard_NotConj(pChild_0));
      break;
    case CK_ORCONJ:
      nRotations += nFactor_orConj(_pCard_OrConj(pChild_0));
      break;
    case CK_RLNCONJ:
      nRotations += nFactor_rlnConj(_pCard_RlnConj(pChild_0));
      break;

    case CK_TERM: break;

    default:
      NEVER_REACH
  }

  m_pCI->vAncestorPop();

  return nRotations;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Factor_boolPred::vRotate_Or_2ar3(Card_CONJSUBROOT* p_CONJSUBROOT)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  // disassemble (pop conjunctions apart)

  Card_OrConj*  p_0_orConj        = p_CONJSUBROOT->pPop_orConj();
  Card_AndConj* p_andConj         = p_CONJSUBROOT->pPop_andConj();

  Card_AndConj* p_0_0_andConj     = p_0_orConj->pPop_andConj();
  Card_AndConj* p_0_0_0_andConj   = p_0_0_andConj->pPop_andConj();
  Card_RlnConj* p_0_0_0_0_rlnConj = p_0_0_0_andConj->pPop_rlnConj();
  Card_RlnConj* p_0_0_0_1_rlnConj = p_0_0_0_andConj->pPop_rlnConj();
  Card_RlnConj* p_0_0_1_rlnConj   = p_0_0_andConj->pPop_rlnConj();

  Card_AndConj* p_0_1_andConj     = p_0_orConj->pPop_andConj();
  Card_AndConj* p_0_1_0_andConj   = p_0_1_andConj->pPop_andConj();
  Card_RlnConj* p_0_1_0_0_rlnConj = p_0_1_0_andConj->pPop_rlnConj();
  Card_RlnConj* p_0_1_0_1_rlnConj = p_0_1_0_andConj->pPop_rlnConj();
  Card_RlnConj* p_0_1_1_rlnConj   = p_0_1_andConj->pPop_rlnConj();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // reassemble (append conjunctions together)

  p_CONJSUBROOT->vAppendChild(p_0_0_andConj);
    p_0_0_andConj->vAppendChild(p_0_0_0_0_rlnConj);
    p_0_0_andConj->vAppendChild(p_0_orConj);
      p_0_orConj->vAppendChild(p_0_0_0_andConj);
        p_0_0_0_andConj->vAppendChild(p_0_0_0_1_rlnConj);
        p_0_0_0_andConj->vAppendChild(p_0_0_1_rlnConj);
      p_0_orConj->vAppendChild(p_0_1_0_andConj);
        p_0_1_0_andConj->vAppendChild(p_0_1_0_1_rlnConj);
        p_0_1_0_andConj->vAppendChild(p_0_1_1_rlnConj);
  p_CONJSUBROOT->vAppendChild(p_andConj);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete unneeded conjunctions

  delete p_0_1_andConj;
  delete p_0_1_0_0_rlnConj;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_Factor_boolPred::vRotate_Or_2arm(Card_CONJSUBROOT* p_CONJSUBROOT)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  // disassemble (pop conjunctions apart)

  Card_OrConj* p_0_orConj = p_CONJSUBROOT->pPop_orConj();

  Card_AndConj* p_0_0_andConj   = p_0_orConj->pPop_andConj();
  Card_RlnConj* p_0_0_0_rlnConj = p_0_0_andConj->pPop_rlnConj();
  Card_OrConj*  p_0_0_1_orConj  = p_0_0_andConj->pPop_orConj();

  Card_AndConj* p_0_1_andConj     = p_0_orConj->pPop_andConj();
  Card_AndConj* p_0_1_0_andConj   = p_0_1_andConj->pPop_andConj();
  Card_RlnConj* p_0_1_0_0_rlnConj = p_0_1_0_andConj->pPop_rlnConj();
  Card_RlnConj* p_0_1_0_1_rlnConj = p_0_1_0_andConj->pPop_rlnConj();
  Card_RlnConj* p_0_1_1_rlnConj   = p_0_1_andConj->pPop_rlnConj();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // reassemble (append conjunctions together)

  p_CONJSUBROOT->vAppendChild(p_0_0_andConj);
    p_0_0_andConj->vAppendChild(p_0_0_0_rlnConj);
    p_0_0_andConj->vAppendChild(p_0_orConj);
      p_0_orConj->vAppendChild(p_0_0_1_orConj);
      p_0_orConj->vAppendChild(p_0_1_andConj);
        p_0_1_andConj->vAppendChild(p_0_1_0_1_rlnConj);
        p_0_1_andConj->vAppendChild(p_0_1_1_rlnConj);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete unneeded conjunctions

  delete p_0_1_0_andConj;
  delete p_0_1_0_0_rlnConj;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Factor_boolPred::bTest_Rotate_Or_2ar3(Card_CONJSUBROOT* p_CONJSUBROOT)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  Card_OrConj* p_0_orConj = p_CONJSUBROOT->pGet_orConj(0);

  RASSERT(RC_FACTOR_OR_2AND_RLN_TRIPLE == p_0_orConj->eConjRot());

  Card_AndConj* p_0_0_andConj = _pCard_AndConj(p_0_orConj->pGetChild(0));
  Card_AndConj* p_0_1_andConj = _pCard_AndConj(p_0_orConj->pGetChild(1));

  Card* pCard_0_0_andConj;
  if (CK_ANDCONJ == p_0_0_andConj->eGetChild(0, pCard_0_0_andConj)) {
    Card_AndConj *p_0_0_0_andConj = _pCard_AndConj(pCard_0_0_andConj);

    Card* pCard_0_1_0_andConj;
    if (CK_ANDCONJ == p_0_1_andConj->eGetChild(0, pCard_0_1_0_andConj)) {
      Card_AndConj *p_0_1_0_andConj = _pCard_AndConj(pCard_0_1_0_andConj);

      Card_RlnConj* p_0_0_0_0_rlnConj = p_0_0_0_andConj->pGet_rlnConj(0);
      Card_RlnConj* p_0_1_0_0_rlnConj = p_0_1_0_andConj->pGet_rlnConj(0);

      if (p_0_0_0_0_rlnConj->bIsEq(p_0_1_0_0_rlnConj)) {
//        p_0_0_0_rlnConj->vDump(__func__ ,"p_0_0_0_rlnConj"_Stz);
//        p_1_0_0_rlnConj->vDump(__func__ ,"p_1_0_0_rlnConj"_Stz);
        return true;
      }
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_Factor_boolPred::bTest_Rotate_Or_2arm(Card_CONJSUBROOT* p_CONJSUBROOT)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  Card_OrConj* p_0_orConj = p_CONJSUBROOT->pGet_orConj(0);

  Card_AndConj* p_0_0_andConj   = p_0_orConj->pGet_andConj(0);
  Card_RlnConj* p_0_0_0_rlnConj = p_0_0_andConj->pGet_rlnConj(0);

  Card_AndConj* p_0_1_andConj     = p_0_orConj->pGet_andConj(1);
  Card_AndConj* p_0_1_0_andConj   = p_0_1_andConj->pGet_andConj(0);
  Card_RlnConj* p_0_1_0_0_rlnConj = p_0_1_0_andConj->pGet_rlnConj(0);

  if (p_0_0_0_rlnConj->bIsEq(p_0_1_0_0_rlnConj)) {
//        p_0_0_0_rlnConj->vDump(__func__ ,"p_0_0_0_rlnConj"_Stz);
//        p_0_1_0_0_rlnConj->vDump(__func__ ,"p_0_1_0_0_rlnConj"_Stz);
    return true;
  }

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
