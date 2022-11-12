/*============================================================================*/
/* Xbase.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "BoardIter.hpp"

#include "Query.hpp"


namespace STORE
{
  class Xbase : public Object
  {
  public:
    Query* m_pQuery;

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
