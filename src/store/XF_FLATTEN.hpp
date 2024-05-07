/*============================================================================*/
/* XF_FLATTEN.hpp -- FLATTEN                                              */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"
#include "eQF.h"

namespace STORE
{
  class XF_FLATTEN : public XF_TRANSFORM
  {
  protected:
   eQF m_eFlatten;

  public:

    XF_FLATTEN(eCN eClass, CardIter* pCI);
   ~XF_FLATTEN(void) noexcept(false);

  protected:
    void vFold_subQuery(CardIter* pCI, Stz* szFoldNm, eCK eCardWithSQ);

    void vFold_from(Stz* szFoldNm, CardIter* pCI);

    void vSet_carry(CardIter* pCI, eCK eCardWithSQ, eQO eWhatOp);

    void vSmooth(CardIter* pCI);

    void vInto_KEYnotEXISTS(CardIter* pCI);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
