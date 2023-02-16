/*============================================================================*/
/* Fold.hpp -- Fold                                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class Fold : public Object
  {
  private:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Fold(void);
   ~Fold(void) noexcept(false);

  protected:
    Footer* aFooter(void);
  };

  Fold* _pFold(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
