/*============================================================================*/
/* Smoosh.hpp -- Smoosh                                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "eSM.h"

namespace STORE
{
  class CardIter;
  class CarryForward;

  class Smoosh : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Smoosh(void);
   ~Smoosh(void) noexcept(false);

    void vSmoosh(CardIter* pCI, eSM eSmoosh);

    void vInto_KEYnotEXISTS(CardIter* pCI);

  private:
    Footer* aFooter(void);
  };

  Smoosh* _pSmoosh(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
