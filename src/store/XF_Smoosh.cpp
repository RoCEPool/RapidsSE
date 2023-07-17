/*============================================================================*/
/* XF_Smoosh.cpp -- XF_Smoosh                                                       */
/*============================================================================*/

#include "XF_Smoosh.hpp"

#include "CardIter.hpp"
#include "CarryForward.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Smoosh, CN_XF_SMOOSH)
__aNewFunction(XF_Smoosh, CN_XF_SMOOSH)
__vDeleteFunction(XF_Smoosh, CN_XF_SMOOSH)
__aFooterFunction(XF_Smoosh)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Smoosh::XF_Smoosh(void)
: Object(CN_XF_SMOOSH)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SMOOSH, _FOOTER_);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_SMOOSH, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Smoosh::~XF_Smoosh(void) noexcept(false)
{
_precondition_(_ADDRESS_, _MAGIC_, CN_XF_SMOOSH, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Smoosh::vSmoosh(CardIter* pCI, eSM Smoosh)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SMOOSH, _FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Smoosh::vInto_KEYnotEXISTS(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SMOOSH, _FOOTER_);

  pCI->G_vRestart();
  RASSERT(pCI->G_bEachMain_foldBreak(CK_TABLE));


  //RASSERT(pCI->G_bEachMain(CK_WHERE));
  RASSERT(pCI->G_bEachMain_foldBreak(CK_PROJECT));

//  pCI->G.p_constraint = pCI->G.p_where->pSub_constraint();
//  pCI->G.p_where->vRemoveSub(pCI->G.p_constraint);
//
//  //pCI->G.p_filter = Card_Filter::pNew(p_constraint);
//  pCI->G.p_table->vAppend(pCI->G.p_constraint);

  Card_Table* p_table_exists = pCI->G.p_table;
  pCI->G.pDeck->vRemove(p_table_exists);

  pCI->G.p_term = pCI->G.p_project->pSub_term();
  pCI->G.p_project->vRemoveSub(pCI->G.p_term);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  pCI->G.pDeck->vDeleteChild(pCI->G.p_where);
  pCI->G.pDeck->vDeleteChild(pCI->G.p_project);

  pCI->vPrintBoard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->vBeginEach();
  RASSERT(pCI->G_bEachMain_reverse_foldBreak(CK_WHERE));

  RASSERT(pCI->G_bEachMain_reverse_foldBreak(CK_TABLE));
  Card_Projection* p_projection_table = pCI->G.p_table->pSub_projection();

  pCI->vBeginEach();
  RASSERT(pCI->G_bEachMain_reverse_foldBreak(CK_WHERE));


  pCI->G.pDeck->vInsertBefore(p_table_exists,pCI->G.p_where);

  pCI->G.p_boolCalc = pCI->G.p_where->pSub_boolCalc();
  pCI->G.p_cmpPred = pCI->G.p_boolCalc->pCmpPred_whichCallsSubQuery(pCI);

  if (pCI->G.p_cmpPred->bIsSymbol("in"_Stz) || pCI->G.p_cmpPred->bIsSymbol("exists"_Stz))
    p_table_exists->vAppend(new Card_KeyExists);
  else
  if (pCI->G.p_cmpPred->bIsSymbol("notIn"_Stz) || pCI->G.p_cmpPred->bIsSymbol("notExists"_Stz))
    p_table_exists->vAppend(new Card_KeyNotExists);

  pCI->Analysis.m_pProjection_prior = p_projection_table;
  pCI->G.p_cmpPred->vMorph_SqlOp_To_Eq(pCI, pCI->G.p_term);

  DELETE_CARD(pCI->G.p_term);

  pCI->vRenumberCards();

  pCI->vPrintBoard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (! pCI->Sqy.m_pCorrelation) {
    pCI->vRenumberCards();
    return;
  }

  // todo: the %1 needs to be decoded... its a parameter
  Card* p_parent;
  Card_Term* p_term_sqyarg = _pCard_Term(p_table_exists->pFindSymbolRecursive(CK_TERM,"%1"_Stz,p_parent));

  Card_Expr* p_expr_found =
      p_projection_table->pGetExpr(p_term_sqyarg->iGetOdn() - 1);

  //p_expr_found->vDump();
  Card_Term* p_term_found = p_expr_found->pGetTerm(0);

  Card_Term* p_term_sqarg = new Card_Term(pCI->Sqy.m_sSqyArg);
  p_term_sqarg->pSymbol()->Y_vSet(P_Type, p_term_found->pSymbol()->Y_szGet(P_Type));
  //p_term_sqarg->vDump();


  //Card_Term::bReplaceValueOfSub(pCI, p_table_exists,CK_TERM,"%1"_Stz,pCI->Sqy.m_sSqyArg);
  p_table_exists->bUpdateSymbolRecursive(CK_TERM, "%1"_Stz, p_term_sqarg);

  delete p_term_sqarg;

  pCI->vPrintBoard();

  pCI->G_vRestart();
  if (pCI->G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    pCI->G.p_boolCalc->vCullPred(MARK_AT);
  }

  pCI->vPrintBoard();

  pCI->G_vRestart();
  pCI->G_vDeleteWhenEmptySub(CK_WHERE,CK_BOOLCALC);

  pCI->vRenumberCards();
  pCI->vPrintBoard(__LINE__);
  vDebugPoint();

}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
