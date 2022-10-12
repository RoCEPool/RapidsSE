/*============================================================================*/
/* Modifications.cpp --                                                          */
/*============================================================================*/

#include "Modifications.hpp"
#include "Resolver.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Modifications::Modifications(eCN eClassNo, Bench* pBench)
: TransformerTool(eClassNo, pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  _postcondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Modifications::~Modifications(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* cull all <mainCard> == eCard from <board>                                  */
/*----------------------------------------------------------------------------*/
void Modifications::vCullAll(eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  while (I_bEachMain(eCard)) {
    _deck_->vReplaceChildWithItsSubs(m_pMainCard);
    DELETE_CARD(m_pMainCard);
  }
}

/*----------------------------------------------------------------------------*/
/* cull all <mainCard> == eCard from <board>                                  */
/*----------------------------------------------------------------------------*/
void Modifications::vDeleteAll(eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  while (I_bEachMain(eCard))
    m_pM->vRemoveDeleteCurr(eCard);

  vRenumberMainCards();
}

/*----------------------------------------------------------------------------*/
/* renumber the main cards                                                    */
/*----------------------------------------------------------------------------*/
void Modifications::vRenumberMainCards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  //m_pBoard->vRenumberMainCards();

  _deck_->vRenumberRecursive();
}

/*----------------------------------------------------------------------------*/
/* convert <calc> to <calcBool>                                               */
/*----------------------------------------------------------------------------*/
void Modifications::R_vReplaceSub(Card* pInCard, Card* pSub, Card* pWith)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  Card* pParent = pInCard->pFindParent(pSub);

  pParent->vReplaceChild(pSub,pWith);
}

/*----------------------------------------------------------------------------*/
/* convert <calc> to <calcBool>                                               */
/*----------------------------------------------------------------------------*/
void Modifications::X_calc_To_calcBool(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_AndPred* pCard_PredAnd = nullptr;
  Card_CmpPred* pCard_PredCmp = nullptr;
  Card_OrPred*  pCard_PredOr  = nullptr;

  Card_SubSet*  pCard_SubExpr = new Card_SubSet;

  N_vNewMember(CK_CALCBOOL);

  while (m_p_calc->nChildCards()) {
    m_p_term = _pCard_Term(m_p_calc->pPop());

    eTE eTopTerm = m_p_term->eTermEval();
    switch (eTopTerm) {

      case TE_AND:
        RASSERT(2 == m_p_calcBool->nChildCards());
        pCard_PredAnd = new Card_AndPred;
        pCard_PredAnd->vTransferChildren(m_p_calcBool);
        RASSERT(0 == m_p_calcBool->nChildCards());
        m_p_calcBool->vAppend(pCard_PredAnd);
        RASSERT(1 == m_p_calcBool->nChildCards());
        pCard_PredAnd = nullptr;
        D_vDeleteMember(m_p_term);
        break;

      case TE_OR:
        RASSERT(2 == m_p_calcBool->nChildCards());
        pCard_PredOr = new Card_OrPred;
        pCard_PredOr->vTransferChildren(m_p_calcBool);
        RASSERT(0 == m_p_calcBool->nChildCards());
        m_p_calcBool->vAppend(pCard_PredOr);
        RASSERT(1 == m_p_calcBool->nChildCards());
        pCard_PredOr = nullptr;
        D_vDeleteMember(m_p_term);
        break;

      case TE_COMPARE:
        RASSERT(pCard_SubExpr->nChildCards() > 0);
        pCard_PredCmp = new Card_CmpPred;
        pCard_PredCmp->vValue(m_p_term->sz());
        pCard_PredCmp->vTransferChildren(pCard_SubExpr);
        m_p_calcBool->vAppend(pCard_PredCmp);
        pCard_PredCmp = nullptr;
        D_vDeleteMember(m_p_term);
        break;

      case TE_OTHER:
        pCard_SubExpr->vAppend(m_p_term);
        break;

        // - - - - - - -

      case TE_BOOLEAN:
        NEVER_REACH;
        break;

      case TE_TYPE:
        NEVER_REACH;
        break;

      case TE_AS:
        NEVER_REACH;
        break;

      default:
        NEVER_REACH;
        break;
    }
  }

  if (pCard_SubExpr->nItems()) {
    RASSERT(0 == m_p_calcBool->nChildCards());
    pCard_PredCmp->vTransferChildren(pCard_SubExpr);
    RASSERT(1 == m_p_calcBool->nChildCards());
    RASSERT(0 == pCard_SubExpr->nChildCards());
  }

  delete pCard_SubExpr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
