/*============================================================================*/
/* Region.hpp -- Region                                                       */
/*============================================================================*/
#pragma once

namespace Pool
{
  class Region
  {
  public:
    static bool bAttach(s32& sRegionName, void *aRegionAddr);
    static void vCreate(s32& sRegionName, void *aRegionAddr, i8 iRegionSize,
                        i8 iJumboTook, bool bTouch, bool bLock, bool bRestart,
                        bool bDontFree, bool bFreeRB);
    static bool bTest(s32& sRegionName);
    static void vUnlink(s32& sRegionName);
    
    ////////////////////////////////////////////////////////////////////////////

    static void* aOpen(s32& sRegionName, void *aRegionAddr, int& iFD);
    static void  vClose(void* aRegion, int iFD);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
