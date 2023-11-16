/*============================================================================*/
/* Card_BoolPred.cpp -- Boolean Predicate                                     */
/*============================================================================*/

#include "Card_BoolPred.hpp"

#include "Card_CONJ.hpp"
#include "Card_RlnConj.hpp"
#include "Card_Term.hpp"
#include "CardIter.hpp"
#include "Card_AndConj.hpp"
#include "Card_OuterColNm.hpp"
#include "Card_Filter.hpp"
#include "Card_Table.hpp"
#include "Card_Deck.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_BoolPred, CK_BOOLPRED)
__aNewFunction(Card_BoolPred, CN_CARD)
__vDeleteFunction(Card_BoolPred, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_BoolPred::Card_BoolPred(void)
: Card_CONJSUBROOT(CK_BOOLPRED, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
      
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_BoolPred::~Card_BoolPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert <rpnCalc> to <boolPred>                                            */
/*----------------------------------------------------------------------------*/
Card_BoolPred* Card_BoolPred::pTo_boolPred(Card_RpnCalc* p_rpnCalc_arg)
{
  _static_

  p_rpnCalc_arg->vDump(__func__,"RPNCALC_ARG"_Stz);

  Card_BoolPred* p_boolPred_new = new Card_BoolPred;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // does the rpn calc have a when in it?

  Symbol xWhen(SF_WHEN,"WHEN"_Stz);
  if (p_rpnCalc_arg->bHasTerm(&xWhen)) {
    p_boolPred_new->vTransferChildren(p_rpnCalc_arg);
    return p_boolPred_new;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_RpnExpr* pStack_conj   = new Card_RpnExpr;
  Card_RpnExpr* pStack_term   = new Card_RpnExpr;

  Card* p_child_0 = nullptr;
  Card_AndConj* p_andConj = nullptr;
  Card_RlnConj* p_rlnConj = nullptr;
  Card_NotConj* p_notConj = nullptr;
  Card_OrConj*  p_orConj  = nullptr;


  while (Card_Term* p_term_rpn = _pCard_Term(p_rpnCalc_arg->pPopChild())) {

    p_boolPred_new->vDump(__func__,"BOOLPRED_NEW"_Stz);
    pStack_conj->vDump(__func__, "STACK_OP"_Stz);
    pStack_term->vDump(__func__,"STACK_TERM"_Stz);

    eTE eTerm_rpn = p_term_rpn->eEvalTerm();
    switch (eTerm_rpn) {

      case TE_AND:
        if (p_boolPred_new->bIsPeek(CK_NOTCONJ, CK_ANDCONJ)) {
          DELETE_CARD(p_term_rpn);
          p_notConj = _pCard_NotConj(p_boolPred_new->pPopChild());
          p_andConj = _pCard_AndConj(p_boolPred_new->pPopChild());
          p_andConj->vAppendChild(p_notConj);
          p_boolPred_new->vPush(p_andConj);
        }
        else
        if (p_boolPred_new->bIsPeek(1, CK_ANDCONJ)) {
          DELETE_CARD(p_term_rpn);
          p_child_0 = p_boolPred_new->pPopChild();
          p_andConj = _pCard_AndConj(p_boolPred_new->pPopChild());
          p_andConj->vAppendChild(p_child_0);
          p_boolPred_new->vPush(p_andConj);
        }
        else {
          p_andConj  = _pCard_AndConj(p_term_rpn->pCard(CK_ANDCONJ));
          p_andConj->vPushChild(p_boolPred_new->pPopChild());
          p_andConj->vPushChild(p_boolPred_new->pPopChild());
          p_boolPred_new->vPush(p_andConj);
        }
        break;

      case TE_OR:
        if (p_boolPred_new->bIsPeek(CK_NOTCONJ, CK_ORCONJ)) {
          DELETE_CARD(p_term_rpn);
          p_notConj = _pCard_NotConj(p_boolPred_new->pPopChild());
          p_orConj  = _pCard_OrConj(p_boolPred_new->pPopChild());
          p_orConj->vAppendChild(p_notConj);
          p_boolPred_new->vPush(p_orConj);
        }
        else
        if (p_boolPred_new->bIsPeek(1, CK_ORCONJ)) {
          DELETE_CARD(p_term_rpn);
          p_child_0 = p_boolPred_new->pPopChild();
          p_orConj  = _pCard_OrConj(p_boolPred_new->pPopChild());
          p_orConj->vAppendChild(p_child_0);
          p_boolPred_new->vPush(p_orConj);
        }
        else {
          p_orConj = _pCard_OrConj(p_term_rpn->pCard(CK_ORCONJ));
          p_orConj->vAppendChild(p_boolPred_new->pPopChild());
          p_orConj->vAppendChild(p_boolPred_new->pPopChild());
         p_boolPred_new->vPush(p_orConj);
        }
        break;

      case TE_NOT:
        p_notConj = _pCard_NotConj(p_term_rpn->pCard(CK_NOTCONJ));
        p_notConj->vAppend(p_boolPred_new->pPopChild());
        p_boolPred_new->vPush(p_notConj);
        break;

      case TE_RELNOP:
      case TE_EXISTS:
      case TE_NOTEXISTS:
      case TE_IN:
      case TE_NOTIN:
        p_rlnConj = _pCard_RlnConj(p_term_rpn->pCard(CK_RLNCONJ));
        p_rlnConj->vTransferChildren(pStack_term);
        p_boolPred_new->vPush(p_rlnConj);
        break;

      case TE_BOOLFN:
        p_rlnConj = _pCard_RlnConj(p_term_rpn->pCard(CK_RLNCONJ));
        p_rlnConj->vTransferChildren(pStack_term);
        p_boolPred_new->vPush(p_rlnConj);
        break;

      case TE_OTHER:
        pStack_term->vAppend(p_term_rpn->pCard(CK_TERM));
        break;

      case TE_EXECUTE:
        DELETE_CARD(p_term_rpn);
        break;

      default:
        NEVER_REACH
    }
  }


  RASSERT(0 == pStack_conj->nChildCards());
  DELETE_CARD(pStack_conj);
  RASSERT(0 == pStack_term->nChildCards());
  DELETE_CARD(pStack_term);

  return p_boolPred_new;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* bump any expCol by given amount                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vBump_expCol(i8 nBumpBy, bool bSkipFoldExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CONJ* p_conj_ROOT = this->pRoot_conj();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfBump = [nBumpBy,bSkipFoldExpr](Card_CONJ* p_conj, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_conj->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_conj);
    if (bSkipFoldExpr && p_term->pSymbol()->bSame("$1"_Stz))
      return C_CONTINUE_RECURSION;

    if (p_term->pSymbol()->bIs_Exp_Col()) {
      p_term->vBump_expCol(nBumpBy);
      p_term->vMark1(MARK_CHANGED);
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_conj_ROOT->bConjRecursive(bfBump);
  UNUSED(bIgnore);
}

/*----------------------------------------------------------------------------*/
/* get typeID                                                                 */
/*----------------------------------------------------------------------------*/
Cast Card_BoolPred::xCast(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Cast xCast;

  xCast.m_xTypeId = TypeId::xIdOf(CT_BOOLEAN);

  return xCast
}

/*----------------------------------------------------------------------------*/
/* does this boolPred contain given symbol?                                   */
/*----------------------------------------------------------------------------*/
bool Card_BoolPred::bContains(Symbol* pGivenSymbol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CONJ* p_conj_ROOT = this->pRoot_conj();

  bool bDoesContain = false;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfContains = [pGivenSymbol,&bDoesContain](Card_CONJ* p_conj, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_conj->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_conj);

    if (p_term->pSymbol()->bMatches(pGivenSymbol)) {
      bDoesContain = true;
      return C_TERMINATE_RECURSION;
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_conj_ROOT->bConjRecursive(bfContains);
  UNUSED(bIgnore);

  return bDoesContain;
}

/*----------------------------------------------------------------------------*/
/* copy any marked rlnCon to its <OP/filter>                                  */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vCopyMarked_rlnConj_To_table_filter(CardIter* pCI, Stz* szWithMark)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Deck* pDeck = pCI->G.pDeck;
  Card *pMainCard;

  pCI->vRecurseBegin(this);

  while (Card_CONJ* p_conj = (Card_CONJ*)pCI->pRecurseEach()) {
    if (! p_conj->bMark1(szWithMark))
      continue;

    RASSERT(p_conj->bIsCard(CK_RLNCONJ));
    Card_RlnConj* p_rlnConj = _pCard_RlnConj(p_conj);

    char cTableSyn = p_rlnConj->cMark2();
    Card_Table *p_table = pDeck->pFind_table(cTableSyn, pMainCard);
    if (! p_table)
      continue;

    pCI->G.p_filter = ((Card_OP *) pMainCard)->pEnsure_filter();
    pCI->G.p_filter->vExtend_rlnConj(p_rlnConj->pClone());

    pCI->G.p_filter->vMark1(p_table->cMark1());
    pCI->G.p_filter->vMark2(p_table->cMark2());
  }

  pCI->vRecurseEnd();

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vCorrect_expCol(Card_Emit* p_arriving)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfCorrect = [p_arriving](Card* p_card)
  {
    if (p_card->pSymbol()->bIs_Exp_Col())
      _pCard_Term(p_card)->vCorrect_expCol(p_arriving);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = bApplyRecursive(CK_TERM, bfCorrect);
  UNUSED(bIgnore);
}

/*----------------------------------------------------------------------------*/
/* calculate number of conjunctions with given mark                           */
/*----------------------------------------------------------------------------*/
i8 Card_BoolPred::nCullMarked_rlnConj(Stz* szWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nCulled = 0;

  if (! nChildCards())
    return 0;

  Card_CONJ* p_conj_root = pRoot_conj();

  while (p_conj_root) {

    if (!bCullMarked_rlnConj(p_conj_root, szWithMark))
      break;

    nCulled++;

    if (! this->nChildCards())
      break;

    p_conj_root = pRoot_conj();
  }

  return nCulled;
}

/*----------------------------------------------------------------------------*/
/* remove given conjunction                                                     */
/*----------------------------------------------------------------------------*/
bool Card_BoolPred::bCullMarked_rlnConj(Card_CONJ* p_conj, Stz* szWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eCK eCard_conj = p_conj->eCard();

  if (CK_TERM == eCard_conj)
    return false;

  if (CK_RLNCONJ == eCard_conj) {
    if (p_conj->bMark1(szWithMark)) {
      vRemove_rlnConj(_pCard_RlnConj(p_conj));
      delete p_conj;
      return true;
    }
    return false;
  }

  RASSERT (CK_ANDCONJ == eCard_conj || CK_ORCONJ == eCard_conj || CK_NOTCONJ == eCard_conj);

  i8 nChildren = p_conj->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_CONJ* pCard_conj_child = static_cast<Card_CONJ*>(p_conj->pGetChild(i_Child));
    if (this->bCullMarked_rlnConj(pCard_conj_child, szWithMark))
      return true;
  }

  return  false;
}

/*----------------------------------------------------------------------------*/
/* delete facet if                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vDeleteFacetIf(eSF eFacet, eSF eIfFacet, char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  auto fvDeleteFacetIf = [eFacet,eIfFacet,cTblSyn](Card* pCard_term)
  {
    Card_Term* p_term = _pCard_Term(pCard_term);

    p_term->vDeleteFacetIf(eFacet, eIfFacet, cTblSyn);
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = bApplyRecursive(CK_TERM,fvDeleteFacetIf);
  UNUSED(bIgnore);
}

/*----------------------------------------------------------------------------*/
/* ensure all terms have a type                                               */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vEnsureTypeId_expCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CONJ* p_conj = pRoot_conj();

  if (p_conj)
    p_conj->vEnsureTypeId_expCol(pCI);
}

/*----------------------------------------------------------------------------*/
/* add given rlnConj to this boolPred                                         */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vExtend_andConj(Card_RlnConj *p_rlnConj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  // no conjunctions; simply append given rlnConj to this boolPred
  if (0 == nChildCards()) {
    vAppendChild(p_rlnConj);
    return;
  }

  // top is andConj; append given rlnConj to that andConj
  if (CK_ANDCONJ == ePeekChild()) {
    pPeekChild()->vAppendChild(p_rlnConj);
    return;
  }

  Card* p_top = pPopChild();

  Card_AndConj* pCard_conjAnd = new Card_AndConj;
  pCard_conjAnd->vAppendChild(p_top);
  pCard_conjAnd->vAppendChild(p_rlnConj);

  vAppendChild(pCard_conjAnd);
}

/*----------------------------------------------------------------------------*/
/* inspect <boolPred>> and mark any <conj> which could be moved               */
/*----------------------------------------------------------------------------*/
Card_RlnConj * Card_BoolPred::pFind_rlnConj(Stz* szWithPpy)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_RlnConj* p_rlnConj =
      _pCard_RlnConj(pFindPpyRecursive(szWithPpy, CK_RLNCONJ));

  return p_rlnConj;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_RlnConj* Card_BoolPred::pFind_rlnConj_With_sqyCall(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_RlnConj* p_rlnConj = nullptr;

  auto bLambdaFn = [&p_rlnConj](Card_CONJ* p_conj, bool bUpwards)
  {
    UNUSED(bUpwards);

    if (p_conj->bContains_sqyCall()) {
      p_rlnConj = _pCard_RlnConj(p_conj);
      return C_TERMINATE_RECURSION;
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_CONJ* p_conj_ROOT = this->pRoot_conj();

  bool bIgnore = p_conj_ROOT->bConjRecursive(bLambdaFn);
  UNUSED(bIgnore);

  return p_rlnConj;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_BoolPred::bHas_term(Symbol* pWithSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool bHas = false;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pWithSymbol, &bHas](Card* p_card)
  {
    if (p_card->pSymbol()->bMatches(pWithSymbol)) {
      bHas = true;
      return C_TERMINATE_RECURSION;
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = bApplyRecursive(CK_TERM,bLambdaFn);
  UNUSED(bIgnore);

  return bHas;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_BoolPred::bIsBraced(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool bBraced = false;
  auto bTestBraced=[&bBraced](Card* p_card) {

    if (p_card->pSymbol()->bNeedsBrace()) {
      bBraced = true;
      return C_TERMINATE_RECURSION;
    }

    return C_CONTINUE_RECURSION;
  };

  bApplyRecursive(CK_NONE,bTestBraced);

  return bBraced;
}

/*----------------------------------------------------------------------------*/
/* is this a compound boolPred                                                */
/*----------------------------------------------------------------------------*/
bool Card_BoolPred::bIsCompound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (nChildCards() > 1)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a compound boolPred                                                */
/*----------------------------------------------------------------------------*/
bool Card_BoolPred::bIsLiterals(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bLiterals = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  auto fbTestLiterals = [&bLiterals](Card* p_card)
  {
    Card_Term* p_term = _pCard_Term(p_card);

    if (p_term->bIsColSym()) {
      bLiterals = true;
      return C_TERMINATE_RECURSION;
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = bApplyRecursive(CK_TERM,fbTestLiterals);
  UNUSED(bIgnore);

  return bLiterals;
}

/*----------------------------------------------------------------------------*/
/* is this boolPred a simple expression of a column                           */
/*----------------------------------------------------------------------------*/
bool Card_BoolPred::bIsScalarCol(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (nChildCards() > 1)
    return false;

  Card *p_card_0 = pGetChild(0);
  if (!p_card_0->bIsCard(CK_TERM))
    return false;

  Card_Term *p_term_0 = _pCard_Term(p_card_0);
  if (!p_term_0->bIsTblCol())
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this boolPred a simple expression of a key                              */
/*----------------------------------------------------------------------------*/
bool Card_BoolPred::bIsScalarKey(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (nChildCards() > 1)
    return false;

  Card *p_card_0 = pGetChild(0);
  if (!p_card_0->bIsCard(CK_TERM))
    return false;

  Card_Term *p_term_0 = _pCard_Term(p_card_0);
  if (!p_term_0->bIsKeyCol())
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* inspect <boolPred> and mark any <conj> which could be moved                */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vMark_rlnConj_Wrt_table(CardIter* pCI, Stz* szWithMark)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->vRecurseBegin(this);
  while (Card* p_card = pCI->pRecurseEach()) {

    //p_card->vDump(__func__);

    if (p_card->bIsCard(CK_ANDCONJ)) {
      p_card->vMark1(MARK_AND);
      continue;
    }

    if (p_card->bIsCard(CK_NOTCONJ)) {
      p_card->vMark1(MARK_NOT);
      pCI->vDontDescend(true);
      continue;
    }

    if (p_card->bIsCard(CK_ORCONJ)) {
      p_card->vMark1(MARK_OR);
      pCI->vDontDescend(true);
      continue;
    }

    if (p_card->bIsCard(CK_RLNCONJ)) {
      p_card->vMark1(MARK_COMPARE);
      pCI->vDontDescend(true);
      if (((Card_CONJ *) p_card)->bContains_sqyCall())
        continue;
      Symbol* pWhichTable = ((Card_CONJ *) p_card)->pTable_WhichTableFor_Conj(pCI);
      if (pWhichTable) {
        p_card->vMark1(szWithMark);
        p_card->vMark2(pWhichTable->cSF_char());
      }
      continue;
    }

    if (p_card->bIsCard(CK_TERM))
      continue;

    NEVER_REACH
  }

  pCI->vRecurseEnd();
}

/*----------------------------------------------------------------------------*/
/* append <rpnExpr> for <boolPred>                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vOntoRpnExpr(String* psRpnExpr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (bIsBraced())
    psRpnExpr->vConcat(" }"_Stz);

  if (pPeekChild()->bIsConj())
    Card_CONJSUBROOT::vOntoRpnExpr(psRpnExpr,pRoot_conj());
  else
    Card_CONJSUBROOT::vOntoRpnExpr(psRpnExpr);
}

/*----------------------------------------------------------------------------*/
/* remove given rlnConj from this boolPred                                    */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vRemove_rlnConj(Card_RlnConj *p_rlnConj)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //vDump(__func__,"BEFORE"_Stz);
  //p_rlnConj->vDump(__func__,"REMOVE"_Stz);
  Card* p_rlnConj_parent = pFindParentRecursive(p_rlnConj);

  p_rlnConj_parent->vRemoveSub(p_rlnConj);

  if (CK_ANDCONJ == p_rlnConj_parent->eCard() ||
      CK_ORCONJ  == p_rlnConj_parent->eCard()) {

    if (1 == p_rlnConj_parent->nChildCards()) {
      Card* p_rlnConj_sibling = p_rlnConj_parent->pPopChild();
      Card* p_rlnConj_grand = this->pFindParentRecursive((p_rlnConj_parent));
      p_rlnConj_grand->vReplaceChild(p_rlnConj_parent,p_rlnConj_sibling);
      delete p_rlnConj_parent;
    }

    //vDump(__func__,"AFTER"_Stz);
    vDebugPoint();
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_CONJ* Card_BoolPred::pRoot_conj(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! nChildCards())
    return nullptr;

  Card_CONJ* p_conj = static_cast<Card_CONJ*>(pGetChild(0));

  return p_conj;
}

/*----------------------------------------------------------------------------*/
/* short form                                                                 */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vShortForm(String* pString)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! nChildCards())
    return;

  Card_CONJ* p_conj = pRoot_conj();

  p_conj->vShortForm(pString);
}

/*----------------------------------------------------------------------------*/
/* get typeID                                                                 */
/*----------------------------------------------------------------------------*/
TypeId Card_BoolPred::xTypeId(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return TypeId::xIdOf(CT_BOOLEAN);
}

/*----------------------------------------------------------------------------*/
/* remove given conjunction                                                   */
/*----------------------------------------------------------------------------*/
Card_RlnConj * Card_BoolPred::pYank_rlnConj(Stz* szWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! this->nChildCards())
    return nullptr;

  Card_RlnConj* p_rlnConj_yanked = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //
  auto bYank = [](Card_BoolPred* p_boolPred,
                  Stz* szWithMark,
                  Card* p_test,
                  Card_RlnConj*& p_conj_yanked,
                  auto&& bYank /*oddity permits recursive lambda*/)
  {
    //p_test->vDump();

    if (Card_CONJ::bIsConj(p_test)) {
      if (p_test->bMark1(szWithMark)) {
        p_conj_yanked = _pCard_RlnConj(p_test);
        p_boolPred->vRemove_rlnConj(p_conj_yanked);
         return true;
      }
    }

    Kid xKid(p_test);
    while (xKid.bEach()) {
      if (bYank(p_boolPred,szWithMark,xKid.pChild(),p_conj_yanked,bYank)) {
        return true;
      }
    }

    RASSERT(! p_conj_yanked);
    return false;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  Kid xKid(this);
  while (xKid.bEach()) {
    if (bYank(this, szWithMark, xKid.pChild(), p_rlnConj_yanked, bYank)) {
      return _pCard_RlnConj(p_rlnConj_yanked);
    }
  }

  RASSERT(! p_rlnConj_yanked);
  return nullptr;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vX_expCol_To_tblCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CONJ* p_conj_root = this->pRoot_conj();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI](Card_CONJ* p_conj, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_conj->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_conj);
    Symbol* pSymbol = p_term->pSymbol();
    if (!pSymbol->bIs_Exp_Col())
      return C_CONTINUE_RECURSION;

    p_term->vX_expCol_To_tblCol(pCI);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_conj_root->bConjRecursive(bLambdaFn);
  UNUSED(bIgnore);
}

/*----------------------------------------------------------------------------*/
/* convert tblCol to stmCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vX_tblCol_To_expCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CONJ* p_conj_root = this->pRoot_conj();
  if (! p_conj_root)
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfTo_expCol = [pCI](Card_CONJ* p_conj, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_conj->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_conj);
    Symbol* pSymbol = p_term->pSymbol();
    if (!pSymbol->bIs_Tbl_Col())
      return C_CONTINUE_RECURSION;

    p_term->vX_tblCol_To_expCol(pCI);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_conj_root->bConjRecursive(bfTo_expCol);
  UNUSED(bIgnore);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

//  Card::vValidate_hasChildren();
  if (nChildCards())
    Card::eValidate_mustSome(CK_ANDCONJ,CK_NOTCONJ,CK_ORCONJ,CK_RLNCONJ,
                               CK_TERM,CK_NONE);

  (void)Card::eValidate_parentEither(pCI,CK_CONSTRAINT,CK_FILTER,CK_NONE);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
