/*============================================================================*/
/* XF_Flatten_53_W_L_NC_NI_join.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_53_W_L_NC_NI_join.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_53_W_L_NC_NI_join, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN)
__aNewFunction(XF_Flatten_53_W_L_NC_NI_join, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN)
__vDeleteFunction(XF_Flatten_53_W_L_NC_NI_join, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN)
__aFooterFunction(XF_Flatten_53_W_L_NC_NI_join)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_53_W_L_NC_NI_join::XF_Flatten_53_W_L_NC_NI_join(CardIter* pCI)
: XF_FLATTEN(CN_XF_FLATTEN_53_W_L_NC_NI_JOIN, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_53_W_L_NC_NI_JOIN);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_53_W_L_NC_NI_join::~XF_Flatten_53_W_L_NC_NI_join(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_53_W_L_NC_NI_join::vFlatten(CardIter* pCI)
{
  _static_

  XF_Flatten_53_W_L_NC_NI_join* pXF_Flatten_53_W_L_NC_NI_join = new XF_Flatten_53_W_L_NC_NI_join(pCI);

  pXF_Flatten_53_W_L_NC_NI_join->vFlatten();

  delete pXF_Flatten_53_W_L_NC_NI_join;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_53_W_L_NC_NI_join::vFlatten(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN, _FOOTER_);

 // m_pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  // W L NC NI
  // (refer: Q16)

  /* ARCHETYPE :
      select count(*)
        from partsupp, part
       where PS_PARTKEY = P_PARTKEY
         and PS_SUPPKEY not in (select S_SUPPKEY
                                  from supplier
                                 where s_comment like '%Customer%Complaints%')
      ;
 */

  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_53_W_L_NC_NI_JOIN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold of exists subQuery


  vFold_basic(m_pCI, "53_W_L_NC_NI_join"_Stz, CK_o_WHERE);

 // m_pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smooshing

  vInto_KEYnotEXISTS(m_pCI);
 // m_pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing

  m_pCI->vClearRestart();
  //RASSERT(m_pCI->bEachMainWithSub(CK_o_PROJECT,CK_t_EMIT));
  RASSERT(m_pCI->bEachMain(CK_c_FOLD));

  CarryForward* pCarryForward = new CarryForward;
  pCarryForward->Table.m_bReproject = true;

  vSmooth(m_pCI,pCarryForward);

 // m_pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  m_pCI->G.pDeck->vMarksClearRecursive();

  delete pCarryForward;

  m_pCI->vEndTransform(__LINE__);
//vValidateBench();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_53_W_L_NC_NI_join::vInto_KEYnotEXISTS(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN, _FOOTER_);

  pCI->vClearRestart();
  RASSERT(pCI->bEachMain_foldBreak(CK_o_FROM));


  //RASSERT(pCI->bEachMain(CK_o_WHERE));
  RASSERT(pCI->bEachMain_foldBreak(CK_o_PROJECT));

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

  // pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->vBeginEachReverse();
  RASSERT(pCI->bEachMain_reverse_foldBreak_b(CK_o_WHERE));

  RASSERT(pCI->bEachMain_reverse_foldBreak_b(CK_o_FROM));
  Card_Emit* p_emitted_table = pCI->G.p_from->pSub_emit();

  pCI->vBeginEachReverse();
  RASSERT(pCI->bEachMain_reverse_foldBreak_b(CK_o_WHERE));


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

  pCI->Dic.m_p_expr = pCI->Flux.m_pArriving = p_emitted_table;
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
  p_term_sqyArg->vApplyCast_p(p_term_found);
  //p_term_sqyArg->vDump();


  Symbol xSymbol_prmCol(SF_SQY_PRM, 1);
  p_from_exists->bUpdateSymbolRecursive(CK_TERM, &xSymbol_prmCol, p_term_sqyArg->pSymbol());

  delete p_term_sqyArg;

  // pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  pCI->vClearRestart();
  if (pCI->bEachMainWithSub(CK_o_WHERE, CK_BOOLPRED)) {
    (void)pCI->G.p_boolPred->nCullMarked_rlnConj(MARK_AT);
  }

  pCI->vClearRestart();
  pCI->vDeleteWhenEmptySub(CK_o_WHERE, CK_BOOLPRED);

  vDebugPoint();

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
