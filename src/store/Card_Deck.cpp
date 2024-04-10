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
  while (pCI->bEachOp()) {

    if (CK_o_INDEXJOIN == pCI->E.e_Op ||
        CK_o_INDEXSCAN == pCI->E.e_Op ||
        CK_o_KEYEXISTS == pCI->E.e_Op ||
        CK_o_KEYNOTEXISTS == pCI->E.e_Op ||
        CK_o_SCAN == pCI->E.e_Op) {

      if (! p_OP_prev) {
        Card_Table* p_table_first = pCI->E.p_Op->pSub_table();
        p_streamPtn->vAddKey(p_table_first->pSub_tablePtn()->pSymbol());
      }
      else {
        eStageCast = pCI->E.p_deck->eAddStageCast_toPrev_cast(pCI,
                                                              p_streamPtn,
                                                              p_project_prior,
                                                              p_combined_prior,
                                                              p_cast_prior,
                                                              p_arriving,
                                                              (Card_OP*)pCI->E.p_Op);
        UNUSED(eStageCast);
        pCI->vResynchOp();
       // pCI->PRINTBOARD_dbg();
        vDebugPoint();
      }

      p_OP_prev = (Card_OP*)pCI->E.p_Op;
      p_project_prior  = nullptr;
      p_combined_prior = nullptr;
      p_cast_prior     = nullptr;
    }
    else if (CK_o_PROJECT == pCI->E.e_Op) {
      p_project_prior  = pCI->E.p_project;
      p_combined_prior = nullptr;
      p_cast_prior     = nullptr;
    }
    else if (CK_c_COMBINED == pCI->E.e_Op) {
      p_combined_prior = pCI->E.p_combined;
      p_cast_prior     = nullptr;
    }
    else if (CK_o_CAST == pCI->E.e_Op) {
      p_cast_prior = pCI->E.p_cast;
    }

    // -- -- -- -- --
    if (pCI->E.p_Op->pSub_emit())
      p_arriving = pCI->E.p_Op->pSub_emit();
  }

  delete p_streamPtn;

 // pCI->PRINTBOARD_dbg();
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

  RASSERT(CK_o_INDEXJOIN == p_OP_curr->eCard() ||
          CK_o_INDEXSCAN == p_OP_curr->eCard() ||
          CK_o_KEYEXISTS == p_OP_curr->eCard() ||
          CK_o_KEYNOTEXISTS == p_OP_curr->eCard() ||
          CK_o_SCAN == p_OP_curr->eCard());

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
      p_idxColList_join->pGetIdxCol_byIdxCol(p_term_idxCol_join->szFacet(SF_IDX_COL));
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
        pCI->vInsertBefore(p_combined, p_cast_prev);
      }

      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case XK_HASHCAST:
      p_cast_prev->SYM(SF_STG_CAST,"hashcast"_Stz);

      pCI->E.p_expr = pCI->pBackSearch_expr_For_col(p_term_tblCol_hash->pSymbol(), pCI->E.i_Op);
      p_hash_prior_new = Card_Hash::pDerive(pCI->E.p_expr, p_emitted_prev);

      if (p_combined_prev) {
        pCI->vInsertBefore(p_hash_prior_new, p_combined_prev);
      }
      else {
        pCI->vInsertBefore(p_hash_prior_new, p_cast_prev);
        Card_Combined* p_combined = new Card_Combined;
        pCI->vInsertBefore(p_combined, p_cast_prev);
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

  if (Card_Emit* p_emitted_fold = this->p_emit_fold())
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
    if (pMain->bIsCard(CK_c_FOLD))
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
    if (pMain->bIsCard(CK_c_FOLD))
      return _pCard_Fold(pMain);
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find emit prior to fold (reverse)                                          */
/*----------------------------------------------------------------------------*/
Card_Emit* Card_Deck::p_emit_fold(void)
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
/* validate children                                                          */
/*----------------------------------------------------------------------------*/
void Card_Deck::vValidate(CardIter* pCI)
{
  _precondition_(_MAGIC_, _FOOTER_);

  //vMarksTestRecursive();

  vMarksClearRecursive();
  vMark1Recursive(MARK_UNVALID);

  RASSERT(0 == pCI->nAncestors());

  for (Kid xDeck(this); xDeck.bEach(); /**/) {

    Card* pChild = xDeck.pChild();

    eCK eChild = pChild->eCard();

    pCI->vMember(eChild,pChild);

    switch (eChild) {

      case CK_c_ABSJOIN:      pCI->E.p_absJoin->vValidate(pCI, 1, this); break;
      case CK_o_AGGREG:       pCI->E.p_aggreg->vValidate(pCI, 1, this); break;
      case CK_ANDCONJ:      pCI->E.p_andConj->vValidate(pCI, 1, this); break;

      case CK_BOOLPRED:     pCI->E.p_boolPred->vValidate(pCI, 1, this); break;
      case CK_BOUND:        pCI->E.p_bound->vValidate(pCI, 1, this); break;
      case CK_BOUNDS:       pCI->E.p_bounds->vValidate(pCI, 1, this); break;

      case CK_o_CALL:         pCI->E.p_call->vValidate(pCI, 1, this); break;
      case CK_o_CAST:         pCI->E.p_cast->vValidate(pCI, 1, this); break;

      case CK_c_COMBINED:     pCI->E.p_combined->vValidate(pCI, 1, this); break;
      case CK_s_CONSTRAINT:   pCI->E.p_constraint->vValidate(pCI, 1, this); break;
      case CK_CORRCOLS:     pCI->E.p_corrCols->vValidate(pCI, 1, this); break;
      case CK_o_CORRELATION:  pCI->E.p_correlation->vValidate(pCI, 1, this); break;
      case CK_COSTJOINING:  pCI->E.p_costJoining->vValidate(pCI, 1, this); break;
      case CK_COSTNETWORK:  pCI->E.p_costNetwork->vValidate(pCI, 1, this); break;
      case CK_COSTSTREAM:   pCI->E.p_costQuery->vValidate(pCI, 1, this); break;
      case CK_COSTUNICAST:  pCI->E.p_costUnicast->vValidate(pCI, 1, this); break;
      case CK_COUNT:        pCI->E.p_count->vValidate(pCI, 1, this); break;
      case CK_o_CROSSJOIN:    pCI->E.p_crossJoin->vValidate(pCI, 1, this); break;

      case CK_t_EMIT:         pCI->E.p_emit->vValidate(pCI, 1, this); break;
      case CK_EXPR:         pCI->E.p_expr->vValidate(pCI, 1, this); break;

      case CK_o_FILTER:       pCI->E.p_filter->vValidate(pCI, 1, this); break;
      case CK_FILTPRED:     pCI->E.p_filtPred->vValidate(pCI, 1, this); break;
      case CK_FILTPREDLIST: pCI->E.p_filtPredList->vValidate(pCI, 1, this); break;
      case CK_c_FLAT:         pCI->E.p_flat->vValidate(pCI, 1, this); break;
      case CK_c_FOLD:         pCI->E.p_fold->vValidate(pCI, 1, this); break;
      case CK_o_FROM:         pCI->E.p_from->vValidate(pCI, 1, this); break;

      case CK_o_GBYAGG:       pCI->E.p_gbyAgg->vValidate(pCI, 1, this); break;

      case CK_o_HASH:         pCI->E.p_hash->vValidate(pCI, 1, this); break;
      case CK_o_HAVING:       pCI->E.p_having->vValidate(pCI, 1, this); break;
      case CK_t_HSHCOL:       pCI->E.p_hshCol->vValidate(pCI, 1, this); break;

      case CK_IDXCOL:       pCI->E.p_idxCol->vValidate(pCI, 1, this); break;
      case CK_IDXCOLLIST:   pCI->E.p_idxColList->vValidate(pCI, 1, this); break;
      case CK_INDEX:        pCI->E.p_index->vValidate(pCI, 1, this); break;
      case CK_o_INDEXJOIN:    pCI->E.p_indexJoin->vValidate(pCI, 1, this); break;
      case CK_t_INDEXLIST:    pCI->E.p_indexList->vValidate(pCI, 1, this); break;
      case CK_INDEXNM:      pCI->E.p_indexNm->vValidate(pCI, 1, this); break;
      case CK_o_INDEXSCAN:    pCI->E.p_indexScan->vValidate(pCI, 1, this); break;
      case CK_o_INFLATE:      pCI->E.p_inflate->vValidate(pCI, 1, this); break;
      case CK_s_INJECTABLE:     pCI->E.p_injectable->vValidate(pCI, 1, this); break;

      case CK_JOINCAST:     pCI->E.p_joinCast->vValidate(pCI, 1, this); break;
      case CK_JOINESTIMATE: pCI->E.p_joinEstimate->vValidate(pCI, 1, this); break;
      case CK_JOINORDER:    pCI->E.p_joinOrder->vValidate(pCI, 1, this); break;
      case CK_JOINORDERING: pCI->E.p_joinOrdering->vValidate(pCI, 1, this); break;
      case CK_JOINPREDLIST: pCI->E.p_joinPredList->vValidate(pCI, 1, this); break;

      case CK_s_KEY:          pCI->E.p_key->vValidate(pCI, 1, this); break;
      case CK_KEYCELLS:     pCI->E.p_keyCells->vValidate(pCI, 1, this); break;
      case CK_o_KEYEXISTS:    pCI->E.p_keyExists->vValidate(pCI, 1, this); break;
      case CK_o_KEYNOTEXISTS: pCI->E.p_keyNotExists->vValidate(pCI, 1, this); break;

      case CK_s_LEVEL:        pCI->E.p_level->vValidate(pCI, 1, this); break;
      case CK_o_LIMIT:        pCI->E.p_limit->vValidate(pCI, 1, this); break;

      case CK_MAP:          pCI->E.p_map->vValidate(pCI, 1, this); break;
      case CK_s_MAPPING:      pCI->E.p_mapping->vValidate(pCI, 1, this); break;

      case CK_s_NEEDED:       pCI->E.p_needed->vValidate(pCI, 1, this); break;
      case CK_NOTE:         pCI->E.p_note->vValidate(pCI, 1, this); break;
      case CK_NOTCONJ:      pCI->E.p_notConj->vValidate(pCI, 1, this); break;

      case CK_ON:           pCI->E.p_on->vValidate(pCI, 1, this); break;
      case CK_o_ORDAGG:       pCI->E.p_ordAgg->vValidate(pCI, 1, this); break;
      case CK_ORCONJ:       pCI->E.p_orConj->vValidate(pCI, 1, this); break;
      case CK_OUTERCOLNM:   pCI->E.p_outerColNm->vValidate(pCI, 1, this); break;
      case CK_OUTERJOIN:    pCI->E.p_outerJoin->vValidate(pCI, 1, this); break;

      case CK_PAYLOADCELLS: pCI->E.p_payloadCells->vValidate(pCI, 1, this); break;
      case CK_o_PROJECT:      pCI->E.p_project->vValidate(pCI, 1, this); break;

      case CK_RLNCONJ:      pCI->E.p_rlnConj->vValidate(pCI, 1, this); break;
      case CK_ROWCOUNT:     pCI->E.p_rowCount->vValidate(pCI, 1, this); break;
      case CK_ROWSIZE:      pCI->E.p_rowSize->vValidate(pCI, 1, this); break;

      case CK_o_SCAN:         pCI->E.p_scan->vValidate(pCI, 1, this); break;
      case CK_SECTION:      pCI->E.p_section->vValidate(pCI, 1, this); break;
      case CK_c_SEND:         pCI->E.p_send->vValidate(pCI, 1, this); break;
      case CK_o_SORT:         pCI->E.p_sort->vValidate(pCI, 1, this); break;
      case CK_c_STAGE:        pCI->E.p_stage->vValidate(pCI, 1, this); break;
      case CK_STREAMHEAD:   pCI->E.p_streamHead->vValidate(pCI, 1, this); break;
      case CK_STREAMJOIN:   pCI->E.p_streamJoin->vValidate(pCI, 1, this); break;
      case CK_STREAMKEY:    pCI->E.p_streamKey->vValidate(pCI, 1, this); break;
      case CK_STREAMPER:    pCI->E.p_streamPer->vValidate(pCI, 1, this); break;
      case CK_STREAMPTN:    pCI->E.p_streamPtn->vValidate(pCI, 1, this); break;
      case CK_STREAMSET:    pCI->E.p_streamSet->vValidate(pCI, 1, this); break;
      case CK_SUBQUERY:     pCI->E.p_subQuery->vValidate(pCI, 1, this); break;
      case CK_SUBSET:       pCI->E.p_subSet->vValidate(pCI, 1, this); break;

      case CK_s_TABLE:        pCI->E.p_table->vValidate(pCI, 1, this); break;
      case CK_TABLELIST:    pCI->E.p_tableList->vValidate(pCI, 1, this); break;
      case CK_t_TABLENM:      pCI->E.p_tableNm->vValidate(pCI, 1, this); break;
      case CK_TABLEPTN:     pCI->E.p_tablePtn->vValidate(pCI, 1, this); break;
      case CK_TABLESIZE:    pCI->E.p_tableSize->vValidate(pCI, 1, this); break;
      case CK_TERM:         pCI->E.p_term->vValidate(pCI, 1, this); break;
      case CK_o_TOP:          pCI->E.p_top->vValidate(pCI, 1, this); break;
      case CK_TOPOLOGY:     pCI->E.p_topology->vValidate(pCI, 1, this); break;

      case CK_UNIQUE:       pCI->E.p_unique->vValidate(pCI, 1, this); break;

      case CK_o_WHERE:        pCI->E.p_where->vValidate(pCI, 1, this); break;

      default:
        NEVER_REACH
    }

  }

  RASSERT(0 == pCI->nAncestors());

  vValidationPassed();

  vMark1RecursiveIf(MARK_BLANK, MARK_VALID);

  if (Card* pCard_notValidate = pFindMark1Recursive(MARK_UNVALID)) {
   // pCI->PRINTBOARD_dbg();
    pCard_notValidate->vDump(__func__);
    vDebugPoint();
    NEVER_REACH
  }

  if (Card* pCard_notValidate = pFindMark1Recursive(MARK_DUPLICATE)) {
   // pCI->PRINTBOARD_dbg();
    pCard_notValidate->vDump(__func__);
    vDebugPoint();
    NEVER_REACH
  }

  vMarksClearRecursive();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
