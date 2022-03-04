/*============================================================================*/
/* Board.hpp -- Board                                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Dissect.hpp"
#include "eRN.h"


namespace STORE
{
  class Board : public Object
              , public CardSet // public ListExo2_t<Card>
  {
  protected:
    Optimiser* m_pOptimiser;
    CardCtl*   m_pCardCtl;

    s32 m_sQueryNm;

    // registers
    Card_Aggreg*     m_pReg_Aggreg;
    Card_CandIndex*  m_pReg_CandIndex;
    Card_Combine*    m_pReg_Combine;
    Card_Filter*     m_pReg_Filter;
    Card_Hash*       m_pReg_Hash;
    Card_IndexJoin*  m_pReg_IndexJoin;
    Card_IndexScan*  m_pReg_IndexScan;
    Card_Limit*      m_pReg_Limit;
    Card_OrdAgg*     m_pReg_OrdAgg;
    Card_Project*    m_pReg_Project;
    Card_Scan*       m_pReg_Scan;
    Card_Send*       m_pReg_Send;
    Card_Sort*       m_pReg_Sort;
    Card_Stage*      m_pReg_Stage;
    Card_Stream*     m_pReg_Stream;

    Card_PredCmp*    m_pPredCmp_ebound;
    Card_PredCmp*    m_pPredCmp_lbound;
    Card_PredCmp*    m_pPredCmp_ubound;

    Index*     m_pIndex;
    Table*     m_pTable;

    bool m_baFlag[RN_ENUMS];
  
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Board(Stz* szCardNm);
   ~Board(void) noexcept(false);

    void vOptimiser(Optimiser* pOptimiser, CardCtl* pCardCtl);

    void vPinCards(Ribbon* pRibbon, i8 i_Board);
    void vPrintBoard(Stz* szCue);

    Stz* szQueryNm(void);
    void vQueryNm(Stz* szQueryNm);
  
    void vWalkAllCards(std::function<void(Card* pCard)> f_vClosure);
  
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // registers
  
    void vReg_Aggreg   (Card_Aggreg*    pAggreg);
    void vReg_CandIndex(Card_CandIndex* pCandIndex);
    void vReg_Combine  (Card_Combine*   pCombine);
    void vReg_Filter   (Card_Filter*    pFilter);
    void vReg_Hash     (Card_Hash*      pHash);
    void vReg_IndexJoin(Card_IndexJoin* pIndexJoin);
    void vReg_IndexScan(Card_IndexScan* pIndexScan);
    void vReg_Limit    (Card_Limit*     pLimit);
    void vReg_OrdAgg   (Card_OrdAgg*    pOrdAgg);
    void vReg_Project  (Card_Project*   pProject);
    void vReg_Scan     (Card_Scan*      pScan);
    void vReg_Send     (Card_Send*      pSend);
    void vReg_Sort     (Card_Sort*      pSort);
    void vReg_Stage    (Card_Stage*     pStage);
    void vReg_Stream(Card_Stream*   pStream);

    void vPredCmp_ebound(Card_PredCmp* pPredCmp);
    void vPredCmp_lbound(Card_PredCmp* pPredCmp);
    void vPredCmp_ubound(Card_PredCmp* pPredCmp);

    void vIndex    (Index*     pIndex);
    void vTable    (Table*     pTable);

    bool bFlag(i8 i_Flag);
    void vFlag(i8 i_Flag, bool bFlag);

    bool bInRegister    (Card* pCard);
    void vResetRegisters(void);
    void vSetRegister   (Card* pCard);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    void vRankCandidateIndex(Card_CandIndex* pCard_CandIndex);
  
    void vResolveCard      (Card*           pCard);
    void vResolveTerm      (Card_Term*      pCard_Term);

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
  
    void vRenumber_Cards             (void);
  
    void vResolve_Symbols            (void);

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
 
  private:
      Footer* aFooter(void);
  };

  Board* _pBoard(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
