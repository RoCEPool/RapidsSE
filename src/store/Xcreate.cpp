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
/* create bounds under table from filter                                      */
/*----------------------------------------------------------------------------*/
void Xcreate::C_vBounds_DeriveFrom_filter(Card_Table* p_table,
                                          Card_Filter* p_filter,
                                          bool bParamAllowedInBounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  //M.p_table->vDump();

  Card_BoolCalc* p_boolCalc = p_filter->pSub_boolCalc();
  RASSERT(1 == p_boolCalc->nChildCards());

  Card_PRED* p_pred_ofFilter_ROOT = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this,p_table,bParamAllowedInBounds](Card_PRED* p_pred_Of_filter, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred_Of_filter->bIsCard(CK_CMPPRED))
      return C_CONTINUE_RECURSION;

    Card_CmpPred* p_cmpPred = _pCard_CmpPred(p_pred_Of_filter);
    p_cmpPred->vDump();

    if (!T_bCmpPred_MaybeBounds(p_cmpPred,p_table,bParamAllowedInBounds))
      return C_CONTINUE_RECURSION;

    // - - - -

    char cTblSyn = p_table->cValue();

    Card_Bounds* p_bounds = p_table->pSub_bounds();
    if (! p_bounds)
      p_bounds = new Card_Bounds;
    Card_Stream* p_stream = p_table->pSub_stream();
    p_table->vInsertAfter(p_bounds, p_stream);

    Card_Term* p_term = new Card_Term("BOUND"_Stz);
    p_bounds->vAppend(p_term);

    // - - - -

    // one of {pCard_term_0,pCard_term_1} is $In

    Card_Term* pTerm_0 = _pCard_Term(p_cmpPred->pGetChild(0));
    Card_Term* pTerm_1 = _pCard_Term(p_cmpPred->pGetChild(1));

    if (T_bValue_IsTableColOrdinal(pTerm_0->szValue())) {
      char cTblSyn_term_0 = E_cValue_TableColSyn(pTerm_0->szValue());
      if (cTblSyn_term_0 == cTblSyn) {
        p_term = new Card_Term("$I1"_Stz);
        p_bounds->vAppend(p_term);
        p_term = new Card_Term(pTerm_1->sz());
        p_bounds->vAppend(p_term);
      }
      else
      if (T_bValue_IsTableColOrdinal(pTerm_1->szValue())) {
        char cTblSyn_term_1 = E_cValue_TableColSyn(pTerm_1->szValue());
        if (cTblSyn_term_1 == cTblSyn) {
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

    p_cmpPred->vMark1(MARK_UP);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = M_bPredRecursive(p_pred_ofFilter_ROOT, bLambdaFn);

}
/*----------------------------------------------------------------------------*/
//void Xcreate::C_vBounds_DeriveFrom_filter(Card_Table* p_table, Card_Filter* p_filter)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
//
//  //M.p_table->vDump();
//
//  Card_BoolCalc* p_boolCalc = p_filter->pSub_boolCalc();
//  RASSERT(1 == p_boolCalc->nChildCards());
//
//  Card_PRED* p_pred_ofFilter_ROOT = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  auto bLambdaFn = [this,p_table](Card_PRED* p_pred_Of_filter, bool bUpwards)
//  {
//    if (bUpwards)
//      return C_TERMINATE_RECURSION;
//
//    if (! p_pred_Of_filter->bIsCard(CK_CMPPRED))
//      return C_CONTINUE_RECURSION;
//
//    Card_CmpPred* p_cmpPred = _pCard_CmpPred(p_pred_Of_filter);
//
//    Card_Term* pTerm_0 = _pCard_Term(p_cmpPred->pGetChild(0));
//    Card_Term* pTerm_1 = _pCard_Term(p_cmpPred->pGetChild(1));
//
//    if (! T_bValue_IsTableColOrdinal(pTerm_0->szValue()) ||
//        ! T_bValue_IsTableColOrdinal(pTerm_1->szValue()))
//      return C_CONTINUE_RECURSION;
//
//    // - - - -
//
//    Card_Bounds* p_bounds = p_table->pSub_bounds();
//    if (! p_bounds)
//      p_bounds = new Card_Bounds;
//    Card_Stream* p_stream = p_table->pSub_stream();
//    p_table->vInsertAfter(p_bounds, p_stream);
//
//    Card_Term* p_term = new Card_Term("BOUND"_Stz);
//    p_bounds->vAppend(p_term);
//
//    // - - - -
//
//    // one of {pCard_term_0,pCard_term_1} is $In
//
//    i8 iStreamOdnLo = p_stream->Y_iVal(P_OdnLo);
//    i8 iStreamOdnHi = p_stream->Y_iVal(P_OdnHi);
//
//    i8 i_iOdnLo_0 = E_iTerm_StreamColOdn(pTerm_0);
//    i8 i_iOdnLo_1 = E_iTerm_StreamColOdn(pTerm_1);
//
//    if (i_iOdnLo_0 >= iStreamOdnLo && i_iOdnLo_0 <= iStreamOdnHi) {
//      p_term = new Card_Term("$I1"_Stz);
//      p_bounds->vAppend(p_term);
//      p_term = new Card_Term(pTerm_1->sz());
//      p_bounds->vAppend(p_term);
//    }
//    else
//    if (i_iOdnLo_1 >= iStreamOdnLo && i_iOdnLo_1 <= iStreamOdnHi) {
//      p_term = new Card_Term("$I1"_Stz);
//      p_bounds->vAppend(p_term);
//      p_term = new Card_Term(pTerm_0->sz());
//      p_bounds->vAppend(p_term);
//    }
//    else {
//      NEVER_REACH
//    }
//
//    p_term = new Card_Term(p_cmpPred->sz());
//    p_bounds->vAppend(p_term);
//
//    p_cmpPred->vMark1(MARK_UP);
//
//    return C_CONTINUE_RECURSION;
//  };
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  bool bIgnore = M_bPredRecursive(p_pred_ofFilter_ROOT, bLambdaFn);
//
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_StmColList* Xcreate::C_pDerive_streamColList(Card_ELIST* p_ELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

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
/* convert <rpnCalc> to <boolCalc>                                            */
/*----------------------------------------------------------------------------*/
Card_BoolCalc* Xcreate::C_pConvert_rpnCalc_To_boolCalc(Card_RpnCalc* p_rpnCalc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Stz* szSqyNm;
  Card_AndPred* p_andPred = nullptr;
  Card_BolPred* p_bolPred = nullptr;
  Card_CmpPred* p_cmpPred = nullptr;
  Card_SqtPred* p_sqtPred = nullptr;
  Card_OrPred*  p_orPred  = nullptr;

  bool bSawSqy = false;

  Card_Term*    p_subQueryNm = nullptr;

  Card_SubSet*  p_TermSeries = new Card_SubSet;

  //G_vNewMember(CK_BOOLCALC);
  Card_BoolCalc* p_boolCalc = new Card_BoolCalc;

  p_rpnCalc->vDump();

  while (p_rpnCalc->nChildCards()) {
    Card_Term* p_term = _pCard_Term(p_rpnCalc->pPop());

    eTE eTopTerm = E_eTerm_TermEval(p_term);

    vTrace("%s %s\n"_Stz,aTE[eTopTerm],p_term->szValue());

    switch (eTopTerm) {

      case TE_AND:
        // <boolCalc> = <and>/<boolCalc>
        p_andPred = new Card_AndPred;
        p_andPred->vTransferChildren(p_boolCalc);
        p_boolCalc->vPush(p_andPred);
        p_andPred = nullptr;
        DELETE_CARD(p_term);
        break;

      case TE_OR:
        // <boolCalc> = <or>/<boolCalc>
        p_orPred = new Card_OrPred;
        p_orPred->vTransferChildren(p_boolCalc);
        p_boolCalc->vPush(p_orPred);
        p_orPred = nullptr;
        DELETE_CARD(p_term);
        break;

      case TE_COMPARE:
        // <boolCalc> = <cmpPred><termSeries><boolCalc>
        RASSERT(p_TermSeries->nChildCards() > 0);
        if (bSawSqy) {
          p_sqtPred = new Card_SqtPred;
          p_sqtPred->vValue(p_term->sz());
          p_sqtPred->vTransferChildren(p_TermSeries);
          p_boolCalc->vPush(p_sqtPred);
          p_sqtPred = nullptr;
        }
        else {
          p_cmpPred = new Card_CmpPred;
          p_cmpPred->vValue(p_term->sz());
          p_cmpPred->vTransferChildren(p_TermSeries);
          p_boolCalc->vPush(p_cmpPred);
          p_cmpPred = nullptr;
        }
        DELETE_CARD(p_term);
        bSawSqy = false;
        break;

      case TE_EXISTS:
      case TE_NOTEXISTS:
        // <boolCalc> = <exists><boolCalc>
        p_sqtPred = new Card_SqtPred;
        p_sqtPred->vValue(p_term->sz());
        p_sqtPred->vTransferChildren(p_TermSeries);
        p_boolCalc->vPush(p_sqtPred);
        p_sqtPred = nullptr;
        DELETE_CARD(p_term);
        break;

      case TE_IN:
      case TE_NOTIN:
        // <boolCalc> = <in><boolCalc>
        p_sqtPred = new Card_SqtPred;
        p_sqtPred->vValue(p_term->sz());
        p_sqtPred->vTransferChildren(p_TermSeries);
        p_boolCalc->vAppend(p_sqtPred);
        p_sqtPred = nullptr;
        DELETE_CARD(p_term);
        break;

//      case TE_BOOLFN:
//        p_sqtPred = new Card_SqtPred;
//        p_sqtPred->vValue(p_term->sz());
//        p_sqtPred->vTransferChildren(p_TermSeries);
//        p_boolCalc->vAppend(p_sqtPred);
//        p_sqtPred = nullptr;
//        DELETE_CARD(p_term);
//        break;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // <termSeries>

      case TE_EXECUTE:
        DELETE_CARD(p_term);
        break;

      case TE_BOOLFN:
      case TE_OTHER:
        if (StrLib::bBeginsWith(p_term->szValue()->azChar(),"__"))
          bSawSqy = true;
        p_TermSeries->vAppend(p_term);
        break;

      default:
        NEVER_REACH;
        break;
    }
  }

  p_boolCalc->vDump();

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
/* produce composite <index> of table                                         */
/*----------------------------------------------------------------------------*/
Card_Index* Xcreate::C_pProduce_index_ofTable(Table* pTable)
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
Card_IndexList* Xcreate::C_pProduce_indexList_ofTable(Table* pTable)
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
  }

  return p_indexList;
}

/*----------------------------------------------------------------------------*/
/* produce <size> of table                                                    */
/*----------------------------------------------------------------------------*/
Card_Size* Xcreate::C_pProduce_size_ofTable(Table* pTable)
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
Card_Stream* Xcreate::C_pProduce_stream_ofAggreg(Card_AggElist* p_aggElist)
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
Card_Stream* Xcreate::C_pProduce_stream_ofProject(Card_ProjElist* p_projElist)
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
      p_stmColNm->vValueSprint("__%lld"_Stz,i_Expr);;
    p_stmCol->vAppend(p_stmColNm);

    Card_Type* p_type = _pCard_Type(p_expr_type->pDupCardAndSubCards());
    p_stmCol->vAppend(p_type);
  }

  return p_stream;
}

/*----------------------------------------------------------------------------*/
/* produce composite <stream> of table                                        */
/*----------------------------------------------------------------------------*/
Card_Stream* Xcreate::C_pProduce_stream_ofTable(Table* pTable)
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
Card_TablePtn* Xcreate::C_pProduce_tablePtn_ofTable(Card_Table* p_table)
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
