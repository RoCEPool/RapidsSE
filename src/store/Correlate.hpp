/*============================================================================*/
/* Correlate.hpp -- Correlate                                                 */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "eSM.h"

namespace STORE
{
  class CardIter;
  class CarryForward;

  class Correlate : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Correlate(void);
   ~Correlate(void) noexcept(false);

    void vCorrelate(CardIter* pCI, eSM eCorrelate);

    void vInto_GBYAGG(CardIter* pCI);

  private:
    Footer* aFooter(void);
  };

  Correlate* _pCorrelate(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
