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
    static bool bAttach(s& sRegionName);
    static void vCreate(s& sRegionName);
    static bool bTest(s& sRegionName);
    static void vUnlink(s& sRegionName);
    
    ////////////////////////////////////////////////////////////////////////////

    static void* pOpen(s& sRegionName, int& iFD);
    static void  vClose(void* pRegion, int iFD);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
