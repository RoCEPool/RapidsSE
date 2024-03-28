/*============================================================================*/
/* XF_Flatten_24_W_A_C_ro.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_24_W_A_C_ro.hpp"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_24_W_A_C_ro, CN_XF_FLATTEN_24_W_A_C_RO)
__aNewFunction(XF_Flatten_24_W_A_C_ro, CN_XF_FLATTEN_24_W_A_C_RO)
__vDeleteFunction(XF_Flatten_24_W_A_C_ro, CN_XF_FLATTEN_24_W_A_C_RO)
__aFooterFunction(XF_Flatten_24_W_A_C_ro)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_24_W_A_C_ro::XF_Flatten_24_W_A_C_ro(CardIter* pCI)
: XF_FLATTEN(CN_XF_FLATTEN_24_W_A_C_RO, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);

  m_eFlatten = QF_24_W_A_C_ro;

  _checkInitialised_(CN_XF_FLATTEN_24_W_A_C_RO);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_24_W_A_C_ro::~XF_Flatten_24_W_A_C_ro(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_24_W_A_C_ro::vFlatten(CardIter* pCI)
{
  _static_

  XF_Flatten_24_W_A_C_ro* pXF_Flatten_24_W_A_C_ro = new XF_Flatten_24_W_A_C_ro(pCI);

  pXF_Flatten_24_W_A_C_ro->vFlatten();

  delete pXF_Flatten_24_W_A_C_ro;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* flatten                                                                    */
/*----------------------------------------------------------------------------*/
void XF_Flatten_24_W_A_C_ro::vFlatten(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);

 // m_pCI->PRINTBOARD_dbg();

  // W A C ro

  /* ARCHETYPE :
    select count(*)
      from Lineitem, Part, PartSupp
     where P_PARTKEY  = L_PARTKEY
       and PS_PARTKEY = P_PARTKEY
       and l_quantity < (select avg(l_quantity)
                           from lineitem
                          where L_PARTKEY = P_PARTKEY)
    ;
 */

  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_24_W_A_C_RO);

  vFold_basic(m_pCI, "24_W_A_C_ro"_Stz, CK_o_WHERE);

  vPromote_aggreg_To_gbyAgg(m_pCI);

  vRestate_emit_fold(m_pCI);

  m_pCI->E.p_deck->vMarksClearRecursive();

  m_pCI->vEndTransform(__LINE__);
}

