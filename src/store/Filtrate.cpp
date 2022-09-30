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
void Filtrate::vAssignLeftwards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTRATE,_FOOTER_);

  vBeginIteration();

  while (Card_Filter *pCard_filter = _pCard_Filter(pEachMain(CK_FILTER))) {
    vAssignFilterLeftwards(pCard_filter);
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
/* inspect all <filter> and copy <predCmp> to new <scan|absJoin>/<filter>     */
/*----------------------------------------------------------------------------*/
void Filtrate::vCopyLeftwards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTRATE,_FOOTER_);

  vBeginIteration();

  while (Card_Filter *pCard_filter = _pCard_Filter(pEachMain(CK_FILTER))) {
    vCopyFilterLeftwards(pCard_filter);
  }

  vEndIteration();
}

/*----------------------------------------------------------------------------*/
/* inspect all <absJoin>/<filter> and cull predicates                         */
/*----------------------------------------------------------------------------*/
void Filtrate::vCull_indexJoin_filterPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTRATE,_FOOTER_);

  vBeginIteration();

  while (Card_AbsJoin *pCard_absJoin = _pCard_AbsJoin(pEachMain(CK_ABSJOIN))) {

    Card_Filter *pCard_filter = _pGetSub_Card_Filter(pCard_absJoin);

    Card_CalcBool *pCard_calcBool = _pGetSub_Card_CalcBool(pCard_filter);

    pCard_calcBool->vCullPred('^');
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

    if (0 == pCard_calcBool->nChildCards()) {
      m_pBoard->vRemoveMainCard(pCard_filter);
      delete pCard_filter;
      break;
    }
  }

  vEndIteration();
}

/*----------------------------------------------------------------------------*/
/* delete any <filter> which is effectively empty                             */
/*----------------------------------------------------------------------------*/
void Filtrate::vDeleteEmpty_sub_filter(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  vBeginIteration();

  while (Card *pCard_main = pEachMain()) {

    Card_Filter* pCard_filter_sub = _pGetSub_Card_Filter(pCard_main);
    if (! pCard_filter_sub)
      continue;

    if (! pCard_filter_sub->bIsEmptyFilter())
      continue;

    pCard_main->vRemoveChild(pCard_filter_sub);
    delete pCard_filter_sub;
  }

  vEndIteration();
}

