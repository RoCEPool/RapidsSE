/*============================================================================*/
/* Region.hpp -- Region                                                       */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class Region
  {
  public:
    static bool bAttach(StrVal& sRegionName);
    static void vCreate(StrVal& sRegionName, i8 iRegionSize);
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
