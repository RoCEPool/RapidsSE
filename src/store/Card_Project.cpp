/*============================================================================*/
/* Card_Project.cpp -- Project                                                */
/*============================================================================*/

#include "Card_Project.hpp"

#include "Card_As.hpp"
#include "Card_Level.hpp"
#include "Card_Projection.hpp"
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
  p_project_unproject->pSymbol()->Y_vSet(P_Unproject);

  Card_Level* p_level = new Card_Level(AL_ORIGIN);
  p_project_unproject->vAppend(p_level);
  //pCI->G.p_level->Y_vSet(P_Level, (i8)AL_ORIGIN);

  Card_Projection* p_projection_unproject = new Card_Projection(pCI->iXform());
  p_project_unproject->vAppend(p_projection_unproject);

  i8 i_Col = 0;
  i8 nChildren_projection = pCI->G.p_projection->nChildCards();
  for (i8 i_expr_projection = 0;
       i_expr_projection < nChildren_projection;
       i_expr_projection++) {

    i_Col++;

    pCI->G.p_expr = _pCard_Expr(pCI->G.p_projection->pGetChild(i_expr_projection));
    if (pCI->G.p_expr->bSameSymbol("$1"_Stz))
      continue;

    pCI->G.p_as = pCI->G.p_expr->pSub_as();

    if (pCI->G.p_as->pSymbol()->bIsSubQueryColumn())
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
    pCI->G.p_projection->vConvertAll_stmOdn_To_colOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/* craft projection for early project                                         */
/*----------------------------------------------------------------------------*/
Card_Projection* Card_Project::pCraft_projection_early(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Projection* p_projection_early = new Card_Projection(pCI->iXform());

  //pCI->Early.m_pNeeded_later->vDump();

  for (Kid x_needed(pCI->Early.m_pNeeded_later); x_needed.bEach(CK_EXPR); /**/) {
    Card_Expr* p_expr_needed = x_needed.p_expr();

//    if (! p_expr_needed->pGetTerm(0)->bIsColOdn()) {
//      this->vDump(__func__ );
//      pCI->Prior.m_pProjection->vDump(__func__);
//      p_expr_needed->vDump(__func__);
//      NEVER_REACH;
//      //continue;
//    }

    if (! pCI->Prior.m_pProjection->pContains(p_expr_needed))
      continue;

    Card_Expr* p_expr_clone = p_expr_needed->pClone();

    if (! p_expr_clone->pSub_as()) {
      Card_As* p_as_new = new Card_As(p_projection_early->nChildCards());
      p_expr_clone->vAppend(p_as_new);
    }

    p_projection_early->vAppend(p_expr_clone);
  }


  p_projection_early->vRenumberAll_expr();

  return p_projection_early;
}

/*----------------------------------------------------------------------------*/
/* dedupe as                                                                  */
/*----------------------------------------------------------------------------*/
void Card_Project::vDedupe_as(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Projection* p_projection = pSub_projection())
    p_projection->vDedupe_as();

}

/*----------------------------------------------------------------------------*/
/* derive projection key                                                      */
/*----------------------------------------------------------------------------*/
void Card_Project::vDerive_Projection_Key(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // snap onto <project>/<projection>, delete existing <project>/<projection>/<key>
  pCI->G.p_projection = pSub_projection();
  pCI->G.p_projection->vDeletePrjKey(pCI);

  // derive new <project>/<projection>/<key>
  pCI->G.p_key = new Card_Key;
  pCI->G.p_projection->vDerivePrjKey(pCI);

  if (! pCI->G.p_key->nChildCards()) {
    delete pCI->G.p_key;
    pCI->StreamOrder.m_pKey_running = nullptr;
    return;
  }
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
  //vDump(0,(Stz*)__func__);

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
  //vDump(0,(Stz*)__func__);

  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

  RASSERT(pCI->Early.m_pOP_leaf);

  vCorrectStmOdn(pCI->Prior.m_pProjection);

  if (! (pCI->G.p_needed = pSub_needed()))
    return;

  pCI->G.p_projection = pCraft_projection_early(pCI);

  vInsertBefore(pCI->G.p_projection,pCI->G.p_needed);

  pCI->Prior.m_pProjection = pCI->G.p_projection;

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

  pCI->G.p_expr->pSymbol()->Y_vSet(P_Injected);

  pCI->G.p_expr->vAppend(pCI->Sqy.m_pExpr_fold->pSub_as()->pDupCardRecursive());

  pCI->G.p_rpnCalc = new Card_RpnCalc;
  pCI->G.p_expr->vAppend(pCI->G.p_rpnCalc);
  pCI->G.p_expr->vColType(pCI->G.p_rpnCalc);

  Symbol x_Symbol_stmCol1;
  x_Symbol_stmCol1.vInit_Col_No(1);
  pCI->G.p_term = pCI->Sqy.m_pExpr_fold->pSub_term()->pClone(&x_Symbol_stmCol1);

  pCI->G.p_rpnCalc->vAppend(pCI->G.p_term);

  pCI->G.p_projection = pSub_projection();

  pCI->G.p_projection->vPush(pCI->G.p_expr);
  pCI->G.p_projection->vRenumberAll_expr();

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
  pCI->G.p_projection = pSub_projection();

  // get the root needed form of projection
  pCI->Early.m_pOP_root = this;
  pCI->Early.m_pOP_leaf = nullptr;
  pCI->Early.m_pNeeded_later  = pCI->G.p_projection->pCraftRoot_needed(pCI);

  // insert empty early projection (has only needed) before this ordAgg
//  Card_Project* p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
//  pCI->G_vInsertBefore(p_project_early, this);
//  pCI->G_vMainResynch();
  vAppend(pCI->Early.m_pNeeded_later->pDupCardRecursive());

  pCI->vRenumberCards();
  pCI->vPrintBoard();


  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

  RASSERT(pCI->Early.m_pOP_leaf);

  vCorrectStmOdn(pCI->Prior.m_pProjection);

  pCI->vPrintBoard();

  pCI->G.p_key = pSub_key();

  pCI->vPrintBoard();

  pCI->G.p_projection = pSub_projection();

  pCI->Prior.m_pProjection = pCI->G.p_projection;

  pCI->vPrintBoard();

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

  pCI->Prior.m_pProjection = pSub_projection();

}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Project::vSmooth_postAgg(CardIter* pCI, CarryForward* pCarryForward)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_project->pSymbol()->Y_vSet(P_Smoothed);

  if (pCarryForward->Project_postAgg.m_nBumpBy) {
    pCI->G.p_project->vBump_Rightwards(pCarryForward->Project_postAgg.m_nBumpBy);
  }

  pCI->Prior.m_pProjection = pSub_projection();

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Project::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_LEVEL,CK_PROJECTION,CK_NONE);

  pSub_level()->vValidate(pCI,iDepth+1,this);
  pSub_projection()->vValidate(pCI,iDepth+1,this);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
