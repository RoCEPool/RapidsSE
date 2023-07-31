/*============================================================================*/
/* Card_PRED.cpp -- Abstract Predicate                                        */
/*============================================================================*/

#include "Card_PRED.hpp"
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
Card_PRED::Card_PRED(eCK eCard)
: Card(eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_PRED::Card_PRED(eCK eCard, Stz* szValue)
    : Card(eCard,szValue, SF_OTHER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_PRED::~Card_PRED(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is card a PRED ?                                                           */
/*----------------------------------------------------------------------------*/
bool Card_PRED::bIsPRED(Card* p_card)
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
bool Card_PRED::bContains_callToSubQuery(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  //this->vDump();

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_TERM))
    if (pCI->m_pBench->bIsSubQueryNm(xCI.G.p_term->szSymbol()))
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_PRED::bDescend(std::function<bool(Card_PRED* )> bfLambdaFn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain()) {
    if (! Card_PRED::bIsPRED(xCI.G.p_MainCard))
      continue;

    bool bContinue = ((Card_PRED*)xCI.G.p_MainCard)->bDescend(bfLambdaFn);
    if (! bContinue)
      return C_TERMINATE_RECURSION;
  }

  return C_CONTINUE_RECURSION;
}

/*----------------------------------------------------------------------------*/
/* attempt to get board name from one of the terms of this predicate          */
/*----------------------------------------------------------------------------*/
Stz* Card_PRED::szGetBoardNmInTerm(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Term* p_term = pGetTerm_boardNm(pCI))
    return p_term->szSymbol();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a term                                                                 */
/*----------------------------------------------------------------------------*/
Card_Term* Card_PRED::pGetFirstTerm_tblColOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_TERM))
    if (xCI.G.p_term->bIsColOdn())
      return xCI.G.p_term;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a term                                                                 */
/*----------------------------------------------------------------------------*/
Card_Term* Card_PRED::pGetTerm(i8 i_Term)
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
Card_Term* Card_PRED::pGetTerm_boardNm(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_TERM)) {

    Stz* szValue_term = xCI.G.p_term->szSymbol();
    if (pCI->G_bIsBoardNm(szValue_term))
      return xCI.G.p_term;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* is this predicate a bool fn?                                               */
/*----------------------------------------------------------------------------*/
bool Card_PRED::bIsBoolFn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSymbol()->bIsBoolFn())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a correlation predicate? (a predicate in a sub-query)              */
/*----------------------------------------------------------------------------*/
bool Card_PRED::bIsCorrPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (!bSameSymbol("=="_Stz))
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
bool Card_PRED::bIsIndexBounds_v3(Card_Index* p_index)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! p_index)
    return false;

  i8 nChildren = nChildCards();
  if (2 != nChildren)
    return false;

  Card_Term* pTerm_0 = _pCard_Term(this->pGetChild(0));
  Card_Term* pTerm_1 = _pCard_Term(this->pGetChild(1));

  if (pTerm_0->pSymbol()->bIsColumnOrdinal()) {
    // + term_0 is $c.n

    if (pTerm_1->pSymbol()->bIsColumnOrdinal()) {
      // + both are term:$c.n

      // ? is either term_0:$c.n or term_1:$c.n an index column
      if (p_index->bTblColOdn_IsKeyIn_index(pTerm_0->szSymbol()) ||
          p_index->bTblColOdn_IsKeyIn_index(pTerm_1->szSymbol()) )
        return true;

      return false;
    }
    else {
      // + only term_0 is $c.n

      // ? is term_0:$c.n an index column
      if (p_index->bTblColOdn_IsKeyIn_index(pTerm_0->szSymbol()))
        return true;

      return false;
    }
  }
  else
  if (pTerm_1->pSymbol()->bIsColumnOrdinal()) {
    // + only term_1 is $c.n

    // ? is term_0:$c.n an index column
    if (p_index->bTblColOdn_IsKeyIn_index(pTerm_1->szSymbol()) )
      return true;

    return false;
  }

  // + neither term_0 nor term_1 is $c.n
  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a ootential join predicate?                                        */
