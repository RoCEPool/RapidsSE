/*============================================================================*/
/* Flattening.hpp -- Join Ordering                                          */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class Flattening : public Object
  {
  private:

    CardIter* m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Flattening(CardIter* pCI);
   ~Flattening(void) noexcept(false);

  protected:
    Footer* aFooter(void);
  };

  Flattening* _pFlattening(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
