/*============================================================================*/
/* Card_Project.cpp -- Project                                                */
/*============================================================================*/

#include "Card_Project.hpp"

#include "Card_As.hpp"
#include "Card_Level.hpp"
#include "Card_Emit.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_Term.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Project,CK_PROJECT)
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

  Card_Project* p_project_unproject = new Card_Project(CK_PROJECT);
  pCI->G.pDeck->vAppendChild(p_project_unproject);
  p_project_unproject->pSymbol()->Y_vSet(P_Unproject);

  Card_Level* p_level = new Card_Level(AL_ORIGIN);
  p_project_unproject->vAppendChild(p_level);
  //pCI->G.p_level->Y_vSet(P_Level, (i8)AL_ORIGIN);

  Card_Stream* p_stream = new Card_Stream;
  p_project_unproject->vAppendChild(p_stream);

  Card_Emit* p_emitted_unproject = new Card_Emit;
  p_stream->vAppendChild(p_emitted_unproject);

  i8 i_Col = 0;
  i8 nChildren_projection = pCI->G.p_emit->nChildCards();
  for (i8 i_expr_projection = 0;
       i_expr_projection < nChildren_projection;
       i_expr_projection++) {

    i_Col++;

    pCI->G.p_expr = _pCard_Expr(pCI->G.p_emit->pGetChild(i_expr_projection));
    if (pCI->G.p_expr->pSymbol()->bMatches(SF_STM_COL,"$1"_Stz))
      continue;

   // pCI->vPrintBoard(__LINE__);

    Card_Expr* p_expr_simple = Card_Expr::pProj_Simple_expr_Of(pCI->G.p_expr,i_Col);

    p_emitted_unproject->vAppendChild(p_expr_simple);

   // pCI->vPrintBoard(__LINE__);

    vDebugPoint();
  }

  p_emitted_unproject->vRenumberAll_expr();
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Project::vBump_Rightwards(i8 nBumpBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_Emit* p_emit = pSub_emit())
    p_emit->vBump_Rightwards(nBumpBy);
}


/*----------------------------------------------------------------------------*/
/* apply stream                                                               */
/*----------------------------------------------------------------------------*/
void Card_Project::vComputeProjection(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  UNUSED(pCI);
}

/*----------------------------------------------------------------------------*/
/* craft projection for early project                                         */
/*----------------------------------------------------------------------------*/
Card_Emit* Card_Project::pCraft_emit_early(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Emit* p_emit_early = new Card_Emit;

  //pCI->Early.m_pNeeded_later->vDump();

  for (Kid x_needed(pCI->Early.m_pNeeded_later); x_needed.bEach(CK_EXPR); /**/) {
    Card_Expr* p_expr_needed = x_needed.p_expr();

    if (! pCI->Flux.m_pArriving->pContains(p_expr_needed))
      continue;

    Card_Expr* p_expr_clone = p_expr_needed->pClone();

    if (! p_expr_clone->pSub_as()) {
      Card_As* p_as_new = (new Card_As) ->SYM(SF_NMD_COL, p_emit_early->nChildCards());
      p_expr_clone->vAppendChild(p_as_new);

    }

    if (! p_expr_clone->pSymbol()->bHasFacet(SF_NMD_COL)) {
      Symbol oSymbol_nmdCol;
      oSymbol_nmdCol.vFacet(SF_NMD_COL, p_emit_early->nChildCards());
      p_expr_clone->pSymbol()->vAppendFacet(oSymbol_nmdCol.aFacet(SF_NMD_COL));
    }
    p_emit_early->vAppendChild(p_expr_clone);
  }


  p_emit_early->vRenumberAll_expr();

  return p_emit_early;
}

/*----------------------------------------------------------------------------*/
/* dedupe as                                                                  */
/*----------------------------------------------------------------------------*/
void Card_Project::vDedupe_as(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Emit* p_emit = pSub_emit())
    p_emit->vDedupe_as();

}

