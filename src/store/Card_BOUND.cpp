/*============================================================================*/
/* Card_Bound.cpp -- Abstract Bound                                           */
/*============================================================================*/

#include "Card_Bound.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Term.hpp"
#include "Card_LBound.hpp"
#include "Card_UBound.hpp"
#include "Card_EBound.hpp"

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
    : Card(CK_BOUND)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Bound::Card_Bound(eCK eCard)
: Card(eCard)
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
/* create a bound given cmpPred                                               */
/*----------------------------------------------------------------------------*/
Card_Bound* Card_Bound::pBound_CreateFrom_cmpPred(Card_CmpPred* p_cmpPred,
                                                 char cTblSyn)
{
  _static_

  Card_Term* p_term;

  Card_Bound* p_bound = Card_Bound::pBound_New(p_cmpPred->szValue());

  Card_Term* pTerm_0 = _pCard_Term(p_cmpPred->pGetChild(0));
  Card_Term* pTerm_1 = _pCard_Term(p_cmpPred->pGetChild(1));

  if (Card::bIsTableColOrdinal(pTerm_0->szValue())) {
    char cTblSyn_term_0 = Card::cTableColSyn(pTerm_0->szValue());
    if (cTblSyn_term_0 == cTblSyn) {
      p_term = new Card_Term("$I1"_Stz);
      p_bound->vAppend(p_term);
      p_term = new Card_Term(pTerm_1->sz());
      p_bound->vAppend(p_term);
    }
    else
    if (Card::bIsTableColOrdinal(pTerm_1->szValue())) {
      char cTblSyn_term_1 = Card::cTableColSyn(pTerm_1->szValue());
      if (cTblSyn_term_1 == cTblSyn) {
        p_term = new Card_Term("$I1"_Stz);
        p_bound->vAppend(p_term);
        p_term = new Card_Term(pTerm_0->sz());
        p_bound->vAppend(p_term);
      } else {
        NEVER_REACH
      }
    }
  }
  else
  if (Card::bIsTableColOrdinal(pTerm_1->szValue())) {
    char cTblSyn_term_1 = Card::cTableColSyn(pTerm_1->szValue());
    if (cTblSyn_term_1 == cTblSyn) {
      p_term = new Card_Term("$I1"_Stz);
      p_bound->vAppend(p_term);
      p_term = new Card_Term(pTerm_0->sz());
      p_bound->vAppend(p_term);
    }
    else {
      NEVER_REACH
    }
  }
  else {
    NEVER_REACH
  }

  p_term = new Card_Term(p_cmpPred->sz());
  p_bound->vAppend(p_term);

  return p_bound;
}

/*----------------------------------------------------------------------------*/
/* create a bound given pred                                                  */
/*----------------------------------------------------------------------------*/
Card_Bound* Card_Bound::pBound_CreateFrom_PRED(Card_PRED* p_PRED, char cTblSyn)
{
  _static_

  // todo: hack

  Card_CmpPred* p_cmpPred = _pCard_CmpPred(p_PRED);

  Card_Bound* p_bound = Card_Bound::pBound_CreateFrom_cmpPred(p_cmpPred,cTblSyn);

  return p_bound;
}

/*----------------------------------------------------------------------------*/
/* create a bound given cmpPred                                               */
/*----------------------------------------------------------------------------*/
Card_Bound* Card_Bound::pBound_New(Stz* szRelnOp)
{
  _static_

  Stz* szBound = Card::szBound(szRelnOp);
  Card_Term* p_term_bound = new Card_Term(szBound);

  if (szRelnOp->bSame(">"_Stz) || szRelnOp->bSame(">="_Stz)) {
    Card_LBound* p_lBound =  new Card_LBound;
    p_lBound->vAppend(p_term_bound);
    return p_lBound;
  }

  if (szRelnOp->bSame("<"_Stz) || szRelnOp->bSame("<="_Stz)) {
    Card_UBound* p_uBound =  new Card_UBound;
    p_uBound->vAppend(p_term_bound);
    return p_uBound;
  }

  if (szRelnOp->bSame("=="_Stz)) {
    Card_EBound* p_eBound =  new Card_EBound;
    p_eBound->vAppend(p_term_bound);
    return p_eBound;
  }

  NEVER_REACH
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
