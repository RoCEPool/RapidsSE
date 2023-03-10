/*============================================================================*/
/* CorrelatedJoin.hpp -- CorrelatedJoin                                                 */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "eSM.h"

namespace STORE
{
  class CardIter;
  class CarryForward;

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

    void vCorrelate(CardIter* pCI, eSM eCorrelate);

    void vVia_gbyAgg_Promotion(CardIter* pCI);

  private:
    Footer* aFooter(void);
  };

  CorrelatedJoin* _pCorrelatedJoin(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
