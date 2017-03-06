/*============================================================================*/
/* Region.hpp -- Region                                                       */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class Region
  {
  public:
    static bool bAttach(StrVbl& sRegionName);
    static void vCreate(StrVbl& sRegionName);
    static bool bTest(StrVbl& sRegionName);
    static void vUnlink(StrVbl& sRegionName);
    
    ////////////////////////////////////////////////////////////////////////////

    static void* pOpen(StrVbl& sRegionName, int& iFD);
    static void  vClose(void* pRegion, int iFD);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
