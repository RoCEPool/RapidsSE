/*============================================================================*/
/* Card_Table.cpp -- Table                                                    */
/*============================================================================*/

#include "Card_Table.hpp"

#include "Card_As.hpp"
#include "Card_IdxColList.hpp"
#include "Card_IndexList.hpp"
#include "Card_Filter.hpp"
#include "Card_BoolCalc.hpp"
#include "CardIter.hpp"
#include "Card_Index.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Bounds.hpp"
#include "Card_Bound.hpp"
#include "Card_Term.hpp"
#include "Card_TableNm.hpp"
#include "PtnDef.hpp"
#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Table, CK_TABLE)
__aNewFunction(Card_Table, CN_CARD)
__vDeleteFunction(Card_Table, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Table::Card_Table(void)
: Card_OP(CK_TABLE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Table::Card_Table(Stz* szTableNm)
    : Card_OP(CK_TABLE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vValue(szTableNm);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Table::~Card_Table(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* affix table column ordinals to all idx cols                                */
/*----------------------------------------------------------------------------*/
void Card_Table::vAffixTableColOrd(CardIter* pCI) {

  _static_

  while (pCI->G_bEachMainWith2Sub(CK_TABLE,CK_STREAM,CK_INDEXLIST)) {
    CardIter xCI(pCI->G.p_indexList);
    while ((pCI->G.p_index = _pCard_Index(xCI.pEachChild()))) {
      pCI->G.p_idxColList = pCI->G.p_index->pSub_idxColList();
      pCI->G.p_idxColList->vAffix_tblColOdn(pCI->G.p_stream);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* affix table syntax to all tables                                           */
/*----------------------------------------------------------------------------*/
void Card_Table::vAffixTableSyn(CardIter* pCI, char* cTableSyn) {

_static_

  while (pCI->G_bEachMainWithSub(CK_TABLE, CK_TABLENM))  {
    pCI->G.p_table->vSet_Syn(cTableSyn);
  }
}

/*----------------------------------------------------------------------------*/
/* append index list under all tables                                         */
/*----------------------------------------------------------------------------*/
void Card_Table::vAppendIndexList(CardIter* pCI) {

  _static_

  while (pCI->G_bEachMain(CK_TABLE)) {
    pCI->G.p_tableNm = pCI->G.p_table->pSub_tableNm();
    Table* pTable = Card::pAccess_Table(pCI->G.p_tableNm->sz());

    pCI->G.p_indexList = Card_IndexList::pDerive(pTable);

    pCI->G_vAppendChild(CK_TABLE,CK_INDEXLIST);
  }
}

/*----------------------------------------------------------------------------*/
/* derive bounds from filter                                                  */
/*----------------------------------------------------------------------------*/
void Card_Table::vDeriveBoundsFromFilter(CardIter* pCI) {

  _static_

  pCI->G.pSubQuery = pCI->G.pBoard->pSubQuery();

  while (pCI->G_bEachMainWithSub(CK_TABLE, CK_FILTER)) {
    bool bParamAllowedInBounds = false;
    if (pCI->G.pSubQuery->bIsCorrelated() && pCI->G.pSubQuery->bByExists())
      bParamAllowedInBounds = true;

    pCI->G.p_table->vBounds_DeriveFrom_filter(pCI->G.p_filter,bParamAllowedInBounds);
  }
}
/*----------------------------------------------------------------------------*/
/* affix table syntax to all tables                                           */
/*----------------------------------------------------------------------------*/
void Card_Table::vProduceTablePtns(CardIter* pCI) {

  _static_

  while (pCI->G_bEachMainWithSub(CK_TABLE, CK_TABLENM)) {

    pCI->G.p_tablePtn = Card_TablePtn::pDerive(pCI->G.p_table);
    pCI->G.p_table->vAppend(pCI->G.p_tablePtn);

    Card_TableNm* p_tableNm = pCI->G.p_table->pSub_tableNm();
    Table* pTable = AM::pTableFind(p_tableNm->sz());

    pCI->G.p_size = Card_Size::pDerive(pTable);
    pCI->G.p_table->vAppend(pCI->G.p_size);
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Table* Card_Table::pResolve_table_By_tblSyn(CardIter* pCI, char cTblSyn, i8 i_Before)
{
  _static_

  Card_Table* p_table = nullptr;

  pCI->G_vPushIter();

  if (pCI->G_bResolveCard(CK_TABLE,cTblSyn,i_Before)) {
    RASSERT(pCI->m_pCard_resolved->bIsCard(CK_TABLE));

    p_table = _pCard_Table(pCI->m_pCard_resolved);
  }

  pCI->G_vPopIter();

  return p_table;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* move <where> to <table>/<filter>                                           */
/*----------------------------------------------------------------------------*/
void Card_Table::vAddPredToTableFilter(CardIter* pCI, Card_PRED* p_PRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if ((pCI->G.p_filter = this->pSub_filter())) {
    pCI->G.p_boolCalc = pCI->G.p_filter->pSub_boolCalc();
    pCI->G.p_boolCalc->vPushPred(p_PRED);
  }
  else {
    pCI->G_vAppendNewChild(this,CK_FILTER);
    pCI->G_vAppendNewChild(pCI->G.p_filter,CK_BOOLCALC);
    pCI->G_vAppendChild(CK_BOOLCALC,p_PRED);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Table::vBounds_AddPred(Card_Index* p_index,
                                Card_Bounds* p_bounds,
                                Card_PRED *p_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  char cTblSyn = this->cValue();

  Card_Bound* p_bound;
  Card_PRED* p_childPRED;
  if (p_PRED->bIsCard(CK_ANDPRED)) {
    p_childPRED = (Card_PRED*)p_PRED->pGetChild(0);
    this->vBounds_AddPred(p_index,p_bounds,p_childPRED);
    p_childPRED = (Card_PRED*)p_PRED->pGetChild(1);
    this->vBounds_AddPred(p_index,p_bounds,p_childPRED);
  }
  else
  if (p_PRED->bIsCard(CK_CMPPRED)) {
    p_bound = Card_Bound::pBound_CreateFrom_PRED(p_PRED,cTblSyn);
    p_bounds->vAppend(p_bound);
  }
  else {
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* create bounds under table from filter                                      */
/*----------------------------------------------------------------------------*/
void Card_Table::vBounds_DeriveFrom_filter(Card_Filter* p_filter,
                                          bool bParamAllowedInBounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_BoolCalc* p_boolCalc = p_filter->pSub_boolCalc();
  RASSERT(1 == p_boolCalc->nChildCards());

  Card_PRED* p_pred_ofFilter_ROOT = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  // TODO: this is a hack to put bounds under one and only one index

  Card_IndexList* p_indexList = this->pSub_indexList();
  //Card_Index* p_index = this->pSub_index();

  CardIter xCI(p_indexList);
  while (Card_Index* p_index = _pCard_Index(xCI.pEachChild(CK_INDEX))) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    auto bLambdaFn = [this, p_index, bParamAllowedInBounds](Card_PRED *p_pred_Of_filter, bool bUpwards) {
      if (bUpwards)
        return C_CONTINUE_RECURSION;

      if (!p_pred_Of_filter->bIsCard(CK_CMPPRED))
        return C_CONTINUE_RECURSION;

      Card_CmpPred *p_cmpPred = _pCard_CmpPred(p_pred_Of_filter);

      //if (!T_bCmpPred_MaybeBounds(p_cmpPred,this,bParamAllowedInBounds))
      if (!p_cmpPred->bMaybeBounds_v2(this, p_index, bParamAllowedInBounds))
        return C_CONTINUE_RECURSION;

      // - - - -

      Card_Bounds *p_bounds = p_index->pSub_bounds();
      if (!p_bounds) {
        p_bounds = new Card_Bounds;
        p_index->vAppend(p_bounds);
      }

      char cTblSyn = this->cValue();
      Card_Bound *p_bound = Card_Bound::pBound_CreateFrom_cmpPred(p_cmpPred, cTblSyn);
      p_bounds->vAppend(p_bound);

      p_cmpPred->vMark1(MARK_UP);

      return C_CONTINUE_RECURSION;
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    bool bIgnore = p_pred_ofFilter_ROOT->bPredRecursive(bLambdaFn);

  }
}

/*----------------------------------------------------------------------------*/
/* table projection analysis                                                  */
/*----------------------------------------------------------------------------*/
void Card_Table::vProjectionAnalysis(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_tableNm = pSub_tableNm();

  Table* pTable = Card::pAccess_Table(pCI->G.p_tableNm->sz());
  StreamDef* pStreamDef = pTable->pStreamDef();

  Card_Projection* p_projection;
  if (pCI->G.p_projection)
    p_projection = _pCard_Projection(pCI->G.p_projection->pDupCardAndSubCards());
  else
    p_projection = new Card_Projection;

  i8 i_Expr = p_projection->nChildCards() + 1;

  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    Card_Expr* p_expr = new Card_Expr;
    p_expr->vValueSprint("$%lld"_Stz,i_Expr++);
    p_projection->vAppend(p_expr);

//    Card_StmCol* p_stmCol = new Card_StmCol();
//    p_stmCol->vValue(pColumn->szColNm());
//    p_expr->vAppend(p_stmCol);

    Stz* szType = aCT[pColumn->eColumnType()];
    Card_Type *pCard_Type = new Card_Type(szType);
    p_expr->vAppend(pCard_Type);

    Card_As* p_as = new Card_As(pColumn->szColNm());
    p_expr->vAppend(p_as);

    Card_ColNm* p_colNm = new Card_ColNm;
    p_colNm->vValue(pColumn->szColNm());
    p_expr->vAppend(p_colNm);

    Card_TblColOdn* p_tblColOdn = new Card_TblColOdn;
    p_tblColOdn->vValueSprint("$%c.%lld"_Stz,cValue(),i_Col+1);
    p_expr->vAppend(p_tblColOdn);

  }

  vAppend(p_projection);

  pCI->G.p_projection = p_projection;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Table::bScopeContains_Pred(Card_PRED* p_pred,
                                        Scope* pScope)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI(p_pred);
  while (Card_Term* p_term = (Card_Term*)xCI.pEachChild()) {
    if (! this->bScopeContains_term(p_term, pScope))
      return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* is given <term> within this table                                          */
/*----------------------------------------------------------------------------*/
bool Card_Table::bScopeContains_term(Card_Term* p_term,
                                        Scope* pScope)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Stz* szTerm = p_term->sz();

  // parameters are considered within scope
  if (Card::bIsParamOrdinal(szTerm))
    return true;

  // literals are considered within scope
  if (! Card::bIsTableColOrdinal(szTerm)) {
    return true;
  }

  // is the table column within scope?
  char cTblSyn_table = this->cValue();
  char cTblSyn_term = Card::cTableColSyn(szTerm);
//  if (cTblSyn_term <= cTblSyn_table)
//    return true;
  if (pScope->bInScope(cTblSyn_term))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* set synonym of table                                                       */
/*----------------------------------------------------------------------------*/
void Card_Table::vSet_Syn(char* cTableSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_TableNm* p_tableNm = this->pSub_tableNm();

  Table* pTable = Card::pAccess_Table(p_tableNm->sz());

  this->vMark1(MARK_CHANGED);

  char cTableSyn_actual = *cTableSyn;
  if (pTable->bIsReplicated())
    cTableSyn_actual = StrLib::cToUpper(cTableSyn_actual);

  this->vValueSprint("%c"_Stz,cTableSyn_actual);

  (*cTableSyn)++;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
