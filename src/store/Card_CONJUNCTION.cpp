/*============================================================================*/
/* Card_CONJUNCTION.cpp -- Abstract Conjunction                               */
/*============================================================================*/

#include "Card_CONJUNCTION.hpp"
#include "Card_Term.hpp"
#include "Card_OrConj.hpp"
#include "Card_AndConj.hpp"
#include "Card_RlnConj.hpp"
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
: Card_CONJSUBROOT(eCard, iFixedDepth)
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
bool Card_CONJUNCTION::bIsConj(Card* p_card)
{
  _static_;

  switch (p_card->eCard()){
    case CK_ANDCONJ:
    case CK_NOTCONJ:
    case CK_ORCONJ :
    case CK_RLNCONJ:
      return true;
    default:
      return false;
  }
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* does predicate have some bound wrt given index?                            */
/*----------------------------------------------------------------------------*/
bool Card_CONJUNCTION::bConjHasBounds(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eCK eCard = this->eCard();

  switch (eCard) {

    case CK_ANDCONJ: return _pCard_AndConj(this)->bAndPredHasBounds(pIndex);

    case CK_ORCONJ:  return _pCard_OrConj(this)->bOrPredHasBounds(pIndex);

    case CK_RLNCONJ: return _pCard_RlnConj(this)->bCmpPredHasBounds(pIndex);

    default:
      vTrace("bConjHasBounds: unexpected eCard %lld %s\n"_Stz
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
bool Card_CONJUNCTION::bConjRecursive(std::function<bool(Card_CONJUNCTION* p_conj, bool bUpwards)> bfApply)
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
    bContinue = static_cast<Card_CONJUNCTION *>(pSubCard)->bConjRecursive(bfApply);
    if (! bContinue)
      return false;
  }

  // apply pred function for upwards
  bContinue = bfApply(this,true);
  return bContinue;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CONJUNCTION::bContains_callToSubQuery(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  //this->vDump();

  Kid xKid(this);
  while (xKid.bEach(CK_TERM))
    if (pCI->m_pBench->bIsSubQueryNm(xKid.p_term()->szFacet(SF_NONE)))
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
    if (!Card_CONJUNCTION::bIsConj(xKid.pChild()))
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
void Card_CONJUNCTION::vEnsureTypesFor_stmCol(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  eCK eCard = this->eCard();

  switch (eCard) {

    case CK_ANDCONJ: _pCard_AndConj(this)->vEnsureTypesFor_stmCol(pCI); break;
    case CK_NOTCONJ: _pCard_NotConj(this)->vEnsureTypesFor_stmCol(pCI); break;
    case CK_ORCONJ:  _pCard_OrConj(this)->vEnsureTypesFor_stmCol(pCI);  break;
    case CK_RLNCONJ: _pCard_RlnConj(this)->vEnsureTypesFor_stmCol(pCI); break;

    case CK_TERM:    _pCard_Term(this)->vEnsureTypeFor_stmCol(pCI);     break;

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
    return p_term->szFacet(SF_NONE);

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a term                                                                 */
/*----------------------------------------------------------------------------*/
Card_Term* Card_CONJUNCTION::pGetFirstTerm_tblCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_TERM))
    if (xKid.p_term()->bIsTblCol())
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

    Stz* szValue_term = xKid.p_term()->szFacet(SF_NONE);
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
/* is this a correlation conjunction? (a conjunction in a sub-query)          */
/*----------------------------------------------------------------------------*/
bool Card_CONJUNCTION::bIsCorrConj(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (!pSymbol()->bSame("=="_Stz))
    return false;

  i8 nChildren = nChildCards();
  if (2 != nChildren)
    return false;

  Card_Term* p_term_0 = pGetTerm(0);
  Card_Term* p_term_1 = pGetTerm(1);

  if ((p_term_0->bIsSqyPrm() && (p_term_1->bIsStmCol() || p_term_1->bIsTblCol())) ||
      (p_term_1->bIsSqyPrm() && (p_term_0->bIsStmCol() || p_term_0->bIsTblCol())))
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

  if (pTerm_0->pSymbol()->bIs_Tbl_Col()) {
    // + term_0 is $c.n

    if (pTerm_1->pSymbol()->bIs_Tbl_Col()) {
      // + both are term:$c.n

      // ? is either term_0:$c.n or term_1:$c.n an index column
      if (p_index->bTblCol_IsKeyIn_index(pTerm_0->szFacet(SF_NONE)) ||
          p_index->bTblCol_IsKeyIn_index(pTerm_1->szFacet(SF_NONE)) )
        return true;

      return false;
    }
    else {
      // + only term_0 is $c.n

      // ? is term_0:$c.n an index column
      if (p_index->bTblCol_IsKeyIn_index(pTerm_0->szFacet(SF_NONE)))
        return true;

      return false;
    }
  }
  else
  if (pTerm_1->pSymbol()->bIs_Tbl_Col()) {
    // + only term_1 is $c.n

    // ? is term_0:$c.n an index column
    if (p_index->bTblCol_IsKeyIn_index(pTerm_1->szFacet(SF_NONE)) )
      return true;

    return false;
  }

  // + neither term_0 nor term_1 is $c.n
  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a potential join conjunction?                                      */
/*----------------------------------------------------------------------------*/
bool Card_CONJUNCTION::bIsJoinConj(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! pSymbol()->bSame("=="_Stz))
    return false;

  i8 nChildren = nChildCards();
  if (2 != nChildren)
    return false;

  Card_Term* p_term_0 = pGetTerm(0);
  Card_Term* p_term_1 = pGetTerm(1);

  if ((p_term_0->bIsStmCol() || p_term_0->bIsTblCol()) &&
      (p_term_1->bIsStmCol() || p_term_1->bIsTblCol()))
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
bool Card_CONJUNCTION::bIsSqyOp(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSymbol()->bIs_Sqy_Op())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_CONJUNCTION::vMorph_sqlOp_To_EQ(CardIter* pCI, Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pSymbol()->bSame("exists"_Stz)) {

   // pCI->vPrintBoard(__LINE__);

    SYM(SF_RLN_OP,"=="_Stz);

    Card_Term* p_term1 = _pCard_Term(pGetChild(1));
    ASSERT(pCI->m_pBench->bIsSubQueryNm(p_term1->szFacet(SF_NONE))); //temp

    Symbol x_Symbol_stmCol1;
    if (p_term) {
      x_Symbol_stmCol1.vFacets(p_term->pSymbol());
      p_term1->pSymbol()->vFacets(&x_Symbol_stmCol1);
    }
    else {
      //x_Symbol_stmCol1.vSet_Col_No(1);
      p_term1->SYM(SF_EXP_COL, (i8)1);
    }

    Card_Expr* p_expr_found =
        pCI->Flux.m_pArriving->pGet_expr((i8) 0);

    p_term1->pSymbol()->vColTypeId(p_expr_found->xColTypeId());

   // pCI->vPrintBoard(__LINE__);
    return;
  }

  if (pSymbol()->bSame("in"_Stz)) {

    SYM(SF_RLN_OP,"=="_Stz);

    Card_Term* p_term_popped = _pCard_Term(pPopChild());
    ASSERT(pCI->m_pBench->bIsSubQueryNm(p_term_popped->szFacet(SF_NONE))); //temp
    DELETE_CARD(p_term_popped);

    Card_Term* p_term_head = _pCard_Term(pGetChild(0));
    ASSERT(p_term_head->pSymbol()->bSame("#1"_Stz)); //temp

    if (p_term) {
      Symbol x_Symbol_stmCol1;
      x_Symbol_stmCol1.vFacets(p_term->pSymbol());
      p_term_head->pSymbol()->vFacets(&x_Symbol_stmCol1);
    }
    else {
      Symbol x_Symbol_stmCol1(SF_EXP_COL, 1);
      p_term_head->pSymbol()->vFacets(&x_Symbol_stmCol1);
    }

    Card_Expr* p_expr_found =
        pCI->Flux.m_pArriving->pGet_expr((i8) 0);

    p_term_head->pSymbol()->vColTypeId(p_expr_found->xColTypeId());

   // pCI->vPrintBoard(__LINE__);

    return;
  }

  if (pSymbol()->bSame("notIn"_Stz)) {

    SYM(SF_RLN_OP,"=="_Stz);

    Card_Term* p_term_popped = _pCard_Term(pPopChild());
    ASSERT(pCI->m_pBench->bIsSubQueryNm(p_term_popped->szFacet(SF_NONE))); //temp
    DELETE_CARD(p_term_popped);

    Card_Term* p_term_head = _pCard_Term(pGetChild(0));
    ASSERT(p_term_head->pSymbol()->bSame("#1"_Stz)); //temp

    Symbol x_Symbol_stmCol1(SF_EXP_COL, 1);
    if (p_term)
      x_Symbol_stmCol1.vFacetsPpy(p_term->pSymbol());
    p_term_head->pSymbol()->vFacetsPpy(&x_Symbol_stmCol1);
    return;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the E_eTerm_Bound of given index               */
/*----------------------------------------------------------------------------*/
Card_RlnConj* Card_CONJUNCTION::pPredFindBound(Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eCK eCard = this->eCard();

  switch (eCard) {

    case CK_ANDCONJ:
      return _pCard_AndConj(this)->pAndConjFindBound(pIndex, eBound);

    case CK_ORCONJ:
      return _pCard_OrConj(this)->pOrPredFindBound(pIndex, eBound);

    case CK_RLNCONJ:
      return _pCard_RlnConj(this)->pCmpPredFindBound(pIndex, eBound);

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_CONJUNCTION::vPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pRibbon->vSprintSnippet(false, "%c%c%s%lld]%s"_Stz,
                          cMark1(),
                          cMark2(),
                          Stz::szDots(iDepth - 1),
                          m_i_Child,

                          aRC[m_eConjRot]);

}

/*----------------------------------------------------------------------------*/
/* inspect <PRED>                                                             */
/*----------------------------------------------------------------------------*/
Symbol* Card_CONJUNCTION::pTable_WhichTableFor_Conj(CardIter* pCI)
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
      if (p_table->bScopeContains_Conj(this, pScope)) {
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
/* concat short form of this conjunction                                      */
/*----------------------------------------------------------------------------*/
void Card_CONJUNCTION::vShortForm(String* pString)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  //RASSERT(bIsOne(CK_ANDCONJ,CK_RLNCONJ,CK_NOTCONJ,CK_ORCONJ,CK_NONE));

  for (Kid xKid(this); xKid.bEach(); /**/) {
    if (xKid.eChild() == CK_TERM) {
      xKid.p_term()->vShortForm(pString, " "_Stz);
    }
    else {
      xKid.p_CONJUNCTION()->vShortForm(pString);
    }
  }

  if (bIsCard(CK_ANDCONJ)) {
    pString->vConcat(" and"_Stz);
  }
  else
  if (bIsCard(CK_RLNCONJ)) {
    pString->vCatSprint(" %s"_Stz, szFacet(SF_NONE));
  }
  else
  if (bIsCard(CK_NOTCONJ)) {
    pString->vConcat(" not"_Stz);
  }
  else
  if (bIsCard(CK_ORCONJ)) {
    pString->vConcat(" or"_Stz);
  }
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

    case CK_ANDCONJ: _pCard_AndConj(this)->vValidate(pCI, iDepth + 1, this); break;
    case CK_NOTCONJ:
      _pCard_NotConj(this)->vValidate(pCI, iDepth + 1, this); break;
    case CK_ORCONJ:
      _pCard_OrConj(this)->vValidate(pCI, iDepth + 1, this);  break;
    case CK_RLNCONJ:
      _pCard_RlnConj(this)->vValidate(pCI, iDepth + 1, this); break;

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
