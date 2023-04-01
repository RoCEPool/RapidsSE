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
    : Card(CK_BOUND)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Bound::Card_Bound(Stz* szRelnOp)
    : Card(CK_BOUND)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vValue(szRelnOp);

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

  p_PRED->vDump();

  Card_Term* p_term;

  Card_Term* pTerm_0 = _pCard_Term(p_PRED->pGetChild(0));
  Card_Term* pTerm_1 = _pCard_Term(p_PRED->pGetChild(1));

  Card_Bound* p_bound = Card_Bound::pBound_New(p_PRED->szValue());

  if (SymVal::bIsTableColOrdinal(pTerm_0->szValue())) {
    char cTblSyn_term_0 = Card::cTableColSyn(pTerm_0->szValue());
    if (cTblSyn_term_0 == cTblSyn_boundedTo) {
      p_term = new Card_Term("$I1"_Stz);
      p_bound->vAppend(p_term);
      p_term = pTerm_1->pClone();
      p_bound->vAppend(p_term);
    }
    else
    if (SymVal::bIsTableColOrdinal(pTerm_1->szValue())) {
      char cTblSyn_term_1 = Card::cTableColSyn(pTerm_1->szValue());
      if (cTblSyn_term_1 == cTblSyn_boundedTo) {
        p_term = new Card_Term("$I1"_Stz);
        p_bound->vAppend(p_term);
        p_term = pTerm_0->pClone();
        p_bound->vAppend(p_term);
      } else {
        NEVER_REACH
      }
    }
  }
  else
  if (SymVal::bIsTableColOrdinal(pTerm_1->szValue())) {
    char cTblSyn_term_1 = Card::cTableColSyn(pTerm_1->szValue());
    if (cTblSyn_term_1 == cTblSyn_boundedTo) {
      p_term = new Card_Term("$I1"_Stz);
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
    Card_Bound* p_lBound =  new Card_Bound(szRelnOp);
    p_lBound->vValue(szRelnOp);
    //p_lBound->vAppend(p_term_bound);
    return p_lBound;
  }

  if (szRelnOp->bSame("<"_Stz) || szRelnOp->bSame("<="_Stz)) {
    Card_Bound* p_uBound =  new Card_Bound(szRelnOp);
    p_uBound->vValue(szRelnOp);
    //p_uBound->vAppend(p_term_bound);
    return p_uBound;
  }

  if (szRelnOp->bSame("=="_Stz)) {
    Card_Bound* p_eBound =  new Card_Bound(szRelnOp);
    p_eBound->vValue(szRelnOp);
    //p_eBound->vAppend(p_term_bound);
    return p_eBound;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* determine the cast for the prior p_next                                    */
/*----------------------------------------------------------------------------*/
eXK Card_Bound::eNextCast(Card_TablePtn* p_tablePtn_prior,
                          Card_TablePtn* p_tablePtn_later,
                          Card_Index*    p_index_later,
                          Card_Term*&    rp_term_tblCol_to_be_hashed)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_IdxColList* p_idxColList_later = p_index_later->pSub_idxColList();
  Card_Term*       p_term_later_idxColOdn = pGet_idxColOdn();
  Card_IdxCol *    p_idxCol_later =
                       p_idxColList_later->pGetIdxCol_byIdxColOdn(p_term_later_idxColOdn->szValue());

  if (p_idxCol_later && p_idxCol_later->bIsReplicated()) {
    // + later table is replicated so always unicast
    p_idxCol_later->vDump();
    vDebugPoint();
    return XK_UNICAST;
  }

  Card_Term* p_term_prior_tblColOdn = pGet_tblColOdn();
  if (! p_term_prior_tblColOdn)
    // - no prior table column (referenced in this bound) -- probably cross join
    return XK_BROADCAST;

  if (p_idxCol_later->bSameValue(p_tablePtn_later)) {
    // + later indexed column (referenced in this bound) is later table ptn
    if (p_term_prior_tblColOdn->bSameValue(p_tablePtn_prior)) {
      // + prior table column (referenced in this bound) is prior table ptn
      return XK_UNICAST;
    }
    else {
      // - prior table column (referenced in this bound) is not prior table ptn
      //     but the prior table column can be used for hash cast
      rp_term_tblCol_to_be_hashed = p_term_prior_tblColOdn;
      return XK_HASHCAST;
    }
  }
  else {
    // - later indexed column in this bound is not later table ptn
    return XK_BROADCAST;
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
bool Card_Bound::bIsEBound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (bIsValue("=="_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Bound::bIsLBound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (bIsValue(">"_Stz) || bIsValue(">="_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Bound::bIsUBound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (bIsValue("<"_Stz) || bIsValue("<="_Stz))
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
    if (Card::bIsIndexColOrdinal(xCI.G.p_term->szValue()))
      return xCI.G.p_term;

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* get $In of ebound                                                          */
/*----------------------------------------------------------------------------*/
Stz* Card_Bound::szGet_idxColOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_TERM))
    if (Card::bIsIndexColOrdinal(xCI.G.p_term->szValue()))
      return xCI.G.p_term->szValue();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get $t.c of ebound                                                         */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Bound::pGet_tblColOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/)
    if (xKid.p_term()->bIsTblColOdn())
      return xKid.p_term();

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* get $t.c of ebound                                                         */
/*----------------------------------------------------------------------------*/
Stz* Card_Bound::szGet_tblColOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_TERM))
    if (SymVal::bIsTableColOrdinal(xCI.G.p_term->szValue()))
      return xCI.G.p_term->szValue();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Bound::vFlipTerms(Card_Projection* p_projection_reduced)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_TERM))
    xCI.G.p_term->vFlip(p_projection_reduced);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
