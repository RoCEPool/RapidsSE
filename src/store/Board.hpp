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
  
    s32 m_sQueryNm;

    // registers
    Card_CandIndex*  m_pCandidateIndex;
    Card_PredCmp*    m_pComparePred_ebound;
    Card_PredCmp*    m_pComparePred_lbound;
    Card_PredCmp*    m_pComparePred_ubound;
    Card_Filter*     m_pFilter;
    Card_IndexScan*  m_pIndexScan;
    Card_Project*    m_pProject;
    Card_Scan*       m_pScan;
    Card_Stage*      m_pStage;

    Index*     m_pIndex;
    StreamDef* m_pStreamDef;
    Table*     m_pTable;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Board(Stz* szCardNm);
   ~Board(void) noexcept(false);

    void vOptimiser(Optimiser* pOptimiser);

    void vPinCards(Ribbon* pRibbon, i8 i_Board);
    void vPrintBoard(Stz* szCue);

    Stz* szQueryNm(void);
    void vQueryNm(Stz* szQueryNm);
  
    void vWalkAllCards(std::function<void(Card* pCard)> f_vClosure);
  
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    void vCandidateIndex(Card_CandIndex* pCandidateIndex);
    void vComparePred_ebound(Card_PredCmp* pComparePred);
    void vComparePred_lbound(Card_PredCmp* pComparePred);
    void vComparePred_ubound(Card_PredCmp* pComparePred);
    void vFilter(Card_Filter* pFilter);
    void vIndexScan(Card_IndexScan* pIndexScan);
    void vProject(Card_Project* pProject);
    void vScan(Card_Scan* pScan);
    void vStage(Card_Stage* pStage);

    void vIndex(Index* pIndex);
    void vStreamDef(StreamDef* pStreamDef);
    void vTable(Table* pTable);

    void vResetRegisters(void);
  
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    void vRankCandidateIndex(Card_CandIndex* pCard_CandIndex);
  
    void vCardResolve      (Card*           pCard);
    void vTermResolve      (Card_Term*      pCard_Term);

    Card_StreamDef* pNewStreamDef(Card_ELIST* pCard_ELIST);
    Card_StreamDef* pNewStreamDef(Card_Table* pCard_Table);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  
    bool bAdd_CandIndexes            (void);

    void vAdd_IndexScan              (void);

    void vCull_Filter                (void);
  
    void vCut_CandidateIndexes       (void);
  
    void vCut_ComparePreds           (void);
  
    void vCut_Scan                   (void);
  
    bool bFind_Scan_AggGby           (bool bMark);
  
    bool bFind_Scan_Filter           (bool bMark);
  
    void vMark_ComparePreds          (void);
  
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
