/*============================================================================*/
/* Xcreate.cpp --                                                       */
/*============================================================================*/

#include "Xcreate.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Xcreate)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xcreate::Xcreate(eCN eClass)
: Xverify(eClass)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xcreate::~Xcreate(void) noexcept(false)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create a bound given cmpPred                                               */
/*----------------------------------------------------------------------------*/
Card_Bound* Xcreate::C_pBound_CreateFrom_cmpPred(Card_CmpPred* p_cmpPred,
                                                   char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Term* p_term;

  Card_Bound* p_bound = C_pBound_New(p_cmpPred->szValue());

  Card_Term* pTerm_0 = _pCard_Term(p_cmpPred->pGetChild(0));
  Card_Term* pTerm_1 = _pCard_Term(p_cmpPred->pGetChild(1));

  if (T_bValue_IsTableColOrdinal(pTerm_0->szValue())) {
    char cTblSyn_term_0 = E_cValue_TableColSyn(pTerm_0->szValue());
    if (cTblSyn_term_0 == cTblSyn) {
      p_term = new Card_Term("$I1"_Stz);
      p_bound->vAppend(p_term);
      p_term = new Card_Term(pTerm_1->sz());
      p_bound->vAppend(p_term);
    }
    else
    if (T_bValue_IsTableColOrdinal(pTerm_1->szValue())) {
      char cTblSyn_term_1 = E_cValue_TableColSyn(pTerm_1->szValue());
      if (cTblSyn_term_1 == cTblSyn) {
        p_term = new Card_Term("$I1"_Stz);
        p_bound->vAppend(p_term);
        p_term = new Card_Term(pTerm_0->sz());
        p_bound->vAppend(p_term);
      } else {
        NEVER_REACH
      }
    }
  }
  else
  if (T_bValue_IsTableColOrdinal(pTerm_1->szValue())) {
    char cTblSyn_term_1 = E_cValue_TableColSyn(pTerm_1->szValue());
    if (cTblSyn_term_1 == cTblSyn) {
      p_term = new Card_Term("$I1"_Stz);
      p_bound->vAppend(p_term);
      p_term = new Card_Term(pTerm_0->sz());
      p_bound->vAppend(p_term);
    }
    else {
      NEVER_REACH
    }
  }
  else {
    NEVER_REACH
  }

  p_term = new Card_Term(p_cmpPred->sz());
  p_bound->vAppend(p_term);

  return p_bound;
}

