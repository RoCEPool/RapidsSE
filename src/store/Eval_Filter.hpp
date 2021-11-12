/*============================================================================*/
/* Eval_Filter.hpp -- Filter Evaluator                                        */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Eval.hpp"
#include "Card_Atom.hpp"
#include "Card_Filter.hpp"

namespace STORE
{
  class Eval_Filter : public Eval
  {
  protected:

  private:
    // must be last
    Footer m_xFooter;

  public:  
    void* operator new(size_t);
    void  operator delete(void*);
  
    Eval_Filter(void);
   ~Eval_Filter(void) noexcept(false);
      
    void vEvaluateFilter(Card_Filter* pCard_Filter);

  private:
    Footer* aFooter(void);
  };

  Eval_Filter* _pEval_Filter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