/*----------------------------------------------------------------------------*/
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Flatten_24_W_A_C_ro::bSnap_IQ_crlKey(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pre-fold identify the <table>/<filter>/<boolPred>/<cmpPred> which is correlated by parameter
  bool rbSnapped = false;

  auto fbSnap_sqyArg = [pCI,&rbSnapped](Card_CONJ* p_conj, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;
    if (! p_conj->bIsCorrConj())
      return C_CONTINUE_RECURSION;

    pCI->Flat.m_p_conj_crlKey = p_conj;
    pCI->Flat.m_p_conj_crlKey->vMark1(MARK_AT);
    pCI->Flat.m_p_conj_crlKey->pSymbol()->Y_vSet(P_CrlKeyConj);

    pCI->Flat.m_p_term_crlKey = static_cast<Card_RlnConj *>(p_conj)->pGet_term_crlKey();
    pCI->Flat.m_p_term_crlKey->pSymbol()->Y_vSet(P_CrlKeyTerm);

    rbSnapped = true;

    return C_TERMINATE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->vClearRegisters();
  pCI->vBeginEach();

  while (pCI->bEachOp(CK_o_FROM, CK_t_EMIT, CK_o_FILTER)) {

    pCI->E.p_boolPred = pCI->E.p_filter->pSub_boolPred();
    Card_CONJ *p_conj_root = pCI->E.p_boolPred->pRoot_conj();

    bool bIgnore = p_conj_root->bLambdaWalk_conj(fbSnap_sqyArg);
    UNUSED(bIgnore);
    if (rbSnapped)
      break;
  }
  
  if (! rbSnapped)
    return false;

  pCI->Flat.m_p_expr_crlKey = pCI->pBackSearch_expr_For_col(pCI->Flat.m_p_term_crlKey->pSymbol(),
                                                            pCI->E.i_Op + 1);

  pCI->Flat.m_p_expr_crlKey->vMark1(MARK_AT);
  pCI->Flat.m_p_expr_crlKey->pSymbol()->Y_vSet(P_CrlKeyDefn);

  return true;
}
/*----------------------------------------------------------------------------*/
/* promote aggreg of inner query to be a gbyAgg keyed by crlKey               */
/*----------------------------------------------------------------------------*/
void XF_Flatten_24_W_A_C_ro::vPromote_IQ_aggreg_To_gbyAgg_crlKey(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);

  // promote IQ <aggreg>... to <gbyAgg>...

  pCI->vClearRegisters();
  pCI->vBeginEach();
  RASSERT(pCI->bEachOp_break(CK_o_AGGREG));

  for (i8 i_agg = 0; i_agg < 3; i_agg++) {

    pCI->E.p_Op->vCard(CK_o_GBYAGG);
    pCI->E.p_gbyAgg = _pCard_GbyAgg(pCI->E.p_Op);

    pCI->E.p_level = pCI->E.p_gbyAgg->pSub_level();
    eAL e_Level = (eAL) pCI->E.p_level->pSymbol()->Y_iGet(P_Level);

    pCI->E.p_stream = pCI->E.p_gbyAgg->pSub_stream();
    pCI->E.p_emit = pCI->E.p_stream->pSub_emit();

    Card_Key::vAssemble_injCrlKey(pCI);
    pCI->E.p_gbyAgg->vInsertChildBefore(pCI->E.p_key, pCI->E.p_stream);


    // key

    if (AL_PARTITION == e_Level) {
/**Z_KEY_DIRECTION promote*/

    }
    else {
/**Z_KEY_DIRECTION promote*/

      pCI->E.p_rpnCalc->vApplyCast_x(pCI->Flat.m_p_expr_crlKey->xCast());
      pCI->E.p_term->vApplyCast_x(pCI->Flat.m_p_expr_crlKey->xCast());

    }


    pCI->E.p_expr->pSymbol()->Y_vSet(P_InjCrlKey);

    // projection

    if (AL_PARTITION == e_Level) {
      pCI->E.p_expr = Card_Expr::pCraft_expr_project_From_expr_gbyAgg(pCI, pCI->E.p_expr);
      //pCI->G.p_expr->vDump(__func__);
      vDebugPoint();
    }
    else {
      pCI->E.p_key->pSymbol()->Y_vSet(P_ProjBump, (i8)1);


      Symbol xSymbol_KeyCol(SF_KEY_COL,1);

      pCI->vNewMember(CK_EXPR);
      i8 i_Board = pCI->Flat.m_pSqy_inner->pBoard()->i_Board();
      pCI->E.p_expr->pSymbol()->vApplyFacet_varg(SF_NMD_COL, C_CRLKEY, i_Board);
      pCI->vAppendNewChild(pCI->E.p_expr, CK_RPNCALC);
      pCI->vAppendNewChild(pCI->E.p_rpnCalc, CK_TERM, &xSymbol_KeyCol);

      pCI->E.p_rpnCalc->vApplyCast_x(pCI->E.p_key->xCast());
      pCI->E.p_term->vApplyCast_x(pCI->E.p_key->xCast());
      pCI->E.p_expr->vApplyCast_x(pCI->E.p_key->xCast());
    }

    pCI->E.p_expr->pSymbol()->Y_vSet(P_InjCrlKey);
    pCI->E.p_emit->vPushChild(pCI->E.p_expr);


    if (AL_PARTITION != e_Level) {
      pCI->E.p_emit->vBump_expCol_In_rpnCalc(pCI);
      //pCI->G.p_key->Y_vCut(P_ProjBump);
      pCI->E.p_key->pSymbol()->Y_vSet(P_ProjBumped);
    }

//Z_KEY_DIRECTION
    for (pCI->vBeginTarget(pCI->E.p_key,CK_EXPR); pCI->bEachTarget(); /**/) {
      pCI->E.p_expr->pSymbol()->vApplyFacet(SF_KEY_COL,pCI->i_Target()+1);
    }

    pCI->E.p_emit->vRenumberAll_expr();

    //pCI->G.p_key->vDump(__func__);
    vDebugPoint();

    if (pCI->bEachOp()) {
      if (! pCI->E.p_Op->bIsCard(CK_o_AGGREG)) {
        bool b = pCI->bRewindOp();
        RASSERT(b);
        break;
      }
    }

  }

}

