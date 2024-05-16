/*============================================================================*/
/* Equivalence.hpp -- Smooth Forward                                          */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class Equivalence : public Object
  {
  private:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Equivalence(void);
   ~Equivalence(void) noexcept(false);

  protected:
    Footer* aFooter(void);
  };

  Equivalence* _pEquivalence(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
