/*============================================================================*/
/* Xglean.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xaccess.hpp"

namespace STORE
{
  class Table;

  class Xglean : public Xaccess
  {
  public:

    // must be last
    Footer m_xFooter;

  public:
    Xglean(eCN eClass);
   ~Xglean(void) noexcept(false);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
