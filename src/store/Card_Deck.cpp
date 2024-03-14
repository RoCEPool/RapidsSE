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

    if (CK_o_INDEXJOIN == pCI->G.e_MainCard ||
        CK_o_INDEXSCAN == pCI->G.e_MainCard ||
        CK_o_KEYEXISTS == pCI->G.e_MainCard ||
        CK_o_KEYNOTEXISTS == pCI->G.e_MainCard ||
        CK_o_SCAN == pCI->G.e_MainCard) {

      if (! p_OP_prev) {
        Card_Table* p_table_first = pCI->G.p_MainCard->pSub_table();
        p_streamPtn->vAddKey(p_table_first->pSub_tablePtn()->pSymbol());
      }
      else {
        eStageCast = pCI->G.pDeck->eAddStageCast_toPrev_cast(pCI,
                                                             p_streamPtn,
                                                             p_project_prior,
                                                             p_combined_prior,
                                                             p_cast_prior,
                                                             p_arriving,
                                                             (Card_OP*)pCI->G.p_MainCard);
        UNUSED(eStageCast);
        pCI->vResynchOp();
       // pCI->PRINTBOARD_dbg();
        vDebugPoint();
      }

      p_OP_prev = (Card_OP*)pCI->G.p_MainCard;
      p_project_prior  = nullptr;
      p_combined_prior = nullptr;
      p_cast_prior     = nullptr;
    }
    else if (CK_o_PROJECT == pCI->G.e_MainCard) {
      p_project_prior  = pCI->G.p_project;
      p_combined_prior = nullptr;
      p_cast_prior     = nullptr;
    }
    else if (CK_c_COMBINED == pCI->G.e_MainCard) {
      p_combined_prior = pCI->G.p_combined;
      p_cast_prior     = nullptr;
    }
    else if (CK_o_CAST == pCI->G.e_MainCard) {
      p_cast_prior = pCI->G.p_cast;
    }

    // -- -- -- -- --
    if (pCI->G.p_MainCard->pSub_emit())
      p_arriving = pCI->G.p_MainCard->pSub_emit();
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

      pCI->G.p_expr = pCI->pBackSearch_expr_For_col(p_term_tblCol_hash->pSymbol(), pCI->G.i_MainCard);
      p_hash_prior_new = Card_Hash::pDerive(pCI->G.p_expr,p_emitted_prev);

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

  for (Kid xKid(this); xKid.bEach(); /**/) {

    Card* pChild = xKid.pChild();

    eCK eChild = pChild->eCard();

    pCI->vMember(eChild,pChild);

    switch (eChild) {

      case CK_c_ABSJOIN:      pCI->G.p_absJoin->vValidate(pCI, 1, this); break;
      case CK_o_AGGREG:       pCI->G.p_aggreg->vValidate(pCI, 1, this); break;
      case CK_ANDCONJ:      pCI->G.p_andConj->vValidate(pCI, 1, this); break;

      case CK_BOOLPRED:     pCI->G.p_boolPred->vValidate(pCI, 1, this); break;
      case CK_BOUND:        pCI->G.p_bound->vValidate(pCI, 1, this); break;
      case CK_BOUNDS:       pCI->G.p_bounds->vValidate(pCI, 1, this); break;

      case CK_o_CALL:         pCI->G.p_call->vValidate(pCI, 1, this); break;
      case CK_o_CAST:         pCI->G.p_cast->vValidate(pCI, 1, this); break;

      case CK_c_COMBINED:     pCI->G.p_combined->vValidate(pCI, 1, this); break;
      case CK_s_CONSTRAINT:   pCI->G.p_constraint->vValidate(pCI, 1, this); break;
      case CK_CORRCOLS:     pCI->G.p_corrCols->vValidate(pCI, 1, this); break;
      case CK_o_CORRELATION:  pCI->G.p_correlation->vValidate(pCI, 1, this); break;
      case CK_COSTJOINING:  pCI->G.p_costJoining->vValidate(pCI, 1, this); break;
      case CK_COSTNETWORK:  pCI->G.p_costNetwork->vValidate(pCI, 1, this); break;
      case CK_COSTSTREAM:   pCI->G.p_costQuery->vValidate(pCI, 1, this); break;
      case CK_COSTUNICAST:  pCI->G.p_costUnicast->vValidate(pCI, 1, this); break;
      case CK_COUNT:        pCI->G.p_count->vValidate(pCI, 1, this); break;
      case CK_o_CROSSJOIN:    pCI->G.p_crossJoin->vValidate(pCI, 1, this); break;

      case CK_DIRECTION:    pCI->G.p_direction->vValidate(pCI, 1, this); break;

      case CK_t_EMIT:         pCI->G.p_emit->vValidate(pCI, 1, this); break;
      case CK_EXPR:         pCI->G.p_expr->vValidate(pCI, 1, this); break;

      case CK_o_FILTER:       pCI->G.p_filter->vValidate(pCI, 1, this); break;
      case CK_FILTPRED:     pCI->G.p_filtPred->vValidate(pCI, 1, this); break;
      case CK_FILTPREDLIST: pCI->G.p_filtPredList->vValidate(pCI, 1, this); break;
      case CK_c_FLAT:         pCI->G.p_flat->vValidate(pCI, 1, this); break;
      case CK_c_FOLD:         pCI->G.p_fold->vValidate(pCI, 1, this); break;
      case CK_o_FROM:         pCI->G.p_from->vValidate(pCI, 1, this); break;

      case CK_o_GBYAGG:       pCI->G.p_gbyAgg->vValidate(pCI, 1, this); break;

      case CK_o_HASH:         pCI->G.p_hash->vValidate(pCI, 1, this); break;
      case CK_o_HAVING:       pCI->G.p_having->vValidate(pCI, 1, this); break;
      case CK_t_HSHCOL:       pCI->G.p_hshCol->vValidate(pCI, 1, this); break;

      case CK_IDXCOL:       pCI->G.p_idxCol->vValidate(pCI, 1, this); break;
      case CK_IDXCOLLIST:   pCI->G.p_idxColList->vValidate(pCI, 1, this); break;
      case CK_INDEX:        pCI->G.p_index->vValidate(pCI, 1, this); break;
      case CK_o_INDEXJOIN:    pCI->G.p_indexJoin->vValidate(pCI, 1, this); break;
      case CK_t_INDEXLIST:    pCI->G.p_indexList->vValidate(pCI, 1, this); break;
      case CK_INDEXNM:      pCI->G.p_indexNm->vValidate(pCI, 1, this); break;
      case CK_o_INDEXSCAN:    pCI->G.p_indexScan->vValidate(pCI, 1, this); break;
      case CK_o_INFLATE:      pCI->G.p_inflate->vValidate(pCI, 1, this); break;
      case CK_s_INJECTABLE:     pCI->G.p_injectable->vValidate(pCI, 1, this); break;

      case CK_JOINCAST:     pCI->G.p_joinCast->vValidate(pCI, 1, this); break;
      case CK_JOINESTIMATE: pCI->G.p_joinEstimate->vValidate(pCI, 1, this); break;
      case CK_JOINORDER:    pCI->G.p_joinOrder->vValidate(pCI, 1, this); break;
      case CK_JOINORDERING: pCI->G.p_joinOrdering->vValidate(pCI, 1, this); break;
      case CK_JOINPREDLIST: pCI->G.p_joinPredList->vValidate(pCI, 1, this); break;

      case CK_s_KEY:          pCI->G.p_key->vValidate(pCI, 1, this); break;
      case CK_KEYCELLS:     pCI->G.p_keyCells->vValidate(pCI, 1, this); break;
      case CK_o_KEYEXISTS:    pCI->G.p_keyExists->vValidate(pCI, 1, this); break;
      case CK_o_KEYNOTEXISTS: pCI->G.p_keyNotExists->vValidate(pCI, 1, this); break;

      case CK_s_LEVEL:        pCI->G.p_level->vValidate(pCI, 1, this); break;
      case CK_o_LIMIT:        pCI->G.p_limit->vValidate(pCI, 1, this); break;

      case CK_MAP:          pCI->G.p_map->vValidate(pCI, 1, this); break;
      case CK_s_MAPPING:      pCI->G.p_mapping->vValidate(pCI, 1, this); break;

      case CK_s_NEEDED:       pCI->G.p_needed->vValidate(pCI, 1, this); break;
      case CK_NOTE:         pCI->G.p_note->vValidate(pCI, 1, this); break;
      case CK_NOTCONJ:      pCI->G.p_notConj->vValidate(pCI, 1, this); break;

      case CK_ON:           pCI->G.p_on->vValidate(pCI, 1, this); break;
      case CK_o_ORDAGG:       pCI->G.p_ordAgg->vValidate(pCI, 1, this); break;
      case CK_ORCONJ:       pCI->G.p_orConj->vValidate(pCI, 1, this); break;
      case CK_OUTERCOLNM:   pCI->G.p_outerColNm->vValidate(pCI, 1, this); break;
      case CK_OUTERJOIN:    pCI->G.p_outerJoin->vValidate(pCI, 1, this); break;

      case CK_PAYLOADCELLS: pCI->G.p_payloadCells->vValidate(pCI, 1, this); break;
      case CK_o_PROJECT:      pCI->G.p_project->vValidate(pCI, 1, this); break;

      case CK_RLNCONJ:      pCI->G.p_rlnConj->vValidate(pCI, 1, this); break;
      case CK_ROWCOUNT:     pCI->G.p_rowCount->vValidate(pCI, 1, this); break;
      case CK_ROWSIZE:      pCI->G.p_rowSize->vValidate(pCI, 1, this); break;
      case CK_RPNCALC:      pCI->G.p_rpnCalc->vValidate(pCI, 1, this); break;

      case CK_o_SCAN:         pCI->G.p_scan->vValidate(pCI, 1, this); break;
      case CK_SECTION:      pCI->G.p_section->vValidate(pCI, 1, this); break;
      case CK_c_SEND:         pCI->G.p_send->vValidate(pCI, 1, this); break;
      case CK_o_SORT:         pCI->G.p_sort->vValidate(pCI, 1, this); break;
      case CK_c_STAGE:        pCI->G.p_stage->vValidate(pCI, 1, this); break;
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
      case CK_o_TOP:          pCI->G.p_top->vValidate(pCI, 1, this); break;
      case CK_TOPOLOGY:     pCI->G.p_topology->vValidate(pCI, 1, this); break;

      case CK_UNIQUE:       pCI->G.p_unique->vValidate(pCI, 1, this); break;

      case CK_o_WHERE:        pCI->G.p_where->vValidate(pCI, 1, this); break;

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
