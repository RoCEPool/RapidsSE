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
    Card_CandidateIndex* m_pCandidateIndex; // C
    Card_Filter*         m_pFilter;         // F
    Card_IndexScan*      m_pIndexScan;      // I
    Card_Project*        m_pProject;        // P
    Card_Scan*           m_pScan;           // S
    Card_Stage*          m_pStage;          // G

    Index*               m_pIndex;
    Table*               m_pTable;

  
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
    void vProject(Card_Project* pProject);
    void vScan(Card_Scan* pScan);
    void vStage(Card_Stage* pStage);

    void vIndex(Index* pIndex);
    void vTable(Table* pTable);

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
    void vRankCandidateIndex(Card_CandidateIndex* pCard_CandidateIndex);
 
  private:
      Footer* aFooter(void);
  };

  Board* _pBoard(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
