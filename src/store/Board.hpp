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
    s32 m_sQueryNm;

    // registers
    Card_CandidateIndex* m_pCandidateIndex;
    Card_Filter*         m_pFilter;
    Card_IndexScan*      m_pIndexScan;
    Card_CandidateIndex* m_pPickedCandidateIndex;
    Card_Scan*           m_pScan;
  
    Table*               m_pScanTable;

  
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Board(Stz* szCardNm);
   ~Board(void) noexcept(false);
     
    void vPinCards(Ribbon* pRibbon, i8 i_Board);

    Stz* szQueryNm(void);
    void vQueryNm(Stz* szQueryNm);
  
    void vWalkAllCards(std::function<void(Card* pCard)> f_vClosure);
  

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  
    void vOptimiseBoard(void);
  
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    void vCandidateIndex(Card_CandidateIndex* pCandidateIndex);
    void vFilter(Card_Filter* pFilter);
    void vIndexScan(Card_IndexScan* pIndexScan);
    void vPickedCandidateIndex(Card_CandidateIndex* pPickedCandidateIndex);
    void vScan(Card_Scan* pScan);
    void vScanTable(Table* pScanTable);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  
    bool bAdd_CandidateIndexes         (void);

    void vAdd_IndexScan                (void);

    void vCut_CandidateIndexes         (void);
  
    void vCut_Scan_Filter              (void);
  
    bool bFind_Scan_Filter             (void);
  
    bool bPick_CandidateIndex          (void);
  
    void vRank_CandidateIndexes        (void);
  
    void vRenumber_Cards               (void);
  
    void vReplace_Ordinals             (void);
  
    void vResolve_GenerativeStreamDefs (void);
  
    void vResolve_ProjectiveStreamDefs (void);
  
    void vSubstitute_Scan_IndexScan    (void);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  
    void vPrintBoard(Stz* szCue);
    void vRemoveCards(eCK eCard);

  private:
      Footer* aFooter(void);
  };

  Board* _pBoard(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
