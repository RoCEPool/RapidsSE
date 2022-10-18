/*============================================================================*/
/* TransformIter.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "BoardIter.hpp"


namespace STORE
{
  struct _TransformIterMembers
  {
    BoardIter pM;

    Card * p_PriorToMain;
    eCK    e_PriorToMain;
    Card * p_MainCard;
    eCK    e_MainCard;
    Card * p_SubOfMain;
    eCK    e_SubOfMain;
    Card * p_Sub2OfMain;
    eCK    e_Sub2OfMain;
    Card * p_SubsequentToMain;
    eCK    e_SubsequentToMain;

    Card * p_SubMember;
    eCK    e_SubMember;

    Card_AbsJoin *          p_absJoin;
    Card_Aggreg *           p_aggreg;
    Card_BoolCalc *         p_boolCalc;
    Card_Calc *             p_calc;
    Card_CmpPred *          p_cmpPred;
    Card_Combined *         p_combined;
    Card_Comment *          p_comment;
    Card_CoPtnJoin *        p_coPtnJoin;
    Card_CoPtnJoinList *    p_coPtnJoinList;
    Card_CoPtnPred *        p_coPtnPred;
    Card_CoPtnPredList *    p_coPtnPredList;
    Card_CrossPtnJoin *     p_crossPtnJoin;
    Card_CrossPtnJoinList * p_crossPtnJoinList;
    Card_Expr *             p_expr;
    Card_Filter *           p_filter;
    Card_GbyElist *         p_gbyElist;
    Card_IdxCol *           p_idxCol;
    Card_IdxColList *       p_idxColList;
    Card_IndexJoin *        p_indexJoin;
    Card_IndexList *        p_indexList;
    Card_IndexPtr *         p_indexPtr;
    Card_IndexScan *        p_indexScan;
    Card_JoinOrder *        p_joinOrder;
    Card_JoinOrderSeq *     p_joinOrderSeq;
    Card_JoinPred *         p_joinPred;
    Card_JoinPredList *     p_joinPredList;
    Card_KeyExists *        p_keyExists;
    Card_LBound *           p_lBound;
    Card_Param *            p_param;
    Card_ParamList *        p_paramList;
    Card_Permutation *      p_permutation;
    Card_Permutation *      p_permutation_low;
    Card_Project *          p_project;
    Card_PtndJoin *         p_ptndJoin;
    Card_PtndJoinSet *      p_ptndJoinSet;
    Card_SbyElist *         p_sbyElist;
    Card_Scan *             p_scan;
    Card_ScanPred *         p_scanPred;
    Card_Send *             p_send;
    Card_Sort *             p_sort;
    Card_Stage *            p_stage;
    Card_StageTopo *        p_stageTopo;
    Card_Stream *           p_stream;
    Card_StreamCol *        p_streamCol;
    Card_StreamColList *    p_streamColList;
    Card_SubSet *           p_subSet;
    Card_Table *            p_table;
    Card_TableList *        p_tableList;
    Card_TablePtn *         p_tablePtn;
    Card_TablePtr *         p_tablePtr;
    Card_Term *             p_term;
    Card_UBound *           p_uBound;
    Card_Where *            p_where;
  } ;

  class TransformIter : public Object
                      , public ListItem_t<TransformIter>
  {
  public:
//    struct {
//      BoardIter pM;
//
//      Card * p_PriorToMain;
//      eCK    e_PriorToMain;
//      Card * p_MainCard;
//      eCK    e_MainCard;
//      Card * p_SubOfMain;
//      eCK    e_SubOfMain;
//      Card * p_Sub2OfMain;
//      eCK    e_Sub2OfMain;
//      Card * p_SubsequentToMain;
//      eCK    e_SubsequentToMain;
//
//      Card * p_SubMember;
//      eCK    e_SubMember;
//
//      Card_AbsJoin *          p_absJoin;
//      Card_Aggreg *           p_aggreg;
//      Card_BoolCalc *         p_boolCalc;
//      Card_Calc *             p_calc;
//      Card_CmpPred *          p_cmpPred;
//      Card_Combined *         p_combined;
//      Card_Comment *          p_comment;
//      Card_CoPtnJoin *        p_coPtnJoin;
//      Card_CoPtnJoinList *    p_coPtnJoinList;
//      Card_CoPtnPred *        p_coPtnPred;
//      Card_CoPtnPredList *    p_coPtnPredList;
//      Card_CrossPtnJoin *     p_crossPtnJoin;
//      Card_CrossPtnJoinList * p_crossPtnJoinList;
//      Card_Expr *             p_expr;
//      Card_Filter *           p_filter;
//      Card_GbyElist *         p_gbyElist;
//      Card_IdxCol *           p_idxCol;
//      Card_IdxColList *       p_idxColList;
//      Card_IndexJoin *        p_indexJoin;
//      Card_IndexList *        p_indexList;
//      Card_IndexPtr *         p_indexPtr;
//      Card_IndexScan *        p_indexScan;
//      Card_JoinOrder *        p_joinOrder;
//      Card_JoinOrderSeq *     p_joinOrderSeq;
//      Card_JoinPred *         p_joinPred;
//      Card_JoinPredList *     p_joinPredList;
//      Card_KeyExists *        p_keyExists;
//      Card_LBound *           p_lBound;
//      Card_Param *            p_param;
//      Card_ParamList *        p_paramList;
//      Card_Permutation *      p_permutation;
//      Card_Permutation *      p_permutation_low;
//      Card_Project *          p_project;
//      Card_PtndJoin *         p_ptndJoin;
//      Card_PtndJoinSet *      p_ptndJoinSet;
//      Card_SbyElist *         p_sbyElist;
//      Card_Scan *             p_scan;
//      Card_ScanPred *         p_scanPred;
//      Card_Send *             p_send;
//      Card_Sort *             p_sort;
//      Card_Stage *            p_stage;
//      Card_StageTopo *        p_stageTopo;
//      Card_Stream *           p_stream;
//      Card_StreamCol *        p_streamCol;
//      Card_StreamColList *    p_streamColList;
//      Card_SubSet *           p_subSet;
//      Card_Table *            p_table;
//      Card_TableList *        p_tableList;
//      Card_TablePtn *         p_tablePtn;
//      Card_TablePtr *         p_tablePtr;
//      Card_Term *             p_term;
//      Card_UBound *           p_uBound;
//      Card_Where *            p_where;
//    } M;
    _TransformIterMembers M;

    // must be last
    Footer m_xFooter;


  public:
    void* operator new(size_t);
    void  operator delete(void*);

    TransformIter(void);
    TransformIter(eCN eClass);
   ~TransformIter(void) noexcept(false);

    void vCopyFrom(_TransformIterMembers* aTransformIterMembers);

    _TransformIterMembers* aTransformIterMembers(void);

    Footer* aFooter(void);
  };


  TransformIter* _pTransformIter(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
