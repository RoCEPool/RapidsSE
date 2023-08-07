/*============================================================================*/
/* Card_Having.cpp -- Having                                                  */
/*============================================================================*/

#include "Card_Having.hpp"

#include "CardIter.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Having,CK_HAVING)
__aNewFunction(Card_Having,CN_CARD)
__vDeleteFunction(Card_Having,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Having::Card_Having(void)
: Card_COND(CK_HAVING)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Having::~Card_Having(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* acclimate for injected fold expr                                           */
/*----------------------------------------------------------------------------*/
void Card_Having::vAcclimate_foldExpr(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_constraint = pSub_constraint();

  pCI->G.p_constraint->vAcclimate_foldExpr(pCI,pCI->Sqy.m_pExpr_fold);
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Having::vSmooth(CardIter* pCI, bool bSawProject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_having->pSymbol()->Y_vSet(P_Smoothed);

  bool bBump = ! bSawProject;

  vCorrect_After_Inject(pCI, bBump);
  pCI->vRenumberCards();
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Having::vSmooth(CardIter* pCI, CarryForward* pCarryForward)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_having->pSymbol()->Y_vSet(P_Smoothed);

  bool bBump = ! pCarryForward->Project_postAgg.m_bSeen;

  if (pCarryForward->Having.m_bCorrect_After_Inject)
    vCorrect_After_Inject(pCI, bBump);

  pCI->G.p_having->vUpdate_projection(pCI);

  pCI->Prior.m_pProjection = pSub_projection();


  pCI->vRenumberCards();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Having::vCorrect_After_Inject(CardIter* pCI, bool bBump)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_boolCalc = pSub_boolCalc();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn_cmp = [](Card_Term *p_term)
  {

    if (p_term->bIsStmOdn())
      p_term->vBump_stmOdn();

    return true;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (bBump)
    pCI->G.p_boolCalc->bApplyRecursive_term(bLambdaFn_cmp);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn_ro = [pCI,bBump](Card *pCard)
  {
    if (MARK_AT != pCard->cMark1())
      return C_CONTINUE_RECURSION;

    Card_Term* p_term_1 = _pCard_Term(pCard->pGet(1));
    pCard->vRemoveChild(p_term_1);
    Symbol x_Symbol_stmCol1(SF_COL_NO,1);
    pCI->G_vAppendNewChild(pCard, CK_TERM, Symbol(SF_COL_NO, "$1"_Stz));

    Card_Expr* p_expr_found =
        pCI->Prior.m_pProjection->pGetExpr(0);

    RASSERT(p_expr_found->szP_Type());

    pCI->G.p_term->pSymbol()->Y_vSet(P_Type, p_expr_found->szP_Type());

    DELETE_CARD(p_term_1);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_boolCalc->bApplyRecursive(CK_CMPPRED, bLambdaFn_ro);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Having::vUpdate_projection(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ((pCI->G.p_projection = pCI->G.p_having->pSub_projection())) {

    pCI->G.p_having->vDeleteChild(pCI->G.p_projection);
    //pCI->G.p_having->vAppend(pCI->Prior.m_pProjection->pClone());
    pCI->G.p_having->vAppend(pCI->Prior.m_pProjection->pPropagate(pCI));

  }

  pCI->Prior.m_pProjection = pSub_projection();
  pCI->vRenumberCards();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Having::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_LEVEL,CK_NONE);
  pSub_level()->vValidate(pCI,iDepth+1,this);

  if (CK_BOOLCALC == eValidate_mustEither(CK_BOOLCALC,CK_RPNCALC,CK_NONE))
    pSub_boolCalc()->vValidate(pCI,iDepth+1,this);
  else
    pSub_rpnCalc()->vValidate(pCI,iDepth+1,this);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
