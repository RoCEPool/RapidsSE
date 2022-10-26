/*============================================================================*/
/* Xtest.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xglean.hpp"

namespace STORE
{
  class Table;

  class Xtest : public Xglean
  {
  public:

    // must be last
    Footer m_xFooter;

  public:
    Xtest(eCN eClass);
   ~Xtest(void) noexcept(false);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
