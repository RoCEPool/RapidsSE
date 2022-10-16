/*============================================================================*/
/* TransformerTool.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "BoardIter.hpp"

#include "PermGen.hpp"

namespace STORE
{
  class TransformerTool : public Object
  {
  protected:

    static const bool C_DONT_PRINT    = false;
    static const bool C_UPDATE_MEMBER = true;

    Bench*      m_pBench;
    CardCtl*    m_pCardCtl;
    bool        m_bTracing;
    i8          m_iTrace;

    i8          m_iTransformNo;
    bool        m_bPrintTransformation;
    bool        m_bInteractive;
    s32         m_sPhase;
    s256        m_sTransformation;
    s256        m_sDescription;

    PermGen*    m_pPermGen;

    Table*      m_pTable;
    StreamDef*  m_pStreamDef;
    IdxVec*     m_pIdxVec;
    Index*      m_pIndex;

    Board*      m_pBoard;
    Card_Deck*  m_pDeck;

    Board*      m_pBoard_costEst;
    Board*      m_pBoard_partition;
    Board*      m_pBoard_permute;
    Board*      m_pBoard_query;

    Card* m_pCard_child;
    Card* m_pCard_dup;
    Card* m_pCard_new;
    Card* m_pCard_popped;
    Card* m_pCard_temp;

    // the M iterator - - - - - - - - - - - - - - - - - - - - -
    BoardIter* m_pM;

    Card* m_p_PriorToMain;
    eCK   m_e_PriorToMain;
    Card* m_p_MainCard;
    eCK   m_e_MainCard;
    Card* m_p_SubOfMain;
    eCK   m_e_SubOfMain;
    Card* m_p_SubsequentToMain;
    eCK   m_e_SubsequentToMain;

    Card* m_p_SubMember;
    eCK   m_e_SubMember;

    Card_Term* m_pTerm_0;
    Card_Term* m_pTerm_1;

    Card_AbsJoin*          m_p_absJoin;
    Card_Aggreg*           m_p_aggreg;
    Card_Calc*             m_p_calc;
    Card_BoolCalc*         m_p_calcBool;
    Card_CmpPred*          m_p_cmpPred;
    Card_Combined*         m_p_combined;
    Card_Comment*          m_p_comment;
    Card_CoPtnJoin*        m_p_coPtnJoin;
    Card_CoPtnJoinList*    m_p_coPtnJoinList;
    Card_CoPtnPred*        m_p_coPtnPred;
    Card_CoPtnPredList*    m_p_coPtnPredList;
    Card_CrossPtnJoin*     m_p_crossPtnJoin;
    Card_CrossPtnJoinList* m_p_crossPtnJoinList;
    Card_Expr*             m_p_expr;
    Card_Filter*           m_p_filter;
    Card_GbyElist*         m_p_gbyElist;
    Card_IdxCol*           m_p_idxCol;
    Card_IdxColList*       m_p_idxColList;
    Card_IndexJoin*        m_p_indexJoin;
    Card_IndexList*        m_p_indexList;
    Card_IndexPtr*         m_p_indexPtr;
    Card_IndexScan*        m_p_indexScan;
    Card_JoinOrder*        m_p_joinOrder;
    Card_JoinOrderSeq*     m_p_joinOrderSeq;
    Card_JoinPred*         m_p_joinPred;
    Card_JoinPredList*     m_p_joinPredList;
    Card_LBound*           m_p_lBound;
    Card_Permutation*      m_p_permutation;
    Card_Permutation*      m_p_permutation_low;
    Card_Project*          m_p_project;
    Card_PtndJoin*         m_p_ptndJoin;
    Card_PtndJoinSet*      m_p_ptndJoinSet;
    Card_SbyElist*         m_p_sbyElist;
    Card_Scan*             m_p_scan;
    Card_ScanPred*         m_p_scanPred;
    Card_Sort*             m_p_sort;
    Card_Stage*            m_p_stage;
    Card_StageTopo*        m_p_stageTopo;
    Card_Stream*           m_p_stream;
    Card_StreamCol*        m_p_streamCol;
    Card_StreamColList*    m_p_streamColList;
    Card_SubSet*           m_p_subSet;
    Card_Table*            m_p_table;
    Card_TableList*        m_p_tableList;
    Card_TablePtn*         m_p_tablePtn;
    Card_TablePtr*         m_p_tablePtr;
    Card_Term*             m_p_term;
    Card_UBound*           m_p_uBound;
    Card_Where*            m_p_where;

    s256 m_sT;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    TransformerTool(eCN eClass, Bench* pBench);
   ~TransformerTool(void) noexcept(false);

    Board* pBoard(void);

    eCK   eDecodeMain(Card* pCard);

    void vEndTransformation(void);

    Stz* szDescription(void);
    void vDescription(Stz* szDescription);

    Stz* szPhase(void);
    void vPhase(Stz* szPhase);
    void vPrintBoard(void);

    void vSetPrintByKind(eCK eCard, bool bPrintByKind);
    void vSetPrintProperties(eCK eCard, bool bPrintProperties);
    void vSetPrintSubCards(eCK eCard, bool bPrintSubCards);

    Stz* szTransformation(void);
    void vTransformation(eBK eBoard, char* azTransformation, bool bPrintTransformation=true);
    i8   iTransformNoInc(void);

  protected:
    bool bIsTracing(void);

    void vSwapToBoard(eBK eBoard);
    void vSetupBoard(eBK eBoard);
    void vSetupCostEstBoard(void);
    void vSetupPartitionBoard(void);
    void vSetupPermuteBoard(void);
    void vSetupQueryBoard(void);

    void vTracing(bool bTracing);
    void vTracer(Stz* szFormat, ...);

    void D_vDeleteMember(eCK eMember);
    void D_vDeleteMember(Card* pMember);

    bool E_bEachMain(void);
    bool E_bEachMain(eCK eOfMainKind);
    bool E_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind);
    bool E_bMoreMain(void);
    void E_vRemoveMain(eCK eCrossCheck);
    void E_vRemoveDeleteMain(eCK eCrossCheck);
    void E_vSkipNextMain(bool bClearMembers=false, bool bClearRegisters=false);
    void E_vSubsequentToMain(bool bUpdateMember = false);

    Card* F_pFetchMember(eCK eMember);

    bool H_bHasPriorToMain(bool bUpdateMember = false);
    bool H_bHasPriorToMainIf(eCK eOfPriorKind, bool bUpdateMember = false);
    bool H_bHasSubsequentToMain(bool bUpdateMember = false);
    bool H_bHasSubsequentToMainIf(eCK eOfSubsequentKind, bool bUpdateMember = false);

    void M_vClearMembers(void);
    void M_vClearMembersRegisters(void);
    void M_vClearRegisters(void);
    void M_vClearRegistersIf(eCK eCard);
    void M_vClearRegistersIf(Card* pCard);
    void M_vRestart(void);

    void N_vNewMember(eCK eMember, bool bUpdateMember=true);
    void N_vNewMember(eCK eMember, Stz* szValue, bool bUpdateMember=true);

    bool S_bSearchForSubMember(eCK eMember, eCK eSubCard, bool bUpdateMember=true);
    bool S_bSearchForSubMember(Card* pMember, eCK eSubCard, bool bUpdateMember=true);

    void U_vUpdateMember(eCK eMember, Card* pCard, const char* asFuncNm);

    Stz* szT(eCK eMember);
    Stz* szMemberNm(eCK eMember);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
