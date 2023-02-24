/*============================================================================*/
/* SawOpCards.hpp -- Saw Op Cards                                             */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class SawOpCards : public Object
  {
  public:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SawOpCards(CardIter* pCI);
   ~SawOpCards(void) noexcept(false);

  protected:
    Footer* aFooter(void);
  };

  SawOpCards* _pSawOpCards(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
