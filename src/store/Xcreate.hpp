/*============================================================================*/
/* Xcreate.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xtest.hpp"

namespace STORE
{
  class Table;

  class Xcreate : public Xtest
  {
  public:

    // must be last
    Footer m_xFooter;

  public:
    Xcreate(eCN eClass);
   ~Xcreate(void) noexcept(false);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
