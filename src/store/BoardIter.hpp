/*============================================================================*/
/* BoardIter.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class TransformerTool;

  class BoardIter : public Object
  {
  private:
    Board* m_pBoard;
    i8     m_i_Main;
    Card*  m_pCurrMain;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    BoardIter(Board* pBoard);
   ~BoardIter(void) noexcept(false);

    void  vApplyMain(eCK eCard, std::function<void(Card* pCard)> vfApply);

    Card* pEachMain(void);
    Card* pEachMain(i8& ri_Main);
    Card* pEachMain(eCK eCard);
    Card* pEachMain(eCK eCard, i8& ri_Main);
    Card* pEachMain(i8& ri_Main, eCK& reCard);
    Card* pNextMain(void);
    Card* pPrevMain(void);

    Card_Sort*   pEach_sort(void);

    Card_Aggreg* pPrev_aggreg(void);


    i8 i_Main(void);
    void vPrev(void);
    bool bMore(void);
    void vSkip(void);


    // - - - - - - - - - - - - - - - - - -

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
