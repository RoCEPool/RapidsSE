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

    Table* A_pAccess_Table(Stz* szTableNm);

    bool M_bPredRecursive(Card_PRED* p_pred,
                          std::function<bool(Card_PRED* p_pred, bool bUpwards)> bfApply);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
