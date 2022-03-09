/*============================================================================*/
/* Board.hpp -- Board                                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Dissect.hpp"
#include "CardRegs.hpp"
#include "RuleCtl.hpp"

namespace STORE
{
  class Board : public Card_OTHER
  {
  protected:
    s32 m_sQueryNm;
    Optimiser* m_pOptimiser;

    CardRegs*  m_pCardRegs;
    RuleCtl*   m_pRuleCtl;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Board(Stz* szQueryNm, Optimiser* pOptimiser);
   ~Board(void) noexcept(false);

    CardRegs* pCardRegs(void);
    void vClear(void);

    void vDeleteAllCards(void);
  
    Card* pFindMain(Stz* szCardNm);
 
    Card* pGetMainCard(i8 i_MainCard);
    Card* pGetMainCard(i8 i_MainCard, eCK& reCard);
  
    i8   nMainCards(void);
  
    void vPinCards(Ribbon* pRibbon, i8 i_Board);
  
    void vPrintBoard(Stz* szCue);

    Stz* szQueryNm(void);
    void vQueryNm(Stz* szQueryNm);
      
    Card* pSearch(Stz* szCardNm);
    void vSearch(Stz* szCardNm, Card*& rpMainCard, Card*& rpSubCard);


    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    void vRankCandIndex(Card_CandIndex* pCard_CandIndex);
  
    void vResolveAll(Card*        pAnyCard);
    void vResolveTerm(Card_Term*  pCard_Term);

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
