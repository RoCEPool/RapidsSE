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
    Card* pEachMain(eCK eCard);

    Card_Sort*   pEach_sort(void);

    Card_Stage*  pFirst_stage(void);

    Card_Stage*  pNext_stage(void);

    Card_Aggreg* pPrev_aggreg(void);


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
