/*============================================================================*/
/* Card_Bound.cpp -- Abstract Bound                                           */
/*============================================================================*/

#include "Card_Bound.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Term.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Bound, CK_BOUND)
__aNewFunction(Card_Bound, CN_CARD)
__vDeleteFunction(Card_Bound, CN_CARD)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Bound::Card_Bound(void)
    : Card_symbol(CK_BOUND)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Bound::~Card_Bound(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create a (e|l|u) index bound given PRED                                    */
/*----------------------------------------------------------------------------*/
Card_Bound* Card_Bound::pCreateFrom_PRED(Card_PRED* p_PRED,
                                         char cTblSyn_boundedTo)
{
  _static_

  //p_PRED->vDump();

  Card_Term* p_term;

  Card_Term* pTerm_0 = _pCard_Term(p_PRED->pGetChild(0));
  Card_Term* pTerm_1 = _pCard_Term(p_PRED->pGetChild(1));

  Card_Bound* p_bound = Card_Bound::pBound_New(p_PRED->szSymbol(SF_NONE));

  Symbol xSymbol_idxColOdn(SF_IDX_COL,1);

  if (pTerm_0->pSymbol()->bIs_Col_Tc()) {
    char cTblSyn_term_0 = pTerm_0->pSymbol()->cSF_tblSyn();
    if (cTblSyn_term_0 == cTblSyn_boundedTo) {
      p_term = (new Card_Term) ->SYMPPY(&xSymbol_idxColOdn);
      p_bound->vAppend(p_term);
      p_term = pTerm_1->pClone();
      p_bound->vAppend(p_term);
    }
    else
    if (pTerm_1->pSymbol()->bIs_Col_Tc()) {
      char cTblSyn_term_1 = pTerm_1->pSymbol()->cSF_tblSyn();
      if (cTblSyn_term_1 == cTblSyn_boundedTo) {
        p_term = (new Card_Term) ->SYMPPY(&xSymbol_idxColOdn);
        p_bound->vAppend(p_term);
        p_term = pTerm_0->pClone();
        p_bound->vAppend(p_term);
      } else {
        NEVER_REACH
      }
    }
  }
  else
  if (pTerm_1->pSymbol()->bIs_Col_Tc()) {
    char cTblSyn_term_1 = pTerm_1->pSymbol()->cSF_tblSyn();
    if (cTblSyn_term_1 == cTblSyn_boundedTo) {
      p_term = (new Card_Term) ->SYMPPY(&xSymbol_idxColOdn);
      p_bound->vAppend(p_term);
      p_term = pTerm_0->pClone();
      p_bound->vAppend(p_term);
    }
    else {
      NEVER_REACH
    }
  }
  else {
    NEVER_REACH
  }


  return p_bound;
}

/*----------------------------------------------------------------------------*/
/* create a bound given cmpPred                                               */
/*----------------------------------------------------------------------------*/
Card_Bound* Card_Bound::pBound_New(Stz* szRelnOp)
{
  _static_


  if (szRelnOp->bSame(">"_Stz) || szRelnOp->bSame(">="_Stz)) {
    Card_Bound* p_lBound =  (new Card_Bound) ->SYM(SF_RLN_OP,szRelnOp);
    return p_lBound;
  }

  if (szRelnOp->bSame("<"_Stz) || szRelnOp->bSame("<="_Stz)) {
    Card_Bound* p_uBound =  (new Card_Bound) ->SYM(SF_RLN_OP,szRelnOp);
    return p_uBound;
  }

  if (szRelnOp->bSame("=="_Stz)) {
    Card_Bound* p_eBound =  (new Card_Bound) ->SYM(SF_RLN_OP,szRelnOp);
    return p_eBound;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* test if relnOp is valid for bound                                          */
/*----------------------------------------------------------------------------*/
bool Card_Bound::bValidBoundForRelnOp(Symbol *pBound, Symbol *pRelnOp)
{
  _static_

  if (pBound->bSame("BOUND"_Stz))
    if (pRelnOp->bSame("="_Stz) || pRelnOp->bSame("=="_Stz))
      return true;
    else
      return false;
  else
  if (pBound->bSame("LBOUND"_Stz))
    if (pRelnOp->bSame(">"_Stz) || pRelnOp->bSame(">="_Stz))
      return true;
    else
      return false;
  else
  if (pBound->bSame("UBOUND"_Stz))
    if (pRelnOp->bSame("<="_Stz) || pRelnOp->bSame("<"_Stz))
      return true;
    else
      return false;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this bound logically equivalent to given compare?                       */
/*----------------------------------------------------------------------------*/
bool Card_Bound::bEquivalent(Card_CmpPred* p_cmpPred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* pBnd_rlnOpr = pSymbol();
  Symbol* pBnd_idxOdn = pGetChild(0)->pSymbol(); UNUSED(pBnd_idxOdn);
  Symbol* pBnd_stmOdn = pGetChild(1)->pSymbol();

  Symbol* pCmp_rlnOpr = p_cmpPred->pSymbol();
  Symbol* pCmp_arg_0  = p_cmpPred->pGetChild(0)->pSymbol();
  Symbol* pCmp_arg_1  = p_cmpPred->pGetChild(1)->pSymbol();

  if (! pCmp_rlnOpr->bSame(pBnd_rlnOpr))
    return false;

  if (pBnd_stmOdn->bSame(pCmp_arg_0) ||
      pBnd_stmOdn->bSame(pCmp_arg_1))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Bound::vEnsureTypes_For_stmOdn(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/) {
    xKid.p_term()->vEnsureTypeFor_stmOdn(pCI);
  }

}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Bound::pGetTerm(i8 i_Term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Term* p_term = _pCard_Term(pGetChild((i_Term)));

  return p_term;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Bound::pGetTerm_last(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 i_Term = nChildCards() - 1;
  if (i_Term < 0)
    return nullptr;

  Card_Term* p_term = _pCard_Term(pGetChild((i_Term)));

  return p_term;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Bound::bIsEBound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSymbol()->bSame("=="_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Bound::bIsLBound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSymbol()->bSame(">"_Stz) || pSymbol()->bSame(">="_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Bound::bIsUBound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSymbol()->bSame("<"_Stz) || pSymbol()->bSame("<="_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get $In of ebound                                                          */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Bound::pGet_idxColOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_TERM))
    if (xCI.G.p_term->pSymbol()->bIs_Idx_Col())
      return xCI.G.p_term;

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* get $n or $t.c of ebound                                                   */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Bound::pGet_colOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/)
    if (!xKid.p_term()->pSymbol()->bIs_Idx_Col())
      return xKid.p_term();

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* get $t.c of ebound                                                         */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Bound::pGet_tblColOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/)
    if (xKid.p_term()->bIsColOdn())
      return xKid.p_term();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Bound::bUsesSqyRsl(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/) {
    if (xKid.p_term()->bIsStmOdn() && 1 == xKid.p_term()->iGetOdn())
      return true;
  }

  return false;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Bound::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSymbol()->bIs_Idx_Col())
    return;

  RASSERT(pSymbol()->bIs_Rln_Op() || pSymbol()->bIs_None());

  UNUSED(pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
