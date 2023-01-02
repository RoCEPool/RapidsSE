/*============================================================================*/
/* Card_Bounds.cpp -- Join Predicate                                        */
/*============================================================================*/

#include "Card_Bounds.hpp"

#include "Card_EBound.hpp"
#include "Card_IdxCol.hpp"
#include "Card_IdxColList.hpp"
#include "Card_Index.hpp"
#include "Card_Table.hpp"
#include "Card_TablePtn.hpp"
#include "Card_Term.hpp"
#include "Card_PRED.hpp"
#include "CardIter.hpp"
#include "Card_CmpPred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Bounds, CK_BOUNDS)
__aNewFunction(Card_Bounds, CN_CARD)
__vDeleteFunction(Card_Bounds, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Bounds::Card_Bounds(void)
: Card(CK_BOUNDS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Bounds::~Card_Bounds(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create a bounds given tblColOdn                                            */
/*----------------------------------------------------------------------------*/
Card_Bounds* Card_Bounds::pBounds_CreateFrom_PRED(Card_PRED* p_PRED,
                                                char cTblSyn)
{
  _static_

  Card_Bound* p_bound;
  Card_CmpPred* p_cmpPred;

  Card_Bounds* p_bounds = new Card_Bounds;

  if (p_PRED->bIsCard(CK_CMPPRED)) {
    p_bound = Card_Bound::pBound_CreateFrom_PRED(p_PRED,cTblSyn);
    p_bounds->vAppend(p_bound);
  }
  else
  if (p_PRED->bIsCard(CK_ANDPRED)) {
    p_cmpPred = _pCard_CmpPred(p_PRED->pGetChild(0));
    p_bound = Card_Bound::pBound_CreateFrom_PRED(p_cmpPred,cTblSyn);
    p_bounds->vAppend(p_bound);

    p_cmpPred = _pCard_CmpPred(p_PRED->pGetChild(1));
    p_bound = Card_Bound::pBound_CreateFrom_PRED(p_cmpPred,cTblSyn);
    p_bounds->vAppend(p_bound);
  }
  else {
    NEVER_REACH
  }

  return p_bounds;
}

/*----------------------------------------------------------------------------*/
/* create a bounds given tblColOdn                                            */
/*----------------------------------------------------------------------------*/
Card_Bounds* Card_Bounds::pBounds_Create_Eq_ForGivenCol(Stz* szTblColOdn)
{
  _static_

  Card_Bounds* p_bounds = new Card_Bounds;

  Card_Term* p_term = new Card_Term("BOUND"_Stz);
  p_bounds->vAppend(p_term);

  p_term = new Card_Term("$I1"_Stz);
  p_bounds->vAppend(p_term);
  p_term = new Card_Term(szTblColOdn);
  p_bounds->vAppend(p_term);
  p_term = new Card_Term("=="_Stz);
  p_bounds->vAppend(p_term);

  return p_bounds;
}

/*----------------------------------------------------------------------------*/
/* create a bounds given tblColOdn                                            */
/*----------------------------------------------------------------------------*/
Card_Bounds* Card_Bounds::pBounds_CreateFrom_cmpPred(Card_CmpPred* p_cmpPred,
                                                   char cTblSyn)
{
  _static_

  Card_Term* p_term;
  Stz* szBound;

  Card_Bounds* p_bounds = new Card_Bounds;


  Card_Term* pTerm_0 = _pCard_Term(p_cmpPred->pGetChild(0));
  Card_Term* pTerm_1 = _pCard_Term(p_cmpPred->pGetChild(1));

  if (Card::bIsTableColOrdinal(pTerm_0->szValue())) {
    char cTblSyn_term_0 = Card::cTableColSyn(pTerm_0->szValue());
    if (cTblSyn_term_0 == cTblSyn) {
      szBound = Card::szBound(pTerm_1->sz());
      p_term = new Card_Term(szBound);
      p_bounds->vAppend(p_term);
      p_term = new Card_Term("$I1"_Stz);
      p_bounds->vAppend(p_term);
      p_term = new Card_Term(pTerm_1->sz());
      p_bounds->vAppend(p_term);
    }
    else
    if (Card::bIsTableColOrdinal(pTerm_1->szValue())) {
      char cTblSyn_term_1 = Card::cTableColSyn(pTerm_0->szValue());
      if (cTblSyn_term_1 == cTblSyn) {
        szBound = Card::szBound(pTerm_1->sz());
        p_term = new Card_Term(szBound);
        p_bounds->vAppend(p_term);
        p_term = new Card_Term("$I1"_Stz);
        p_bounds->vAppend(p_term);
        p_term = new Card_Term(pTerm_0->sz());
        p_bounds->vAppend(p_term);
      } else {
        NEVER_REACH
      }
    }
  }
  else
  if (Card::bIsTableColOrdinal(pTerm_1->szValue())) {
    char cTblSyn_term_1 = Card::cTableColSyn(pTerm_1->szValue());
    if (cTblSyn_term_1 == cTblSyn) {
      szBound = Card::szBound(pTerm_1->sz());
      p_term = new Card_Term(szBound);
      p_bounds->vAppend(p_term);
      p_term = new Card_Term("$I1"_Stz);
      p_bounds->vAppend(p_term);
      p_term = new Card_Term(pTerm_0->sz());
      p_bounds->vAppend(p_term);
    }
    else {
      NEVER_REACH
    }
  }
  else {
    NEVER_REACH
  }

  p_term = new Card_Term(p_cmpPred->sz());
  p_bounds->vAppend(p_term);

  return p_bounds;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eXK Card_Bounds::eCastKind(Card_Index* p_index_r,
                           Card_Table* p_table_l, Card_Table* p_table_r,
                           Card_Term*& rp_term_hash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_IdxColList* p_idxColList = p_index_r->pSub_idxColList();
  Card_TablePtn* p_tablePtn_l = p_table_l->pSub_tablePtn();
  Card_TablePtn* p_tablePtn_r = p_table_r->pSub_tablePtn();

  rp_term_hash = nullptr;

  CardIter xCI_bounds(this);
  while (Card_EBound* p_ebound = _pCard_EBound(xCI_bounds.pEachChild(CK_EBOUND))) {

    bool bMatch_l = false;
    bool bMatch_r = false;
    CardIter xCI_bound(p_ebound);
    while (Card_Term* p_term_bound = _pCard_Term(xCI_bound.pEachChild(CK_TERM))) {

      if (Card::bIsIndexColOrdinal(p_term_bound->szValue())) {
        Card_IdxCol* p_idxCol = p_idxColList->pIdxCol_byIdxColOdn(p_term_bound->szValue());
        if (! p_idxCol)
          continue;

        vTrace("p_idxCol %s tblptn_l %s tblptn_r %s\n"_Stz,
               p_idxCol->szValue(),p_tablePtn_l->szValue(),p_tablePtn_r->szValue());

        if (p_tablePtn_r->bSame(p_idxCol->szValue())) {
          bMatch_r = true;
        }

      }
      else
      if (Card::bIsTableColOrdinal(p_term_bound->szValue())) {

        if (p_tablePtn_l->bSame(p_term_bound->szValue())) {
          bMatch_l = true;
          rp_term_hash = p_term_bound;
        }

      }

    }

    if (bMatch_l) {
      if (bMatch_r)
        return XK_UNICAST;
      else
        return XK_HASHCAST;
    }
  }

  return XK_BROADCAST;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Bounds::vCullBound(char cWithMark)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI_bounds(this);
  while (xCI_bounds.bEachChild()) {
    if (xCI_bounds.G.m_pChild->cMark1() != cWithMark)
      continue;

    vRemoveChild(xCI_bounds.G.m_pChild);
    DELETE_CARD(xCI_bounds.G.m_pChild);
    xCI_bounds.vRewind();
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
