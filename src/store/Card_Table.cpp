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
/* access table                                                               */
/*----------------------------------------------------------------------------*/
Table* Card_Table::pAccess_Table(Stz* szTableNm)
{
  _static_

  Table* pTable = AM::pTableFind(szTableNm);

  return pTable;
}


/*----------------------------------------------------------------------------*/
/* affix table column ordinals to all idx cols                                */
/*----------------------------------------------------------------------------*/
void Card_Table::vAffixTableColOrd(CardIter* pCI) {

  _static_

  while (pCI->G_bEachMainWith2Sub(CK_TABLE,CK_PROJECTION,CK_INDEXLIST)) {

    CardIter xCI(pCI->G.p_indexList);
    while (xCI.G_bEachMain(CK_INDEX)) {
      pCI->G.p_idxColList = xCI.G.p_index->pSub_idxColList();
      pCI->G.p_idxColList->vAffix_tblColOdn(pCI->G.p_projection);
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
    Table* pTable = Card_Table::pAccess_Table(pCI->G.p_tableNm->szValue());

    pCI->G.p_indexList = Card_IndexList::pDerive(pTable);

    pCI->G_vAppendChild(CK_TABLE,CK_INDEXLIST);
  }
}

/*----------------------------------------------------------------------------*/
/* derive bounds from filter                                                  */
/*----------------------------------------------------------------------------*/
void Card_Table::vDeriveBoundsFromFilter(CardIter* pCI) {

  _static_

  pCI->vRestart();
  while (pCI->G_bEachMainWithSub(CK_TABLE, CK_INDEXLIST))
    pCI->G.p_indexList->vDeleteAll_bounds();

  pCI->vRestart();
  while (pCI->G_bEachMainWithSub(CK_TABLE, CK_FILTER))
    pCI->G.p_table->vDeriveBoundsFrom_filter(pCI->G.p_filter);
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
    Table* pTable = AM::pTableFind(p_tableNm->szValue());

    pCI->G.p_size = Card_Size::pDerive(pTable);
    pCI->G.p_table->vAppend(pCI->G.p_size);
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Table* Card_Table::pFind_By_tblSyn(CardIter* pCI, char cTblSyn, i8 i_Before)
{
  _static_

  Card_Table* p_table = nullptr;

  if (C_UNKNOWN == i_Before)
    i_Before = pCI->i_Child();

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card *pMain = pCI->G.pDeck->pGetChild(i_Main);
    if (pMain->bIsCard(CK_TABLE)) {
      Card_Table* p_table_test = _pCard_Table(pMain);
      if (cTblSyn == p_table_test->cValue()) {
        p_table = p_table_test;
        break;
      }
    }
  }

  return p_table;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Card_Table::nTables(CardIter* pCI)
{
  _static_

  i8 nTables = 0;

  pCI->vRestart();
  while (pCI->G_bEachMain(CK_TABLE))
    nTables++;

  return nTables;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Card_Table::nTables_foldBreak(CardIter* pCI)
{
  _static_

  i8 nTables = 0;

  pCI->vRestart();
  while (pCI->G_bEachMain()) {
    if (pCI->G.p_MainCard->bIsCard(CK_FOLD))
      break;
    if (pCI->G.p_MainCard->bIsCard(CK_TABLE))
    nTables++;
  }

  return nTables;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Card_Table::nTables_reverse_foldBreak(CardIter* pCI)
{
  _static_

  i8 nTables = 0;

  pCI->vRestart();
  while (pCI->G_bEachMain_reverse_foldBreak(CK_TABLE))
    nTables++;

  return nTables;
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
    p_bound = Card_Bound::pCreateFrom_PRED(p_PRED, cTblSyn);
    p_bounds->vAppend(p_bound);
  }
  else {
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* create bounds under table from filter                                      */
/*----------------------------------------------------------------------------*/
void Card_Table::vDeriveBoundsFrom_filter(Card_Filter* p_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_IndexList* p_indexList = this->pSub_indexList();

  Card_BoolCalc* p_boolCalc = p_filter->pSub_boolCalc();
  RASSERT(1 == p_boolCalc->nChildCards());
  Card_PRED* p_pred_root = p_boolCalc->pRootPred();

  CardIter xCI(p_indexList);
  while (xCI.G_bEachMain(CK_INDEX)) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    Card_Index* p_index = xCI.G.p_index;
    auto bLambdaFn = [this, p_index](Card_PRED *p_pred_Of_filter, bool bUpwards)
    {
      if (bUpwards)
        return C_CONTINUE_RECURSION;

      if (! p_pred_Of_filter->bIsCard(CK_CMPPRED))
        return C_CONTINUE_RECURSION;

      if (!p_pred_Of_filter->bIsIndexBounds_v3(p_index))
        return C_CONTINUE_RECURSION;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

      Card_Bounds *p_bounds = p_index->pSub_bounds();
      if (! p_bounds) {
        p_bounds = new Card_Bounds;
        p_index->vAppend(p_bounds);
      }

      char cTblSyn = this->cValue();
      Card_Bound *p_bound = Card_Bound::pCreateFrom_PRED(p_pred_Of_filter, cTblSyn);
      p_bounds->vAppend(p_bound);

      p_pred_Of_filter->vMark1(MARK_UP);

      if (p_bound->bIsEBound())
        p_index->vSet_Choose_index();

      return C_CONTINUE_RECURSION;
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    bool bIgnore = p_pred_root->bPredRecursive(bLambdaFn);
  }
}

/*----------------------------------------------------------------------------*/
/* table projection analysis                                                  */
/*----------------------------------------------------------------------------*/
Stz* Card_Table::szColNm(i8 i_Col_base0)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_TableNm* p_tableNm = pSub_tableNm();

  Table* pTable = Card_Table::pAccess_Table(p_tableNm->szValue());

  StreamDef* pStreamDef = pTable->pStreamDef();

  Column* pColumn = pStreamDef->pGetColumn(i_Col_base0);

  Stz* szColNm = pColumn->szColNm();

  return szColNm;
}

/*----------------------------------------------------------------------------*/
/* convert <table> into either <scan>, <indexJoin> or <keyExists>             */
/*----------------------------------------------------------------------------*/
void Card_Table::vMorph_table_To_EEop(CardIter* pCI)
{
  _static_

  pCI->vPrintBoard(__LINE__);


  // TABLE to:
  // a) SCAN
  if (0 == pCI->G.i_MainCard)  {
    Card_Scan::vMorph_table_To_scan(pCI);
  }
    // b) KEYEXISTS
  else if (pCI->G.p_table->pSub_keyExists()) {
    Card_KeyExists::vMorph_table_To_keyExists(pCI);
  }
    // c) KEYNOTEXISTS
  else if (pCI->G.p_table->pSub_keyNotExists()) {
    Card_KeyNotExists::vMorph_table_To_keyNotExists(pCI);
  }
    // d) CROSSJOIN
  else if (! pCI->G.p_table->pSub_bounds()) { // && bFold && pCI->G.i_MainCard == i_Fold+1) {
    Card_CrossJoin::vMorph_table_To_crossJoin(pCI);
  }
    // e) INDEXJOIN
  else {
    Card_IndexJoin::vMorph_table_To_indexJoin(pCI);
  }

  pCI->G.pDeck->vRenumberRecursive();
  pCI->vPrintBoard(__LINE__);
  vDebugPoint();

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Table::vPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  s32 sName;
  sName = aCK_short[m_eCard];
  if (0 == iDepth)
    sName.vToUpper();

  if (bHasValue())
    sName.vCatSprint(" %c"_Stz, cValue());

  vPrintCard_byCustomNm(pRibbon, iDepth, sName);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Table::bScopeContains_Pred(Card_PRED* p_pred,
                                        Scope* pScope)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI(p_pred);
  while (xCI.G_bEachMain(CK_TERM)) {
    if (! this->bScopeContains_term(xCI.G.p_term, pScope))
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

  Stz* szTerm = p_term->szValue();

  // parameters are considered within scope
  if (SymVal::b_IsCorrParamOrdinal(szTerm))
    return true;

  // literals are considered within scope
  if (! SymVal::b_IsTableColOrdinal(szTerm)) {
    return true;
  }

  // is the table column within scope?
  char cTblSyn_table = this->cValue();
  char cTblSyn_term = SymVal::c_TableColSyn(szTerm);
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

  Table* pTable = Card_Table::pAccess_Table(p_tableNm->szValue());

  this->vMark1(MARK_CHANGED);

  char cTableSyn_actual = *cTableSyn;
  if (pTable->bIsReplicated())
    cTableSyn_actual = StrLib::cToUpper(cTableSyn_actual);

  this->vValueSprint("%c"_Stz,cTableSyn_actual);

  (*cTableSyn)++;
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Table::vSmooth(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_table->Y_vSet(P_Smoothed);

  pCI->G.p_table->vUpdate_projection(pCI);
  pCI->G.pDeck->vRenumberRecursive();
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Table::vSmooth(CardIter* pCI, CarryForward* pCarryForward)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_table->Y_vSet(P_Smoothed);

  RASSERT(pCarryForward->Table.m_bReproject);
  pCI->G.p_table->vUpdate_projection(pCI);
  pCI->G.pDeck->vRenumberRecursive();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
