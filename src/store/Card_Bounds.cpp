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
eXK Card_Bounds::eCastKind(Card_TablePtn* p_tablePtn_prior,
                           Card_TablePtn* p_tablePtn_later,
                           Card_Index*    p_index_later,
                           Card_Term*&    rp_term_tblCol_to_be_hashed)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_IdxColList* p_idxColList_later = p_index_later->pSub_idxColList();

  Card_Bound* p_bound = pSub_bound();
  Card_Term*  p_term_tblColOdn_bound = p_bound->pGet_tblColOdn();
  Card_Term*  p_term_idxColOdn_bound = p_bound->pGet_idxColOdn();

  Card_IdxCol *p_idxCol_bound =
      p_idxColList_later->pGetIdxCol_byIdxColOdn(p_term_idxColOdn_bound->szValue());

  if (! p_term_tblColOdn_bound)
    return XK_BROADCAST;

  if (p_idxCol_bound->bSameValue(p_tablePtn_later))
    if (p_term_tblColOdn_bound->bSameValue(p_tablePtn_prior))
      return XK_UNICAST;
    else {
      rp_term_tblCol_to_be_hashed = p_term_tblColOdn_bound;
      return XK_HASHCAST;
    }
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
