/*============================================================================*/
/* Card_Deck.cpp -- Board                                                    */
/*============================================================================*/

#include "Card_Deck.hpp"

#include "Card_OP.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Deck, CK_DECK)
__aNewFunction(Card_Deck, CN_CARD)
__vDeleteFunction(Card_Deck, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Deck::Card_Deck(void)
: Card(CK_DECK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

    
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Deck::~Card_Deck(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add next casts                                                             */
/*----------------------------------------------------------------------------*/
void Card_Deck::vAddNextCasts(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_OP*       p_main_prior     = nullptr;
  Card_Project*  p_project_prior  = nullptr;
  Card_Next*     p_next_prior     = nullptr;
  Card_Combined* p_combined_prior = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  while (pCI->G_bEachMain()) {

    if (CK_INDEXJOIN == pCI->G.e_MainCard ||
        CK_INDEXSCAN == pCI->G.e_MainCard ||
        CK_KEYEXISTS == pCI->G.e_MainCard ||
        CK_KEYNOTEXISTS == pCI->G.e_MainCard ||
        CK_SCAN == pCI->G.e_MainCard) {
      if (p_main_prior) {
        i8 nSkip;
        eXK eCast = pCI->G.pDeck->eAddNextCast(pCI,
                                               p_main_prior, p_project_prior, p_combined_prior, p_next_prior,
                                              /*&*/nSkip);
//        while (nSkip--)
//          pCI->G_vSkipNextMain();
        pCI->G_vMainResynch();
        pCI->vPrintBoard(__LINE__);
        vDebugPoint();
      }
      p_main_prior = pCI->G.p_OP;
      p_project_prior  = nullptr;
      p_combined_prior = nullptr;
      p_next_prior     = nullptr;
    }
    else
    if (CK_PROJECT == pCI->G.e_MainCard) {
      p_project_prior = pCI->G.p_project;
      p_combined_prior = nullptr;
      p_next_prior     = nullptr;
    }
    else
    if (CK_COMBINED == pCI->G.e_MainCard) {
      p_combined_prior = pCI->G.p_combined;
      p_next_prior     = nullptr;
    }
    else
    if (CK_NEXT == pCI->G.e_MainCard) {
      p_next_prior = pCI->G.p_next;
    }

  }

  pCI->vPrintBoard(__LINE__);
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eXK Card_Deck::eAddNextCast(CardIter*      pCI,
                            Card_OP*       p_main_prior,
                            Card_Project*  p_project_prior,
                            Card_Combined* p_combined_prior,
                            Card_Next*     p_next_prior,
                            i8&            rnSkip)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (p_project_prior)
    vDebugPoint();

  Card_Table*     p_table_prior     = p_main_prior->pSub_table();
  Card_TablePtn*  p_tablePtn_prior  = p_table_prior->pSub_tablePtn();

  Card_Table*     p_table_later     = pCI->G.p_MainCard->pSub_table();
  Card_TablePtn*  p_tablePtn_later  = p_table_later->pSub_tablePtn();
  Card_IndexList* p_indexList_later = pCI->G.p_MainCard->pSub_indexList();
  Card_Index*     p_index_later     = p_indexList_later->pFindChosenIndex();
  Card_Bounds*    p_bounds_later    = p_index_later->pSub_bounds();

  Card_Hash* p_hash_prior_new;
  Card_Term* p_term_tblCol_to_be_hashed;

  eXK eNextCast = p_bounds_later->pSub_bound()->eNextCast(p_tablePtn_prior,
                                                          p_tablePtn_later,
                                                          p_index_later,
                                                          /*&*/p_term_tblCol_to_be_hashed);
  switch (eNextCast) {
    case XK_BROADCAST:
      p_next_prior->vValue("broadcast"_Stz);

      if (! p_project_prior) {
        rnSkip = 0;
        return XK_BROADCAST;
      }

      if (p_next_prior) {
        if (! p_combined_prior) {
          Card_Combined *p_combined = new Card_Combined;
          pCI->G_vInsertBefore(p_combined, p_next_prior);
        }
        rnSkip = 1;
        return XK_BROADCAST;
      }
      else
        NEVER_REACH

    case XK_HASHCAST:
      p_next_prior->vValue("hashcast"_Stz);

      pCI->G.p_expr = p_term_tblCol_to_be_hashed->pExpr_For_col(pCI, pCI->G.i_MainCard);
      p_hash_prior_new = Card_Hash::pDerive(pCI->G.p_expr);

      if (p_combined_prior) {
        pCI->G_vInsertBefore(p_hash_prior_new, p_combined_prior);
        rnSkip = 1;
        return XK_HASHCAST;
      }
      else
      if (p_next_prior) {
        pCI->G_vInsertBefore(p_hash_prior_new, p_next_prior);
        Card_Combined* p_combined = new Card_Combined;
        pCI->G_vInsertBefore(p_combined, p_next_prior);
        rnSkip = 2;
        return XK_HASHCAST;
      }
      else
        NEVER_REACH

    case XK_UNICAST:
      rnSkip = 0;
      return XK_UNICAST;

    default:
      NEVER_REACH
  }

  NEVER_REACH
}



/*----------------------------------------------------------------------------*/
/* find i_fold (reverse)                                                      */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Deck::p_expr_fold(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Projection* p_projection_fold = this-> p_projection_fold())
    return p_projection_fold->pSub_expr();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find i_fold (reverse)                                                      */
/*----------------------------------------------------------------------------*/
i8 Card_Deck::i_Fold(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nChildren = nChildCards();
  for (i8 i_Main = nChildren - 1; i_Main >= 0; i_Main--) {
    Card *pMain = pGetChild(i_Main);
    if (pMain->bIsCard(CK_FOLD))
      return i_Main;
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* find p_fold (reverse)                                                      */
/*----------------------------------------------------------------------------*/
Card_Fold* Card_Deck::p_fold(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nChildren = nChildCards();
  for (i8 i_Main = nChildren - 1; i_Main >= 0; i_Main--) {
    Card *pMain = pGetChild(i_Main);
    if (pMain->bIsCard(CK_FOLD))
      return _pCard_Fold(pMain);
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find project before fold (reverse)                                         */
/*----------------------------------------------------------------------------*/
Card_Projection* Card_Deck::p_projection_fold(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 i_Fold = this->i_Fold();
  for (i8 i_Main = i_Fold - 1; i_Main >= 0; i_Main--) {
    Card *pMain = pGetChild(i_Main);
    if (Card_Projection* p_projection = pMain->pSub_projection())
      return p_projection;
  }

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* initial projection analysis                                                */
/*----------------------------------------------------------------------------*/
void Card_Deck::vProjectionAnalysis(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->vRestart();
  while (pCI->G_bEachMain(CK_TABLE))
    if ((pCI->G.p_projection = pCI->G.p_table->pSub_projection()))
      pCI->G.p_table->vRemoveDeleteChild(pCI->G.p_projection);

  pCI->vRestart();
  pCI->Analysis.m_pProjection = pCI->G.p_projection = nullptr;

  while (pCI->G_bEachMain()) {


    if (CK_TABLE == pCI->G.e_MainCard) {
      pCI->G.p_table->vUpdate_projection(pCI);
      continue;
    }

    if (CK_WHERE == pCI->G.e_MainCard || CK_HAVING == pCI->G.e_MainCard) {
      if ((pCI->G.p_rpnCalc = pCI->G.p_MainCard->pSub_rpnCalc()))
        pCI->G.p_rpnCalc->vTypes_For_stmColOdn(pCI);
      continue;
    }

    if (33 == pCI->G.p_MainCard->iCardNo())
      vDebugPoint();

    if ((pCI->G.p_key = pCI->G.p_MainCard->pSub_key())) {
      if (pCI->Analysis.m_pProjection) {
        pCI->G.p_key->vTypes_For_stmColOdn(pCI);
        pCI->G.p_key->vRenumberAllExpr();
      }
    }

    if ((pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection())) {

      if (pCI->Analysis.m_pProjection) {
        pCI->G.p_projection->vTypes_For_stmColOdn(pCI);
        pCI->G.p_projection->vRenumberAllExpr();
      }

      pCI->Analysis.m_pProjection = pCI->G.p_MainCard->pSub_projection();
    }

  }
}

/*----------------------------------------------------------------------------*/
/* update projections after some change                                       */
/*----------------------------------------------------------------------------*/
void Card_Deck::vProjectionUpdate(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->vRestart();
  while (pCI->G_bEachMain(CK_TABLE))
    if ((pCI->G.p_projection = pCI->G.p_table->pSub_projection()))
      pCI->G.p_table->vRemoveDeleteChild(pCI->G.p_projection);

  bool bSawFold = false;

  pCI->vRestart();
  pCI->Analysis.m_pProjection = pCI->G.p_projection = nullptr;

  while (pCI->G_bEachMain()) {

    if (CK_FOLD == pCI->G.e_MainCard) {
      bSawFold = true;
      continue;
    }

    if (CK_TABLE == pCI->G.e_MainCard) {
      pCI->G.p_table->vUpdate_projection(pCI);
    }
    else if (CK_PROJECT == pCI->G.e_MainCard) {
      if (pCI->G.p_MainCard->pSub_needed()) {
        pCI->G.p_project->Card_Project::vUpdate_projection(pCI);
      }
    }
    else if (CK_AGGREG == pCI->G.e_MainCard) {
      pCI->G.p_aggreg->Card_Aggreg::vUpdate_projection(pCI);
    }
    else if (CK_GBYAGG == pCI->G.e_MainCard) {
      pCI->G.p_gbyAgg->Card_GbyAgg::vUpdate_projection(pCI);
    }
    else if (CK_ORDAGG == pCI->G.e_MainCard) {
      pCI->G.p_ordAgg->Card_OrdAgg::vUpdate_projection(pCI);
    }
    else if (pCI->G.p_MainCard->pSub_projection()) {
      vTrace("UPDATE? %s"_Stz,pCI->G.p_MainCard->szCardNm());
      vDebugPoint();
    }

    if (pCI->G.p_MainCard->pSub_projection()) {
      pCI->Analysis.m_pProjection = pCI->G.p_MainCard->pSub_projection();
    }

  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
