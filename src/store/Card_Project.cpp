/*============================================================================*/
/* Card_Project.cpp -- Project                                                */
/*============================================================================*/

#include "Card_Project.hpp"

#include "Card_Level.hpp"
#include "Card_Emit.hpp"
#include "Card_Term.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Project, CK_o_PROJECT)
__aNewFunction(Card_Project,CN_CARD)
__vDeleteFunction(Card_Project,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Project::Card_Project(eCK eCard)
: Card_OP(eCard,1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Project::~Card_Project(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Project::vUnproject_foldExpr(CardIter* pCI,
                                       CarryForward* pCarryForward)
{
  _static_

  RASSERT(pCarryForward->Unproject.m_bUnproject_foldExpr);

  Card_Project* p_project_unproject = new Card_Project(CK_o_PROJECT);
  pCI->E.p_deck->vAppendChild(p_project_unproject);
  p_project_unproject->pSymbol()->Y_vSet(P_Unproject);

  Card_Level* p_level = new Card_Level(AL_ORIGIN);
  p_project_unproject->vAppendChild(p_level);

  Card_Stream* p_stream = new Card_Stream;
  p_project_unproject->vAppendChild(p_stream);

  Card_Emit* p_emitted_unproject = new Card_Emit;
  p_stream->vAppendChild(p_emitted_unproject);

  i8 i_Col = 0;
  i8 nChildren_projection = pCI->E.p_emit->nChildCards();
  for (i8 i_expr_projection = 0;
       i_expr_projection < nChildren_projection;
       i_expr_projection++) {

    i_Col++;

    pCI->E.p_expr = _pCard_Expr(pCI->E.p_emit->pGetChild(i_expr_projection));
    if (pCI->E.p_expr->pSymbol()->bMatches(SF_EXP_COL, "$1"_Stz))
      continue;

    Card_Expr* p_expr_simple = Card_Expr::pCraft_expr_Of_expr(pCI->E.p_expr, i_Col);
    p_expr_simple->vDeleteFacetRecursive(SF_KEY_COL);

    p_emitted_unproject->vAppendChild(p_expr_simple);

    vDebugPoint();
  }

  p_emitted_unproject->vRenumberAll_expr();
  //p_emitted_unproject->vDump(__func__);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* craft emit for early project                                               */
/*----------------------------------------------------------------------------*/
Card_Emit* Card_Project::pCraft_emit_early(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (8 == pCI->m_pBench->iBenchNo() && 57 == pCI->m_pBench->nPrintBoard()) {
    //pCI->E.p_Op->vDump(__func__);
    //this->vDump(__func__);
    //pCI->Early.m_pNeeded_later->vDump(__func__);
    vDebugPoint();
  }

  Card_Emit* p_emit_early = new Card_Emit;

//  pCI->Early.m_pNeeded_later->vDump(__func__);
//  pCI->Flux.m_pArriving->vDump(__func__);

  for (Kid x_needed(pCI->Early.m_pNeeded_later); x_needed.bEach(CK_EXPR); /**/) {
    Card_Expr* p_expr_needed = x_needed.p_expr();
    if (8 == pCI->m_pBench->iBenchNo() && 57 == pCI->m_pBench->nPrintBoard()) {
      //p_expr_needed->vDump(__func__);
      vDebugPoint();
    }

      //RASSERT(pCI->Dic.m_p_expr == pCI->Flux.m_pArriving);
    Card_Expr* p_expr_arriving = pCI->Flux.m_pArriving->pContains(p_expr_needed);
    if (! p_expr_arriving) {
      //vTrace("NOT MATCHING\n"_Stz);
      continue;
    }

    //vTrace("MATCHING\n"_Stz);

    Card_Expr* p_expr_clone = p_expr_arriving->pClone();

    if (! p_expr_clone->pSymbol()->bHasFacet(SF_NMD_COL)) {
      Symbol oSymbol_nmdCol;
      oSymbol_nmdCol.vApplyFacet(SF_NMD_COL, p_emit_early->nChildCards());
      p_expr_clone->pSymbol()->vApplyFacet(oSymbol_nmdCol.aFacet(SF_NMD_COL));
    }
    p_emit_early->vAppendChild(p_expr_clone);
  }


  p_emit_early->vRenumberAll_expr();

  if (8 == pCI->m_pBench->iBenchNo() && 57 == pCI->m_pBench->nPrintBoard()) {
    //p_emit_early->vDump(__func__);
    vDebugPoint();
  }

  return p_emit_early;
}

/*----------------------------------------------------------------------------*/
/* derive projection key                                                      */
/*----------------------------------------------------------------------------*/
void Card_Project::vDerive_streamKey(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // snap onto <project>/<projection>, delete existing <project>/<projection>/<key>
  RASSERT(bPeg_Projection(pCI));
  pCI->E.p_emit->vDelete_streamKey(pCI);

  // derive new <project>/<projection>/<key>
  pCI->E.p_streamKey = new Card_StreamKey;
  pCI->E.p_emit->vDerive_streamKey(pCI);

  if (! pCI->E.p_streamKey->nChildCards()) {

    delete pCI->E.p_streamKey;
    pCI->E.p_streamKey = nullptr;
    return;
  }
  pCI->E.p_emit->vAppendChild(pCI->E.p_streamKey);


  // set running projection for use by next op
  pCI->StreamOrder.m_pEmitted = pCI->E.p_emit;
  pCI->StreamOrder.m_pStreamKey = pCI->E.p_streamKey;
}

/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_Project::vEarlyProject_given_leaf(CardIter* pCI)
{
  //vDump(0,(Stz*)__func__);

  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->Early.m_pOP_leaf = this;

 // pCI->PRINTBOARD_dbg();

  vDebugPoint();
}


/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_Project::vEarlyProject_recurse_needed(CardIter* pCI)
{
  //vDump(0,(Stz*)__func__);

  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

  RASSERT(pCI->Early.m_pOP_leaf);
  RASSERT(pCI->E.p_project->pSymbol()->Y_bHas(P_Early));

  if (! (pCI->E.p_needed = pSub_needed()))
    return;

  vEnsure_stream(pCI);

  pCI->E.p_emit = pCraft_emit_early(pCI);

  pCI->E.p_stream->vAppendChild(pCI->E.p_emit);

  //vInsertChildBefore(pCI->G.p_emit,pCI->G.p_needed);

  pCI->Dic.m_p_expr = pCI->Flux.m_pArriving = pCI->E.p_emit;

 // pCI->PRINTBOARD_dbg();

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* is this a compound project?                                                */
/*----------------------------------------------------------------------------*/
bool Card_Project::bIsCompound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSub_emit()->bIsCompound())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a compound project?                                                */
/*----------------------------------------------------------------------------*/
bool Card_Project::bIsLiterals(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSub_emit()->bIsLiterals(pCI))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_Project::vEarlyProject_root(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bIsPartitionLevel());

  RASSERT(! pCI->Early.m_pNeeded_later);


  pCI->E.p_key = pSub_key();
  pCI->E.p_emit = pSub_emit();

  // get the root needed form of projection
  pCI->Early.m_pOP_root = this;
  pCI->Early.m_pOP_leaf = nullptr;
  pCI->Early.m_pNeeded_later  = pCI->E.p_emit->pCraftRoot_needed(pCI);

  // insert empty early projection (has only needed) before this ordAgg
//  Card_Project* p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
//  pCI->vInsertBefore(p_project_early, this);
//  pCI->vResynchOp();
  vAppendChild(pCI->Early.m_pNeeded_later->pDupCardRecursive());

  Card_OP::vEarlyProject_backstep(pCI);

  pCI->E.p_Op = this;
  pCI->E.e_Op = CK_o_PROJECT;

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

  RASSERT(pCI->Early.m_pOP_leaf);
  RASSERT(! pCI->E.p_project->pSymbol()->Y_bHas(P_Early));

  ((Card_OP*)pCI->E.p_Op)->vRestate_emit(pCI);
}

/*----------------------------------------------------------------------------*/
/* craft needed                                                               */
/*----------------------------------------------------------------------------*/
void Card_Project::vCraft_needed_root(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->Early.m_pNeeded_later = pCI->E.p_emit->pCraft_needed_root(pCI);
  DELETE_CARD(pCI->Early.m_pNeeded_later);
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Project::vSmooth_postAgg(CardIter* pCI, CarryForward* pCarryForward)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  UNUSED(pCarryForward);

  pCI->E.p_project->pSymbol()->Y_vSet(P_Smoothed);

  pCI->Dic.m_p_expr = pCI->Flux.m_pArriving = pSub_emit();

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Project::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

#ifdef Z_NO_STREAM
  vValidate_mustHave(CK_s_LEVEL, CK_t_EMIT, CK_NONE);
#else
  vValidate_mustHave(CK_s_LEVEL, CK_s_STREAM, CK_NONE);
#endif

  Card::vValidate(pCI,iDepth,pParent);
  Card_OP::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
