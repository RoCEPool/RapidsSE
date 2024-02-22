/*============================================================================*/
/* Card_CONJ.cpp -- Abstract Conjunction                               */
/*============================================================================*/

#include "Card_CONJ.hpp"
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
Card_CONJ::Card_CONJ(eCK eCard, i8 iFixedDepth)
: Card_CONJSUBROOT(eCard, iFixedDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CONJ::~Card_CONJ(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is card a PRED ?                                                           */
/*----------------------------------------------------------------------------*/
bool Card_CONJ::bIsConj(Card* p_card)
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
/* visit all sub-cards of pred and apply lambda function                      */
/*----------------------------------------------------------------------------*/
bool Card_CONJ::bConjRecursive(std::function<bool(Card_CONJ* p_conj, bool bUpwards)> bfApply)
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
    bContinue = static_cast<Card_CONJ *>(pSubCard)->bConjRecursive(bfApply);
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
bool Card_CONJ::bContains_sqyNo(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  //this->vDump();

  Kid xKid(this);
  while (xKid.bEach(CK_TERM))
    if (xKid.p_term()->pSymbol()->bHasFacet(SF_SQY_NO)) {
      return true;
    }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CONJ::bDescend(std::function<bool(Card_CONJ* )> bfLambdaFn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach()) {
    if (!Card_CONJ::bIsConj(xKid.pChild()))
      continue;

    bool bContinue = ((Card_CONJ*)xKid.pChild())->bDescend(bfLambdaFn);
    if (! bContinue)
      return C_TERMINATE_RECURSION;
  }

  return C_CONTINUE_RECURSION;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_CONJ::vEnsure_Cast(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Cast xCast_last;
  xCast_last.m_xTypeId = TypeId();
  xCast_last.m_nTypeLength = 0;
  xCast_last.m_nTypeScale = 0;
  RASSERT(! xCast_last.m_xTypeId.bIsValid());

  eCK eCard = this->eCard();

  switch (eCard) {

    case CK_ANDCONJ:
      _pCard_AndConj(this)->vEnsure_Cast(pCI); break;
    case CK_NOTCONJ:
      _pCard_NotConj(this)->vEnsure_Cast(pCI); break;
    case CK_ORCONJ:
      _pCard_OrConj(this)->vEnsure_Cast(pCI);  break;
    case CK_RLNCONJ:
      _pCard_RlnConj(this)->vEnsure_Cast(pCI); break;

    case CK_TERM:
      _pCard_Term(this)->vApplyCast(pCI, /*??*/nullptr, xCast_last);  break;

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* attempt to get board name from one of the terms of this predicate          */
/*----------------------------------------------------------------------------*/
Stz* Card_CONJ::szGetTerm_sqyNo(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Term* p_term = pGetTerm_sqyNo(pCI))
    return p_term->szFacet(SF_SQY_NO);

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a term                                                                 */
/*----------------------------------------------------------------------------*/
Card_Term* Card_CONJ::pGetFirstTerm_tblCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_TERM))
    if (xKid.p_term()->pSymbol()->bHasFacet(SF_TBL_COL))
      return xKid.p_term();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a term                                                                 */
/*----------------------------------------------------------------------------*/
Card_Term* Card_CONJ::pGetTerm(i8 i_Term)
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
Card_Term* Card_CONJ::pGetTerm_sqyNo(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

  Kid xKid(this);
  while (xKid.bEach(CK_TERM)) {
    Card_Term* p_term = xKid.p_term();
    if (! p_term->pSymbol()->bHasFacet(SF_SQY_NO))
      continue;

    return p_term;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* is this predicate a bool fn?                                               */
/*----------------------------------------------------------------------------*/
bool Card_CONJ::bIsBoolFn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSymbol()->bHasFacet(SF_BOL_FN))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a correlation conjunction? (a conjunction in a sub-query)          */
/*----------------------------------------------------------------------------*/
bool Card_CONJ::bIsCorrConj(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (!pSymbol()->bHasFacet("=="_Stz))
    return false;

  i8 nChildren = nChildCards();
  if (2 != nChildren)
    return false;

  Card_Term* p_term_0 = pGetTerm(0);
  Card_Term* p_term_1 = pGetTerm(1);

  if ((p_term_0->pSymbol()->bHasFacet(SF_SQY_PRM) && (p_term_1->pSymbol()->bHasFacet(SF_EXP_COL) || p_term_1->pSymbol()->bHasFacet(SF_TBL_COL))) ||
      (p_term_1->pSymbol()->bHasFacet(SF_SQY_PRM) && (p_term_0->pSymbol()->bHasFacet(SF_EXP_COL) || p_term_0->pSymbol()->bHasFacet(SF_TBL_COL))))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a potential join conjunction?                                      */
/*----------------------------------------------------------------------------*/
bool Card_CONJ::bIsJoinConj(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! pSymbol()->bHasFacet("=="_Stz))
    return false;

  i8 nChildren = nChildCards();
  if (2 != nChildren)
    return false;

  Card_Term* p_term_0 = pGetTerm(0);
  Card_Term* p_term_1 = pGetTerm(1);

  if ((p_term_0->pSymbol()->bHasFacet(SF_EXP_COL) || p_term_0->pSymbol()->bHasFacet(SF_TBL_COL)) &&
      (p_term_1->pSymbol()->bHasFacet(SF_EXP_COL) || p_term_1->pSymbol()->bHasFacet(SF_TBL_COL)))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this predicate a relational operator?                                   */
/*----------------------------------------------------------------------------*/
bool Card_CONJ::bIsRelnOp(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSymbol()->bHasFacet(SF_RLN_OP))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this predicate a subquery operator?                                     */
/*----------------------------------------------------------------------------*/
bool Card_CONJ::bIsSqyOp(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSymbol()->bHasFacet(SF_SQY_OP))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_CONJ::vMorph_sqlOp_To_EQ(CardIter* pCI, Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pSymbol()->bHasFacet("exists"_Stz)) {

   // pCI->vPrintBoard(__func__,__LINE__);

    SYM(SF_RLN_OP,"=="_Stz);

    Card_Term* p_term1 = _pCard_Term(pGetChild(1));
    RASSERT(pCI->m_pBench->bKnown_sqy(p_term1->pSymbol())); //temp

    Symbol x_Symbol_expCol1;
    if (p_term) {
      x_Symbol_expCol1.vApplyCastFacets(p_term->pSymbol());
      p_term1->pSymbol()->vApplyCastFacets(&x_Symbol_expCol1);
    }
    else {
      //x_Symbol_expCol1.vSet_Col_No(1);
      p_term1->SYM(SF_EXP_COL, (i8)1);
    }

    Card_Expr* p_expr_found =
        pCI->Flux.m_pArriving->pGet_expr((i8) 0);

    p_term1->vApplyCast_x(p_expr_found->xCast());

   // pCI->vPrintBoard(__func__,__LINE__);
    return;
  }

  if (pSymbol()->bHasFacet("in"_Stz)) {

    SYM(SF_RLN_OP,"=="_Stz);

    Card_Term* p_term_popped = _pCard_Term(pPopChild());
    RASSERT(pCI->m_pBench->bKnown_sqy(p_term_popped->pSymbol())); //temp
    DELETE_CARD(p_term_popped);

    Card_Term* p_term_head = _pCard_Term(pGetChild(0));
    ASSERT(p_term_head->pSymbol()->bHasFacet("#1"_Stz)); //temp

    if (p_term) {
      Symbol x_Symbol_expCol1;
      x_Symbol_expCol1.vApplyCastFacets(p_term->pSymbol());
      p_term_head->pSymbol()->vApplyCastFacets(&x_Symbol_expCol1);
    }
    else {
      Symbol x_Symbol_expCol1(SF_EXP_COL, 1);
      p_term_head->pSymbol()->vApplyCastFacets(&x_Symbol_expCol1);
    }

    Card_Expr* p_expr_found =
        pCI->Flux.m_pArriving->pGet_expr((i8) 0);

    p_term_head->vApplyCast_x(p_expr_found->xCast());

   // pCI->vPrintBoard(__func__,__LINE__);

    return;
  }

  if (pSymbol()->bHasFacet("notIn"_Stz)) {

    SYM(SF_RLN_OP,"=="_Stz);

    Card_Term* p_term_popped = _pCard_Term(pPopChild());
    ASSERT(pCI->m_pBench->bKnown_sqy(p_term_popped->pSymbol())); //temp
    DELETE_CARD(p_term_popped);

    Card_Term* p_term_head = _pCard_Term(pGetChild(0));
    ASSERT(p_term_head->pSymbol()->bHasFacet("#1"_Stz)); //temp

    Symbol x_Symbol_expCol1(SF_EXP_COL, 1);
    if (p_term)
      x_Symbol_expCol1.vApplyCastFacetsPpy(p_term->pSymbol());
    p_term_head->pSymbol()->vApplyCastFacetsPpy(&x_Symbol_expCol1);
    return;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_CONJ::vPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pRibbon->vSprintSnippet(false, "%c%c%s%lld]"_Stz,
                          cMark1(),
                          cMark2(),
                          Stz::szDots(iDepth - 1),
                          m_i_Child);

}

/*----------------------------------------------------------------------------*/
/* inspect <PRED>                                                             */
/*----------------------------------------------------------------------------*/
Symbol* Card_CONJ::pTable_WhichTableFor_Conj(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Scope* pScope = new Scope;

  i8 i_Main = 0;

  i8 i_Fold = C_UNKNOWN;
  for (i8 i_Test = 0 ; i_Test < pCI->G.pDeck->nChildCards() ; i_Test++) {
    Card *pTestCard = pCI->G.pDeck->pGetChild(i_Test);

    if (pTestCard->bIsCard(CK_c_FOLD))
      i_Fold = i_Test;
  }

  if (C_UNKNOWN != i_Fold)
    i_Main = i_Fold + 1;

  for (/* none */ ; /* none */ ; i_Main++) {
    Card *pMainCard = pCI->G.pDeck->pGetChild(i_Main);

    if (CK_o_WHERE == pMainCard->eCard()) {
      delete pScope;
      return nullptr;
    }

    eCK eMainCard = pMainCard->eCard();

    if (CK_o_FROM == eMainCard) {
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
void Card_CONJ::vShortForm(String* pString)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  //RASSERT(bIsOne(CK_ANDCONJ,CK_RLNCONJ,CK_NOTCONJ,CK_ORCONJ,CK_NONE));

  for (Kid xKid(this); xKid.bEach(); /**/) {
    if (xKid.eChild() == CK_TERM) {
      xKid.p_term()->vShortForm(pString, " "_Stz);
    }
    else {
      xKid.p_CONJ()->vShortForm(pString);
    }
  }

  if (bIsCard(CK_ANDCONJ)) {
    pString->vConcat(" and"_Stz);
  }
  else
  if (bIsCard(CK_RLNCONJ)) {
    pString->vCatSprint(" %s"_Stz, szFacet(SF_RLN_OP));
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
void Card_CONJ::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
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
     // pCI->vPrintBoard(__func__,__LINE__,true);
      vDump(__func__);
      vValidationFailed("<<replace>> %p"_Stz,this);
  }

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
