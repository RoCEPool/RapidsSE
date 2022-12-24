/*============================================================================*/
/* Xverify.hpp --                                                             */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "Bench.hpp"
#include "BoardIter.hpp"
#include "CardRecurse.hpp"

#include "SubQuery.hpp"

namespace STORE
{
  class Table;

  class Xverify : public Object
  {
  public:

    CardCtl*    m_pCardCtl;

    void V_vNo_Terms_StmColOdn(Card* p_card);

    // must be last
    Footer m_xFooter;

  public:
    Xverify(eCN eClass);
   ~Xverify(void) noexcept(false);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
