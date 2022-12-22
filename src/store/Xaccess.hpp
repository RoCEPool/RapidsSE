/*============================================================================*/
/* Xaccess.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xbase.hpp"

namespace STORE
{
  class Table;

  class Xaccess : public Xbase
  {
  public:

    // must be last
    Footer m_xFooter;

  public:
    Xaccess(eCN eClass);
   ~Xaccess(void) noexcept(false);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
