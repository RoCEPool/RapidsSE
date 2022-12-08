/*============================================================================*/
/* Xmodify.cpp --                                                       */
/*============================================================================*/

#include "Xmodify.hpp"

#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Xmodify)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xmodify::Xmodify(eCN eClass)
: Xcreate(eClass)
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
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (! p_boolCalc->nChildCards())
    return;

  Card_PRED* pCard_pred = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  while (pCard_pred &&
         M_bBoolCalc_CullPred(p_boolCalc, pCard_pred, cWithMark) &&
         p_boolCalc->nChildCards()) {
    pCard_pred = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));
  }
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
bool Xmodify::M_bBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred, char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  eCK eCard_pred = p_pred->eCard();

  if (CK_ANDPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      M_vBoolCalc_RemovePred(p_boolCalc, p_pred);
      delete p_pred;
      return true;
    }
  }
  else
  if (CK_ORPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      M_vBoolCalc_RemovePred(p_boolCalc, p_pred);
      delete p_pred;
      return true;
    }
  }
  else
  if (CK_CMPPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      M_vBoolCalc_RemovePred(p_boolCalc, p_pred);
      delete p_pred;
      return true;
    }
    return false;
  }
  else
  if (CK_ROPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      M_vBoolCalc_RemovePred(p_boolCalc, p_pred);
      delete p_pred;
      return true;
    }
    return false;
  }
  else
  if (CK_BOLPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      M_vBoolCalc_RemovePred(p_boolCalc, p_pred);
      delete p_pred;
      return true;
    }
    return false;
  }

  RASSERT (CK_ANDPRED == eCard_pred || CK_ORPRED == eCard_pred);

  i8 nChildren = p_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_PRED* pCard_pred_child = static_cast<Card_PRED*>(p_pred->pGetChild(i_Child));
    if (M_bBoolCalc_CullPred(p_boolCalc, pCard_pred_child, cWithMark))
      return true;
  }

  return  false;
}

/*----------------------------------------------------------------------------*/
/* inspect <boolCalc>> and mark any <pred> which could be moved               */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vBoolCalc_MarkWrt_table(Card_BoolCalc* p_boolCalc,
                                        Card_Deck* p_deck)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  CardRecurse xCR(p_boolCalc);
  while (Card* p_card = xCR.pRecurseEach()) {

    if (p_card->bIsCard(CK_ANDPRED)) {
      p_card->vMark1('&');
      continue;
    }

    if (p_card->bIsCard(CK_CMPPRED)) {
      p_card->vMark1('?');
      xCR.vDontDescend();
      char cWhichTable = cTable_WhichTableFor_Pred((Card_PRED *) p_card, p_deck);
      if (C_UNKNOWN != cWhichTable) {
        p_card->vMark1(MARK_AT);
        p_card->vMark2(cWhichTable);
      }
      continue;
    }

    if (p_card->bIsCard(CK_ORPRED)) {
      p_card->vMark1('|');
      xCR.vDontDescend();
      continue;
    }

    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vBoolCalc_PushPred(Card_BoolCalc* p_boolCalc,
                                   Card_PRED* p_pred_toBeAdded)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_PRED* p_pred_oldTop = static_cast<Card_PRED*>(p_boolCalc->pPop());
  RASSERT(! p_boolCalc->nChildCards());

  Card_AndPred* p_andPred_new = new Card_AndPred;
  p_andPred_new->vAppend(p_pred_oldTop);
  p_andPred_new->vAppend(p_pred_toBeAdded);

  p_boolCalc->vPush(p_andPred_new);
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vBoolCalc_RemovePred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card* pParentCard = p_boolCalc->pFindParent(p_pred);

  pParentCard->vRemoveSubCard(p_pred);

  if (CK_ANDPRED == pParentCard->eCard() || CK_ORPRED == pParentCard->eCard()) {

    if (0 == pParentCard->nChildCards()) {
      p_boolCalc->vRemoveSubCard(pParentCard);
    }
    else {
      Card* pSiblingCard = pParentCard->pGetChild(0);
      Card* pGrandCard = p_boolCalc->pFindParent((pParentCard));
      pParentCard->vRemoveSubCard(pSiblingCard);
      pGrandCard->vReplaceChild(pParentCard, pSiblingCard);
    }

    delete pParentCard;
  }
}

