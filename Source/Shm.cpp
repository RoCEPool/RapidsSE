/*============================================================================*/
/* Shm.cpp -- shared memory                                                   */
/*============================================================================*/

#include "Env.hpp"
#include "Arguments.hpp"

Atomic gzShmOpenCount;

extern Arguments gxArguments;

/*----------------------------------------------------------------------------*/
/* attach/open to an existing shared memory region                            */
/*----------------------------------------------------------------------------*/
bool Shm::bAttach(s& sRegionName, int& iFD, SysCall& xSysCall)
{
  xSysCall.vClear();
    
  // open shared memory region and get a fd
  iFD = shm_open(sRegionName.a(), O_RDWR, S_IRUSR | S_IWUSR);
  
  if (iFD == -1) {
    xSysCall.vError(errno);
    //vWarn("Shm::bAttach: shm_open() error; errno=%d %s\n",errno, strerror(errno));
    return false;
  }
  
  ++gzShmOpenCount;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* close an existing memory region                                            */
/*----------------------------------------------------------------------------*/
bool Shm::bClose(int iFD, SysCall& xSysCall)
{
  xSysCall.vClear();
  
  int rCode = close(iFD);
  
  if (0 != rCode) {
    xSysCall.vError(errno);
    vWarn("Shm::bClose: munmap() error; errno=%d %s\n",errno,strerror(errno));
    return false;
  }
  
  --gzShmOpenCount;
  
  xSysCall.vSuccess();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create/open a new shared memory region                                     */
/*----------------------------------------------------------------------------*/
bool Shm::bCreate(s& sRegionName, i8 iSize, int& iFD,
                    bool bRecreate, SysCall& xSysCall)
{
  int rCode;
  
  xSysCall.vClear();
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm::bCreate A\n");
  
  // unlink the shm region (in case created earlier; this needs work)
  if (bRecreate)
    rCode = shm_unlink(sRegionName.a());
  
  // we ignore the error

  if (gxArguments.m_bVerbose)
    vTrace("Shm::bCreate B\n");
  
  // open shared memory region and get a fd
  iFD = shm_open(sRegionName.a(), O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm::bCreate C\n");
  
  if (iFD == -1) {
    xSysCall.vError(errno);
    vWarn("Shm::bCreate: shm_open() error; errno=%d %s\n",errno,strerror(errno));
    return false;
  }
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm::bCreate D\n");
  
  ++gzShmOpenCount;

  // fill with 0's
  rCode = ftruncate(iFD, iSize);
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm::bCreate E\n");
  
  if (rCode == -1) {
    if (gxArguments.m_bVerbose)
      vTrace("Shm::bCreate F\n");
   
    xSysCall.vError(errno);
    vTrace("Shm::bCreate: ftruncate() error; errno=%d %s\n",errno,strerror(errno));
    
    int rCode = shm_unlink(sRegionName.a());
    
    if (0 != rCode) {
      xSysCall.vError(errno);
      vWarn("Shm::bCreate: shm_unlink() error; errno=%d %s\n",errno,strerror(errno));
      return false;
    }
    
    return false;
  }
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm::bCreate G\n");
  return true;
}

/*----------------------------------------------------------------------------*/
/* map a new an anonymous region of memory (private or shared)                */
/*----------------------------------------------------------------------------*/
bool Shm::bLock(void* pAddress, i8 iBytes, SysCall& SysCall)
{
  SysCall.vClear();

  if (gxArguments.m_bVerbose)
    vTrace("Shm::vLock A\n");
  
  if (mlock(pAddress,iBytes) != 0) {
    SysCall.vError(errno);
    vWarn("Shm::vLock: mlock() error; errno=%d %s line=%d\n",errno,strerror(errno), __LINE__);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* map a new an anonymous region of memory (private or shared)                */
/*----------------------------------------------------------------------------*/
void* Shm::pMap(int iFd, void* pAddress, i8 iBytes, bool bShared, bool bHugeTlb, SysCall& SysCall)
{
  SysCall.vClear();
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm::pMap A\n");
  
  int iProt = PROT_READ | PROT_WRITE;

  if (gxArguments.m_bVerbose)
    vTrace("Shm::pMap B\n");
  
#ifdef __APPLE__
  int iFlags =   (bShared  ? MAP_SHARED : MAP_PRIVATE | MAP_ANONYMOUS)
               | (pAddress ? MAP_FIXED  : 0);
#else
  int iFlags =   (bShared  ? MAP_SHARED : MAP_PRIVATE)
               | (pAddress ? MAP_FIXED  : 0)
               | (bHugeTlb ? MAP_HUGETLB | MAP_ANONYMOUS : 0);
#endif
  
  size_t x;
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm::pMap C\n");
  
  void* pRegion = mmap(pAddress, // starting address
                       iBytes,   // length of the region
                       iProt,    // region protection
                       iFlags,   // mapping flags
                       iFd,      // file descriptor
                       0         // no offset in file
                       );
  
  if (MAP_FAILED == pRegion) {
    SysCall.vError(errno);
    vWarn("Shm::pMap: mmap() error; errno=%d %s line=%d\n",errno,strerror(errno), __LINE__);
    return nullptr;
  }
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm::pMap D\n");
  
  SysCall.vSuccess();
  
  return pRegion;
}

/*----------------------------------------------------------------------------*/
/* unmap an existing memory region                                            */
/*----------------------------------------------------------------------------*/
bool Shm::bUnmap(void *pAddress, i8 iBytes, SysCall& xSysCall)
{
  xSysCall.vClear();
  
  int rCode = munmap(pAddress, // starting address
                     iBytes    // length of the region
                     );
  
  if (0 != rCode) {
    xSysCall.vError(errno);
    vWarn("Shm::bUnmap: munmap() error; errno=%d %s\n",errno,strerror(errno));
    return false;
  }
  
  xSysCall.vSuccess();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* unlink an existing memory region                                           */
/*----------------------------------------------------------------------------*/
bool Shm::bUnlink(s& sRegionName, SysCall& xSysCall)
{
  xSysCall.vClear();
  
  int rCode = shm_unlink(sRegionName.a());

  if (0 != rCode) {
    xSysCall.vError(errno);
    vWarn("Shm::bUnlink: shm_unlink() error; errno=%d %s\n",errno,strerror(errno));
    return false;
  }
  
  xSysCall.vSuccess();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
