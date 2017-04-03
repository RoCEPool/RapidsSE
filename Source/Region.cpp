/*============================================================================*/
/* Region.cpp -- Region                                                       */
/*============================================================================*/

#include "StorageEngine.h"

extern Arguments  gxArguments;
extern AttachMap* gpAttachMap;
extern void*      mpRegion;

extern bool      gbVerbose;

extern void* C_ADDR_POOL;

/*----------------------------------------------------------------------------*/
/* attach to an existing shm region                                           */
/*----------------------------------------------------------------------------*/
bool Region::bAttach(StrVal& sRegionName)
{
  int iFD;
  SysCall xSysCall;
  bool bResult;
  
  if (gxArguments.m_bWelcome)
    vTrace("Attaching to SE instance %s\n"_Str,sRegionName.s());
  
  // open existing shm region
  bResult = Shm::bAttach(sRegionName,iFD,xSysCall);
  if ( ! bResult ) {
    vTrace("RapidsSE::vAttach: failed to open region: %s\n"_Str,
           sRegionName.s());
    return false;
  }
  
  // allocate a temporary extent of memory
  mpRegion = Shm::pMap(iFD,
                       C_ADDR_POOL,
                       10_MB,
                       C_SHARED_REGION,
                       false,
                       xSysCall);
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vAttach failed to map\n"_Str);
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
    vTrace("RapidsSE::vAttach: failed to open region: %s\n"_Str,
           sRegionName.s());
    return false;
  }
  
  // allocate the full extent of memory required
  mpRegion = Shm::pMap(iFD,
                       C_ADDR_POOL,
                       iRegionSize,
                       C_SHARED_REGION,
                       false,
                       xSysCall);
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vAttach failed to map\n"_Str);
    Shm::bClose(iFD,xSysCall);
    return false;
  }
  
  // overlay buffer pool onto 1st buffer (buffer #0)
  gpBufferPool = reinterpret_cast<BufferPool*>(mpRegion);
  
  if (gbVerbose)
    vTrace("Shm mapped at %p\n"_Str,mpRegion);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create new shm region                                                      */
/*----------------------------------------------------------------------------*/
void Region::vCreate(StrVal& sRegionName)
{
  int iFD;
  SysCall xSysCall;
  s256 sSize;
  
  Magnitudes::vMagnitude(gxArguments.m_iRegionSize, sSize);

  vTrace("Creating RapidsSE instance %s; size %s (%lldB)...\n"_Str,
         sRegionName.s(),
         sSize.s(),
         gxArguments.m_iRegionSize);
  
  if (gbVerbose)
    vTrace("Region::vCreate A\n"_Str);
  
  // open a new shm region
  bool bResult = Shm::bCreate(sRegionName,
                              gxArguments.m_iRegionSize,
                              iFD,
                              gxArguments.m_bRestart,
                              xSysCall);

  if (gbVerbose)
    vTrace("Region::vCreate B\n"_Str);
  
  if ( ! bResult ) {
    vTrace("RapidsSE::vCreate: failed to create region %s\n"_Str,
           sRegionName.s());
    ASSERT(0);
  }
  
  // allocate memory for the shm region
  mpRegion = Shm::pMap(iFD,
                       C_ADDR_POOL,
                       gxArguments.m_iRegionSize,
                       true,
                       false,
                       xSysCall);
  
  if (gbVerbose)
    vTrace("Region::vCreate C; mpRegion=%p\n"_Str,mpRegion);
  
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vCreate failed to map!\n"_Str);
    Shm::bClose(iFD,xSysCall);
    ASSERT(0);
  }
  
  if (gbVerbose)
    vTrace("Region::vCreate D\n"_Str);
  
  gpBufferPool = new(mpRegion) BufferPool;

  if (gbVerbose)
    vTrace("Region::vCreate E\n"_Str);
  
  if (gbVerbose)
    vTrace("Shm mapped at %p\n"_Str,mpRegion);
  
  // lock the shared memory in
  if (gxArguments.m_bLock) {
    bResult = Shm::bLock(mpRegion,
                         gxArguments.m_iRegionSize,
                         xSysCall);
  
    if ( ! bResult )  {
      vTrace("RapidsSE::vAttach failed to lock\n"_Str);
      bResult = Shm::bUnmap(mpRegion, gxArguments.m_iRegionSize, xSysCall);
      if ( ! bResult )  {
        vTrace("RapidsSE::vAttach failed to unmap\n"_Str);
      }
      Shm::bClose(iFD,xSysCall);
      exit(EXIT_FAILURE);
    }
  }
  
  if (gbVerbose)
    vTrace("Shm pages locked in memory\n"_Str);
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
  
  if (Shm::bUnlink(sRegionName,xSysCall))
    vTrace("Region %s succesfully unlinked\n"_Str,sRegionName.s());
  else
    vTrace("Region %s not unlinked\n"_Str,sRegionName.s());
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* attempt to open a pool                                                     */
/*----------------------------------------------------------------------------*/
void* Region::pOpen(StrVal& sRegionName, int& iFD)
{
  SysCall xSysCall;
  bool bResult;
  
  vTrace("Region::pOpen %s\n"_Str,sRegionName.s());
  
  // open shm region
  bResult = Shm::bAttach(sRegionName,iFD,xSysCall);
  if ( ! bResult ) {
    vTrace("Region::pOpen: failed to open region: %s\n"_Str,
           sRegionName.s());
    return nullptr;
  }
  
  // allocate a temporary extent of memory
  void* pRegion = Shm::pMap(iFD,
                            C_ADDR_POOL,
                            10_MB,
                            C_SHARED_REGION,
                            false,
                            xSysCall);
  if ( ! pRegion )  {
    vTrace("Region::pOpen failed to map\n"_Str);
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
