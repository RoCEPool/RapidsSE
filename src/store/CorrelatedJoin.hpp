/*============================================================================*/
/* CorrelatedJoin.hpp -- CorrelatedJoin                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class CardIter;
  class CarryForward;
  class Card_BoolPred;

  class CorrelatedJoin : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    CorrelatedJoin(void);
   ~CorrelatedJoin(void) noexcept(false);


  private:


  private:
    Footer* aFooter(void);
  };

  CorrelatedJoin* _pCorrelatedJoin(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