/*----------------------------------------------------------------------------*/
/* create a bound given cmpPred                                               */
/*----------------------------------------------------------------------------*/
Card_Bound* Xcreate::C_pBound_New(Stz* szRelnOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Stz* szBound = E_szBound(szRelnOp);
  Card_Term* p_term_bound = new Card_Term(szBound);

  if (szRelnOp->bSame(">"_Stz) || szRelnOp->bSame(">="_Stz)) {
   Card_LBound* p_lBound =  new Card_LBound;
   p_lBound->vAppend(p_term_bound);
   return p_lBound;
  }

  if (szRelnOp->bSame("<"_Stz) || szRelnOp->bSame("<="_Stz)) {
    Card_UBound* p_uBound =  new Card_UBound;
    p_uBound->vAppend(p_term_bound);
    return p_uBound;
  }

  if (szRelnOp->bSame("=="_Stz)) {
    Card_EBound* p_eBound =  new Card_EBound;
    p_eBound->vAppend(p_term_bound);
    return p_eBound;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* create a bound given pred                                                  */
/*----------------------------------------------------------------------------*/
Card_Bound* Xcreate::C_pBound_CreateFrom_PRED(Card_PRED* p_PRED, char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // todo: hack

  Card_CmpPred* p_cmpPred = _pCard_CmpPred(p_PRED);

  Card_Bound* p_bound = C_pBound_CreateFrom_cmpPred(p_cmpPred,cTblSyn);

  return p_bound;
}


/*----------------------------------------------------------------------------*/
/* create a bounds given tblColOdn                                            */
/*----------------------------------------------------------------------------*/
Card_Bounds* Xcreate::C_pBounds_CreateFrom_PRED(Card_PRED* p_PRED,
                                                char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Bound* p_bound;
  Card_CmpPred* p_cmpPred;

  Card_Bounds* p_bounds = new Card_Bounds;

  if (p_PRED->bIsCard(CK_CMPPRED)) {
    p_bound = C_pBound_CreateFrom_PRED(p_PRED,cTblSyn);
    p_bounds->vAppend(p_bound);
  }
  else
  if (p_PRED->bIsCard(CK_ANDPRED)) {
    p_cmpPred = _pCard_CmpPred(p_PRED->pGetChild(0));
    p_bound = C_pBound_CreateFrom_PRED(p_cmpPred,cTblSyn);
    p_bounds->vAppend(p_bound);

    p_cmpPred = _pCard_CmpPred(p_PRED->pGetChild(1));
    p_bound = C_pBound_CreateFrom_PRED(p_cmpPred,cTblSyn);
    p_bounds->vAppend(p_bound);
  }
  else {
    NEVER_REACH
  }

  return p_bounds;
}
/*----------------------------------------------------------------------------*/
//Card_Bounds* Xcreate::C_pBounds_CreateFrom_PRED(Card_PRED* p_PRED,
//                                                char cTblSyn)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
//
//  // todo: hack
//
//  Card_CmpPred* p_cmpPred = _pCard_CmpPred(p_PRED);
//
//  Card_Bounds* p_bounds = C_pBounds_CreateFrom_cmpPred(p_cmpPred,cTblSyn);
//
//  return p_bounds;
//}

/*----------------------------------------------------------------------------*/
/* create a bounds given tblColOdn                                            */
/*----------------------------------------------------------------------------*/
Card_Bounds* Xcreate::C_pBounds_Create_Eq_ForGivenCol(Stz* szTblColOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Bounds* p_bounds = new Card_Bounds;

  Card_Term* p_term = new Card_Term("BOUND"_Stz);
  p_bounds->vAppend(p_term);

  p_term = new Card_Term("$I1"_Stz);
  p_bounds->vAppend(p_term);
  p_term = new Card_Term(szTblColOdn);
  p_bounds->vAppend(p_term);
  p_term = new Card_Term("=="_Stz);
  p_bounds->vAppend(p_term);

  return p_bounds;
}

/*----------------------------------------------------------------------------*/
/* create a bounds given tblColOdn                                            */
/*----------------------------------------------------------------------------*/
Card_Bounds* Xcreate::C_pBounds_CreateFrom_cmpPred(Card_CmpPred* p_cmpPred,
                                                   char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Term* p_term;
  Stz* szBound;

  Card_Bounds* p_bounds = new Card_Bounds;


  Card_Term* pTerm_0 = _pCard_Term(p_cmpPred->pGetChild(0));
  Card_Term* pTerm_1 = _pCard_Term(p_cmpPred->pGetChild(1));

  if (T_bValue_IsTableColOrdinal(pTerm_0->szValue())) {
    char cTblSyn_term_0 = E_cValue_TableColSyn(pTerm_0->szValue());
    if (cTblSyn_term_0 == cTblSyn) {
      szBound = E_szBound(pTerm_1->sz());
      p_term = new Card_Term(szBound);
      p_bounds->vAppend(p_term);
      p_term = new Card_Term("$I1"_Stz);
      p_bounds->vAppend(p_term);
      p_term = new Card_Term(pTerm_1->sz());
      p_bounds->vAppend(p_term);
    }
    else
    if (T_bValue_IsTableColOrdinal(pTerm_1->szValue())) {
      char cTblSyn_term_1 = E_cValue_TableColSyn(pTerm_1->szValue());
      if (cTblSyn_term_1 == cTblSyn) {
        szBound = E_szBound(pTerm_1->sz());
        p_term = new Card_Term(szBound);
        p_bounds->vAppend(p_term);
        p_term = new Card_Term("$I1"_Stz);
        p_bounds->vAppend(p_term);
        p_term = new Card_Term(pTerm_0->sz());
        p_bounds->vAppend(p_term);
      } else {
        NEVER_REACH
      }
    }
  }
  else
  if (T_bValue_IsTableColOrdinal(pTerm_1->szValue())) {
    char cTblSyn_term_1 = E_cValue_TableColSyn(pTerm_1->szValue());
    if (cTblSyn_term_1 == cTblSyn) {
      szBound = E_szBound(pTerm_1->sz());
      p_term = new Card_Term(szBound);
      p_bounds->vAppend(p_term);
      p_term = new Card_Term("$I1"_Stz);
      p_bounds->vAppend(p_term);
      p_term = new Card_Term(pTerm_0->sz());
      p_bounds->vAppend(p_term);
    }
    else {
      NEVER_REACH
    }
  }
  else {
    NEVER_REACH
  }

  p_term = new Card_Term(p_cmpPred->sz());
  p_bounds->vAppend(p_term);

  return p_bounds;
}

/*----------------------------------------------------------------------------*/
/* create bounds under table from filter                                      */
/*----------------------------------------------------------------------------*/
void Xcreate::C_vBounds_DeriveFrom_filter(Card_Table* p_table,
                                          Card_Filter* p_filter,
                                          bool bParamAllowedInBounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_BoolCalc* p_boolCalc = p_filter->pSub_boolCalc();
  RASSERT(1 == p_boolCalc->nChildCards());

  Card_PRED* p_pred_ofFilter_ROOT = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  // TODO: this is a hack to put bounds under one and only one index

  Card_IndexList* p_indexList = p_table->pSub_indexList();
  Card_Index* p_index = p_table->pSub_index();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this,p_table,p_index,bParamAllowedInBounds](Card_PRED* p_pred_Of_filter, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred_Of_filter->bIsCard(CK_CMPPRED))
      return C_CONTINUE_RECURSION;

    Card_CmpPred* p_cmpPred = _pCard_CmpPred(p_pred_Of_filter);

    //if (!T_bCmpPred_MaybeBounds(p_cmpPred,p_table,bParamAllowedInBounds))
    if (!T_bCmpPred_MaybeBounds_v2(p_cmpPred,p_table,bParamAllowedInBounds))
      return C_CONTINUE_RECURSION;

    // - - - -

    Card_Bounds* p_bounds = p_table->pSub_bounds();
    if (! p_bounds) {
      p_bounds = new Card_Bounds;
      p_index->vAppend(p_bounds);
    }

    char cTblSyn = p_table->cValue();
    Card_Bound* p_bound = C_pBound_CreateFrom_cmpPred(p_cmpPred,cTblSyn);
    p_bounds->vAppend(p_bound);

    p_cmpPred->vMark1(MARK_UP);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = M_bPredRecursive(p_pred_ofFilter_ROOT, bLambdaFn);

}
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_StmColList* Xcreate::C_pStmColList_DeriveFrom_ELIST(Card_ELIST* p_ELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_StmColList* p_stmColList = new Card_StmColList;

  CardIter xCI(p_ELIST);
  while (Card_Expr* p_expr = _pCard_Expr(xCI.pEachChild(CK_EXPR))) {

    Card *p_type = p_expr->pFindSubCard(CK_TYPE);
    Card *p_as = p_expr->pFindSubCard(CK_AS);

    Card_StmCol* pCard_StreamCol = new Card_StmCol;
    p_stmColList->vAppend(pCard_StreamCol);

    if (p_as)
      p_as = _pCard_As(p_as->pDupCardAndSubCards());
    else {
      s32 sAs;
      sAs.vSprint("$%lld"_Stz,xCI.i_Child() + 1);
      p_as = new Card_As(sAs);
    }

    pCard_StreamCol->vAppend(p_as);
    pCard_StreamCol->vAppend(p_type->pDupCardAndSubCards());
  }

  return p_stmColList;
}

/*----------------------------------------------------------------------------*/
/* convert <rpnCalc> to <boolCalc>                                            */
/*----------------------------------------------------------------------------*/
Card_BoolCalc* Xcreate::C_pConvert_rpnCalc_To_boolCalc(Card_RpnCalc* p_rpnCalc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Stz* szSqyNm;
  Card_AndPred* p_andPred = nullptr;
  Card_BolPred* p_bolPred = nullptr;
  Card_CmpPred* p_cmpPred = nullptr;
  Card_NotPred* p_notPred = nullptr;
  Card_OrPred*  p_orPred  = nullptr;
  Card_RoPred*  p_roPred  = nullptr;

  Card*  p_childA  = nullptr;
  Card*  p_childB  = nullptr;

  bool bSawSqy = false;

  Card_Term*    p_subQueryNm = nullptr;

  Card_SubSet*  p_TermSeries = new Card_SubSet;

  //G_vNewMember(CK_BOOLCALC);
  Card_BoolCalc* p_boolCalc = new Card_BoolCalc;

  while (p_rpnCalc->nChildCards()) {
    Card_Term* p_term_next = _pCard_Term(p_rpnCalc->pPop());

    eTE eTerm_next = E_eTerm_TermEval(p_term_next);

    //vTrace("%s %s\n"_Stz, aTE[eTerm_next], p_term_next->szValue());

    switch (eTerm_next) {

      case TE_AND:
        // <boolCalc> = <and>/<boolCalc>
        p_childB = p_boolCalc->pPlopChild();
        p_childA = p_boolCalc->pPlopChild();

        p_andPred = new Card_AndPred;
        p_andPred->vAppend(p_childA);
        p_andPred->vAppend(p_childB);

        p_boolCalc->vAppend(p_andPred);
        p_andPred = nullptr;
        p_childA  = nullptr;
        p_childB  = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolCalc->vDump(0,"AND"_Stz);
        break;

      case TE_OR:
        // <boolCalc> = <or>/<boolCalc>
        p_childB = p_boolCalc->pPlopChild();
        p_childA = p_boolCalc->pPlopChild();

        p_orPred = new Card_OrPred;
        p_orPred->vAppend(p_childA);
        p_orPred->vAppend(p_childB);

        p_boolCalc->vAppend(p_orPred);
        p_orPred = nullptr;
        p_childA = nullptr;
        p_childB = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolCalc->vDump(0,"OR"_Stz);
        break;

      case TE_NOT:
        p_childA = p_boolCalc->pPlopChild();

        p_notPred = new Card_NotPred;
        p_notPred->vAppend(p_childA);

        p_boolCalc->vAppend(p_notPred);
        p_notPred = nullptr;
        p_childA  = nullptr;
        DELETE_CARD(p_term_next);
        break;

      case TE_COMPARE:
        // <boolCalc> = <cmpPred><termSeries><boolCalc>
        RASSERT(p_TermSeries->nChildCards() > 0);
//        if (bSawSqy) {
//          p_roPred = new Card_RoPred;
//          p_roPred->vValue(p_term_next->sz());
//          p_roPred->vTransferChildren(p_TermSeries);
//          //p_boolCalc->vPush(p_roPred);
//          p_boolCalc->vAppend(p_roPred);
//          p_roPred = nullptr;
//        }
//        else {
          p_cmpPred = new Card_CmpPred;
          p_cmpPred->vValue(p_term_next->sz());
          p_cmpPred->vTransferChildren(p_TermSeries);
          if (T_bCmpPred_Contains_Sqy_Call(p_cmpPred))
            p_cmpPred->vCard(CK_ROPRED);
          //p_boolCalc->vPush(p_cmpPred);
          p_boolCalc->vAppend(p_cmpPred);
          p_cmpPred = nullptr;
//        }
        DELETE_CARD(p_term_next);
        bSawSqy = false;
        //p_boolCalc->vDump(0,"COMPARE"_Stz);
        break;

      case TE_EXISTS:
      case TE_NOTEXISTS:
        // <boolCalc> = <exists><boolCalc>
        p_roPred = new Card_RoPred;
        p_roPred->vValue(p_term_next->sz());
        p_roPred->vTransferChildren(p_TermSeries);
        p_boolCalc->vAppend(p_roPred);
        p_roPred = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolCalc->vDump(0,"EXISTS"_Stz);
        break;

      case TE_IN:
      case TE_NOTIN:
        // <boolCalc> = <in><boolCalc>
        p_roPred = new Card_RoPred;
        p_roPred->vValue(p_term_next->sz());
        p_roPred->vTransferChildren(p_TermSeries);
        p_boolCalc->vAppend(p_roPred);
        p_roPred = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolCalc->vDump(0,"IN"_Stz);
        break;

      case TE_BOOLFN:
        p_bolPred = new Card_BolPred;
        p_bolPred->vValue(p_term_next->sz());
        p_bolPred->vTransferChildren(p_TermSeries);
        p_boolCalc->vAppend(p_bolPred);
        p_bolPred = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolCalc->vDump(0,"BOOLFN"_Stz);
        break;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // <termSeries>

      case TE_EXECUTE:
        DELETE_CARD(p_term_next);
        break;

      case TE_OTHER:
        if (StrLib::bBeginsWith(p_term_next->szValue()->azChar(), "__"))
          bSawSqy = true;
        p_TermSeries->vAppend(p_term_next);
        //p_TermSeries->vDump(0,"TERM_SERIES"_Stz);
        break;

      default:
        NEVER_REACH;
        break;
    }
  }

  if (p_TermSeries->nItems()) {
    RASSERT(0 == p_boolCalc->nChildCards());
    p_boolCalc->vDump(0,"boolCalc"_Stz);
    p_TermSeries->vDump(0,"termSeries"_Stz);

    p_bolPred = new Card_BolPred;
    p_bolPred->vTransferChildren(p_TermSeries);

    p_boolCalc->vAppend(p_bolPred);
  }

  RASSERT(0 == p_TermSeries->nChildCards());
  delete p_TermSeries;

  return p_boolCalc;
}

/*----------------------------------------------------------------------------*/
/* create a filter from/duplicating given PRED                                */
/*----------------------------------------------------------------------------*/
Card_Filter* Xcreate::C_pFilter_From_PRED(Card_PRED* p_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Filter* p_filter = new Card_Filter;

  Card_BoolCalc* p_boolCalc = new Card_BoolCalc;
  p_filter->vAppend(p_boolCalc);

  p_boolCalc->vAppend(p_PRED->pDupCardAndSubCards());

  return p_filter;
}

/*----------------------------------------------------------------------------*/
/* produce composite <index> of table                                         */
/*----------------------------------------------------------------------------*/
Card_Index* Xcreate::C_pProduce_index_Of_table(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (!pTable)
    return nullptr;

  IdxVec* pIdxVec = pTable->pIdxVec();
  if (! pIdxVec)
    return nullptr;

  Index* pIndex = pIdxVec->pGet(0);

  Card_Index*  p_index = new Card_Index;

  Card_IndexNm* p_indexNm = new Card_IndexNm;
  p_indexNm->vValue(pIndex->szIndexNm());
  p_index->vAppend(p_indexNm);

  Card_IdxColList* p_idxColList = new Card_IdxColList;
  p_index->vAppend(p_idxColList);

  IndexDef* pIndexDef = pIndex->pIndexDef();

  i8 nColumns = pIndexDef->nColumns();
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

    Column* pColumn = pIndexDef->pGetColumn(i_Col);
    Card_IdxCol* p_idxCol = new Card_IdxCol;
    p_idxCol->vValue(pColumn->szColNm());

    p_idxColList->vAppend(p_idxCol);

    Card_IdxColNm* p_idxColNm = new Card_IdxColNm;
    p_idxColNm->vValue(pColumn->szColNm());
    p_idxCol->vAppend(p_idxColNm);
  }

  return p_index;
}

