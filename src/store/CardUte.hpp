/*============================================================================*/
/* CardUte.hpp -- Card Utility                                                    */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class CardUte : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void operator delete(void*);

    CardUte(void);
   ~CardUte(void) noexcept(false);

  private:
    Footer* aFooter(void);

  };

  CardUte* _pCardUte(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
