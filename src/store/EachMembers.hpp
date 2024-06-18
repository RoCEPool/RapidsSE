/*============================================================================*/
/* EachMembers.hpp --                                                     */
/*============================================================================*/
#pragma once

#include "Board.hpp"

#include "Kid.hpp"
#include "Elem.hpp"

namespace STORE
{
  class EachMembers : public Object
  {
    friend class CardIter;
  public:
    i8          i_Board;
    Board*      pBoard;

  // - - - - - - - - - - - - -
    Kid xEachOp;
    Kid xTarget;
    struct {
      eCK ePrimary;
      eCK eTarget;
      // --
      eCK eSecondary;
      eCK eTertiary;
    } Iter;

  // - - - - - - - - - - - - -
  public:
    i8     m_i_RecursionDepth;
    bool   m_bDontDescend;

    // - - - - - - - - - - - - -

    char   md_Coord[256];
    Card_OP * p_Op;
    eCK    e_Op;
    i8     i_Op;

    Card * p_Sub;
    Card * p_Sub2;
    Card * p_Sub3;

    // - - - - - - - - - - - - - - - - - -

    Card_AbsJoin *       p_absJoin;
    Card_Address *       p_address;
    Card_Aggreg *        p_aggreg;
    Card_AndConj *       p_andConj;

    Card_Bound *         p_bound;
    Card_Bounds *        p_bounds;

    Card_Call *          p_call;
    Card_Caller *        p_caller;
    Card_CallOp *        p_callOp;
    Card_Cast *          p_cast;
    Card_Clause *        p_clause;
    Card_Constraint *    p_constraint;
    Card_CorrCols *      p_corrCols;
    Card_Correlation *   p_correlation;
    Card_CostJoining *   p_costJoining;
    Card_CostNetwork *   p_costNetwork;
    Card_CostStream *    p_costQuery;
    Card_CostUnicast *   p_costUnicast;
    Card_Count *         p_count;
    Card_CrossJoin *     p_crossJoin;

    Card_Decision *      p_decision;
    Card_Deck*           p_deck;
    Card_Disposition *   p_disposition;

    Card_Emit *          p_emit;
    Card_Expr *          p_expr;

    Card_Filter *        p_filter;
    Card_FiltPred *      p_filtPred;
    Card_FiltPredList *  p_filtPredList;
    Card_Fold *          p_fold;
    Card_From *          p_from;

    Card_GbyAgg *        p_gbyAgg;
    Card_GlobalHash *    p_globalHash;

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
    Card_Inflate *       p_inflate;
    Card_Injectable *    p_injectable;

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
    Card_Neededv2 *      p_neededv2;
    Card_Note *          p_note;
    Card_NotConj *       p_notConj;

    Card_On *            p_on;
    Card_OrdAgg *        p_ordAgg;
    Card_OrConj *        p_orConj;
    Card_OuterColNm *    p_outerColNm;
    Card_OuterJoin *     p_outerJoin;

    Card_Payload *       p_payload;
    Card_PayloadCells *  p_payloadCells;
    Card_PdsRow *        p_pdsRow;
    Card_Predicate *     p_predicate;
    Card_Project *       p_project;

    Card_RelnOp *        p_relnOp;
    Card_Required *      p_required;
    Card_RlnConj *       p_rlnConj;
    Card_RrfJoin *       p_rrfJoin;
    Card_RowCount *      p_rowCount;
    Card_RowSize *       p_rowSize;

    Card_Scan *          p_scan;
    Card_Section *       p_section;
    Card_Send *          p_send;
    Card_Sort *          p_sort;
    Card_Spare *         p_spare;
    Card_SqyCalls *      p_sqyCalls;
    Card_SqyInfo *       p_sqyInfo;
    Card_SqyNo *         p_sqyNo;
    Card_Stage *         p_stage;
    Card_StreamHead *    p_streamHead;
    Card_StreamJoin *    p_streamJoin;
    Card_StreamKey *     p_streamKey;
    Card_StreamPer *     p_streamPer;
    Card_StreamPtn *     p_streamPtn;
    Card_StreamSet *     p_streamSet;
    Card_SubQuery *      p_subQuery;
    Card_SubSet *        p_subSet;

    Card_Table *         p_table;
    Card_TableList *     p_tableList;
    Card_TableNm *       p_tableNm;
    Card_TablePtn *      p_tablePtn;
    Card_TableSize *     p_tableSize;
    Card_Term *          p_term;
    Card_Top *           p_top;
    Card_Topology *      p_topology;

    Card_Unique *        p_unique;

    Card_Where *         p_where;

    Card_XCard *         p_xcard;


  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    EachMembers(void);
   ~EachMembers(void) noexcept(false);

    void vCopyFrom(EachMembers* pMembers);

  private:

    Footer* aFooter(void);
  } ;

  Kid* _pKid(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
