/*============================================================================*/
/* CardIterMembers.hpp --                                                     */
/*============================================================================*/
#pragma once

#include "Board.hpp"

#include "Kid.hpp"

namespace STORE
{
  class CardIterMembers : public Object
  {
    friend class CardIter;
  public:
    i8          i_Board;
    Board*      pBoard;
    Card_Deck*  pDeck;

  // - - - - - - - - - - - - -
  private:
    Kid xKid;

  // - - - - - - - - - - - - -
  public:
    i8     m_i_RecursionDepth;
    bool   m_bDontDescend;

    // - - - - - - - - - - - - -

    char   md_Coord[256];
    Card * p_MainCard;
    eCK    e_MainCard;
    i8     i_MainCard;
    Card * p_SubOfMain;
    eCK    e_SubOfMain;
    Card * p_Sub2OfMain;
    eCK    e_Sub2OfMain;
    Card * p_Sub3OfMain;
    eCK    e_Sub3OfMain;

    // - - - - - - - - - - - - - - - - - -

    Card_AbsJoin *       p_absJoin;
    Card_Aggreg *        p_aggreg;
    Card_AndConj *       p_andConj;
    Card_As *            p_as;

    Card_BoolCalc *      p_boolCalc;
    Card_Bound *         p_bound;
    Card_Bounds *        p_bounds;

    Card_Call *          p_call;
    Card_Caller *        p_caller;
    Card_CallOp *        p_callOp;
    Card_Cast *          p_cast;
    Card_CmpConj *       p_cmpConj;
    Card_Clause *        p_clause;
    Card_Combined *      p_combined;
    Card_Constraint *    p_constraint;
    Card_CorrCols *      p_corrCols;
    Card_Correlation *   p_correlation;
    Card_CostJoining *   p_costJoining;
    Card_CostNetwork *   p_costNetwork;
    Card_CostStream *    p_costQuery;
    Card_CostUnicast *   p_costUnicast;
    Card_Count *         p_count;
    Card_CrossJoin *     p_crossJoin;

    Card_Direction *     p_direction;

    Card_Expr *          p_expr;

    Card_Filter *        p_filter;
    Card_FiltPred *      p_filtPred;
    Card_FiltPredList *  p_filtPredList;
    Card_Flat *          p_flat;
    Card_Fold *          p_fold;
    Card_From *          p_from;

    Card_GbyAgg *        p_gbyAgg;

    Card_Hash *          p_hash;
    Card_Hashing *       p_hashing;
    Card_Having *        p_having;
    Card_HshCol *        p_hshCol;
    Card_HshColList *    p_hshColList;

    Card_IdxCol *        p_idxCol;
    Card_IdxColList *    p_idxColList;
    Card_Index *         p_index;
    Card_IndexJoin *     p_indexJoin;
    Card_IndexList *     p_indexList;
    Card_IndexNm *       p_indexNm;
    Card_IndexScan *     p_indexScan;

    Card_JoinCast *      p_joinCast;
    Card_JoinEstimate *  p_joinEstimate;
    Card_JoinOrder *     p_joinOrder;
    Card_JoinOrdering *  p_joinOrdering;
    Card_JoinPredList *  p_joinPredList;

    Card_Key *           p_key;
    Card_KeyCells *      p_keyCells;
    Card_KeyExists *     p_keyExists;
    Card_KeyNotExists *  p_keyNotExists;

    Card_Level *         p_level;
    Card_Limit *         p_limit;

    Card_Map *           p_map;
    Card_Mapping *       p_mapping;

    Card_Needed *        p_needed;
    Card_Note *          p_note;
    Card_NotConj *       p_notConj;

    Card_On *            p_on;
    Card_OrdAgg *        p_ordAgg;
    Card_OrConj *        p_orConj;
    Card_OuterColNm *    p_outerColNm;
    Card_OuterJoin *     p_outerJoin;

    Card_PayloadCells *  p_payloadCells;
    Card_Project *       p_project;
    Card_Projection *    p_projection;

    Card_RelnOp *        p_relnOp;
    Card_RowCount *      p_rowCount;
    Card_RowSize *       p_rowSize;
    Card_RpnCalc *       p_rpnCalc;

    Card_Scan *          p_scan;
    Card_Section *       p_section;
    Card_Send *          p_send;
    Card_Sort *          p_sort;
    Card_Spare *         p_spare;
    Card_SqyCalls *      p_sqyCalls;
    Card_SqyNo *         p_sqyNo;
    Card_Stage *         p_stage;
    Card_Stream *        p_stream;
    Card_StreamHead *    p_streamHead;
    Card_StreamJoin *    p_streamJoin;
    Card_StreamKey *     p_streamKey;
    Card_StreamPer *     p_streamPer;
    Card_StreamPtn *     p_streamPtn;
    Card_StreamSet *     p_streamSet;
    Card_SubQuery *      p_subQuery;
    Card_SubSet *        p_subSet;

    Card_Table *         p_table;
    Card_TableInfo *     p_tableInfo;
    Card_TableList *     p_tableList;
    Card_TableNm *       p_tableNm;
    Card_TablePtn *      p_tablePtn;
    Card_TableSize *     p_tableSize;
    Card_Term *          p_term;
    Card_Top *           p_top;
    Card_Topology *      p_topology;

    Card_Unique *        p_unique;

    Card_Where *         p_where;

    Card*                p_NEW;
    Card_OP *            p_OP;
    Card_CONJUNCTION *   p_CONJ;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CardIterMembers(void);
   ~CardIterMembers(void) noexcept(false);

    void vCopyFrom(CardIterMembers* pMembers);

  private:

    Footer* aFooter(void);
  } ;

  Kid* _pKid(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
