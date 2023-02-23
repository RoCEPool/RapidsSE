/*============================================================================*/
/* Smoosh.cpp -- Smoosh                                                       */
/*============================================================================*/

#include "Smoosh.hpp"

#include "CardIter.hpp"
#include "CarryForward.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Smoosh, CN_SMOOSH)
__aNewFunction(Smoosh, CN_SMOOSH)
__vDeleteFunction(Smoosh, CN_SMOOSH)
__aFooterFunction(Smoosh)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Smoosh::Smoosh(void)
: Object(CN_SMOOSH)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SMOOSH, _FOOTER_);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_SMOOSH, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Smoosh::~Smoosh(void) noexcept(false)
{
_precondition_(_ADDRESS_, _MAGIC_, CN_SMOOSH, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Smoosh::vSmoosh(CardIter* pCI, eSM Smoosh)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SMOOSH, _FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Smoosh::vInto_GBYAGG(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SMOOSH, _FOOTER_);
  pCI->G_vRestart();
  RASSERT(pCI->G_bEachMainWith2Sub(CK_TABLE,CK_PROJECTION,CK_FILTER));
  Board::vIdentify_Correlation_Column_In_cmpPred_Of_sq_boolCalc(pCI);
  RASSERT(pCI->m_pTerm_corrCol); // eg $d.2 (L_PARTKEY)
  Board::vFind_Correlation_Expr_Of_Correlation_Column(pCI);
  RASSERT(pCI->m_pExpr_corr);    // eg L_PARTKEY

  pCI->vPrintBoard(__LINE__);

  // T A A A P F T T T W A A A     //
  //   v v v                       //
  // T G G G P F T T T W A A A     // push corrPred <expr> into <gbyElist> and <aggElist>

  RASSERT(pCI->G_bEachMain(CK_AGGREG));
  Board::vEnrich_sq_aggreg_ToBe_gbyAgg_Gby_Correlation_Column(pCI);

  pCI->G.pDeck->vRenumberRecursive();
  pCI->vPrintBoard(__LINE__);

  // T A A A P F T T T W A A A     //
  //         v                     //
  // T G G G P F T T T W A A A     // push corrPred <expr> into <projElist>

  if (pCI->G_bIsNextMain(CK_PROJECT)) {
    RASSERT(pCI->G_bEachMain(CK_PROJECT));
    Board::vEnhance_sq_project_To_Include_Correlation_Column(pCI,"__SQCRLKEY"_Stz);
  }
  else {

  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Smoosh::vInto_KEYnotEXISTS(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SMOOSH, _FOOTER_);

  pCI->G_vRestart();
  RASSERT(pCI->G_bEachMain(CK_TABLE));
  RASSERT(pCI->G_bEachMain(CK_WHERE));
  RASSERT(pCI->G_bEachMain(CK_PROJECT));

  pCI->G.p_constraint = pCI->G.p_where->pSub_constraint();
  pCI->G.p_where->vRemoveSubCard(pCI->G.p_constraint);

  //pCI->G.p_filter = Card_Filter::pNew(p_constraint);
  pCI->G.p_table->vAppend(pCI->G.p_constraint);

  pCI->G.pDeck->vRemove(pCI->G.p_table);

  pCI->G.p_term = pCI->G.p_project->pSub_term();
  pCI->G.p_project->vRemoveSubCard(pCI->G.p_term);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.pDeck->vRemoveDeleteChild(pCI->G.p_where);
  pCI->G.pDeck->vRemoveDeleteChild(pCI->G.p_project);

  pCI->pBoard()->vPrintBoard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->vRestart();
  RASSERT(pCI->G_bEachMain(CK_WHERE));
  pCI->G.pDeck->vInsertBefore(pCI->G.p_table,pCI->G.p_where);

  pCI->G.p_bolPred = pCI->G.p_where->pSub_bolPred();

  if (pCI->G.p_bolPred->bIsValue("in"_Stz) || pCI->G.p_bolPred->bIsValue("exists"_Stz))
    pCI->G.p_table->vAppend(new Card_KeyExists);
  else
  if (pCI->G.p_bolPred->bIsValue("notIn"_Stz) || pCI->G.p_bolPred->bIsValue("notExists"_Stz))
    pCI->G.p_table->vAppend(new Card_KeyNotExists);

  pCI->G.p_bolPred->vMorph_To_cmpPred(pCI,pCI->G.p_term);

  DELETE_CARD(pCI->G.p_term);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (! pCI->m_pCorrelation) {
    pCI->G.pDeck->vRenumberRecursive();
    return;
  }

  Card_Term::bReplaceValueOfSub(pCI, pCI->G.p_table,CK_TERM,"%1"_Stz,pCI->m_sSqyArg);
  pCI->G_vCopyCard(nullptr);
  
  pCI->G_vRestart();
  if (pCI->G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    pCI->G.p_boolCalc->vCullPred(MARK_AT);
  }
  
  pCI->G_vRestart();
  pCI->G_vDeleteWhenEmptySub(CK_WHERE,CK_BOOLCALC);

  pCI->G.pDeck->vRenumberRecursive();
//  pCI->vPrintBoard(__LINE__);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
