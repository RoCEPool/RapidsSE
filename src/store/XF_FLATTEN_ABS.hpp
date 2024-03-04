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

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