/*----------------------------------------------------------------------------*/
/* derive projection key                                                      */
/*----------------------------------------------------------------------------*/
void Card_Project::vDerive_streamKey(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // snap onto <project>/<projection>, delete existing <project>/<projection>/<key>
  RASSERT(bPeg_Projection(pCI));
  pCI->G.p_emit->vDelete_streamKey(pCI);

  // derive new <project>/<projection>/<key>
  pCI->G.p_streamKey = new Card_StreamKey;
  pCI->G.p_emit->vDerive_streamKey(pCI);

  if (! pCI->G.p_streamKey->nChildCards()) {

    delete pCI->G.p_streamKey;
    pCI->G.p_streamKey = nullptr;
    return;
  }
  pCI->G.p_emit->vAppendChild(pCI->G.p_streamKey);


  // set running projection for use by next op
  pCI->StreamOrder.m_pEmitted = pCI->G.p_emit;
  pCI->StreamOrder.m_pStreamKey = pCI->G.p_streamKey;
}

/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_Project::vEarlyProject_given_leaf(CardIter* pCI)
{
  //vDump(0,(Stz*)__func__);

  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->Early.m_pOP_leaf = this;

 // pCI->vPrintBoard();

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

  vCorrectStmCol(pCI->Flux.m_pArriving);

  if (! (pCI->G.p_needed = pSub_needed()))
    return;

  vEnsure_stream(pCI);

  pCI->G.p_emit = pCraft_emit_early(pCI);

  pCI->G.p_stream->vAppendChild(pCI->G.p_emit);

  //vInsertChildBefore(pCI->G.p_emit,pCI->G.p_needed);

  pCI->Flux.m_pArriving = pCI->G.p_emit;

 // pCI->vPrintBoard();

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* inject fold expr into this project                                         */
/*----------------------------------------------------------------------------*/
void Card_Project::vInject_foldExpr(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_expr = new Card_Expr;

  pCI->G.p_expr->pSymbol()->Y_vSet(P_Injected);

  pCI->G.p_expr->vAppendChild(pCI->Sqy.m_pExpr_fold->pSub_as()->pDupCardRecursive());

  pCI->G.p_rpnCalc = new Card_RpnCalc;
  pCI->G.p_expr->vAppendChild(pCI->G.p_rpnCalc);
  pCI->G.p_expr->vColType(pCI->G.p_rpnCalc);

  Symbol x_Symbol_stmCol1(SF_STM_COL, 1);
  pCI->G.p_term = pCI->Sqy.m_pExpr_fold->pSub_term()->pClone(&x_Symbol_stmCol1);

  pCI->G.p_rpnCalc->vAppendChild(pCI->G.p_term);

  pCI->G.p_emit = pSub_emit();

  pCI->G.p_emit->vPushChild(pCI->G.p_expr);
  pCI->G.p_emit->vRenumberAll_expr();

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
bool Card_Project::bIsLiterals(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSub_emit()->bIsLiterals())
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


  pCI->G.p_key = pSub_key();
  pCI->G.p_emit = pSub_emit();

  // get the root needed form of projection
  pCI->Early.m_pOP_root = this;
  pCI->Early.m_pOP_leaf = nullptr;
  pCI->Early.m_pNeeded_later  = pCI->G.p_emit->pCraftRoot_needed(pCI);

  // insert empty early projection (has only needed) before this ordAgg
//  Card_Project* p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
//  pCI->G_vInsertBefore(p_project_early, this);
//  pCI->G_vMainResynch();
  vAppendChild(pCI->Early.m_pNeeded_later->pDupCardRecursive());

 // pCI->vRenumberCards();
 // pCI->vPrintBoard();


  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

  RASSERT(pCI->Early.m_pOP_leaf);

  vCorrectStmCol(pCI->Flux.m_pArriving);

 // pCI->vPrintBoard();

  pCI->G.p_key = pSub_key();

 // pCI->vPrintBoard();

  pCI->G.p_emit = pSub_emit();

  pCI->Flux.m_pArriving = pCI->G.p_emit;

 // pCI->vPrintBoard();

  vDebugPoint();
}


/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Project::vSmooth_postAgg(CardIter* pCI, i8 nBumpBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_project->pSymbol()->Y_vSet(P_Smoothed);

  if (nBumpBy) {
    pCI->G.p_project->vBump_Rightwards(nBumpBy);
  }

  pCI->Flux.m_pArriving = pSub_emit();

}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Project::vSmooth_postAgg(CardIter* pCI, CarryForward* pCarryForward)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  UNUSED(pCarryForward);

  pCI->G.p_project->pSymbol()->Y_vSet(P_Smoothed);

  pCI->Flux.m_pArriving = pSub_emit();

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Project::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  vValidate_mustHave(CK_LEVEL,CK_STREAM,CK_NONE);

  Card::vValidate(pCI,iDepth,pParent);
  Card_OP::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
