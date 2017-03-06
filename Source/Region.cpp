/*============================================================================*/
/* Region.cpp -- Region                                                       */
/*============================================================================*/

#include "StorageEngine.h"

extern Arguments  gxArguments;
extern AttachMap* gpAttachMap;
extern void*      mpRegion;

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
    vTrace("Attaching to SE instance %s\n",sRegionName.aStr());
  
  // open existing shm region
  bResult = Shm::bAttach(sRegionName,iFD,xSysCall);
  if ( ! bResult ) {
    vTrace("RapidsSE::vAttach: failed to open region: %s\n",
           sRegionName.aStr());
    return false;
  }
  
  // allocate a temporary extent of memory
  mpRegion = Shm::pMap(iFD,
                       gxArguments.m_pRegionAddr,
                       10_MB,
                       C_SHARED_REGION,
                       gxArguments.m_bHugePages,
                       xSysCall);
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vAttach failed to map\n");
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
    vTrace("RapidsSE::vAttach: failed to open region: %s\n",
           sRegionName.aStr());
    return false;
  }
  
  // allocate the full extent of memory required
  mpRegion = Shm::pMap(iFD,
                      gxArguments.m_pRegionAddr,
                       iRegionSize,
                       C_SHARED_REGION,
                       gxArguments.m_bHugePages,
                       xSysCall);
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vAttach failed to map\n");
    Shm::bClose(iFD,xSysCall);
    return false;
  }
  
  // overlay buffer pool onto 1st buffer (buffer #0)
  gpBufferPool = reinterpret_cast<BufferPool*>(mpRegion);
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm mapped at %p\n",mpRegion);
  
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
  
  Magnitudes::vMagnitude(gxArguments.m_iRegionSize, sSize.aStr());

  vTrace("Creating RapidsSE instance %s; size %s (%lldB)...\n",
         sRegionName.aStr(),
         sSize.aStr(),
         gxArguments.m_iRegionSize);
  
  if (gxArguments.m_bVerbose)
    vTrace("Region::vCreate A\n");
  
  // open a new shm region
  bool bResult = Shm::bCreate(sRegionName,
                              gxArguments.m_iRegionSize,
                              iFD,
                              gxArguments.m_bRestart,
                              xSysCall);

  if (gxArguments.m_bVerbose)
    vTrace("Region::vCreate B\n");
  
  if ( ! bResult ) {
    vTrace("RapidsSE::vCreate: failed to create region %s\n",
           sRegionName.aStr());
    ASSERT(0);
  }
  
  // allocate memory for the shm region
  mpRegion = Shm::pMap(iFD,
                       gxArguments.m_pRegionAddr,
                       gxArguments.m_iRegionSize,
                       true,
                       gxArguments.m_bHugePages,
                       xSysCall);
  
  if (gxArguments.m_bVerbose)
    vTrace("Region::vCreate C; mpRegion=%p\n",mpRegion);
  
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vCreate failed to map!\n");
    Shm::bClose(iFD,xSysCall);
    ASSERT(0);
  }
  
  if (gxArguments.m_bVerbose)
    vTrace("Region::vCreate D\n");
  
  gpBufferPool = new(mpRegion) BufferPool;

  if (gxArguments.m_bVerbose)
    vTrace("Region::vCreate E\n");
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm mapped at %p\n",mpRegion);
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
    vTrace("Region %s succesfully unlinked\n",sRegionName.aStr());
  else
    vTrace("Region %s not unlinked\n",sRegionName.aStr());
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* attempt to open a pool                                                     */
/*----------------------------------------------------------------------------*/
void* Region::pOpen(StrVal& sRegionName, int& iFD)
{
  SysCall xSysCall;
  bool bResult;
  
  vTrace("Region::pOpen %s\n",sRegionName.aStr());
  
  // open shm region
  bResult = Shm::bAttach(sRegionName,iFD,xSysCall);
  if ( ! bResult ) {
    vTrace("Region::pOpen: failed to open region: %s\n",
           sRegionName.aStr());
    return nullptr;
  }
  
  // allocate a temporary extent of memory
  void* pRegion = Shm::pMap(iFD,
                            gxArguments.m_pRegionAddr,
                            10_MB,
                            C_SHARED_REGION,
                            gxArguments.m_bHugePages,
                            xSysCall);
  if ( ! pRegion )  {
    vTrace("Region::pOpen failed to map\n");
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
