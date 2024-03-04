/*============================================================================*/
/* XF_FLATTEN_ABS.hpp -- FLATTEN                                              */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_FLATTEN_ABS : public XF_TRANSFORM
  {
  private:

  public:

    XF_FLATTEN_ABS(eCN eClass, CardIter* pCI);
   ~XF_FLATTEN_ABS(void) noexcept(false);

  protected:
    void vFold_basic(CardIter* pCI, Stz* szFoldNm, eCK eCardWithSQ);

    CarryForward* pFold_from(Stz* szFoldNm, CardIter* pCI);

    CarryForward* pFold_nc_subQuery(CardIter* pCI, Stz* szFoldNm,
                                    eCK eCardWithSQ, eQO eWhatOp);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
