/*============================================================================*/
/* Xverify.hpp --                                                             */
/*============================================================================*/
#pragma once

#include "Xtest.hpp"

namespace STORE
{
  class Table;

  class Xverify : public Xtest
  {
  public:

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
