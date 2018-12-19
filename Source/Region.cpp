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
bool Region::bAttach(StrVal& sRegionName, bool bWecome)
{
  int iFD;
  SysCall xSysCall;
  bool bResult;
  
  if (bWecome)
    vTrace("Attaching to SE instance %s\n"_Stz,sRegionName.aChar());
  
  // open existing shm region
  bResult = Shm::bAttach(sRegionName,iFD,xSysCall);
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
                       xSysCall);
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vAttach failed to map\n"_Stz);
    Shm::bClose(iFD,xSysCall);
    return false;
  }
  
  // overlay attach map onto 2nd buffer (buffer #1)
  gpAttachMap = pAdd<AttachMap*>(mpRegion,C_SIZE_POOL_BUFFER);
  
  // get the actual size
  i8 iRegionSize = gpAttachMap->m_iRegionSize;
  gpAttachMap = nullptr;
  
  // close the temporary mapping
  bResult = Shm::bClose(iFD,xSysCall);
  bResult = Shm::bUnmap(mpRegion,10_MB,xSysCall);
  
  //--------------------------------------------------------------------------
  
  // reopen the existing shm region
  bResult = Shm::bAttach(sRegionName,iFD,xSysCall);
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
                       xSysCall);
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vAttach failed to map\n"_Stz);
    Shm::bClose(iFD,xSysCall);
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
void Region::vCreate(StrVal& sRegionName, i8 iRegionSize, i8 iJumboTook,
                     bool bTouch, bool bLock, bool bRestart,
                     bool bDontFree, bool bFreeRB)
{
  int iFD;
  SysCall xSysCall;
  s256 sSize;
  
  Magnitudes::vByteMag(iRegionSize, sSize);

  if (gxArguments.m_bVerbose)
    vInfo("Creating RapidsSE instance %s; size %s (%lldB)...\n"_Stz,
           sRegionName.aChar(),
           sSize.aChar(),
           iRegionSize);
  
  //if (gxArguments.m_bVerbose)
  //  vTrace("Region::vCreate A\n"_Stz);
  
  // open a new shm region
  bool bResult = Shm::bCreate(sRegionName,
                              iRegionSize,
                              iFD,
                              bRestart,
                              xSysCall);

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
                       xSysCall);
  
  //if (gxArguments.m_bVerbose)
  //  vTrace("Region::vCreate C; mpRegion=%p\n"_Stz,mpRegion);
  
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vCreate failed to map!\n"_Stz);
    Shm::bClose(iFD,xSysCall);
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
                         xSysCall);
  
    if ( ! bResult )  {
      vFatal("RapidsSE::vAttach failed to lock\n"_Stz);
      bResult = Shm::bUnmap(mpRegion, iRegionSize, xSysCall);
      if ( ! bResult )  {
        vFatal("RapidsSE::vAttach failed to unmap\n"_Stz);
      }
      Shm::bClose(iFD,xSysCall);
      exit(EXIT_FAILURE);
    }
    
    if (gxArguments.m_bVerbose)
      vTrace("Shm pages locked in memory\n"_Stz);
  }
  
}

/*----------------------------------------------------------------------------*/
/* test for region                                                            */
/*----------------------------------------------------------------------------*/
bool Region::bTest(StrVal& sRegionName)
{
  int iFD;
  SysCall xSysCall;
  
  // open existing shm region
  bool bResult = Shm::bAttach(sRegionName,iFD,xSysCall);
  if ( ! bResult )
    return false;
  
  bResult = Shm::bClose(iFD,xSysCall);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* unlink region                                                              */
/*----------------------------------------------------------------------------*/
void Region::vUnlink(StrVal& sRegionName)
{
  SysCall xSysCall;
  
  if (Shm::bUnlink(sRegionName,xSysCall)) {
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
void* Region::pOpen(StrVal& sRegionName, int& iFD)
{
  SysCall xSysCall;
  bool bResult;
  
  //if (gxArguments.m_bVerbose)
  //  vInfo("Region::pOpen %s\n"_Stz,sRegionName.aChar());
  
  // open shm region
  bResult = Shm::bAttach(sRegionName,iFD,xSysCall);
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
                            xSysCall);
  if ( ! pRegion )  {
    vTrace("Region::pOpen failed to map\n"_Stz);
    Shm::bClose(iFD,xSysCall);
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
  SysCall xSysCall;
  
  // close the temporary mapping
  bResult = Shm::bClose(iFD,xSysCall);
  bResult = Shm::bUnmap(pRegion,10_MB,xSysCall);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
