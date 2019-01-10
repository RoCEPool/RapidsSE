/*============================================================================*/
/* Shm.cpp -- shared memory                                                   */
/*============================================================================*/

#include "System.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;
extern Arguments gxArguments;

extern ResourceTracker gxResourceTracker;

/*----------------------------------------------------------------------------*/
/* attach/open to an existing shared memory region                            */
/*----------------------------------------------------------------------------*/
bool Shm::bAttach(s32& sRegionName, int& iFD, SysCall& oSysCall)
{
  oSysCall.vClear();
    
  // open shared memory region and get a fd
  iFD = shm_open(sRegionName.aChar(), O_RDWR, S_IRUSR | S_IWUSR);
  
  if (iFD == -1) {
    oSysCall.vError(errno);
    //vWarn("Shm::bAttach: shm_open() error; errno=%d %s\n"_Stz,errno, strerror(errno));
    return false;
  }
  
  gxResourceTracker.vIncShmOpen();

  return true;
}

/*----------------------------------------------------------------------------*/
/* close an existing memory region                                            */
/*----------------------------------------------------------------------------*/
bool Shm::bClose(int iFD, SysCall& oSysCall)
{
  oSysCall.vClear();
  
  int rCode = close(iFD);
  
  if (0 != rCode) {
    oSysCall.vError(errno);
    vWarn("Shm::bClose: munmap() error; errno=%d %s\n"_Stz,errno,strerror(errno));
    return false;
  }
  
  gxResourceTracker.vDecShmOpen();

  oSysCall.vSuccess();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create/open a new shared memory region                                     */
/*----------------------------------------------------------------------------*/
bool Shm::bCreate(s32& sRegionName, i8 iSize, int& iFD,
                    bool bRecreate, SysCall& oSysCall)
{
  int rCode;
  
  oSysCall.vClear();
  
  //if (gxArguments.m_bVerbose)
  //  vTrace("Shm::bCreate A\n"_Stz);
  
  // unlink the shm region (in case created earlier; this needs work)
  if (bRecreate)
    rCode = shm_unlink(sRegionName.aChar());
  
  // we ignore the error

  //if (gxArguments.m_bVerbose)
  //  vTrace("Shm::bCreate B\n"_Stz);
  
  // open shared memory region and get a fd
  iFD = shm_open(sRegionName.aChar(), O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
  
  //if (gxArguments.m_bVerbose)
  //  vTrace("Shm::bCreate C\n"_Stz);
  
  if (iFD == -1) {
    oSysCall.vError(errno);
    vWarn("Shm::bCreate: shm_open() error; errno=%d %s\n"_Stz,errno,strerror(errno));
    return false;
  }
  
  //if (gxArguments.m_bVerbose)
  //  vTrace("Shm::bCreate D\n"_Stz);
  
  gxResourceTracker.vIncShmOpen();

  // fill with 0's
  rCode = ftruncate(iFD, iSize);
  
  //if (gxArguments.m_bVerbose)
  //  vTrace("Shm::bCreate E\n"_Stz);
  
  if (rCode == -1) {
    
    //if (gxArguments.m_bVerbose)
    //  vTrace("Shm::bCreate F\n"_Stz);
   
    oSysCall.vError(errno);
    vTrace("Shm::bCreate: ftruncate() error; errno=%d %s\n"_Stz,errno,strerror(errno));
    
    int rCode = shm_unlink(sRegionName.aChar());
    
    if (0 != rCode) {
      oSysCall.vError(errno);
      vWarn("Shm::bCreate: shm_unlink() error; errno=%d %s\n"_Stz,errno,strerror(errno));
      return false;
    }
    
    return false;
  }
  
  //if (gxArguments.m_bVerbose)
  //  vTrace("Shm::bCreate G\n"_Stz);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* map a new an anonymous region of memory (private or shared)                */
/*----------------------------------------------------------------------------*/
bool Shm::bLock(void* pAddress, i8 iBytes, SysCall& SysCall)
{
  SysCall.vClear();
  
  if (gpAttachMap->m_bVerbose)
    vTrace("Shm::vLock\n"_Stz);
  
  if (mlock(pAddress,iBytes) != 0) {
    SysCall.vError(errno);
    vWarn("Shm::vLock: mlock() error; errno=%d %s line=%d\n"_Stz,errno,strerror(errno), __LINE__);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* map a new an anonymous region of memory (private or shared)                */
/*----------------------------------------------------------------------------*/
void* Shm::aMap(int iFd, void* aAddress, i8 iBytes, bool bShared,
                bool bHugeTlb, SysCall& SysCall)
{
  SysCall.vClear();
  
  //if (gxArguments.m_bVerbose)
  //  vTrace("Shm::pMap A\n"_Stz);
  
  int iProt = PROT_READ | PROT_WRITE;

  //if (gxArguments.m_bVerbose)
  //  vTrace("Shm::pMap B\n"_Stz);
  
#ifdef __APPLE__
  int iFlags =   (bShared  ? MAP_SHARED : MAP_PRIVATE | MAP_ANONYMOUS)
               | (aAddress ? MAP_FIXED  : 0);
#else
  int iFlags =   (bShared  ? MAP_SHARED : MAP_PRIVATE)
               | (aAddress ? MAP_FIXED  : 0)
               | MAP_NORESERVE
               | (bHugeTlb ? MAP_HUGETLB | MAP_ANONYMOUS : 0);
#endif
  
  //if (gxArguments.m_bVerbose)
  //  vTrace("Shm::pMap C\n"_Stz);
  
  void* pRegion = mmap(aAddress, // starting address
                       iBytes,   // length of the region
                       iProt,    // region protection
                       iFlags,   // mapping flags
                       iFd,      // file descriptor
                       0         // no offset in file
                       );
  
  if (MAP_FAILED == pRegion) {
    SysCall.vError(errno);
    vWarn("Shm::pMap: mmap() error; errno=%d %s line=%d\n"_Stz,errno,strerror(errno), __LINE__);
    return nullptr;
  }
  
  //if (gxArguments.m_bVerbose)
  //  vTrace("Shm::pMap D\n"_Stz);
  
  SysCall.vSuccess();
  
  return pRegion;
}

/*----------------------------------------------------------------------------*/
/* unmap an existing memory region                                            */
/*----------------------------------------------------------------------------*/
bool Shm::bUnmap(void *aAddress, i8 iBytes, SysCall& oSysCall)
{
  oSysCall.vClear();
  
  int rCode = munmap(aAddress, // starting address
                     iBytes    // length of the region
                     );
  
  if (0 != rCode) {
    oSysCall.vError(errno);
    vWarn("Shm::bUnmap: munmap() error; errno=%d %s\n"_Stz,errno,strerror(errno));
    return false;
  }
  
  oSysCall.vSuccess();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* unlink an existing memory region                                           */
/*----------------------------------------------------------------------------*/
bool Shm::bUnlink(s32& sRegionName, SysCall& oSysCall)
{
  oSysCall.vClear();
  
  int rCode = shm_unlink(sRegionName.aChar());

  if (0 != rCode) {
    oSysCall.vError(errno);
    vWarn("Shm::bUnlink: shm_unlink() error; errno=%d %s\n"_Stz,errno,strerror(errno));
    return false;
  }
  
  oSysCall.vSuccess();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
