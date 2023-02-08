/*============================================================================*/
/* Card_Project.cpp -- Project                                                */
/*============================================================================*/

#include "Card_Project.hpp"

#include "Card_As.hpp"
#include "Card_Level.hpp"
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
void Card_Project::vUnproject_foldExpr(CardIter* pCI)
{
  _static_

  //pCI->G.p_projection = pCI->G.p_project->pSub_projection();

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

    if (Card::bIsSqyColOrdinal(pCI->G.p_as->szValue()))
      continue;

    Card_Expr* p_expr_simple = Card_Expr::pProj_Simple_expr_Of(pCI->G.p_expr,i_Col);

    p_projection_unproject->vAppend(p_expr_simple);
  }

  p_projection_unproject->vRenumberAllExpr();
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
/* inject fold expr into this project                                         */
/*----------------------------------------------------------------------------*/
void Card_Project::vInject_foldExpr(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_expr = new Card_Expr;

  pCI->G.p_expr->Y_vSet(P_Injected);

  pCI->G.p_expr->vAppend(pCI->m_pExpr_fold->pSub_type()->pDupCardAndSubCards());

  pCI->G.p_expr->vAppend(pCI->m_pExpr_fold->pSub_as()->pDupCardAndSubCards());

  pCI->G.p_rpnCalc = new Card_RpnCalc;
  pCI->G.p_expr->vAppend(pCI->G.p_rpnCalc);

  pCI->G.p_term = new Card_Term("$1"_Stz);

  pCI->G.p_rpnCalc->vAppend(pCI->G.p_term);

  pCI->G.p_projection = pSub_projection();

  pCI->G.p_projection->vPush(pCI->G.p_expr);
  pCI->G.p_projection->vRenumberAllExpr();

}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Project::vSmooth(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_project->Y_vSet(P_Smoothed);

  pCI->G.p_project->vBump_Rightwards(1);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
