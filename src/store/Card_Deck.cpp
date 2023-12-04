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
: Card_symbol(CK_DECK, C_UNKNOWN)
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

  Card_OP*         p_OP_prev          = nullptr;
  Card_Project*    p_project_prior    = nullptr;
  Card_Emit* p_arriving = nullptr;
  Card_Cast*       p_cast_prior       = nullptr;
  Card_Combined*   p_combined_prior   = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  eXK eStageCast;

  Card_StreamPtn* p_streamPtn = new Card_StreamPtn;

  pCI->vBeginEach();
  while (pCI->G_bEachMain_NONE()) {

    if (CK_m_INDEXJOIN == pCI->G.e_MainCard ||
        CK_m_INDEXSCAN == pCI->G.e_MainCard ||
        CK_m_KEYEXISTS == pCI->G.e_MainCard ||
        CK_m_KEYNOTEXISTS == pCI->G.e_MainCard ||
        CK_m_SCAN == pCI->G.e_MainCard) {

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
                                                             p_arriving,
                                                             pCI->G.p_OP);
        UNUSED(eStageCast);
        pCI->G_vMainResynch();
       // pCI->vPrintBoard(__LINE__);
        vDebugPoint();
      }

      p_OP_prev = pCI->G.p_OP;
      p_project_prior  = nullptr;
      p_combined_prior = nullptr;
      p_cast_prior     = nullptr;
    }
    else if (CK_m_PROJECT == pCI->G.e_MainCard) {
      p_project_prior  = pCI->G.p_project;
      p_combined_prior = nullptr;
      p_cast_prior     = nullptr;
    }
    else if (CK_m_COMBINED == pCI->G.e_MainCard) {
      p_combined_prior = pCI->G.p_combined;
      p_cast_prior     = nullptr;
    }
    else if (CK_m_CAST == pCI->G.e_MainCard) {
      p_cast_prior = pCI->G.p_cast;
    }

    // -- -- -- -- --
    if (pCI->G.p_OP->pSub_emit())
      p_arriving = pCI->G.p_OP->pSub_emit();
  }

  delete p_streamPtn;

 // pCI->vPrintBoard(__LINE__);
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eXK Card_Deck::eAddStageCast_toPrev_cast(CardIter*        pCI,
                                         Card_StreamPtn*  p_streamPtn,
                                         Card_Project*    p_project_prev,
                                         Card_Combined*   p_combined_prev,
                                         Card_Cast*       p_cast_prev,
                                         Card_Emit* p_emitted_prev,
                                         Card_OP*         p_OP_curr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(CK_m_INDEXJOIN == p_OP_curr->eCard() ||
          CK_m_INDEXSCAN == p_OP_curr->eCard() ||
          CK_m_KEYEXISTS == p_OP_curr->eCard() ||
          CK_m_KEYNOTEXISTS == p_OP_curr->eCard() ||
          CK_m_SCAN == p_OP_curr->eCard());

  RASSERT(p_streamPtn);

  //Card_Table*      p_table_join      = p_OP_curr->pSub_table();
  //Card_From*       p_from_join       = p_OP_curr->pSub_from();
  Card_TablePtn*   p_tablePtn_join   = p_OP_curr->pSub_tablePtn();
  Card_IndexList*  p_indexList_join  = p_OP_curr->pSub_indexList();
  Card_Index*      p_index_join      = p_indexList_join->pFindChosenIndex();
  Card_IdxColList* p_idxColList_join = p_index_join->pSub_idxColList();
  Card_Bounds*     p_bounds_join     = p_index_join->pSub_bounds();
  Card_Bound*      p_bound_join      = p_bounds_join->pSub_bound();

  Card_Term* p_term_tblCol_hash      = p_bound_join->pGet_tblCol();
  Card_Term* p_term_idxCol_join   = p_bound_join->pGet_idxCol();

  Card_IdxCol* p_idxCol_join =
      p_idxColList_join->pGetIdxCol_byIdxCol(p_term_idxCol_join->szFacet(SF_NONE));
  Symbol* p_Y_idxCol_join = p_idxCol_join->pSymbol();

  Symbol* p_Y_tblCol_prior = p_bound_join->pGet_tblCol()
                               ? p_bound_join->pGet_tblCol()->pSymbol()
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
      p_cast_prev->SYM(SF_STG_CAST,"broadcast"_Stz);

      if (p_project_prev && ! p_combined_prev) {
        Card_Combined *p_combined = new Card_Combined;
        pCI->G_vInsertBefore(p_combined, p_cast_prev);
      }

      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case XK_HASHCAST:
      p_cast_prev->SYM(SF_STG_CAST,"hashcast"_Stz);

      pCI->G.p_expr = pCI->pExpr_For_col(p_term_tblCol_hash, pCI->G.i_MainCard);
      p_hash_prior_new = Card_Hash::pDerive(pCI->G.p_expr,p_emitted_prev);

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

  if (Card_Emit* p_emitted_fold = this-> p_emitted_fold())
    return p_emitted_fold->pSub_expr();

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
    if (pMain->bIsCard(CK_m_FOLD))
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
    if (pMain->bIsCard(CK_m_FOLD))
      return _pCard_Fold(pMain);
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find project before fold (reverse)                                         */
/*----------------------------------------------------------------------------*/
Card_Emit* Card_Deck::p_emitted_fold(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 i_Fold = this->i_Fold();
  for (i8 i_Main = i_Fold - 1; i_Main >= 0; i_Main--) {
    Card *pMain = pGetChild(i_Main);
    if (Card_Emit* p_emit = pMain->pSub_emit())
      return p_emit;
  }

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* initial emitted analysis                                                   */
/*----------------------------------------------------------------------------*/
void Card_Deck::vAnalyse_emitted(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->vBeginEach();
  while (pCI->G_bEachMainWithSub(CK_m_FROM, CK_s_TABLE))
    if ((pCI->G.p_emit = pCI->G.p_table->pSub_emit()))
      pCI->G.p_table->vDeleteChild(pCI->G.p_emit);

  pCI->vBeginEach();
  pCI->Flux.m_pArriving = pCI->G.p_emit = nullptr;

  while (pCI->G_bEachMain_NONE()) {

//    if (pCI->Flux.m_pArriving)
//      pCI->Flux.m_pArriving->vDump(__func__);

    if (CK_m_PROJECT == pCI->G.e_MainCard)
      vDebugPoint();

//    if (pCI->Flux.m_pArriving) {
//      pCI->Flux.m_pArriving->vDump(__func__);
//      vDebugPoint();
//    }

    if (pCI->G.p_MainCard->bIsCard(CK_m_TOP))
      vDebugPoint();

    // todo [Ray A 146]

    if (CK_m_FROM == pCI->G.e_MainCard) {
      pCI->G.p_table = pCI->G.p_MainCard->pSub_table();
      pCI->G.p_from->vUpdate_emitted(pCI);

      if (pCI->G.p_MainCard->pSub_emit())
        pCI->Flux.m_pArriving = pCI->G.p_MainCard->pSub_emit();

        // todo: fix...
//      pCI->G.p_emit = pCI->G.p_table->pSub_emit();
//      pCI->G.p_table->vRemoveChild(pCI->G.p_emit);
//      pCI->G.p_from->vAppendChild(pCI->G.p_emit);

     // pCI->vPrintBoard();

      if ((pCI->G.p_constraint = pCI->G.p_MainCard->pSub_constraint())) {
        if (pCI->Flux.m_pArriving) {
          pCI->G.p_constraint->vEnsureTypeId_expCol(pCI);
        }
      }

      continue;
    }

    if (CK_m_WHERE == pCI->G.e_MainCard || CK_m_HAVING == pCI->G.e_MainCard) {
      if ((pCI->G.p_rpnCalc = pCI->G.p_MainCard->pSub_rpnCalc()))
        pCI->G.p_rpnCalc->vEnsureTypeId_expCol(pCI);

      pCI->G.p_OP->vEnsure_stream(pCI);
      Card_Emit* p_emit = pCI->Flux.m_pArriving->pPropagate();
      //p_emit->vDump(__func__ );
      pCI->G.p_stream->vAppendChild(p_emit);

      //continue;
    }

//    if (CK_m_LIMIT == pCI->G.e_MainCard || CK_m_SORT == pCI->G.e_MainCard) {
//      pCI->G.p_MainCard->vAppendChild(pCI->Flux.m_pArriving->pClone());
//    }
//    if (CK_m_SORT == pCI->G.e_MainCard) {
//      Card_Emit* p_emitted_propagated =
//          pCI->Flux.m_pArriving->pPropagate();
//      p_emitted_propagated->vAdjust_Wrt_key(pCI->G.p_MainCard->pSub_key());
//
//      pCI->G.p_MainCard->vAppendChild(p_emitted_propagated);
//    }

    if (pCI->G.p_MainCard->bIsOne(CK_m_HASH, CK_m_KEYEXISTS, CK_m_KEYNOTEXISTS, CK_m_LIMIT, CK_m_TOP, CK_NONE)) {
      pCI->G.p_OP->vEnsure_stream(pCI);
      pCI->G.p_emit = pCI->Flux.m_pArriving->pPropagate();

      pCI->G.p_stream->vAppendChild(pCI->G.p_emit);
    }
    else
    if (CK_m_SORT == pCI->G.e_MainCard) {

      pCI->G.p_OP->vEnsure_stream(pCI);

      Card_Emit* p_emitted_propagated =
          pCI->Flux.m_pArriving->pPropagate();
      p_emitted_propagated->vAdjust_Wrt_key(pCI->G.p_MainCard->pSub_key());

      pCI->G.p_stream->vAppendChild(p_emitted_propagated);
    }

    if ((pCI->G.p_key = pCI->G.p_MainCard->pSub_key())) {
      if (pCI->Flux.m_pArriving) {
        pCI->G.p_key->vEnsureTypeId_expCol(pCI);
        pCI->G.p_key->vRenumberAll_expr();
      }
    }

    if ((pCI->G.p_emit = pCI->G.p_MainCard->pSub_emit())) {

      if (pCI->Flux.m_pArriving) {
        //pCI->G.p_emit->vDump(__func__);
        pCI->G.p_emit->vEnsureTypeId_expCol(pCI);
        //pCI->G.p_emit->vDump(__func__);
        pCI->G.p_emit->vRenumberAll_expr();
        //pCI->G.p_emit->vDump(__func__);
        vDebugPoint();
      }

      pCI->Flux.m_pArriving = pCI->G.p_MainCard->pSub_emit();
    }

    if ((pCI->G.p_constraint = pCI->G.p_MainCard->pSub_constraint())) {
      if (pCI->Flux.m_pArriving) {
        pCI->G.p_constraint->vEnsureTypeId_expCol(pCI);
      }
    }

  }
}

/*----------------------------------------------------------------------------*/
/* initial emitted analysis                                                   */
/*----------------------------------------------------------------------------*/
void Card_Deck::vAnalyse_emitted_passThru(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete all <>/
  pCI->vBeginEach();
  while (pCI->G_bEachSubOfMain(CK_s_TABLE))
    if ((pCI->G.p_emit = pCI->G.p_table->pSub_emit()))
      pCI->G.p_table->vDeleteChild(pCI->G.p_emit);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->vClearRestart();
  while (pCI->G_bEachMain_NONE()) {

    // --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --

    if (CK_m_KEYEXISTS == pCI->G.e_MainCard ||
        CK_m_KEYNOTEXISTS == pCI->G.e_MainCard) {

      pCI->G.p_MainCard->vAppendChild(pCI->Flux.m_pArriving->pPropagate());

      if ((pCI->G.p_indexList = pCI->G.p_MainCard->pSub_indexList())) {
        if (pCI->Flux.m_pArriving) {
          pCI->G.p_indexList->vTypes_For_expCol(pCI);
        }
      }

      continue;
    }

    // --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --

    if (CK_m_SCAN == pCI->G.e_MainCard ||
        CK_m_INDEXJOIN == pCI->G.e_MainCard ||
        CK_m_INDEXSCAN == pCI->G.e_MainCard) {

      pCI->G.p_table = pCI->G.p_MainCard->pSub_table();
      pCI->G.p_OP->vUpdate_emitted(pCI);

      pCI->Flux.m_pArriving = pCI->G.p_MainCard->pSub_emit();

      if ((pCI->G.p_indexList = pCI->G.p_MainCard->pSub_indexList())) {
        if (pCI->Flux.m_pArriving) {
          pCI->G.p_indexList->vTypes_For_expCol(pCI);
        }
      }

      continue;
    }

    // --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --

    if (pCI->G.p_MainCard->bIsCard(CK_m_HASH)) {
      pCI->G.p_hshCol = pCI->G.p_MainCard->pSub_hshCol();
      //pCI->G.p_hshCol->vDump(__func__);
      continue;
    }

    // --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
    // --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --

    if (CK_m_WHERE == pCI->G.e_MainCard || CK_m_HAVING == pCI->G.e_MainCard) {
      if ((pCI->G.p_rpnCalc = pCI->G.p_MainCard->pSub_rpnCalc()))
        pCI->G.p_rpnCalc->vEnsureTypeId_expCol(pCI);

      //pCI->G.p_MainCard->vAppendChild(pCI->Flux.m_pArriving->pClone());
      pCI->G.p_MainCard->vAppendChild(pCI->Flux.m_pArriving->pPropagate());

      //continue;
    }

    if (pCI->G.p_MainCard->bIsOne(CK_m_HASH, CK_m_KEYEXISTS, CK_m_KEYNOTEXISTS, CK_m_LIMIT, CK_NONE)) {
      pCI->G.p_MainCard->vAppendChild(pCI->Flux.m_pArriving->pClone());
    }
    else
    if (CK_m_SORT == pCI->G.e_MainCard || CK_m_TOP == pCI->G.e_MainCard) {
      Card_Emit* p_emitted_propagated =
          pCI->Flux.m_pArriving->pPropagate();
      p_emitted_propagated->vAdjust_Wrt_key(pCI->G.p_MainCard->pSub_key());

      pCI->G.p_MainCard->vAppendChild(p_emitted_propagated);
    }

    if ((pCI->G.p_key = pCI->G.p_MainCard->pSub_key())) {
      if (pCI->Flux.m_pArriving) {
        pCI->G.p_key->vEnsureTypeId_expCol(pCI);
        pCI->G.p_key->vRenumberAll_expr();
      }
    }

    if ((pCI->G.p_emit = pCI->G.p_MainCard->pSub_emit())) {

      if (pCI->Flux.m_pArriving) {
        pCI->G.p_emit->vEnsureTypeId_expCol(pCI);
        pCI->G.p_emit->vRenumberAll_expr();
      }

      pCI->Flux.m_pArriving = pCI->G.p_MainCard->pSub_emit();
    }

  }
}

/*----------------------------------------------------------------------------*/
/* update projections after some change                                       */
/*----------------------------------------------------------------------------*/
void Card_Deck::vProjectionUpdate(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->vBeginEach();
  while (pCI->G_bEachMainWith2Sub(CK_m_FROM, CK_s_STREAM, CK_t_EMIT))
    pCI->G.p_stream->vDeleteChild(pCI->G.p_emit);


  pCI->vBeginEach();
  pCI->Flux.m_pArriving = pCI->G.p_emit = nullptr;

  while (pCI->G_bEachMain_NONE()) {

    if (CK_m_FOLD == pCI->G.e_MainCard)
      continue;

    switch (pCI->G.e_MainCard) {

      case CK_m_LIMIT:   break;
      case CK_m_SORT:    break;

      case CK_m_AGGREG:  pCI->G.p_aggreg->Card_Aggreg::vUpdate_emitted(pCI); break;
      case CK_m_GBYAGG:  pCI->G.p_gbyAgg->Card_GbyAgg::vUpdate_emitted(pCI); break;
      case CK_m_HAVING:  pCI->G.p_having->Card_Having::vUpdate_emitted(pCI); break;
      case CK_m_ORDAGG:  pCI->G.p_ordAgg->Card_OrdAgg::vUpdate_emitted(pCI); break;
      case CK_m_PROJECT: pCI->G.p_project->Card_Project::vUpdate_emitted(pCI); break;
      case CK_m_WHERE:   pCI->G.p_where->Card_Where::vUpdate_emitted(pCI); break;

      case CK_m_FROM:
        pCI->G.p_table = pCI->G.p_MainCard->pSub_table();
        //pCI->vPrintBoard(__LINE__);
        pCI->G.p_from->vUpdate_emitted(pCI);

        if ((pCI->G.p_filter = pCI->G.p_from->pSub_filter())) {
          if ((pCI->G.p_stream = pCI->G.p_filter->pSub_stream())) {
            if ((pCI->G.p_emit = pCI->G.p_stream->pSub_emit())) {
              pCI->G.p_stream->vDeleteChild(pCI->G.p_emit);
              pCI->G.p_stream->vAppendChild(pCI->G.p_from->pSub_emit()->pClone());
            }
          }
        }
        break;

      default:
        if (pCI->G.p_MainCard->pSub_emit()) {
          vTrace("UPDATE? %s"_Stz, pCI->G.p_MainCard->szCardNm());
          vDebugPoint();
        }
        break;
    }

//    if (pCI->G.p_filter = pCI->G.p_MainCard->pSub_filter()) {
//      pCI->G.p_filter->Card_Filter::vUpdate_emitted(pCI);
//    }

    if (pCI->G.p_MainCard->pSub_emit()) {
      pCI->Flux.m_pArriving = pCI->G.p_MainCard->pSub_emit();
    }

    //pCI->vPrintBoard(__LINE__);
  }

  pCI->vValidateBench();
}

/*----------------------------------------------------------------------------*/
/* validate children                                                          */
/*----------------------------------------------------------------------------*/
void Card_Deck::vValidate(CardIter* pCI)
{
  _precondition_(_MAGIC_, _FOOTER_);

  //vMarksTestRecursive();

  vMarksClearRecursive();
  vMark1Recursive(MARK_UNVALID);

  RASSERT(0 == pCI->nAncestors());

  for (Kid xKid(this); xKid.bEach(); /**/) {

    Card* pChild = xKid.pChild();

    eCK eChild = pChild->eCard();

    pCI->vMember(pChild);

    switch (eChild) {

      case CK_m_ABSJOIN:      pCI->G.p_absJoin->vValidate(pCI, 1, this); break;
      case CK_m_AGGREG:       pCI->G.p_aggreg->vValidate(pCI, 1, this); break;
      case CK_ANDCONJ:      pCI->G.p_andConj->vValidate(pCI, 1, this); break;

      case CK_BOOLPRED:     pCI->G.p_boolPred->vValidate(pCI, 1, this); break;
      case CK_BOUND:        pCI->G.p_bound->vValidate(pCI, 1, this); break;
      case CK_BOUNDS:       pCI->G.p_bounds->vValidate(pCI, 1, this); break;

      case CK_m_CALL:         pCI->G.p_call->vValidate(pCI, 1, this); break;
      case CK_m_CAST:         pCI->G.p_cast->vValidate(pCI, 1, this); break;

      case CK_m_COMBINED:     pCI->G.p_combined->vValidate(pCI, 1, this); break;
      case CK_s_CONSTRAINT:   pCI->G.p_constraint->vValidate(pCI, 1, this); break;
      case CK_CORRCOLS:     pCI->G.p_corrCols->vValidate(pCI, 1, this); break;
      case CK_m_CORRELATION:  pCI->G.p_correlation->vValidate(pCI, 1, this); break;
      case CK_COSTJOINING:  pCI->G.p_costJoining->vValidate(pCI, 1, this); break;
      case CK_COSTNETWORK:  pCI->G.p_costNetwork->vValidate(pCI, 1, this); break;
      case CK_COSTSTREAM:   pCI->G.p_costQuery->vValidate(pCI, 1, this); break;
      case CK_COSTUNICAST:  pCI->G.p_costUnicast->vValidate(pCI, 1, this); break;
      case CK_COUNT:        pCI->G.p_count->vValidate(pCI, 1, this); break;
      case CK_m_CROSSJOIN:    pCI->G.p_crossJoin->vValidate(pCI, 1, this); break;

      case CK_DIRECTION:    pCI->G.p_direction->vValidate(pCI, 1, this); break;

      case CK_t_EMIT:         pCI->G.p_emit->vValidate(pCI, 1, this); break;
      case CK_EXPR:         pCI->G.p_expr->vValidate(pCI, 1, this); break;

      case CK_m_FILTER:       pCI->G.p_filter->vValidate(pCI, 1, this); break;
      case CK_FILTPRED:     pCI->G.p_filtPred->vValidate(pCI, 1, this); break;
      case CK_FILTPREDLIST: pCI->G.p_filtPredList->vValidate(pCI, 1, this); break;
      case CK_m_FLAT:         pCI->G.p_flat->vValidate(pCI, 1, this); break;
      case CK_m_FOLD:         pCI->G.p_fold->vValidate(pCI, 1, this); break;
      case CK_m_FROM:         pCI->G.p_from->vValidate(pCI, 1, this); break;

      case CK_m_GBYAGG:       pCI->G.p_gbyAgg->vValidate(pCI, 1, this); break;

      case CK_m_HASH:         pCI->G.p_hash->vValidate(pCI, 1, this); break;
      case CK_m_HAVING:       pCI->G.p_having->vValidate(pCI, 1, this); break;
      case CK_t_HSHCOL:       pCI->G.p_hshCol->vValidate(pCI, 1, this); break;

      case CK_IDXCOL:       pCI->G.p_idxCol->vValidate(pCI, 1, this); break;
      case CK_IDXCOLLIST:   pCI->G.p_idxColList->vValidate(pCI, 1, this); break;
      case CK_INDEX:        pCI->G.p_index->vValidate(pCI, 1, this); break;
      case CK_m_INDEXJOIN:    pCI->G.p_indexJoin->vValidate(pCI, 1, this); break;
      case CK_t_INDEXLIST:    pCI->G.p_indexList->vValidate(pCI, 1, this); break;
      case CK_INDEXNM:      pCI->G.p_indexNm->vValidate(pCI, 1, this); break;
      case CK_m_INDEXSCAN:    pCI->G.p_indexScan->vValidate(pCI, 1, this); break;
      case CK_m_INFLATE:      pCI->G.p_inflate->vValidate(pCI, 1, this); break;
      case CK_s_INJECTABLE:     pCI->G.p_injectable->vValidate(pCI, 1, this); break;

      case CK_JOINCAST:     pCI->G.p_joinCast->vValidate(pCI, 1, this); break;
      case CK_JOINESTIMATE: pCI->G.p_joinEstimate->vValidate(pCI, 1, this); break;
      case CK_JOINORDER:    pCI->G.p_joinOrder->vValidate(pCI, 1, this); break;
      case CK_JOINORDERING: pCI->G.p_joinOrdering->vValidate(pCI, 1, this); break;
      case CK_JOINPREDLIST: pCI->G.p_joinPredList->vValidate(pCI, 1, this); break;

      case CK_s_KEY:          pCI->G.p_key->vValidate(pCI, 1, this); break;
      case CK_KEYCELLS:     pCI->G.p_keyCells->vValidate(pCI, 1, this); break;
      case CK_m_KEYEXISTS:    pCI->G.p_keyExists->vValidate(pCI, 1, this); break;
      case CK_m_KEYNOTEXISTS: pCI->G.p_keyNotExists->vValidate(pCI, 1, this); break;

      case CK_s_LEVEL:        pCI->G.p_level->vValidate(pCI, 1, this); break;
      case CK_m_LIMIT:        pCI->G.p_limit->vValidate(pCI, 1, this); break;

      case CK_MAP:          pCI->G.p_map->vValidate(pCI, 1, this); break;
      case CK_s_MAPPING:      pCI->G.p_mapping->vValidate(pCI, 1, this); break;

      case CK_s_NEEDED:       pCI->G.p_needed->vValidate(pCI, 1, this); break;
      case CK_NOTE:         pCI->G.p_note->vValidate(pCI, 1, this); break;
      case CK_NOTCONJ:      pCI->G.p_notConj->vValidate(pCI, 1, this); break;

      case CK_ON:           pCI->G.p_on->vValidate(pCI, 1, this); break;
      case CK_m_ORDAGG:       pCI->G.p_ordAgg->vValidate(pCI, 1, this); break;
      case CK_ORCONJ:       pCI->G.p_orConj->vValidate(pCI, 1, this); break;
      case CK_OUTERCOLNM:   pCI->G.p_outerColNm->vValidate(pCI, 1, this); break;
      case CK_OUTERJOIN:    pCI->G.p_outerJoin->vValidate(pCI, 1, this); break;

      case CK_PAYLOADCELLS: pCI->G.p_payloadCells->vValidate(pCI, 1, this); break;
      case CK_m_PROJECT:      pCI->G.p_project->vValidate(pCI, 1, this); break;

      case CK_RLNCONJ:      pCI->G.p_rlnConj->vValidate(pCI, 1, this); break;
      case CK_ROWCOUNT:     pCI->G.p_rowCount->vValidate(pCI, 1, this); break;
      case CK_ROWSIZE:      pCI->G.p_rowSize->vValidate(pCI, 1, this); break;
      case CK_RPNCALC:      pCI->G.p_rpnCalc->vValidate(pCI, 1, this); break;

      case CK_m_SCAN:         pCI->G.p_scan->vValidate(pCI, 1, this); break;
      case CK_SECTION:      pCI->G.p_section->vValidate(pCI, 1, this); break;
      case CK_m_SEND:         pCI->G.p_send->vValidate(pCI, 1, this); break;
      case CK_m_SORT:         pCI->G.p_sort->vValidate(pCI, 1, this); break;
      case CK_m_STAGE:        pCI->G.p_stage->vValidate(pCI, 1, this); break;
      case CK_STREAMHEAD:   pCI->G.p_streamHead->vValidate(pCI, 1, this); break;
      case CK_STREAMJOIN:   pCI->G.p_streamJoin->vValidate(pCI, 1, this); break;
      case CK_STREAMKEY:    pCI->G.p_streamKey->vValidate(pCI, 1, this); break;
      case CK_STREAMPER:    pCI->G.p_streamPer->vValidate(pCI, 1, this); break;
      case CK_STREAMPTN:    pCI->G.p_streamPtn->vValidate(pCI, 1, this); break;
      case CK_STREAMSET:    pCI->G.p_streamSet->vValidate(pCI, 1, this); break;
      case CK_SUBQUERY:     pCI->G.p_subQuery->vValidate(pCI, 1, this); break;
      case CK_SUBSET:       pCI->G.p_subSet->vValidate(pCI, 1, this); break;

      case CK_s_TABLE:        pCI->G.p_table->vValidate(pCI, 1, this); break;
      case CK_TABLELIST:    pCI->G.p_tableList->vValidate(pCI, 1, this); break;
      case CK_t_TABLENM:      pCI->G.p_tableNm->vValidate(pCI, 1, this); break;
      case CK_TABLEPTN:     pCI->G.p_tablePtn->vValidate(pCI, 1, this); break;
      case CK_TABLESIZE:    pCI->G.p_tableSize->vValidate(pCI, 1, this); break;
      case CK_TERM:         pCI->G.p_term->vValidate(pCI, 1, this); break;
      case CK_m_TOP:          pCI->G.p_top->vValidate(pCI, 1, this); break;
      case CK_TOPOLOGY:     pCI->G.p_topology->vValidate(pCI, 1, this); break;

      case CK_UNIQUE:       pCI->G.p_unique->vValidate(pCI, 1, this); break;

      case CK_m_WHERE:        pCI->G.p_where->vValidate(pCI, 1, this); break;

      default:
        NEVER_REACH
    }

  }

  RASSERT(0 == pCI->nAncestors());

  vValidationPassed();

  vMark1RecursiveIf(MARK_BLANK, MARK_VALID);

  if (Card* pCard_notValidate = pFindMark1Recursive(MARK_UNVALID)) {
   // pCI->vPrintBoard(__LINE__,true);
    pCard_notValidate->vDump(__func__);
    vDebugPoint();
    NEVER_REACH
  }

  if (Card* pCard_notValidate = pFindMark1Recursive(MARK_DUPLICATE)) {
   // pCI->vPrintBoard(__LINE__,true);
    pCard_notValidate->vDump(__func__);
    vDebugPoint();
    NEVER_REACH
  }

  vMarksClearRecursive();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
