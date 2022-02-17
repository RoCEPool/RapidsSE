/*============================================================================*/
/* Board.hpp -- Board                                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Dissect.hpp"


namespace STORE
{
  class Board : public Object
              , public ListExo2_t<Card>
  {
  protected:
    s32 m_sQueryNm;

    // registers
    Card_CandidateIndex* m_pCandidateIndex;
    Card_IndexScan*      m_pIndexScan;
    Card_CandidateIndex* m_pPickedCandidateIndex;
    Card_Scan*           m_pScan;
  
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
    void vIndexScan(Card_IndexScan* pIndexScan);
    void vPickedCandidateIndex(Card_CandidateIndex* pPickedCandidateIndex);
    void vScan(Card_Scan* pScan);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  
    void vAddCandidateIndexes   (void);

    void vAddIndexScan          (void);

    void vCutCandidateIndexes   (void);
  
    void vGenerativeStreamDefs  (void);
  
    void vReplaceOrdinals    (void);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  
    void vPrintBoard(void);
    void vRenumberCards(void);

  private:
      Footer* aFooter(void);
  };

  Board* _pBoard(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
