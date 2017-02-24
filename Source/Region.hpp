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
    static bool bAttach(sRefactor& sRegionName);
    static void vCreate(sRefactor& sRegionName);
    static bool bTest(sRefactor& sRegionName);
    static void vUnlink(sRefactor& sRegionName);
    
    ////////////////////////////////////////////////////////////////////////////

    static void* pOpen(sRefactor& sRegionName, int& iFD);
    static void  vClose(void* pRegion, int iFD);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
