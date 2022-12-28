/*============================================================================*/
/* BoardIter.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "CardIter.hpp"

namespace STORE
{
  class Board;

  class BoardIter : public CardIter
  {
  private:
    Board* m_pBoard;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    BoardIter(void);
    BoardIter(Board* pBoard);
   ~BoardIter(void) noexcept(false);

    void  vApplyMain(eCK eCard, std::function<void(Card* pCard)> vfApply);

    void vChangeBoard(Board* pBoard);

    Card* pEachMain(void);
    Card* pEachMain(i8& ri_Main);
    Card* pEachMain(eCK eCard);
    Card* pEachMain_reverse(eCK eCard);

    Card* pNextMain(void);
    Card* pNextMainIf(eCK eCard);

    Card* pPrevMain(void);
    Card* pPrevMainIf(eCK eCard);

    i8 i_Main(void);
    Card* pRewindMain(void);
    bool bMore(void);
    void vForward(void);


    // - - - - - - - - - - - - - - - - - -

    void vRemoveCard(Card* pCard);
    void vRemoveCurr(eCK eCrossCheck);
    void vRemoveDeleteCurr(eCK eCrossCheck);

    void vSet(Card* pCurrMain);
    void vSet(i8 i_Main, Card* pCurrMain);

  private:

    Footer* aFooter(void);
  };

  BoardIter* _pBoardIter(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
