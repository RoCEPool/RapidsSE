/*============================================================================*/
/* Card_Project.cpp -- Project                                                */
/*============================================================================*/

#include "Card_Project.hpp"

#include "Card_As.hpp"
#include "Card_Level.hpp"
#include "Card_Projection.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_Term.hpp"
#include "Card_Type.hpp"

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
: Card_OP(eCard)
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
  pCI->G.pDeck->vAppend(p_project_unproject);
  p_project_unproject->Y_vSet(P_Unproject);

  Card_Level* p_level = new Card_Level(AL_ORIGIN);
  p_project_unproject->vAppend(p_level);
  //pCI->G.p_level->Y_vSet(P_Level, (i8)AL_ORIGIN);

  Card_Projection* p_projection_unproject = new Card_Projection;
  p_project_unproject->vAppend(p_projection_unproject);

  i8 i_Col = 0;
  i8 nChildren_projection = pCI->G.p_projection->nChildCards();
  for (i8 i_expr_projection = 0;
       i_expr_projection < nChildren_projection;
       i_expr_projection++) {

    i_Col++;

    pCI->G.p_expr = _pCard_Expr(pCI->G.p_projection->pGetChild(i_expr_projection));
    if (pCI->G.p_expr->bIsValue("$1"_Stz))
      continue;

    pCI->G.p_as = pCI->G.p_expr->pSub_as();

    if (SymVal::bIsSqyColOrdinal(pCI->G.p_as->szValue()))
      continue;

    pCI->vPrintBoard(__LINE__);

    Card_Expr* p_expr_simple = Card_Expr::pProj_Simple_expr_Of(pCI->G.p_expr,i_Col);

    p_projection_unproject->vAppend(p_expr_simple);

    pCI->vPrintBoard(__LINE__);

    vDebugPoint();
  }

  p_projection_unproject->vRenumberAll_expr();
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

  if (Card_Projection* p_projection = pSub_projection())
    p_projection->vBump_Rightwards(nBumpBy);
}


/*----------------------------------------------------------------------------*/
/* apply stream                                                               */
/*----------------------------------------------------------------------------*/
void Card_Project::vComputeProjection(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* convert terms which are stmCol to tblCol                                   */
/*----------------------------------------------------------------------------*/
void Card_Project::vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if ((pCI->G.p_projection = pSub_projection()))
    pCI->G.p_projection->vConvert_stmColOdn_terms_To_tblColOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/* craft projection for early project                                         */
/*----------------------------------------------------------------------------*/
Card_Projection* Card_Project::pCraft_projection_early(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Projection* p_projection_early = new Card_Projection;

  //pCI->Early.m_pNeeded_later->vDump();

  for (Kid x_prior(pCI->Analysis.m_pProjection_prior); x_prior.bEach(CK_EXPR); /**/) {
    x_prior.p_expr()->vDump();

    if (! pCI->Early.m_pNeeded_later->pContains(x_prior.p_expr()))
      continue;
    if (p_projection_early->pContains(x_prior.p_expr()))
      continue;

    // denoted expr (of this denoted projection of denoted main op)
    //   is needed and not yet in early projection

    Card_Expr* p_expr_early = x_prior.p_expr()->pCraft_expr_early(pCI);
    p_projection_early->vAppend(p_expr_early);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (Kid x_needed(pCI->Early.m_pNeeded_later); x_needed.bEach(CK_EXPR); /**/) {
    if (! x_needed.p_expr()->pProjectedTerm()->bIsStmColOdn())
      continue;
    if (p_projection_early->pContains(x_needed.p_expr()))
      continue;

    Card_Expr* p_expr_early = x_needed.p_expr()->pClone(false);
    p_projection_early->vInsert(p_expr_early);
  }

  p_projection_early->vRenumberAll_expr();

  p_projection_early->vDump();

  return p_projection_early;
}


/*----------------------------------------------------------------------------*/
/* derive projection key                                                      */
/*----------------------------------------------------------------------------*/
void Card_Project::vDerive_Projection_Key(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // snap onto <project>/<projection>, delete existing <project>/<projection>/<key>
  pCI->G.p_projection = pSub_projection();
  pCI->G.p_projection->vDelete_key(pCI);

  // derive new <project>/<projection>/<key>
  pCI->G.p_key = new Card_Key;
  pCI->G.p_projection->vKeyProjection(pCI);
  pCI->G.p_projection->vAppend(pCI->G.p_key);

  // set running projection for use by next op
  pCI->StreamOrder.m_pProjection_running = pCI->G.p_projection;
  pCI->StreamOrder.m_pKey_running = pCI->G.p_key;
}

/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_Project::vEarlyProject_given_leaf(CardIter* pCI)
{
  vDump(0,(Stz*)__func__);

  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->Early.m_pOP_leaf = this;

  pCI->vPrintBoard();

  vDebugPoint();
}


/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_Project::vEarlyProject_needed_recurse(CardIter* pCI)
{
  vDump(0,(Stz*)__func__);

  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

  RASSERT(pCI->Early.m_pOP_leaf);

  if (! (pCI->G.p_needed = pSub_needed()))
    return;

  pCI->G.p_projection = pCraft_projection_early(pCI);

  vInsertBefore(pCI->G.p_projection,pCI->G.p_needed);

  pCI->Analysis.m_pProjection_prior = pCI->G.p_projection;

  pCI->vPrintBoard();

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* inject fold expr into this project                                         */
/*----------------------------------------------------------------------------*/
void Card_Project::vInject_foldExpr(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_expr = new Card_Expr;

  pCI->G.p_expr->Y_vSet(P_Injected);

  pCI->G.p_expr->vAppend(pCI->Sqy.m_pExpr_fold->pSub_type()->pDupCardAndSubCards());

  pCI->G.p_expr->vAppend(pCI->Sqy.m_pExpr_fold->pSub_as()->pDupCardAndSubCards());

  pCI->G.p_rpnCalc = new Card_RpnCalc;
  pCI->G.p_expr->vAppend(pCI->G.p_rpnCalc);

  pCI->G.p_term = pCI->Sqy.m_pExpr_fold->pSub_term()->pClone("$1"_Stz);

  pCI->G.p_rpnCalc->vAppend(pCI->G.p_term);

  pCI->G.p_projection = pSub_projection();

  pCI->G.p_projection->vPush(pCI->G.p_expr);
  pCI->G.p_projection->vRenumberAll_expr();

}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Project::vSmooth_postAgg(CardIter* pCI, i8 nBumpBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_project->Y_vSet(P_Smoothed);

  if (nBumpBy) {
    pCI->G.p_project->vBump_Rightwards(nBumpBy);
  }

  pCI->Analysis.m_pProjection_prior = pSub_projection();

}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Project::vSmooth_postAgg(CardIter* pCI, CarryForward* pCarryForward)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_project->Y_vSet(P_Smoothed);

  if (pCarryForward->Project_postAgg.m_nBumpBy) {
    pCI->G.p_project->vBump_Rightwards(pCarryForward->Project_postAgg.m_nBumpBy);
  }

  pCI->Analysis.m_pProjection_prior = pSub_projection();

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
