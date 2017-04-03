/*============================================================================*/
/* Shm.cpp -- shared memory                                                   */
/*============================================================================*/

#include "System.h"

#include "StorageEngine.h"

Atomic gzShmOpenCount;

extern Arguments gxArguments;

/*----------------------------------------------------------------------------*/
/* attach/open to an existing shared memory region                            */
/*----------------------------------------------------------------------------*/
bool Shm::bAttach(StrVal& sRegionName, int& iFD, SysCall& xSysCall)
{
  xSysCall.vClear();
    
  // open shared memory region and get a fd
  iFD = shm_open(sRegionName.s(), O_RDWR, S_IRUSR | S_IWUSR);
  
  if (iFD == -1) {
    xSysCall.vError(errno);
    //vWarn("Shm::bAttach: shm_open() error; errno=%d %s\n"_Str,errno, strerror(errno));
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
    vWarn("Shm::bClose: munmap() error; errno=%d %s\n"_Str,errno,strerror(errno));
    return false;
  }
  
  --gzShmOpenCount;
  
  xSysCall.vSuccess();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create/open a new shared memory region                                     */
/*----------------------------------------------------------------------------*/
bool Shm::bCreate(StrVal& sRegionName, i8 iSize, int& iFD,
                    bool bRecreate, SysCall& xSysCall)
{
  int rCode;
  
  xSysCall.vClear();
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm::bCreate A\n"_Str);
  
  // unlink the shm region (in case created earlier; this needs work)
  if (bRecreate)
    rCode = shm_unlink(sRegionName.s());
  
  // we ignore the error

  if (gxArguments.m_bVerbose)
    vTrace("Shm::bCreate B\n"_Str);
  
  // open shared memory region and get a fd
  iFD = shm_open(sRegionName.s(), O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm::bCreate C\n"_Str);
  
  if (iFD == -1) {
    xSysCall.vError(errno);
    vWarn("Shm::bCreate: shm_open() error; errno=%d %s\n"_Str,errno,strerror(errno));
    return false;
  }
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm::bCreate D\n"_Str);
  
  ++gzShmOpenCount;

  // fill with 0's
  rCode = ftruncate(iFD, iSize);
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm::bCreate E\n"_Str);
  
  if (rCode == -1) {
    if (gxArguments.m_bVerbose)
      vTrace("Shm::bCreate F\n"_Str);
   
    xSysCall.vError(errno);
    vTrace("Shm::bCreate: ftruncate() error; errno=%d %s\n"_Str,errno,strerror(errno));
    
    int rCode = shm_unlink(sRegionName.s());
    
    if (0 != rCode) {
      xSysCall.vError(errno);
      vWarn("Shm::bCreate: shm_unlink() error; errno=%d %s\n"_Str,errno,strerror(errno));
      return false;
    }
    
    return false;
  }
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm::bCreate G\n"_Str);
  return true;
}

/*----------------------------------------------------------------------------*/
/* map a new an anonymous region of memory (private or shared)                */
/*----------------------------------------------------------------------------*/
bool Shm::bLock(void* pAddress, i8 iBytes, SysCall& SysCall)
{
  SysCall.vClear();
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm::vLock\n"_Str);
  
  if (mlock(pAddress,iBytes) != 0) {
    SysCall.vError(errno);
    vWarn("Shm::vLock: mlock() error; errno=%d %s line=%d\n"_Str,errno,strerror(errno), __LINE__);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* map a new an anonymous region of memory (private or shared)                */
/*----------------------------------------------------------------------------*/
void* Shm::pMap(int iFd, void* aAddress, i8 iBytes, bool bShared, bool bHugeTlb, SysCall& SysCall)
{
  SysCall.vClear();
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm::pMap A\n"_Str);
  
  int iProt = PROT_READ | PROT_WRITE;

  if (gxArguments.m_bVerbose)
    vTrace("Shm::pMap B\n"_Str);
  
#ifdef __APPLE__
  int iFlags =   (bShared  ? MAP_SHARED : MAP_PRIVATE | MAP_ANONYMOUS)
               | (aAddress ? MAP_FIXED  : 0);
#else
  int iFlags =   (bShared  ? MAP_SHARED : MAP_PRIVATE)
               | (aAddress ? MAP_FIXED  : 0)
               | MAP_NORESERVE
               | (bHugeTlb ? MAP_HUGETLB | MAP_ANONYMOUS : 0);
#endif
  
  size_t x;
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm::pMap C\n"_Str);
  
  void* pRegion = mmap(aAddress, // starting address
                       iBytes,   // length of the region
                       iProt,    // region protection
                       iFlags,   // mapping flags
                       iFd,      // file descriptor
                       0         // no offset in file
                       );
  
  if (MAP_FAILED == pRegion) {
    SysCall.vError(errno);
    vWarn("Shm::pMap: mmap() error; errno=%d %s line=%d\n"_Str,errno,strerror(errno), __LINE__);
    return nullptr;
  }
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm::pMap D\n"_Str);
  
  SysCall.vSuccess();
  
  return pRegion;
}

/*----------------------------------------------------------------------------*/
/* unmap an existing memory region                                            */
/*----------------------------------------------------------------------------*/
bool Shm::bUnmap(void *aAddress, i8 iBytes, SysCall& xSysCall)
{
  xSysCall.vClear();
  
  int rCode = munmap(aAddress, // starting address
                     iBytes    // length of the region
                     );
  
  if (0 != rCode) {
    xSysCall.vError(errno);
    vWarn("Shm::bUnmap: munmap() error; errno=%d %s\n"_Str,errno,strerror(errno));
    return false;
  }
  
  xSysCall.vSuccess();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* unlink an existing memory region                                           */
/*----------------------------------------------------------------------------*/
bool Shm::bUnlink(StrVal& sRegionName, SysCall& xSysCall)
{
  xSysCall.vClear();
  
  int rCode = shm_unlink(sRegionName.s());

  if (0 != rCode) {
    xSysCall.vError(errno);
    vWarn("Shm::bUnlink: shm_unlink() error; errno=%d %s\n"_Str,errno,strerror(errno));
    return false;
  }
  
  xSysCall.vSuccess();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
