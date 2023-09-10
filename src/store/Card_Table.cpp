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
#include "Error.hpp"

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
: Card_symbol(CK_TABLE, 2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
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

  if (! pTable) {
    s256 sErrorMsg;
    sErrorMsg.vSprint("table: %s"_Stz,szTableNm);
    Error xError(EC_NO_SUCH_TABLE,sErrorMsg,(Stz*)__FILE__,__LINE__);
    throw xError;
  }

  return pTable;
}

/*----------------------------------------------------------------------------*/
/* get size of a table and its rows                                           */
/*----------------------------------------------------------------------------*/
void Card_Table::vGetSizes(Table* pTable, i8& jTableSize, i8& jRowSize, i8& nRowCount)
{
  _static_

  pTable->vGetGridEstimates(jTableSize, jRowSize, nRowCount);
}

/*----------------------------------------------------------------------------*/
/* affix table column ordinals to all idx cols                                */
/*----------------------------------------------------------------------------*/
void Card_Table::vAffixTableColOrd(CardIter* pCI) {

  _static_

  while (pCI->G_bEachMainWith3Sub(CK_FROM,CK_TABLE,CK_PROJECTION,CK_INDEXLIST)) {

    Kid xKid(pCI->G.p_indexList);
    while (xKid.bEach(CK_INDEX)) {
      pCI->G.p_idxColList = xKid.p_index()->pSub_idxColList();
      pCI->G.p_idxColList->vAffix_tblColOdn(pCI->G.p_projection);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* affix table syntax to all tables                                           */
/*----------------------------------------------------------------------------*/
void Card_Table::vAffixTableSyn_passThru(CardIter* pCI, Symbol* pSymbol_tableSyn)
{

  _static_

  pCI->vBeginEach();
  while (pCI->G_bEachSubOfMain(CK_TABLE))  {
    pCI->G.p_table->vSet_Syn(pCI, pSymbol_tableSyn);
  }
}

/*----------------------------------------------------------------------------*/
/* append index list under all tables                                         */
/*----------------------------------------------------------------------------*/
void Card_Table::vAppendIndexList(CardIter* pCI) {

  _static_

  pCI->vBeginEach();
  while (pCI->G_bEachMainWithSub(CK_FROM, CK_TABLE)) {
    pCI->G.p_tableNm = pCI->G.p_table->pSub_tableNm();
    Table* pTable = Card_Table::pAccess_Table(pCI->G.p_tableNm->szSymbol(SF_NONE));

    pCI->G.p_indexList = Card_IndexList::pDerive(pTable);

    pCI->G_vAppendChild(CK_TABLE,CK_INDEXLIST);
  }
}

/*----------------------------------------------------------------------------*/
/* derive bounds from filter                                                  */
/*----------------------------------------------------------------------------*/
void Card_Table::vDeriveBoundsFromFilter(CardIter* pCI) {

  _static_

  pCI->vBeginEach();
  while (pCI->G_bEachMainWith2Sub(CK_FROM, CK_TABLE, CK_INDEXLIST))
    pCI->G.p_indexList->vDeleteAll_bounds();

  pCI->vBeginEach();
  while (pCI->G_bEachMainWith3Sub(CK_FROM, CK_TABLE, CK_FILTER, CK_INDEXLIST))
    pCI->G.p_table->vDeriveBoundsFrom_filter(pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_OP* Card_Table::pFind_By_tblSyn(CardIter* pCI, char cTblSyn, i8 i_Before)
{
  _static_

  if (C_UNKNOWN == i_Before)
    i_Before = pCI->i_Child();

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card_OP *p_OP = (Card_OP*)pCI->G.pDeck->pGetChild(i_Main);
    if (p_OP->bIsCard(CK_FROM)) {
      Card_Table* p_table_test = p_OP->pSub_table();
      if (cTblSyn == p_table_test->pSymbol()->cSF_char()) {
        return p_OP;
      }
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Table::vFixTablePos(CardIter* pCI)
{
  _static_

  for (Kid xKid(pCI->G.pDeck); xKid.bEach(CK_FROM); /**/) {
    Card_Table* p_table = xKid.pChild()->pSub_table();
    if (p_table->bUsesSqyRsl()) {
      p_table->pSymbol()->Y_vSet(P_FixedPos,(i8)0);
      break;
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Card_Table::nTables(CardIter* pCI)
{
  _static_

  i8 nTables = 0;

  pCI->vBeginEach();
  while (pCI->G_bEachMainWithSub(CK_FROM,CK_TABLE))
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

  pCI->vBeginEach();
  while (pCI->G_bEachMain_NONE()) {
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

  pCI->vBeginEachReverse();
  while (pCI->G_bEachMain_reverse_foldBreak_b(CK_FROM))
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

  char cTblSyn = this->pSymbol()->cSF_char();

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
    p_bounds->vAppendChild(p_bound);
  }
  else {
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* table projection analysis                                                  */
/*----------------------------------------------------------------------------*/
Stz* Card_Table::szColNm(i8 i_Col_base0)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_TableNm* p_tableNm = pSub_tableNm();

  Table* pTable = Card_Table::pAccess_Table(p_tableNm->szSymbol(SF_NONE));

  StreamDef* pStreamDef = pTable->pStreamDef();

  Column* pColumn = pStreamDef->pGetColumn(i_Col_base0);

  Stz* szColNm = pColumn->szColNm();

  return szColNm;
}

/*----------------------------------------------------------------------------*/
/* create bounds under table from filter                                      */
/*----------------------------------------------------------------------------*/
void Card_Table::vDeriveBoundsFrom_filter(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSymbol()->bSame('I'))
    vDebugPoint();


  Card_BoolCalc* p_boolCalc = pCI->G.p_filter->pSub_boolCalc();
  RASSERT(1 == p_boolCalc->nChildCards());
  Card_PRED* p_pred_root = p_boolCalc->pRootPred();

  Kid xKid(pCI->G.p_indexList);
  while (xKid.bEach(CK_INDEX)) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    Card_Index* p_index = xKid.p_index();

    //p_index->vDump(__func__);

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
        p_index->vAppendChild(p_bounds);
      }

      char cTblSyn = this->pSymbol()->cSF_char();
      Card_Bound *p_bound = Card_Bound::pCreateFrom_PRED(p_pred_Of_filter, cTblSyn);
      p_bounds->vAppendChild(p_bound);

      p_pred_Of_filter->vMark1(MARK_UP);

//      if (p_bound->bIsEBound())
//        p_index->vSet_Choose_index();

      return C_CONTINUE_RECURSION;
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    bool bIgnore = p_pred_root->bPredRecursive(bLambdaFn);
    UNUSED(bIgnore);
  }
}

/*----------------------------------------------------------------------------*/
/* does this table contain a sub-query parameter ordinal                      */
/*----------------------------------------------------------------------------*/
bool Card_Table::bUsesSqyRsl(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_IndexList* p_indexList = pSub_indexList())
    return p_indexList->bUsesSqyRsl();

  return false;
}

/*----------------------------------------------------------------------------*/
/* is given symbol a $c.n of this table?                                      */
/*----------------------------------------------------------------------------*/
bool Card_Table::bIsTblColOf(Stz* szTblCol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  char cTbl_this = pSymbol()->cSF_char();
  char cTbl_arg  = *(szTblCol->azChar()+1);

  if (cTbl_this == cTbl_arg)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* convert <table> into either <scan>, <indexJoin> or <keyExists>             */
/*----------------------------------------------------------------------------*/
void Card_Table::vMorph_table_To_EEop(CardIter* pCI)
{
  _static_

  // TABLE to:
  // a) SCAN
  if (0 == pCI->G.i_MainCard)  {
    Card_Scan::vMorph_table_To_scan(pCI);
  }
    // b) KEYEXISTS
  else if (pCI->G.p_from->pSub_keyExists()) {
    Card_KeyExists::vMorph_table_To_keyExists(pCI);
  }
    // c) KEYNOTEXISTS
  else if (pCI->G.p_from->pSub_keyNotExists()) {
    Card_KeyNotExists::vMorph_table_To_keyNotExists(pCI);
  }
    // d) CROSSJOIN
  else if (! pCI->G.p_from->pSub_bounds()) { // && bFold && pCI->G.i_MainCard == i_Fold+1) {
    Card_CrossJoin::vMorph_table_To_crossJoin(pCI);
  }
    // e) INDEXJOIN
  else {
    Card_IndexJoin::vMorph_table_To_indexJoin(pCI);
  }
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

  if (m_xSymbol.bHasSymbol())
    sName.vCatSprint(" %c"_Stz, pSymbol()->cSF_tblSyn());

  vPrintCard_byCustomNm(pRibbon, iDepth, sName);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Table::bScopeContains_Pred(Card_PRED* p_pred,
                                        Scope* pScope)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xKid(p_pred);
  while (xKid.bEach(CK_TERM)) {
    if (! this->bScopeContains_term(xKid.p_term(), pScope))
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

  // parameters are considered within scope
  if (p_term->pSymbol()->bIs_Sqy_Prm())
    return true;

  // literals are considered within scope
  if (!p_term->pSymbol()->bIs_Col_Tc()) {
    return true;
  }

  // is the table column within scope?
  if (pScope->bInScope(p_term->pSymbol()))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* set synonym of table                                                       */
/*----------------------------------------------------------------------------*/
void Card_Table::vSet_Syn(CardIter *pCI, Symbol* pSymbol_tableSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pCI);

  SYM(pSymbol_tableSyn);
  this->vMark1(MARK_CHANGED);

  Card_TableNm* p_tableNm = pSub_tableNm();
  Table* pTable = Card_Table::pAccess_Table(p_tableNm->szSymbol(SF_NONE));
  if (pTable->bIsReplicated())
    SYM(SF_TBL_SYN_rep);

  pSymbol_tableSyn->vInc_Tbl_Syn();
}

/*----------------------------------------------------------------------------*/
/* validate table                                                             */
/*----------------------------------------------------------------------------*/
void Card_Table::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pParent);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
