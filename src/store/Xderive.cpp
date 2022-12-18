/*============================================================================*/
/* Xderive.cpp --                                                       */
/*============================================================================*/

#include "Xderive.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Xderive)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xderive::Xderive(eCN eClass)
: Xmodify(eClass)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xderive::~Xderive(void) noexcept(false)
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Xderive::D_pDerive_Gby_expr_From_stmCol(Card_StmCol* p_stmCol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Expr*    p_expr    = new Card_Expr;
  Card_Type*    p_type    = new Card_Type(p_stmCol->pSub_type()->szValue());
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = new Card_Term(p_stmCol->pSub_stmColOdn()->szValue());

  p_expr->vAppend(p_type);
  p_expr->vAppend(p_rpnCalc);
  p_rpnCalc->vAppend(p_term);

  return p_expr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Xderive::D_pDerive_Proj_expr_From_Gby_expr(Card_Expr* p_expr_gby, Stz* szAs)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  s32 sKn;
  sKn.vSprint("$K%lld"_Stz,p_expr_gby->iCardNo()+1);

  Card_Expr*    p_expr    = new Card_Expr;
  Card_Type*    p_type    = new Card_Type(p_expr_gby->pSub_type()->szValue());
  Card_As*      p_as      = szAs ? new Card_As(szAs) : nullptr;
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = new Card_Term(sKn.szStz());

  p_expr->vAppend(p_type);
  if (p_as)
    p_expr->vAppend(p_as);
  p_expr->vAppend(p_rpnCalc);
  p_rpnCalc->vAppend(p_term);

  return p_expr;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Xderive::D_pDerive_Proj_expr_From_stmCol(Card_StmCol* p_stmCol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Expr*    p_expr    = new Card_Expr;
  Card_Type*    p_type    = new Card_Type(p_stmCol->pSub_type()->szValue());
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = new Card_Term(p_stmCol->pSub_stmColOdn()->szValue());

  p_expr->vAppend(p_type);
  p_expr->vAppend(p_rpnCalc);
  p_rpnCalc->vAppend(p_term);

  return p_expr;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xderive::D_vDerive_JoinOrderSeq(Card_JoinOrderSeq* p_joinOrderSeq,
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
      E_vColumnRef_TableSyn_x(p_columnRef, sTableSyn);

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
void Xderive::D_vDerive_JoinPredList(Card_JoinPredList* p_joinPredList, Card_BoolCalc* p_boolCalc)
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

  p_boolCalc->bApplyRecursive(CK_CMPPRED, bfExtract);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xderive::D_vDerive_ObyElist(Card_ObyElist* p_obyElist, Index* pIndex)
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
void Xderive::D_vDerive_PtndJoin(Card_PtndJoin* p_ptndJoin, Card_TableList* p_tableList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nJoinedBuff = 0;

  i8 nTables = p_tableList->nChildCards();
  for (i8 i_Table = 0; i_Table < nTables; i_Table++) {
    Card_Table *p_table =
        _pCard_Table(p_tableList->pGetChild(i_Table));

    if (p_table->Y_bHas(P_TableUsed))
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
char Xderive::D_cPRED_TableSyn_First(Card_PRED* p_PRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  char cTblSyn = ' ';
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this,&cTblSyn](Card_Term* p_term)
  {
    if (! T_bValue_IsTableColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    char cTblSyn_test = E_cValue_TableColSyn((p_term->szValue()));
    if (' ' == cTblSyn)
      cTblSyn = cTblSyn_test;
    else
    if (cTblSyn_test < cTblSyn)
      cTblSyn = cTblSyn_test;

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  p_PRED->vApplyRecursive_term(bLambdaFn);

  return cTblSyn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Xderive::D_pProject_Simple_expr_Of(Card_Expr* p_expr, i8 i_Col)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  //Card_Expr* p_expr_simple = _pCard_Expr(p_expr->pDupCardAndSubCards());

  Card_Expr* p_expr_simple = new Card_Expr;

  p_expr_simple->vAppend(p_expr->pSub_type()->pDupCardAndSubCards());

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term* p_term = new Card_Term;
  p_term->vValueSprint("$%lld"_Stz,i_Col);
  p_rpnCalc->vAppend(p_term);

  p_expr_simple->vAppend(p_rpnCalc);

  p_expr_simple->vAppend(p_expr->pSub_as()->pDupCardAndSubCards());

  return p_expr_simple;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
