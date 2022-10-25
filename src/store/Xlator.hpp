/*============================================================================*/
/* Xlator.hpp --                                                              */
/*============================================================================*/
#pragma once

#include "Accessor.hpp"

namespace STORE
{
  class Xlator : public Accessor
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Xlator(eCN eClassNo, Bench* pBench);
   ~Xlator(void) noexcept(false);

    void vCullAll(eCK eCard);
    void vDeleteAll(eCK eCard);
    void vRenumberMainCards(void);

    void R_vReplaceSub(Card* pSub, Card* pInParent, Card* pWith);

    void X_calc_To_calcBool(void);

    Card_Table* pFind_table(Card_TableList* p_tableList, char cSyn);

    bool bPredCmpIsCoPtn(Card_TableList* p_tableList, Card_CmpPred* p_predCmp);



    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


    Card_CmpPred* pAndPred_FindBound(Card_AndPred* p_andPred, Index* pIndex, ePB eBound);
    bool bAndPred_HasBounds(Card_AndPred* p_andPred, Index* pIndex);

    bool bBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred, char cWithMark);
    void vBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, char cWithMark);
    void vBoolCalc_RemovePred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred);

    i8   iCandIndex_Rank(Card_CandIndex* p_candIndex);
    void vCandIndex_Rank(Card_CandIndex* p_candIndex, i8 iRank);

    void vCmpPred_MarkBound(Card_CmpPred* p_cmpPred);
    bool bCmpPred_CoPtnWith(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg);
    Card_CmpPred* pCmpPred_FindBound(Card_CmpPred* p_cmpPred, Index* pIndex, ePB eBound);
    bool bCmpPred_HasBounds(Card_CmpPred* p_cmpPred, Index* pIndex);
    bool bCmpPred_IsEq(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg);
    bool bCmpPred_OverlapsWith(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg);
    bool bCmpPred_UsesTable(Card_CmpPred* p_cmpPred, char cTableSyn);

    bool bColumnRef_IsColumnRef(Card_ColumnRef* p_columnRef);
    i8   iColumnRef_ColumnOrd(Card_ColumnRef* p_columnRef);
    char cColumnRef_TableSyn(Card_ColumnRef* p_columnRef);
    char cColumnRef_TableSyn_x(Card_ColumnRef* p_columnRef);
    void vColumnRef_TableSyn_x(Card_ColumnRef* p_columnRef, String* psTableSyn);

    bool bCoPtnJoin_CoPtnWith(Card_CoPtnJoin* p_coPtnJoin, Card_CoPtnPred* p_CoPtnPred);
    Card_CoPtnJoin* pCoPtnJoinList_Find_coPtn_for(Card_CoPtnJoinList* p_coPtnJoinList, Card_CoPtnPred* p_coPtnPred);
    bool bCoPtnJoinList_HasEqPredCmp(Card_CoPtnJoinList* p_coPtnJoinList, Card_CmpPred* p_predCmp);

    Card_StmColList* pELIST_DeriveStreamColList(Card_ELIST* p_ELIST);

    void vExpr_AsEnsure(Card_Expr* p_expr);

    void vFilter_CullPred(Card_Filter* p_filter, char cWithMark);
    bool bFilter_IsEmptyFilter(Card_Filter* p_filter);

    void vIdxColList_Ordinate(Card_IdxColList* p_idxColList, Card_Stream* pCard_Stream);

    void vJoinOrderSeq_ExtractJoinOrder(Card_JoinOrderSeq* p_joinOrderSeq, Card_PtndJoin* p_PtndJoin);

    void vJoinPredList_Extract(Card_JoinPredList* p_joinPredList, Card_BoolCalc* p_boolCalc);

    void vObyElist_Populate(Card_ObyElist* p_obyElist, Index* pIndex);

    Card_CmpPred* pOrPred_FindBound(Card_OrPred* p_orPred, Index* pIndex, ePB eBound);
    bool bOrPred_HasBounds(Card_OrPred* p_orPred, Index* pIndex);

    void vPRED_ConcatTerms(Card_PRED* p_PRED, Stz* szTerms, i8 i_From, i8 i_Till= C_UNKNOWN);
    void vPRED_CullByKids(Card_PRED* p_PRED);
    Card_CmpPred* pPRED_FindBound(Card_PRED* p_PRED, Index* pIndex, ePB eBound);
    bool bPRED_HasBounds (Card_PRED* p_PRED, Index* pIndex);
    static bool bPRED_IsPred(Card* pArgCard);

    bool bPtndJoin_MayFollowAfter(Card_PtndJoin* p_ptndJoin, Card_SubSet* pPriorPtnJoins_sofar);
    void vPtndJoin_JoinEst(Card_PtndJoin* p_ptndJoin, Card_TableList* p_tableList);

    Card_Term* pRpnTerms_HeadTerm(Card_RpnTerms* p_rpnTerms);
    Card_Term* pRpnTerms_PopTerm(Card_RpnTerms* p_rpnTerms);

    Card_StmCol* pStmColList_Find_streamCol(Card_StmColList* p_stmColList, Stz *szColNm, i8& i_StreamColNo);

    ePB  eTerm_Bound(Card_Term* p_term);
    i8   iTerm_IndexColNo(Card_Term* p_term);
    bool bTerm_IsAggOp(Card_Term* p_term);
    bool bTerm_IsCommaSeparator(Card_Term* p_term);
    bool bTerm_IsIndexColOrdinal(Card_Term* p_term);
    bool bTerm_IsParamOrdinal(Card_Term* p_term);
    bool bTerm_IsStreamColOrdinal(Card_Term* p_term);
    bool bTerm_IsTableColOrdinal(Card_Term* p_term);
    eRO  eTerm_RelOp(Card_Term* p_term);
    i8   iTerm_StreamColNo(Card_Term* p_term);
    i8   iTerm_TableColNo(Card_Term* p_term);
    char cTerm_TableSyn(Card_Term* p_term);
    eTE  eTerm_TermEval(Card_Term* p_term);


  private:

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
