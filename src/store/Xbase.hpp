/*============================================================================*/
/* Xbase.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "BoardIter.hpp"


namespace STORE
{
  class Xbase : public Object
  {
  public:

    // must be last
    Footer m_xFooter;

  public:
    Xbase(eCN eClass);
   ~Xbase(void) noexcept(false);

    Footer* aFooter(void);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
