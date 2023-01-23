/*============================================================================*/
/* Board.hpp -- Board                                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eBK.h"
#include "eBT.h"
#include "eBR.h"
#include "eBF.h"
#include "eBC.h"

#include "eQS.h"

#include "Card.hpp"
#include "CardCtl.hpp"

#include "Card_AbsJoin.hpp"
#include "Card_Agg.hpp"
#include "Card_Aggreg.hpp"
#include "Card_Aggregates.hpp"
#include "Card_AndPred.hpp"
#include "Card_As.hpp"

#include "Card_BolPred.hpp"
#include "Card_BoolCalc.hpp"
#include "Card_Bounds.hpp"

#include "Card_CalcOp.hpp"
#include "Card_Call.hpp"
#include "Card_CandIndex.hpp"
#include "Card_Cast.hpp"
#include "Card_Clause.hpp"
#include "Card_CmpPred.hpp"
#include "Card_ColNm.hpp"
#include "Card_Combined.hpp"
#include "Card_Comment.hpp"
#include "Card_Constraint.hpp"
#include "Card_CoPtnJoin.hpp"
#include "Card_CoPtnJoinList.hpp"
#include "Card_CoPtnPred.hpp"
#include "Card_CoPtnPredList.hpp"
#include "Card_CrossJoin.hpp"
#include "Card_CrossPtnJoin.hpp"
#include "Card_CrossPtnJoinList.hpp"
#include "Card_CrossPtnPred.hpp"

#include "Card_Debug.hpp"
#include "Card_Deck.hpp"
#include "Card_Direction.hpp"

#include "Card_EBound.hpp"
#include "Card_Exists.hpp"
#include "Card_Expr.hpp"
#include "Card_ExprList.hpp"

#include "Card_Filter.hpp"
#include "Card_Fold.hpp"
#include "Card_FoldElist.hpp"
#include "Card_From.hpp"

#include "Card_GbyAgg.hpp"
#include "Card_Genesis.hpp"
#include "Card_Group.hpp"

#include "Card_Hash.hpp"
#include "Card_Having.hpp"

#include "Card_IdxCol.hpp"
#include "Card_IdxColList.hpp"
#include "Card_IdxColNm.hpp"
#include "Card_Index.hpp"
#include "Card_IndexJoin.hpp"
#include "Card_IndexList.hpp"
#include "Card_IndexNm.hpp"
#include "Card_IndexScan.hpp"

#include "Card_JoinOrder.hpp"
#include "Card_JoinOrdering.hpp"
#include "Card_JoinPerm.hpp"
#include "Card_JoinPermList.hpp"
#include "Card_JoinPredList.hpp"

#include "Card_Key.hpp"
#include "Card_KeyExists.hpp"
#include "Card_KeyNotExists.hpp"

#include "Card_LBound.hpp"
#include "Card_Level.hpp"
#include "Card_Limit.hpp"
#include "Card_LinksTo.hpp"

#include "Card_Manner.hpp"

#include "Card_NCorrCols.hpp"
#include "Card_Next.hpp"
#include "Card_Note.hpp"
#include "Card_NotPred.hpp"
#include "Card_NParmCols.hpp"

#include "Card_ObyElist.hpp"
#include "Card_On.hpp"
#include "Card_OP.hpp"
#include "Card_OrdAgg.hpp"
#include "Card_Order.hpp"
#include "Card_OrdElmOdn.hpp"
#include "Card_OrPred.hpp"

#include "Card_Param.hpp"
#include "Card_ParamList.hpp"
#include "Card_Partition.hpp"
#include "Card_Permutation.hpp"
#include "Card_PmzWhere.hpp"
#include "Card_Project.hpp"
#include "Card_Projection.hpp"
#include "Card_PtnCol.hpp"
#include "Card_PtnColList.hpp"
#include "Card_PtnColNm.hpp"
#include "Card_PtndJoin.hpp"
#include "Card_PtndJoinSet.hpp"

#include "Card_RBound.hpp"
#include "Card_Reason.hpp"
#include "Card_RelnOp.hpp"
#include "Card_RoPred.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_RpnTerms.hpp"

#include "Card_Scan.hpp"
#include "Card_Send.hpp"
#include "Card_Size.hpp"
#include "Card_Sort.hpp"
#include "Card_Stage.hpp"
#include "Card_StmColNm.hpp"
#include "Card_StmColOdn.hpp"
#include "Card_Stream.hpp"
#include "Card_SubSet.hpp"

#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_TableNm.hpp"
#include "Card_TablePtn.hpp"
#include "Card_TblColOdn.hpp"
#include "Card_TblSyn.hpp"
#include "Card_Term.hpp"
#include "Card_Top.hpp"
#include "Card_Topology.hpp"
#include "Card_Twist.hpp"
#include "Card_Type.hpp"

#include "Card_UBound.hpp"
#include "Card_Unique.hpp"

#include "Card_Where.hpp"

#include "Card_XBound.hpp"

#include "VertexSpec.hpp"
#include "DeckSpec.hpp"
#include "StageSpec.hpp"
#include "OpSpec.hpp"

#include "SubQuery.hpp"
#include "Scope.hpp"


#include "Card_Av_AggP.hpp"
#include "Card_Av_AggPN.hpp"
#include "Card_Av_AggPNO.hpp"
#include "Card_Av_AggregP.hpp"
#include "Card_Av_AggregPN.hpp"
#include "Card_Av_AggregPNO.hpp"
#include "Card_Av_Filter.hpp"
#include "Card_Av_FilterProject.hpp"
#include "Card_Av_GbyAggP.hpp"
#include "Card_Av_GbyAggPN.hpp"
#include "Card_Av_GbyPNO.hpp"
#include "Card_Av_JoinSeries.hpp"
#include "Card_Av_Limit.hpp"
#include "Card_Av_Project.hpp"
#include "Card_Av_ScanJoinSeries.hpp"
#include "Card_Av_ScanOnly.hpp"
#include "Card_Av_Sort.hpp"

namespace STORE
{
  class Bench;
  class Dissector;
  class Drafter;
  class Optimiser;
  class CardIter;
  class Weaver;

  class Board : public Object
  {
  protected:
    i8          m_i_Board;
    s32         m_sBoardNm;
    eBK         m_eBoardKind;  // costEst, partition, permute, subpipe
    eBT         m_eBoardType;  // original, view
    eBR         m_eBoardRole;  // driver, follower
    eBF         m_eBoardForm;  // fanin, linear
    eBC         m_eBoardCall;  // exists, from, in

    eQS         m_eSubQuery;

    Card_Deck*  m_pCard_Deck;

    Optimiser*  m_pOptimiser;
    Bench*      m_pBench;

    s32         m_sXhand;
    s256        m_sXform;
    i8          m_iXform;
    s256        m_sXdesc;

    bool        m_bMayHaveStmColOdn;

    SubQuery*   m_pSubQuery;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Board(Stz* szBoardNm, eBK eBoardKind, i8 i_Board, eBT eBoardType, Bench* pBench);
   ~Board(void) noexcept(false);

    static void vAdd_Partitioning(CardIter* pCI);

    static void vCorrect_having_BaseOn_foldExpr(CardIter* pCI);

    static void vDerive_foldElist_From_project_stream(CardIter* pCI);
    static void vDerive_group_order_For_gbyAgg(CardIter* pCI);
    static void vDerive_stream_order_For_gbyAgg(CardIter* pCI);
    static void vDerive_stream_order_For_indexJoin(CardIter* pCI);
    static void vDerive_stream_order_For_indexScan(CardIter* pCI);
    static void vDerive_stream_order_For_project(CardIter* pCI);
    static void vDerive_stream_order_For_sort(CardIter* pCI);
    static void vDerive_order(CardIter* pCI);
    static void vDerive_foldExpr(CardIter* pCI);

    static void vEnrich_sq_aggreg_ToBe_gbyAgg_Gby_Correlation_Column(CardIter* pCI);
    static void vEnhance_sq_project_To_Include_Correlation_Column(CardIter* pCI);

    static bool bFindAndMarkSubQryCall(CardIter* pCI);
    static void vFoldIn_foldExpr(CardIter* pCI);

    static void vIdentify_Correlation_Column_In_cmpPred_Of_sq_boolCalc(CardIter* pCI);

    static void vMorph_Table_To_EEop(CardIter* pCI);
    static void vMove_corrPred_To_mq_table_As_bounds(CardIter* pCI, Card_BoolCalc* p_boolCalc);

    static void vProjection_Analysis(CardIter* pCI);

    static void vRelabelExpressions(CardIter* pCI);
    static void vRenumberStmOdn(CardIter* pCI);
    static void vResolve_SubqueryCall_In_mq_boolCalc_Into_cmpPred(CardIter* pCI, Card_BoolCalc* p_boolCalc);

    static i8 iStage_Partitioning(CardIter* pCI,
                                  Card* p_main_prior,
                                  Card_Combined* p_combined_prior,
                                  Card_Next* p_next_prior,
                                  Card* p_main_curr);
    static void vUnproject_foldExpr_After_having(CardIter* pCI);

    // vSeries_26_SubQuery_Analysis
    static void vS26_T10d_Main_Query_Analysis_Of_where_rpnCalc(CardIter* pCI);
    static void vS26_T10e_Main_Query_Analysis_Of_having_rpnCalc(CardIter* pCI);


    // vSeries_36_Bound_Analysis
    static void vS36_T30_Delete_Empty_filter(CardIter* pCI);

    // vSeries_75_EE_operators
    static void vS75_T30_Correct_stmColOdn_Of_keyExists_stream(CardIter* pCI);
    static void vS75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(CardIter* pCI);

    // vSeries_A0_Make_Weavable
    static void vSA0_T10_Individuate_indexJoin_filter_b(CardIter* pCI);
    static void vSA0_T10c_Individuate_crossJoin_filter(CardIter* pCI);

    // vSeries_Z0_Restate_Pipeline
    static void vReinstate_stages(CardIter* pCI);


    i8  i_Board(void);
    eBC  eBoardCall(void);
    void vBoardCall(eBC eBoardCall);
    eBF  eBoardForm(void);
    void vBoardForm(eBF eBoardForm);
    eBK eBoardKind(void);
    Stz* szBoardNm(void);
    void vBoardNm(Stz* szBoardNm);
    eBR  eBoardRole(void);
    void vBoardRole(eBR eBoardRole);

    CardCtl* pCardCtl(void);
    void vClearMarks(void);

    Card_Deck* pDeck(void);

    bool bMayHaveStmColOdn(void);
    void vMayHaveStmColOdn(bool bMayHaveStmColOdn);

    void vPinBanners(Ribbon* pRibbon);
    void vPinCards(Ribbon* pRibbon);
    void vPinTitle(Ribbon* pRibbon,
                   i8 iTransformNo,
                   Stz* szExplain=nullptr);
    void vPrintBoard(Stz* szDescription=nullptr);

    eQS  eSubQuery(void);
    void vSubQuery(eQS  eSubQuery);

    SubQuery* pSubQuery(void);

    Stz* szXdesc(void);
    void vXdesc(Stz* szDescription);
    i8   iXform(void);
    void vXform(i8   iXform);
    Stz* szXform(void);
    void vXform(Stz* szTransformation);
    Stz* szXhand(void);
    void vXhand(const char* asXhand);

  private:
    Footer* aFooter(void);
  };

  Board* _pBoard(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