/*----------------------------------------------------------------------------*/
/* produce composite <indexList> of table                                     */
/*----------------------------------------------------------------------------*/
Card_IndexList* Xcreate::C_pProduce_indexList_Of_table(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (!pTable)
    return nullptr;

  IdxVec* pIdxVec = pTable->pIdxVec();
  if (! pIdxVec)
    return nullptr;

  Card_IndexList* p_indexList = new Card_IndexList;

  i8 nIndexes = pIdxVec->nItems();
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    Index* pIndex = pIdxVec->pGet(i_Index);

    Card_Index*  p_index = new Card_Index;
    p_indexList->vAppend(p_index);

    Card_IndexNm* p_indexNm = new Card_IndexNm;
    p_indexNm->vValue(pIndex->szIndexNm());
    p_index->vAppend(p_indexNm);

    IndexDef* pIndexDef = pIndex->pIndexDef();
    if (pIndexDef->bIsUnique()) {
      p_index->vAppendNewCardTail(CK_UNIQUE);
    }

    Card_IdxColList* p_idxColList = new Card_IdxColList;
    p_index->vAppend(p_idxColList);

    i8 nColumns = pIndexDef->nColumns();
    for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

      Column* pColumn = pIndexDef->pGetColumn(i_Col);
      Card_IdxCol* p_idxCol = new Card_IdxCol;
      p_idxCol->vValue(pColumn->szColNm());

      p_idxColList->vAppend(p_idxCol);

      Card_IdxColNm* p_idxColNm = new Card_IdxColNm;
      p_idxColNm->vValue(pColumn->szColNm());
      p_idxCol->vAppend(p_idxColNm);
    }
  }

  return p_indexList;
}

