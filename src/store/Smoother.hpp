/*============================================================================*/
/* Smoother.hpp -- Smoother                                                   */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class Smoother : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Smoother(void);
   ~Smoother(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };

  Smoother* _pSmoother(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
