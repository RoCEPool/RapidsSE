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
  Card_Projection* p_projection_prior = nullptr;
  Card_Cast*       p_cast_prior       = nullptr;
  Card_Combined*   p_combined_prior   = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  eXK eStageCast;

  Card_StreamPtn* p_streamPtn = new Card_StreamPtn;

  pCI->vBeginEach();
  while (pCI->G_bEachMain_NONE()) {

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
                                                             p_projection_prior,
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

    // -- -- -- -- --
    if (pCI->G.p_OP->pSub_projection())
      p_projection_prior = pCI->G.p_OP->pSub_projection();
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
                                         Card_Projection* p_projection_prev,
                                         Card_OP*         p_OP_curr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(CK_INDEXJOIN    == p_OP_curr->eCard() ||
          CK_INDEXSCAN    == p_OP_curr->eCard() ||
          CK_KEYEXISTS    == p_OP_curr->eCard() ||
          CK_KEYNOTEXISTS == p_OP_curr->eCard() ||
          CK_SCAN         == p_OP_curr->eCard());

  RASSERT(p_streamPtn);

  //Card_Table*      p_table_join      = p_OP_curr->pSub_table();
  //Card_From*       p_from_join       = p_OP_curr->pSub_from();
  Card_TablePtn*   p_tablePtn_join   = p_OP_curr->pSub_tablePtn();
  Card_IndexList*  p_indexList_join  = p_OP_curr->pSub_indexList();
  Card_Index*      p_index_join      = p_indexList_join->pFindChosenIndex();
  Card_IdxColList* p_idxColList_join = p_index_join->pSub_idxColList();
  Card_Bounds*     p_bounds_join     = p_index_join->pSub_bounds();
  Card_Bound*      p_bound_join      = p_bounds_join->pSub_bound();

  Card_Term* p_term_tblCol_hash      = p_bound_join->pGet_tblColOdn();
  Card_Term* p_term_idxColOdn_join   = p_bound_join->pGet_idxColOdn();

  Card_IdxCol* p_idxCol_join =
      p_idxColList_join->pGetIdxCol_byIdxColOdn(p_term_idxColOdn_join->szSymbol(SF_NONE));
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
      p_hash_prior_new = Card_Hash::pDerive(pCI->G.p_expr,p_projection_prev);

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

  pCI->vBeginEach();
  while (pCI->G_bEachMainWithSub(CK_FROM,CK_TABLE))
    if ((pCI->G.p_projection = pCI->G.p_table->pSub_projection()))
      pCI->G.p_table->vDeleteChild(pCI->G.p_projection);

  pCI->vBeginEach();
  pCI->Prior.m_pProjection = pCI->G.p_projection = nullptr;

  while (pCI->G_bEachMain_NONE()) {

//    if (pCI->Prior.m_pProjection)
//      pCI->Prior.m_pProjection->vDump(__func__);

    if (CK_PROJECT == pCI->G.e_MainCard)
      vDebugPoint();

//    if (pCI->Prior.m_pProjection) {
//      pCI->Prior.m_pProjection->vDump(__func__);
//      vDebugPoint();
//    }

    if (pCI->G.p_MainCard->bIsCard(CK_TOP))
      vDebugPoint();

    // todo [Ray A 146]

    if (CK_FROM == pCI->G.e_MainCard) {
      pCI->G.p_table = pCI->G.p_MainCard->pSub_table();
      pCI->G.p_from->vUpdate_projection(pCI);

      if (pCI->G.p_MainCard->pSub_projection())
        pCI->Prior.m_pProjection = pCI->G.p_MainCard->pSub_projection();

        // todo: fix...
//      pCI->G.p_projection = pCI->G.p_table->pSub_projection();
//      pCI->G.p_table->vRemoveChild(pCI->G.p_projection);
//      pCI->G.p_from->vAppendChild(pCI->G.p_projection);

     // pCI->vPrintBoard();

      if ((pCI->G.p_constraint = pCI->G.p_MainCard->pSub_constraint())) {
        if (pCI->Prior.m_pProjection) {
          pCI->G.p_constraint->vEnsureTypes_For_stmColOdn(pCI);
        }
      }

      continue;
    }

    if (CK_WHERE == pCI->G.e_MainCard || CK_HAVING == pCI->G.e_MainCard) {
      if ((pCI->G.p_rpnCalc = pCI->G.p_MainCard->pSub_rpnCalc()))
        pCI->G.p_rpnCalc->vEnsureTypesFor_stmOdn(pCI);

      pCI->G.p_OP->vEnsure_stream(pCI);
      Card_Projection* p_projection_clone = pCI->Prior.m_pProjection->pClone();
      //p_projection_clone->vDump(__func__ );
      pCI->G.p_stream->vAppendChild(p_projection_clone);

      //continue;
    }

//    if (CK_LIMIT == pCI->G.e_MainCard || CK_SORT == pCI->G.e_MainCard) {
//      pCI->G.p_MainCard->vAppendChild(pCI->Prior.m_pProjection->pClone());
//    }
//    if (CK_SORT == pCI->G.e_MainCard) {
//      Card_Projection* p_projection_propagated =
//          pCI->Prior.m_pProjection->pPropagate();
//      p_projection_propagated->vAdjust_Wrt_key(pCI->G.p_MainCard->pSub_key());
//
//      pCI->G.p_MainCard->vAppendChild(p_projection_propagated);
//    }

    if (pCI->G.p_MainCard->bIsOne(CK_HASH,CK_KEYEXISTS,CK_KEYNOTEXISTS,CK_LIMIT,CK_TOP,CK_NONE)) {
      pCI->G.p_OP->vEnsure_stream(pCI);
      pCI->G.p_projection = pCI->Prior.m_pProjection->pPropagate(pCI);

      pCI->G.p_stream->vAppendChild(pCI->G.p_projection);
    }
    else
    if (CK_SORT == pCI->G.e_MainCard) {

      pCI->G.p_OP->vEnsure_stream(pCI);

      Card_Projection* p_projection_propagated =
          pCI->Prior.m_pProjection->pPropagate(pCI);
      p_projection_propagated->vAdjust_Wrt_key(pCI->G.p_MainCard->pSub_key());

      pCI->G.p_stream->vAppendChild(p_projection_propagated);
    }

    if ((pCI->G.p_key = pCI->G.p_MainCard->pSub_key())) {
      if (pCI->Prior.m_pProjection) {
        pCI->G.p_key->vTypes_For_stmColOdn(pCI);
        pCI->G.p_key->vRenumberAll_expr();
      }
    }

    if ((pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection())) {

      if (pCI->Prior.m_pProjection) {
        //pCI->G.p_projection->vDump(__func__);
        pCI->G.p_projection->vEnsureTypes_For_stmColOdn(pCI);
        //pCI->G.p_projection->vDump(__func__);
        pCI->G.p_projection->vRenumberAll_expr();
        //pCI->G.p_projection->vDump(__func__);
        vDebugPoint();
      }

      pCI->Prior.m_pProjection = pCI->G.p_MainCard->pSub_projection();
    }

    if ((pCI->G.p_constraint = pCI->G.p_MainCard->pSub_constraint())) {
      if (pCI->Prior.m_pProjection) {
        pCI->G.p_constraint->vEnsureTypes_For_stmColOdn(pCI);
      }
    }

  }
}

