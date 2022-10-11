/*============================================================================*/
/* TransformerTool.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "BoardIter.hpp"
#include "CardMembers.hpp"

#include "PermGen.hpp"

namespace STORE
{
  class TransformerTool : public Object
                        , public CardMembers
  {
  protected:

    Bench*      m_pBench;
    Board*      m_pBoard;
    Board*      m_pBoard_costEst;
    Board*      m_pBoard_partition;
    Board*      m_pBoard_permute;
    Board*      m_pBoard_query;
    CardCtl*    m_pCardCtl;

    s256        m_sTranformation;
    bool        m_bPrintTransformation;

    Table*      m_pTable;
    StreamDef*  m_pStreamDef;
    IdxVec*     m_pIdxVec;
    Index*      m_pIndex;

    bool        m_bInteractive;

    PermGen*    m_pPermGen;

    eCK m_eMainCard;
    eCK m_eNextMain;
    eYG m_eStageTopo;

    BoardIter* m_pBoardIter;

//    Card* m_pChildCard;
//    i8    m_i_Child;
//    Card* m_pDupCard;
//    Card* m_pMainCard;
//    i8    m_i_Main;
//    Card* m_pNextMain;
//    i8    m_i_Next;
//    Card* m_pPoppedCard;
//    i8    m_i_Popped;
//
//    Card_AbsJoin*          m_p_absJoin;
//    Card_Aggreg*           m_p_aggreg;
//    Card_CalcBool*         m_p_calcBool;
//    Card_Combined*         m_p_combined;
//    Card_Comment*          m_p_comment;
//    Card_CoPtnJoin*        m_p_coPtnJoin;
//    Card_CoPtnJoinList*    m_p_coPtnJoinList;
//    Card_CoPtnPred*        m_p_coPtnPred;
//    Card_CoPtnPredList*    m_p_coPtnPredList;
//    Card_CrossPtnJoin*     m_p_crossPtnJoin;
//    Card_CrossPtnJoinList* m_p_crossPtnJoinList;
//    Card_Expr*             m_p_expr;
//    Card_Filter*           m_p_filter;
//    Card_GbyElist*         m_p_gbyElist;
//    Card_IdxColList*       m_p_idxColList;
//    Card_IndexJoin*        m_p_indexJoin;
//    Card_IndexList*        m_p_indexList;
//    Card_IndexPtr*         m_p_indexPtr;
//    Card_IndexScan*        m_p_indexScan;
//    Card_JoinOrder*        m_p_joinOrder;
//    Card_JoinOrderSeq*     m_p_joinOrderSeq;
//    Card_JoinPred*         m_p_joinPred;
//    Card_JoinPredList*     m_p_joinPredList;
//    Card_LBound*           m_p_lBound;
//    Card_Permutation*      m_p_permutation;
//    Card_Permutation*      m_p_permutation_low;
//    Card_CmpPred*          m_p_predCmp;
//    Card_Project*          m_p_project;
//    Card_PtndJoin*         m_p_ptndJoin;
//    Card_PtndJoinSet*      m_p_ptndJoinSet;
//    Card_SbyElist*         m_p_sbyElist;
//    Card_Scan*             m_p_scan;
//    Card_ScanPred*         m_p_scanPred;
//    Card_Sort*             m_p_sort;
//    Card_Stage*            m_p_stage;
//    Card_Stage*            m_p_stage_a;
//    Card_Stage*            m_p_stage_b;
//    Card_StageTopo*        m_p_stageTopo;
//    Card_StageTopo*        m_p_stageTopo_a;
//    Card_StageTopo*        m_p_stageTopo_b;
//    Card_Stream*           m_p_stream;
//    Card_StreamColList*    m_p_streamColList;
//    Card_SubSet*           m_p_subSet;
//    Card_Table*            m_p_table;
//    Card_TableList*        m_p_tableList;
//    Card_TablePtn*         m_p_tablePtn;
//    Card_TablePtr*         m_p_tablePtr;
//    Card_TableSyn*         m_p_tableSyn;
//    Card_Term*             m_p_term;
//    Card_Term*             m_p_term_0;
//    Card_Term*             m_p_term_1;
//    Card_UBound*           m_p_uBound;

    s256 m_sExplain;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    TransformerTool(eCN eClass, Bench* pBench);
   ~TransformerTool(void) noexcept(false);

    Board* pBoard(void);

    eCK   eDecodeMain(Card* pCard);

    void vExplain(Stz* szExplain);

    void vPrintBoard(void);

    bool bRecursivelyTheSame(Card* p_card_a, Card* p_card_b, i8 iDepth=0, bool bTestL0=false);

    void vSetPrintByKind(eCK eCard, bool bPrintByKind);
    void vSetPrintProperties(eCK eCard, bool bPrintProperties);
    void vSetPrintSubCards(eCK eCard, bool bPrintSubCards);

    void vTransformation(eBK eBoard, char* azTranformation, bool bPrintTransformation=true);


  protected:
    void vSetupCostEstBoard(void);
    void vSetupPartitionBoard(void);
    void vSetupPermuteBoard(void);
    void vSetupQueryBoard(void);

    bool I_bEach_mainCard_(void);
    bool I_bEach_mainCard_(eCK eCard);

    void vSetMember(eCK eCard, Card* pCard);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
