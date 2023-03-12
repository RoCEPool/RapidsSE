/*============================================================================*/
/* Card_BOUND.cpp -- Abstract Bound                                           */
/*============================================================================*/

#include "Card_BOUND.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Term.hpp"
#include "Card_LBound.hpp"
#include "Card_UBound.hpp"
#include "Card_EBound.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_BOUND, CK_BOUND)
__aNewFunction(Card_BOUND, CN_CARD)
__vDeleteFunction(Card_BOUND, CN_CARD)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_BOUND::Card_BOUND(void)
    : Card(CK_BOUND)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_BOUND::Card_BOUND(eCK eCard)
: Card(eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_BOUND::~Card_BOUND(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create a (e|l|u) index bound given PRED                                    */
/*----------------------------------------------------------------------------*/
Card_BOUND* Card_BOUND::pCreateFrom_PRED(Card_PRED* p_PRED,
                                         char cTblSyn_boundedTo)
{
  _static_

  Card_Term* p_term;

  Card_Term* pTerm_0 = _pCard_Term(p_PRED->pGetChild(0));
  Card_Term* pTerm_1 = _pCard_Term(p_PRED->pGetChild(1));

  Card_BOUND* p_bound = Card_BOUND::pBound_New(p_PRED->szValue());

  if (Card::bIsTableColOrdinal(pTerm_0->szValue())) {
    char cTblSyn_term_0 = Card::cTableColSyn(pTerm_0->szValue());
    if (cTblSyn_term_0 == cTblSyn_boundedTo) {
      p_term = new Card_Term("$I1"_Stz);
      p_bound->vAppend(p_term);
      p_term = new Card_Term(pTerm_1->sz());
      p_bound->vAppend(p_term);
    }
    else
    if (Card::bIsTableColOrdinal(pTerm_1->szValue())) {
      char cTblSyn_term_1 = Card::cTableColSyn(pTerm_1->szValue());
      if (cTblSyn_term_1 == cTblSyn_boundedTo) {
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
    if (cTblSyn_term_1 == cTblSyn_boundedTo) {
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

  p_term = new Card_Term(p_PRED->sz());
  p_bound->vAppend(p_term);

  return p_bound;
}

/*----------------------------------------------------------------------------*/
/* create a bound given cmpPred                                               */
/*----------------------------------------------------------------------------*/
Card_BOUND* Card_BOUND::pBound_New(Stz* szRelnOp)
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

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Card_BOUND::pGetTerm(i8 i_Term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Term* p_term = _pCard_Term(pGetChild((i_Term)));

  return p_term;
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
