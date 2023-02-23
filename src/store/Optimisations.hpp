/*============================================================================*/
/* Optimisations.hpp -- Join Ordering                                          */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class Optimisations : public Object
  {
  private:

    CardIter* m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Optimisations(CardIter* pCI);
   ~Optimisations(void) noexcept(false);

  protected:
    Footer* aFooter(void);
  };

  Optimisations* _pOptimisations(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
