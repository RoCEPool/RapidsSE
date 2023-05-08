/*============================================================================*/
/* Smooth.hpp -- Smooth                                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class CardIter;
  class CarryForward;

  class Smooth : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Smooth(void);
   ~Smooth(void) noexcept(false);

    void vSmooth(CardIter* pCI, CarryForward* pCarryForward, bool bEarlyExit=false);

  private:
    Footer* aFooter(void);
  };

  Smooth* _pSmooth(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
