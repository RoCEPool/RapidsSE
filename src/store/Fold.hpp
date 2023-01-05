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
      
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Fold(eCN eClassNo);
   ~Fold(void) noexcept(false);
   
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
