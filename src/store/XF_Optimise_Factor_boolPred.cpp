/*============================================================================*/
/* XF_Optimise_Factor_boolPred.cpp -- Refactor boolPred                       */
/*============================================================================*/

#include "XF_Optimise_Factor_boolPred.hpp"

//#define Z_FACTOR_NARY

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

#ifdef Z_FACTOR_NARY

void XF_Optimise_Factor_boolPred::vFactor_boolPred(Card_BoolPred* p_boolPred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  Card_CONJ *p_conj_root = p_boolPred->pRoot_conj();

  if (!p_conj_root->bIsCard(CK_ORCONJ))
    return;

  if (!p_conj_root->bAllChildren(CK_ANDCONJ))
    return;

  for (Kid xKid_or(p_conj_root); xKid_or.bEach(); /**/) {

    for (Kid xKid_and(p_conj_root); xKid_and.bEach(); /**/) {

    }

  }
}



#else

void XF_Optimise_Factor_boolPred::vFactor_boolPred(Card_BoolPred* p_boolPred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  eRC eRotation;
  do {
    RASSERT(!m_pCI->nAncestors());
    eRotation = eFactor_boolPred(p_boolPred);
    RASSERT(!m_pCI->nAncestors());

    p_boolPred->vMarksClearRecursive();
    p_boolPred->vConjFormClearRecursive();
    p_boolPred->vConjRotClearRecursive();
  } while (eRotation);

  m_pCI->G.p_boolPred = nullptr;
}

