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
bool Region::bAttach(s& sRegionName)
{
  int iFD;
  SysCall xSysCall;
  bool bResult;
  
  if (gxArguments.m_bWelcome)
    vTrace("Attaching to SE instance %s\n",sRegionName.a());
  
  // open existing shm region
  bResult = Shm::bAttach(sRegionName,iFD,xSysCall);
  if ( ! bResult ) {
    vTrace("RapidsSE::vAttach: failed to open region: %s\n",
           sRegionName.a());
    return false;
  }
  
  // allocate a temporary extent of memory
  mpRegion = Shm::pMap(iFD,C_ADDR_POOL,10_MB,C_SHARED_REGION,
                       gxArguments.m_bHugePages,xSysCall);
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vAttach failed to map\n");
    Shm::bClose(iFD,xSysCall);
    return false;
  }
  
  // overlay attach map onto 2nd buffer (buffer #1)
  gpAttachMap = reinterpret_cast<AttachMap*>(pcAdd(mpRegion,C_SIZE_POOL_BUFFER));
  
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
           sRegionName.a());
    return false;
  }
  
  // allocate the full extent of memory required
  mpRegion = Shm::pMap(iFD,C_ADDR_POOL,iRegionSize,C_SHARED_REGION,
                       gxArguments.m_bHugePages,xSysCall);
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
void Region::vCreate(void)
{
  int iFD;
  SysCall xSysCall;
  s256 sSize;
  
  Magnitudes::vMagnitude(gxArguments.m_iRegionSize, sSize.a());
  
  vTrace("Creating RapidsSE instance %s; size %s (%lldB)...\n",
         gxArguments.m_sRegionName.a(),
         sSize.a(),
         gxArguments.m_iRegionSize);
  
  // open a new shm region
  bool bResult = Shm::bCreate(gxArguments.m_sRegionName,
                              gxArguments.m_iRegionSize,
                              iFD,
                              gxArguments.m_bRestart,
                              xSysCall);
  if ( ! bResult ) {
    vTrace("RapidsSE::vCreate: failed to create region %s\n",
           gxArguments.m_sRegionName.a());
    ASSERT(0);
  }
  
  // allocate memory for the shm region
  mpRegion = Shm::pMap(iFD,
                       C_ADDR_POOL,
                       gxArguments.m_iRegionSize,
                       true,
                       gxArguments.m_bHugePages,
                       xSysCall);
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vCreate failed to map!\n");
    Shm::bClose(iFD,xSysCall);
    ASSERT(0);
  }
  
  gpBufferPool = new(mpRegion) BufferPool;
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm mapped at %p\n",mpRegion);
}

/*----------------------------------------------------------------------------*/
/* test for region                                                            */
/*----------------------------------------------------------------------------*/
bool Region::bTest(void)
{
  int iFD;
  SysCall xSysCall;
  
  // open existing shm region
  bool bResult = Shm::bAttach(gxArguments.m_sRegionName,iFD,xSysCall);
  if ( ! bResult )
    return false;
  
  bResult = Shm::bClose(iFD,xSysCall);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* unlink region                                                              */
/*----------------------------------------------------------------------------*/
void Region::vUnlink(void)
{
  SysCall xSysCall;
  
  if (Shm::bUnlink(gxArguments.m_sRegionName,xSysCall))
    vTrace("Region %s succesfully unlinked\n",gxArguments.m_sRegionName.a());
  else
    vTrace("Region %s not unlinked\n",gxArguments.m_sRegionName.a());
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
