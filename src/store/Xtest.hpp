/*============================================================================*/
/* Xtest.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xextract.hpp"

namespace STORE
{
  class Table;

  class Xtest : public Xextract
  {
  public:


    bool T_bOrPred_HasBounds(Card_OrPred* p_orPred, Index* pIndex);
    bool T_bAndPred_HasBounds(Card_AndPred* p_andPred, Index* pIndex);
    bool T_bCmpPred_HasBounds(Card_CmpPred* p_cmpPred, Index* pIndex);

    bool        T_bPRED_HasBounds (Card_PRED* p_PRED, Index* pIndex);
    static bool T_bPRED_IsPred(Card* pArgCard);

    Card_CmpPred* T_pOrPred_FindBound(Card_OrPred* p_orPred, Index* pIndex, ePB eBound);
    Card_CmpPred* T_pAndPred_FindBound(Card_AndPred* p_andPred, Index* pIndex, ePB eBound);
    Card_CmpPred* T_pCmpPred_FindBound(Card_CmpPred* p_cmpPred, Index* pIndex, ePB eBound);
    Card_CmpPred* T_pPRED_FindBound(Card_PRED* p_PRED, Index* pIndex, ePB eBound);

    bool T_bBounds_Is_lbound(Card_Bounds* p_bounds);
    bool T_bBounds_Is_lbound_Or_ubound(Card_Bounds* p_bounds);
    bool T_bBounds_Is_ubound(Card_Bounds* p_bounds);

    bool T_bCmpPred_Contains_Sqy_Call(Card_CmpPred* p_cmpPred);
    bool T_bCmpPred_CoPtnWith(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg);
    bool T_bCmpPred_EqualTo_bounds(Card_CmpPred* p_cmpPred, Card_Bounds* p_bounds);
    bool T_bCmpPred_IsEq(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg);
    bool T_bCmpPred_MaybeBounds(Card_CmpPred* p_cmpPred, Card_Table* p_table,
                                bool bParamAllowedInBounds);
    bool T_bCmpPred_MaybeBounds_v2(Card_CmpPred* p_cmpPred, Card_Table* p_table,
                                   bool bParamAllowedInBounds);
    bool T_bCmpPred_MaybeBoundsOf_index(Card_CmpPred* p_cmpPred, Card_Index* p_index,
                                        bool bParamAllowedInBounds);
    bool T_bCmpPred_OverlapsWith(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg);
    Card_Term* E_pCmpPred_Term_param(Card_CmpPred* p_cmpPred);
    bool T_bCmpPred_UsesTable(Card_CmpPred* p_cmpPred, char cTableSyn);

    i8 T_iCard_ConvertableTo_StmColOdn(Card* p_card);
    i8 T_iCard_ConvertableTo_TblColOdn(Card* p_card);

    bool T_bCombined_Reinstate(eCK eMain, eCK eSubsequent);

    bool T_bColumnRef_IsColumnRef(Card_ColumnRef* p_columnRef);

    bool            T_bCoPtnJoin_CoPtnWith(Card_CoPtnJoin* p_coPtnJoin, Card_CoPtnPred* p_CoPtnPred);
    Card_CoPtnJoin* T_pCoPtnJoinList_Find_coPtn_for(Card_CoPtnJoinList* p_coPtnJoinList, Card_CoPtnPred* p_coPtnPred);
    bool            T_bCoPtnJoinList_HasEqPredCmp(Card_CoPtnJoinList* p_coPtnJoinList, Card_CmpPred* p_predCmp);

    bool T_bFilter_IsEmpty(Card_Filter* p_filter);

    bool T_bIndex_tblColOdn_IsKeyIn_index(Stz* szTblColOdn, Card_Index* p_index);

    bool T_bPRED_AllSubs_MaybeBoundsOf_index(Card_PRED* p_PRED, Card_Index* p_index,
                                             bool bParamAllowedInBounds);
    Card_PRED* T_pPRED_Containing_MaybeBoundsOf_index(Card_PRED* p_PRED, Card_Index* p_index,
                                                      bool bParamAllowedInBounds);

    bool T_bPtndJoin_MayFollowAfter(Card_PtndJoin* p_ptndJoin, Card_SubSet* pPriorPtnJoins_sofar);

    bool T_bStage_IsIgnorable(Card_Stage* p_stage);
    bool T_bStage_IsRedundant(Card_Stage* p_stage, Card_Stage* p_prior);
    bool T_bStage_Reinstate(eCK eMain, eCK eSubsequent);
    bool T_bStage_ReinstateV2(eCK eMain, eCK eSubsequent, eAL eLevel);
    bool T_bStage_ReinstateV2(eAL& reLevel, Card* p_Main, Card* p_PriorToMain);
    bool T_bStage_ReinstateV3(eAL& reLevel, Card* p_Main, Card* p_PriorToMain);

    Card_StmCol* T_pStmColList_Find_streamCol(Card_StmColList* p_stmColList, Stz *szColNm, i8& ri_StreamColNo);

    Card_Table*  T_pTableList_FindTable(Card_TableList* p_tableList, char cSyn);
    bool         T_bTableList_PredCmpIsCoPtn(Card_TableList* p_tableList, Card_CmpPred* p_predCmp);

    bool T_bTblColSyn_Same(char cTblSyn0, char cTblSyn1);

    bool T_bValue_IsIndexColOrdinal(Stz* szValue);  // $In
    bool T_bValue_IsKeyOrdinal(Stz* szValue);       // #n
    bool T_bValue_IsParamOrdinal(Stz* szValue);     // $Pn
    bool T_bValue_IsResultOrdinal(Stz* szValue);    // #n
    bool T_bValue_IsSubQueryName(Stz* szValue);     // __subqueryN
    bool T_bValue_IsSqyColOrdinal(Stz* szValue);    // __n
    bool T_bValue_IsStreamColOrdinal(Stz* szValue); // $n
    bool T_bValue_IsTableColOrdinal(Stz* szValue);  // $c.n

    // must be last
    Footer m_xFooter;

  public:
    Xtest(eCN eClass);
   ~Xtest(void) noexcept(false);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