/*----------------------------------------------------------------------------*/
/* produce <size> of table                                                    */
/*----------------------------------------------------------------------------*/
Card_Size* Xcreate::C_pProduce_size_Of_table(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (! pTable) {
    vTrace("A: m_pTable not set\n"_Stz);
    return nullptr;
  }

  i8 jTableSize = pTable->pStreamDef()->jTableSize();

  Card_Size* p_size = new Card_Size;
  s32 sSize(jTableSize);
  p_size->vValue(sSize);

  return p_size;
}

/*----------------------------------------------------------------------------*/
/* produce composite <stream> of <aggreg>                                     */
/*----------------------------------------------------------------------------*/
Card_Stream* Xcreate::C_pProduce_stream_Of_aggreg(Card_AggElist* p_aggElist)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Stream *p_stream = new Card_Stream;
  Card_StmColList *p_stmColList = new Card_StmColList;
  p_stream->vAppend(p_stmColList);

  i8 i_Expr = 0;
  CardIter xCI_elist(p_aggElist);
  while (Card_Expr* p_expr = _pCard_Expr(xCI_elist.pEachChild())) {
    Card_As*   p_expr_as   = nullptr;
    Card_Type* p_expr_type = nullptr;

    i_Expr++;
    CardIter xCI_expr(p_expr);
    while (Card* p_expr_sub = xCI_expr.pEachChild()) {

      eCK e_expr_sub = p_expr_sub->eCard();

      if (CK_AS == e_expr_sub)
        p_expr_as = _pCard_As(p_expr_sub);
      else
      if (CK_TYPE == e_expr_sub)
        p_expr_type = _pCard_Type(p_expr_sub);

    }

    Card_StmCol* p_stmCol = new Card_StmCol();
    p_stmColList->vAppend(p_stmCol);

    Card_StmColNm* p_stmColNm = new Card_StmColNm();
    if (p_expr_as)
      p_stmColNm->vValue(p_expr_as->szValue());
    else
      p_stmColNm->vValueSprint("__%lld"_Stz,i_Expr);;
    p_stmCol->vAppend(p_stmColNm);

    Card_Type* p_type = _pCard_Type(p_expr_type->pDupCardAndSubCards());
    p_stmCol->vAppend(p_type);
  }

  return p_stream;
}

