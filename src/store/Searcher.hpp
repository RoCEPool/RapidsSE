/*============================================================================*/
/* Searcher.hpp --                                                             */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Searcher : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Searcher(Bench* pBench);
  
   ~Searcher(void) noexcept(false);

    Card* pFindMainOf(Card* pSubCard);

  private:

    Footer* aFooter(void);
  };

  Searcher* _pSearcher(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