/*----------------------------------------------------------------------------*/
bool Card_PRED::bIsJoinPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! bSameSymbol("=="_Stz))
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
bool Card_PRED::bIsRelnOp(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSymbol()->bIsRelnOp())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this predicate a subquery operator?                                     */
/*----------------------------------------------------------------------------*/
bool Card_PRED::bIsSubQueryOp(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSymbol()->bIsSubQueryOp())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this predicate a possible join predicate                                */
/*----------------------------------------------------------------------------*/
bool Card_PRED::bMaybeFilterPred(CardIter* pCI)
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
bool Card_PRED::bMaybeJoinPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (!bSameSymbol("=="_Stz))
    return false;

  i8 nChildren = nChildCards();
  if (2 != nChildren)
    return false;

  Card_Term* pTerm_0 = pGetTerm(0);
  Card_Term* pTerm_1 = pGetTerm(1);

  if (pTerm_0->pSymbol()->bIsColumnOrdinal() &&
      pTerm_1->pSymbol()->bIsColumnOrdinal())
    // + term_0 and term_1 are $c.n
    return true;

  // + neither term_0 nor term_1 is $c.n
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_PRED::vMorph_SqlOp_To_Eq(CardIter* pCI, Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bSameSymbol("exists"_Stz)) {

    pCI->vPrintBoard(__LINE__);

    vSymbol("=="_Stz);

    Card_Term* p_term_one = _pCard_Term(pGetChild(1));
    ASSERT(pCI->m_pBench->bIsSubQueryNm(p_term_one->szSymbol())); //temp

    Symbol x_Symbol_stmCol1;
    if (p_term)
      x_Symbol_stmCol1.vSet(p_term->pSymbol());
    else
      x_Symbol_stmCol1.vSetStreamOrdinal(1);
    p_term_one->pSymbol()->vSet(&x_Symbol_stmCol1);

    Card_Expr* p_expr_found =
        pCI->Prior.m_pProjection->pGetExpr(0);

    p_term_one->pSymbol()->Y_vSet(P_Type, p_expr_found->szP_Type());

    pCI->vPrintBoard(__LINE__);
    return;
  }

  if (bSameSymbol("in"_Stz)) {

    vSymbol("=="_Stz);

    Card_Term* p_term_popped = _pCard_Term(pPopChild());
    ASSERT(pCI->m_pBench->bIsSubQueryNm(p_term_popped->szSymbol())); //temp
    DELETE_CARD(p_term_popped);

    Card_Term* p_term_head = _pCard_Term(pGetChild(0));
    ASSERT(p_term_head->bSameSymbol("#1"_Stz)); //temp

    Symbol x_Symbol_stmCol1;
    if (p_term)
      x_Symbol_stmCol1.vSet(p_term->pSymbol());
    else
      x_Symbol_stmCol1.vSetStreamOrdinal(1);
    p_term_head->pSymbol()->vSet(&x_Symbol_stmCol1);

    Card_Expr* p_expr_found =
        pCI->Prior.m_pProjection->pGetExpr(0);

    p_term_head->pSymbol()->Y_vSet(P_Type, p_expr_found->szP_Type());

    pCI->vPrintBoard(__LINE__);

    return;
  }

  if (bSameSymbol("notIn"_Stz)) {

    vSymbol("=="_Stz);

    Card_Term* p_term_popped = _pCard_Term(pPopChild());
    ASSERT(pCI->m_pBench->bIsSubQueryNm(p_term_popped->szSymbol())); //temp
    DELETE_CARD(p_term_popped);

    Card_Term* p_term_head = _pCard_Term(pGetChild(0));
    ASSERT(p_term_head->bSameSymbol("#1"_Stz)); //temp

    Symbol x_Symbol_stmCol1;
    x_Symbol_stmCol1.vSetStreamOrdinal(1);
    if (p_term)
      x_Symbol_stmCol1.m_sSymbol = p_term->szSymbol();
    else
      x_Symbol_stmCol1.vSetStreamOrdinal(1);
    p_term_head->pSymbol()->vSet(&x_Symbol_stmCol1);
    return;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the E_eTerm_Bound of given index               */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Card_PRED::pPredFindBound(Index* pIndex, ePB eBound)
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
bool Card_PRED::bPredHasBounds(Index* pIndex)
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
bool Card_PRED::bPredRecursive(std::function<bool(Card_PRED* p_pred, bool bUpwards)> bfApply)
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
    bContinue = static_cast<Card_PRED *>(pSubCard)->bPredRecursive(bfApply);
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
char Card_PRED::cTable_WhichTableFor_Pred(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Deck* p_deck = pCI->G.pDeck;

  Scope* pScope = new Scope;

  i8 i_Main = 0;

  i8 i_Fold = C_UNKNOWN;
  for (i8 i_Test = 0 ; i_Test < p_deck->nChildCards() ; i_Test++) {
    Card *pTestCard = p_deck->pGetChild(i_Test);

    if (pTestCard->bIsCard(CK_FOLD))
      i_Fold = i_Test;
  }

  if (C_UNKNOWN != i_Fold)
    i_Main = i_Fold + 1;

  for (/* none */ ; /* none */ ; i_Main++) {
    Card *pMainCard = p_deck->pGetChild(i_Main);

    if (CK_WHERE == pMainCard->eCard()) {
      delete pScope;
      return C_UNKNOWN;
    }

    eCK eMainCard = pMainCard->eCard();

    if (CK_TABLE == eMainCard) {
      Card_Table* p_table = _pCard_Table(pMainCard);
      char cTblSyn = p_table->cSymbol();
      pScope->vInScope(cTblSyn,true);
      if (p_table->bScopeContains_Pred(this, pScope)) {
        char cTblSyn = p_table->cSymbol();
        delete pScope;
        return cTblSyn;
      }
      continue;
    }

    // skip over any other card??
    //vTrace("bMovePredLeft skipping mainCard %s\n"_Stz,aCK[eMainCard]);
  }

  NEVER_REACH;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
