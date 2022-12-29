/*============================================================================*/
/* Xmodify.cpp --                                                       */
/*============================================================================*/

#include "Xmodify.hpp"

#include "Table.hpp"
#include "Scope.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Xmodify)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xmodify::Xmodify(eCN eClass)
: Xsentinals(eClass)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_iOdnLo      = 1;
  m_iOdnHi      = C_UNKNOWN;
  m_cTableSyn   = 'a';
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xmodify::~Xmodify(void) noexcept(false)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to assign <predCmp> after <scan>/<absJoin>*/
/*----------------------------------------------------------------------------*/
void Xmodify::vDeck_CopyMarkedPredToTable(Card_BoolCalc* p_calcBool,
                                          Card_Deck* p_deck)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card *pMainCard;

  CardIter xCR(p_calcBool);
  while (Card_PRED* p_pred = (Card_PRED*)xCR.pRecurseEach()) {
    if (MARK_AT != p_pred->cMark1())
      continue;

    char cTableSyn = p_pred->cMark2();
    Card_Table *p_table = p_deck->pFindTableCard(cTableSyn, pMainCard);
    if (! p_table)
      continue;

    M_vFilter_AddPredUnder_main(pMainCard,
                                (Card_PRED*)p_pred->pDupCardAndSubCards());

    // todo: is this needed?
    Card_Filter* p_filter = pMainCard->pSub_filter();
    p_filter->vMark1(p_table->cMark1());
    p_filter->vMark2(p_table->cMark2());

  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Xmodify::E_iELIST_rpnCalc_Has_stmOrdOdn(Card_ELIST* p_ELIST, Card_OrdElmOdn* p_stmOrdOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Stz* szStrOrdOdn = p_stmOrdOdn->szValue();

  CardIter xCI_elist(p_ELIST);
  while (Card_Expr* p_expr = _pCard_Expr(xCI_elist.pEachChild())) {

    Card_RpnCalc* p_rpnCalc = p_expr->pSub_rpnCalc();
    if (! p_rpnCalc)
      continue;

    if (p_rpnCalc->nChildCards() > 1)
      continue;

    CardIter xCI_rpnCalc(p_rpnCalc);
    while (Card_Term* p_term = _pCard_Term(xCI_rpnCalc.pEachChild())) {
      if (p_term->bIsValue(szStrOrdOdn))
        return xCI_elist.i_Child();
    }
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Xmodify::E_iExpr_rpnCalc_WhichGbyKey(Card_Expr* p_expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_RpnCalc* p_rpnCalc = p_expr->pSub_rpnCalc();
  if (! p_rpnCalc)
    return C_UNKNOWN;

  if (p_rpnCalc->nChildCards() > 1)
    return C_UNKNOWN;

  CardIter xCI(p_rpnCalc);
  while (Card_Term* p_term = _pCard_Term(xCI.pEachChild())) {
    if (Card::bIsKeyOrdinal(p_term->szValue())) {
      i8 i_Key = Card::iGetColOdn(p_term->szValue());
      return i_Key;
    }
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vFilter_AddPred(Card_Filter* p_filter, Card_PRED *p_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_BoolCalc* p_calcBool = p_filter->pSub_boolCalc();

  if (0 == p_calcBool->nChildCards()) {
    p_calcBool->vAppend(p_PRED);
    return;
  }

  Card* p_top = p_calcBool->pPop();

  Card_AndPred* pCard_predAnd = new Card_AndPred;
  pCard_predAnd->vAppend(p_top);
  pCard_predAnd->vAppend(p_PRED);

  p_calcBool->vAppend(pCard_predAnd);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vFilter_AddPredUnder_main(Card* pMainCard, Card_PRED *p_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Filter* p_filter = pMainCard->pSub_filter();
  if (! p_filter) {
    p_filter = _pCard_Filter(pMainCard->pAppendNewCardTail(CK_FILTER));
    p_filter->vAppendNewCardTail(CK_BOOLCALC);
  }

  M_vFilter_AddPred(p_filter,p_PRED);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vFilter_CullPred(Card_Filter* p_filter, char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nChildren = p_filter->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChildCard = p_filter->pGetChild(i_Child);

    eCK eCard = pChildCard->eCard();
    if (eCard != CK_BOOLCALC)
      continue;

    Card_BoolCalc* p_boolCalc = _pCard_BoolCalc(pChildCard);

    p_boolCalc->vBoolCalc_CullPred(cWithMark);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vFilter_DeleteIfEmpty(Card_Filter* p_filter, Card* pMainCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! p_filter->bIsEmpty())
    return;

  pMainCard->vRemoveChild(p_filter);
  DELETE_CARD(p_filter);

}

/*----------------------------------------------------------------------------*/
/* affix each idxCol with it's $c.n of this table stream                      */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vIdxColList_Affix_tblColOdn(Card_IdxColList* p_idxColList, Card_Stream* p_stream)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 i_StreamColNo = C_UNKNOWN;

  Card_StmColList* p_streamColList = p_stream->pSub_stmColList();

  CardIter xCI(p_idxColList);
  while (Card_IdxCol* p_idxCol = _pCard_IdxCol(xCI.pEachChild(CK_IDXCOL))) {
    Stz* szColNm = p_idxCol->sz();

    Card_StmCol* p_streamCol =
        p_streamColList->pStmColList_Find_streamCol(szColNm, i_StreamColNo);

    Card_TblColOdn* p_tblColOdn = p_streamCol->pSub_tblColOdn();
    Stz* szTblColOdn = p_tblColOdn->sz();

    p_idxCol->vSetValueMark1(szTblColOdn, MARK_CHANGED);
  }
}

/*----------------------------------------------------------------------------*/
/* convert eLevel to P/N/O                                                    */
/*----------------------------------------------------------------------------*/
char Xmodify::M_eLevel_To_char(eAL eLevel)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  switch (eLevel) {
    case AL_PARTITION : return 'P';
    case AL_NODE      : return 'N';
    case AL_ORIGIN    : return 'O';
    default           : return '?';
  }
}


/*----------------------------------------------------------------------------*/
/* convert eAL to eYG                                                         */
/*----------------------------------------------------------------------------*/
eYG Xmodify::M_eLevel_To_StageTopology(eAL eLevel)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  switch (eLevel) {
    case AL_PARTITION : return YG_ALL_PARTITIONS;
    case AL_NODE      : return YG_ALL_NODES;
    case AL_ORIGIN    : return YG_ORIGIN_NODE;
    default           : return YG_ALL_PARTITIONS;
  }
}

/*----------------------------------------------------------------------------*/
/* get the one (and only) stmColOdn from rpnCalc                              */
/*----------------------------------------------------------------------------*/
Card_Term* Xmodify::M_pRpnCalc_GetSingle_stmColOdn(Card_RpnCalc* p_rpnCalc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (1 != p_rpnCalc->nChildCards())
    return nullptr;

  Card_Term* p_term = p_rpnCalc->pSub_term();
  Stz* szTerm = p_term->szValue();
  if (! Card::bIsStreamColOrdinal(szTerm))
    return nullptr;

  return p_term;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vRpnCalc_Convert_sqyOp_not_To_notSqyOp(Card_RpnCalc* p_rpnCalc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  CardIter xCI(p_rpnCalc);
  while (Card_Term* p_term = _pCard_Term(xCI.pEachChild())) {

    if (! xCI.bMore())
      break;

    if (p_term->bSame("exists"_Stz)) {

      Card_Term* p_term_next = _pCard_Term(xCI.pEachChild());
      if (p_term_next->bSame("not"_Stz)) {
        xCI.vRewind();
        p_rpnCalc->vRemoveChild(p_term_next);
        p_term->vValue("notExists"_Stz);
        continue;
      }
      continue;
    }

    if (p_term->bSame("in"_Stz)) {

      Card_Term* p_term_next = _pCard_Term(xCI.pEachChild());
      if (p_term_next->bSame("not"_Stz)) {
        xCI.vRewind();
        p_rpnCalc->vRemoveChild(p_term_next);
        p_term->vValue("notIn"_Stz);
        DELETE_CARD(p_term_next);
        continue;
      }
      continue;
    }

  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vScan_Add_cmpPred_To_bounds(Card_Scan* p_scan, Card_CmpPred* p_cmpPred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! ('L' ==p_cmpPred->cMark1() || 'U' == p_cmpPred->cMark1()) )
    return;

  Card_Filter* p_filter = p_scan->pSub_filter();

  Card_Bounds* p_bounds = p_scan->pSub_bounds();
  if (! p_bounds) {
    p_bounds = new Card_Bounds;
    p_scan->vInsertBefore(p_bounds,p_filter);
  }

  if ('L' == p_cmpPred->cMark1()) {
    Card_LBound* p_lBound = p_bounds->pSub_lBound();
    if (! p_lBound) {
      p_lBound = new Card_LBound;
      p_bounds->vAppend(p_lBound);
    }
    Card_CmpPred* p_cmpPred_dup = _pCard_CmpPred(p_cmpPred->pDupCardAndSubCards());
    StrLib::vInsertChar(p_cmpPred_dup->pGetChild(0)->sz()->azChar()+1,'I');
    p_lBound->vAppend(p_cmpPred_dup);
  }
  else
  if ('U' == p_cmpPred->cMark1()) {
    Card_UBound* p_uBound = p_bounds->pSub_uBound();
    if (! p_uBound) {
      p_uBound = new Card_UBound;
      p_bounds->vAppend(p_uBound);
    }
    Card_CmpPred* p_cmpPred_dup = _pCard_CmpPred(p_cmpPred->pDupCardAndSubCards());
    StrLib::vInsertChar(p_cmpPred_dup->pGetChild(0)->sz()->azChar()+1,'I');
    p_uBound->vAppend(p_cmpPred_dup);
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vScan_Produce_indexList(Card_Scan* p_scan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_TableNm* p_tableNm = p_scan->pSub_tableNm();
  Table* pTable = Card::pAccess_Table(p_tableNm->sz());

  Card_IndexList* p_indexList = Card_IndexList::pDerive(pTable);
  if (! p_indexList)
    return;

  Card_Note* p_note = new Card_Note(SEP_SCRATCH);
  p_scan->vAppend(p_note);

  p_scan->vAppend(p_indexList);
}

/*----------------------------------------------------------------------------*/
/* convert eYG to eAL                                                         */
/*----------------------------------------------------------------------------*/
eAL Xmodify::M_eStageTopology_To_Level(eYG eStageTopo)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  eAL eLevel;

  switch (eStageTopo) {
    case YG_ALL_PARTITIONS : eLevel = AL_PARTITION; break;
    case YG_ALL_NODES      : eLevel = AL_NODE;      break;
    case YG_ORIGIN_NODE    : eLevel = AL_ORIGIN;    break;
    default                : eLevel = AL_NONE;      break;
  }

  return eLevel;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vStream_Derive_OrdinalRange_aggreg(Card_Stream* p_stream)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  i8 nStreamCols = p_stream->pSub_stmColList()->nChildCards();
  p_stream->Y_vSet(P_OdnLo, (i8)1);
  p_stream->Y_vSet(P_OdnHi, nStreamCols);

  p_stream->vMark1(MARK_CHANGED);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vStream_Derive_OrdinalRange_project(Card_Stream* p_stream)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  i8 nStreamCols = p_stream->pSub_stmColList()->nChildCards();
  p_stream->Y_vSet(P_OdnLo, (i8)1);
  p_stream->Y_vSet(P_OdnHi, nStreamCols);

  p_stream->vMark1(MARK_CHANGED);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vStream_Derive_OrdinalRange_table(Card_Stream* p_stream, char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // <scan>/<stream>:iOrdLo,iOdnHi
  i8 nStreamCols = p_stream->pSub_stmColList()->nChildCards();
  p_stream->Y_vSet(P_OdnLo, m_iOdnLo);
  m_iOdnHi = m_iOdnLo + nStreamCols - 1;
  p_stream->Y_vSet(P_OdnHi, m_iOdnHi);

  p_stream->vMark1(MARK_CHANGED);

  CardIter xCI(p_stream->pSub_stmColList());
  i8 iTblColOdn = 1;
  i8 iStmColOdn = m_iOdnLo;
  while (Card_StmCol* p_stmCol = _pCard_StmCol(xCI.pEachChild(CK_STMCOL))) {
    Card_StmColOdn* p_stmColOrd = new Card_StmColOdn;
    p_stmColOrd->vSetStreamColOrdinal(iStmColOdn);
    p_stmCol->vAppend(p_stmColOrd);

    Card_TblColOdn* p_tblColOrd = new Card_TblColOdn;
    p_tblColOrd->vSetTableColOrdinal(cTblSyn, iTblColOdn);
    p_stmCol->vAppend(p_tblColOrd);

    iStmColOdn++;
    iTblColOdn++;
  }

  m_iOdnLo = m_iOdnHi + 1;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vStream_Adjust_OrdinalRange(Card_Stream* p_stream, i8 iLo, i8 iHi)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  p_stream->Y_vSet(P_OdnLo, iLo);
  p_stream->Y_vSet(P_OdnHi, iHi);

  p_stream->vMark1(MARK_CHANGED);
}

/*----------------------------------------------------------------------------*/
/* set synonym of table                                                       */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vTable_Set_Syn(Card_Table* p_table)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_TableNm* p_tableNm = p_table->pSub_tableNm();

  Table* pTable = Card::pAccess_Table(p_tableNm->sz());

  p_table->vMark1(MARK_CHANGED);

  char cTableSyn_actual = m_cTableSyn;
  if (pTable->bIsReplicated())
    cTableSyn_actual = StrLib::cToUpper(cTableSyn_actual);

  p_table->vValueSprint("%c"_Stz,cTableSyn_actual);

  m_cTableSyn++;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vTerm_Bump_KeyOrdinal(Card_Term* p_term, i8 nBy)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  i8 iKeyOdn = Card::iGetColOdn(p_term->szValue());

  p_term->vSetKeyOrdinal(iKeyOdn+nBy);

  if (p_term->Y_bHas(P_BumpedBy)) {
    i8 nBumpedBy = p_term->Y_iGet(P_BumpedBy);
    p_term->Y_vSet(P_BumpedBy,nBumpedBy+nBy);
  }
  else {
    p_term->Y_vSet(P_BumpedBy,(i8)nBy);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vTerm_Bump_StreamColOrdinal(Card_Term* p_term, i8 nBy)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  i8 iStmColOdn = Card::iGetColOdn(p_term->szValue());

  p_term->vSetStreamColOrdinal(iStmColOdn+nBy);

  if (p_term->Y_bHas(P_BumpedBy)) {
    i8 nBumpedBy = p_term->Y_iGet(P_BumpedBy);
    p_term->Y_vSet(P_BumpedBy,nBumpedBy+nBy);
  }
  else {
    p_term->Y_vSet(P_BumpedBy,(i8)nBy);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vTerm_MarkIfMatchesAn_idxCol(Card_Term* p_term, Card_IdxColList* p_idxColList)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Stz* szTerm = p_term->sz();
  if (!szTerm->bIsStreamColOrd())
    return;

  i8 i_IdxCol = C_UNKNOWN;

  Card_IdxCol* p_idxCol =
      _pCard_IdxCol(p_idxColList->pFindChild(CK_IDXCOL, szTerm, i_IdxCol));
  if (! p_idxCol)
    return;

  p_term->vMark1('I');
  p_term->vMark2(i_IdxCol);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xmodify::bPRED_Descend(Card_PRED* p_pred,
                            std::function<bool(Card_PRED* )> bfLambdaFn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(p_pred);
  while (Card* pChildCard = xCI.pEachChild()) {
    if (! pChildCard->bIsPRED())
      continue;

    bool bContinue = bPRED_Descend((Card_PRED*)pChildCard, bfLambdaFn);
    if (! bContinue)
      return C_TERMINATE_RECURSION;
  }

  return C_CONTINUE_RECURSION;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::vStreamAnalysisReset(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_iOdnLo      = 1;
  m_iOdnHi      = C_UNKNOWN;
  m_cTableSyn   = 'a';
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
