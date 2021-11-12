/*============================================================================*/
/* Eval_Or.hpp -- Or Evaluator                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Eval.hpp"

namespace STORE
{
  class Eval_Or : public Eval
  {
  protected:

  private:
    // must be last
    Footer m_xFooter;

  public:  
    void* operator new(size_t);
    void  operator delete(void*);
  
    Eval_Or(void);
   ~Eval_Or(void) noexcept(false);
      
  private:
    Footer* aFooter(void);
  };

  Eval_Or* _pEval_Or(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
