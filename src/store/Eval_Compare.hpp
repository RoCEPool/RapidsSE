/*============================================================================*/
/* Eval_Compare.hpp -- Compare Evaluator                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Eval.hpp"

namespace STORE
{
  class Eval_Compare : public Eval
  {
  protected:

  private:
    // must be last
    Footer m_xFooter;

  public:  
    void* operator new(size_t);
    void  operator delete(void*);
  
    Eval_Compare(void);
   ~Eval_Compare(void) noexcept(false);
      
  private:
    Footer* aFooter(void);
  };

  Eval_Compare* _pEval_Compare(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
