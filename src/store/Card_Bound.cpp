/*============================================================================*/
/* Card_Bound.cpp -- Abstract Bound                                           */
/*============================================================================*/

#include "Card_Bound.hpp"
#include "Card_RlnConj.hpp"
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
    : Card_symbol(CK_BOUND, C_UNKNOWN)
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
Card_Bound* Card_Bound::pCreateFrom_rlnConj(Card_RlnConj *p_rlnConj,
                                            char cTblSyn_boundedTo)
{
  _static_

  //p_CONJ->vDump();

  Card_Term* p_term;

  Card_Term* pTerm_0 = _pCard_Term(p_rlnConj->pGetChild(0));
  Card_Term* pTerm_1 = _pCard_Term(p_rlnConj->pGetChild(1));

  Card_Bound* p_bound = Card_Bound::pBound_New(p_rlnConj->szFacet(SF_RLN_OP));

  Symbol xSymbol_idxCol(SF_IDX_COL,1);

  if (pTerm_0->pSymbol()->bHasFacet(SF_TBL_COL)) {
    char cTblSyn_term_0 = pTerm_0->pSymbol()->cTblSyn();
    if (cTblSyn_term_0 == cTblSyn_boundedTo) {
      p_term = (new Card_Term) ->SYMPPY(&xSymbol_idxCol);
      p_bound->vAppendChild(p_term);
      p_term = pTerm_1->pClone();
      p_bound->vAppendChild(p_term);
    }
    else
    if (pTerm_1->pSymbol()->bHasFacet(SF_TBL_COL)) {
      char cTblSyn_term_1 = pTerm_1->pSymbol()->cTblSyn();
      if (cTblSyn_term_1 == cTblSyn_boundedTo) {
        p_term = (new Card_Term) ->SYMPPY(&xSymbol_idxCol);
        p_bound->vAppendChild(p_term);
        p_term = pTerm_0->pClone();
        p_bound->vAppendChild(p_term);
      } else {
        NEVER_REACH
      }
    }
  }
  else
  if (pTerm_1->pSymbol()->bHasFacet(SF_TBL_COL)) {
    char cTblSyn_term_1 = pTerm_1->pSymbol()->cTblSyn();
    if (cTblSyn_term_1 == cTblSyn_boundedTo) {
      p_term = (new Card_Term) ->SYMPPY(&xSymbol_idxCol);
      p_bound->vAppendChild(p_term);
      p_term = pTerm_0->pClone();
      p_bound->vAppendChild(p_term);
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

  if (pBound->bHasFacet("BOUND"_Stz))
    if (pRelnOp->bHasFacet("="_Stz) || pRelnOp->bHasFacet("=="_Stz))
      return true;
    else
      return false;
  else
  if (pBound->bHasFacet("LBOUND"_Stz))
    if (pRelnOp->bHasFacet(">"_Stz) || pRelnOp->bHasFacet(">="_Stz))
      return true;
    else
      return false;
  else
  if (pBound->bHasFacet("UBOUND"_Stz))
    if (pRelnOp->bHasFacet("<="_Stz) || pRelnOp->bHasFacet("<"_Stz))
      return true;
    else
      return false;
  else
    return false;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert tblCol to stmCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_Bound::vX_tblCol_To_expCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_TERM)) {
    if (!xKid.p_term()->pSymbol()->bHasFacet(SF_TBL_COL))
      continue;

    xKid.p_term()->vX_tblCol_To_expCol(pCI);
  }
}

/*----------------------------------------------------------------------------*/
/* is this bound logically equivalent to given compare?                       */
/*----------------------------------------------------------------------------*/
bool Card_Bound::bEquivalent(Card_RlnConj* p_rlnConj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* pBnd_rlnOpr = pSymbol();
  Symbol* pBnd_idxCol = pGetChild(0)->pSymbol(); UNUSED(pBnd_idxCol);
  Symbol* pBnd_expCol = pGetChild(1)->pSymbol();

  Symbol* pCmp_rlnOpr = p_rlnConj->pSymbol();
  Symbol* pCmp_arg_0  = p_rlnConj->pGetChild(0)->pSymbol();
  Symbol* pCmp_arg_1  = p_rlnConj->pGetChild(1)->pSymbol();

  if (!pCmp_rlnOpr->bMatches(pBnd_rlnOpr))
    return false;

  if (pBnd_expCol->bMatches(pCmp_arg_0) ||
      pBnd_expCol->bMatches(pCmp_arg_1))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Bound::vApplyCast(CardIter *pCI, Card_Emit* p_Elist_ref)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Cast xCast_last;
  xCast_last.m_xTypeId = TypeId();
  xCast_last.m_nTypeLength = 0;
  xCast_last.m_nTypeScale = 0;
  RASSERT(! xCast_last.m_xTypeId.bIsValid());

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/) {
    xKid.p_term()->vApplyCast(pCI, /*??*/p_Elist_ref, xCast_last);
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

  if (pSymbol()->bHasFacet("=="_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Bound::bIsLBound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSymbol()->bHasFacet(">"_Stz) || pSymbol()->bHasFacet(">="_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Bound::bIsUBound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSymbol()->bHasFacet("<"_Stz) || pSymbol()->bHasFacet("<="_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get $In of ebound                                                          */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Bound::pGet_idxCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_TERM))
    if (xKid.p_term()->pSymbol()->bHasFacet(SF_IDX_COL))
      return xKid.p_term();

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* get $n or $t.c of ebound                                                   */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Bound::pGet_IfNot_idxCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/)
    if (!xKid.p_term()->pSymbol()->bHasFacet(SF_IDX_COL))
      return xKid.p_term();

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* get $t.c of ebound                                                         */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Bound::pGet_tblCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/)
    if (xKid.p_term()->pSymbol()->bHasFacet(SF_TBL_COL))
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
    if (xKid.p_term()->pSymbol()->bHasFacet(SF_EXP_COL) &&
        1 == xKid.p_term()->pSymbol()->iTo_ordinal(SF_EXP_COL))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Bound::vShortForm(String* pString_b, Stz* szSep)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/) {
    xKid.p_term()->vShortForm(pString_b,szSep);
    szSep = " "_Stz;
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Bound::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSymbol()->bHasFacet(SF_IDX_COL)) {
    Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
    vValidationPassed();
    return;
  }

  RASSERT(pSymbol()->bHasFacet(SF_RLN_OP) || ! pSymbol()->nFacets());

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
