/*============================================================================*/
/* Filtrate.cpp --                                                            */
/*============================================================================*/

#include "Filtrate.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Filtrate, CN_FILTRATE)
__aNewFunction(Filtrate, CN_FILTRATE)
__vDeleteFunction(Filtrate, CN_FILTRATE)
__aFooterFunction(Filtrate)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Filtrate::Filtrate(Bench* pBench)
: TransformerTool(CN_FILTRATE, pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTRATE,_FOOTER_);

  _checkInitialised_(CN_FILTRATE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_FILTRATE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Filtrate::~Filtrate(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTRATE,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <predCmp> after <scan>/<absJoin>  */
/*----------------------------------------------------------------------------*/
void Filtrate::vBurstLeftwards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTRATE,_FOOTER_);

  vBeginIteration();

  while (Card_Filter *pCard_filter = _pCard_Filter(pEachMain(CK_FILTER))) {
    vBurstLeftwards(pCard_filter);
  }

  vEndIteration();
}

/*----------------------------------------------------------------------------*/
/* convert all <filter> from calc form to calcBool                            */
/*----------------------------------------------------------------------------*/
void Filtrate::vCalc_to_calcBool(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTRATE,_FOOTER_);

  vBeginIteration();

  while (Card *pMainCard = pEachMain(CK_FILTER)) {

    vCalc_to_calcBool(pMainCard);
  }

  vEndIteration();
}

/*----------------------------------------------------------------------------*/
/* delete any <filter> which is effectively empty                             */
/*----------------------------------------------------------------------------*/
void Filtrate::vDeleteEmpty_filter(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  vBeginIteration();

  while (Card_Filter *pCard_filter =
      _pCard_Filter(pEachMain(CK_FILTER))) {

    Card_CalcBool* pCard_calcBool =
        _pCard_CalcBool(pCard_filter->pFindSubCard(CK_CALCBOOL));

    if (0 == pCard_calcBool->nSubCards()) {
      m_pBoard->vRemoveMainCard(pCard_filter);
      delete pCard_filter;
      break;
    }
  }

  vEndIteration();
}

