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
    vBurstFilterLeftwards(pCard_filter);
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
void Filtrate::vBurstFilterLeftwards(Card_Filter* pCard_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  // get <filter>.<calcBool>
  //Card_CalcBool* pCard_bool =
  //    _pCard_CalcBool(pCard_filter->pFindSubCard(CK_CALCBOOL));

  Card_CalcBool* pCard_bool = _pGetSub_Card_CalcBool(pCard_filter);

  vPredDrillDown(pCard_bool,pCard_filter);
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
void Filtrate::vPredDrillDown(Card* pParentCard, Card_Filter* pCard_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  i8 iWhichScan = C_UNKNOWN;

  Card_PRED* pCandidatePred = nullptr;

  i8 nSubCards = pParentCard->nSubCards();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card *pSubCard = pParentCard->pGetSubCard(i_SubCard);

    eCK eSubCard = pSubCard->eCard();

    if (CK_PREDOR  == eSubCard) {
      pSubCard->vMark1('|');
      return;
    }
    else
    if (CK_PREDAND  == eSubCard) {
      pSubCard->vMark1('&');
      if ((iWhichScan =
          iMovePredLeft(static_cast<Card_PRED*>(pSubCard),pCard_filter))) {
        pSubCard->vMark2('0' + iWhichScan);
        return;
      }
    }
    else
    if (CK_PREDCMP == eSubCard) {
      pSubCard->vMark1('?');
      if ((iWhichScan =
          iMovePredLeft(static_cast<Card_PRED*>(pSubCard),pCard_filter))) {
        pSubCard->vMark2('0' + iWhichScan);
        return;
      }
    }
    else {
      pSubCard->vMark1('.');
    }

    vPredDrillDown(pSubCard,pCard_filter);
  }
}

/*----------------------------------------------------------------------------*/
/* inspect <PRED>                                                             */
/*----------------------------------------------------------------------------*/
i8 Filtrate::iMovePredLeft(Card_PRED* pCard_PRED, Card_Filter* pCard_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  i8 nAbsJoin = 0;
  i8 nScan    = 0;

  vTrace("bMovePredLeft candidate %p\n"_Stz,pCard_PRED);
  pCard_PRED->vDump();

  for (i8 i_MainCard = 0; /* none */ ; i_MainCard++) {
    Card *pMainCard = m_pBoard->pGetMainCard(i_MainCard);

    if (pMainCard == pCard_filter)
      return false;

    eCK eMainCard = pMainCard->eCard();

    if (CK_ABSJOIN == eMainCard) {
      nAbsJoin++;
      if (bMovePredToAbsJoin(_pCard_AbsJoin(pMainCard),pCard_PRED)) {
        return true;
      }
    }

    if (CK_SCAN == eMainCard) {
      nScan++;
      if (1 == nScan && bMovePredToScan(_pCard_Scan(pMainCard),pCard_PRED)) {
        return true;
      }
    }

    // skip over any other card??
    vTrace("bMovePredLeft skipping mainCard %s\n"_Stz,aCK[eMainCard]);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Filtrate::bMovePredToAbsJoin(Card_AbsJoin* pCard_absJoin, Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  i8 iWhichScan = iIsPredMovableToAbsJoin(pCard_absJoin,pCard_PRED);
  if (C_UNKNOWN == iWhichScan)
    return false;

  //vPredMovesToAbsJoin(pCard_PRED,pCard_absJoin);

  //vMarkPredCardsAsMoved(pCard_PRED);

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Filtrate::bMovePredToScan(Card_Scan* pCard_scan, Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  i8 iWhichScan = iIsPredMovableToScan(pCard_scan,pCard_PRED);
  if (C_UNKNOWN == iWhichScan)
    return false;

  //vPredMovesToScan(pCard_PRED, pCard_scan);

  //vMarkPredCardsAsMoved(pCard_PRED);

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Filtrate::vMarkPredCardsAsMoved(Card* pPredCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  i8 nSubCards = pPredCard->nSubCards();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card *pSubCard = pPredCard->pGetSubCard(i_SubCard);

    //eCK eSubCard = pSubCard->eCard();

    // mark the

    vMarkPredCardsAsMoved(pSubCard);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Filtrate::iIsPredMovableToAbsJoin(Card_AbsJoin* pCard_absJoin, Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  Card_Stream *pCard_stream = _pGetSub_Card_Stream(pCard_absJoin);
  pCard_stream->vMark1('#');

  // are all $n of <pred> in this <stream>?
  if (pCard_stream->bContainsPred(pCard_PRED)) {
    pCard_stream->vMark1('^');
    return pCard_stream->cMark2() - '0';
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Filtrate::iIsPredMovableToScan(Card_Scan* pCard_scan, Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  Card_Stream* pCard_stream = _pGetSub_Card_Stream(pCard_scan);
  pCard_stream->vMark1('#');

  // are all $n of <pred> in this <stream>?
  if (pCard_stream->bContainsPred(pCard_PRED)) {
    pCard_stream->vMark1('^');
    return pCard_stream->cMark2() - '0';
  }
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Filtrate::vPredMovesToAbsJoin(Card* pPredCard, Card_AbsJoin* pCard_absJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Filtrate::vPredMovesToScan(Card* pPredCard, Card_Scan* pCard_scan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

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
