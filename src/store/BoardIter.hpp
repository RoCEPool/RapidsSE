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
    TransformerTool* m_pTransformerTool;
    Board* m_pBoard;
    i8     m_i_Main;
    Card*  m_pCurrMain;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    BoardIter(TransformerTool* pTransformerTool);
   ~BoardIter(void) noexcept(false);

    Card* pEachMain(void);
    Card* pEachMain(i8& ri_Main);
    Card* pEachMain(eCK eCard);
    Card* pEachMain(eCK eCard, i8& ri_Main);

    Card_Sort*   pEach_sort(void);

    Card_Stage*  pFirst_stage(void);

    Card_Stage*  pNext_stage(void);

    Card_Aggreg* pPrev_aggreg(void);


    i8 i_Main(void);
    void vNext(void);
    void vPrev(void);
    bool bMore(void);


    // - - - - - - - - - - - - - - - - - -

    void vRemoveCurr(eCK eCrossCheck);
    void vRemoveDeleteCurr(eCK eCrossCheck);

  private:

    Footer* aFooter(void);
  };

  BoardIter* _pBoardIter(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
