/*============================================================================*/
/* XF_PROLOG.hpp -- PROLOG                                                    */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_PROLOG : public XF_TRANSFORM
  {
  private:

  public:

    XF_PROLOG(eCN eClass, CardIter* pCI);
   ~XF_PROLOG(void) noexcept(false);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
