/*============================================================================*/
/* Region.cpp -- Region                                                       */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;
extern Header*    gaSeShmRegion;

extern Arguments gxArguments;

//extern void* C_ADDR_POOL;

/*----------------------------------------------------------------------------*/
/* attach to an existing shm region                                           */
/*----------------------------------------------------------------------------*/
bool Region::bAttach(s32& sRegionName, bool bVerbose, void* aRegionAddr)
{
  int iFD;
  SysCall oSysCall;
  bool bResult;
  
  // open existing shm region
  bResult = Shm::bOpenExisting(sRegionName,iFD,oSysCall);
  if ( ! bResult ) {
    vTrace("Engine::vAttach: failed to open region: %s\n"_Stz,
           sRegionName.aChar());
    return false;
  }
  
  // allocate a temporary extent of memory
  gaSeShmRegion = (Header*)Shm::aMap(iFD,
                                     aRegionAddr,
                                     10_MB,
                                     C_NO_HUGE_PAGES,
                                     oSysCall);
  if (! gaSeShmRegion)  {
    vTrace("Engine::vAttach failed to map\n"_Stz);
    Shm::bClose(iFD,oSysCall);
    return false;
  }
  
  // overlay attach map onto 2nd buffer (buffer #1)
  gpAttachMap = pAdd<AttachMap*>((Object*)gaSeShmRegion,C_SIZE_POOL_BUFFER);
  
  // get the actual size
  i8 iRegionSize = gpAttachMap->Arg.m_iRegionSize;
  gpAttachMap = nullptr;
  
  // close the temporary mapping
  bResult = Shm::bClose(iFD,oSysCall);
  bResult = Shm::bUnmap(gaSeShmRegion,10_MB,oSysCall);
  
  //--------------------------------------------------------------------------
  
  // reopen the existing shm region
  bResult = Shm::bOpenExisting(sRegionName,iFD,oSysCall);
  if ( ! bResult ) {
    vTrace("Engine::vAttach: failed to open region: %s\n"_Stz,
           sRegionName.aChar());
    return false;
  }
  
  // map the full extent of memory required
  gaSeShmRegion = (Header*)Shm::aMap(iFD,
                                     aRegionAddr,
                                     iRegionSize,
                                     C_NO_HUGE_PAGES,
                                     oSysCall);
  if (! gaSeShmRegion)  {
    vTrace("Engine::vAttach failed to map\n"_Stz);
    Shm::bClose(iFD,oSysCall);
    return false;
  }
  
  // overlay buffer pool onto 1st buffer (buffer #0)
  gpBufferPool = reinterpret_cast<BufferPool*>(gaSeShmRegion);
  
  if (bVerbose)
    vTrace("Shm mapped at %p\n"_Stz,gaSeShmRegion);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create new shm region                                                      */
/*----------------------------------------------------------------------------*/
void Region::vCreate(s32& sRegionName, bool bVerbose,
                     void* aRegionAddr,
                     i8 iRegionSize, i8 iJumboTook,
                     bool bTouch, bool bProgress,
                     bool bLock, bool bRestart,
                     bool bDontFree, bool bFreeRB)
{
  int iRegionFd;
  SysCall oSysCall;
  s32 sSizeMag;
  bool bResult;
  
  // verbose
  Magnitudes::vByteMag(iRegionSize, sSizeMag);
  if (bVerbose)
    vInfo("Creating Engine instance %s; size %s (%lldB)...\n"_Stz,
           sRegionName.aChar(),
           sSizeMag.aChar(),
           iRegionSize);
  
  // open a new shm region which will contain the activity map
  bResult = Shm::bOpenCreate(sRegionName,iRegionFd,oSysCall);
  if (! bResult)
    vFatal("Engine::vCreate: failed to create region %s\n"_Stz,
           sRegionName.aChar());

  // extend the shm region
  bResult = Shm::bExtend(iRegionFd,iRegionSize,oSysCall);
  if (! bResult)
    vFatal("Engine::vCreate: failed to extend region %s\n"_Stz,
           sRegionName.aChar());
  
  // map the shm region into a vm segment within this process address space
  gaSeShmRegion = (Header*)Shm::aMap(iRegionFd,
                                     aRegionAddr,
                                     iRegionSize,
                                     C_NO_HUGE_PAGES,
                                     oSysCall);
  if (! gaSeShmRegion)
    vFatal("Engine::vCreate failed to map region!\n"_Stz);
  
  gpBufferPool = new(gaSeShmRegion) BufferPool(sRegionName, iRegionSize,
                                               iJumboTook, bTouch, bProgress,
                                               bDontFree, bFreeRB);

  
  if (bVerbose)
    vTrace("Shm mapped at %p\n"_Stz,gaSeShmRegion);
  
  // lock the shared memory in
  if (bLock) {
    bResult = Shm::bLock(gaSeShmRegion,iRegionSize,oSysCall);
    if (! bResult)
      vFatal("Engine::vAttach failed to lock\n"_Stz);
    
    if (bVerbose)
      vTrace("Shm pages locked in memory\n"_Stz);
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
  bool bResult = Shm::bOpenExisting(sRegionName,iFD,oSysCall);
  if ( ! bResult )
    return false;
  
  bResult = Shm::bClose(iFD,oSysCall);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* unlink region                                                              */
/*----------------------------------------------------------------------------*/
void Region::vUnlink(s32& sRegionName)
{
  SysCall oSysCall;
  
  if (! Shm::bUnlink(sRegionName,oSysCall))
    vTrace("Region %s not unlinked\n"_Stz,sRegionName.aChar());
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* attempt to open a pool                                                     */
/*----------------------------------------------------------------------------*/
void* Region::aOpen(s32& sRegionName, void* aRegionAddr, int& iFD)
{
  SysCall oSysCall;
  bool bResult;
  
  //if (gxArguments.m_bVerbose)
  //  vInfo("Region::pOpen %s\n"_Stz,sRegionName.aChar());
  
  // open shm region
  bResult = Shm::bOpenExisting(sRegionName,iFD,oSysCall);
  if ( ! bResult ) {
    vTrace("Region::pOpen: failed to open region: %s\n"_Stz,
           sRegionName.aChar());
    return nullptr;
  }
  
  // allocate a temporary extent of memory
  void* aRegion = Shm::aMap(iFD,
                            aRegionAddr,
                            10_MB,
                            C_NO_HUGE_PAGES,
                            oSysCall);
  if ( ! aRegion )  {
    vTrace("Region::pOpen failed to map\n"_Stz);
    Shm::bClose(iFD,oSysCall);
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
  bResult = Shm::bClose(iFD,oSysCall);
  bResult = Shm::bUnmap(aRegionAddr,10_MB,oSysCall);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
