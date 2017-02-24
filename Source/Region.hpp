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
    static bool bAttach(StrVal& sRegionName);
    static void vCreate(StrVal& sRegionName);
    static bool bTest(StrVal& sRegionName);
    static void vUnlink(StrVal& sRegionName);
    
    ////////////////////////////////////////////////////////////////////////////

    static void* pOpen(StrVal& sRegionName, int& iFD);
    static void  vClose(void* pRegion, int iFD);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
