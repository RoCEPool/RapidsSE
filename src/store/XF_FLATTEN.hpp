/*============================================================================*/
/* XF_FLATTEN.hpp -- FLATTEN                                              */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_FLATTEN : public XF_TRANSFORM
  {
  public:

    XF_FLATTEN(eCN eClass, CardIter* pCI);
   ~XF_FLATTEN(void) noexcept(false);

  protected:
    void vFold_subQuery(CardIter* pCI, Stz* szFoldNm, eCK eCardWithSQ);

    void vFold_from(Stz* szFoldNm, CardIter* pCI);

    void vSet_smooth(CardIter* pCI, eCK eCardWithSQ, eQO eWhatOp);

    void vSmooth(CardIter* pCI);

    static i8 nTables_In_SubQuery(CardIter* pCI, Stz* szFold, eCK eCardWithSQ);

    void vInto_KEYnotEXISTS(CardIter* pCI);

    void vRotate_Next(void);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
