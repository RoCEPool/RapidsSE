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
: TransformerTool(eClassNo, pBench)
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
/*X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* convert <calc> to <calcBool>                                               */
/*----------------------------------------------------------------------------*/
void Xlator::X_vSubCard_ReplaceSub(Card* pSub, Card* pInParent, Card* pWith)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  Card* pParent = pInParent->pFindParent(pSub);

  pParent->vReplaceChild(pSub,pWith);
}

/*----------------------------------------------------------------------------*/
/* convert <rpnCalc> to <boolCalc>                                            */
/*----------------------------------------------------------------------------*/
Card_BoolCalc* Xlator::X_pConvert_rpnCalc_To_boolCalc(Card_RpnCalc* p_rpnCalc)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_AndPred* p_andPred = nullptr;
  Card_CmpPred* p_cmpPred = nullptr;
  Card_TstPred* p_tstPred = nullptr;
  Card_OrPred*  p_orPred  = nullptr;

  Card_SubSet*  pCard_SubExpr = new Card_SubSet;

  //N_vNewMember(CK_BOOLCALC);
  Card_BoolCalc* p_boolCalc = new Card_BoolCalc;

  while (p_rpnCalc->nChildCards()) {
    Card_Term* p_term = _pCard_Term(p_rpnCalc->pPop());

    eTE eTopTerm = T_eTerm_TermEval(p_term);
    switch (eTopTerm) {

      case TE_AND:
        RASSERT(2 == p_boolCalc->nChildCards());
        p_andPred = new Card_AndPred;
        p_andPred->vTransferChildren(p_boolCalc);
        RASSERT(0 == p_boolCalc->nChildCards());
        p_boolCalc->vAppend(p_andPred);
        RASSERT(1 == p_boolCalc->nChildCards());
        p_andPred = nullptr;
        D_vDeleteMember(p_term);
        break;

      case TE_OR:
        RASSERT(2 == p_boolCalc->nChildCards());
        p_orPred = new Card_OrPred;
        p_orPred->vTransferChildren(p_boolCalc);
        RASSERT(0 == p_boolCalc->nChildCards());
        p_boolCalc->vAppend(p_orPred);
        RASSERT(1 == p_boolCalc->nChildCards());
        p_orPred = nullptr;
        D_vDeleteMember(p_term);
        break;

      case TE_COMPARE:
        RASSERT(pCard_SubExpr->nChildCards() > 0);
        p_cmpPred = new Card_CmpPred;
        p_cmpPred->vValue(p_term->sz());
        p_cmpPred->vTransferChildren(pCard_SubExpr);
        p_boolCalc->vAppend(p_cmpPred);
        p_cmpPred = nullptr;
        D_vDeleteMember(p_term);
        break;

      case TE_EXISTS:
        p_tstPred = new Card_TstPred;
        p_tstPred->vValue(p_term->sz());
        p_tstPred->vTransferChildren(pCard_SubExpr);
        p_boolCalc->vAppend(p_tstPred);
        p_tstPred = nullptr;
        D_vDeleteMember(p_term);
        break;

      case TE_OTHER:
        pCard_SubExpr->vAppend(p_term);
        break;

      default:
        NEVER_REACH;
        break;
    }
  }

  //M.p_boolCalc->vDump();

  if (pCard_SubExpr->nItems()) {
    RASSERT(0 == p_boolCalc->nChildCards());
    p_cmpPred->vTransferChildren(pCard_SubExpr);
    RASSERT(1 == p_boolCalc->nChildCards());
    RASSERT(0 == pCard_SubExpr->nChildCards());
  }

  delete pCard_SubExpr;

  return p_boolCalc;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Table* Xlator::X_pTableList_FindTable(Card_TableList* p_tableList, char cSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 nChildren = p_tableList->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChildCard = p_tableList->pGetChild(i_Child);
    if (CK_TABLE != pChildCard->eCard())
      continue;

    Card_Table* p_table = _pCard_Table(pChildCard);
    char cTableSyn = p_table->cValue();
    if (cTableSyn == cSyn)
      return p_table;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xlator::X_bTableList_PredCmpIsCoPtn(Card_TableList* p_tableList, Card_CmpPred* p_predCmp)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_ColumnRef* p_columnRef_0 =
      _pCard_ColumnRef(p_predCmp->pGetChild(0));
  Stz* szPredColumnRef_0 = p_columnRef_0->sz();

  Card_ColumnRef* p_columnRef_1 =
      _pCard_ColumnRef(p_predCmp->pGetChild(1));
  Stz* szPredColumnRef_1 = p_columnRef_1->sz();

  char cTableSyn_0 = T_cColumnRef_TableSyn(p_columnRef_0);
  char cTableSyn_1 = T_cColumnRef_TableSyn(p_columnRef_1);

  if (StrLib::bIsUpperCase(cTableSyn_0) || StrLib::bIsUpperCase(cTableSyn_1))
    return true;

  Card_Table* p_table_0 = X_pTableList_FindTable(p_tableList, cTableSyn_0);
  Card_TablePtn* p_tablePtn_0 = p_table_0->pSub_tablePtn();
  Stz* szTablePtn_0 = p_tablePtn_0->sz();

  Card_Table* p_table_1 = X_pTableList_FindTable(p_tableList, cTableSyn_1);
  Card_TablePtn* p_tablePtn_1 = p_table_1->pSub_tablePtn();
  Stz* szTablePtn_1 = p_tablePtn_1->sz();

  if (szTablePtn_0->bSame(szPredColumnRef_0) && szTablePtn_1->bSame(szPredColumnRef_1))
    return true;

  return false;
}


