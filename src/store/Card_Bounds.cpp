/*============================================================================*/
/* Card_Bounds.cpp -- Join Predicate                                        */
/*============================================================================*/

#include "Card_Bound.hpp"
#include "Card_Bounds.hpp"
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eXK Card_Bounds::eCastKind_v2(Card_Table* p_table_prior,
                              Card_Table* p_table_curr,
                              Card_Index* p_index_curr,
                              Card_Term*& rp_term_to_be_hashed)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_IdxColList* p_idxColList = p_index_curr->pSub_idxColList();
  Card_TablePtn*   p_tablePtn_l = p_table_prior->pSub_tablePtn();
  Card_TablePtn*   p_tablePtn_r = p_table_curr->pSub_tablePtn();

  //p_idxColList->vDump();
  //p_tablePtn_l->vDump();
  //p_tablePtn_r->vDump();


  Card_Bound *p_ebound = pSub_bound();

  Card_Term* pIdxColOdn_inBounds = p_ebound->pGet_idxColOdn();
  Card_IdxCol *p_idxCol_inBounds =
      p_idxColList->pIdxCol_byIdxColOdn(pIdxColOdn_inBounds->szValue());

  Card_Term* pTblColOdn_inBounds = p_ebound->pGet_tblColOdn();
  rp_term_to_be_hashed = pTblColOdn_inBounds;

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
  while (xCI_bounds.G_bEachMain()) {
    if (xCI_bounds.G.p_MainCard->cMark1() != cWithMark)
      continue;

    vRemoveChild(xCI_bounds.G.p_MainCard);
    DELETE_CARD(xCI_bounds.G.p_MainCard);
    xCI_bounds.vRewind();
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Bounds::vFlipTerms(Card_Projection* p_projection_reduced)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_BOUND))
    xCI.G.p_bound->vFlipTerms(p_projection_reduced);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