/*----------------------------------------------------------------------------*/
/* initial projection analysis                                                */
/*----------------------------------------------------------------------------*/
void Card_Deck::vProjectionAnalysis_passThru(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->vBeginEach();
  while (pCI->G_bEachSubOfMain(CK_TABLE))
    if ((pCI->G.p_projection = pCI->G.p_table->pSub_projection()))
      pCI->G.p_table->vDeleteChild(pCI->G.p_projection);

  pCI->vBeginEach();
  pCI->Prior.m_pProjection = pCI->G.p_projection = nullptr;

  while (pCI->G_bEachMain_NONE()) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    if (CK_KEYEXISTS == pCI->G.e_MainCard ||
        CK_KEYNOTEXISTS == pCI->G.e_MainCard) {

      pCI->G.p_MainCard->vAppendChild(pCI->Prior.m_pProjection->pPropagate(pCI));

      if ((pCI->G.p_indexList = pCI->G.p_MainCard->pSub_indexList())) {
        if (pCI->Prior.m_pProjection) {
          pCI->G.p_indexList->vTypes_For_stmColOdn(pCI);
        }
      }

      continue;
    }

    if (CK_SCAN == pCI->G.e_MainCard ||
        CK_INDEXJOIN == pCI->G.e_MainCard ||
        CK_INDEXSCAN == pCI->G.e_MainCard) {
      pCI->G.p_table = pCI->G.p_MainCard->pSub_table();
       pCI->G.p_OP->vUpdate_projection(pCI);

      pCI->Prior.m_pProjection = pCI->G.p_MainCard->pSub_projection();

      if ((pCI->G.p_indexList = pCI->G.p_MainCard->pSub_indexList())) {
        if (pCI->Prior.m_pProjection) {
          pCI->G.p_indexList->vTypes_For_stmColOdn(pCI);
        }
      }

      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (pCI->G.p_MainCard->bIsCard(CK_HASH)) {
      pCI->G.p_hshCol = pCI->G.p_MainCard->pSub_hshCol();
      //pCI->G.p_hshCol->vDump(__func__);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    if (CK_WHERE == pCI->G.e_MainCard || CK_HAVING == pCI->G.e_MainCard) {
      if ((pCI->G.p_rpnCalc = pCI->G.p_MainCard->pSub_rpnCalc()))
        pCI->G.p_rpnCalc->vEnsureTypesFor_stmOdn(pCI);

      pCI->G.p_MainCard->vAppendChild(pCI->Prior.m_pProjection->pClone());

      //continue;
    }

    if (pCI->G.p_MainCard->bIsOne(CK_HASH,CK_KEYEXISTS,CK_KEYNOTEXISTS,CK_LIMIT,CK_NONE)) {
      pCI->G.p_MainCard->vAppendChild(pCI->Prior.m_pProjection->pClone());
    }
    else
    if (CK_SORT == pCI->G.e_MainCard || CK_TOP == pCI->G.e_MainCard) {
      Card_Projection* p_projection_propagated =
          pCI->Prior.m_pProjection->pPropagate(pCI);
      p_projection_propagated->vAdjust_Wrt_key(pCI->G.p_MainCard->pSub_key());

      pCI->G.p_MainCard->vAppendChild(p_projection_propagated);
    }

    if ((pCI->G.p_key = pCI->G.p_MainCard->pSub_key())) {
      if (pCI->Prior.m_pProjection) {
        pCI->G.p_key->vTypes_For_stmColOdn(pCI);
        pCI->G.p_key->vRenumberAll_expr();
      }
    }

    if ((pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection())) {

      if (pCI->Prior.m_pProjection) {
        pCI->G.p_projection->vEnsureTypes_For_stmColOdn(pCI);
        pCI->G.p_projection->vRenumberAll_expr();
      }

      pCI->Prior.m_pProjection = pCI->G.p_MainCard->pSub_projection();
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
  while (pCI->G_bEachMainWith2Sub(CK_FROM,CK_STREAM,CK_PROJECTION))
    pCI->G.p_stream->vDeleteChild(pCI->G.p_projection);


  pCI->vBeginEach();
  pCI->Prior.m_pProjection = pCI->G.p_projection = nullptr;

  while (pCI->G_bEachMain_NONE()) {

    if (CK_FOLD == pCI->G.e_MainCard) {
      continue;
    }

    switch (pCI->G.e_MainCard) {

      case CK_AGGREG:  pCI->G.p_aggreg->Card_Aggreg::vUpdate_projection(pCI); break;
      case CK_GBYAGG:  pCI->G.p_gbyAgg->Card_GbyAgg::vUpdate_projection(pCI); break;
      case CK_HAVING:  pCI->G.p_having->Card_Having::vUpdate_projection(pCI); break;
      case CK_ORDAGG:  pCI->G.p_ordAgg->Card_OrdAgg::vUpdate_projection(pCI); break;
      case CK_PROJECT: pCI->G.p_project->Card_Project::vUpdate_projection(pCI); break;
      case CK_SORT:    break;
      case CK_FROM:
        pCI->G.p_table = pCI->G.p_MainCard->pSub_table();
        //pCI->vPrintBoard(__LINE__);
        pCI->G.p_from->vUpdate_projection(pCI);

        if ((pCI->G.p_filter = pCI->G.p_from->pSub_filter())) {
          if ((pCI->G.p_stream = pCI->G.p_filter->pSub_stream())) {
            if ((pCI->G.p_projection = pCI->G.p_stream->pSub_projection())) {
              pCI->G.p_stream->vDeleteChild(pCI->G.p_projection);
              pCI->G.p_stream->vAppendChild(pCI->G.p_from->pSub_projection()->pClone());
            }
          }
        }

        break;

      case CK_WHERE:  pCI->G.p_where->Card_Where::vUpdate_projection(pCI); break;

      default:
        if (pCI->G.p_MainCard->pSub_projection()) {
          vTrace("UPDATE? %s"_Stz, pCI->G.p_MainCard->szCardNm());
          vDebugPoint();
        }
        break;
    }

//    if (pCI->G.p_filter = pCI->G.p_MainCard->pSub_filter()) {
//      pCI->G.p_filter->Card_Filter::vUpdate_projection(pCI);
//    }

    if (pCI->G.p_MainCard->pSub_projection()) {
      pCI->Prior.m_pProjection = pCI->G.p_MainCard->pSub_projection();
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

  vMarksTestRecursive();

  vMarksClearRecursive();
  vMark1Recursive(MARK_UNVALID);

  RASSERT(0 == pCI->nAncestors());

  for (Kid xKid(this); xKid.bEach(); /**/) {

    Card* pChild = xKid.pChild();

    eCK eChild = pChild->eCard();

    pCI->vMember(pChild);

    switch (eChild) {

      case CK_ABSJOIN:      pCI->G.p_absJoin->vValidate(pCI, 1, this); break;
      case CK_AGGREG:       pCI->G.p_aggreg->vValidate(pCI, 1, this); break;
      case CK_ANDPRED:      pCI->G.p_andPred->vValidate(pCI, 1, this); break;
      case CK_AS:           pCI->G.p_as->vValidate(pCI, 1, this); break;

      case CK_BOOLCALC:     pCI->G.p_boolCalc->vValidate(pCI, 1, this); break;
      case CK_BOUND:        pCI->G.p_bound->vValidate(pCI, 1, this); break;
      case CK_BOUNDS:       pCI->G.p_bounds->vValidate(pCI, 1, this); break;

      case CK_CALL:         pCI->G.p_call->vValidate(pCI, 1, this); break;
      case CK_CAST:         pCI->G.p_cast->vValidate(pCI, 1, this); break;
      case CK_CMPPRED:      pCI->G.p_cmpPred->vValidate(pCI, 1, this); break;

      case CK_COMBINED:     pCI->G.p_combined->vValidate(pCI, 1, this); break;
      case CK_CONSTRAINT:   pCI->G.p_constraint->vValidate(pCI, 1, this); break;
      case CK_CORRELATION:  pCI->G.p_correlation->vValidate(pCI, 1, this); break;
      case CK_COSTJOINING:  pCI->G.p_costJoining->vValidate(pCI, 1, this); break;
      case CK_COSTNETWORK:  pCI->G.p_costNetwork->vValidate(pCI, 1, this); break;
      case CK_COSTSTREAM:   pCI->G.p_costQuery->vValidate(pCI, 1, this); break;
      case CK_COSTUNICAST:  pCI->G.p_costUnicast->vValidate(pCI, 1, this); break;
      case CK_COUNT:        pCI->G.p_count->vValidate(pCI, 1, this); break;
      case CK_CROSSJOIN:    pCI->G.p_crossJoin->vValidate(pCI, 1, this); break;

      case CK_DIRECTION:    pCI->G.p_direction->vValidate(pCI, 1, this); break;

      case CK_EXPR:         pCI->G.p_expr->vValidate(pCI, 1, this); break;

      case CK_FILTER:       pCI->G.p_filter->vValidate(pCI, 1, this); break;
      case CK_FILTPRED:     pCI->G.p_filtPred->vValidate(pCI, 1, this); break;
      case CK_FILTPREDLIST: pCI->G.p_filtPredList->vValidate(pCI, 1, this); break;
      case CK_FLAT:         pCI->G.p_flat->vValidate(pCI, 1, this); break;
      case CK_FOLD:         pCI->G.p_fold->vValidate(pCI, 1, this); break;
      case CK_FOLDELIST:    pCI->G.p_foldElist->vValidate(pCI, 1, this); break;
      case CK_FROM:         pCI->G.p_from->vValidate(pCI, 1, this); break;

      case CK_GBYAGG:       pCI->G.p_gbyAgg->vValidate(pCI, 1, this); break;

      case CK_HASH:         pCI->G.p_hash->vValidate(pCI, 1, this); break;
      case CK_HAVING:       pCI->G.p_having->vValidate(pCI, 1, this); break;
      case CK_HSHCOL:       pCI->G.p_hshCol->vValidate(pCI, 1, this); break;

      case CK_IDXCOL:       pCI->G.p_idxCol->vValidate(pCI, 1, this); break;
      case CK_IDXCOLLIST:   pCI->G.p_idxColList->vValidate(pCI, 1, this); break;
      case CK_INDEX:        pCI->G.p_index->vValidate(pCI, 1, this); break;
      case CK_INDEXJOIN:    pCI->G.p_indexJoin->vValidate(pCI, 1, this); break;
      case CK_INDEXLIST:    pCI->G.p_indexList->vValidate(pCI, 1, this); break;
      case CK_INDEXNM:      pCI->G.p_indexNm->vValidate(pCI, 1, this); break;
      case CK_INDEXSCAN:    pCI->G.p_indexScan->vValidate(pCI, 1, this); break;

      case CK_JOINCAST:     pCI->G.p_joinCast->vValidate(pCI, 1, this); break;
      case CK_JOINESTIMATE: pCI->G.p_joinEstimate->vValidate(pCI, 1, this); break;
      case CK_JOINORDER:    pCI->G.p_joinOrder->vValidate(pCI, 1, this); break;
      case CK_JOINORDERING: pCI->G.p_joinOrdering->vValidate(pCI, 1, this); break;
      case CK_JOINPREDLIST: pCI->G.p_joinPredList->vValidate(pCI, 1, this); break;

      case CK_KEY:          pCI->G.p_key->vValidate(pCI, 1, this); break;
      case CK_KEYCELLS:     pCI->G.p_keyCells->vValidate(pCI, 1, this); break;
      case CK_KEYEXISTS:    pCI->G.p_keyExists->vValidate(pCI, 1, this); break;
      case CK_KEYNOTEXISTS: pCI->G.p_keyNotExists->vValidate(pCI, 1, this); break;

      case CK_LEVEL:        pCI->G.p_level->vValidate(pCI, 1, this); break;
      case CK_LIMIT:        pCI->G.p_limit->vValidate(pCI, 1, this); break;

      case CK_MAP:          pCI->G.p_map->vValidate(pCI, 1, this); break;
      case CK_MAPPING:      pCI->G.p_mapping->vValidate(pCI, 1, this); break;

      case CK_NCORRCOLS:    pCI->G.p_nCorrCols->vValidate(pCI, 1, this); break;
      case CK_NEEDED:       pCI->G.p_needed->vValidate(pCI, 1, this); break;
      case CK_NOTE:         pCI->G.p_note->vValidate(pCI, 1, this); break;
      case CK_NOTPRED:      pCI->G.p_notPred->vValidate(pCI, 1, this); break;

      case CK_ON:           pCI->G.p_on->vValidate(pCI, 1, this); break;
      case CK_ORDAGG:       pCI->G.p_ordAgg->vValidate(pCI, 1, this); break;
      case CK_ORPRED:       pCI->G.p_orPred->vValidate(pCI, 1, this); break;
      case CK_OUTERCOLNM:   pCI->G.p_outerColNm->vValidate(pCI, 1, this); break;
      case CK_OUTERJOIN:    pCI->G.p_outerJoin->vValidate(pCI, 1, this); break;

      case CK_PAYLOADCELLS: pCI->G.p_payloadCells->vValidate(pCI, 1, this); break;
      case CK_PROJECT:      pCI->G.p_project->vValidate(pCI, 1, this); break;
      case CK_PROJECTION:   pCI->G.p_projection->vValidate(pCI, 1, this); break;

      case CK_ROWCOUNT:     pCI->G.p_rowCount->vValidate(pCI, 1, this); break;
      case CK_ROWSIZE:      pCI->G.p_rowSize->vValidate(pCI, 1, this); break;
      case CK_RPNCALC:      pCI->G.p_rpnCalc->vValidate(pCI, 1, this); break;

      case CK_SCAN:         pCI->G.p_scan->vValidate(pCI, 1, this); break;
      case CK_SECTION:      pCI->G.p_section->vValidate(pCI, 1, this); break;
      case CK_SEND:         pCI->G.p_send->vValidate(pCI, 1, this); break;
      case CK_SORT:         pCI->G.p_sort->vValidate(pCI, 1, this); break;
      case CK_STAGE:        pCI->G.p_stage->vValidate(pCI, 1, this); break;
      case CK_STREAMHEAD:   pCI->G.p_streamHead->vValidate(pCI, 1, this); break;
      case CK_STREAMJOIN:   pCI->G.p_streamJoin->vValidate(pCI, 1, this); break;
      case CK_STREAMKEY:    pCI->G.p_streamKey->vValidate(pCI, 1, this); break;
      case CK_STREAMPER:    pCI->G.p_streamPer->vValidate(pCI, 1, this); break;
      case CK_STREAMPTN:    pCI->G.p_streamPtn->vValidate(pCI, 1, this); break;
      case CK_STREAMSET:    pCI->G.p_streamSet->vValidate(pCI, 1, this); break;
      case CK_SUBQUERY:     pCI->G.p_subQuery->vValidate(pCI, 1, this); break;
      case CK_SUBSET:       pCI->G.p_subSet->vValidate(pCI, 1, this); break;

      case CK_TABLE:        pCI->G.p_table->vValidate(pCI, 1, this); break;
      case CK_TABLELIST:    pCI->G.p_tableList->vValidate(pCI, 1, this); break;
      case CK_TABLENM:      pCI->G.p_tableNm->vValidate(pCI, 1, this); break;
      case CK_TABLEPTN:     pCI->G.p_tablePtn->vValidate(pCI, 1, this); break;
      case CK_TABLESIZE:    pCI->G.p_tableSize->vValidate(pCI, 1, this); break;
      case CK_TERM:         pCI->G.p_term->vValidate(pCI, 1, this); break;
      case CK_TOP:          pCI->G.p_top->vValidate(pCI, 1, this); break;
      case CK_TOPOLOGY:     pCI->G.p_topology->vValidate(pCI, 1, this); break;

      case CK_UNIQUE:       pCI->G.p_unique->vValidate(pCI, 1, this); break;

      case CK_WHERE:        pCI->G.p_where->vValidate(pCI, 1, this); break;

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