/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
bool Xlator::X_bBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred, char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  eCK eCard_pred = p_pred->eCard();

  if (CK_ANDPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      X_vBoolCalc_RemovePred(p_boolCalc, p_pred);
      delete p_pred;
      return true;
    }
  }
  else
  if (CK_ORPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      X_vBoolCalc_RemovePred(p_boolCalc, p_pred);
      delete p_pred;
      return true;
    }
  }
  else
  if (CK_CMPPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      X_vBoolCalc_RemovePred(p_boolCalc, p_pred);
      delete p_pred;
      return true;
    }
    return false;
  }

  RASSERT (CK_ANDPRED == eCard_pred || CK_ORPRED == eCard_pred);

  i8 nChildren = p_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_PRED* pCard_pred_child = static_cast<Card_PRED*>(p_pred->pGetChild(i_Child));
    if (X_bBoolCalc_CullPred(p_boolCalc, pCard_pred_child, cWithMark))
      return true;
  }

  return  false;
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
void Xlator::X_vBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if (! p_boolCalc->nChildCards())
    return;

  Card_PRED* pCard_pred = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  while (pCard_pred &&
      X_bBoolCalc_CullPred(p_boolCalc, pCard_pred, cWithMark) &&
         p_boolCalc->nChildCards()) {
    pCard_pred = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));
  }
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
void Xlator::X_vBoolCalc_RemovePred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_);

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
/* get rank                                                                   */
/*----------------------------------------------------------------------------*/
i8 Xlator::X_iCandIndex_Rank(Card_CandIndex* p_candIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 iRank = p_candIndex->Y_iVal(P_Rank);

  return iRank;
}

