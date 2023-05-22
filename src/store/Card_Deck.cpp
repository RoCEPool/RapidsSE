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
void Card_Deck::vAddStageCasts(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_OP*       p_OP_prev        = nullptr;
  Card_Project*  p_project_prior  = nullptr;
  Card_Cast*     p_cast_prior     = nullptr;
  Card_Combined* p_combined_prior = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  eXK eStageCast;
  Symbol* p_Y_streamPtn_curr = nullptr;

  Card_StreamPtn* p_streamPtn = new Card_StreamPtn;

  while (pCI->G_bEachMain()) {

    if (CK_INDEXJOIN    == pCI->G.e_MainCard ||
        CK_INDEXSCAN    == pCI->G.e_MainCard ||
        CK_KEYEXISTS    == pCI->G.e_MainCard ||
        CK_KEYNOTEXISTS == pCI->G.e_MainCard ||
        CK_SCAN         == pCI->G.e_MainCard) {

      if (! p_OP_prev) {
        Card_Table* p_table_first = pCI->G.p_OP->pSub_table();
        p_streamPtn->vAddKey(p_table_first->pSub_tablePtn()->pSymbol());
      }
      else {
        eStageCast = pCI->G.pDeck->eAddStageCast_toPrev_cast(pCI,
                                                             p_streamPtn,
                                                             p_project_prior,
                                                             p_combined_prior,
                                                             p_cast_prior,
                                                             pCI->G.p_OP);
        pCI->G_vMainResynch();
        pCI->vPrintBoard(__LINE__);
        vDebugPoint();
      }

      p_OP_prev = pCI->G.p_OP;
      p_project_prior  = nullptr;
      p_combined_prior = nullptr;
      p_cast_prior     = nullptr;
    }
    else if (CK_PROJECT == pCI->G.e_MainCard) {
      p_project_prior  = pCI->G.p_project;
      p_combined_prior = nullptr;
      p_cast_prior     = nullptr;
    }
    else if (CK_COMBINED == pCI->G.e_MainCard) {
      p_combined_prior = pCI->G.p_combined;
      p_cast_prior     = nullptr;
    }
    else if (CK_CAST == pCI->G.e_MainCard) {
      p_cast_prior = pCI->G.p_cast;
    }
  }

  delete p_streamPtn;

  pCI->vPrintBoard(__LINE__);
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eXK Card_Deck::eAddStageCast_toPrev_cast(CardIter*       pCI,
                                         Card_StreamPtn* p_streamPtn,
                                         Card_Project*   p_project_prev,
                                         Card_Combined*  p_combined_prev,
                                         Card_Cast*      p_cast_prev,
                                         Card_OP*        p_OP_curr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(CK_INDEXJOIN    == p_OP_curr->eCard() ||
          CK_INDEXSCAN    == p_OP_curr->eCard() ||
          CK_KEYEXISTS    == p_OP_curr->eCard() ||
          CK_KEYNOTEXISTS == p_OP_curr->eCard() ||
          CK_SCAN         == p_OP_curr->eCard());

  RASSERT(p_streamPtn);

  Card_Table*      p_table_join      = p_OP_curr->pSub_table();
  Card_TablePtn*   p_tablePtn_join   = p_table_join->pSub_tablePtn();
  Card_IndexList*  p_indexList_join  = p_OP_curr->pSub_indexList();
  Card_Index*      p_index_join      = p_indexList_join->pFindChosenIndex();
  Card_IdxColList* p_idxColList_join = p_index_join->pSub_idxColList();
  Card_Bounds*     p_bounds_join     = p_index_join->pSub_bounds();
  Card_Bound*      p_bound_join      = p_bounds_join->pSub_bound();

  Card_Term* p_term_tblCol_hash      = p_bound_join->pGet_tblColOdn();
  Card_Term* p_term_idxColOdn_join   = p_bound_join->pGet_idxColOdn();

  Card_IdxCol* p_idxCol_join =
      p_idxColList_join->pGetIdxCol_byIdxColOdn(p_term_idxColOdn_join->szSymbol());
  Symbol* p_Y_idxCol_join = p_idxCol_join->pSymbol();

  Symbol* p_Y_tblCol_prior = p_bound_join->pGet_tblColOdn()
                               ? p_bound_join->pGet_tblColOdn()->pSymbol()
                               : nullptr;

  Symbol* p_Y_tablePtn_join = p_tablePtn_join->pSymbol();

  eXK eStageCast = Card_Cast::eStageCast(p_streamPtn,
                                         p_Y_tblCol_prior,
                                         p_Y_tablePtn_join,
                                         p_Y_idxCol_join);
  Card_Hash* p_hash_prior_new;
  switch (eStageCast) {

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case XK_BROADCAST:
      p_cast_prev->vSymbol("broadcast"_Stz);

      if (p_project_prev && ! p_combined_prev) {
        Card_Combined *p_combined = new Card_Combined;
        pCI->G_vInsertBefore(p_combined, p_cast_prev);
      }

      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case XK_HASHCAST:
      p_cast_prev->vSymbol("hashcast"_Stz);

      pCI->G.p_expr = pCI->pExpr_For_col(p_term_tblCol_hash, pCI->G.i_MainCard);
      p_hash_prior_new = Card_Hash::pDerive(pCI->G.p_expr);

      if (p_combined_prev) {
        pCI->G_vInsertBefore(p_hash_prior_new, p_combined_prev);
      }
      else {
        pCI->G_vInsertBefore(p_hash_prior_new, p_cast_prev);
        Card_Combined* p_combined = new Card_Combined;
        pCI->G_vInsertBefore(p_combined, p_cast_prev);
      }

      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case XK_UNICAST:
      // no change to next card; default is unicast
      break;

    default:
      NEVER_REACH
  }

  return eStageCast;
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
      pCI->G.p_table->vDeleteChild(pCI->G.p_projection);

  pCI->vRestart();
  pCI->Analysis.m_pProjection_prior = pCI->G.p_projection = nullptr;

  while (pCI->G_bEachMain()) {

    if (CK_TABLE == pCI->G.e_MainCard) {
      pCI->G.p_table->vUpdate_projection(pCI);
      continue;
    }

    if (CK_WHERE == pCI->G.e_MainCard || CK_HAVING == pCI->G.e_MainCard) {
      if ((pCI->G.p_rpnCalc = pCI->G.p_MainCard->pSub_rpnCalc()))
        pCI->G.p_rpnCalc->vEnsureTypesFor_stmOdn(pCI);

      //pCI->G.p_MainCard->vAppend(pCI->Analysis.m_pProjection_prior->pClone());

      continue;
    }

    if ((pCI->G.p_key = pCI->G.p_MainCard->pSub_key())) {
      if (pCI->Analysis.m_pProjection_prior) {
        pCI->G.p_key->vTypes_For_stmColOdn(pCI);
        pCI->G.p_key->vRenumberAll_expr();
      }
    }

    if ((pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection())) {

      if (pCI->Analysis.m_pProjection_prior) {
        pCI->G.p_projection->vEnsureTypes_For_stmColOdn(pCI);
        pCI->G.p_projection->vRenumberAll_expr();
      }

      pCI->Analysis.m_pProjection_prior = pCI->G.p_MainCard->pSub_projection();
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
      pCI->G.p_table->vDeleteChild(pCI->G.p_projection);

  bool bSawFold = false;

  pCI->vRestart();
  pCI->Analysis.m_pProjection_prior = pCI->G.p_projection = nullptr;

  while (pCI->G_bEachMain()) {

    if (CK_FOLD == pCI->G.e_MainCard) {
      bSawFold = true;
      continue;
    }

    switch (pCI->G.e_MainCard) {

      case CK_AGGREG:
        pCI->G.p_aggreg->Card_Aggreg::vUpdate_projection(pCI);
        break;

      case CK_GBYAGG:
        pCI->G.p_gbyAgg->Card_GbyAgg::vUpdate_projection(pCI);
        break;

      case CK_HAVING:
        vDebugPoint();
        break;

      case CK_ORDAGG:
        pCI->G.p_ordAgg->Card_OrdAgg::vUpdate_projection(pCI);
        break;

      case CK_PROJECT:
        if (pCI->G.p_MainCard->pSub_needed())
          pCI->G.p_project->Card_Project::vUpdate_projection(pCI);
        break;

      case CK_TABLE:
        pCI->G.p_table->vUpdate_projection(pCI);
        break;

      case CK_WHERE:
        vDebugPoint();
        break;

      default:
        if (pCI->G.p_MainCard->pSub_projection()) {
          vTrace("UPDATE? %s"_Stz, pCI->G.p_MainCard->szCardNm());
          vDebugPoint();
        }
        break;
    }

    if (pCI->G.p_MainCard->pSub_projection()) {
      pCI->Analysis.m_pProjection_prior = pCI->G.p_MainCard->pSub_projection();
    }

  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