/*----------------------------------------------------------------------------*/
/* resolve the parameters terms in given boolCalc                             */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vBoolCalc_Substitute_Parameters(Card_BoolCalc* p_boolCalc,
                                                Card_ParamList* p_paramList)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_PRED* p_pred_ROOT = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfXYZ = [this,p_paramList](Card_PRED* p_pred, bool bUpwards)
  {
    if (! p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_pred);
    if (! T_bValue_IsParamOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    Card_Param* p_param = _pCard_Param(p_paramList->pFindPpyRecursive(P_ParamOdn));
    if (! p_param)
      return false;

    //Card_StmColOdn* p_stmColOrd = p_param->pSub_stmColOdn();
    //M_vCard_Set_Value(p_term,p_stmColOrd->szValue(),MARK_CHANGED);
    Card_TblColOdn* p_tblColOrd = p_param->pSub_tblColOdn();
    M_vCard_Set_Value(p_term,p_tblColOrd->szValue(),MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = M_bPredRecursive(p_pred_ROOT,bfXYZ);
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
bool Xmodify::M_bBoolCalc_Yank_PRED(Card_BoolCalc* p_boolCalc, char cWithMark,
                                    Card_PRED*& p_pred_yanked)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  RASSERT(' ' != cWithMark);

  p_pred_yanked = nullptr;

  if (! p_boolCalc->nChildCards()) {
    RASSERT(! p_pred_yanked);
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //
  auto bYank = [this](Card_BoolCalc* p_boolCalc, char cWithMark,
                  Card* p_test, Card_PRED*& p_pred_yanked,
                  auto&& bYank /*oddity permits recursive lambda*/)
  {
    if (p_test->bIsPRED()) {
      if (p_test->cMark1() == cWithMark) {
        p_pred_yanked = static_cast<Card_PRED*>(p_test);
        M_vBoolCalc_RemovePred(p_boolCalc, p_pred_yanked);
        RASSERT(p_pred_yanked);
        return true;
      }
    }

    CardIter xCI(p_boolCalc);
    while (Card* p_child = xCI.pEachChild()) {
      if (bYank(p_boolCalc,cWithMark,p_child,p_pred_yanked,bYank)) {
        RASSERT(p_pred_yanked);
        return true;
      }
    }

    RASSERT(! p_pred_yanked);
    return false;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  CardIter xCI(p_boolCalc);

  while (Card* p_child = xCI.pEachChild()) {
    if (bYank(p_boolCalc,cWithMark,p_child,p_pred_yanked,bYank)) {
      RASSERT(p_pred_yanked);
      return true;
    }
  }

  RASSERT(! p_pred_yanked);
  return false;
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
bool Xmodify::M_bBoolCalc_Yank_PRED(Card_BoolCalc* p_boolCalc, char cWithMark,
                                    Card* p_test, Card_PRED*& p_pred_yanked)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  RASSERT(' ' != cWithMark);

  if (p_test->bIsPRED()) {
    if (p_test->cMark1() == cWithMark) {
      p_pred_yanked = static_cast<Card_PRED*>(p_test);
      M_vBoolCalc_RemovePred(p_boolCalc, p_pred_yanked);
      RASSERT(p_pred_yanked);
      return true;
    }
  }

  CardIter xCI(p_boolCalc);
  while (Card* p_child = xCI.pEachChild()) {
    if (M_bBoolCalc_Yank_PRED(p_boolCalc,cWithMark,p_child,p_pred_yanked)) {
      RASSERT(p_pred_yanked);
      return true;
    }
  }

  RASSERT(! p_pred_yanked);
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vBounds_AddPred(Card_Table* p_table,
                                Card_Index* p_index,
                                Card_Bounds* p_bounds,
                                Card_PRED *p_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  char cTblSyn = p_table->cValue();

  Card_Bound* p_bound;
  Card_PRED* p_childPRED;
  if (p_PRED->bIsCard(CK_ANDPRED)) {
    p_childPRED = (Card_PRED*)p_PRED->pGetChild(0);
    M_vBounds_AddPred(p_table,p_index,p_bounds,p_childPRED);
    p_childPRED = (Card_PRED*)p_PRED->pGetChild(1);
    M_vBounds_AddPred(p_table,p_index,p_bounds,p_childPRED);
  }
  else
  if (p_PRED->bIsCard(CK_CMPPRED)) {
    p_bound = C_pBound_CreateFrom_PRED(p_PRED,cTblSyn);
    p_bounds->vAppend(p_bound);
  }
  else {
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vBounds_AddPredUnder_index(Card_Table* p_table,
                                           Card_Index* p_index,
                                           Card_PRED *p_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Bounds* p_bounds = p_index->pSub_bounds();
  if (! p_bounds) {
    p_bounds = _pCard_Bounds(p_index->pAppendNewCardTail(CK_BOUNDS));
  }

  M_vBounds_AddPred(p_table,p_index,p_bounds,p_PRED);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vCard_Mark_Changed(Card* pCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  pCard->vMark1(MARK_CHANGED);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vCard_Set_IndexColOrdinal(Card* p_card, i8 iIdxColOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  p_card->vValueSprint("$I%lld"_Stz, iIdxColOdn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vCard_Set_KeyOrdinal(Card* p_card, i8 iKeyOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  p_card->vValueSprint("$K%lld"_Stz, iKeyOdn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vCard_Set_ParamOrdinal(Card* p_card, i8 iParamOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  p_card->vValueSprint("$P%lld"_Stz, iParamOdn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vCard_Set_StreamColOrdinal(Card* p_card, i8 iStmColOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  p_card->vValueSprint("$%lld"_Stz, iStmColOdn);
}

/*----------------------------------------------------------------------------*/
/* replace c in $c.n                                                          */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vCard_Set_TableColSyn(Card* p_card, char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  ((char*)p_card->sz())[1] = cTblSyn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vCard_Set_TableColOrdinal(Card* p_card, char cTblSyn, i8 iTblColOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  p_card->vValueSprint("$%c.%lld"_Stz, cTblSyn, iTblColOdn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vCard_Set_Value(Card* pCard, Stz* szValue)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  pCard->vValue(szValue);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vCard_Set_Value(Card* pCard, Stz* szValue, char cMark1)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  pCard->vValue(szValue);
  pCard->vMark1(cMark1);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Xmodify::E_pCmpPred_Extract_Arg_term(Card_CmpPred* p_cmpPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nChildren = p_cmpPred->nChildCards();
  RASSERT(2 == nChildren);

  Card* p_child_0 = p_cmpPred->pGetChild(0);
  Card* p_child_1 = p_cmpPred->pGetChild(1);

  if (T_bValue_IsParamOrdinal(p_child_0->szValue()))
    return _pCard_Term(p_child_1);

  else
  if (T_bValue_IsParamOrdinal(p_child_1->szValue()))
    return _pCard_Term(p_child_0);

  else
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Xmodify::E_pCmpPred_Extract_Param_term(Card_CmpPred* p_cmpPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nChildren = p_cmpPred->nChildCards();
  RASSERT(2 == nChildren);

  Card* p_child_0 = p_cmpPred->pGetChild(0);
  Card* p_child_1 = p_cmpPred->pGetChild(1);

  if (T_bValue_IsParamOrdinal(p_child_0->szValue()))
    return _pCard_Term(p_child_0);

  else
  if (T_bValue_IsParamOrdinal(p_child_1->szValue()))
    return _pCard_Term(p_child_1);

  else
    NEVER_REACH;
  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* mark the bounds of this <predCmp>                                          */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vCmpPred_MarkBound(Card_CmpPred* p_cmpPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Stz* szCmp = p_cmpPred->sz();

  char cBound = '?';

  if (szCmp->bSame("<"_Stz))  cBound = 'U';
  else if (szCmp->bSame("<="_Stz)) cBound = 'U';
  else if (szCmp->bSame("="_Stz))  cBound = 'E';
  else if (szCmp->bSame("=="_Stz)) cBound = 'E';
  else if (szCmp->bSame("!="_Stz)) cBound = 'N';
  else if (szCmp->bSame(">="_Stz)) cBound = 'L';
  else if (szCmp->bSame(">"_Stz))  cBound = 'L';

  p_cmpPred->vMark1(cBound);
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to assign <predCmp> after <scan>/<absJoin>*/
/*----------------------------------------------------------------------------*/
void Xmodify::vDeck_CopyMarkedPredLeftwards(Card_BoolCalc* p_calcBool,
                                            Card_Deck* p_deck)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card *pMainCard;

  CardRecurse xCR(p_calcBool);
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
i8 Xmodify::E_iELIST_rpnCalc_Has_stmOrdOdn(Card_ELIST* p_ELIST, Card_StmOrdOdn* p_stmOrdOdn)
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
    if (T_bValue_IsKeyOrdinal(p_term->szValue())) {
      i8 i_Key = E_iValue_KeyOdn(p_term->szValue());
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

    M_vBoolCalc_CullPred(p_boolCalc, cWithMark);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vFilter_DeleteIfEmpty(Card_Filter* p_filter, Card* pMainCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! T_bFilter_IsEmpty(p_filter))
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
        T_pStmColList_Find_streamCol(p_streamColList, szColNm, i_StreamColNo);

    Card_TblColOdn* p_tblColOdn = p_streamCol->pSub_tblColOdn();
    Stz* szTblColOdn = p_tblColOdn->sz();

    M_vCard_Set_Value(p_idxCol,szTblColOdn,MARK_CHANGED);
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
  Table* pTable = A_pAccess_Table(p_tableNm->sz());

  Card_IndexList* p_indexList = C_pProduce_indexList_Of_table(pTable);
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
    M_vCard_Set_StreamColOrdinal(p_stmColOrd, iStmColOdn);
    p_stmCol->vAppend(p_stmColOrd);

    Card_TblColOdn* p_tblColOrd = new Card_TblColOdn;
    M_vCard_Set_TableColOrdinal(p_tblColOrd, cTblSyn, iTblColOdn);
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
/* inspect <PRED>                                                             */
/*----------------------------------------------------------------------------*/
char Xmodify::cTable_WhichTableFor_Pred(Card_PRED* p_pred,
                                        Card_Deck* p_deck)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nAbsJoin = 0;
  i8 nTable    = 0;
  char cWhichTable = C_UNKNOWN;

  //vTraceing("iAssignPredLeft candidate %p\n"_Stz,p_pred);
  //p_pred->vDump();

  for (i8 i_Main = 0; /* none */ ; i_Main++) {
    Card *pMainCard = p_deck->pGetChild(i_Main);

    if (CK_WHERE == pMainCard->eCard())
      return C_UNKNOWN;

    eCK eMainCard = pMainCard->eCard();

    if (CK_TABLE == eMainCard) {
      nTable++;
      cWhichTable = cTable_Contains_Pred(_pCard_Table(pMainCard), p_pred);
      if (C_UNKNOWN != cWhichTable)
        return cWhichTable;
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
char Xmodify::cTable_Contains_Pred(Card_Table* p_table,
                                   Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_Stream* p_stream = p_table->pSub_stream();

  // are all $n of <pred> in this <stream>?
  if (bTable_Contains_Pred_b(p_table, p_pred)) {
    char cTableSyn = p_table->cValue();
    return cTableSyn;
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* is given <pred> within this table+stream?                                  */
/*----------------------------------------------------------------------------*/
bool Xmodify::bTable_Contains_Pred_b(Card_Table* p_table,
                                     Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  eCK ePredCard = pCard_PRED->eCard();

  char cTableSyn = p_table->cValue();

  if (CK_TERM == ePredCard) {

    Stz* szTerm = pCard_PRED->sz();

    if (T_bValue_IsParamOrdinal(szTerm))
      return true;

    if (! T_bValue_IsTableColOrdinal(szTerm))
      return false;

    char cTblColSyn = E_cValue_TableColSyn(szTerm);
    if (cTableSyn < cTblColSyn)
      return false;

    return true;
  }

  i8 nSubCards = pCard_PRED->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pCard_PRED->pGetChild(i_Child);

    if (!bTable_Contains_Pred_b(p_table, static_cast<Card_PRED *>(pSubCard)))
      return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
char Xmodify::cTable_Contains_Pred_v2(Card_Table* p_table,
                                      Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  CardIter xCI(p_pred);
  while (Card_Term* p_term = (Card_Term*)xCI.pEachChild()) {
    if (! bTable_Contains_Term(p_table,p_term))
      continue;
    char cTableSyn = p_table->cValue();
    return cTableSyn;
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* is given <term> within this table                                          */
/*----------------------------------------------------------------------------*/
bool Xmodify::bTable_Contains_Term(Card_Table* p_table,
                                   Card_Term* p_term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  char cTableSyn = p_table->cValue();

  Stz* szTerm = p_term->sz();

  if (T_bValue_IsParamOrdinal(szTerm))
    return true;

  if (! T_bValue_IsTableColOrdinal(szTerm))
    return false;

  char cTblColSyn = E_cValue_TableColSyn(szTerm);
  if (cTableSyn < cTblColSyn)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vTable_Produce_MaybeIndexJoin(Card_Table* p_table)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_TableNm* p_tableNm = p_table->pSub_tableNm();

  Table* pTable = A_pAccess_Table(p_tableNm->sz());

  Card_IndexList* p_indexList = C_pProduce_indexList_Of_table(pTable);

  while (Card_Index* p_index = _pCard_Index(p_indexList->pPopChild())) {
    Card_MaybeIndexJoin* p_maybeIndexJoin = new Card_MaybeIndexJoin;
    p_maybeIndexJoin->vAppend(p_index);
    Card_Note* p_note = new Card_Note(SEP_SCRATCH);
    p_table->vAppend(p_note);
    p_table->vAppend(p_maybeIndexJoin);
  }

  DELETE_CARD(p_indexList);
}

/*----------------------------------------------------------------------------*/
/* set synonym of table                                                       */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vTable_Set_Syn(Card_Table* p_table)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_TableNm* p_tableNm = p_table->pSub_tableNm();

  Table* pTable = A_pAccess_Table(p_tableNm->sz());

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

  i8 iKeyOdn = E_iValue_KeyOdn(p_term->szValue());

  M_vCard_Set_KeyOrdinal(p_term,iKeyOdn+nBy);

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

  i8 iStmColOdn = E_iValue_StreamColOdn(p_term->szValue());

  M_vCard_Set_StreamColOrdinal(p_term,iStmColOdn+nBy);

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
