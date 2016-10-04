/*============================================================================*/
/* Region.hpp -- Region                                                       */
/*============================================================================*/

#pragma once

#include "StorageEngine.h"

namespace StorageEngine
{
  class Region
  {
  public:
    static void vAttach(void);
    static void vCreate(void);
    static bool bTest(void);
    static void vUnlink(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/