/*============================================================================*/
/* Xreshape.cpp --                                                       */
/*============================================================================*/

#include "Xreshape.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"

#include "RelOp.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Xreshape)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xreshape::Xreshape(eCN eClass, Bench* pBench)
: Xgearing(eClass,pBench)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xreshape::~Xreshape(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   vSeries_10_Condense_Info */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S10_T80_Delete_Redundant_stage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G_vRemoveDeleteMain(CK_STAGE);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S10_T90_Hide_Ignorable_stage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  if (! T_bStage_IsIgnorable(G.p_stage))
    return;

  G.p_stage->Y_vSet(P_HideHeat);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S10_T10_Delete_absJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G_vRemoveDeleteMain(CK_ABSJOIN);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S10_T20_Delete_combined(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G_vRemoveDeleteMain(CK_COMBINED);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S10_T30_Delete_send(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G_vRemoveDeleteMain(CK_SEND);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S10_T40_Delete_stage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G_vRemoveDeleteMain(CK_STAGE);

  vEndReshape();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S10_T50_Cull_scan_Leaving_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.pDeck->vReplaceChildWithItsSubs(G.p_scan);
  DELETE_CARD(G.p_scan);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S10_T60_Add_level(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_level = G.p_MainCard->pSub_level();
  if (G.p_level)
    return;

  G.p_level = new Card_Level(AL_PARTITION);
  G.p_MainCard->vPush(G.p_level);

  /*
  if (CK_INDEXJOIN == G.e_MainCard) {
    G.e_Level = AL_PARTITION;
    G.p_PriorToMain = G.p_MainCard;
    G.e_PriorToMain = G.e_MainCard;
    return;
  }
  if (CK_SCAN == G.e_MainCard) {
    G.e_Level = AL_PARTITION;
    G.p_PriorToMain = G.p_MainCard;
    G.e_PriorToMain = G.e_MainCard;
    return;
  }
  if (CK_TABLE == G.e_MainCard) {
    G.e_Level = AL_PARTITION;
    G.p_PriorToMain = G.p_MainCard;
    G.e_PriorToMain = G.e_MainCard;
    return;
  }

  Card_Level* p_level;
  if (CK_AGGREG == G.e_MainCard) {
    G.e_Level = AL_PARTITION;
    G.p_PriorToMain = G.p_MainCard;
    G.e_PriorToMain = G.e_MainCard;
    return;
  }


//  if ((CK_FILTER == G.e_MainCard || CK_WHERE == G.e_MainCard) && CK_AGGREG != ePrior) {
//    // filter can be at P/N/O
//    return;
//  }
*/


  vEndReshape();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_20_Table_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S20_T10_AAffix_tableSyn_To_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  M_vTable_Set_Syn(G.p_table);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S20_T20_Produce_tablePtn_Under_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_tablePtn = C_pProduce_tablePtn_ofTable(G.p_table);

  G.p_table->vAppend(G.p_tablePtn);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_24_Index_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S24_T10_Produce_indexList_Under_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_tableNm = G.p_table->pSub_tableNm();
  Table* pTable = A_pAccess_Table(G.p_tableNm->sz());

  G.p_indexList = C_pProduce_indexList_ofTable(pTable);

  G_vAppendChild(CK_TABLE,CK_INDEXLIST);

  vEndReshape();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S24_T20_Affix_tblColOdn_To_idxCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  CardIter xCI(G.p_indexList);
  while ((G.p_index = _pCard_Index(xCI.pEachChild()))) {
    G.p_idxColList = G.p_index->pSub_idxColList();
    M_vIdxColList_Affix_tblColOdn(G.p_idxColList, G.p_stream);
  }

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S24_T30(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                 vSeries_22_Stream_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S22_T10_Produce_stream_Under_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  m_pTable = A_pAccess_Table(G.p_tableNm->sz());
  G.p_stream = C_pProduce_stream_ofTable(m_pTable);

  G.p_table->vAppend(G.p_stream);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S22_T12_Produce_stream_Under_project(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  G.p_stream = C_pProduce_stream_ofProject(G.p_projElist);

  G.p_project->vAppend(G.p_stream);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S22_T14_Produce_stream_Under_aggreg(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  G.p_stream = C_pProduce_stream_ofAggreg(G.p_aggElist);

  G.p_aggreg->vAppend(G.p_stream);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S22_T20_Derive_ordinalRange_For_table_stream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  M_vStream_Derive_OrdinalRange_table(G.p_stream, G.p_table->cValue());

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S22_T22_Derive_ordinalRange_For_project_stream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  M_vStream_Derive_OrdinalRange_project(G.p_stream);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S22_T24_Derive_ordinalRange_For_aggreg_stream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  M_vStream_Derive_OrdinalRange_aggreg(G.p_stream);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_26_SubQuery_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S26_T10a_Main_Query_Analysis_aggreg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  // the main-query has an aggreg

  SubQuery* pMainQuery = m_pBench->pGetBoard((i8)0)->pSubQuery();

  pMainQuery->vDisposition(QD_AGGREGATION);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S26_T10b_Main_Query_Analysis_gbyagg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  // the main-query has an gnyAggr

  SubQuery* pMainQuery = m_pBench->pGetBoard((i8)0)->pSubQuery();

  pMainQuery->vDisposition(QD_AGGREGATION);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S26_T10c_Main_Query_Analysis_Of_from(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  SubQuery* pSubQuery = m_pBench->pGetBoard((i8)1)->pSubQuery();

  pSubQuery->vCaller(0);

  pSubQuery->vClause(QC_FROM_CLAUSE);

  SubQuery* pMainQuery = m_pBench->pGetBoard((i8)0)->pSubQuery();
  pMainQuery->vSubQueries(1);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S26_T10d_Main_Query_Analysis_Of_where_rpnCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  SubQuery* pSubQuery = m_pBench->pGetBoard((i8)1)->pSubQuery();

  pSubQuery->vCaller(0);

  // there is a subQuery, how is it called?

  pSubQuery->vClause(QC_WHERE_CLAUSE);

  CardIter xCI(G.p_rpnCalc);
  while ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
    Stz* szValue_term = G.p_term->szValue();

    if (!G_bIsBoard(szValue_term))
      continue;

    if (! (G.p_term = _pCard_Term(xCI.pEachChild())))
      break;

    szValue_term = G.p_term->szValue();

    if (szValue_term->bSame("exists"_Stz)) {
      if ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
        szValue_term = G.p_term->szValue();
        if (szValue_term->bSame("not"_Stz))
          pSubQuery->vCalcOp(QO_NOT_EXISTS_OP);
        else
          pSubQuery->vCalcOp(QO_EXISTS_OP);
      }
      else {
        pSubQuery->vCalcOp(QO_EXISTS_OP);
      }
    }
    else
    if (szValue_term->bSame("in"_Stz)) {
      if ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
        szValue_term = G.p_term->szValue();
        if (szValue_term->bSame("not"_Stz))
          pSubQuery->vCalcOp(QO_NOT_IN_OP);
        else
          pSubQuery->vCalcOp(QO_IN_OP);
      }
      else {
        pSubQuery->vCalcOp(QO_IN_OP);
      }
    }
    else
    {
      eRO eRelnOp = RelOp::eDecode(szValue_term);
      if (RO_NONE != eRelnOp) {
        pSubQuery->vCalcOp(QO_COMPARE_OP);
        pSubQuery->vRelnOp(eRelnOp);
      }
    }

    break;
  }

  SubQuery* pMainQuery = m_pBench->pGetBoard((i8)0)->pSubQuery();
  pMainQuery->vSubQueries(1);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S26_T10e_Main_Query_Analysis_Of_having_rpnCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  SubQuery* pSubQuery = m_pBench->pGetBoard((i8)1)->pSubQuery();

  pSubQuery->vCaller(0);

  // there is a subQuery, how is it called?

  pSubQuery->vClause(QC_HAVING_CLAUSE);

  CardIter xCI(G.p_rpnCalc);
  while ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
    Stz* szValue_term = G.p_term->szValue();

    if (!G_bIsBoard(szValue_term))
      continue;

    if (! (G.p_term = _pCard_Term(xCI.pEachChild())))
      break;

    szValue_term = G.p_term->szValue();

    if (szValue_term->bSame("exists"_Stz)) {
      if ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
        szValue_term = G.p_term->szValue();
        if (szValue_term->bSame("not"_Stz))
          pSubQuery->vCalcOp(QO_NOT_EXISTS_OP);
        else
          pSubQuery->vCalcOp(QO_EXISTS_OP);
      }
      else {
        pSubQuery->vCalcOp(QO_EXISTS_OP);
      }
    }
    else
    if (szValue_term->bSame("in"_Stz)) {
      if ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
        szValue_term = G.p_term->szValue();
        if (szValue_term->bSame("not"_Stz))
          pSubQuery->vCalcOp(QO_NOT_IN_OP);
        else
          pSubQuery->vCalcOp(QO_IN_OP);
      }
      else {
        pSubQuery->vCalcOp(QO_IN_OP);
      }
    }
    else
    {
      eRO eRelnOp = RelOp::eDecode(szValue_term);
      if (RO_NONE != eRelnOp) {
        pSubQuery->vCalcOp(QO_COMPARE_OP);
        pSubQuery->vRelnOp(eRelnOp);
      }
    }

    break;
  }

  SubQuery* pMainQuery = m_pBench->pGetBoard((i8)0)->pSubQuery();
  pMainQuery->vSubQueries(1);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S26_T10f_Main_Query_Genesis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G_vNewMember(CK_GENESIS,"mq"_Stz);
  if ((G.p_level = G.p_MainCard->pSub_level()))
    G.p_MainCard->vInsertAfter(G.p_genesis,G.p_level);
  else
    G.p_MainCard->vPush(G.p_genesis);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S26_T20a_Sub_Query_Analysis_Only_main(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  // there is no sub-query

  SubQuery* pMainQuery = m_pBench->pGetBoard((i8)0)->pSubQuery();

  pMainQuery->vSubQueries(0);


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S26_T20b_Sub_Query_Analysis_paramList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  // there is a subQuery and it has a paramList

  SubQuery* pSubQuery = m_pBench->pGetBoard(1)->pSubQuery();

  i8 nParams = G.p_paramList->nChildCards();
  pSubQuery->vCorrCols(nParams);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S26_T20c_Sub_Query_Analysis_aggreg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  // there is a subQuery and it has an aggreg

  SubQuery* pSubQuery = m_pBench->pGetBoard(1)->pSubQuery();

  pSubQuery->vDisposition(QD_AGGREGATION);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S26_T20d_Sub_Query_Analysis_gbyagg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  // there is a subQuery and it has an gbyAgg
  SubQuery* pSubQuery = m_pBench->pGetBoard(1)->pSubQuery();
  pSubQuery->vDisposition(QD_AGGREGATION);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S26_T10e_Sub_Query_Genesis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G_vNewMember(CK_GENESIS,"sq"_Stz);
  if ((G.p_level = G.p_MainCard->pSub_level()))
    G.p_MainCard->vInsertAfter(G.p_genesis,G.p_level);
  else
    G.p_MainCard->vPush(G.p_genesis);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S26_T30(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);


  vEndReshape();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                      vSeries_32_To_BoolCalc */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S32_T10_Convert_sqyOp_not_To_notSqyOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  M_vRpnCalc_Convert_sqyOp_not_To_notSqyOp(G.p_rpnCalc);

  vEndReshape();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S32_T20_Convert_where_rpnCalc_To_boolCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_boolCalc = C_pConvert_rpnCalc_To_boolCalc(G.p_rpnCalc);

  G.p_where->vReplaceSubCard(G.p_rpnCalc,G.p_boolCalc);

  G_vDeleteMember(G.p_rpnCalc);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    vSeries_33_To_TblColOdn */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S33_T10_Convert_stmColOdn_To_tblColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this](Card* pCard)
  {
    Card_Term* p_term = _pCard_Term(pCard);
    if (! T_bValue_IsStreamColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    Stz* szStmColOdn = p_term->szValue();
    if (! G_bResolveSubCard(CK_STREAM,CK_STMCOLODN,szStmColOdn,G.i_MainCard))
      return C_CONTINUE_RECURSION;

    RASSERT(m_pCard_parent->bIsCard(CK_STMCOL));

    G.p_tblColOdn = m_pCard_parent->pSub_tblColOdn();
    M_vCard_Set_Value(p_term, G.p_tblColOdn->szValue(),MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_MainCard->bApplyRecursive(CK_TERM, bLambdaFn);


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                vSeries_34_Predicate_Pullup */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S34_T10_AtMark_where_pred_Wrt_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  vPullUp_MarkPredLeftwards(G.p_boolCalc, G.pDeck);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S34_T20_Move_AtMarked_where_pred_To_table_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  vPullUp_MovePredLeftwards(G.p_boolCalc, G.pDeck);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S34_T30_Delete_Empty_where(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  if (0 == G.p_boolCalc->nChildCards()) {
    G_vRemoveDeleteMain(CK_WHERE);
  }

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_36_Bound_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S36_T10_UpCopy_filter_pred_Into_bounds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  C_vBounds_DeriveFrom_filter(G.p_table, G.p_filter);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S36_T20_Cull_UpCopied_filter_pred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_boolCalc = G.p_filter->pSub_boolCalc();

  M_vBoolCalc_CullPred(G.p_boolCalc, MARK_UP);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S36_T30_Delete_Empty_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  if (! T_bFilter_IsEmpty(G.p_filter))
    return;

  G.p_MainCard->vRemoveChild(G.p_filter);
  DELETE_CARD(G.p_filter);

  vEndReshape();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   vSeries_40_Flatten_Query */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S40_T10a_Fold_F_Before_Mq_As_opSeq(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S40_T20a_Fold_W_A_C_ro_Before_Mq_As_AggFanKcorrAval_GbyKey(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S40_T30a_Fold_W_A_NC_I_Before_Mq_As_aggFanAval(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S40_T40a_Fold_W_A_NC_ro_Before_Mq_As_aggFanAval(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S40_T50a_Fold_W_L_C_E_Before_Mq_As_keyExists(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  vEndReshape();
}
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S40_T60a_Fold_W_L_NC_NI_Into_Mq_As_keyNotExists(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S40_T70a_Fold_H_A_NC_ro_Before_Mq_As_AggFan_AvalInjectMq(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S40_Ute_a(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S40_Ute_b(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S40_Ute_c(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S40_Ute_d(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S40_Ute_e(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S40_Ute_f(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   vSeries_50_Join_Ordering */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T00_Extract_tableList_From_queryBoard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_tableList->vAppend(G.p_table->pDupCardAndSubCards());

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T10_Extract_joinPredList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  D_vDerive_JoinPredList(G.p_joinPredList, G.p_boolCalc);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T20_Compute_coPtnPredList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_coPtnPred = new Card_CoPtnPred;
  G.p_coPtnPred->vAppend(G.p_cmpPred->pDupCardAndSubCards());

  G.p_coPtnPredList->vAppend(G.p_coPtnPred);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T30_Compute_coPtnJoinList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_coPtnJoinList = new Card_CoPtnJoinList;
  G.pDeck->vAppendMain(G.p_coPtnJoinList);

  G.p_coPtnPredList =
      _pCard_CoPtnPredList(G.pDeck->pFindChild(CK_COPTNPREDLIST));

  i8 nChildren = G.p_coPtnPredList->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    G.p_coPtnPred =
        _pCard_CoPtnPred(G.p_coPtnPredList->pGetChild(i_Child));

    G.p_subSet = G.p_coPtnPred->pDupSubCards();

    if (! (G.p_coPtnJoin = T_pCoPtnJoinList_Find_coPtn_for(G.p_coPtnJoinList, G.p_coPtnPred)) ) {
      G.p_coPtnJoin = new Card_CoPtnJoin;
      G.p_coPtnJoinList->vAppend(G.p_coPtnJoin);
    }

    G.p_coPtnJoin->vTransferChildren(G.p_subSet);
    DELETE_CARD(G.p_subSet);
  }


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T40_Compute_crossPtnJoinList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_joinPredList =
      _pCard_JoinPredList(G.pDeck->pFindChild(CK_JOINPREDLIST));

  G.p_crossPtnJoinList = new Card_CrossPtnJoinList;
  G.pDeck->vAppendMain(G.p_crossPtnJoinList);

  i8 nChildren = G.p_joinPredList->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    G.p_cmpPred =
        _pCard_CmpPred(G.p_joinPredList->pGetChild(i_Child));


    if (T_bCoPtnJoinList_HasEqPredCmp(G.p_coPtnJoinList, G.p_cmpPred))
      continue;

    G.p_crossPtnJoin = new Card_CrossPtnJoin;
    G.p_crossPtnJoin->vAppend(G.p_cmpPred->pDupCardAndSubCards());
    G.p_crossPtnJoinList->vAppend(G.p_crossPtnJoin);
  }

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T50_Compute_ptndJoinSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_ptndJoinSet = new Card_PtndJoinSet;
  G.pDeck->vAppendMain(G.p_ptndJoinSet);

  G.p_coPtnJoinList =
      _pCard_CoPtnJoinList(G.pDeck->pFindChild(CK_COPTNJOINLIST));
  G.p_subSet = G.p_coPtnJoinList->pDupSubCards();
  G.p_ptndJoinSet->vTransferChildren(G.p_subSet);
  DELETE_CARD(G.p_subSet);

  G.p_crossPtnJoinList =
      _pCard_CrossPtnJoinList(G.pDeck->pFindChild(CK_CROSSPTNJOINLIST));
  G.p_subSet = G.p_crossPtnJoinList->pDupSubCards();
  G.p_ptndJoinSet->vTransferChildren(G.p_subSet);
  DELETE_CARD(G.p_subSet);

  i8 nChildren = G.p_ptndJoinSet->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    m_pCard_child = G.p_ptndJoinSet->pGetChild(i_Child);
    m_pCard_child->vCard(CK_PTNDJOIN);
    m_pCard_child->Y_vSet(P_PtndJoin, i_Child);
  }

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T60_Copy_ptndJoinSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  m_pBench->pCardCtl()->vNextHorizon(CK_PERMUTATION, true);

  G.p_ptndJoinSet =
      _pCard_PtndJoinSet(G.pDeck->pFindChild(CK_PTNDJOINSET));

  G.pDeck->vAppendMain(new Card_Note("PERMUTATIONS"_Stz));

  i8 i_PtdnJoin = 0;
  G.p_subSet = G.p_ptndJoinSet->pDupSubCards();
  while ((m_pCard_popped = G.p_subSet->pPop())) {
    G.pDeck->vAppendMain(m_pCard_popped);
  }
  DELETE_CARD(G.p_subSet);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T70_Generate_ptndPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  m_pPermGen = new PermGen(G.p_ptndJoinSet->nChildCards());

  G.p_subSet = new Card_SubSet;

  while (m_pPermGen->bNextPermutation()) {
    G.pDeck->vAppendMain((G.p_permutation = new Card_Permutation));
    G.p_permutation->Y_vSet(P_Permutation, m_pPermGen->nPermutations() - 1);

    RASSERT(0 == G.p_subSet->nChildCards());
    for (i8 i_Item = 0; i_Item < m_pPermGen->nItems(); i_Item++) {
      i8 i_Child = m_pPermGen->iItem(i_Item);
      m_pCard_child = G.p_ptndJoinSet->pGetChild(i_Child);
      G.pDeck->vAppendMain(m_pCard_child->pDupCardAndSubCards());

      G.p_ptndJoin = _pCard_PtndJoin(m_pCard_child);
      if (!T_bPtndJoin_MayFollowAfter(G.p_ptndJoin, G.p_subSet)) {
        G.pDeck->vAppendMain(new Card_Note("**DOESN'T FOLLOW**"_Stz));
        G.p_permutation->Y_vSet(P_Invalid);
        break;
      }
      G.p_subSet->vAppend(m_pCard_child);
    }
    G.p_subSet->vRemoveAll();
  }

  DELETE_CARD(G.p_subSet);


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T80_Remove_invalidPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  i8 i_Main = 0;
  i8 i_Permutation = 0;
  while (i_Main < G.pDeck->nChildCards()) {
    G.p_MainCard = G.pDeck->pGetChild(i_Main);

    if (CK_PERMUTATION != G.p_MainCard->eCard()) {
      i_Main++;
      continue;
    }

    if (! G.p_MainCard->bHasProperty(P_Invalid)) {
      G.p_MainCard->Y_vSet(P_Permutation, i_Permutation++);
      i_Main++;
      continue;
    }


    G.pDeck->vRemoveChild(G.p_MainCard);
    while (i_Main < G.pDeck->nChildCards()) {
      G.p_MainCard = G.pDeck->pGetChild(i_Main);
      if (CK_PTNDJOIN != G.p_MainCard->eCard() && CK_NOTE != G.p_MainCard->eCard())
        break;
      G.pDeck->vRemoveChild(G.p_MainCard);
      delete G.p_MainCard;
    }
  }


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T90_Setup_costEst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  while ((G.p_MainCard = G.pDeck->pGetChild(0))) {

    if (CK_PERMUTATION == G.p_MainCard->eCard())
      break;

    G.pDeck->vRemoveChild(G.p_MainCard);
    delete G.p_MainCard;
  }


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T92_Estimate_ptndJoins(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_tableList =
      _pCard_TableList(G.pDeck->pFindChild(CK_TABLELIST));


  BoardIter xBI(G.pBoard);

//  while (G.p_ptndJoin = _pCard_PtndJoin(xBI.pEachMain(CK_PTNDJOIN))) {
  while ((G.p_MainCard = xBI.pEachMain())) {
    eCK eCard = G.p_MainCard->eCard();
    if (CK_PERMUTATION == eCard) {
      G.p_tableList->vDeletePropertyRecursive(P_TableUsed);
      continue;
    }
    if (CK_PTNDJOIN != eCard)
      continue;


    G.p_ptndJoin = _pCard_PtndJoin(G.p_MainCard);
    D_vDerive_PtndJoin(G.p_ptndJoin, G.p_tableList);
  }


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T94_Estimate_network(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  BoardIter xBI(G.pBoard);

  i8 nNetwork = 0;
  i8 nSentOut = 0;
  G.p_permutation = nullptr;
  G.p_ptndJoin    = nullptr;

  while ((G.p_MainCard = xBI.pEachMain())) {


    eCK eCard = G.p_MainCard->eCard();
    if (CK_PERMUTATION == eCard) {
      if (G.p_permutation)
        G.p_permutation->Y_vSet(P_Network, nNetwork);
      G.p_permutation = _pCard_Permutation(G.p_MainCard);

      G.p_ptndJoin = nullptr;
      nNetwork = 0;
      nSentOut = 0;
    }
    else
    if (CK_PTNDJOIN == eCard) {
      if (G.p_ptndJoin) {
        nSentOut += G.p_ptndJoin->Y_iVal(P_JoinedBuff);
        nNetwork += nSentOut;
        G.p_ptndJoin->Y_vSet(P_SentOut, nSentOut);
      }
      G.p_ptndJoin = _pCard_PtndJoin(G.p_MainCard);
    }
  }

  if (G.p_permutation)
    G.p_permutation->Y_vSet(P_Network, nNetwork);


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T96_Estimate_joinOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  i8 iLowestNetwork = 999999999;

  BoardIter xBI(G.pBoard);

  while ((G.p_permutation =
              _pCard_Permutation(xBI.pEachMain(CK_PERMUTATION)))) {
    i8 iNetwork = G.p_permutation->Y_iVal(P_Network);
    if (iNetwork >= iLowestNetwork)
      continue;
    G.p_permutation_low = G.p_permutation;
    iLowestNetwork = iNetwork;
  }

  G.pDeck->vAppendMain((G.p_joinOrderSeq = new Card_JoinOrderSeq));
  G.p_joinOrderSeq->Y_vSet(P_Network, iLowestNetwork);

  xBI.vSet(G.p_permutation_low);
  while ((G.p_MainCard = xBI.pEachMain())) {
    if (CK_PTNDJOIN != G.p_MainCard->eCard())
      break;

    G.p_ptndJoin = _pCard_PtndJoin(G.p_MainCard);
    D_vDerive_JoinOrderSeq(G.p_joinOrderSeq, G.p_ptndJoin);
  }


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T98_Extract_joinOrderSeq(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.pDeck->vAppendMain((G.p_joinOrderSeq = new Card_JoinOrderSeq));

  BoardIter xBI(G.pBoard);

  xBI.vSet(G.p_permutation_low);
  while ((G.p_MainCard = xBI.pEachMain())) {
    if (CK_PTNDJOIN != G.p_MainCard->eCard())
      break;


    G.p_ptndJoin = _pCard_PtndJoin(G.p_MainCard);
    D_vDerive_JoinOrderSeq(G.p_joinOrderSeq, G.p_ptndJoin);
  }

  vEndReshape();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                      vSeries_60_Index_Join */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S60_T05_Twist_in_Becomes_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  RASSERT(G.p_tstPred);

  char cTblSyn = D_cPRED_TableSyn_First(G.p_tstPred);

  if (' ' == cTblSyn)
    return;

  s32 sTblSyn;
  sTblSyn.vSprint("%c"_Stz,cTblSyn);
  if (! G_bLocateCard(CK_TABLE,sTblSyn,C_UNKNOWN))
    return;

  vDebugPoint();

  G.p_table = _pCard_Table(G.p_Located);
  G.p_filter = G.p_table->pSub_filter();
  if (! G.p_filter) {
    G_vNewMember(CK_FILTER);
    G.p_table->vAppend(G.p_filter);
    G_vNewMember(CK_BOOLCALC);
    G.p_filter->vAppend(G.p_boolCalc);
  }

  Card* p_copy = G.p_tstPred->pDupCardAndSubCards();
  p_copy->vCard(CK_CMPPRED);
  p_copy->vValue("=="_Stz);

  G.p_boolCalc->vAppend(p_copy);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S60_T10_Produce_maybeIndexJoin_Under_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  M_vTable_Produce_MaybeIndexJoin(G.p_table);

  vEndReshape();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S60_T20_Affix_tblColOdn_To_idxCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_stream = G.p_table->pSub_stream();;
  G.p_stmColList = G.p_table->pSub_stmColList();
  G.p_idxColList = G.p_maybeIndexJoin->pSub_idxColList();

  M_vIdxColList_Affix_tblColOdn(G.p_idxColList, G.p_stream);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S60_T60_Delete_Twist(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G_vRemoveDeleteMain(CK_TWIST);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    vSeries_75_EE_operators */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S75_T10_Morph_table_To_EEop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  if (! G.p_bounds) {

    G.p_scan = new Card_Scan;
    G.p_scan->vTransferChildren(G.p_table);

    G.pBoard->pDeck()->vReplace(G.p_table,G.p_scan);
    G.p_scan->vInsert(G.p_table);

    if (G.p_indexJoin) {
      G.p_scan->vRemoveChild(G.p_indexJoin);
      DELETE_CARD(G.p_indexJoin);
    }
    return;
  }

  if (G.p_keyExists) {
    G.p_table->vRemoveSubCard(G.p_keyExists);

    G.p_table->vRemoveSubCard(G.p_maybeIndexJoin);
    G.p_maybeIndexJoin->vTransferChildren(G.p_table);

    G.pBoard->pDeck()->vReplace(G.p_table, G.p_maybeIndexJoin);
    G.p_maybeIndexJoin->vInsert(G.p_table);

    G.p_maybeIndexJoin->vRemoveSubCard(G.p_index);
    G.p_maybeIndexJoin->vAppend(G.p_index);

    G.p_maybeIndexJoin->vCard(CK_KEYEXISTS);
    DELETE_CARD(G.p_keyExists);
    return;
  }

  if (G.p_maybeIndexJoin) {
    G.p_table->vRemoveSubCard(G.p_maybeIndexJoin);
    G.p_maybeIndexJoin->vTransferChildren(G.p_table);

    G.pBoard->pDeck()->vReplace(G.p_table, G.p_maybeIndexJoin);
    G.p_maybeIndexJoin->vInsert(G.p_table);

    G.p_maybeIndexJoin->vRemoveSubCard(G.p_index);
    G.p_maybeIndexJoin->vAppend(G.p_index);
    G.p_maybeIndexJoin->vCard(CK_INDEXJOIN);
    return;
  }

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S75_T10_Morph_table_To_EEop_b(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  i8 i_Child;
  G.p_note = _pCard_Note(G.p_scan->pFindChild(CK_NOTE,i_Child));

  if (i_Child == G.p_scan->nChildCards() - 1) {
    G.p_scan->vRemoveChild(G.p_note);
    DELETE_CARD(G.p_note);
  }


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S75_T10_Morph_table_To_EEop_c(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  G.p_indexJoin->vDump();

  i8 i_Child;
  G.p_note = _pCard_Note(G.p_indexJoin->pFindChild(CK_NOTE,i_Child));
  G.p_note->vValue(SEP_DERIVED);


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S75_T20_Morph_keyExists_filter_To_constraint(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  G.p_filter->vCard(CK_CONSTRAINT);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S75_T30_Correct_stmColOdn_Of_keyExists_stream(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  i8 iOrdLo_stream = E_iStream_OdnLo(G.p_stream);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this, iOrdLo_stream](Card* pCard)
  {
    Card_StmColOdn* p_stmColOrd = _pCard_StmColOdn(pCard);
    if (! T_bValue_IsStreamColOrdinal(p_stmColOrd->szValue()))
      return C_CONTINUE_RECURSION;

    i8 iStmColOdn = E_iValue_StreamColOdn(p_stmColOrd->szValue());
    i8 iStmColOdn_corrected = iOrdLo_stream + iStmColOdn - 1;
    M_vCard_Set_StreamColOrdinal(p_stmColOrd, iStmColOdn_corrected);

    p_stmColOrd->vMark1(MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_stream->bApplyRecursive(CK_STMCOLODN, bLambdaFn);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  i8 i_Main = G.pDeck->iFindChild(G.p_MainCard);

  Card_BoolCalc* p_boolCalc = G.p_constraint->pSub_boolCalc();
  RASSERT(1 == p_boolCalc->nChildCards());

  Card_PRED* p_pred_ROOT = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this,i_Main](Card_PRED* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_pred);
    if (! T_bValue_IsStreamColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    G_bResolveSubCard(CK_STMCOL, CK_STMCOLODN, p_term->szValue(), i_Main + 1);

    Card_StmColNm* p_stmColNm = G.p_stmCol->pSub_stmColNm();

    M_vCard_Set_Value(p_term,p_stmColNm->szValue(),MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = M_bPredRecursive(p_pred_ROOT,bLambdaFn);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                      vSeries_80_Index_Scan */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S80_T10_Produce_indexList_Under_scan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  M_vScan_Produce_indexList(G.p_scan);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S80_T20_Affix_idxCol_name_Of_scan_To_ordinal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  M_vIdxColList_Affix_tblColOdn(G.p_idxColList, G.p_stream);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S80_T30_Mark_ULBoundsOf_scan_filter_cmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfMarkPredBounds = [this](Card* pCard) {
    Card_CmpPred* p_cmpPred = _pCard_CmpPred(pCard);
    M_vCmpPred_MarkBound(p_cmpPred);
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_filter->bApplyRecursive(CK_CMPPRED, bfMarkPredBounds);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S80_T40_Mark_IndexMatchOf_scan_filter_terms(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  if (! (G.p_index = _pCard_Index(G.p_indexList->pGetChild(0))))
    return;
  if (! (G.p_idxColList = (G.p_index->pSub_idxColList())))
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfPredBounds = [this](Card* pCard) {
    Card_Term* p_term = _pCard_Term(pCard);
    M_vTerm_MarkIfMatchesAn_idxCol(p_term, G.p_idxColList);
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_filter->bApplyRecursive(CK_TERM, bfPredBounds);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S80_T50_Add_scan_bounds_ByCopyingMarked_filter_cmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfAdd = [this](Card* pCard) {
    G.p_cmpPred = _pCard_CmpPred(pCard);
    M_vScan_Add_cmpPred_To_bounds(G.p_scan, G.p_cmpPred);
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_filter->bApplyRecursive(CK_CMPPRED, bfAdd);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S80_T60_Cull_Marked_filter_cmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  vTracer("Culling <filter> of L & U bounds %s\n"_Stz,G.p_filter->szBearing());
  M_vFilter_CullPred(G.p_filter, 'L');
  M_vFilter_CullPred(G.p_filter, 'U');

  //G.p_filter->vDump();
  if (!T_bFilter_IsEmpty(G.p_filter))
    return;

  vTracer("Deleting empty <filter> %s\n"_Stz,G.p_filter->szBearing());
  G.p_MainCard->vRemoveSubCard(G.p_filter);
  DELETE_CARD(G.p_filter);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S80_T70_Morph_scan_To_indexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_scan->vCard(CK_INDEXSCAN);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S80_T70_Morph_scan_To_indexScan_b(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  i8 i_Child;
  G.p_note = _pCard_Note(G.p_indexScan->pFindChild(CK_NOTE,i_Child));
  G.p_note->vValue(SEP_DERIVED);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S80_T70_Morph_scan_To_indexScan_c(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_indexScan->vReplaceChildWithItsSubs(G.p_indexList);
  DELETE_CARD((G.p_indexList));

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                      vSeries_85_Cross_Join */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S85_T10a_Transmute_table_To_Scan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_scan = new Card_Scan;
  G.p_scan->vTransferChildren(G.p_table);

  G.pBoard->pDeck()->vReplace(G.p_table,G.p_scan);
  G.p_scan->vInsert(G.p_table);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S85_T10b_Transmute_table_To_crossJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_crossJoin = new Card_CrossJoin;
  G.p_crossJoin->vTransferChildren(G.p_table);

  G.pBoard->pDeck()->vReplace(G.p_table,G.p_crossJoin);
  G.p_crossJoin->vInsert(G.p_table);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S85_T20_Add_index_For_crossJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  m_pTable = A_pAccess_Table(G.p_tableNm->szValue());
  G.p_index = C_pProduce_index_ofTable(m_pTable);

  G.p_note = new Card_Note(SEP_DERIVED);
  G.p_crossJoin->vAppend(G.p_note);

  G.p_crossJoin->vAppend(G.p_index);


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S85_T30_Add_bounds_For_crossJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G_vNewMember(CK_BOUNDS);
  G.p_crossJoin->vAppend(G.p_bounds);

  G_vNewMember(CK_TERM,"bound"_Stz);
  G.p_bounds->vAppend(G.p_term);

  G_vNewMember(CK_TERM,"$I1"_Stz);
  G.p_bounds->vAppend(G.p_term);

  vEndReshape();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                 vSeries_90_Stream_Ordering */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S90_T10_Remove_sort_IfSameAs_gby(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_gbyElist = G.p_aggreg->pSub_gbyElist();
  G.p_sbyElist = G.p_sort->pSub_sbyElist();

  // TODO: needs to be a T_ test
  if (! G.p_gbyElist->bValuesSameRecursive(G.p_sbyElist))
    return;

  G_vRemoveDeleteMain(CK_SORT);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                               vSeries_A0_Filter_Refinement */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_SA0_T10_Individuate_scan_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_scan->vRemoveChild(G.p_filter);
  G.pDeck->vInsertAfter(G.p_filter, G.p_scan);

  if (! G.p_filter->pSub_level()) {
    G.p_filter->vPush(G.p_scan->pSub_level()->pDupCardAndSubCards());
  }

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_SA0_T10_Individuate_indexJoin_filter_b(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_indexJoin->vRemoveChild(G.p_filter);
  G.pDeck->vInsertAfter(G.p_filter, G.p_indexJoin);

  if (! G.p_filter->pSub_level()) {
    G.p_filter->vPush(G.p_indexJoin->pSub_level()->pDupCardAndSubCards());
  }

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                vSeries_Z0_Restate_Pipeline */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_SZ0_T10_Restore_stmColOdn_from_tblColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this](Card* pCard)
  {
    Card_Term* p_term = _pCard_Term(pCard);

    if (T_bValue_IsResultOrdinal(p_term->szValue())) {

      i8 iResultOrd = E_iValue_ResultOdn(p_term->szValue());

      // TODO: put this in a fn
      s32 sStmColOdn;
      sStmColOdn.vSprint("$%lld"_Stz,iResultOrd);
      p_term->vValue(sStmColOdn);

      return C_CONTINUE_RECURSION;
    }


    if (! T_bValue_IsTableColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    Stz* szTblColOdn = p_term->szValue();
    if (! G_bResolveSubCard(CK_TABLE,CK_TBLCOLODN,szTblColOdn,G.i_MainCard))
      return C_CONTINUE_RECURSION;

    RASSERT(m_pCard_parent->bIsCard(CK_STMCOL));

    G.p_stmColOdn = m_pCard_parent->pSub_stmColOdn();
    M_vCard_Set_Value(p_term, G.p_stmColOdn->szValue(),MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_MainCard->bApplyRecursive(CK_TERM, bLambdaFn);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_SZ0_T20_Reinstate_stages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  if (! T_bStage_Reinstate(G.e_MainCard, G.e_SubsequentToMain))
    return;

  m_pCard_dup = G.p_stage->pDupCardAndSubCards();
  G.pDeck->vInsertAfter(m_pCard_dup, G.p_MainCard);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_SZ0_T20_Reinstate_stagesV2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  if (! T_bStage_ReinstateV3(G.e_Level, G.p_MainCard, G.p_PriorToMain)) {
    G.p_PriorToMain = G.p_MainCard;
    return;
  }

  G.p_PriorToMain = G.p_MainCard;

  G.p_stage = new Card_Stage;
  G.p_topology = new Card_Topology(M_eLevel_To_StageTopology(G.e_Level));
  G.p_stage->vAppend(G.p_topology);
  G.pDeck->vInsertBefore(G.p_stage, G.p_MainCard);
  G_vSkipNextMain();

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_SZ0_T20_Reinstate_stagesV3(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  i8 nStages = 0;

  Card_Stage* p_stage = new Card_Stage;
  p_stage->vValueSprint("S%02lld"_Stz,++nStages);
  Card_Topology* p_topology = new Card_Topology(YG_ALL_PARTITIONS);
  p_stage->vAppend(p_topology);
  G.pDeck->vPush(p_stage);

  eAL e_level_running = M_eStageTopology_To_Level(YG_ALL_PARTITIONS);

  bool bSawProjectOp = false;
  bool bSawAggOp = false;

  auto vAddCombined = [this] (Card* p_main, i8& ri_main) {
    Card_Combined* p_combined = new Card_Combined;
    if (C_UNKNOWN == ri_main) {
      G.pDeck->vAppend(p_combined);
      return;
    }
    G.pDeck->vInsertBefore(p_combined, p_main);
    ri_main++;
    RASSERT(G.pDeck->pGetChild(ri_main) == p_main);
  };

  auto vAddNext = [this] (Card* p_main, i8& ri_main, Stz* szNextValue) {
    Card_Next* p_next = new Card_Next;
    p_next->vValue(szNextValue);
    if (C_UNKNOWN == ri_main) {
      G.pDeck->vAppend(p_next);
      return;
    }
    G.pDeck->vInsertBefore(p_next, p_main);
    ri_main++;
    RASSERT(G.pDeck->pGetChild(ri_main) == p_main);
  };

  auto vAddStage = [this] (Card* p_main, i8& ri_main, eAL e_level_running, i8& nStages) {
    Card_Stage *p_stage = new Card_Stage;
    p_stage->vValueSprint("S%02lld"_Stz,++nStages);
    eYG eStageTopo = M_eLevel_To_StageTopology(e_level_running);
    Card_Topology *p_topology = new Card_Topology(eStageTopo);
    p_stage->vAppend(p_topology);
    G.pDeck->vInsertBefore(p_stage, p_main);
    ri_main++;
    RASSERT(G.pDeck->pGetChild(ri_main) == p_main);
  };

  Card* p_prev = nullptr;
  eCK   e_prev = CK_NONE;
  Card* p_main = p_stage;
  eCK   e_main = p_main->eCard();

  for (i8 i_Main = 1; i_Main < G.pDeck->nChildCards(); i_Main++) {

    p_prev = p_main;
    e_prev = e_main;

    p_main = G.pDeck->pGetChild(i_Main);
    e_main = p_main->eCard();
    eAL e_level_main = (eAL)p_main->pSub_level()->Y_iVal(P_Level);

    // change of level
    if (e_level_main != e_level_running) {
      if (CK_SCAN != e_prev)
        vAddCombined(p_main, i_Main);
      if (e_level_running == AL_ORIGIN &&
          e_level_main == AL_PARTITION)
        vAddNext(p_main, i_Main,"broadcast"_Stz);
      else
        vAddNext(p_main, i_Main,""_Stz);
      e_level_running = e_level_main;
      vAddStage(p_main, i_Main, e_level_running,nStages);
      if (CK_GBYAGG == e_main || CK_AGGREG == e_main)
        bSawAggOp = true;
      else
        bSawAggOp = false;
      if (CK_PROJECT == e_main)
        bSawProjectOp = true;
      else
        bSawProjectOp = false;
      continue;
    }

    if (CK_CROSSJOIN == e_main || CK_INDEXJOIN == e_main ) {
      if (CK_SCAN != e_prev)
        vAddCombined(p_main, i_Main);
      vAddNext(p_main, i_Main,""_Stz);
      vAddStage(p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawProjectOp = false;
      continue;
    }

    if (CK_PROJECT == e_main && (bSawAggOp || bSawProjectOp) ) {
      if (CK_SCAN != e_prev)
        vAddCombined(p_main, i_Main);
      vAddNext(p_main, i_Main,""_Stz);
      vAddStage(p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawProjectOp = false;
      continue;
    }

    if (CK_AGGREG == e_main && (bSawAggOp || bSawProjectOp)) {
      vAddNext(p_main, i_Main,""_Stz);
      vAddStage(p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawProjectOp = false;
      continue;
    }

    if (CK_GBYAGG == e_main && (bSawAggOp || bSawProjectOp)) {
      vAddNext(p_main, i_Main,""_Stz);
      vAddStage(p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawProjectOp = false;
      continue;
    }

    // if filter then continue?
    if (CK_FILTER == e_main) {
      continue;
    }

    if (bSawAggOp) {
      vAddCombined(p_main, i_Main);
      vAddNext(p_main, i_Main,""_Stz);
      vAddStage(p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawProjectOp = false;
      continue;
    }

    if (CK_GBYAGG == e_main || CK_AGGREG == e_main)
      bSawAggOp = true;
    if (CK_PROJECT == e_main)
      bSawProjectOp = true;
  }

  i8 i_End = C_UNKNOWN;
  if (CK_SCAN != e_prev)
    vAddCombined(p_main, i_End);
  vAddNext(p_main, i_End,""_Stz);

  vEndReshape();
}
/*----------------------------------------------------------------------------*/
//void Xreshape::vR_SZ0_T20_Reinstate_stagesV3(void)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
//
//  Card_Stage* p_stage = new Card_Stage;
//  Card_Topology* p_topology = new Card_Topology(YG_ALL_PARTITIONS);
//  p_stage->vAppend(p_topology);
//  G.pDeck->vPush(p_stage);
//
//  eCK e_prev = CK_STAGE;
//
//  eAL eLevel_running = M_eStageTopology_To_Level(YG_ALL_PARTITIONS);
//
//  i8 nOpsInStage = 0;
//
//  bool bAddCombined = false;
//  bool bAddStage    = false;
//
//  for (i8 i_Main = 1; i_Main < G.pDeck->nChildCards(); i_Main++) {
//    Card* p_main = G.pDeck->pGetChild(i_Main);
//    eCK e_main = p_main->eCard();
//    eAL e_level_main = (eAL)p_main->pSub_level()->Y_iVal(P_Level);
//
//    Card* p_next = nullptr;
//    eCK e_next = CK_NONE;
//    eAL e_level_next = (eAL)p_main->pSub_level()->Y_iVal(P_Level);
//    if (i_Main < G.pDeck->nChildCards() - 1) {
//      p_next = G.pDeck->pGetChild(i_Main + 1);
//      e_next = p_next->eCard();
//      e_level_next = (eAL)p_next->pSub_level()->Y_iVal(P_Level);
//    }
//
//
//    switch (e_main) {
//
//      case CK_AGGREG:
//        bAddCombined = true;
//        if (CK_FILTER != e_next)
//          bAddStage = true;
//        break;
//
//      case CK_FILTER:
//      case CK_HAVING:
//        break;
//
//      case CK_INDEXJOIN:
//        bAddCombined = true;
//        if (CK_FILTER != e_next)
//          bAddStage = true;
//        break;
//
//      case CK_INDEXSCAN:
//        bAddCombined = true;
//        if (CK_FILTER != e_next)
//          bAddStage = true;
//        break;
//
//      case CK_PROJECT:
//        bAddCombined = true;
//        break;
//
//      case CK_SCAN:
//        RASSERT(0 == nOpsInStage);
//        break;
//
//      default:
//        NEVER_REACH;
//    }
//
//    e_prev = e_main;
//
//    if (! bAddStage)
//      continue;
//
//    if (bAddCombined) {
//      Card_Combined* p_combined = new Card_Combined;
//
//      G.pDeck->vInsertBefore(p_combined, p_main);
//      i_Main++;
//      RASSERT(G.pDeck->pGetChild(i_Main) == p_main);
//    }
//
//    G.pDeck->vInsertBefore(p_next, p_main);
//    i_Main++;
//    RASSERT(G.pDeck->pGetChild(i_Main) == p_main);
//
//    Card_Stage *p_stage = new Card_Stage;
//    eYG eStageTopo = M_eLevel_To_StageTopology(eLevel_running);
//    Card_Topology *p_topology = new Card_Topology(eStageTopo);
//    p_stage->vAppend(p_topology);
//
//    G.pDeck->vInsertBefore(p_stage, p_main);
//    i_Main++;
//    RASSERT(G.pDeck->pGetChild(i_Main) == p_main);
//
//    bAddCombined = false;
//    bAddStage    = false;
//  }
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_SZ0_T30_Reinstate_combined(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_combined = new Card_Combined;
  G.pDeck->vInsertAfter(G.p_combined, G.p_MainCard);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_SZ0_T40_Reinstate_next(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G_vNewMember(CK_NEXT);
  G.pDeck->vInsertBefore(G.p_next, G.p_MainCard);
  G_vSkipNextMain();

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_SZ0_T40_Reinstate_next_b(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G_vNewMember(CK_NEXT);
  G.pDeck->vAppend(G.p_next);

  vEndReshape();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vRR_Convert_All_TblColOrd_to_StmColOrd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this](Card* pCard)
  {
    if (C_YES == T_iCard_ConvertableTo_StmColOdn(pCard)) {
      if (G_bLocateCard(CK_STREAM,CK_TBLCOLODN,pCard->szValue(),C_UNKNOWN)) {
        G.p_stmColOdn = G.p_SubLocatedParent->pSub_stmColOdn();
        M_vCard_Set_Value(pCard,G.p_stmColOdn->szValue());
      }
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.pDeck->bApplyRecursive(CK_NONE, bLambdaFn);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vRR_Convert_All_StmColOrd_to_TblColOrd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this](Card* pCard)
  {
    if (C_YES == T_iCard_ConvertableTo_TblColOdn(pCard)) {
      if (G_bLocateCard(CK_STREAM,CK_STMCOLODN,pCard->szValue(),C_UNKNOWN)) {
        G.p_tblColOdn = G.p_SubLocatedParent->pSub_tblColOdn();
        M_vCard_Set_Value(pCard,G.p_tblColOdn->szValue());
      }
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.pDeck->bApplyRecursive(CK_NONE, bLambdaFn);

  vEndReshape();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    vSeries_35X_Flatten_From */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S35X_T10a_Copy_From_SubQuery(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  m_pCard_yanked = E_pSubset_ChildrenBetween(G.pDeck, 0);


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S35X_T10b_Push_sqB_Before_mqA(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  Card* p_card = G.pDeck->pGetChild(1);
  Card_Comment* p_comment = new Card_Comment("main-query A"_Stz);
  p_card->vAppend(p_comment);

  G_vNewMember(CK_TWIST,"<-b | a->"_Stz);
  G_vNewMember(CK_NOTE,"- delete call"_Stz);
  G.p_twist->vAppend(G.p_note);
  G_vNewMember(CK_NOTE,"- delete twist"_Stz);
  G.p_twist->vAppend(G.p_note);

  //G.pDeck->vInsertAfter(G.p_twist,G.p_call);
  G.pDeck->vPush(G.p_twist);


  G_vNewMember(CK_FOLD);
  G_vNewMember(CK_MANNER,"<-b | a->"_Stz);
  G_vAppendChild(CK_FOLD,CK_MANNER);
  G_vNewMember(CK_CLAUSE,"from"_Stz);
  G_vAppendChild(CK_FOLD,CK_CLAUSE);

  G.pDeck->vInsertAfter(G.p_fold,G.p_twist);


  //G.pDeck->vInsertAfter(m_pCard_yanked,G.p_call);
  G.pDeck->vPush(m_pCard_yanked);
  G.pDeck->vReplaceChildWithItsSubs(m_pCard_yanked);

  DELETE_CARD(m_pCard_yanked);

  //p_card = G.pDeck->pGetChild(1);
  p_card = G.pDeck->pGetChild(0);
  p_comment = new Card_Comment("sub-query B"_Stz);
  p_card->vAppend(p_comment);


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S35X_T30_Adjust_From_SubQuery(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   vSeries_36X_Flatten_Where */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S36X_T104_Mark_pmzWhere_In_sqB(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfMarkPredBounds = [this](Card* pCard) {
    Card_CmpPred* p_cmpPred = _pCard_CmpPred(pCard);
    M_vCmpPred_MarkBound(p_cmpPred);
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_filter->bApplyRecursive(CK_CMPPRED, bfMarkPredBounds);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S36X_T105_Pullup_pmzWhere_Into_mqA(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S36X_T10a_Copy_From_SubQuery(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  m_pCard_yanked = E_pSubset_ChildrenBetween(G.pDeck, 0);


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S36X_T10b_Push_sqB_Before_mqA(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  Card* p_card = G.pDeck->pGetChild(0);
  Card_Comment* p_comment = new Card_Comment("main-query"_Stz);
  p_card->vAppend(p_comment);

  G_vNewMember(CK_TWIST,"<-b | a->"_Stz);

  G_vNewMember(CK_NOTE,"- xfer whereB"_Stz);
  G.p_twist->vAppend(G.p_note);
  G_vNewMember(CK_NOTE,"- paramListB"_Stz);
  G.p_twist->vAppend(G.p_note);
  G_vNewMember(CK_NOTE,"- tstPredA"_Stz);
  G.p_twist->vAppend(G.p_note);
  G_vNewMember(CK_NOTE,"- adj stmOdn"_Stz);
  G.p_twist->vAppend(G.p_note);
  G_vNewMember(CK_NOTE,"- adj stmOdn"_Stz);
  G.p_twist->vAppend(G.p_note);
  G_vNewMember(CK_NOTE,"- cmpPred term"_Stz);
  G.p_twist->vAppend(G.p_note);
  G_vNewMember(CK_NOTE,"- crossJoin"_Stz);
  G.p_twist->vAppend(G.p_note);

  G.pDeck->vPush(G.p_twist);

  G.pDeck->vPush(m_pCard_yanked);
  G.pDeck->vReplaceChildWithItsSubs(m_pCard_yanked);

  DELETE_CARD(m_pCard_yanked);

  p_card = G.pDeck->pGetChild(1);
  p_comment = new Card_Comment("sub-query"_Stz);
  p_card->vAppend(p_comment);


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S36X_T20a_Get_cmpPred_Of_whereB(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.pDeck->vRemoveChild(G.p_where);
  G.p_boolCalc = G.p_where->pSub_boolCalc();

  // TODO: this could be a complex pred sequence
  G.p_cmpPred = _pCard_CmpPred(G.p_boolCalc->pPop());

  DELETE_CARD(G.p_where);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S36X_T20b_Push_cmpPred_Onto_whereA(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_boolCalc = G.p_where->pSub_boolCalc();

  M_vBoolCalc_PushPred(G.p_boolCalc,G.p_cmpPred);

  G.p_cmpPred->vMark1(MARK_REPLACE);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S36X_T30a_Replace_param_In_whereA(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this](Card* pCard)
  {
    Card_CmpPred* p_cmpPred = _pCard_CmpPred(pCard);

    Card_Term* p_term_param = E_pCmpPred_Term_param(p_cmpPred);

    if (! p_term_param)
      return C_CONTINUE_RECURSION;

    // TODO: this is a hack

    G.p_paramList = _pCard_ParamList(G.pDeck->pGetChild(0));
    G.p_param     = _pCard_Param(G.p_paramList->pGetChild(1));



//    Stz* szParamNm = p_term_param->szValue();
//    if (! G_bResolveSubCard(CK_PARAMLIST,CK_PARAM, szParamNm,G.i_MainCard))
//      return C_CONTINUE_RECURSION;

    p_term_param->vValue(G.p_param->szValue());
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_boolCalc->bApplyRecursive(CK_CMPPRED, bLambdaFn);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S36X_T30b_Delete_param_In_whereB(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  G_vRemoveDeleteMain(CK_PARAMLIST);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S36X_T40_Delete_tstPred_In_whereA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this](Card *pCard) {

    Card_TstPred* p_tstPred = _pCard_TstPred(pCard);

    M_vBoolCalc_RemovePred(G.p_boolCalc,p_tstPred);

    DELETE_CARD(p_tstPred);

    return C_TERMINATE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_boolCalc->bApplyRecursive(CK_TSTPRED, bLambdaFn);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S36X_T50_Adjust_StmOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  i8 iOdn_shift = 0;

  // TODO: G_bEachSub(CK_STREAM)

  while (G_bEachMain()) {

    if (! (G.p_MainCard->bIsCard(CK_TABLE) || G.p_MainCard->bIsCard(CK_PROJECT)))
      continue;

    G.p_stream = G.p_MainCard->pSub_stream();

    if (G.p_MainCard->bIsCard(CK_PROJECT))
      iOdn_shift = 0;

    G.p_stmColList = G.p_stream->pSub_stmColList();
    i8 nStmCols = G.p_stmColList->nChildCards();

    i8 iLoOdn_stream = E_iStream_OdnLo(G.p_stream);

    i8 iLoOdn_adjusted = iOdn_shift + 1;
    i8 iHiOdn_adjusted = iLoOdn_adjusted + nStmCols - 1;

    M_vStream_Adjust_OrdinalRange(G.p_stream,iLoOdn_adjusted,iHiOdn_adjusted);

    iOdn_shift = iHiOdn_adjusted;
  }

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S36X_T60_Adjust_StmColOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this](Card *pCard) {

    G.p_stmCol = _pCard_StmCol(pCard);

    //G.p_tblColOdn = G.p_stmCol->pSub_tblColOdn();
    //char cTblSyn = E_cValue_TableColSyn(G.p_tblColOdn->szValue());
    //M_vCard_Set_TableColOrdinal(G.p_tblColOdn, cTblSyn, i_StmColOdn++);

    G.p_stmColOdn = G.p_stmCol->pSub_stmColOdn();
    M_vCard_Set_StreamColOrdinal(G.p_stmColOdn,G.i_stmColOdn++);
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.i_stmColOdn = 1;
  while (G_bEachMain()) {

    if (! (G.p_MainCard->bIsCard(CK_TABLE) ))
      continue;

    G.p_stream = G.p_MainCard->pSub_stream();

    G.p_stmColList = G.p_stream->pSub_stmColList();

    G.p_stmColList->bApplyRecursive(CK_STMCOL,bLambdaFn);
  }


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S36X_T70_Replace_term_In_cmpPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this](Card *pCard) {

    if (MARK_REPLACE != pCard->cMark1())
      return C_CONTINUE_RECURSION;

    pCard->vMark1(' ');

    m_pTerm_0 = _pCard_Term(pCard->pGetChild(0));

    Stz* szNm0 = m_pTerm_0->szValue();
    if (G_bResolveSubCard(CK_STREAM,CK_STMCOLNM,szNm0,G.i_MainCard)) {
      G.p_tblColOdn = m_pCard_resolved->pSub_tblColOdn();
      m_pTerm_0->vValue(G.p_tblColOdn->szValue());
    }

    m_pTerm_1 = _pCard_Term(pCard->pGetChild(1));

    Stz* szNm1 = m_pTerm_1->szValue();
    if (G_bResolveSubCard(CK_STREAM,CK_STMCOLNM,szNm1,G.i_MainCard)) {
      G.p_tblColOdn = m_pCard_resolved->pSub_tblColOdn();
      m_pTerm_1->vValue(G.p_tblColOdn->szValue());
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_boolCalc->bApplyRecursive(CK_CMPPRED,bLambdaFn);


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_37X_Flatten_Having */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S37X_T10a_Copy_sqB(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  m_pCard_yanked = E_pSubset_ChildrenBetween(G.pDeck, 0);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S37X_T10a_Append_sqB_After_mqA(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  Card* p_card = G.pDeck->pGetChild(0);
  Card_Comment* p_comment = new Card_Comment("main-query A"_Stz);
  p_card->vAppend(p_comment);



  G_vNewMember(CK_TWIST,"<-a b->"_Stz);
  G_vNewMemberOf(CK_NOTE,"- link __1"_Stz,CK_TWIST);
  G_vNewMemberOf(CK_NOTE,"- gby __1"_Stz,CK_TWIST);
  G_vNewMemberOf(CK_NOTE,"- agg __1"_Stz,CK_TWIST);
  G_vNewMemberOf(CK_NOTE,"- unproject __1"_Stz,CK_TWIST);
  G_vNewMemberOf(CK_NOTE,"- reverse a b"_Stz,CK_TWIST);
  G_vNewMemberOf(CK_NOTE,"- stmOdn"_Stz,CK_TWIST);
  G_vNewMemberOf(CK_NOTE,"- stmColOdn"_Stz,CK_TWIST);
  G_vNewMemberOf(CK_NOTE,"- delete twist"_Stz,CK_TWIST);

  G.pDeck->vAppend(G.p_twist);

  i8 i_StartOfB = G.pDeck->nChildCards();

  G.pDeck->vAppend(m_pCard_yanked);
  G.pDeck->vReplaceChildWithItsSubs(m_pCard_yanked);

  DELETE_CARD(m_pCard_yanked);

  p_card = G.pDeck->pGetChild(i_StartOfB);
  p_comment = new Card_Comment("sub-query B"_Stz);
  p_card->vAppend(p_comment);


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S37X_T20_Find_stmCol_Link(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_project->vDump();

  G_vNewMemberOf(CK_COMMENT,"__1 is sub-query column"_Stz,CK_PROJECT);

  G.p_stmColList = G.p_project->pSub_stmColList();
  if (G.p_stmColList) {
    G.p_stmCol = G.p_stmColList->pSub_stmCol();
    if (G.p_stmCol) {
      G.p_stmCol->vMark1(MARK_LINK);
      G.p_stmColNm = G.p_stmCol->pSub_stmColNm();
      G.p_type = G.p_stmCol->pSub_type();
    }
  }

  G.p_expr = G.p_project->pSub_expr();
  G.p_rpnCalc = G.p_expr->pSub_rpnCalc();

  G_vNewMember(CK_AS,"__1"_Stz);
  G.p_expr->vInsertBefore(G.p_as,G.p_rpnCalc);


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S37X_T30_Link_sqCall_In_having(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  // TODO: this is a hack

  CardIter xCI(G.p_rpnCalc);
  while ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
    Stz* szTerm = G.p_term->szValue();
    if (G_bIsBoard(szTerm)) {
      G.p_term->vValue("__1"_Stz); // need to discover the stmColNm
      G.p_term->vMark1(MARK_LINK);
    }
  }

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S37X_T35_Bump_gbyElist_rpnCalc_In_sqB(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  CardIter xCI(G.p_gbyElist);
  while ((G.p_expr = _pCard_Expr(xCI.pEachChild()))) {

    G.p_rpnCalc = G.p_expr->pSub_rpnCalc();

    CardIter xCI2(G.p_rpnCalc);
    while ((G.p_term = _pCard_Term(xCI2.pEachChild()))) {
      Stz *szTerm = G.p_term->szValue();
      if (!T_bValue_IsStreamColOrdinal(szTerm))
        continue;
      M_vTerm_Bump_StreamColOrdinal(G.p_term);
      G.p_term->vMark1(MARK_CHANGED);
    }
  }

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S37X_T40Add_sqCol_To_AggGbyElist(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_gbyElist = G.p_gbyAgg->pSub_gbyElist();

  Card_Type* p_type = G.p_type;

  G_vNewMember(CK_EXPR,""_Stz);
  G.p_gbyElist->vPush(G.p_expr);

  G_vNewMemberOf(CK_TYPE,      p_type->szValue(),       CK_EXPR);
  //G_vNewMemberOf(CK_AS,        G.p_stmColNm->szValue(), CK_EXPR);
  G_vNewMemberOf(CK_RPNCALC,   ""_Stz,                  CK_EXPR);
  G_vNewMemberOf(CK_TERM,      "$1"_Stz,                CK_RPNCALC);
  G_vNewMemberOf(CK_DIRECTION, "ASC"_Stz,               CK_EXPR);

  G.p_type = p_type;

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S37X_T45_Bump_aggElist_rpnCalc_In_sqB(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  CardIter xCI(G.p_aggElist);
  while ((G.p_expr = _pCard_Expr(xCI.pEachChild()))) {

    G.p_rpnCalc = G.p_expr->pSub_rpnCalc();

    CardIter xCI2(G.p_rpnCalc);
    while ((G.p_term = _pCard_Term(xCI2.pEachChild()))) {
      Stz *szTerm = G.p_term->szValue();

      if (T_bValue_IsStreamColOrdinal(szTerm)) {
        M_vTerm_Bump_StreamColOrdinal(G.p_term);
        G.p_term->vMark1(MARK_CHANGED);
        continue;
      }

      if (T_bValue_IsKeyOrdinal(szTerm)) {
        M_vTerm_Bump_KeyOrdinal(G.p_term);
        G.p_term->vMark1(MARK_CHANGED);
        continue;
      }
    }
  }

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S37X_T50_Add_sqCol_To_aggAggElist(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_gbyElist = G.p_gbyAgg->pSub_gbyElist();
  i8 nGbyElistExpr = G.p_gbyElist->nChildCards();

  s32 sK_Term = "$K1"_Stz;

  G.p_aggElist = G.p_gbyAgg->pSub_aggElist();

  Card_Type* p_type = G.p_type;

  //G_vNewMemberOf(CK_EXPR,""_Stz,CK_AGGELIST);
  G_vNewMember(CK_EXPR,""_Stz);
  G.p_aggElist->vPush(G.p_expr);

  G_vNewMemberOf(CK_TYPE,      p_type->szValue(),       CK_EXPR);
  G_vNewMemberOf(CK_RPNCALC,   ""_Stz,                  CK_EXPR);
  G_vNewMemberOf(CK_TERM,      sK_Term.szStz(),         CK_RPNCALC);
  G_vNewMemberOf(CK_AS,        G.p_stmColNm->szValue(), CK_EXPR);

  G.p_type = p_type;

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S37X_T55_Bump_having_rpnCalc_In_sqB(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  CardIter xCI(G.p_rpnCalc);
  while ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
    Stz *szTerm = G.p_term->szValue();

    if (T_bValue_IsStreamColOrdinal(szTerm)) {
      M_vTerm_Bump_StreamColOrdinal(G.p_term);
      G.p_term->vMark1(MARK_CHANGED);
      continue;
    }

//    if (T_bValue_IsKeyOrdinal(szTerm)) {
//      M_vTerm_Bump_KeyOrdinal(G.p_term);
//      G.p_term->vMark1(MARK_CHANGED);
//      continue;
//    }
  }

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S37X_T60_Unproject_sqCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  if (! G_bResolveCardWithSub(CK_GBYAGG, CK_AGGELIST, G.i_MainCard))
    return;

  G_vNewMemberBefore(CK_PROJECT,""_Stz,CK_TWIST);

  G_vNewMemberOf(CK_LEVEL,""_Stz,CK_PROJECT);
  G.p_level->Y_vSet(P_Level, (i8)AL_ORIGIN);

  G_vNewMemberOf(CK_PROJELIST,""_Stz,CK_PROJECT);

  i8 i_Col = 0;
  i8 nChildren_aggElist = G.p_aggElist->nChildCards();
  for (i8 i_expr_aggElist = 0;
       i_expr_aggElist < nChildren_aggElist;
       i_expr_aggElist++) {

    i_Col++;

    G.p_expr = _pCard_Expr(G.p_aggElist->pGetChild(i_expr_aggElist));
    G.p_as = G.p_expr->pSub_as();

    if (T_bValue_IsSqyColOrdinal(G.p_as->szValue()))
      continue;

//    G.p_rpnCalc = G.p_expr->pSub_rpnCalc();
//
//    bool bUnproject = false;
//    CardIter xCI2(G.p_rpnCalc);
//    while ((G.p_term = _pCard_Term(xCI2.pEachChild()))) {
//
//      if (T_bValue_IsSqyColOrdinal(G.p_term->szValue())) {
//        bUnproject = true;
//        break;
//      }
//    }
//
//    if (bUnproject)
//      continue;

    Card_Expr* p_expr_simple = D_pProject_Simple_expr_Of(G.p_expr,i_Col);

    G_vAppendChild(CK_PROJELIST,p_expr_simple);
  }


  G.p_stream = C_pProduce_stream_ofProject(G.p_projElist);

  G.p_project->vAppend(G.p_stream);

  G_vNewMember(CK_COMMENT,"unproject of __1"_Stz);
  G.p_project->vAppend(G.p_comment);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S37X_T70_Exhange_sqB_With_mqA(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.pDeck->vRemoveChild(G.p_twist);
  G.pDeck->vPush(G.p_twist);

  G.p_subSet = G.pDeck->pYankSubCards(G.i_MainCard+1,C_UNKNOWN);

  G.pDeck->vPush(G.p_subSet);
  G.pDeck->vReplaceChildWithItsSubs(G.p_subSet);

  RASSERT(0 == G.p_subSet->nChildCards());
  DELETE_CARD(G.p_subSet);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S37X_T80_AdjustStmOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  //TODO: factor with vH36_T60_Adjust_StmColOdn
  i8 iOdn_shift = 0;

  while (G_bEachMain()) {

    if (! (G.p_MainCard->bIsCard(CK_TABLE) || G.p_MainCard->bIsCard(CK_PROJECT)))
      continue;

    G.p_stream = G.p_MainCard->pSub_stream();

    if (G.p_MainCard->bIsCard(CK_PROJECT))
      iOdn_shift = 0;

    G.p_stmColList = G.p_stream->pSub_stmColList();
    i8 nStmCols = G.p_stmColList->nChildCards();

    i8 iLoOdn_stream = E_iStream_OdnLo(G.p_stream);

    i8 iLoOdn_adjusted = iOdn_shift + 1;
    i8 iHiOdn_adjusted = iLoOdn_adjusted + nStmCols - 1;

    M_vStream_Adjust_OrdinalRange(G.p_stream,iLoOdn_adjusted,iHiOdn_adjusted);

    iOdn_shift = iHiOdn_adjusted;
  }


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S37X_T90_Adjust_StmColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this](Card *pCard) {

    G.p_stmCol = _pCard_StmCol(pCard);

    //G.p_tblColOdn = G.p_stmCol->pSub_tblColOdn();
    //char cTblSyn = E_cValue_TableColSyn(G.p_tblColOdn->szValue());
    //M_vCard_Set_TableColOrdinal(G.p_tblColOdn, cTblSyn, i_StmColOdn++);

    G.p_stmColOdn = G.p_stmCol->pSub_stmColOdn();
    M_vCard_Set_StreamColOrdinal(G.p_stmColOdn,G.i_stmColOdn++);
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.i_stmColOdn = 1;
  while (G_bEachMain()) {

    if (! (G.p_MainCard->bIsCard(CK_TABLE) ))
      continue;

    G.p_stream = G.p_MainCard->pSub_stream();

    G.p_stmColList = G.p_stream->pSub_stmColList();

    G.p_stmColList->bApplyRecursive(CK_STMCOL,bLambdaFn);
  }



  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S37X_TA0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S37X_TB0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  vEndReshape();
}



