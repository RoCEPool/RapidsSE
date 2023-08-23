/*============================================================================*/
/* XF_PROLOG.hpp -- PROLOG                                                    */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_EPILOG : public XF_TRANSFORM
  {
  private:

  public:

    XF_EPILOG(eCN eClass, CardIter* pCI);
   ~XF_EPILOG(void) noexcept(false);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
