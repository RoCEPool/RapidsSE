/*============================================================================*/
/* Xlator.hpp --                                                              */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Xlator : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Xlator(eCN eClassNo, Bench* pBench);
   ~Xlator(void) noexcept(false);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    bool X_bBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred, char cWithMark);
    void X_vBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, char cWithMark);
    void X_vBoolCalc_RemovePred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred);

    Card_BoolCalc* X_pConvert_rpnCalc_To_boolCalc(Card_RpnCalc* p_rpnCalc);

    i8   X_iCandIndex_Rank(Card_CandIndex* p_candIndex);
    void X_vCandIndex_Rank(Card_CandIndex* p_candIndex, i8 iRank);

    bool X_bCoPtnJoin_CoPtnWith(Card_CoPtnJoin* p_coPtnJoin, Card_CoPtnPred* p_CoPtnPred);
    Card_CoPtnJoin* X_pCoPtnJoinList_Find_coPtn_for(Card_CoPtnJoinList* p_coPtnJoinList, Card_CoPtnPred* p_coPtnPred);
    bool X_bCoPtnJoinList_HasEqPredCmp(Card_CoPtnJoinList* p_coPtnJoinList, Card_CmpPred* p_predCmp);

    Card_StmColList* X_pELIST_DeriveStreamColList(Card_ELIST* p_ELIST);

    void X_vFilter_CullPred(Card_Filter* p_filter, char cWithMark);
    bool X_bFilter_IsEmptyFilter(Card_Filter* p_filter);

    void X_vIdxColList_Ordinate(Card_IdxColList* p_idxColList, Card_Stream* pCard_Stream);

    void X_vJoinOrderSeq_ExtractJoinOrder(Card_JoinOrderSeq* p_joinOrderSeq, Card_PtndJoin* p_PtndJoin);

    void X_vJoinPredList_Extract(Card_JoinPredList* p_joinPredList, Card_BoolCalc* p_boolCalc);

    void X_vObyElist_Populate(Card_ObyElist* p_obyElist, Index* pIndex);

    bool X_bPtndJoin_MayFollowAfter(Card_PtndJoin* p_ptndJoin, Card_SubSet* pPriorPtnJoins_sofar);
    void X_vPtndJoin_JoinEst(Card_PtndJoin* p_ptndJoin, Card_TableList* p_tableList);

    Card_StmCol* X_pStmColList_Find_streamCol(Card_StmColList* p_stmColList, Stz *szColNm, i8& i_StreamColNo);

    void X_vSubCard_ReplaceSub(Card* pSub, Card* pInParent, Card* pWith);

    Card_Table* X_pTableList_FindTable(Card_TableList* p_tableList, char cSyn);
    bool X_bTableList_PredCmpIsCoPtn(Card_TableList* p_tableList, Card_CmpPred* p_predCmp);

private:

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
