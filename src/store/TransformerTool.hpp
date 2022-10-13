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

    Bench*      m_pBench;
    Board*      m_pBoard;
    Board*      m_pBoard_costEst;
    Board*      m_pBoard_partition;
    Board*      m_pBoard_permute;
    Board*      m_pBoard_query;
    CardCtl*    m_pCardCtl;

    i8          m_iTransformNo;
    s32         m_sPhase;
    s256        m_sTranformation;
    s256        m_sDescription;
    bool        m_bPrintTransformation;

    Table*      m_pTable;
    StreamDef*  m_pStreamDef;
    IdxVec*     m_pIdxVec;
    Index*      m_pIndex;

    bool        m_bInteractive;

    PermGen*    m_pPermGen;

    eCK m_eMainCard;
    eCK m_eNextMain;

    BoardIter* m_pM;

    Card_Deck* _deck_;

    Card* m_pChildCard;
    Card* m_pDupCard;
    Card* m_pMainCard;
    Card* m_pNewCard;
    Card* m_pNextMain;
    Card* m_pPoppedCard;

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
    Card_StreamColList*    m_p_streamColList;
    Card_SubSet*           m_p_subSet;
    Card_Table*            m_p_table;
    Card_TableList*        m_p_tableList;
    Card_TablePtn*         m_p_tablePtn;
    Card_TablePtr*         m_p_tablePtr;
    Card_Term*             m_p_term;
    Card_Term*             m_p_term_0;
    Card_Term*             m_p_term_1;
    Card_UBound*           m_p_uBound;
    Card_Where*            m_p_where;

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
    void vTransformation(eBK eBoard, char* azTranformation, bool bPrintTransformation=true);
    i8   iTransformNoInc(void);

  protected:
    void vSetupCostEstBoard(void);
    void vSetupPartitionBoard(void);
    void vSetupPermuteBoard(void);
    void vSetupQueryBoard(void);

    void D_vDeleteMember(eCK eMember);
    void D_vDeleteMember(Card* pMember);
    Card* G_pGetMember(eCK eMember);
    void N_vNewMember(eCK eCard);
    void N_vNewMember(eCK eCard, Stz* szValue);
    void U_vSetMember(eCK eCard, Card* pCard);

    bool E_bEachMember(void);
    bool E_bEachMain(eCK eOfKind);
    bool E_bEachMain(eCK eOfKind, eCK eAndHasSub);
    void E_vRemoveDelete(eCK eCrossCheck);

    bool S_bSearchMember(eCK eMember, eCK eForSubCard);
    bool S_bSearchMember(Card* pMember, eCK eForSubCard);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
