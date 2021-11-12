/*============================================================================*/
/* Eval_And.hpp -- And Evaluator                                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Eval.hpp"

namespace STORE
{
  class Eval_And : public Eval
  {
  protected:

  private:
    // must be last
    Footer m_xFooter;

  public:  
    void* operator new(size_t);
    void  operator delete(void*);
  
    Eval_And(void);
   ~Eval_And(void) noexcept(false);
      
  private:
    Footer* aFooter(void);
  };

  Eval_And* _pEval_And(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
