/*============================================================================*/
/* V - value helper                                                           */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Values
{
  class V
  {
  private:

  public:
    static void vCopyDatum(eCN eClass, void* pDst, void* pSrc);
    static void vCopyIntDatum(void* pDst, void* pSrc);
    static void vCopyStrDatum(void* pDst, void* pSrc);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
