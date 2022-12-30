/*============================================================================*/
/* Xmodify.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xsentinals.hpp"

namespace STORE
{
  class Table;

  class Xmodify : public Xsentinals
  {
  public:
    CardCtl*     m_pCardCtl;
    // must be last
    Footer m_xFooter;

  public:
    Xmodify(eCN eClass);
   ~Xmodify(void) noexcept(false);

  public:
    // generalised drill down

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
