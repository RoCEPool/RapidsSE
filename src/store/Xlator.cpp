/*============================================================================*/
/* Xlator.cpp --                                                              */
/*============================================================================*/

#include "Xlator.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xlator::Xlator(eCN eClassNo, Bench* pBench)
: Accessor(eClassNo, pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  _postcondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xlator::~Xlator(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* cull all <mainCard> == eCard from <board>                                  */
/*----------------------------------------------------------------------------*/
void Xlator::vCullAll(eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  while (E_bEachMain(eCard)) {
    M.pDeck->vReplaceChildWithItsSubs(M.p_MainCard);
    DELETE_CARD(M.p_MainCard);
  }
}

/*----------------------------------------------------------------------------*/
/* cull all <mainCard> == eCard from <board>                                  */
/*----------------------------------------------------------------------------*/
void Xlator::vDeleteAll(eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  while (E_bEachMain(eCard))
    M.xBI.vRemoveDeleteCurr(eCard);

  vRenumberMainCards();
}

/*----------------------------------------------------------------------------*/
/* renumber the main cards                                                    */
/*----------------------------------------------------------------------------*/
void Xlator::vRenumberMainCards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  //m_pBoard->vRenumberMainCards();

  M.pDeck->vRenumberRecursive();
}

/*----------------------------------------------------------------------------*/
/* convert <calc> to <calcBool>                                               */
/*----------------------------------------------------------------------------*/
void Xlator::R_vReplaceSub(Card* pSub, Card* pInParent, Card* pWith)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  Card* pParent = pInParent->pFindParent(pSub);

  pParent->vReplaceChild(pSub,pWith);
}

/*----------------------------------------------------------------------------*/
/* convert <calc> to <calcBool>                                               */
/*----------------------------------------------------------------------------*/
void Xlator::X_calc_To_calcBool(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_AndPred*  p_andPred = nullptr;
  Card_CmpPred*  p_cmpPred = nullptr;
  Card_TstPred* p_tstPred = nullptr;
  Card_OrPred*   p_orPred  = nullptr;

  Card_SubSet*  pCard_SubExpr = new Card_SubSet;

  N_vNewMember(CK_BOOLCALC);

  while (M.p_rpnCalc->nChildCards()) {
    M.p_term = _pCard_Term(M.p_rpnCalc->pPop());

    eTE eTopTerm = M.p_term->eTermEval();
    switch (eTopTerm) {

      case TE_AND:
        RASSERT(2 == M.p_boolCalc->nChildCards());
        p_andPred = new Card_AndPred;
        p_andPred->vTransferChildren(M.p_boolCalc);
        RASSERT(0 == M.p_boolCalc->nChildCards());
        M.p_boolCalc->vAppend(p_andPred);
        RASSERT(1 == M.p_boolCalc->nChildCards());
        p_andPred = nullptr;
        D_vDeleteMember(M.p_term);
        break;

      case TE_OR:
        RASSERT(2 == M.p_boolCalc->nChildCards());
        p_orPred = new Card_OrPred;
        p_orPred->vTransferChildren(M.p_boolCalc);
        RASSERT(0 == M.p_boolCalc->nChildCards());
        M.p_boolCalc->vAppend(p_orPred);
        RASSERT(1 == M.p_boolCalc->nChildCards());
        p_orPred = nullptr;
        D_vDeleteMember(M.p_term);
        break;

      case TE_COMPARE:
        RASSERT(pCard_SubExpr->nChildCards() > 0);
        p_cmpPred = new Card_CmpPred;
        p_cmpPred->vValue(M.p_term->sz());
        p_cmpPred->vTransferChildren(pCard_SubExpr);
        M.p_boolCalc->vAppend(p_cmpPred);
        p_cmpPred = nullptr;
        D_vDeleteMember(M.p_term);
        break;

      case TE_EXISTS:
        p_tstPred = new Card_TstPred;
        p_tstPred->vValue(M.p_term->sz());
        p_tstPred->vTransferChildren(pCard_SubExpr);
        M.p_boolCalc->vAppend(p_tstPred);
        p_tstPred = nullptr;
        D_vDeleteMember(M.p_term);
        break;

      case TE_OTHER:
        pCard_SubExpr->vAppend(M.p_term);
        break;

      default:
        NEVER_REACH;
        break;
    }
  }

  //M.p_boolCalc->vDump();

  if (pCard_SubExpr->nItems()) {
    RASSERT(0 == M.p_boolCalc->nChildCards());
    p_cmpPred->vTransferChildren(pCard_SubExpr);
    RASSERT(1 == M.p_boolCalc->nChildCards());
    RASSERT(0 == pCard_SubExpr->nChildCards());
  }

  delete pCard_SubExpr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
