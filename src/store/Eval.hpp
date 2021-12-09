/*============================================================================*/
/* Eval.hpp -- Evaluator                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eAE.h"

namespace STORE
{
  class Eval : public Object
  {
  protected:

  private:

  public:
    Eval(eCN eClass);
   ~Eval(void) noexcept(false);
      
    static eAE eAtomEval(Stz* szAtom);
  
  private:
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
