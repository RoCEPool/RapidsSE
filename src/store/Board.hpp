/*============================================================================*/
/* Board.hpp -- Board                                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Dissect.hpp"


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
    Card_Aggreg*     m_pAggreg;
    Card_CandIndex*  m_pCandIndex;
    Card_Combine*    m_pCombine;
    Card_Filter*     m_pFilter;
    Card_Hash*       m_pHash;
    Card_IndexJoin*  m_pIndexJoin;
    Card_IndexScan*  m_pIndexScan;
    Card_Limit*      m_pLimit;
    Card_OrdAgg*     m_pOrdAgg;
    Card_Project*    m_pProject;
    Card_Scan*       m_pScan;
    Card_Send*       m_pSend;
    Card_Sort*       m_pSort;
    Card_Stage*      m_pStage;
    Card_Stream*     m_pReg_Stream;


    Card_PredCmp*    m_pPredCmp_ebound;
    Card_PredCmp*    m_pPredCmp_lbound;
    Card_PredCmp*    m_pPredCmp_ubound;

    Index*     m_pIndex;
    Table*     m_pTable;

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
  
    void vAggreg   (Card_Aggreg*    pAggreg);
    void vCandIndex(Card_CandIndex* pCandIndex);
    void vCombine  (Card_Combine*   pCombine);
    void vFilter   (Card_Filter*    pFilter);
    void vHash     (Card_Hash*      pHash);
    void vIndexJoin(Card_IndexJoin* pIndexJoin);
    void vIndexScan(Card_IndexScan* pIndexScan);
    void vLimit    (Card_Limit*     pLimit);
    void vOrdAgg   (Card_OrdAgg*    pOrdAgg);
    void vProject  (Card_Project*   pProject);
    void vScan     (Card_Scan*      pScan);
    void vSend     (Card_Send*      pSend);
    void vSort     (Card_Sort*      pSort);
    void vStage    (Card_Stage*     pStage);
    void vReg_Stream(Card_Stream*   pStream);

    void vPredCmp_ebound(Card_PredCmp* pPredCmp);
    void vPredCmp_lbound(Card_PredCmp* pPredCmp);
    void vPredCmp_ubound(Card_PredCmp* pPredCmp);

    void vIndex    (Index*     pIndex);
    void vTable    (Table*     pTable);

    bool bInRegister    (Card* pCard);
    void vResetRegisters(void);
    void vSetRegister   (Card* pCard);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    void vRankCandidateIndex(Card_CandIndex* pCard_CandIndex);
  
    void vResolveCard      (Card*           pCard);
    void vResolveTerm      (Card_Term*      pCard_Term);

    Card_StreamDef* pNewStreamDef(Card_ELIST* pCard_ELIST);
    Card_StreamDef* pNewStreamDef(Card_Table* pCard_Table);

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
