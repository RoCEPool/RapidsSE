/*============================================================================*/
/* Card_Table.cpp -- Table                                                    */
/*============================================================================*/

#include "Card_Table.hpp"

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
: Card(CK_TABLE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Table::Card_Table(Stz* szTableNm)
    : Card(CK_TABLE,szTableNm)
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* apply stream                                                               */
/*----------------------------------------------------------------------------*/
void Card_Table::vApplyStream(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Table* pTable = Card::pAccess_Table(pCI->G.p_tableNm->sz());
  pCI->G.p_stream = Card_Stream::pDerive_Of_table(pTable);

  vAppend(pCI->G.p_stream);
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