/*----------------------------------------------------------------------------*/
/* enhance possible last IQ project adding crlKey to emit                     */
/*----------------------------------------------------------------------------*/
void XF_Flatten_24_W_A_C_ro::vEnhance_IQ_emit_project_Add_crlKey(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);

  if (!pCI->pPeekNextIf(CK_o_PROJECT))
    return;
  RASSERT(pCI->bEachOp(CK_o_PROJECT));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pre-fold sq <project>

  RASSERT (pCI->E.p_Op->bIsCard(CK_o_PROJECT));
  RASSERT(CT_NONE != pCI->E.p_key->eCTVal());

  pCI->E.p_emit = pCI->E.p_project->pSub_emit();
  pCI->E.p_emit->vBump_expCol_In_rpnCalc(pCI);// TODO: ,nBumpBy);


  // push <project>/_SQCRLKEY_
  pCI->vPushChild(pCI->E.p_emit, CK_EXPR);
  i8 i_Board = pCI->Flat.m_pSqy_inner->pBoard()->i_Board();
  pCI->E.p_expr->pSymbol()->vApplyFacet_varg(SF_NMD_COL, C_CRLKEY, i_Board);
  pCI->E.p_expr->pSymbol()->Y_vSet(P_InjCrlKey);

  pCI->vAppendNewChild(pCI->E.p_expr, CK_RPNCALC);

  pCI->vAppendNewChild(pCI->E.p_rpnCalc, CK_TERM, Symbol(SF_EXP_COL, "$1"_Stz));
  Symbol x_Symbol_expCol1(SF_EXP_COL, 1);


  pCI->E.p_rpnCalc->vApplyCast_x(pCI->E.p_key->xCast());
  pCI->E.p_term->vApplyCast_x(pCI->E.p_key->xCast());
  pCI->E.p_expr->vApplyCast_x(pCI->E.p_key->xCast());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // supplemental

  pCI->E.p_expr = pCI->E.p_emit->pGet_expr(1);
  pCI->E.p_expr->pSymbol()->vApplyFacet_varg(SF_NMD_COL, C_SQYRSL, i_Board);



  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->E.p_emit->vRenumberAll_expr();
}
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_24_W_A_C_ro::vConvert_OQ_sqyPrm_Into_expCol(CardIter* pCI,
                                                            Card_BoolPred *p_boolPred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  auto vResolve = [pCI](Card_CONJ* p_conj_arg,
                        auto&& vResolve /*oddity permits recursive lambda*/)
  {
    if (! Card_CONJ::bIsConj(p_conj_arg))
      return; /* this effectively declares resolve as void */

    if (p_conj_arg->bIsCard(CK_RLNCONJ)) {

      if (!p_conj_arg->pSymbol()->Y_bHas(P_CrlKeyTest))
        return;

      RASSERT(p_conj_arg->pSymbol()->bHasFacet("=="_Stz));

      Kid xConj(p_conj_arg);
      while (xConj.bEach()) {
        // TODO: test for: == $d.2 %1 => == $1 [TRANSLATE PARAM %1]

        if (xConj.pChild()->pSymbol()->bHasFacet(SF_SQY_PRM)) {

          Symbol xSym_expCol1(SF_EXP_COL, pCI->Flat.m_pSqy_inner->pBoard()->i_Board());
          xConj.pChild()->pSymbol()->vApplyFacets(&xSym_expCol1);

          Card_Expr* p_expr_found =
              pCI->Flux.m_pArriving->pGet_expr((i8) 0);

          xConj.pChild()->vApplyCast_x(p_expr_found->xCast());

          xConj.pChild()->vMark1(MARK_CHANGED);
        }
        else
        if (xConj.pChild()->bIsCard(CK_TERM) && xConj.pChild()->pSymbol()->bHasFacet(SF_TBL_COL)) {
          Card_OuterColNm* p_outerColNm = pCI->Flat.m_pCorrelation->pSub_outerColNm();

          // todo: hack
          Card* p_card = p_outerColNm->pDupCardRecursive();
          p_card->vCard(CK_TERM);
          Card_Term* p_term = _pCard_Term(p_card);
          Symbol* pSym_term = p_term->pSymbol();

          Card_Expr* p_expr_otr = pCI->pBackSearch_expr_For_colNm(pSym_term, pCI->i_Child());
          Stz* szProjectedCol = p_expr_otr->pGet_term(0)->szFacet(SF_TBL_COL);
          xConj.pChild()->pSymbol()->vApplyFacet(SF_TBL_COL, szProjectedCol);  // TODO: hack!!
          xConj.pChild()->vMark1(MARK_CHANGED);

          DELETE_CARD(p_term);
        }
        else {
          NEVER_REACH
        }
      }
      return;
    }
    else
    if (p_conj_arg->bIsCard(CK_ANDCONJ) || p_conj_arg->bIsCard(CK_ORCONJ)) {
      Kid xConj(p_conj_arg);
      while (xConj.bEach()) {
        RASSERT(Card_CONJ::bIsConj(xConj.pChild()));
        vResolve((Card_CONJ*)xConj.pChild(), vResolve);
      }
    }
    else {
      NEVER_REACH;
    }

  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_CONJ* p_conj_root = static_cast<Card_CONJ*>(p_boolPred->pRoot_conj());

  vResolve(p_conj_root, vResolve);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_24_W_A_C_ro::vConvert_OQ_sqyNo_Into_expCol(CardIter* pCI,
                                                           Card_BoolPred *p_boolPred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto vResolve = [pCI](Card_CONJ* p_conj_arg,
                        auto&& vResolve /*oddity permits recursive lambda*/)
  {
    if (! Card_CONJ::bIsConj(p_conj_arg))
      return; /* this effectively declares resolve as void */

    //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // process AND or OR

    if (p_conj_arg->bIsCard(CK_ANDCONJ) || p_conj_arg->bIsCard(CK_ORCONJ)) {
      for (Kid xConj(p_conj_arg); xConj.bEach(); /**/) {
        RASSERT(Card_CONJ::bIsConj(xConj.pChild()));
        vResolve(static_cast<Card_CONJ*>(xConj.pChild()), vResolve);
      }
      return;
    }

    //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // process <RLNCONJ>

    RASSERT(p_conj_arg->bIsCard(CK_RLNCONJ));

    if (! p_conj_arg->bHasSubFacet(SF_SQY_NO))
      return;

    RASSERT(! p_conj_arg->pSymbol()->Y_bHas(P_CrlKeyConj));
    RASSERT(p_conj_arg->bIsRelnOp());

    for (Kid xConj(p_conj_arg); xConj.bEach(); /**/) {
      Card* p_conj_child = xConj.pChild();
      if (Card_CONJ::bIsConj(p_conj_child)) {
        vResolve(static_cast<Card_CONJ*>(p_conj_child), vResolve);
        continue;
      }

      if (! p_conj_child->pSymbol()->bHasFacet(SF_SQY_NO))
        continue;

      RASSERT(pCI->m_pBench->bKnown_sqy(p_conj_child->pSymbol()));

      Card* p_prev_conj = xConj.pPrev();
      p_conj_arg->vRemoveChild(p_prev_conj);
      DELETE_CARD(p_prev_conj);

      p_conj_child->pSymbol()->vApplyFacet(SF_EXP_COL, "$2"_Stz);
      Card_Expr* p_expr_found = pCI->Flux.m_pArriving->pGet_expr(1);
      p_conj_child->vApplyCast_x(p_expr_found->xCast());
      p_conj_child->vMark1(MARK_CHANGED);

      p_conj_arg->pSymbol()->Y_vSet(P_SqyRslTest);
    }

    return;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_CONJ* p_conj_root = static_cast<Card_CONJ*>(p_boolPred->pRoot_conj());

  vResolve(p_conj_root, vResolve);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_24_W_A_C_ro::vMove_OQ_rlnConj_rslTest_To_from_filter(CardIter* pCI, Card_BoolPred *p_boolPred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);

  p_boolPred->vRemove_rlnConj(pCI->Flat.m_p_conj_rslTest);

  //p_rlnConj_sqyRslTest->vDump();

  Card_Term* p_term_tblCol = pCI->Flat.m_p_conj_rslTest->pGetFirstTerm_tblCol();
  Symbol* pSym_term_tblCol = p_term_tblCol->pSymbol();
  Card_OP* p_OP = pCI->pFind_from_By_tblSyn(pSym_term_tblCol, pCI->E.i_Op);

  pCI->E.p_filter = p_OP->pEnsure_filter();

  pCI->E.p_filter->pSub_boolPred()->vAppend_and_rlnConj(pCI->Flat.m_p_conj_rslTest);
}

/*----------------------------------------------------------------------------*/
/* Derive correlated fold projection                                          */
/*----------------------------------------------------------------------------*/
void XF_Flatten_24_W_A_C_ro::vRestate_emit_fold(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);

  pCI->vBeginEach();

  while (pCI->bEachOp()) {

    if (CK_c_FOLD == pCI->E.e_Op)
      break;

    if (Card_Emit* p_emit = pCI->E.p_Op->pSub_emit()) {
      //pCI->PRINTBOARD_dbg();
      pCI->Dic.m_p_expr = pCI->Flux.m_pArriving = p_emit;
    }
  }

  RASSERT(CK_c_FOLD == pCI->E.e_Op);

  pCI->E.p_stream = new Card_Stream;
  pCI->E.p_fold->vAppendChild(pCI->E.p_stream);
  pCI->E.p_stream->vAppendChild(pCI->Flux.m_pArriving->pDupCardRecursive());
}

/*----------------------------------------------------------------------------*/
/* convert call of sub-query to correlated join to left sub-stream            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_24_W_A_C_ro::vPromote_aggreg_To_gbyAgg(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);

  // We have a flattened query.  It has a left-size IQ, and a right-side OQ

  //        IQ... ...OQ           //
  // F F A A A P + F F W A A A    //
  //                              //

  // Start with processing left-side IQ (inner sub-query)

  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // innerQuery innerQuery innerQuery innerQuery innerQuery innerQuery innerQuery
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // snap correlation key in IQ:<from>/<filter>/<boolPred>//<rlnConj>
  //

  //   v                          //
  // F F A A A P + F F W A A A    // ]FROM
  //   f         +                //  .]filter
  //   b         +                //  ..]boolPred
  //   r         +                //  ...]rlnConj == ((pCI->Flat.m_p_conj_crlKey))
  //             +                //            $d.2 ((pCI->Flat.m_p_term_crlKey))
  //             +                //              %1 // correlation parameter

  bool bSnapped = bSnap_IQ_crlKey(pCI);
  RASSERT(bSnapped);
  pCI->PRINTBOARD_dbg();
//  pCI->Flat.m_p_expr_crlKey->vDump(__func__);
//  pCI->Flat.m_p_conj_crlKey->vDump(__func__);
//  pCI->Flat.m_p_term_crlKey->vDump(__func__);
  RASSERT(pCI->Flat.m_p_expr_crlKey->pSymbol()->bMatches(pCI->Flat.m_p_term_crlKey->pSymbol()));


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // promote IQ:<aggreg> to be IQ:<gbyAgg>/<key=crlKey>
  //   thereby producing ordered subQuery stream

  vPromote_IQ_aggreg_To_gbyAgg_crlKey(pCI);
  pCI->PRINTBOARD_dbg();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add correlation key to IQ:[<project>/<emit>/<crlKey>]

  vEnhance_IQ_emit_project_Add_crlKey(pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // snap final IQ:<op>/<emit>

  if (CK_o_PROJECT == pCI->E.e_Op || CK_o_GBYAGG == pCI->E.e_Op)
    pCI->E.p_emit = pCI->E.p_Op->pSub_emit();
  else
    NEVER_REACH

  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  // fold fold fold fold fold fold fold fold fold fold fold fold fold fold fold
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

  RASSERT(pCI->bEachOp(CK_c_FOLD));

  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // outerQuery outerQuery outerQuery outerQuery outerQuery outerQuery outerQuery
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

  // restate the emits of the leading OQ:<from>* sequence
  pCI->Flux.m_pArriving = nullptr;
  while (pCI->pPeekNextIf(CK_o_FROM) && pCI->bEachOp(CK_o_FROM)) {
    ((Card_OP*)pCI->E.p_Op)->vRestate_emit(pCI);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Convert the OQ to join to the IQ gby correlation column ordered streamed from SQ

  // v                             //  ]filter
  // F G G G P + F F F W A A A     //  .]boolPred
  // s         +                   //  ..]cmpPred ==
  // f         +                   //           $d.2  ((pCI->m_pTerm_corrArg))
  //           +                   //             %1


  pCI->vClearRegisters();
  pCI->vBeginEach();
  while (pCI->bEachOp(CK_o_FROM, CK_o_FILTER, CK_BOOLPRED)) {
    if (pCI->E.p_boolPred->bHasSub(pCI->Flat.m_p_conj_crlKey)) {
      pCI->E.p_boolPred->vRemoveSub(pCI->Flat.m_p_conj_crlKey);
      break;
    }
  }
  RASSERT(pCI->Flat.m_p_conj_crlKey->pSymbol()->Y_bHas(P_CrlKeyConj));
  // tidy up empty SQ:<table>/<filter>
  pCI->E.p_from->vCull_filter_IfEmpty(pCI->E.p_filter);

  //                   v           //  ]filter
  // F A A A P + F F F W A A A     //  .]boolPred
  // s         +       f           //  ..]cmpPred ==
  // f         +       bc          //           $d.2  ((pCI->m_pTerm_corrArg))
  //           +                   //             %1

  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  // fold fold fold fold fold fold fold fold fold fold fold fold fold fold fold
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

  RASSERT(pCI->bEachOp(CK_c_FOLD));

  // todo: this is messy and unclear

  pCI->vForwards(false);
  while (pCI->bEachOp_reverse()) {
    if (pCI->E.p_Op->pSub_emit())
      break;
  }
  pCI->vForwards(true);

  pCI->Dic.m_p_expr = pCI->Flux.m_pArriving = pCI->E.p_Op->pSub_emit();
  RASSERT(pCI->bEachOp(CK_c_FOLD));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ???

  RASSERT(pCI->bEachOp(CK_o_WHERE, CK_BOOLPRED));
  pCI->E.p_boolPred->vBump_expCol(2);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append correlation key conjunction onto the OQ:<where>/<filter>/<boolPred>

  pCI->Flat.m_p_conj_crlKey->pSymbol()->Y_vCut(P_CrlKeyConj);
  pCI->Flat.m_p_conj_crlKey->pSymbol()->Y_vSet(P_CrlKeyTest);

  pCI->E.p_boolPred->vAppend_and_rlnConj(_pCard_RlnConj(pCI->Flat.m_p_conj_crlKey));

  //                   v           //  ]filter
  // F A A A P + F F F W A A A     //  .]boolPred
  // s         +       f           //  ..]roPred <
  // f         +       bc          //           $a.5
  //           +                   //           $b.1
  //           +                   //           $Q1

  //pCI->PRINTBOARD_dbg();
  //pCI->G.p_boolPred->vDump(__func__);
  vConvert_OQ_sqyPrm_Into_expCol(pCI, pCI->E.p_boolPred);

  //pCI->G.p_boolPred->vDump(__func__);
  vConvert_OQ_sqyNo_Into_expCol(pCI, pCI->E.p_boolPred);

  //pCI->PRINTBOARD_dbg();
  //pCI->G.p_boolPred->vDump(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // move the correlation predicate to table as a bounds

  // TODO: this should be done using standard predicate moving

  pCI->vClearRegisters();
  pCI->vBeginEach();

  //                   v           //  ]filter
  // F A A A P + F F F W A A A     //  .]boolPred
  // s         +       f           //  ..]cmpPred ==
  // f         +       bc          //           $d.2  ((pCI->m_pTerm_corrArg))
  //           + b <-  cp          //             %1

  RASSERT(pCI->bEachOp(CK_c_FOLD));
  RASSERT(pCI->bEachOp(CK_o_WHERE, CK_BOOLPRED));
  vMove_OQ_rlnConj_rslTest_To_from_filter(pCI, pCI->E.p_boolPred);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  pCI->vClearRegisters();
  pCI->vBeginEach();
//  if (pCI->bEachOp(CK_c_FOLD)) {
//    if (pCI->bEachOp(CK_o_WHERE)) {
//      // TODO:
//      // R_vUTE_Unproject_foldExpr_After_where();
//      R_vUTE_Bump_where_BaseOn_foldExpr();
//    }
//  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
