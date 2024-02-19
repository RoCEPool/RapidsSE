/*============================================================================*/
/* XF_Flatten_Smoosh.cpp -- XF_Flatten_Smoosh                                                       */
/*============================================================================*/

#include "XF_Flatten_Smoosh.hpp"

#include "CardIter.hpp"
#include "CarryForward.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_Smoosh, CN_XF_FLATTEN_SMOOSH)
__aNewFunction(XF_Flatten_Smoosh, CN_XF_FLATTEN_SMOOSH)
__vDeleteFunction(XF_Flatten_Smoosh, CN_XF_FLATTEN_SMOOSH)
__aFooterFunction(XF_Flatten_Smoosh)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_Smoosh::XF_Flatten_Smoosh(void)
: Object(CN_XF_FLATTEN_SMOOSH)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SMOOSH, _FOOTER_);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SMOOSH, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_Smoosh::~XF_Flatten_Smoosh(void) noexcept(false)
{
_precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SMOOSH, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Smoosh::vSmoosh(CardIter* pCI, eSM Smoosh)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SMOOSH, _FOOTER_);
  UNUSED(pCI);
  UNUSED(Smoosh);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Smoosh::vInto_KEYnotEXISTS(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_SMOOSH, _FOOTER_);

  pCI->vClearRestart();
  RASSERT(pCI->G_bEachMain_foldBreak(CK_o_FROM));


  //RASSERT(pCI->G_bEachMain(CK_o_WHERE));
  RASSERT(pCI->G_bEachMain_foldBreak(CK_o_PROJECT));

//  pCI->G.p_constraint = pCI->G.p_where->pSub_constraint();
//  pCI->G.p_where->vRemoveSub(pCI->G.p_constraint);
//
//  //pCI->G.p_filter = Card_Filter::pNew(p_constraint);
//  pCI->G.p_table->vAppendChild(pCI->G.p_constraint);

  Card_From* p_from_exists = pCI->G.p_from;
  pCI->G.pDeck->vRemove(p_from_exists);

  pCI->G.p_term = pCI->G.p_project->pSub_term();
  pCI->G.p_project->vRemoveSub(pCI->G.p_term);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  pCI->G.pDeck->vDeleteChild(pCI->G.p_where);
  pCI->G.pDeck->vDeleteChild(pCI->G.p_project);

 // pCI->vPrintBoard(__func__,__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->vBeginEachReverse();
  RASSERT(pCI->G_bEachMain_reverse_foldBreak_b(CK_o_WHERE));

  RASSERT(pCI->G_bEachMain_reverse_foldBreak_b(CK_o_FROM));
  Card_Emit* p_emitted_table = pCI->G.p_from->pSub_emit();

  pCI->vBeginEachReverse();
  RASSERT(pCI->G_bEachMain_reverse_foldBreak_b(CK_o_WHERE));


  pCI->G.pDeck->vInsertChildBefore(p_from_exists,pCI->G.p_where);

  pCI->G.p_boolPred = pCI->G.p_where->pSub_boolPred();
  pCI->G.p_rlnConj = pCI->G.p_boolPred->pFind_rlnConj_With_sqyCall();

  if (pCI->G.p_rlnConj->pSymbol()->bHasFacet("in"_Stz) ||
      pCI->G.p_rlnConj->pSymbol()->bHasFacet("exists"_Stz))
    p_from_exists->vAppendChild(new Card_KeyExists);
  else
  if (pCI->G.p_rlnConj->pSymbol()->bHasFacet("notIn"_Stz) ||
      pCI->G.p_rlnConj->pSymbol()->bHasFacet("notExists"_Stz))
    p_from_exists->vAppendChild(new Card_KeyNotExists);

  pCI->Flux.m_pArriving = p_emitted_table;
  pCI->G.p_rlnConj->vMorph_sqlOp_To_EQ(pCI, pCI->G.p_term);

  DELETE_CARD(pCI->G.p_term);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (! pCI->Sqy.m_pCorrelation) {
    return;
  }

  // todo: the %1 needs to be decoded... its a parameter
  Card* p_parent;
  Symbol xPrmOrd(SF_SQY_PRM, 1);
  Card_Term* p_term_sqyarg = _pCard_Term(p_from_exists->pFindSymbolRecursive(CK_TERM,&xPrmOrd,p_parent));

  Card_Expr* p_expr_found =
      p_emitted_table->pGet_expr(p_term_sqyarg->pSymbol()->iTo_ordinal(SF_SQY_PRM) - 1);

  //p_expr_found->vDump();
  Card_Term* p_term_found = p_expr_found->pGet_term(0);


  Card_Term* p_term_sqyArg = pCI->Sqy.m_pTerm_sqyArg->pClone();
  p_term_sqyArg->vCast(p_term_found);
  //p_term_sqyArg->vDump();


  Symbol xSymbol_prmCol(SF_SQY_PRM, 1);
  p_from_exists->bUpdateSymbolRecursive(CK_TERM, &xSymbol_prmCol, p_term_sqyArg->pSymbol());

  delete p_term_sqyArg;

 // pCI->vPrintBoard(__func__,__LINE__);

  pCI->vClearRestart();
  if (pCI->G_bEachMainWithSub(CK_o_WHERE, CK_BOOLPRED)) {
    (void)pCI->G.p_boolPred->nCullMarked_rlnConj(MARK_AT);
  }

  pCI->vClearRestart();
  pCI->G_vDeleteWhenEmptySub(CK_o_WHERE, CK_BOOLPRED);

  vDebugPoint();

}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
