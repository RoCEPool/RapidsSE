/*============================================================================*/
/* Xextract.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xaccess.hpp"

namespace STORE
{
  class Table;

  class Xextract : public Xaccess
  {
  public:

    // must be last
    Footer m_xFooter;

  public:
    Xextract(eCN eClass);
   ~Xextract(void) noexcept(false);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
