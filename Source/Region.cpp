/*============================================================================*/
/* Region.cpp -- Region                                                       */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;
extern void*      mpRegion;

extern Arguments gxArguments;

extern void* C_ADDR_POOL;

/*----------------------------------------------------------------------------*/
/* attach to an existing shm region                                           */
/*----------------------------------------------------------------------------*/
bool Region::bAttach(s32& sRegionName, bool bWecome)
{
  int iFD;
  SysCall oSysCall;
  bool bResult;
  
  if (bWecome)
    vTrace("Attaching to SE instance %s\n"_Stz,sRegionName.aChar());
  
  // open existing shm region
  bResult = Shm::bAttach(sRegionName,iFD,oSysCall);
  if ( ! bResult ) {
    vTrace("RapidsSE::vAttach: failed to open region: %s\n"_Stz,
           sRegionName.aChar());
    return false;
  }
  
  // allocate a temporary extent of memory
  mpRegion = Shm::aMap(iFD,
                       gxArguments.m_aRegionAddr,
                       10_MB,
                       C_SHARED_REGION,
                       false,
                       oSysCall);
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vAttach failed to map\n"_Stz);
    Shm::bClose(iFD,oSysCall);
    return false;
  }
  
  // overlay attach map onto 2nd buffer (buffer #1)
  gpAttachMap = pAdd<AttachMap*>((Object*)mpRegion,C_SIZE_POOL_BUFFER);
  
  // get the actual size
  i8 iRegionSize = gpAttachMap->m_iRegionSize;
  gpAttachMap = nullptr;
  
  // close the temporary mapping
  bResult = Shm::bClose(iFD,oSysCall);
  bResult = Shm::bUnmap(mpRegion,10_MB,oSysCall);
  
  //--------------------------------------------------------------------------
  
  // reopen the existing shm region
  bResult = Shm::bAttach(sRegionName,iFD,oSysCall);
  if ( ! bResult ) {
    vTrace("RapidsSE::vAttach: failed to open region: %s\n"_Stz,
           sRegionName.aChar());
    return false;
  }
  
  // allocate the full extent of memory required
  mpRegion = Shm::aMap(iFD,
                       gxArguments.m_aRegionAddr,
                       iRegionSize,
                       C_SHARED_REGION,
                       false,
                       oSysCall);
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vAttach failed to map\n"_Stz);
    Shm::bClose(iFD,oSysCall);
    return false;
  }
  
  // overlay buffer pool onto 1st buffer (buffer #0)
  gpBufferPool = reinterpret_cast<BufferPool*>(mpRegion);
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm mapped at %p\n"_Stz,mpRegion);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create new shm region                                                      */
/*----------------------------------------------------------------------------*/
void Region::vCreate(s32& sRegionName, i8 iRegionSize, i8 iJumboTook,
                     bool bTouch, bool bLock, bool bRestart,
                     bool bDontFree, bool bFreeRB)
{
  int iFD;
  SysCall oSysCall;
  s32 sSizeMag;
  
  Magnitudes::vByteMag(iRegionSize, sSizeMag);

  if (gxArguments.m_bVerbose)
    vInfo("Creating RapidsSE instance %s; size %s (%lldB)...\n"_Stz,
           sRegionName.aChar(),
           sSizeMag.aChar(),
           iRegionSize);
  
  //if (gxArguments.m_bVerbose)
  //  vTrace("Region::vCreate A\n"_Stz);
  
  // open a new shm region
  bool bResult = Shm::bCreate(sRegionName,
                              iRegionSize,
                              iFD,
                              bRestart,
                              oSysCall);

  //if (gxArguments.m_bVerbose)
  //  vTrace("Region::vCreate B\n"_Stz);
  
  if ( ! bResult ) {
    vTrace("RapidsSE::vCreate: failed to create region %s\n"_Stz,
           sRegionName.aChar());
    NEVER_REACH;
  }
  
  // allocate memory for the shm region
  mpRegion = Shm::aMap(iFD,
                       gxArguments.m_aRegionAddr,
                       iRegionSize,
                       true,
                       false,
                       oSysCall);
  
  //if (gxArguments.m_bVerbose)
  //  vTrace("Region::vCreate C; mpRegion=%p\n"_Stz,mpRegion);
  
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vCreate failed to map!\n"_Stz);
    Shm::bClose(iFD,oSysCall);
    NEVER_REACH;
  }
  
  //if (gxArguments.m_bVerbose)
  //  vTrace("Region::vCreate D\n"_Stz);
  
  //vInfo("sleeping... \n"_Stz);
  //PosixThread::vSleepMilli(180000);
  
  gpBufferPool = new(mpRegion) BufferPool(sRegionName, iRegionSize,
                                          iJumboTook, bTouch, bDontFree, bFreeRB);

  //if (gxArguments.m_bVerbose)
  //  vTrace("Region::vCreate E\n"_Stz);
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm mapped at %p\n"_Stz,mpRegion);
  
  // lock the shared memory in
  if (bLock) {
    bResult = Shm::bLock(mpRegion,
                         iRegionSize,
                         oSysCall);
  
    if ( ! bResult )  {
      vFatal("RapidsSE::vAttach failed to lock\n"_Stz);
      bResult = Shm::bUnmap(mpRegion, iRegionSize, oSysCall);
      if ( ! bResult )  {
        vFatal("RapidsSE::vAttach failed to unmap\n"_Stz);
      }
      Shm::bClose(iFD,oSysCall);
      exit(EXIT_FAILURE);
    }
    
    if (gxArguments.m_bVerbose)
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
  bool bResult = Shm::bAttach(sRegionName,iFD,oSysCall);
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
  
  if (Shm::bUnlink(sRegionName,oSysCall)) {
    if (gxArguments.m_bVerbose)
      vTrace("Region %s succesfully unlinked\n"_Stz,sRegionName.aChar());
  }
  else {
    vTrace("Region %s not unlinked\n"_Stz,sRegionName.aChar());
  }
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* attempt to open a pool                                                     */
/*----------------------------------------------------------------------------*/
void* Region::pOpen(s32& sRegionName, int& iFD)
{
  SysCall oSysCall;
  bool bResult;
  
  //if (gxArguments.m_bVerbose)
  //  vInfo("Region::pOpen %s\n"_Stz,sRegionName.aChar());
  
  // open shm region
  bResult = Shm::bAttach(sRegionName,iFD,oSysCall);
  if ( ! bResult ) {
    vTrace("Region::pOpen: failed to open region: %s\n"_Stz,
           sRegionName.aChar());
    return nullptr;
  }
  
  // allocate a temporary extent of memory
  void* pRegion = Shm::aMap(iFD,
                            gxArguments.m_aRegionAddr,
                            10_MB,
                            C_SHARED_REGION,
                            false,
                            oSysCall);
  if ( ! pRegion )  {
    vTrace("Region::pOpen failed to map\n"_Stz);
    Shm::bClose(iFD,oSysCall);
    return nullptr;
  }
  
  return pRegion;
}

/*----------------------------------------------------------------------------*/
/* close region                                                               */
/*----------------------------------------------------------------------------*/
void Region::vClose(void* pRegion, int iFD)
{
  bool bResult;
  SysCall oSysCall;
  
  // close the temporary mapping
  bResult = Shm::bClose(iFD,oSysCall);
  bResult = Shm::bUnmap(pRegion,10_MB,oSysCall);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
