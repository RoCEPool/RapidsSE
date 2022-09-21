/*============================================================================*/
/* TransformerTool.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Dissector.hpp"

#include "Bench.hpp"

namespace STORE
{
  class TransformerTool : public Object
  {
  protected:

    Bench*       m_pBench;
    Board*       m_pBoard;

    CardIter*    m_pMainCardIter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    TransformerTool(eCN eClass, Bench* pBench);
   ~TransformerTool(void) noexcept(false);

    void vBeginIteration(void);
    Card* pEachMain(void);
    Card* pEachMain(eCK eCard);
    void vEndIteration(void);

    Card* pFindMainOf(Card* pSubCard);
    Card* pFindMainOf(Card* pSubCard, Card*& rpParentCard);

  private:

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
