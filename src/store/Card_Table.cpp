/*============================================================================*/
/* Card_Table.cpp -- Table                                                    */
/*============================================================================*/

#include "Card_Table.hpp"

#include "Card_As.hpp"
#include "Card_IdxColList.hpp"
#include "Card_IndexList.hpp"
#include "Card_Filter.hpp"
#include "Card_BoolPred.hpp"
#include "CardIter.hpp"
#include "Card_Index.hpp"
#include "Card_RlnConj.hpp"
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

__pCastFunction_Card(STORE, Card_Table, CK_s_TABLE)
__aNewFunction(Card_Table, CN_CARD)
__vDeleteFunction(Card_Table, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Table::Card_Table(void)
: Card_symbol(CK_s_TABLE, 2)
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

  while (pCI->bEachOp(CK_o_FROM, CK_s_TABLE, CK_s_INJECTABLE, CK_t_INDEXLIST)) {

    Kid xKid(pCI->G.p_indexList);
    while (xKid.bEach(CK_INDEX)) {
      pCI->G.p_idxColList = xKid.p_index()->pSub_idxColList();
      pCI->G.p_idxColList->vAffix_tblCol(pCI);
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
  while (pCI->bEachSubOfOp(CK_s_TABLE))  {
    pCI->G.p_table->vSet_Syn(pSymbol_tableSyn);
  }
}

/*----------------------------------------------------------------------------*/
/* append index list under all tables                                         */
/*----------------------------------------------------------------------------*/
void Card_Table::vAppendIndexList(CardIter* pCI) {

  _static_

  pCI->vBeginEach();
  while (pCI->bEachOp(CK_o_FROM, CK_s_TABLE)) {
    pCI->G.p_tableNm = pCI->G.p_table->pSub_tableNm();
    Table* pTable = Card_Table::pAccess_Table(pCI->G.p_tableNm->szFacet(SF_TBL_NM));

    pCI->G.p_indexList = Card_IndexList::pDerive(pTable);

    pCI->vAppendChild(CK_s_TABLE, pCI->G.p_indexList);
  }
}

/*----------------------------------------------------------------------------*/
/* derive bounds from filter                                                  */
/*----------------------------------------------------------------------------*/
void Card_Table::vDeriveBoundsFromFilter(CardIter* pCI) {

  _static_

  pCI->vBeginEach();
  while (pCI->bEachOp(CK_o_FROM, CK_s_TABLE, CK_t_INDEXLIST))
    pCI->G.p_indexList->vDeleteAll_bounds();

  pCI->vBeginEach();
  while (pCI->bEachOp(CK_o_FROM, CK_s_TABLE, CK_o_FILTER, CK_t_INDEXLIST))
    pCI->G.p_table->vDerive_bounds_From_filter(pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Table::vFixTablePos(CardIter* pCI)
{
  _static_

  for (Kid xKid(pCI->G.pDeck); xKid.bEach(CK_o_FROM); /**/) {
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
  while (pCI->bEachOp(CK_o_FROM, CK_s_TABLE))
    nTables++;

  return nTables;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Card_Table::nTables_reverse_foldBreak(CardIter* pCI)
{
  _static_

  i8 nTables = 0;

  pCI->vBeginEach_reverse();
  while (pCI->bEachOp_reverse_foldBreak(CK_o_FROM))
    nTables++;

  return nTables;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Table::vBounds_AddConj(Card_Index* p_index,
                                 Card_Bounds* p_bounds,
                                 Card_CONJ *p_conj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  char cTblSyn = this->pSymbol()->cTblSyn();

  Card_Bound* p_bound;
  Card_CONJ* p_childPRED;
  if (p_conj->bIsCard(CK_ANDCONJ)) {
    p_childPRED = (Card_CONJ*)p_conj->pGetChild(0);
    this->vBounds_AddConj(p_index, p_bounds, p_childPRED);
    p_childPRED = (Card_CONJ*)p_conj->pGetChild(1);
    this->vBounds_AddConj(p_index, p_bounds, p_childPRED);
  }
  else
  if (p_conj->bIsCard(CK_RLNCONJ)) {
    p_bound = Card_Bound::pCreateFrom_rlnConj(_pCard_RlnConj(p_conj), cTblSyn);
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

  Table* pTable = Card_Table::pAccess_Table(p_tableNm->szFacet(SF_TBL_NM));

  StreamDef* pStreamDef = pTable->pStreamDef();

  Column* pColumn = pStreamDef->pGetColumn(i_Col_base0);

  Stz* szColNm = pColumn->szColNm();

  return szColNm;
}

/*----------------------------------------------------------------------------*/
/* find chosen index                                                          */
/*----------------------------------------------------------------------------*/
Card_Index* Card_Table::pFindChosenIndex(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_IndexList* p_indexList = pSub_indexList())
    return p_indexList->pFindChosenIndex();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* create bounds under table from filter                                      */
/*----------------------------------------------------------------------------*/
void Card_Table::vDerive_bounds_From_filter(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_BoolPred* p_boolPred = pCI->G.p_filter->pSub_boolPred();
  RASSERT(1 == p_boolPred->nChildCards());
  Card_CONJ* p_conj_root = p_boolPred->pRoot_conj();

  Kid xKid(pCI->G.p_indexList);
  while (xKid.bEach(CK_INDEX)) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    Card_Index* p_index = xKid.p_index();

    //p_index->vDump(__func__);

    auto bLambdaFn = [this, p_index](Card_CONJ *p_conj_Of_filter, bool bUpwards)
    {
      if (bUpwards)
        return C_CONTINUE_RECURSION;

      if (! p_conj_Of_filter->bIsCard(CK_RLNCONJ))
        return C_CONTINUE_RECURSION;

      Card_RlnConj* p_rlnConj_filter = _pCard_RlnConj(p_conj_Of_filter);
      if (!p_rlnConj_filter->bIsIndexBound(p_index))
        return C_CONTINUE_RECURSION;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

      Card_Bounds *p_bounds = p_index->pSub_bounds();
      if (! p_bounds) {
        p_bounds = new Card_Bounds;
        p_index->vAppendChild(p_bounds);
      }

      char cTblSyn = this->pSymbol()->cTblSyn();
      Card_Bound *p_bound = Card_Bound::pCreateFrom_rlnConj(p_rlnConj_filter, cTblSyn);
      p_bounds->vAppendChild(p_bound);

      p_rlnConj_filter->vMark1(MARK_UP);

      return C_CONTINUE_RECURSION;
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    bool bIgnore = p_conj_root->bLambdaWalk_conj(bLambdaFn);
    UNUSED(bIgnore);
  }
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* Card_Table::pGet_Table(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Table* pTable = reinterpret_cast<Table*>(pSymbol()->Y_pGet(P_Pointer));

  return pTable;
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
/* convert <table> into either <scan>, <indexJoin> or <keyExists>             */
/*----------------------------------------------------------------------------*/
void Card_Table::vMorph_table_To_EEop(CardIter* pCI)
{
  _static_

  // TABLE to:
  // a) SCAN
  if (0 == pCI->G.i_Op)  {
    Card_Scan::vMorph_table_To_scan(pCI);
  }
    // b) KEYEXISTS
  else if (pCI->G.p_from->pSub_keyExists()) {
    Card_KeyExists::vMorph_from_To_keyExists(pCI);
  }
    // c) KEYNOTEXISTS
  else if (pCI->G.p_from->pSub_keyNotExists()) {
    Card_KeyNotExists::vMorph_from_To_keyNotExists(pCI);
  }
    // d) CROSSJOIN
  else if (! pCI->G.p_from->pSub_bounds()) { // && bFold && pCI->G.i_Op == i_Fold+1) {
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

  s256 sName;
  sName = aCK_short[m_eCard];
  if (0 == iDepth)
    sName.vToUpper();

  if (m_xSymbol.bHasFacets())
    sName.vCatSprint(" %s"_Stz, pSymbol()->szFacet(SF_TBL_SYN));

  vPrintCard_byCustomNm(pRibbon, iDepth, sName);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Table::bScopeContains_Conj(Card_CONJ* p_conj,
                                     Scope* pScope)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xKid(p_conj);
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
  if (p_term->pSymbol()->bHasFacet(SF_SQY_PRM))
    return true;

  // literals are considered within scope
  if (!p_term->pSymbol()->bHasFacet(SF_TBL_COL)) {
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
bool Card_Table::bSame(Symbol* pSymbol_tableSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSymbol()->cTblSyn() == pSymbol_tableSyn->cTblSyn())
    return true;
  return false;
}

/*----------------------------------------------------------------------------*/
/* set synonym of table                                                       */
/*----------------------------------------------------------------------------*/
void Card_Table::vSet_Syn(Symbol* pSymbol_tableSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  SYM(pSymbol_tableSyn);
  this->vMark1(MARK_CHANGED);

  Card_TableNm* p_tableNm = pSub_tableNm();
  Table* pTable = Card_Table::pAccess_Table(p_tableNm->szFacet(SF_TBL_NM));
  if (pTable->bIsReplicated())
    m_xSymbol.vApplyFacet(SF_TBL_SYN_rep);

  pSymbol_tableSyn->vInc_Tbl_Syn();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Table::vShortForm(String* pString_m, String* pString_b)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Stz* szSep = ""_Stz;
  if (Card_TableNm* p_tableNm = pSub_tableNm()) {
    pString_m->vCatSprint("%c.%s"_Stz,
                          pSymbol()->bHasFacets() ? pSymbol()->cTblSyn() : ' ',
                          p_tableNm->szFacet(SF_TBL_NM));
    szSep = ","_Stz;
  }

  if (Card_Index* p_index = pFindChosenIndex()) {
    if (Card_IndexNm* p_indexNm = p_index->pSub_indexNm())
      pString_m->vCatSprint("%s%s"_Stz, szSep, p_indexNm->szFacet(SF_IDX_NM));
    if (Card_Bounds* p_bounds = p_index->pSub_bounds())
      p_bounds->vShortForm(pString_b);
  }


}

/*----------------------------------------------------------------------------*/
/* validate table                                                             */
/*----------------------------------------------------------------------------*/
void Card_Table::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  (void)Card::eValidate_parentEither(pCI,
                                     CK_o_CROSSJOIN,
                                     CK_o_FROM,
                                     CK_o_INDEXJOIN,
                                     CK_o_INDEXSCAN,
                                     CK_o_INFLATE,
                                     CK_o_KEYEXISTS,
                                     CK_o_KEYNOTEXISTS,
                                     CK_o_SCAN,
                                     CK_NONE);

  RASSERT(pSymbol()->bHasFacet(SF_TBL_SYN)
          ||
          pSymbol()->bHasFacet(SF_TBL_SYN_rep));

  vValidate_mustHave(CK_t_TABLENM,CK_NONE);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
