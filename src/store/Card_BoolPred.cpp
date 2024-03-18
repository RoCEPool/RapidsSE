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
void Card_BoolPred::vTo_boolPred(Card_Expr* p_expr_arg)
{
  _static_

  Card_BoolPred* p_boolPred_new = new Card_BoolPred;

  p_boolPred_new->xTypeId() = TypeId::xIdOf(CT_BOOLEAN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // does the expression have a WHEN in it?

  Symbol xWhen(SF_WHEN,"WHEN"_Stz);
  if (p_expr_arg->bHasTerm(&xWhen)) {
    Card_RpnCalc* p_rpnCalc_arg = p_expr_arg->pSub_rpnCalc(C_NOT_NULL);
    p_boolPred_new->vTransferChildren(p_rpnCalc_arg);
    p_expr_arg->vReplaceSub(p_rpnCalc_arg, p_boolPred_new);
    DELETE_CARD(p_rpnCalc_arg);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_RpnExpr* pStack_conj   = new Card_RpnExpr;
  Card_RpnExpr* pStack_term   = new Card_RpnExpr;

  Card* p_child_0 = nullptr;
  Card_AndConj* p_andConj = nullptr;
  Card_RlnConj* p_rlnConj = nullptr;
  Card_NotConj* p_notConj = nullptr;
  Card_OrConj*  p_orConj  = nullptr;

  while (Card_Term* p_term_rpn = _pCard_Term(p_expr_arg->pSub_rpnCalc(C_NOT_NULL)->pPopChild())) {

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

  Card_RpnCalc* p_rpnCalc_arg = p_expr_arg->pSub_rpnCalc(C_NOT_NULL);
  p_expr_arg->vReplaceSub(p_rpnCalc_arg, p_boolPred_new);
  DELETE_CARD(p_rpnCalc_arg);
}

/*----------------------------------------------------------------------------*/
/* convert <expr>/<rpnCalc> to <boolPred>                                     */
/*----------------------------------------------------------------------------*/
Card_BoolPred* Card_BoolPred::pTo_boolPred(Card_Expr* p_expr_arg)
{
  _static_

  Card_BoolPred* p_boolPred_new = new Card_BoolPred;

  p_boolPred_new->xTypeId() = TypeId::xIdOf(CT_BOOLEAN);

  Card_RpnCalc* p_rpnCalc_arg = p_expr_arg->pSub_rpnCalc(C_NOT_NULL);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // does the expression have a WHEN in it?

  Symbol xWhen(SF_WHEN,"WHEN"_Stz);
  if (p_expr_arg->bHasTerm(&xWhen)) {
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
          p_andConj = _pCard_AndConj(p_term_rpn->pCard(CK_ANDCONJ));
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
/* add given rlnConj to this boolPred                                         */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vAppend_and_rlnConj(Card_RlnConj *p_rlnConj)
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
/* bump any expCol by given amount                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vBump_expCol(i8 nBumpBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CONJ* p_conj_ROOT = this->pRoot_conj();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfBump = [nBumpBy](Card_CONJ* p_conj, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_conj->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_conj);

    if (p_term->pSymbol()->bHasFacet(SF_EXP_COL)) {
      p_term->vBump_ordCol(SF_EXP_COL,nBumpBy);
      p_term->vMark1(MARK_CHANGED);
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_conj_ROOT->bLambdaWalk_conj(bfBump);
  UNUSED(bIgnore);
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

  bool bIgnore = p_conj_ROOT->bLambdaWalk_conj(bfContains);
  UNUSED(bIgnore);

  return bDoesContain;
}

/*----------------------------------------------------------------------------*/
/* copy any marked rlnCon to its <OP/filter>                                  */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vCopyMarked_rlnConj_To_table_filter(CardIter* pCI, Stz* szWithMark)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Deck* pDeck = pCI->G.p_deck;
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
    pCI->G.p_filter->pSub_boolPred()->vAppend_and_rlnConj(p_rlnConj->pClone());

    pCI->G.p_filter->vMark1(p_table->cMark1());
    pCI->G.p_filter->vMark2(p_table->cMark2());
  }

  pCI->vRecurseEnd();

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vUpdate_expCol(Card_Emit* p_Dic_expr, bool bAlt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfCorrect = [bAlt,p_Dic_expr](Card* p_card)
  {
    if (p_card->pSymbol()->bHasFacet(SF_EXP_COL))
      _pCard_Term(p_card)->vUpdate_expCol(p_Dic_expr,bAlt);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = bLambdaWalk(CK_TERM, bfCorrect);
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

  bool bIgnore = bLambdaWalk(CK_TERM,fvDeleteFacetIf);
  UNUSED(bIgnore);
}

/*----------------------------------------------------------------------------*/
/* ensure all terms have a type                                               */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vApplyCast(CardIter* pCI, Card_Emit* p_Dic_expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CONJ* p_conj = pRoot_conj();

  if (CK_TERM != p_conj->eCard()) {
    p_conj->vApplyCast(pCI,/*?*/p_Dic_expr);
    return;
  }

  Cast xCast_last;
  xCast_last.m_xTypeId = TypeId();
  xCast_last.m_nTypeLength = 0;
  xCast_last.m_nTypeScale = 0;
  RASSERT(! xCast_last.m_xTypeId.bIsValid());

  for (Kid xBoolPred(this); xBoolPred.bEach(CK_TERM); /**/) {
    xBoolPred.p_term()->vApplyCast(pCI, /*??*/p_Dic_expr, xCast_last);
  }
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

  bool bIgnore = bLambdaWalk(CK_TERM,bLambdaFn);
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

  bLambdaWalk(CK_NONE,bTestBraced);

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

    if (p_term->pSymbol()->bHasFacet(SF_EXP_COL) || p_term->pSymbol()->bHasFacet(SF_TBL_COL)) {
      bLiterals = true;
      return C_TERMINATE_RECURSION;
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = bLambdaWalk(CK_TERM,fbTestLiterals);
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
  if (! p_term_0->pSymbol()->bHasFacet(SF_TBL_COL))
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
  if (!p_term_0->pSymbol()->bHasFacet(SF_KEY_COL))
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
      if (((Card_CONJ *) p_card)->bHasChildFacet(SF_SQY_NO))
        continue;
      Symbol* pWhichTable = ((Card_CONJ *) p_card)->pTable_WhichTableFor_Conj(pCI);
      if (pWhichTable) {
        p_card->vMark1(szWithMark);
        p_card->vMark2(pWhichTable->cTblSyn());
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

  if (pPeekChild()->bEither(CK_ANDCONJ, CK_RLNCONJ, CK_NOTCONJ, CK_ORCONJ, CK_NONE))
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

    pCI->G.p_term = _pCard_Term(p_conj);
    Symbol* pSymbol = pCI->G.p_term->pSymbol();
    if (!pSymbol->bHasFacet(SF_EXP_COL)) {
      pCI->G.p_term = nullptr;
      return C_CONTINUE_RECURSION;
    }

    pCI->G.p_term->vX_expCol_To_tblCol(pCI);

    pCI->G.p_term = nullptr;
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_conj_root->bLambdaWalk_conj(bLambdaFn);
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
    if (!pSymbol->bHasFacet(SF_TBL_COL))
      return C_CONTINUE_RECURSION;

    p_term->vX_tblCol_To_expCol(pCI);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_conj_root->bLambdaWalk_conj(bfTo_expCol);
  UNUSED(bIgnore);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  (void)Card::eValidate_parentEither(pCI,
                                     CK_s_CONSTRAINT,
                                     CK_EXPR,
                                     CK_o_FILTER,
                                     CK_NONE);

  if (nChildCards())
    Card::eValidate_mustSome(CK_ANDCONJ,
                             CK_NOTCONJ,
                             CK_ORCONJ,
                             CK_RLNCONJ,
                             CK_TERM,
                             CK_NONE);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
