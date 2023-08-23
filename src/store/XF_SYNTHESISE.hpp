/*============================================================================*/
/* XF_PROLOG.hpp -- PROLOG                                                    */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_SYNTHESISE : public XF_TRANSFORM
  {
  private:

  public:

    XF_SYNTHESISE(eCN eClass, CardIter* pCI);
   ~XF_SYNTHESISE(void) noexcept(false);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
