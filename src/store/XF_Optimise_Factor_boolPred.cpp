/*============================================================================*/
/* XF_Optimise_Factor_boolPred.cpp -- Refactor boolPred                       */
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

  m_pCI->vTransform(__FILE__, __func__, XF_OPTIMISE_FACTOR_BOOLPRED);

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();

  while (m_pCI->bEachOp()) {
    switch (m_pCI->E.e_Op) {

      case CK_o_CROSSJOIN:
        RASSERT(! m_pCI->E.p_Op->pSub_constraint());
        if ((m_pCI->E.p_filter = m_pCI->E.p_Op->pSub_filter())) {
          if ((m_pCI->E.p_boolPred = m_pCI->E.p_filter->pSub_boolPred()))
            vFactor_boolPred(m_pCI->E.p_boolPred);
        }
        break;

      case CK_o_FILTER:
        if ((m_pCI->E.p_boolPred = m_pCI->E.p_filter->pSub_boolPred()))
          vFactor_boolPred(m_pCI->E.p_boolPred);
        break;

      case CK_o_FROM:
        if ((m_pCI->E.p_constraint = m_pCI->E.p_Op->pSub_constraint())) {
          if ((m_pCI->E.p_boolPred = m_pCI->E.p_constraint->pSub_boolPred()))
            vFactor_boolPred(m_pCI->E.p_boolPred);
        }
        if ((m_pCI->E.p_filter = m_pCI->E.p_Op->pSub_filter())) {
          if ((m_pCI->E.p_boolPred = m_pCI->E.p_filter->pSub_boolPred()))
            vFactor_boolPred(m_pCI->E.p_boolPred);
        }
        break;

      case CK_o_HAVING:
        RASSERT(! m_pCI->E.p_Op->pSub_constraint());
        if ((m_pCI->E.p_filter = m_pCI->E.p_Op->pSub_filter())) {
          if ((m_pCI->E.p_boolPred = m_pCI->E.p_filter->pSub_boolPred()))
            vFactor_boolPred(m_pCI->E.p_boolPred);
        }
        break;

      case CK_o_INDEXJOIN:
        if ((m_pCI->E.p_constraint = m_pCI->E.p_Op->pSub_constraint())) {
          if ((m_pCI->E.p_boolPred = m_pCI->E.p_constraint->pSub_boolPred()))
            vFactor_boolPred(m_pCI->E.p_boolPred);
        }
        if ((m_pCI->E.p_filter = m_pCI->E.p_Op->pSub_filter())) {
          if ((m_pCI->E.p_boolPred = m_pCI->E.p_filter->pSub_boolPred()))
            vFactor_boolPred(m_pCI->E.p_boolPred);
        }
        break;

      case CK_o_INDEXSCAN:
        RASSERT(! m_pCI->E.p_Op->pSub_constraint());
        if ((m_pCI->E.p_filter = m_pCI->E.p_Op->pSub_filter())) {
          if ((m_pCI->E.p_boolPred = m_pCI->E.p_filter->pSub_boolPred()))
            vFactor_boolPred(m_pCI->E.p_boolPred);
        }
        break;

      case CK_o_KEYEXISTS:
        if ((m_pCI->E.p_constraint = m_pCI->E.p_Op->pSub_constraint())) {
          if ((m_pCI->E.p_boolPred = m_pCI->E.p_constraint->pSub_boolPred()))
            vFactor_boolPred(m_pCI->E.p_boolPred);
        }
        RASSERT(! m_pCI->E.p_Op->pSub_filter());
        break;

      case CK_o_KEYNOTEXISTS:
        if ((m_pCI->E.p_constraint = m_pCI->E.p_Op->pSub_constraint())) {
          if ((m_pCI->E.p_boolPred = m_pCI->E.p_constraint->pSub_boolPred()))
            vFactor_boolPred(m_pCI->E.p_boolPred);
        }
        RASSERT(! m_pCI->E.p_Op->pSub_filter());
        break;

      case CK_o_SCAN:
        RASSERT(! m_pCI->E.p_Op->pSub_constraint());
        if ((m_pCI->E.p_filter = m_pCI->E.p_Op->pSub_filter())) {
          if ((m_pCI->E.p_boolPred = m_pCI->E.p_filter->pSub_boolPred()))
            vFactor_boolPred(m_pCI->E.p_boolPred);
        }
        break;

      case CK_o_WHERE:
        RASSERT(! m_pCI->E.p_Op->pSub_constraint());
        if ((m_pCI->E.p_filter = m_pCI->E.p_Op->pSub_filter())) {
          if ((m_pCI->E.p_boolPred = m_pCI->E.p_filter->pSub_boolPred()))
            vFactor_boolPred(m_pCI->E.p_boolPred);
        }
        break;

      default:
        RASSERT(! m_pCI->E.p_Op->pSub_constraint());
        RASSERT(! m_pCI->E.p_Op->pSub_filter());
        break;
    }
  }

  //m_pCI->E.p_deck->vMarksClearRecursive();

  m_pCI->vEndTransform(__LINE__);

  return bOptRun;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void XF_Optimise_Factor_boolPred::vFactor_boolPred(Card_BoolPred* p_boolPred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FACTOR_BOOLPRED, _FOOTER_);

  Card_CONJ *p_conj_root = p_boolPred->pRoot_conj();

  if (!p_conj_root->bIsCard(CK_ORCONJ))
    return;

  if (!p_conj_root->bAllChildren(CK_ANDCONJ))
    return;

  Kid xOrConj(p_conj_root);
  RASSERT(xOrConj.bEach());

  Card_AndConj *p_andConj_ref = xOrConj.p_andConj();

  bool bFoundFactor = true;
  for (Kid xAndConj_ref(p_andConj_ref); xAndConj_ref.bEach(); /**/) {
    if (CK_RLNCONJ != xAndConj_ref.eChild()) {
      xAndConj_ref.pChild()->vMark1(MARK_UNVALID);
      continue;
    }
    Card_RlnConj *p_rlnConj_ref = xAndConj_ref.p_rlnConj();
    p_rlnConj_ref->vMark1(MARK_AT);

    while (xOrConj.bEach()) {

      i8 nAltMatchesRef = 0;
      for (Kid xAndConj_alt(xOrConj.p_andConj()); xAndConj_alt.bEach(); /**/) {
        if (CK_RLNCONJ != xAndConj_alt.eChild()) {
          xAndConj_ref.pChild()->vMark1(MARK_UNVALID);
          continue;
        }
        Card_RlnConj *p_rlnConj_alt = xAndConj_alt.p_rlnConj();
        p_rlnConj_alt->vMark1(MARK_VALID);

        if (p_rlnConj_alt->bIsEq(p_rlnConj_ref)) {
          p_rlnConj_alt->vMark1(MARK_AT);
          nAltMatchesRef++;
          break;
        }
      }

      if (!nAltMatchesRef) {
        bFoundFactor = false;
        break;
      }

    }

    if (bFoundFactor)
      break;
  }

  //m_pCI->PRINTBOARD_dbg();

  if (!bFoundFactor)
    return;


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // find @rln in reference andConj

  Card_RlnConj* p_rlnConj_factored = nullptr;
  for (Kid xAndConj_ref(p_andConj_ref); xAndConj_ref.bEach(); /**/) {
    if (CK_RLNCONJ != xAndConj_ref.eChild() ||
        ! xAndConj_ref.pChild()->bMark1(MARK_AT))
      continue;

    p_rlnConj_factored = xAndConj_ref.p_rlnConj();
  }

  RASSERT(p_rlnConj_factored);

  p_andConj_ref->vRemoveChild(p_rlnConj_factored);

  xOrConj.vBegin();
  RASSERT(xOrConj.bEach());

  while (xOrConj.bEach()) {
    for (Kid xAndConj_alt(xOrConj.p_andConj()); xAndConj_alt.bEach(); /**/) {

      if (CK_RLNCONJ != xAndConj_alt.eChild() ||
          !xAndConj_alt.pChild()->bMark1(MARK_AT))
        continue;

      xOrConj.p_andConj()->vDeleteChild(xAndConj_alt.pChild());
    }
  }

  //m_pCI->PRINTBOARD_dbg();



  Card_AndConj* p_andConj_new = new Card_AndConj;
  p_andConj_new->vAppendChild(p_rlnConj_factored);

  Card_OrConj* p_orConj_top = p_boolPred->pPop_orConj();
  p_andConj_new->vAppendChild(p_orConj_top);

  p_boolPred->vAppendChild(p_andConj_new);

  //m_pCI->PRINTBOARD_dbg();

  vDebugPoint();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
