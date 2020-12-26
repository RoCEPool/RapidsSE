/*============================================================================*/
/* Region.cpp -- Region                                                       */
/*============================================================================*/

#include "Resources.h"


extern Header*    gaSeShmRegion;

/*----------------------------------------------------------------------------*/
/* attach to an existing shm region                                           */
/*----------------------------------------------------------------------------*/
bool Region::bAttach(s32& sRegionName, bool bVerbose, void* aRegionAddr)
{
  int iFD;
  SysCall oSysCall;
  bool bResult;
  
  // open existing shm region
  bResult = ShmLib::bOpenExisting(iFD,sRegionName,oSysCall);
  if ( ! bResult ) {
    vTrace("Engine::vAttach: failed to open region: %s\n"_Stz,
           sRegionName.azChar());
    return false;
  }
  
  // allocate a temporary extent of memory
  gaSeShmRegion = (Header*)ShmLib::aMap(iFD,
                                     aRegionAddr,
                                     10_MB,
                                     C_NO_HUGE_PAGES,
                                     oSysCall);
  if (! gaSeShmRegion)  {
    vTrace("Engine::vAttach failed to map\n"_Stz);
    ShmLib::bClose(iFD,oSysCall);
    return false;
  }
  
  // overlay attach map onto 2nd buffer (buffer #1)
  Header* aAttachMap =
      pAdd<Header*>((Object*)gaSeShmRegion,C_SIZE_POOL_BUFFER);
  
  // temp set
  AM::vAttachMap(aAttachMap);
  
  // get the actual size
  i8 iRegionSize = AM::iRegionSize();
  AM::vAttachMap(nullptr);

  // close the temporary mapping
  bResult = ShmLib::bClose(iFD,oSysCall);
  bResult = ShmLib::bUnmap(gaSeShmRegion,10_MB,oSysCall);
  
  //--------------------------------------------------------------------------
  
  // reopen the existing shm region
  bResult = ShmLib::bOpenExisting(iFD,sRegionName,oSysCall);
  if ( ! bResult ) {
    vTrace("Engine::vAttach: failed to open region: %s\n"_Stz,
           sRegionName.azChar());
    return false;
  }
  
  // map the full extent of memory required
  gaSeShmRegion = (Header*)ShmLib::aMap(iFD,
                                     aRegionAddr,
                                     iRegionSize,
                                     C_NO_HUGE_PAGES,
                                     oSysCall);
  if (! gaSeShmRegion)  {
    vTrace("Engine::vAttach failed to map\n"_Stz);
    ShmLib::bClose(iFD,oSysCall);
    return false;
  }
  
  // overlay buffer pool onto 1st buffer (buffer #0)
  gpBufferPool = reinterpret_cast<BufferPool*>(gaSeShmRegion);
  
  if (bVerbose)
    vTrace("ShmLib mapped at %p\n"_Stz,gaSeShmRegion);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create new shm region                                                      */
/*----------------------------------------------------------------------------*/
void Region::vCreate(s32& sRegionName,  bool bVerbose,
                     void* aRegionAddr, i8 iRegionSize,
                     bool bTouch,       bool bProgress,
                     bool bLock,        bool bRestart,
                     bool bDontFree,    bool bFreeCB)
{
  int iRegionFd;
  SysCall oSysCall;
  s32 sSizeMag;
  bool bResult;
  
  Region::vUnlink(sRegionName);
  
  // verbose
  MagLib::vByteMag(iRegionSize, sSizeMag);
  if (bVerbose)
    vInfo("Creating Engine instance %s; size %s (%lldB)...\n"_Stz,
           sRegionName.azChar(),
           sSizeMag.azChar(),
           iRegionSize);
  
  // open a new shm region which will contain the activity map
  bResult = ShmLib::bOpenCreate(iRegionFd,sRegionName,oSysCall);
  if (! bResult)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Engine::vCreate: failed to create region %s; errno %lld %s\n"_Stz,
           sRegionName.azChar(),
           oSysCall.iError(),
           strerror((int)oSysCall.iError()));

  // extend the shm region
  bResult = ShmLib::bExtend(iRegionFd,iRegionSize,oSysCall);
  if (! bResult)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Engine::vCreate: failed to extend region %s\n"_Stz,
           sRegionName.azChar());
  
  // map the shm region into a vm segment within this process address space
  gaSeShmRegion = (Header*)ShmLib::aMap(iRegionFd,
                                     aRegionAddr,
                                     iRegionSize,
                                     C_NO_HUGE_PAGES,
                                     oSysCall);
  if (! gaSeShmRegion)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Engine::vCreate failed to map region!\n"_Stz);
  
  gpBufferPool = new(gaSeShmRegion) BufferPool(sRegionName, iRegionSize,
                                               bTouch, bProgress,
                                               bDontFree, bFreeCB);

  
  if (bVerbose)
    vTrace("ShmLib mapped at %p\n"_Stz,gaSeShmRegion);
  
  // lock the shared memory in
  if (bLock) {
    bResult = ShmLib::bLock(gaSeShmRegion,iRegionSize,oSysCall);
    if (! bResult)
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "Engine::vAttach failed to lock\n"_Stz);
    
    if (bVerbose)
      vTrace("ShmLib pages locked in memory\n"_Stz);
  }
  
}

/*----------------------------------------------------------------------------*/
/* test for region                                                            */
/*----------------------------------------------------------------------------*/
bool Region::bTest(s32& sRegionName)
{
  int iFD;
  SysCall oSysCall;
  
  // open existing shm region
  bool bResult = ShmLib::bOpenExisting(iFD,sRegionName,oSysCall);
  if ( ! bResult )
    return false;
  
  bResult = ShmLib::bClose(iFD,oSysCall);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* unlink region                                                              */
/*----------------------------------------------------------------------------*/
void Region::vUnlink(s32& sRegionName)
{
  SysCall oSysCall;
  
  if (! ShmLib::bUnlink(sRegionName,oSysCall))
    vTrace("Region %s not unlinked\n"_Stz,sRegionName.azChar());
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* attempt to open a pool                                                     */
/*----------------------------------------------------------------------------*/
void* Region::aOpen(s32& sRegionName, void* aRegionAddr, int& iFD)
{
  SysCall oSysCall;
  bool bResult;
  
  //if (gxArgs.m_bVerbose)
  //  vInfo("Region::pOpen %s\n"_Stz,sRegionName.azChar());
  
  // open shm region
  bResult = ShmLib::bOpenExisting(iFD,sRegionName,oSysCall);
  if ( ! bResult ) {
    vTrace("Region::pOpen: failed to open region: %s\n"_Stz,
           sRegionName.azChar());
    return nullptr;
  }
  
  // allocate a temporary extent of memory
  void* aRegion = ShmLib::aMap(iFD,
                            aRegionAddr,
                            10_MB,
                            C_NO_HUGE_PAGES,
                            oSysCall);
  if ( ! aRegion )  {
    vTrace("Region::pOpen failed to map\n"_Stz);
    ShmLib::bClose(iFD,oSysCall);
    return nullptr;
  }
  
  return aRegion;
}

/*----------------------------------------------------------------------------*/
/* close region                                                               */
/*----------------------------------------------------------------------------*/
void Region::vClose(void* aRegionAddr, int iFD)
{
  bool bResult;
  SysCall oSysCall;
  
  // close the temporary mapping
  bResult = ShmLib::bClose(iFD,oSysCall);
  bResult = ShmLib::bUnmap(aRegionAddr,10_MB,oSysCall);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
