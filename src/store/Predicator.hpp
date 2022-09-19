/*============================================================================*/
/* Predicator.hpp -- predicate tool                                           */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Predicator : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Predicator(Bench* pBench);
   ~Predicator(void) noexcept(false);

    void vDiscover(void);

  private:
    //void vDiscoverRecursive(Card* pAnyCard, Card* pMainCard_filter);
    void vDiscoverRecursive(Card* pMainCard_filter);

    Footer* aFooter(void);
  };

  Predicator* _pPredicator(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
