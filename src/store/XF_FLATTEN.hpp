/*============================================================================*/
/* XF_FLATTEN.hpp -- FLATTEN                                              */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_FLATTEN : public XF_TRANSFORM
  {
  private:

  public:

    XF_FLATTEN(eCN eClass, CardIter* pCI);
   ~XF_FLATTEN(void) noexcept(false);

  protected:
    void vFold_basic(CardIter* pCI, Stz* szFoldNm, eCK eCardWithSQ);

    CarryForward* pFold_from(Stz* szFoldNm, CardIter* pCI);

    CarryForward* pFold_nc_subQuery(CardIter* pCI, Stz* szFoldNm,
                                    eCK eCardWithSQ, eQO eWhatOp);

    void vSmooth(CardIter* pCI, CarryForward* pCarryForward, bool bEarlyExit=false);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