#endif
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eRC XF_Optimise_Factor_boolPred::eFactor_boolPred(Card_BoolPred* p_boolPred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  eRC eRotation = RC_NONE;

  m_pCI->vAncestorPush(p_boolPred);
  p_boolPred->vMark(MARK_AT, MARK_PREDICATE);

  Card *pChild = p_boolPred->pPeekChild();
  if (! pChild) {
    m_pCI->vAncestorPop();
    return eRotation;
  }

  eCK eChild = pChild->eCard();
  switch (eChild) {
    case CK_ANDCONJ:
      eRotation = eFactor_andConj(_pCard_AndConj(pChild));
      break;
    case CK_NOTCONJ:
      eRotation = eFactor_notConj(_pCard_NotConj(pChild));
      break;
    case CK_ORCONJ:
      eRotation = eFactor_orConj(_pCard_OrConj(pChild));
      break;
    case CK_RLNCONJ:
      eRotation = eFactor_rlnConj(_pCard_RlnConj(pChild));
      break;

    case CK_TERM:
      break;

    default:
      NEVER_REACH
  }

  if (RC_NONE != eRotation) {
    m_pCI->vAncestorPop();
    return eRotation;
  }

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  // possible rotation

  if (pChild->eConjForm() == FC_OR_2AND_RLN_MIX) {
    pChild->vConjForm(FC_OR_2AND_RLN_TRIPLE);
    pChild->vMark2(MARK_ENUM);
    pChild->vMark1(StrLib::cToHex(FC_OR_2AND_RLN_MIX));

    pChild->vConjRot(RC_FACTOR_OR_2AND_RLN_MIX);
  }

  if (RC_FACTOR_OR_2AND_RLN_MIX == pChild->eConjRot()) {
    if (bTest_Rotate_Or_2arm(p_boolPred)) {
      eRotation = eRotate_Or_2arm(p_boolPred);
    }
  }

  m_pCI->vAncestorPop();
  return eRotation;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

#ifdef Z_FACTOR_NARY

#else

eRC XF_Optimise_Factor_boolPred::eFactor_andConj(Card_AndConj* p_andConj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  eRC eRotation = RC_NONE;

  p_andConj->vMark(MARK_AT, MARK_AND);
  m_pCI->vAncestorPush(p_andConj);

  Card* pChild_0 = p_andConj->pGetChild(0);
  if (! pChild_0) {
    m_pCI->vAncestorPop();
    return eRotation;
  }

  eCK eChild_0 = pChild_0->eCard();
  switch (eChild_0) {
    case CK_ANDCONJ:
      eRotation = eFactor_andConj(_pCard_AndConj(pChild_0));
      break;
    case CK_NOTCONJ:
      eRotation = eFactor_notConj(_pCard_NotConj(pChild_0));
      break;
    case CK_ORCONJ:
      eRotation = eFactor_orConj(_pCard_OrConj(pChild_0));
      break;
    case CK_RLNCONJ:
      eRotation = eFactor_rlnConj(_pCard_RlnConj(pChild_0));
      break;

    case CK_TERM:
      break;

    default:
      NEVER_REACH
  }

  if (RC_NONE != eRotation) {
    m_pCI->vAncestorPop();
    return eRotation;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card* pChild_1 = p_andConj->pGetChild(1);
  if (! pChild_1) {
    m_pCI->vAncestorPop();
    return eRotation;
  }

  eCK eChild_1 = pChild_1->eCard();
  switch (eChild_1) {
    case CK_ANDCONJ:
      eRotation = eFactor_andConj(_pCard_AndConj(pChild_1));
      break;
    case CK_NOTCONJ:
      eRotation = eFactor_notConj(_pCard_NotConj(pChild_1));
      break;
    case CK_ORCONJ:
      eRotation = eFactor_orConj(_pCard_OrConj(pChild_1));
      break;
    case CK_RLNCONJ:
      eRotation = eFactor_rlnConj(_pCard_RlnConj(pChild_1));
      break;

    case CK_TERM:
      break;

    default:
      NEVER_REACH
  }

  if (RC_NONE != eRotation) {
    m_pCI->vAncestorPop();
    return eRotation;
  }

  // : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : :
  // markup conjunction form

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
  if (eChild_0 == CK_RLNCONJ && eChild_1 == CK_ORCONJ) {
    p_andConj->vConjForm(FC_AND_RLN_OR);
    p_andConj->vMark2(MARK_ENUM);
    p_andConj->vMark1(StrLib::cToHex(FC_AND_RLN_OR));
  }

  m_pCI->vAncestorPop();
  return eRotation;
}

#endif

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eRC XF_Optimise_Factor_boolPred::eFactor_notConj(Card_NotConj* p_notConj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  eRC eRotation = RC_NONE;

  p_notConj->vMark(MARK_AT,MARK_NOT);
  m_pCI->vAncestorPush(p_notConj);

  Card* pChild_0 = p_notConj->pGetChild(0);
  if (! pChild_0) {
    m_pCI->vAncestorPop();
    return eRotation;
  }

  eCK eChild_0 = pChild_0->eCard();
  switch (eChild_0) {
    case CK_ANDCONJ:
      eRotation = eFactor_andConj(_pCard_AndConj(pChild_0));
      break;
    case CK_NOTCONJ:
      eRotation = eFactor_notConj(_pCard_NotConj(pChild_0));
      break;
    case CK_ORCONJ:
      eRotation = eFactor_orConj(_pCard_OrConj(pChild_0));
      break;
    case CK_RLNCONJ:
      eRotation = eFactor_rlnConj(_pCard_RlnConj(pChild_0));
      break;

    case CK_TERM:
      break;

    default:
      NEVER_REACH
  }

  m_pCI->vAncestorPop();
  return eRotation;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

#ifdef Z_FACTOR_NARY

#else

eRC XF_Optimise_Factor_boolPred::eFactor_orConj(Card_OrConj* p_orConj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  eRC eRotation = RC_NONE;

  p_orConj->vMark(MARK_AT,MARK_OR);
  m_pCI->vAncestorPush(p_orConj);

  Card* pChild_0 = p_orConj->pGetChild(0);
  if (! pChild_0) {
    m_pCI->vAncestorPop();
    return eRotation;
  }

  eCK eChild_0 = pChild_0->eCard();
  switch (eChild_0) {
    case CK_ANDCONJ:
      eRotation = eFactor_andConj(_pCard_AndConj(pChild_0));
      break;
    case CK_NOTCONJ:
      eRotation = eFactor_notConj(_pCard_NotConj(pChild_0));
      break;
    case CK_ORCONJ:
      eRotation = eFactor_orConj(_pCard_OrConj(pChild_0));
      break;
    case CK_RLNCONJ:
      eRotation = eFactor_rlnConj(_pCard_RlnConj(pChild_0));
      break;

    case CK_TERM:
      break;

    default:
      NEVER_REACH
  }

  if (RC_NONE != eRotation) {
    m_pCI->vAncestorPop();
    return eRotation;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card* pChild_1 = p_orConj->pGetChild(1);
  if (!pChild_1) {
    m_pCI->vAncestorPop();
    return eRotation;
  }

  eCK eChild_1 = pChild_1->eCard();
  switch (eChild_1) {
    case CK_ANDCONJ:
      eRotation = eFactor_andConj(_pCard_AndConj(pChild_1));
      break;
    case CK_NOTCONJ:
      eRotation = eFactor_notConj(_pCard_NotConj(pChild_1));
      break;
    case CK_ORCONJ:
      eRotation = eFactor_orConj(_pCard_OrConj(pChild_1));
      break;
    case CK_RLNCONJ:
      eRotation = eFactor_rlnConj(_pCard_RlnConj(pChild_1));
      break;

    case CK_TERM:
      break;

    default:
      NEVER_REACH
  }

  if (RC_NONE != eRotation) {
    m_pCI->vAncestorPop();
    return eRotation;
  }

  // : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : :
  // markup conjunction form

  if (eChild_0 == CK_ANDCONJ && eChild_1 == CK_ANDCONJ) {
    if (pChild_0->eConjForm() == FC_AND_RLN_OR &&
        pChild_1->eConjForm() == FC_AND_RLN_TRIPLE)
      p_orConj->vConjForm(FC_OR_2AND_RLN_MIX);
  }

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  // possible rotation

  if (pChild_0->eConjForm() == FC_AND_RLN_TRIPLE &&
      pChild_1->eConjForm() == FC_AND_RLN_TRIPLE) {
    p_orConj->vConjForm(FC_OR_2AND_RLN_TRIPLE);
    p_orConj->vMark2(MARK_ENUM);
    p_orConj->vMark1(StrLib::cToHex(FC_OR_2AND_RLN_TRIPLE));

    p_orConj->vConjRot(RC_FACTOR_OR_2AND_RLN_TRIPLE);
  }

  if (RC_FACTOR_OR_2AND_RLN_TRIPLE == pChild_0->eConjRot()) {
    if (bTest_Rotate_Or_2ar3(p_orConj)) {
      eRotation = eRotate_Or_2ar3(p_orConj);

      pChild_0->vMarksClearRecursive();
      pChild_0->vConjFormClearRecursive();
      pChild_0->vConjRotClearRecursive();
    }
  }

  m_pCI->vAncestorPop();
  return eRotation;
}

#endif

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eRC XF_Optimise_Factor_boolPred::eFactor_rlnConj(Card_RlnConj* p_rlnConj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  eRC eRotation = RC_NONE;

  m_pCI->vAncestorPush(p_rlnConj);
  p_rlnConj->vMark(MARK_AT,MARK_COMPARE);

  Card* pChild_0 = p_rlnConj->pGetChild(0);
  if (! pChild_0) {
    m_pCI->vAncestorPop();
    return eRotation;
  }

  eCK eChild = pChild_0->eCard();
  switch (eChild) {
    case CK_ANDCONJ:
      eRotation = eFactor_andConj(_pCard_AndConj(pChild_0));
      break;
    case CK_NOTCONJ:
      eRotation = eFactor_notConj(_pCard_NotConj(pChild_0));
      break;
    case CK_ORCONJ:
      eRotation = eFactor_orConj(_pCard_OrConj(pChild_0));
      break;
    case CK_RLNCONJ:
      eRotation = eFactor_rlnConj(_pCard_RlnConj(pChild_0));
      break;

    case CK_TERM: break;

    default:
      NEVER_REACH
  }

  m_pCI->vAncestorPop();
  return eRotation;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eRC XF_Optimise_Factor_boolPred::eRotate_Or_2ar3(Card_CONJSUBROOT* p_CONJSUBROOT)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  m_pCI->vPrintBoard(__LINE__);

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
  m_pCI->vPrintBoard(__LINE__);

  return RC_FACTOR_OR_2AND_RLN_TRIPLE;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eRC XF_Optimise_Factor_boolPred::eRotate_Or_2arm(Card_CONJSUBROOT* p_CONJSUBROOT)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  m_pCI->vPrintBoard(__LINE__);

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

  m_pCI->vPrintBoard(__LINE__);

  return RC_FACTOR_OR_2AND_RLN_MIX;
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