/*----------------------------------------------------------------------------*/
/* remove <predCmp> (referenced in <joinPred>) from all <filter>              */
/*----------------------------------------------------------------------------*/
void Filtrate::vRemove_predCmp(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  Card* pParentCard;

  vBeginIteration();

  while (Card_JoinPred* pCard_joinPred =
           _pCard_JoinPred(pEachMain(CK_JOINPRED))) {

    // <card> that is subCard of <joinPred>
    Card_Card* pCard_card = _pCard_Card(pCard_joinPred->pFindSubCard(CK_CARD));

    // <card> is a reference to <predCmp>
    Card_PredCmp* pCard_predCmp = _pCard_PredCmp(pCard_card->pCard());

    // <filter> that is main of <predCmp>
    Card_Filter* pCard_filter =
        _pCard_Filter(pFindMainOf(pCard_predCmp,pParentCard));

    // remove <predCmp> from <parent>
    pParentCard->vRemoveSubCard(pCard_predCmp);

    if ((CK_PREDAND == pParentCard->eCard() || CK_PREDOR == pParentCard->eCard())) {

      if (0 == pParentCard->nSubCards()) {
        pCard_filter->vRemoveSubCard(pParentCard);
      }
      else {
        Card* pSiblingCard = pParentCard->pGetSubCard(0);
        Card* pGrandCard = pCard_filter->pFindParent((pParentCard));
        pParentCard->vRemoveSubCard(pSiblingCard);
        pGrandCard->vReplaceSubCard(pParentCard,pSiblingCard);
      }

      delete pParentCard;
    }
  }

  vEndIteration();
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <predCmp> after <scan>/<absJoin>  */
/*----------------------------------------------------------------------------*/
void Filtrate::vBurstLeftwards(Card_Filter* pCard_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  Card* pMayCard = pDrillForPredCmp(pCard_filter);

  if (pMayCard) {
    vTrace("vBurstLeftwards discovered %p\n"_Stz,pMayCard);
    pMayCard->vDump();
  }
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <predCmp> after <scan>/<absJoin>  */
/*----------------------------------------------------------------------------*/
Card* Filtrate::pDrillForPredCmp(Card* pParentCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  Card* pMayCard = nullptr;

  i8 nSubCards = pParentCard->nSubCards();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card* pSubCard = pParentCard->pGetSubCard(i_SubCard);

    eCK eSubCard = pSubCard->eCard();
    if (CK_PREDCMP == eSubCard) {
      if ( (pMayCard = pCandidatePredCmp(_pCard_PredCmp(pSubCard))) )
        return pMayCard;
   }
    else
    if (CK_PREDAND == eSubCard) {
      if ( (pMayCard = pDrillForPredCmp(pSubCard)) )
        return pMayCard;
    }
    else
    if (CK_PREDOR == eSubCard) {
      return nullptr;
    }
    else {
      if ( (pMayCard = pDrillForPredCmp(pSubCard)) )
        return pMayCard;
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <predCmp> after <scan>/<absJoin>  */
/*----------------------------------------------------------------------------*/
Card* Filtrate::pCandidatePredCmp(Card_PredCmp* pCard_PredCmp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  vTrace("bCandidatePredCmp candidate %p\n"_Stz,pCard_PredCmp);
  pCard_PredCmp->vDump();

  // ADD TEST IF THIS CAN BE MOVED etc

  return nullptr;
}



/*----------------------------------------------------------------------------*/
/* convert a calculation into a boolean calculaiton                           */
/*----------------------------------------------------------------------------*/
Card* Filtrate::pTo_CalcBool(Card* pCard_Calc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);
  RASSERT(CK_CALC == pCard_Calc->eCard());

  Card_PredAnd* pCard_PredAnd = nullptr;
  Card_PredCmp* pCard_PredCmp = nullptr;
  Card_PredOr*  pCard_PredOr  = nullptr;

  Card_SubSet*  pCard_SubExpr = new Card_SubSet;

  Card_CalcBool* pCard_CalcBool = new Card_CalcBool;

  while (pCard_Calc->nSubCards()) {
    Card_Term *pCard_Term = _pCard_Term(pCard_Calc->pPop());

    eTE eTopTerm = pCard_Term->eTermEval();
    switch (eTopTerm) {

      case TE_AND:
        RASSERT(2 == pCard_CalcBool->nSubCards());
        pCard_PredAnd = new Card_PredAnd;
        pCard_PredAnd->vTransferAll(pCard_CalcBool);
        RASSERT(0 == pCard_CalcBool->nSubCards());
        pCard_CalcBool->vAppend(pCard_PredAnd);
        RASSERT(1 == pCard_CalcBool->nSubCards());
        pCard_PredAnd = nullptr;
        delete pCard_Term;
        break;

      case TE_OR:
        RASSERT(2 == pCard_CalcBool->nSubCards());
        pCard_PredOr = new Card_PredOr;
        pCard_PredOr->vTransferAll(pCard_CalcBool);
        RASSERT(0 == pCard_CalcBool->nSubCards());
        pCard_CalcBool->vAppend(pCard_PredOr);
        RASSERT(1 == pCard_CalcBool->nSubCards());
        pCard_PredOr = nullptr;
        delete pCard_Term;
        break;

      case TE_COMPARE:
        RASSERT(pCard_SubExpr->nSubCards() > 0);
        pCard_PredCmp = new Card_PredCmp;
        pCard_PredCmp->vCmp(pCard_Term->szTerm());
        pCard_PredCmp->vTransferAll(pCard_SubExpr);
        pCard_CalcBool->vAppend(pCard_PredCmp);
        pCard_PredCmp = nullptr;
        delete pCard_Term;
        break;

      case TE_OTHER:
        pCard_SubExpr->vAppend(pCard_Term);
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
    RASSERT(0 == pCard_CalcBool->nSubCards());
    pCard_PredCmp->vTransferAll(pCard_SubExpr);
    RASSERT(1 == pCard_CalcBool->nSubCards());
    RASSERT(0 == pCard_SubExpr->nSubCards());
  }

  delete pCard_SubExpr;

  return pCard_CalcBool;
}

/*----------------------------------------------------------------------------*/
/* stream                                                                     */
/*----------------------------------------------------------------------------*/
void Filtrate::vCalc_to_calcBool(Card* pCard_Filter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTRATE,_FOOTER_);

  RASSERT(CK_FILTER == pCard_Filter->eCard());

  RASSERT(1 == pCard_Filter->nSubCards());

  Card_Expr* pCard_Expr = _pCard_Expr(pCard_Filter->pGet(0));
  RASSERT(2 == pCard_Expr->nSubCards());

  Card_Type* pCard_Type = _pCard_Type(pCard_Expr->pGet(0));
  Card_Calc* pCard_Calc = _pCard_Calc(pCard_Expr->pGet(1));

  Card* pCard_CalcBool = pTo_CalcBool(pCard_Calc);

  pCard_Expr->vRemove(pCard_Calc);
  delete pCard_Calc;
  pCard_Expr->vAppend(pCard_CalcBool);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
