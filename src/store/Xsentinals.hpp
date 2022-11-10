/*============================================================================*/
/* Xsentinals.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "BoardIter.hpp"

#include "Xderive.hpp"

namespace STORE
{
  struct _Xsentinels
  {
    BoardIter xBI;

    Board*      pBoard;
    Card_Deck*  pDeck;

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
    Card * p_SubsequentToMain;
    eCK    e_SubsequentToMain;

    Card * p_SubMember;
    eCK    e_SubMember;

    Stz*   sz_SubQueryNm;

    eAL    e_Level;

    i8     i_stmColOdn;

    Card_AbsJoin *          p_absJoin;
    Card_AggElist *         p_aggElist;
    Card_AggGby *           p_aggGby;
    Card_Aggreg *           p_aggreg;
    Card_BoolCalc *         p_boolCalc;
    Card_Bounds *           p_bounds;
    Card_Call *             p_call;
    Card_Cast *             p_cast;
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
    Card_ExePred *          p_exePred;
    Card_Expr *             p_expr;
    Card_Filter *           p_filter;
    Card_GbyElist *         p_gbyElist;
    Card_Having *           p_having;
    Card_IdxCol *           p_idxCol;
    Card_IdxColList *       p_idxColList;
    Card_IdxColNm *         p_idxColNm;//
    Card_Index *            p_index;
    Card_IndexJoin *        p_indexJoin;
    Card_IndexList *        p_indexList;
    Card_IndexNm *          p_indexNm;
    Card_IndexScan *        p_indexScan;
    Card_JoinOrder *        p_joinOrder;
    Card_JoinOrderSeq *     p_joinOrderSeq;
    Card_JoinPredList *     p_joinPredList;
    Card_KeyExists *        p_keyExists;
    Card_LBound *           p_lBound;
    Card_Level *            p_level;
    Card_MaybeIndexJoin *   p_maybeIndexJoin;
    Card_Next *             p_next;
    Card_Param *            p_param;
    Card_ParamList *        p_paramList;
    Card_Partition *        p_partition;
    Card_Permutation *      p_permutation;
    Card_Permutation *      p_permutation_low;
    Card_Project *          p_project;
    Card_ProjElist *        p_projElist;
    Card_PtnCol *           p_ptnCol;
    Card_PtnColList *       p_ptnColList;
    Card_PtnColNm *         p_ptnColNm;
    Card_PtndJoin *         p_ptndJoin;
    Card_PtndJoinSet *      p_ptndJoinSet;
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
    Card_Topology *         p_topology;
    Card_TstPred *          p_tstPred;
    Card_Twist *            p_twist;
    Card_UBound *           p_uBound;
    Card_Where *            p_where;
  } ;

  class Xsentinals : public Xderive
                   , public ListItem_t<Xsentinals>
  {
  public:
    _Xsentinels G;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Xsentinals(void);
    Xsentinals(eCN eClass);
   ~Xsentinals(void) noexcept(false);

    void vCopyFrom(_Xsentinels* aTransformIterMembers);

    _Xsentinels* aXsentinels(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
