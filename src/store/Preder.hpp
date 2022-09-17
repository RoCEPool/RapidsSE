/*============================================================================*/
/* Preder.hpp --                                                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Dissector.hpp"

namespace STORE
{
  class Preder : public Object
  {
  private:

    Card_SubSet* m_pCard_SubSet;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Preder(Card_SubSet* pCard_SubSet);
   ~Preder(void) noexcept(false);

    void vPred(void);

  private:

    Footer* aFooter(void);
  };

  Preder* _pPreder(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