/*----------------------------------------------------------------------------*/
/* produce composite <stream> of <project>                                    */
/*----------------------------------------------------------------------------*/
Card_Stream* Xcreate::C_pProduce_stream_Of_project(Card_ProjElist* p_projElist)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Stream *p_stream = new Card_Stream;
  Card_StmColList *p_stmColList = new Card_StmColList;
  p_stream->vAppend(p_stmColList);

  i8 i_Expr = 0;
  CardIter xCI_elist(p_projElist);
  while (Card_Expr* p_expr = _pCard_Expr(xCI_elist.pEachChild())) {
    Card_As*   p_expr_as   = nullptr;
    Card_Type* p_expr_type = nullptr;

    i_Expr++;
    CardIter xCI_expr(p_expr);
    while (Card* p_expr_sub = xCI_expr.pEachChild()) {

      eCK e_expr_sub = p_expr_sub->eCard();

      if (CK_AS == e_expr_sub)
        p_expr_as = _pCard_As(p_expr_sub);
      else
      if (CK_TYPE == e_expr_sub)
        p_expr_type = _pCard_Type(p_expr_sub);

    }

    Card_StmCol* p_stmCol = new Card_StmCol();
    p_stmColList->vAppend(p_stmCol);

    Card_StmColNm* p_stmColNm = new Card_StmColNm();
    if (p_expr_as)
      p_stmColNm->vValue(p_expr_as->szValue());
    else
      p_stmColNm->vValueSprint("__%lld"_Stz,i_Expr-1);
    p_stmCol->vAppend(p_stmColNm);

    Card_Type* p_type = _pCard_Type(p_expr_type->pDupCardAndSubCards());
    p_stmCol->vAppend(p_type);
  }

  return p_stream;
}

