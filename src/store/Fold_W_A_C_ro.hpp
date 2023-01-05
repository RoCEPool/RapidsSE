/*============================================================================*/
/* Fold_W_A_C_ro.hpp -- Fold_W_A_C_ro                                         */
/*============================================================================*/
#pragma once

#include "Fold.hpp"

namespace STORE
{
  class Fold_W_A_C_ro : public Fold
  {
  private:
      
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Fold_W_A_C_ro(void);
   ~Fold_W_A_C_ro(void) noexcept(false);
   
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
