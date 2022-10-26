/*============================================================================*/
/* Xreshape.cpp --                                                       */
/*============================================================================*/

#include "Xreshape.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Xreshape)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xreshape::Xreshape(eCN eClass)
: Xmodify(eClass)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xreshape::~Xreshape(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A */
/*----------------------------------------------------------------------------*/






/*----------------------------------------------------------------------------*/
/*T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------*/
/*X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* convert <calc> to <calcBool>                                               */
/*----------------------------------------------------------------------------*/
void Xreshape::X_vSubCard_ReplaceSub(Card* pSub, Card* pInParent, Card* pWith)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card* pParent = pInParent->pFindParent(pSub);

  pParent->vReplaceChild(pSub,pWith);
}


/*----------------------------------------------------------------------------*/
/* add table and index                                                        */
/*----------------------------------------------------------------------------*/
void Xreshape::X_vIdxColList_Ordinate(Card_IdxColList* p_idxColList, Card_Stream* pCard_Stream)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

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
void Xreshape::X_vJoinOrderSeq_ExtractJoinOrder(Card_JoinOrderSeq* p_joinOrderSeq,
                                                Card_PtndJoin* p_PtndJoin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  eCK eChild;

  for (i8 i_Child = 0; i_Child < p_PtndJoin->nChildCards(); i_Child++) {
    Card_CmpPred* p_predCmp = _pCard_CmpPred(p_PtndJoin->pGetChild(i_Child));

    for (i8 i_Kid = p_predCmp->nChildCards() - 1; i_Kid >= 0; i_Kid--) {
      Card *pChild = p_predCmp->pGetChild(i_Kid, eChild);
      if (CK_COLUMNREF != eChild)
        continue;

      Card_ColumnRef* p_columnRef = _pCard_ColumnRef(pChild);
      s32 sTableSyn;
      G_vColumnRef_TableSyn_x(p_columnRef, sTableSyn);

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
void Xreshape::X_vJoinPredList_Extract(Card_JoinPredList* p_joinPredList, Card_BoolCalc* p_boolCalc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

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
void Xreshape::X_vObyElist_Populate(Card_ObyElist* p_obyElist, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

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
void Xreshape::X_vPtndJoin_JoinEst(Card_PtndJoin* p_ptndJoin, Card_TableList* p_tableList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

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



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
