/*============================================================================*/
/* CardIterMembers.hpp --                                                     */
/*============================================================================*/
#pragma once

#include "Board.hpp"


namespace STORE
{
  class CardIterMembers : public Object
  {
  public:
    Card*  m_pCard;
    i8     m_i_Child;
    Card*  m_pChild;

    bool   m_bRecursed;
    bool   m_bDontDescend;

    bool   m_bActive;

    // - - - - - - - - - - - - -

    Board*      pBoard;
    Card_Deck*  pDeck;
    SubQuery*   pSubQuery;

    Card * p_Located;
    eCK    e_Located;
    Card * p_PriorToMain;
    eCK    e_PriorToMain;
    Card * p_MainCard;
    eCK    e_MainCard;
    i8     i_MainCard;
    Card * p_SubLocated;
    eCK    e_SubLocated;
    Card * p_SubLocatedParent;
    eCK    e_SubLocatedParent;
    Card * p_SubOfMain;
    eCK    e_SubOfMain;
    Card * p_Sub2OfMain;
    eCK    e_Sub2OfMain;
    Card * p_Sub3OfMain;
    eCK    e_Sub3OfMain;
    Card * p_SubsequentToMain;
    eCK    e_SubsequentToMain;

    Card * p_SubMember;
    eCK    e_SubMember;

    Stz*   sz_SubQueryNm;

    eAL    e_Level;

    i8     i_stmColOdn;
    i8     i_FoldCard;

    Card_PRED * p_PRED;

    // - - - - - - - - - - - - - - - - - -

    Card_AbsJoin *          p_absJoin;
    Card_Agg *              p_agg;
    Card_Aggreg *           p_aggreg;
    Card_As *               p_as;
    Card_BolPred *          p_bolPred;
    Card_BoolCalc *         p_boolCalc;
    Card_Bound *            p_bound;
    Card_Bounds *           p_bounds;
    Card_CalcOp *           p_calcOp;
    Card_Call *             p_call;
    Card_Cast *             p_cast;
    Card_Clause *           p_clause;
    Card_CmpPred *          p_cmpPred;
    Card_Combined *         p_combined;
    Card_Comment *          p_comment;
    Card_Constraint *       p_constraint;
    Card_CoPtnJoin *        p_coPtnJoin;
    Card_CoPtnJoinList *    p_coPtnJoinList;
    Card_CoPtnPred *        p_coPtnPred;
    Card_CoPtnPredList *    p_coPtnPredList;
    Card_CrossJoin *        p_crossJoin;
    Card_CrossPtnJoin *     p_crossPtnJoin;
    Card_CrossPtnJoinList * p_crossPtnJoinList;
    Card_Direction *        p_direction;
    Card_EBound *           p_eBound;
    Card_Expr *             p_expr;
    Card_ExprList *         p_exprList;
    Card_Filter *           p_filter;
    Card_Fold *             p_fold;
    Card_FoldElist *        p_foldElist;
    Card_From *             p_from;
    Card_GbyAgg *           p_gbyAgg;
    Card_Genesis *          p_genesis;
    Card_Group *            p_group;
    Card_Hash *             p_hash;
    Card_Having *           p_having;
    Card_IdxCol *           p_idxCol;
    Card_IdxColList *       p_idxColList;
    Card_IdxColNm *         p_idxColNm;
    Card_Index *            p_index;
    Card_IndexJoin *        p_indexJoin;
    Card_IndexList *        p_indexList;
    Card_IndexNm *          p_indexNm;
    Card_IndexScan *        p_indexScan;
    Card_JoinOrder *        p_joinOrder;
    Card_JoinOrdering *     p_joinOrdering;
    Card_JoinPredList *     p_joinPredList;
    Card_Key *              p_key;
    Card_KeyExists *        p_keyExists;
    Card_KeyNotExists *     p_keyNotExists;
    Card_LBound *           p_lBound;
    Card_Level *            p_level;
    Card_Limit *            p_limit;
    Card_Manner *           p_manner;
    Card_NCorrCols *        p_nCorrCols;
    Card_Next *             p_next;
    Card_Note *             p_note;
    Card_NotPred *          p_notPred;
    Card_NParmCols *        p_nParmCols;
    Card_On *               p_on;
    Card_OrdAgg *           p_ordAgg;
    Card_OrdElmOdn *        p_ordElmOdn;
    Card_Order *            p_order;
    Card_Param *            p_param;
    Card_ParamList *        p_paramList;
    Card_Partition *        p_partition;
    Card_Permutation *      p_permutation;
    Card_Permutation *      p_permutation_low;
    Card_PmzWhere *         p_pmzWhere;
    Card_Project *          p_project;
    Card_Projection *       p_projection;
    Card_ProjElist *        p_projElist;
    Card_PtnCol *           p_ptnCol;
    Card_PtnColList *       p_ptnColList;
    Card_PtnColNm *         p_ptnColNm;
    Card_PtndJoin *         p_ptndJoin;
    Card_PtndJoinSet *      p_ptndJoinSet;
    Card_Reason *           p_reason;
    Card_RelnOp *           p_relnOp;
    Card_RoPred *           p_roPred;
    Card_RpnCalc *          p_rpnCalc;
    Card_SbyElist *         p_sbyElist;
    Card_Scan *             p_scan;
    Card_Send *             p_send;
    Card_Size *             p_size;
    Card_Sort *             p_sort;
    Card_Stage *            p_stage;
    Card_StmCol *           p_stmCol;
    Card_StmColList *       p_stmColList;
    Card_StmColNm *         p_stmColNm;
    Card_StmColOdn *        p_stmColOdn;
    Card_Stream *           p_stream;
    Card_SubSet *           p_subSet;
    Card_Table *            p_table;
    Card_TableList *        p_tableList;
    Card_TableNm *          p_tableNm;
    Card_TablePtn *         p_tablePtn;
    Card_TblColOdn *        p_tblColOdn;
    Card_Term *             p_term;
    Card_Top *              p_top;
    Card_Topology *         p_topology;
    Card_Twist *            p_twist;
    Card_Type *             p_type;
    Card_UBound *           p_uBound;
    Card_Unique *           p_unique;
    Card_Where *            p_where;
    Card_XBound *           p_xBound;

    // - - - - - - - - - - - - - - - - - -

    Card_Av_AggP *           p_av_aggP;
    Card_Av_AggPN *          p_av_aggPN;
    Card_Av_AggPNO *         p_av_aggPNO;
    Card_Av_AggregP *        p_av_aggregP;
    Card_Av_AggregPN *       p_av_aggregPN;
    Card_Av_AggregPNO *      p_av_aggregPNO;
    Card_Av_Filter *         p_av_filter;
    Card_Av_FilterProject *  p_av_filterProject;
    Card_Av_GbyAggP *        p_av_gbyAggP;
    Card_Av_GbyAggPN *       p_av_gbyAggPN;
    Card_Av_GbyPNO *         p_av_gbyPNO;
    Card_Av_JoinSeries *     p_av_joinSeries;
    Card_Av_Limit *          p_av_limit;
    Card_Av_Project *        p_av_project;
    Card_Av_ScanJoinSeries * p_av_scanJoinSeries;
    Card_Av_ScanOnly *       p_av_scanOnly;
    Card_Av_Sort *           p_av_sort;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CardIterMembers(void);
   ~CardIterMembers(void) noexcept(false);

    void vCopyFrom(CardIterMembers* pXsentinals2);

  private:

    Footer* aFooter(void);
  } ;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