/*----------------------------------------------------------------------------*/
/* produce composite <stream> of table                                        */
/*----------------------------------------------------------------------------*/
Card_Stream* Xcreate::C_pProduce_stream_Of_table(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (!pTable)
    return nullptr;

  StreamDef* pStreamDef = pTable->pStreamDef();

  Card_Stream* p_stream = new Card_Stream;
  Card_StmColList* p_stmColList = new Card_StmColList;
  p_stream->vAppend(p_stmColList);

  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    Card_StmCol* p_stmCol = new Card_StmCol();
    p_stmCol->vValue(pColumn->szColNm());
    p_stmColList->vAppend(p_stmCol);

    Card_StmColNm* p_stmColNm = new Card_StmColNm();
    p_stmColNm->vValue(pColumn->szColNm());
    p_stmCol->vAppend(p_stmColNm);

    Stz* szType = aCT[pColumn->eColumnType()];
    Card_Type *pCard_Type = new Card_Type(szType);
    p_stmCol->vAppend(pCard_Type);
  }

  return p_stream;
}

/*----------------------------------------------------------------------------*/
/* produce <tablePtn> of table                                                */
/*----------------------------------------------------------------------------*/
Card_TablePtn* Xcreate::C_pProduce_tablePtn_Of_table(Card_Table* p_table)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_TableNm* p_tableNm = p_table->pSub_tableNm();
  Table* pTable = A_pAccess_Table(p_tableNm->sz());
  char cTableSyn = p_table->cValue();

  PtnDef* pPtnDef = pTable->pPtnDef();
  Column* pPtnColumn = pPtnDef->pGetColumn(0);

  StreamDef* pStreamDef = pTable->pStreamDef();
  Column* pStmColumn = pStreamDef->pFindColumn(pPtnColumn->szColNm());
  i8 iStmColNo = pStmColumn->iColumnNo();
  s32 sTablePtn;
  sTablePtn.vSprint("$$%c.%lld"_Stz, cTableSyn, iStmColNo + 1);

  Card_TablePtn* p_tablePtn = new Card_TablePtn;
  p_tablePtn->vValue(sTablePtn);

  return p_tablePtn;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
