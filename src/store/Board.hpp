/*============================================================================*/
/* Board.hpp -- Board                                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eBK.h"

#include "Card.hpp"
#include "CardCtl.hpp"

#include "Card_AbsJoin.hpp"
#include "Card_AggElist.hpp"
#include "Card_AggLevel.hpp"
#include "Card_Aggreg.hpp"
#include "Card_Aggregates.hpp"
#include "Card_As.hpp"

#include "Card_Calc.hpp"
#include "Card_CalcBool.hpp"
#include "Card_CandIndex.hpp"
#include "Card_CardPtr.hpp"
#include "Card_Combined.hpp"
#include "Card_Comment.hpp"
#include "Card_ColumnRef.hpp"
#include "Card_CoPtn.hpp"
#include "Card_CoPtnList.hpp"
#include "Card_CoPtnPred.hpp"
#include "Card_CoPtnPredList.hpp"
#include "Card_CrossPtn.hpp"
#include "Card_CrossPtnList.hpp"
#include "Card_CrossPtnPred.hpp"

#include "Card_Debug.hpp"
#include "Card_Direction.hpp"

#include "Card_EBound.hpp"
#include "Card_Exists.hpp"
#include "Card_Expr.hpp"
#include "Card_ExprBool.hpp"

#include "Card_Filter.hpp"

#include "Card_GbyElist.hpp"
#include "Card_Generate.hpp"

#include "Card_Hash.hpp"
#include "Card_Having.hpp"

#include "Card_IdxCol.hpp"
#include "Card_IdxColList.hpp"
#include "Card_IndexPtr.hpp"
#include "Card_IndexJoin.hpp"
#include "Card_IndexList.hpp"
#include "Card_IndexPred.hpp"
#include "Card_IndexScan.hpp"

#include "Card_JoinPred.hpp"
#include "Card_JoinPredList.hpp"
#include "Card_PtndJoinSet.hpp"

#include "Card_LBound.hpp"
#include "Card_Limit.hpp"

#include "Card_Map.hpp"
#include "Card_MapSet.hpp"
#include "Card_Mode.hpp"

#include "Card_Name.hpp"

#include "Card_On.hpp"
#include "Card_Option.hpp"
#include "Card_OrdAgg.hpp"
#include "Card_ObyElist.hpp"

#include "Card_Permutation.hpp"
#include "Card_PredAnd.hpp"
#include "Card_PredCmp.hpp"
#include "Card_PredOr.hpp"
#include "Card_Project.hpp"
#include "Card_ProjElist.hpp"

#include "Card_RBound.hpp"
#include "Card_Rows.hpp"
#include "Card_RpnTerms.hpp"

#include "Card_SbyElist.hpp"
#include "Card_Scan.hpp"
#include "Card_ScanPred.hpp"
#include "Card_Send.hpp"
#include "Card_SendKind.hpp"
#include "Card_Sends.hpp"
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

#include "VertexSpec.hpp"
#include "StageSpec.hpp"
#include "OpSpec.hpp"

#include "CardRegs.hpp"
#include "RuleCtl.hpp"

namespace STORE
{
  class Bench;
  class Dissector;
  class Drafter;
  class Optimiser;
  class Weaver;

  class Board : public Object //Card_OTHER
  {
  protected:
    eBK          m_eBoard;
    Bench*       m_pBench;

    s32          m_sQueryNm;

    Optimiser*   m_pOptimiser;

    CardRegs*    m_pCardRegs;
    RuleCtl*     m_pRuleCtl;

    Card_SubSet* m_pCard_SubSet;

    b8           m_bBeenCued;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Board(eBK eBoard, Bench* pBench);
   ~Board(void) noexcept(false);

    void vAppendMainCard(Card* pNewMainCard, bool bRenumber=true);

    eBK eBoard(void);
    CardRegs* pCardRegs(void);
    Card_SubSet* pCard_SubSet(void);

    void vClear(void);

    void vClearAllMarks(void);

    void vDeleteAllCards(void);
    void vDeleteFromTill(Card* pFrom, Card* pTillBefore);

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

    Card* pGetMainCard(i8 i_Main);
    Card* pGetMainCard(i8 i_Main, eCK& reCard);

    void vInsertMainCard_after(Card* pNewMainCard, Card* pAfterMainCard, bool bRenumber=true);
    void vInsertMainCard_before(Card* pNewMainCard, Card* pBeforeMainCard, bool bRenumber=true);

    i8   nMainCards(void);

    CardIter* pNewMainCardIter(void);

    void vPinCards(Ribbon* pRibbon, i8 i_Board);
  
    void vPrintBoard(Stz* szCue=nullptr);
    Card* pPriorSubCard(Card*& rpMainCard, eCK eSubCard, i8 i_PriorToMain);

    void vRemoveMainCard(Card* pMainCard);

    void vRenumberMainCards(void);

    Card* pSearch(Stz* szCardNm);
    void vSearch(Stz* szCardNm, Card*& rpMainCard, Card*& rpSubCard);


    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    void vRankCandIndex(Card_CandIndex* pCard_CandIndex);

    void vResolveFilter              (Card_Filter*  pCard_Filter);
    void vResolveJoin                (Card_AbsJoin* pCard_AbsJoin);
    void vResolveJoin2               (Card_AbsJoin* pCard_AbsJoin);
    void vResolveJoin3               (Card_IndexJoin* pCard_IndexJoin);
    void vAResolveJoin               (Card_IndexJoin* pCard_IndexJoin);
    void vConvertJoin                (Card_AbsJoin* pCard_AbsJoin);
    void vResolveRecursive           (Card*         pAnyCard);
    void vResolveTerm                (Card_Term*    pCard_Term);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  
    bool bAdd_CandIndexes            (void);

    void vAdd_IndexScan              (void);

    void vCull_Filter                (void);
  
    void vCut_CandidateIndexes       (void);
  
    void vCut_PredCmps               (void);
  
    void vCut_Scan                   (void);
  
    bool bFind_Scan_AggGby           (bool bMark);
  
    bool bFind_Scan_Filter           (bool bMark);
  
    void vMark_PredCmps              (void);
  
    void vOrder_ProjectiveStreamDefs (void);
  
    bool bPick_CandidateIndex        (void);

    void vRank_CandidateIndexes      (void);
  
    void vReduce_Filter              (void);

    void vSimplify_Initial           (void);

    //void vResolve_RootStreams        (void);

    void vResolve_Filters              (void);


    void vResolve_Symbols            (void);

    void vAResolve_Joins             (void);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    /* rule 00 */ bool bMay_AggGby_Sort_to_AggGby      (void);
  
    /* rule 00 */ bool bMay_ordStream_AggGby_to_OrdAgg (void);

    /* rule 00 */ bool bMay_OrdAgg_Sort_to_OrdAgg      (void);
  
    /* rule 00 */ bool bMay_Scan_AggGby_to_IndexScan   (void);

    /* rule 00 */ bool bMay_Scan_Filter_to_IndexScan   (void); // implemented
  
    /* rule 00 */ bool bMay_Scan_Sort_to_IndexScan     (void);
  
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    /* rule 00 */ bool bRule_AggGby_Sort_to_AggGby     (void);
  
    /* rule 00 */ bool bRule_ordStream_AggGby_to_OrdAgg(void);
  
    /* rule 00 */ bool bRule_OrdAgg_Sort_to_OrdAgg     (void);
  
    /* rule 00 */ bool bRule_Scan_AggGby_to_IndexScan  (void);
  
    /* rule 00 */ bool bRule_Scan_Filter_to_IndexScan  (void); // implemented

    /* rule 00 */ bool bRule_Scan_Sort_to_IndexScan    (void);
  
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    Card_CalcBool*   pTo_CalcBool  (Card_Calc*    pCard_Calc);
    Card_IndexPred*  pTo_IndexPred (Card_PredCmp* pCard_PredCmp,
                                    Card_MapSet* pCard_MapSet);

  private:
      Footer* aFooter(void);
  };

  Board* _pBoard(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
