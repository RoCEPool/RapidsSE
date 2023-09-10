/*============================================================================*/
/* Card_CONJUNCTION.cpp -- Abstract Conjunction                               */
/*============================================================================*/

#include "Card_CONJUNCTION.hpp"
#include "Card_Term.hpp"
#include "Card_OrPred.hpp"
#include "Card_AndPred.hpp"
#include "Card_CmpPred.hpp"
#include "CardIter.hpp"
#include "Card_Table.hpp"
#include "Card_Deck.hpp"
#include "Scope.hpp"
/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CONJUNCTION::Card_CONJUNCTION(eCK eCard, i8 iFixedDepth)
: Card_symbol(eCard, iFixedDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CONJUNCTION::~Card_CONJUNCTION(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is card a PRED ?                                                           */
/*----------------------------------------------------------------------------*/
bool Card_CONJUNCTION::bIsPRED(Card* p_card)
{
  _static_;

  switch (p_card->eCard()){
    case CK_ANDPRED:
    case CK_CMPPRED:
    case CK_NOTPRED:
    case CK_ORPRED :
      return true;
    default:
      return false;
  }
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CONJUNCTION::bContains_callToSubQuery(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  //this->vDump();

  Kid xKid(this);
  while (xKid.bEach(CK_TERM))
    if (pCI->m_pBench->bIsSubQueryNm(xKid.p_term()->szSymbol(SF_NONE)))
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CONJUNCTION::bDescend(std::function<bool(Card_CONJUNCTION* )> bfLambdaFn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach()) {
    if (! Card_CONJUNCTION::bIsPRED(xKid.pChild()))
      continue;

    bool bContinue = ((Card_CONJUNCTION*)xKid.pChild())->bDescend(bfLambdaFn);
    if (! bContinue)
      return C_TERMINATE_RECURSION;
  }

  return C_CONTINUE_RECURSION;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_CONJUNCTION::vEnsureTypesFor_stmOdn(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  eCK eCard = this->eCard();

  switch (eCard) {

    case CK_ANDPRED: _pCard_AndPred(this)->vEnsureTypesFor_stmOdn(pCI); break;
    case CK_CMPPRED: _pCard_CmpPred(this)->vEnsureTypesFor_stmOdn(pCI); break;
    case CK_NOTPRED: _pCard_NotPred(this)->vEnsureTypesFor_stmOdn(pCI); break;
    case CK_ORPRED:  _pCard_OrPred(this)->vEnsureTypesFor_stmOdn(pCI);  break;

    case CK_TERM:    _pCard_Term(this)->vEnsureTypeFor_stmOdn(pCI);     break;

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* attempt to get board name from one of the terms of this predicate          */
/*----------------------------------------------------------------------------*/
Stz* Card_CONJUNCTION::szGetBoardNmInTerm(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Term* p_term = pGetTerm_boardNm(pCI))
    return p_term->szSymbol(SF_NONE);

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a term                                                                 */
/*----------------------------------------------------------------------------*/
Card_Term* Card_CONJUNCTION::pGetFirstTerm_tblColOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_TERM))
    if (xKid.p_term()->bIsColOdn())
      return xKid.p_term();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a term                                                                 */
/*----------------------------------------------------------------------------*/
Card_Term* Card_CONJUNCTION::pGetTerm(i8 i_Term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (i_Term >= this->nChildCards())
    return nullptr;

  Card_Term* p_term = _pCard_Term(pGetChild(i_Term));

  return p_term;
}


/*----------------------------------------------------------------------------*/
/* attempt to get term which is a board name from this predicate              */
/*----------------------------------------------------------------------------*/
Card_Term* Card_CONJUNCTION::pGetTerm_boardNm(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_TERM)) {

    Stz* szValue_term = xKid.p_term()->szSymbol(SF_NONE);
    if (pCI->G_bIsBoardNm(szValue_term))
      return xKid.p_term();
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* is this predicate a bool fn?                                               */
/*----------------------------------------------------------------------------*/
bool Card_CONJUNCTION::bIsBoolFn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSymbol()->bIs_Bol_Fn())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a correlation predicate? (a predicate in a sub-query)              */
/*----------------------------------------------------------------------------*/
bool Card_CONJUNCTION::bIsCorrPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (!pSymbol()->bSame("=="_Stz))
    return false;

  i8 nChildren = nChildCards();
  if (2 != nChildren)
    return false;

  Card_Term* p_term_0 = pGetTerm(0);
  Card_Term* p_term_1 = pGetTerm(1);

  if ((p_term_0->bIsPrmOdn() && (p_term_1->bIsStmOdn() || p_term_1->bIsColOdn())) ||
      (p_term_1->bIsPrmOdn() && (p_term_0->bIsStmOdn() || p_term_0->bIsColOdn())))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this predicate a bounds for given index                                 */
/*----------------------------------------------------------------------------*/
bool Card_CONJUNCTION::bIsIndexBounds_v3(Card_Index* p_index)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! p_index)
    return false;

  i8 nChildren = nChildCards();
  if (2 != nChildren)
    return false;

  Card_Term* pTerm_0 = _pCard_Term(this->pGetChild(0));
  Card_Term* pTerm_1 = _pCard_Term(this->pGetChild(1));

  if (pTerm_0->pSymbol()->bIs_Col_Tc()) {
    // + term_0 is $c.n

    if (pTerm_1->pSymbol()->bIs_Col_Tc()) {
      // + both are term:$c.n

      // ? is either term_0:$c.n or term_1:$c.n an index column
      if (p_index->bTblColOdn_IsKeyIn_index(pTerm_0->szSymbol(SF_NONE)) ||
          p_index->bTblColOdn_IsKeyIn_index(pTerm_1->szSymbol(SF_NONE)) )
        return true;

      return false;
    }
    else {
      // + only term_0 is $c.n

      // ? is term_0:$c.n an index column
      if (p_index->bTblColOdn_IsKeyIn_index(pTerm_0->szSymbol(SF_NONE)))
        return true;

      return false;
    }
  }
  else
  if (pTerm_1->pSymbol()->bIs_Col_Tc()) {
    // + only term_1 is $c.n

    // ? is term_0:$c.n an index column
    if (p_index->bTblColOdn_IsKeyIn_index(pTerm_1->szSymbol(SF_NONE)) )
      return true;

    return false;
  }

  // + neither term_0 nor term_1 is $c.n
  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a ootential join predicate?                                        */
/*----------------------------------------------------------------------------*/
bool Card_CONJUNCTION::bIsJoinPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! pSymbol()->bSame("=="_Stz))
    return false;

  i8 nChildren = nChildCards();
  if (2 != nChildren)
    return false;

  Card_Term* p_term_0 = pGetTerm(0);
  Card_Term* p_term_1 = pGetTerm(1);

  if ((p_term_0->bIsStmOdn() || p_term_0->bIsColOdn()) &&
      (p_term_1->bIsStmOdn() || p_term_1->bIsColOdn()))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this predicate a relational operator?                                   */
/*----------------------------------------------------------------------------*/
bool Card_CONJUNCTION::bIsRelnOp(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSymbol()->bIs_Rln_Op())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this predicate a subquery operator?                                     */
/*----------------------------------------------------------------------------*/
bool Card_CONJUNCTION::bIsSubQueryOp(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSymbol()->bIs_Sqy_Op())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this predicate a possible join predicate                                */
/*----------------------------------------------------------------------------*/
bool Card_CONJUNCTION::bMaybeFilterPred(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (RO_NONE == pSymbol()->eEvalRelnOp())
    return false;

  if (bContains_callToSubQuery(pCI))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this predicate a possible join predicate                                */
/*----------------------------------------------------------------------------*/
bool Card_CONJUNCTION::bMaybeJoinPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (!pSymbol()->bSame("=="_Stz))
    return false;

  i8 nChildren = nChildCards();
  if (2 != nChildren)
    return false;

  Card_Term* pTerm_0 = pGetTerm(0);
  Card_Term* pTerm_1 = pGetTerm(1);

  if (pTerm_0->pSymbol()->bIs_Col_Tc() &&
      pTerm_1->pSymbol()->bIs_Col_Tc())
    // + term_0 and term_1 are $c.n
    return true;

  // + neither term_0 nor term_1 is $c.n
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_CONJUNCTION::vMorph_SqlOp_To_Eq(CardIter* pCI, Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pSymbol()->bSame("exists"_Stz)) {

   // pCI->vPrintBoard(__LINE__);

    SYM(SF_RLN_OP,"=="_Stz);

    Card_Term* p_term_one = _pCard_Term(pGetChild(1));
    ASSERT(pCI->m_pBench->bIsSubQueryNm(p_term_one->szSymbol(SF_NONE))); //temp

    Symbol x_Symbol_stmCol1;
    if (p_term) {
      x_Symbol_stmCol1.vSym(p_term->pSymbol());
      p_term_one->pSymbol()->vSym(&x_Symbol_stmCol1);
    }
    else {
      //x_Symbol_stmCol1.vSet_Col_No(1);
      p_term_one->SYM(SF_COL_NO,(i8)1);
    }

    Card_Expr* p_expr_found =
        pCI->Prior.m_pProjection->pGetExpr(0);

    p_term_one->pSymbol()->Y_vColType(p_expr_found->szP_Type());

   // pCI->vPrintBoard(__LINE__);
    return;
  }

  if (pSymbol()->bSame("in"_Stz)) {

    SYM(SF_RLN_OP,"=="_Stz);

    Card_Term* p_term_popped = _pCard_Term(pPopChild());
    ASSERT(pCI->m_pBench->bIsSubQueryNm(p_term_popped->szSymbol(SF_NONE))); //temp
    DELETE_CARD(p_term_popped);

    Card_Term* p_term_head = _pCard_Term(pGetChild(0));
    ASSERT(p_term_head->pSymbol()->bSame("#1"_Stz)); //temp

    if (p_term) {
      Symbol x_Symbol_stmCol1;
      x_Symbol_stmCol1.vSym(p_term->pSymbol());
      p_term_head->pSymbol()->vSym(&x_Symbol_stmCol1);
    }
    else {
      Symbol x_Symbol_stmCol1(SF_COL_NO, 1);
      p_term_head->pSymbol()->vSym(&x_Symbol_stmCol1);
    }

    Card_Expr* p_expr_found =
        pCI->Prior.m_pProjection->pGetExpr(0);

    p_term_head->pSymbol()->Y_vColType(p_expr_found->szP_Type());

   // pCI->vPrintBoard(__LINE__);

    return;
  }

  if (pSymbol()->bSame("notIn"_Stz)) {

    SYM(SF_RLN_OP,"=="_Stz);

    Card_Term* p_term_popped = _pCard_Term(pPopChild());
    ASSERT(pCI->m_pBench->bIsSubQueryNm(p_term_popped->szSymbol(SF_NONE))); //temp
    DELETE_CARD(p_term_popped);

    Card_Term* p_term_head = _pCard_Term(pGetChild(0));
    ASSERT(p_term_head->pSymbol()->bSame("#1"_Stz)); //temp

    Symbol x_Symbol_stmCol1(SF_COL_NO,1);
    if (p_term)
      x_Symbol_stmCol1.vSymPpy(p_term->pSymbol());
    p_term_head->pSymbol()->vSymPpy(&x_Symbol_stmCol1);
    return;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the E_eTerm_Bound of given index               */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Card_CONJUNCTION::pPredFindBound(Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eCK eCard = this->eCard();

  switch (eCard) {

    case CK_ORPRED:
      return _pCard_OrPred(this)->pOrPredFindBound(pIndex, eBound);

    case CK_ANDPRED:
      return _pCard_AndPred(this)->pAndPredFindBound(pIndex, eBound);

    case CK_CMPPRED:
      return _pCard_CmpPred(this)->pCmpPredFindBound(pIndex, eBound);

    default:
      vTrace("pPredFindBound: unexpected eCard %lld %s\n"_Stz
          ,(i8)eCard
          ,aCK[eCard]);
      RASSERT(0);
      break;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does predicate have some bound wrt given index?                            */
/*----------------------------------------------------------------------------*/
bool Card_CONJUNCTION::bPredHasBounds(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eCK eCard = this->eCard();

  switch (eCard) {

    case CK_ORPRED:
      return _pCard_OrPred(this)->bOrPredHasBounds(pIndex);

    case CK_ANDPRED:
      return _pCard_AndPred(this)->bAndPredHasBounds(pIndex);

    case CK_CMPPRED:
      return _pCard_CmpPred(this)->bCmpPredHasBounds(pIndex);

    default:
      vTrace("bPredHasBounds: unexpected eCard %lld %s\n"_Stz
          ,(i8)eCard
          ,aCK[eCard]);
      RASSERT(0);
      break;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* visit all sub-cards of pred and apply lambda function                      */
/*----------------------------------------------------------------------------*/
bool Card_CONJUNCTION::bPredRecursive(std::function<bool(Card_CONJUNCTION* p_pred, bool bUpwards)> bfApply)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool bContinue;

  eCK eCard = this->eCard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // only call term once

  if (CK_TERM == eCard) {
    bContinue = bfApply(this,false);
    return bContinue;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // apply pred function for downwards
  bContinue = bfApply(this,false);
  if (! bContinue)
    return false;

  // descend into children
  i8 nSubCards = nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pGetChild(i_Child);
    bContinue = static_cast<Card_CONJUNCTION *>(pSubCard)->bPredRecursive(bfApply);
    if (! bContinue)
      return false;
  }

  // apply pred function for upwards
  bContinue = bfApply(this,true);
  return bContinue;
}

/*----------------------------------------------------------------------------*/
/* inspect <PRED>                                                             */
/*----------------------------------------------------------------------------*/
Symbol* Card_CONJUNCTION::pTable_WhichTableFor_Pred(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Scope* pScope = new Scope;

  i8 i_Main = 0;

  i8 i_Fold = C_UNKNOWN;
  for (i8 i_Test = 0 ; i_Test < pCI->G.pDeck->nChildCards() ; i_Test++) {
    Card *pTestCard = pCI->G.pDeck->pGetChild(i_Test);

    if (pTestCard->bIsCard(CK_FOLD))
      i_Fold = i_Test;
  }

  if (C_UNKNOWN != i_Fold)
    i_Main = i_Fold + 1;

  for (/* none */ ; /* none */ ; i_Main++) {
    Card *pMainCard = pCI->G.pDeck->pGetChild(i_Main);

    if (CK_WHERE == pMainCard->eCard()) {
      delete pScope;
      return nullptr;
    }

    eCK eMainCard = pMainCard->eCard();

    if (CK_FROM == eMainCard) {
      Card_Table* p_table = pMainCard->pSub_table();
      Symbol* pSymbol_table = p_table->pSymbol();
      pScope->vInScope(pSymbol_table,true);
      if (p_table->bScopeContains_Pred(this, pScope)) {
        delete pScope;
        return pSymbol_table;
      }
      continue;
    }

    // skip over any other card??
    //vTrace("bMovePredLeft skipping mainCard %s\n"_Stz,aCK[eMainCard]);
  }

  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_CONJUNCTION::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);
  UNUSED(iDepth);
  UNUSED(pParent);

  eCK eCard = this->eCard();

  switch (eCard) {

    case CK_ANDPRED: _pCard_AndPred(this)->vValidate(pCI,iDepth+1,this); break;
    case CK_CMPPRED: _pCard_CmpPred(this)->vValidate(pCI,iDepth+1,this); break;
    case CK_NOTPRED: _pCard_NotPred(this)->vValidate(pCI,iDepth+1,this); break;
    case CK_ORPRED:  _pCard_OrPred(this)->vValidate(pCI,iDepth+1,this);  break;

    default:
     // pCI->vPrintBoard(__LINE__,true);
      vDump(__func__);
      vValidationFailed("<<replace>> %p"_Stz,this);
  }

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