/*----------------------------------------------------------------------------*/
/* individuate <absJoin>/<filter> as main <filter>                            */
/*----------------------------------------------------------------------------*/
void Filtrate::vIndividuate_filter(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  vBeginIteration();

  while (Card_Scan *pCard_scan =
      _pCard_Scan(pEachMain(CK_SCAN))) {

    Card_Filter* pCard_filter =
        _pGetSub_Card_Filter(pCard_scan);

    if (! pCard_filter)
      continue;

    pCard_scan->vRemoveChild(pCard_filter);
    m_pBoard->vInsertMainCard_after(pCard_filter,pCard_scan);
  }

  vEndIteration();

  vBeginIteration();

  while (Card_IndexJoin *pCard_indexJoin =
      _pCard_IndexJoin(pEachMain(CK_INDEXJOIN))) {

    Card_Filter* pCard_filter =
      _pGetSub_Card_Filter(pCard_indexJoin);

    if (! pCard_filter)
      continue;

    pCard_indexJoin->vRemoveChild(pCard_filter);
    m_pBoard->vInsertMainCard_after(pCard_filter,pCard_indexJoin);
  }

  vEndIteration();
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to assign <predCmp> after <scan>/<absJoin>*/
/*----------------------------------------------------------------------------*/
void Filtrate::vAssignFilterLeftwards(Card_Filter* pCard_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  // get <filter>.<calcBool>
  //Card_CalcBool* pCard_bool =
  //    _pCard_CalcBool(pCard_filter->pFindSubCard(CK_CALCBOOL));

  Card_CalcBool* pCard_bool = _pGetSub_Card_CalcBool(pCard_filter);

  Card_PRED* pCard_pred = static_cast<Card_PRED*>(pCard_bool->pGetChild(0));

  vAssignPredDrillDown(pCard_pred,pCard_filter);
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
void Filtrate::vAssignPredDrillDown(Card_PRED* pCard_pred, Card_Filter* pCard_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  i8 iWhichScan = C_UNKNOWN;

  eCK eCard = pCard_pred->eCard();


  if (CK_PREDOR  == eCard) {
    pCard_pred->vMark1('|');
    return;
  }

  if (CK_PREDCMP == eCard) {
    pCard_pred->vMark1('?');
    iWhichScan =
        iAssignPredLeft(pCard_pred,
                        pCard_filter);
    if (C_UNKNOWN != iWhichScan) {
      pCard_pred->vMark1('@');
      pCard_pred->vMark2(iWhichScan);
    }
    return;
  }

  i8 nSubCards = pCard_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pCard_pred->pGetChild(i_Child);

    vAssignPredDrillDown(static_cast<Card_PRED *>(pSubCard), pCard_filter);
  }


  if (CK_PREDAND  == eCard) {
    pCard_pred->vMark1('&');
    Card *pSubCard_0 = pCard_pred->pGetChild(0);
    i8 iStream_0 = pSubCard_0->cMark2() - '0';

    Card *pSubCard_1 = pCard_pred->pGetChild(1);
    i8 iStream_1 = pSubCard_1->cMark2() - '0';

    if (iStream_0 == iStream_1) {
      pCard_pred->vMark1('@');
      pCard_pred->vMark2(iStream_0);
    }

    return;
  }
}

/*----------------------------------------------------------------------------*/
/* inspect <PRED>                                                             */
/*----------------------------------------------------------------------------*/
i8 Filtrate::iAssignPredLeft(Card_PRED* pCard_PRED, Card_Filter* pCard_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  i8 nAbsJoin = 0;
  i8 nScan    = 0;
  i8 iWhichScan = C_UNKNOWN;

  //vTrace("iAssignPredLeft candidate %p\n"_Stz,pCard_PRED);
  //pCard_PRED->vDump();

  for (i8 i_Main = 0; /* none */ ; i_Main++) {
    Card *pMainCard = m_pBoard->pGetMainCard(i_Main);

    if (pMainCard == pCard_filter)
      return C_UNKNOWN;

    eCK eMainCard = pMainCard->eCard();

    if (CK_ABSJOIN == eMainCard) {
      nAbsJoin++;
      iWhichScan = iAssignPredToAbsJoin(_pCard_AbsJoin(pMainCard), pCard_PRED);
      if (C_UNKNOWN != iWhichScan)
        return iWhichScan;
      continue;
    }

    if (CK_SCAN == eMainCard) {
      nScan++;
      iWhichScan = iAssignPredToScan(_pCard_Scan(pMainCard), pCard_PRED);
      if (C_UNKNOWN != iWhichScan)
        return iWhichScan;
      continue;
    }

    // skip over any other card??
    vTrace("bMovePredLeft skipping mainCard %s\n"_Stz,aCK[eMainCard]);
  }

  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Filtrate::iAssignPredToAbsJoin(Card_AbsJoin* pCard_absJoin, Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  Card_Stream *pCard_stream = _pGetSub_Card_Stream(pCard_absJoin);
  //pCard_stream->vMark1('#');

  // are all $n of <pred> in this <stream>?
  if (pCard_stream->bContainsPred(pCard_PRED)) {
    pCard_stream->vMark1('@');
    return pCard_stream->cMark2() - '0';
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Filtrate::iAssignPredToScan(Card_Scan* pCard_scan, Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  Card_Stream* pCard_stream = _pGetSub_Card_Stream(pCard_scan);
  //pCard_stream->vMark1('#');

  // are all $n of <pred> in this <stream>?
  if (pCard_stream->bContainsPred(pCard_PRED)) {
    pCard_stream->vMark1('@');
    return pCard_stream->cMark2() - '0';
  }
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to assign <predCmp> after <scan>/<absJoin>*/
/*----------------------------------------------------------------------------*/
void Filtrate::vCopyFilterLeftwards(Card_Filter* pCard_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  Card_CalcBool* pCard_bool = _pGetSub_Card_CalcBool(pCard_filter);


  while (pCard_bool->nChildCards()) {
    Card_PRED* pCard_pred = static_cast<Card_PRED*>(pCard_bool->pGetChild(0));
    bool bSubRemoved = bCopyPredDrillDown(pCard_pred,pCard_filter);
    if (! bSubRemoved)
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
bool Filtrate::bCopyPredDrillDown(Card_PRED* pCard_pred, Card_Filter* pCard_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  Card_CalcBool* pCard_calcBool = _pGetSub_Card_CalcBool(pCard_filter);

  if ('@' == pCard_pred->cMark1()) {
    Card* pMainCard;
    i8 iStream = pCard_pred->iMark2();
    Card_Stream* pCard_stream = m_pBoard->pFindStreamCard(iStream,pMainCard);

    Card_Filter* pCard_filter_existing =
        _pGetSub_Card_Filter(pMainCard);

    if (pCard_filter_existing) {
      pCard_filter_existing->vMark1(pCard_stream->cMark1());
      pCard_filter_existing->vMark2(pCard_stream->iMark2());
      vCopyPredToExistingFilter(pCard_filter_existing,pCard_pred);
    }
    else {
      Card_Filter* pCard_filter_new = new Card_Filter;
      pCard_filter_new->vMark1(pCard_stream->cMark1());
      pCard_filter_new->vMark2(pCard_stream->iMark2());
      pMainCard->vAppend(pCard_filter_new);
      vCopyPredToNewFilter(pCard_filter_new,pCard_pred);
    }

    pCard_stream->vMark1('#');


    pCard_calcBool->vRemovePred(pCard_pred);
    //m_pBoard->vPrintBoard();

    return true;
  }

  i8 nSubCards = pCard_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pCard_pred->pGetChild(i_Child);

    bool bRemoved = bCopyPredDrillDown(static_cast<Card_PRED *>(pSubCard), pCard_filter);
    if (bRemoved)
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
void Filtrate::vCopyPredToExistingFilter(Card_Filter* pCard_filter_existing, Card_PRED* pCard_pred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  Card_CalcBool* pCard_calcBool = _pGetSub_Card_CalcBool(pCard_filter_existing);

  Card* pCard_top = pCard_calcBool->pPop();

  Card_PredAnd* pCard_predAnd = new Card_PredAnd;
  pCard_predAnd->vAppend(pCard_top);
  pCard_predAnd->vAppend(pCard_pred);

  pCard_calcBool->vAppend(pCard_predAnd);
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
void Filtrate::vCopyPredToNewFilter(Card_Filter* pCard_filter_new, Card_PRED* pCard_pred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  Card_Expr* pCard_expr = new Card_Expr;
  pCard_filter_new->vAppend(pCard_expr);

  Card_Type* pCard_type = new Card_Type("BOOLEAN"_Stz);
  pCard_expr->vAppend(pCard_type);
  Card_CalcBool* pCard_calcBool = new Card_CalcBool;
  pCard_expr->vAppend(pCard_calcBool);

  pCard_calcBool->vAppend(pCard_pred);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Filtrate::vMarkPredCardsAsMoved(Card* pPredCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  i8 nSubCards = pPredCard->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pPredCard->pGetChild(i_Child);

    //eCK eSubCard = pSubCard->eCard();

    // mark the

    vMarkPredCardsAsMoved(pSubCard);
  }
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

  while (pCard_Calc->nChildCards()) {
    Card_Term *pCard_Term = _pCard_Term(pCard_Calc->pPop());

    eTE eTopTerm = pCard_Term->eTermEval();
    switch (eTopTerm) {

      case TE_AND:
        RASSERT(2 == pCard_CalcBool->nChildCards());
        pCard_PredAnd = new Card_PredAnd;
        pCard_PredAnd->vTransferSubCards(pCard_CalcBool);
        RASSERT(0 == pCard_CalcBool->nChildCards());
        pCard_CalcBool->vAppend(pCard_PredAnd);
        RASSERT(1 == pCard_CalcBool->nChildCards());
        pCard_PredAnd = nullptr;
        delete pCard_Term;
        break;

      case TE_OR:
        RASSERT(2 == pCard_CalcBool->nChildCards());
        pCard_PredOr = new Card_PredOr;
        pCard_PredOr->vTransferSubCards(pCard_CalcBool);
        RASSERT(0 == pCard_CalcBool->nChildCards());
        pCard_CalcBool->vAppend(pCard_PredOr);
        RASSERT(1 == pCard_CalcBool->nChildCards());
        pCard_PredOr = nullptr;
        delete pCard_Term;
        break;

      case TE_COMPARE:
        RASSERT(pCard_SubExpr->nChildCards() > 0);
        pCard_PredCmp = new Card_PredCmp;
        pCard_PredCmp->vCmp(pCard_Term->szTerm());
        pCard_PredCmp->vTransferSubCards(pCard_SubExpr);
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
    RASSERT(0 == pCard_CalcBool->nChildCards());
    pCard_PredCmp->vTransferSubCards(pCard_SubExpr);
    RASSERT(1 == pCard_CalcBool->nChildCards());
    RASSERT(0 == pCard_SubExpr->nChildCards());
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

  RASSERT(1 == pCard_Filter->nChildCards());

  Card_Expr* pCard_Expr = _pCard_Expr(pCard_Filter->pGet(0));
  RASSERT(2 == pCard_Expr->nChildCards());

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
