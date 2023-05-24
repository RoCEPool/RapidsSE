/*============================================================================*/
/* CardIterMembers.hpp --                                                     */
/*============================================================================*/
#pragma once

#include "Board.hpp"


namespace STORE
{
  class Kid //: public Object
  {
  private:
    Card*  m_pParent;
    i8     m_i_Child;
  public:

    bool   m_bActive;

  private:
//    // must be last
//    Footer m_xFooter;

  public:
//    void* operator new(size_t);
//    void  operator delete(void*);

    Kid(void);
    Kid(Card* pParent);
   ~Kid(void) noexcept(false);

    void vBeginForward(void);
    void vBeginReverse(void);

    void vFirst(void);
    void vLast(void);
    void vNext(void);
    void vPrev(void);
    bool bEach(void);
    bool bEach_reverse(void);
    bool bEach(eCK eCard);
    bool bEach_reverse(eCK eCard);

    Card* pParent(void);
    Card* pChild(void);
    Card* pChild(i8 i_Child);
    eCK   eChild(void);
    i8    i_Child(void);

    Card_Expr*   p_expr(void);
    Card_IdxCol* p_idxCol(void);
    Card_OP*     p_OP(void);
    Card_Stage*  p_stage(void);
    Card_Term*   p_term(void);

    i8 iGetOdn(void);

    Card* pFirstChild(void);
    Card* pNextChild(void);
    Card* pPrevChild(void);
    Card* pLastChild(void);

    bool bIsFirst(void);
    bool bIsLast(void);

    void vForward(void);
    void vReverse(void);

    bool bInRange(void);
    i8   nChildren(void);

    void vParent(Card*  pParent);
    void vChild(i8 i_Child);
    void vReset(void);

//  private:
//
//    Footer* aFooter(void);
  };

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
    bool   m_bRecursed;
    bool   m_bDontDescend;

    // - - - - - - - - - - - - -

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
    Card_AndPred *       p_andPred;
    Card_As *            p_as;

    Card_BoolCalc *      p_boolCalc;
    Card_Bound *         p_bound;
    Card_Bounds *        p_bounds;

    Card_Call *          p_call;
    Card_CmpPred *       p_cmpPred;
    Card_Combined *      p_combined;
    Card_Constraint *    p_constraint;
    Card_Correlation *   p_correlation;
    Card_CostJoining *   p_costJoining;
    Card_CostNetwork *   p_costNetwork;
    Card_CostStream *    p_costQuery;
    Card_CostUnicast *   p_costUnicast;
    Card_CrossJoin *     p_crossJoin;

    Card_Direction *     p_direction;

    Card_Expr *          p_expr;

    Card_Filter *        p_filter;
    Card_FiltPred *      p_filtPred;
    Card_FiltPredList *  p_filtPredList;
    Card_Flat *          p_flat;
    Card_Fold *          p_fold;
    Card_FoldElist *     p_foldElist;
    Card_From *          p_from;

    Card_GbyAgg *        p_gbyAgg;

    Card_Hash *          p_hash;
    Card_Having *        p_having;
    Card_HshCol *        p_hshCol;

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
    Card_KeyExists *     p_keyExists;
    Card_KeyNotExists *  p_keyNotExists;

    Card_Level *         p_level;
    Card_Limit *         p_limit;

    Card_NCorrCols *     p_nCorrCols;
    Card_Needed *        p_needed;
    Card_Cast *          p_cast;
    Card_Note *          p_note;
    Card_NotPred *       p_notPred;

    Card_On *            p_on;
    Card_OrdAgg *        p_ordAgg;
    Card_OrPred *        p_orPred;
    Card_OuterColNm *    p_outerColNm;
    Card_OuterJoin *     p_outerJoin;

    Card_Project *       p_project;
    Card_Projection *    p_projection;

    Card_RowCount *      p_rowCount;
    Card_RowSize *       p_rowSize;
    Card_RpnCalc *       p_rpnCalc;

    Card_Scan *          p_scan;
    Card_Section *       p_section;
    Card_Send *          p_send;
    Card_Sort *          p_sort;
    Card_Stage *         p_stage;
    Card_StreamHead *    p_streamHead;
    Card_StreamJoin *    p_streamJoin;
    Card_StreamKey *     p_streamKey;
    Card_StreamPer *     p_streamPer;
    Card_StreamPtn *     p_streamPtn;
    Card_StreamSet *     p_streamSet;
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

    Card*                p_NEW;
    Card_OP *            p_OP;
    Card_PRED *          p_PRED;

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
