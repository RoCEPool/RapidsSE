/*============================================================================*/
/* Xbase.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "BoardIter.hpp"
#include "CardRecurse.hpp"

#include "SubQuery.hpp"


namespace STORE
{
  class Xbase : public Object
  {
  public:
    //SubQuery* m_pMainQuery;
    //SubQuery* m_pSubQuery;

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
