/*============================================================================*/
/* Region.hpp -- Region                                                       */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class Region
  {
  public:
    static bool bAttach(StrVal& sRegionName, bool bWecome);
    static void vCreate(StrVal& sRegionName, i8 iRegionSize, i8 iJumboTook,
                        bool bTouch, bool bLock, bool bRestart);
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
