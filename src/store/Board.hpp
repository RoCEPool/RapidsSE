/*============================================================================*/
/* Board.hpp -- Board                                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eBK.h"
#include "eBT.h"

#include "Card.hpp"
#include "CardCtl.hpp"

#include "Card_AbsJoin.hpp"
#include "Card_AggElist.hpp"
#include "Card_AggLevel.hpp"
#include "Card_Aggreg.hpp"
#include "Card_Aggregates.hpp"
#include "Card_As.hpp"

#include "Card_Board.hpp"

#include "Card_Calc.hpp"
#include "Card_CalcBool.hpp"
#include "Card_CandIndex.hpp"
#include "Card_CardPtr.hpp"
#include "Card_Combined.hpp"
#include "Card_Comment.hpp"
#include "Card_ColumnRef.hpp"
#include "Card_CoPtnJoin.hpp"
#include "Card_CoPtnJoinList.hpp"
#include "Card_CoPtnPred.hpp"
#include "Card_CoPtnPredList.hpp"
#include "Card_CrossPtnJoin.hpp"
#include "Card_CrossPtnJoinList.hpp"
#include "Card_CrossPtnPred.hpp"

#include "Card_Debug.hpp"
#include "Card_Direction.hpp"

#include "Card_EBound.hpp"
#include "Card_Exists.hpp"
#include "Card_Expr.hpp"
#include "Card_ExprBool.hpp"

#include "Card_Filter.hpp"

#include "Card_GbyElist.hpp"

#include "Card_Hash.hpp"
#include "Card_Having.hpp"

#include "Card_IdxColNm.hpp"
#include "Card_IdxColList.hpp"
#include "Card_IndexPtr.hpp"
#include "Card_IndexJoin.hpp"
#include "Card_IndexList.hpp"
#include "Card_IndexPred.hpp"
#include "Card_IndexScan.hpp"

#include "Card_JoinOrder.hpp"
#include "Card_JoinOrderSeq.hpp"
#include "Card_JoinPred.hpp"
#include "Card_JoinPredList.hpp"

#include "Card_LBound.hpp"
#include "Card_Limit.hpp"

#include "Card_OrdAgg.hpp"
#include "Card_ObyElist.hpp"

#include "Card_Permutation.hpp"
#include "Card_PredAnd.hpp"
#include "Card_PredCmp.hpp"
#include "Card_PredOr.hpp"
#include "Card_Project.hpp"
#include "Card_ProjElist.hpp"
#include "Card_PtndJoin.hpp"
#include "Card_PtndJoinSet.hpp"

#include "Card_RBound.hpp"
#include "Card_Retrieve.hpp"
#include "Card_RpnTerms.hpp"

#include "Card_SbyElist.hpp"
#include "Card_Scan.hpp"
#include "Card_ScanPred.hpp"
#include "Card_Send.hpp"
#include "Card_Sort.hpp"
#include "Card_Stage.hpp"
#include "Card_StageTopo.hpp"
#include "Card_Stream.hpp"
#include "Card_StreamElist.hpp"
#include "Card_SubSet.hpp"

#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_TablePtn.hpp"
#include "Card_TablePtr.hpp"
#include "Card_TableSyn.hpp"
#include "Card_Term.hpp"
#include "Card_Type.hpp"

#include "Card_UBound.hpp"
#include "Card_Unique.hpp"

#include "Card_Where.hpp"

#include "VertexSpec.hpp"
#include "StageSpec.hpp"
#include "OpSpec.hpp"


namespace STORE
{
  class Bench;
  class Dissector;
  class Drafter;
  class Optimiser;
  class Weaver;

  class Board : public Object
  {
  protected:
    eBK          m_eBoard;
    eBT          m_eBoardType;

    Optimiser*   m_pOptimiser;
    Bench*       m_pBench;
    Card_Board*  m_pCard_Board;

    b8           m_bBeenCued;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Board(eBK eBoard, eBT eBoardType, Bench* pBench);
   ~Board(void) noexcept(false);

    void vAppendMain(Card* pNewMainCard, bool bRenumber= true);

    eBK eBoard(void);
    CardCtl* pCardCtl(void);
    Card_Board* pCard_Board(void);

    void vClear(void);

    void vClearAllMarks(void);

    void vDeleteAllCards(void);
    void vDeleteFromTill(Card* pFrom, Card* pTillBefore);
    void vDuplicateBoard(Board* pBoard_arg);

    Card_Filter* pFindFilterCard(i8 iFilter, Card*& rpMainCard);
    i8    iFindMain(Card* pMainCard);
    Card* pFindMain(eCK eCard);
    Card* pFindMain(eCK eCard, i8& i_Main);
    Card* pFindMain(Stz* szCardNm);
    Card* pFindNextMain(Card* pAfter, eCK eFindCard);
    Card* pFindNextMain(Card* pAfter, eCK eFindCard, i8& i_Main);
    Card* pFindPrevMain(Card* pBefore, eCK eFindCard);
    Card* pFindPrevMain(Card* pBefore, eCK eFindCard, i8& i_Main);
    Card_Stream* pFindStreamCard(i8 iStream, Card*& rpMainCard);

    Card* pGetMain(i8 i_Main);
    Card* pGetMain(i8 i_Main, eCK& reCard);

    void vInsertMainCard_after(Card* pNewMainCard, Card* pAfterMainCard, bool bRenumber=true);
    void vInsertMainCard_before(Card* pNewMainCard, Card* pBeforeMainCard, bool bRenumber=true);

    i8   nMainCards(void);

    SubCardIter* pNewMainCardIter(void);

    void vPinCards(Ribbon* pRibbon, Stz* szTransformation=nullptr, Stz* szExplain=nullptr);
  
    void vPrintBoard(Stz* szCue=nullptr, Stz* szExplain=nullptr);
    Card* pPriorSubCard(Card*& rpMainCard, eCK eSubCard, i8 i_PriorToMain);

    void vRemoveMainCard(Card* pMainCard);

    void vRenumberMainCards(void);


    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

//    void vRankCandIndex(Card_CandIndex* pCard_CandIndex);
//
//    void vResolveFilter              (Card_Filter*  pCard_Filter);
//    void vResolveJoin                (Card_AbsJoin* pCard_AbsJoin);
//    void vResolveJoin2               (Card_AbsJoin* pCard_AbsJoin);
//    void vResolveJoin3               (Card_IndexJoin* pCard_IndexJoin);
//    void vAResolveJoin               (Card_IndexJoin* pCard_IndexJoin);
//    void vConvertJoin                (Card_AbsJoin* pCard_AbsJoin);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~


//    bool bAdd_CandIndexes            (void);
//
//    void vAdd_IndexScan              (void);
//
//    void vCull_Filter                (void);
//
//    void vCut_CandidateIndexes       (void);
//
//    void vCut_PredCmps               (void);
//
//    void vCut_Scan                   (void);
//
//    bool bFind_Scan_AggGby           (bool bMark);
//
//    bool bFind_Scan_Filter           (bool bMark);
//
//    void vMark_PredCmps              (void);
//
//    void vOrder_ProjectiveStreamDefs (void);
//
//    bool bPick_CandidateIndex        (void);
//
//    void vRank_CandidateIndexes      (void);
//
//    void vReduce_Filter              (void);
//
//    void vSimplify_Initial           (void);
//
//    //void vResolve_RootStreams        (void);
//
//    void vResolve_Filters              (void);
//
//
//    void vResolve_Symbols            (void);
//
//    void vAResolve_Joins             (void);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~


  private:
      Footer* aFooter(void);
  };

  Board* _pBoard(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