/*----------------------------------------------------------------------------*/
/* set rank                                                                   */
/*----------------------------------------------------------------------------*/
void Xlator::X_vCandIndex_Rank(Card_CandIndex* p_candIndex, i8 iRank)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  p_candIndex->Y_vSet(P_Rank, iRank);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xlator::X_bCoPtnJoin_CoPtnWith(Card_CoPtnJoin* p_coPtnJoin, Card_CoPtnPred* p_CoPtnPred)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_CmpPred* p_predCmp_this = _pCard_CmpPred(this);
  Card_CmpPred* p_predCmp_arg  = _pCard_CmpPred(p_CoPtnPred);

  if (T_bCmpPred_CoPtnWith(p_predCmp_this, p_predCmp_arg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_CoPtnJoin* Xlator::X_pCoPtnJoinList_Find_coPtn_for(Card_CoPtnJoinList* p_coPtnJoinList, Card_CoPtnPred* p_coPtnPred)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 nChildren = p_coPtnJoinList->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_CoPtnJoin* p_coPtnJoin = _pCard_CoPtnJoin(p_coPtnJoinList->pGetChild(i_Child));

    if (X_bCoPtnJoin_CoPtnWith(p_coPtnJoin, p_coPtnPred))
      return p_coPtnJoin;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xlator::X_bCoPtnJoinList_HasEqPredCmp(Card_CoPtnJoinList* p_coPtnJoinList, Card_CmpPred* p_predCmp)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 nChildren_coPtn = p_coPtnJoinList->nChildCards();
  for (i8 i_Child_coPtn = 0; i_Child_coPtn < nChildren_coPtn; i_Child_coPtn++) {
    Card_CoPtnJoin* p_coPtnJoin = _pCard_CoPtnJoin(p_coPtnJoinList->pGetChild(i_Child_coPtn));

    i8 nChildren_predCmp = p_coPtnJoin->nChildCards();
    for (i8 i_Child_predCmp = 0; i_Child_predCmp < nChildren_predCmp; i_Child_predCmp++) {
      Card_CmpPred *p_predCmp = _pCard_CmpPred(p_coPtnJoin->pGetChild(i_Child_predCmp));


      if (T_bCmpPred_IsEq(p_predCmp, p_predCmp))
        return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_StmColList* Xlator::X_pELIST_DeriveStreamColList(Card_ELIST* p_ELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_StmColList* pCard_StreamColList = new Card_StmColList;

  eCK eSubCard;
  i8 nSubCards = p_ELIST->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = p_ELIST->pGetChild(i_Child, eSubCard);

    Card *pCard_Type = pSubCard->pFindSubCard(CK_TYPE);
    Card *pCard_As = pSubCard->pFindSubCard(CK_AS);

    Card_StmCol* pCard_StreamCol = new Card_StmCol;
    pCard_StreamColList->vAppend(pCard_StreamCol);

    if (pCard_As)
      pCard_StreamCol->vAppend(pCard_As->pDupCardAndSubCards());
    else {
      s32 sAs;
      sAs.vSprint("$%lld"_Stz,i_Child + 1);
      pCard_As = new Card_As(sAs);
      pCard_StreamCol->vAppend(pCard_As);
    }
    pCard_StreamCol->vAppend(pCard_Type->pDupCardAndSubCards());
  }

  return pCard_StreamColList;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xlator::X_vFilter_CullPred(Card_Filter* p_filter, char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 nChildren = p_filter->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChildCard = p_filter->pGetChild(i_Child);

    eCK eCard = pChildCard->eCard();
    if (eCard != CK_BOOLCALC)
      continue;

    Card_BoolCalc* p_boolCalc = _pCard_BoolCalc(pChildCard);

    X_vBoolCalc_CullPred(p_boolCalc, cWithMark);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xlator::X_bFilter_IsEmptyFilter(Card_Filter* p_filter)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_BoolCalc* pCard_calcBool =
      _pCard_BoolCalc(p_filter->pFindSubCard(CK_BOOLCALC));

  if (0 == pCard_calcBool->nChildCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* add table and index                                                        */
/*----------------------------------------------------------------------------*/
void Xlator::X_vIdxColList_Ordinate(Card_IdxColList* p_idxColList, Card_Stream* pCard_Stream)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 iOrdLo = pCard_Stream->Y_iVal(P_OrdLo);

  Card_StmColList* pCard_StreamColList = pCard_Stream->pSub_stmColList();

  i8 nSubCards = p_idxColList->nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card_IdxCol* pCard_idxCol = _pCard_IdxCol(p_idxColList->pGetChild(i_Sub));

    i8 i_StreamColNo = C_UNKNOWN;
    Stz* szColNm = pCard_idxCol->sz();
    Card_StmCol* pCard_streamCol =
        X_pStmColList_Find_streamCol(pCard_StreamColList, szColNm, i_StreamColNo);

    s32 sOrdinal;
    sOrdinal.vSprint("$%lld"_Stz,iOrdLo+i_StreamColNo);
    pCard_idxCol->vValue(sOrdinal);

    pCard_idxCol->vMark1(MARK_CHANGED);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xlator::X_vJoinOrderSeq_ExtractJoinOrder(Card_JoinOrderSeq* p_joinOrderSeq,
                                              Card_PtndJoin* p_PtndJoin)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  eCK eChild;

  for (i8 i_Child = 0; i_Child < p_PtndJoin->nChildCards(); i_Child++) {
    Card_CmpPred* p_predCmp = _pCard_CmpPred(p_PtndJoin->pGetChild(i_Child));

    for (i8 i_Kid = p_predCmp->nChildCards() - 1; i_Kid >= 0; i_Kid--) {
      Card *pChild = p_predCmp->pGetChild(i_Kid, eChild);
      if (CK_COLUMNREF != eChild)
        continue;

      Card_ColumnRef* p_columnRef = _pCard_ColumnRef(pChild);
      s32 sTableSyn;
      T_vColumnRef_TableSyn_x(p_columnRef, sTableSyn);

      if (p_joinOrderSeq->bHasChildWithValue(sTableSyn))
        continue;

      Card_JoinOrder* p_joinOrder = new Card_JoinOrder(sTableSyn);
      p_joinOrderSeq->vAppend(p_joinOrder);
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xlator::X_vJoinPredList_Extract(Card_JoinPredList* p_joinPredList, Card_BoolCalc* p_boolCalc)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  auto bfExtract = [this,p_joinPredList,p_boolCalc](Card *pCard) {

    Card_CmpPred* p_predCmp = _pCard_CmpPred(pCard);

    if (! p_predCmp->sz()->bSame("=="_Stz)) //RAYMOND CAN DO BETTER!
      return true;

    Card_Term* p_term_0 = _pCard_Term(p_predCmp->pGetChild(0));
    Card_ColumnRef* p_columnRef_0 = p_term_0->pSub_columnRef();
    if (! p_columnRef_0)
      return true;
    Card_Term* p_term_1 = _pCard_Term(p_predCmp->pGetChild(1));
    Card_ColumnRef* p_columnRef_1= p_term_1->pSub_columnRef();
    if (!T_bColumnRef_IsColumnRef(p_columnRef_1))
      return true;

    Card_CmpPred* p_predCmp_new = new Card_CmpPred(p_predCmp->sz());
    p_predCmp_new->vAppend(p_columnRef_0->pDupCardAndSubCards());
    p_predCmp_new->vAppend(p_columnRef_1->pDupCardAndSubCards());

    p_joinPredList->vAppend(p_predCmp_new);

    return true;
  };

  // - - - - - - - - -

  p_boolCalc->vApplyRecursive(CK_CMPPRED, bfExtract);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xlator::X_vObyElist_Populate(Card_ObyElist* p_obyElist, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  IndexDef* pIndexDef = pIndex->pIndexDef();
  i8 nIdxCols = pIndexDef->nColumns();

  for (i8 i_IdxCol = 0; i_IdxCol < nIdxCols; i_IdxCol++) {

    Column* pIdxCol = pIndexDef->pGetColumn(i_IdxCol);

    Card_Expr* pCard_Expr = new Card_Expr;
    p_obyElist->vAppend(pCard_Expr);


    Stz* szType = aCT[pIdxCol->eColumnType()];
    Card_Type* pCard_Type = new Card_Type(szType);
    pCard_Expr->vAppend(pCard_Type);

    // as
    Card_As* pCard_As = new Card_As(pIdxCol->szColNm());
    pCard_Expr->vAppend(pCard_As);

    // exprCalc
    Card_RpnCalc* pCard_RpnCalc = new Card_RpnCalc;
    pCard_Expr->vAppend(pCard_RpnCalc);

    Card_Term* pCard_Term = new Card_Term(pIdxCol->szColNm());
    pCard_RpnCalc->vAppend(pCard_Term);

  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xlator::X_bPtndJoin_MayFollowAfter(Card_PtndJoin* p_ptndJoin, Card_SubSet* pPriorPtnJoins_sofar)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 nPtnJoins_sofar = pPriorPtnJoins_sofar->nChildCards();
  if (! nPtnJoins_sofar)
    return true;

  for (i8 i_PtnJoin_sofar = 0; i_PtnJoin_sofar < nPtnJoins_sofar; i_PtnJoin_sofar++) {
    Card_PtndJoin* p_ptndJoin_sofar =
        _pCard_PtndJoin(pPriorPtnJoins_sofar->pGetChild(i_PtnJoin_sofar));

    i8 nPredCmps_sofar = p_ptndJoin_sofar->nChildCards();
    for (i8 i_PredCmp_sofar = 0; i_PredCmp_sofar < nPredCmps_sofar; i_PredCmp_sofar++) {
      Card_CmpPred* p_predCmp_sofar =
          _pCard_CmpPred(p_ptndJoin_sofar->pGetChild(i_PredCmp_sofar));

      i8 nPredCmps_this = p_ptndJoin->nChildCards();
      for (i8 i_PredCmp_this = 0; i_PredCmp_this < nPredCmps_this; i_PredCmp_this++) {
        Card_CmpPred *p_predCmp_this =
            _pCard_CmpPred(p_ptndJoin->pGetChild(i_PredCmp_this));

        if (T_bCmpPred_OverlapsWith(p_predCmp_this, p_predCmp_sofar))
          return true;
      }

    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xlator::X_vPtndJoin_JoinEst(Card_PtndJoin* p_ptndJoin, Card_TableList* p_tableList)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 nJoinedBuff = 0;

  i8 nTables = p_tableList->nChildCards();
  for (i8 i_Table = 0; i_Table < nTables; i_Table++) {
    Card_Table *p_table =
        _pCard_Table(p_tableList->pGetChild(i_Table));

    if (p_table->bHasProperty(P_TableUsed))
      continue;

    i8 nPredCmps = p_ptndJoin->nChildCards();
    for (i8 i_PredCmp = 0; i_PredCmp < nPredCmps; i_PredCmp++) {
      Card_CmpPred *p_predCmp =
          _pCard_CmpPred(p_ptndJoin->pGetChild(i_PredCmp));

      if (T_bCmpPred_UsesTable(p_predCmp, p_table->cValue())) {
        nJoinedBuff += p_table->pSub_size()->szValue()->iToI8();
        p_table->Y_vSet(P_TableUsed);
      }
    }

  }

  p_ptndJoin->Y_vSet(P_JoinedBuff, nJoinedBuff);
}


/*----------------------------------------------------------------------------*/
/* is given <pred> within this stream?                                        */
/*----------------------------------------------------------------------------*/
Card_StmCol* Xlator::X_pStmColList_Find_streamCol(Card_StmColList* p_stmColList,
                                                  Stz *szColNm,
                                                  i8& i_StreamColNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Card_StmColList* pCard_StreamColList = p_stmColList->pSub_stmColList();

  Card_StmCol* pCard_streamCol = nullptr;

  i8 nSubCards = p_stmColList->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = p_stmColList->pGetChild(i_Child);

    if (CK_STMCOL != pSubCard->eCard())
      continue;

    //Card_As* pCard_as = pSubCard->pSub_as();
    if (pSubCard->bSame(szColNm)) {
      pCard_streamCol = _pCard_StmCol(pSubCard);
      i_StreamColNo = i_Child;
      break;
    }
  }

  return pCard_streamCol;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
