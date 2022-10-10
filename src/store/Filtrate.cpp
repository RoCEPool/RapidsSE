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
void Filtrate::vAssign_predCmp_leftwards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTRATE,_FOOTER_);

  vTransformation(OT_vAssign_predCmp_leftwards);
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while ((m_p_filter = _pCard_Filter(xBI.pEachMain(CK_FILTER))))
    vAssignFilterLeftwards(m_p_filter);

  //vSetPrintSubCards(CK_PREDCMP, true);
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* convert all <filter> from calc form to calcBool                            */
/*----------------------------------------------------------------------------*/
void Filtrate::vConvert_calc_To_calcBool(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTRATE,_FOOTER_);

  vTransformation(OT_vConvert_calc_To_calcBool);
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while ((m_p_filter = _pCard_Filter(xBI.pEachMain(CK_FILTER))))
    m_p_filter->vCalc_to_calcBool();

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and copy <predCmp> to new <scan|absJoin>/<filter>     */
/*----------------------------------------------------------------------------*/
void Filtrate::vCopy_predCmp_leftwards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTRATE,_FOOTER_);

  vTransformation(OT_vCopy_predCmp_leftwards);
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while ((m_p_filter = _pCard_Filter(xBI.pEachMain(CK_FILTER))))
    vCopyFilterLeftwards(m_p_filter);

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* inspect all <absJoin>/<filter> and cull predicates                         */
/*----------------------------------------------------------------------------*/
void Filtrate::vCull_filterPred_Of_indexJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTRATE,_FOOTER_);

  vTransformation(OT_vCull_filterPred_Of_indexJoin);
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while ((m_p_absJoin = _pCard_AbsJoin(xBI.pEachMain(CK_ABSJOIN)))) {

    m_p_filter   = _pSub_Filter(m_p_absJoin);
    m_p_calcBool = _pSub_CalcBool(m_p_filter);

    m_p_calcBool->vCullPred('^');
  }

  //m_pCardCtl->vPrintSubCards(CK_STREAM, true);
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete any <filter> which is effectively empty                             */
/*----------------------------------------------------------------------------*/
void Filtrate::vDelete_Empty_filter(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  vTransformation(OT_vDelete_Empty_filter);
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while ((m_p_filter = _pCard_Filter(xBI.pEachMain(CK_FILTER)))) {
    m_p_calcBool = _pSub_CalcBool(m_p_filter);

    if (0 == m_p_calcBool->nChildCards()) {
      xBI.vRemoveDeleteCurr(CK_FILTER);
      break;
    }
  }

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete any <filter> which is effectively empty                             */
/*----------------------------------------------------------------------------*/
void Filtrate::vDelete_empty_sub_filter(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  vTransformation(OT_vDelete_empty_sub_filter);
  vExplain("Blah"_Stz);

  vBeginIteration();

  while (Card *pCard_main = pEachMain()) {

    Card_Filter* pCard_filter_sub = _pSub_Filter(pCard_main);
    if (! pCard_filter_sub)
      continue;

    if (! pCard_filter_sub->bIsEmptyFilter())
      continue;

    pCard_main->vRemoveChild(pCard_filter_sub);
    delete pCard_filter_sub;
  }

  vEndIteration();

  if (m_bInteractive)
    m_pBoard->vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* individuate <absJoin>/<filter> as main <filter>                            */
/*----------------------------------------------------------------------------*/
void Filtrate::vIndividuate_filter(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  vTransformation(OT_vIndividuate_filter);
  vExplain("Blah"_Stz);

  vBeginIteration();

  while (Card_Scan *pCard_scan =
      _pCard_Scan(pEachMain(CK_SCAN))) {

    Card_Filter* pCard_filter =
        _pSub_Filter(pCard_scan);

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
      _pSub_Filter(pCard_indexJoin);

    if (! pCard_filter)
      continue;

    pCard_indexJoin->vRemoveChild(pCard_filter);
    m_pBoard->vInsertMainCard_after(pCard_filter,pCard_indexJoin);
  }

  vEndIteration();

  if (m_bInteractive)
    m_pBoard->vPrintBoard();
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

  Card_CalcBool* pCard_bool = _pSub_CalcBool(pCard_filter);

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
    Card *pMainCard = m_pBoard->pGetMain(i_Main);

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

  Card_Stream *pCard_stream = _pSub_Stream(pCard_absJoin);
  //pCard_stream->vMark1('#');

  // are all $n of <pred> in this <stream>?
  if (pCard_stream->bContainsPred(pCard_PRED)) {
    //pCard_stream->vMark1('@');
    return pCard_stream->iStream();
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Filtrate::iAssignPredToScan(Card_Scan* pCard_scan, Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  Card_Stream* pCard_stream = _pSub_Stream(pCard_scan);
  //pCard_stream->vMark1('#');

  // are all $n of <pred> in this <stream>?
  if (pCard_stream->bContainsPred(pCard_PRED)) {
    //pCard_stream->vMark1('@');
    return pCard_stream->iStream();
  }
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to assign <predCmp> after <scan>/<absJoin>*/
/*----------------------------------------------------------------------------*/
void Filtrate::vCopyFilterLeftwards(Card_Filter* pCard_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FILTRATE, _FOOTER_);

  Card_CalcBool* pCard_bool = _pSub_CalcBool(pCard_filter);


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

  Card_CalcBool* pCard_calcBool = _pSub_CalcBool(pCard_filter);

  if ('@' == pCard_pred->cMark1()) {
    Card* pMainCard;
    i8 iStream = pCard_pred->iMark2();
    Card_Stream* pCard_stream = m_pBoard->pFindStreamCard(iStream,pMainCard);

    Card_Filter* pCard_filter_existing =
        _pSub_Filter(pMainCard);

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

    //pCard_stream->vMark1('#');


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

  Card_CalcBool* pCard_calcBool = _pSub_CalcBool(pCard_filter_existing);

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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
