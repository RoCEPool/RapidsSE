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

  Card_BOUND* p_bound;
  Card_CmpPred* p_cmpPred;

  Card_Bounds* p_bounds = new Card_Bounds;

  if (p_PRED->bIsCard(CK_CMPPRED)) {
    p_bound = Card_BOUND::pCreateFrom_PRED(p_PRED, cTblSyn);
    p_bounds->vAppend(p_bound);
  }
  else
  if (p_PRED->bIsCard(CK_ANDPRED)) {
    p_cmpPred = _pCard_CmpPred(p_PRED->pGetChild(0));
    p_bound = Card_BOUND::pCreateFrom_PRED(p_cmpPred, cTblSyn);
    p_bounds->vAppend(p_bound);

    p_cmpPred = _pCard_CmpPred(p_PRED->pGetChild(1));
    p_bound = Card_BOUND::pCreateFrom_PRED(p_cmpPred, cTblSyn);
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

  Card_EBound* p_eBound = new Card_EBound;
  p_bounds->vAppend(p_eBound);

  Card_Term* p_term = new Card_Term("BOUND"_Stz);
  p_eBound->vAppend(p_term);


  p_term = new Card_Term("$I1"_Stz);
  p_eBound->vAppend(p_term);
  p_term = new Card_Term(szTblColOdn);
  p_eBound->vAppend(p_term);
  p_term = new Card_Term("=="_Stz);
  p_eBound->vAppend(p_term);

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
                           Card_Term*& rp_left_column_to_be_hashed)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_IdxColList* p_idxColList = p_index_r->pSub_idxColList();
  Card_TablePtn* p_tablePtn_l = p_table_l->pSub_tablePtn();
  Card_TablePtn* p_tablePtn_r = p_table_r->pSub_tablePtn();

  p_idxColList->vDump();
  p_tablePtn_l->vDump();
  p_tablePtn_r->vDump();

  rp_left_column_to_be_hashed = nullptr;

  CardIter xCI_bounds(this);
  while (Card_EBound* p_ebound = _pCard_EBound(xCI_bounds.pEachChild(CK_EBOUND))) {

    bool b_EBound_Index_Colummn_Matches_PtnOf_Right_Table = false;
    bool b_EBound_Left_Column_Matches_PtnOf_Left_Table    = false;

    CardIter xCI_bound(p_ebound);
    while (Card_Term* p_term_bound = _pCard_Term(xCI_bound.pEachChild(CK_TERM))) {

      p_term_bound->vDump();

      if (Card::bIsIndexColOrdinal(p_term_bound->szValue())) {
        //+ p_term_bound is the indexed right column ordinal in the ebound

        // decode the index column ordinal to ultimately get table-column ordinal
        Card_IdxCol* p_idxCol_r = p_idxColList->pIdxCol_byIdxColOdn(p_term_bound->szValue());
        if (! p_idxCol_r)
          continue;

        //vTrace("p_idxCol_r %s tblptn_l %s tblptn_r %s\n"_Stz,
        //       p_idxCol_r->szValue(),p_tablePtn_l->szValue(),p_tablePtn_r->szValue());

        if (p_tablePtn_r->bSame(p_idxCol_r->szValue()))
          b_EBound_Index_Colummn_Matches_PtnOf_Right_Table = true;

      }
      else
      if (Card::bIsTableColOrdinal(p_term_bound->szValue())) {
        //+ p_term_bound is the left column as it appears in the ebound

        rp_left_column_to_be_hashed = p_term_bound;

        if (p_tablePtn_l->bSame(p_term_bound->szValue()))
          b_EBound_Left_Column_Matches_PtnOf_Left_Table = true;

      }

    }

    if (b_EBound_Left_Column_Matches_PtnOf_Left_Table)
      if (b_EBound_Index_Colummn_Matches_PtnOf_Right_Table)
        return XK_UNICAST;
      else
        continue;
    else
    if (b_EBound_Index_Colummn_Matches_PtnOf_Right_Table) {
      RASSERT(rp_left_column_to_be_hashed);
      return XK_HASHCAST;
    }
    else
      continue;
  }

  return XK_BROADCAST;
}

/*----------------------------------------------------------------------------*/
eXK Card_Bounds::eCastKind_v2(Card_Index* p_index_r,
                              Card_Table* p_table_l, Card_Table* p_table_r,
                              Card_Term*& rp_left_column_to_be_hashed)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_IdxColList* p_idxColList = p_index_r->pSub_idxColList();
  Card_TablePtn*   p_tablePtn_l = p_table_l->pSub_tablePtn();
  Card_TablePtn*   p_tablePtn_r = p_table_r->pSub_tablePtn();

  p_idxColList->vDump();
  p_tablePtn_l->vDump();
  p_tablePtn_r->vDump();


  Card_EBound *p_ebound = pSub_eBound();

  Card_Term* pIdxColOdn_inBounds = p_ebound->pGet_idxColOdn();
  Card_IdxCol *p_idxCol_inBounds =
      p_idxColList->pIdxCol_byIdxColOdn(pIdxColOdn_inBounds->szValue());

  Card_Term* pTblColOdn_inBounds = p_ebound->pGet_tblColOdn();
  rp_left_column_to_be_hashed = pTblColOdn_inBounds;

  if (! pTblColOdn_inBounds)
    return XK_BROADCAST;

  if (p_idxCol_inBounds->bSameValue(p_tablePtn_r))
    if (pTblColOdn_inBounds->bSameValue(p_tablePtn_l))
      return XK_UNICAST;
    else
      return XK_HASHCAST;
  else
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
