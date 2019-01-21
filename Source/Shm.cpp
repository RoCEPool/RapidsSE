/*============================================================================*/
/* Shm.cpp -- shared memory                                                   */
/*============================================================================*/

#include "System.h"

#include "StorageEngine.h"

extern ResourceTracker gxResourceTracker;

/*----------------------------------------------------------------------------*/
/* attach to an existing shared memory sement                                 */
/*----------------------------------------------------------------------------*/
bool Shm::bAttach(s32& sRegionName, int& iShmSegFd, SysCall& oSysCall)
{
  oSysCall.vClear();
    
  // open shared memory region and get a file descriptor
  iShmSegFd = shm_open(sRegionName.aChar(), O_RDWR, S_IRUSR | S_IWUSR);
  
  if (iShmSegFd == -1) {
    oSysCall.vError(errno);
    //vWarn("Shm::bAttach: shm_open() error; errno=%d %s\n"_Stz,errno, strerror(errno));
    return false;
  }
  
  gxResourceTracker.vIncShmOpen();

  return true;
}

/*----------------------------------------------------------------------------*/
/* close an existing shared memory segment                                    */
/*----------------------------------------------------------------------------*/
bool Shm::bClose(int iShmSegFd, SysCall& oSysCall)
{
  oSysCall.vClear();
  
  int rCode = close(iShmSegFd);
  
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
bool Shm::bCreate(s32& sRegionName, i8 iSize, bool bRecreate,
                  int& iFD, SysCall& oSysCall)
{
  int rCode;
  
  oSysCall.vClear();
  
  // unlink the shm region (in case created earlier; this needs work)
  if (bRecreate)
    rCode = shm_unlink(sRegionName.aChar());
  // we ignore any error
  
  // open shared memory region and get a fd
  iFD = shm_open(sRegionName.aChar(), O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
  
  if (iFD == -1) {
    oSysCall.vError(errno);
    vWarn("Shm::bCreate: shm_open() error; errno=%d %s\n"_Stz,errno,strerror(errno));
    return false;
  }
  
  gxResourceTracker.vIncShmOpen();

  // extend and fill with 0's
  bool bExtended = bExtend(iFD, iSize, oSysCall);
  
  if (! bExtended) {
    
    int rCode = shm_unlink(sRegionName.aChar());
    
    if (0 != rCode) {
      oSysCall.vError(errno);
      vWarn("Shm::bCreate: shm_unlink() error; errno=%d %s\n"_Stz,errno,strerror(errno));
      return false;
    }
    
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extend newely created shared memory segment to given size                  */
/*----------------------------------------------------------------------------*/
bool Shm::bExtend(int iShmSegFd, i8 iSize, SysCall& oSysCall)
{
  // extend segment to given length (will be filled with zero's)
  int rCode = ftruncate(iShmSegFd, iSize);
  
  if (rCode == -1) {
    
    oSysCall.vError(errno);
    vTrace("Shm::bZeroFill: ftruncate() error; errno=%d %s\n"_Stz,errno,strerror(errno));
    
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* lock a shm region into memory                                              */
/*----------------------------------------------------------------------------*/
bool Shm::bLock(void* pAddress, i8 iBytes, SysCall& SysCall)
{
  SysCall.vClear();
  
  if (mlock(pAddress,iBytes) != 0) {
    SysCall.vError(errno);
    vWarn("Shm::vLock: mlock() error; errno=%d %s line=%d\n"_Stz,
          errno,strerror(errno), __LINE__);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create and open a new shared memory segment                                */
/*----------------------------------------------------------------------------*/
bool Shm::bOpenCreate(s32& sRegionName, int& iShmSegFd, SysCall& oSysCall)
{
  oSysCall.vClear();
  
  // open shared memory region and get a file descriptor
  int    iFlags = O_CREAT | O_EXCL | O_RDWR;
  mode_t iMode  = S_IRUSR | S_IWUSR;
  iShmSegFd = shm_open(sRegionName.aChar(),iFlags,iMode);

  if (iShmSegFd == -1) {
    oSysCall.vError(errno);
    //vWarn("Shm::bAttach: shm_open() error; errno=%d %s\n"_Stz,errno, strerror(errno));
    return false;
  }
  
  gxResourceTracker.vIncShmOpen();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* open an existing shared memory sement                                      */
/*----------------------------------------------------------------------------*/
bool Shm::bOpenExisting(s32& sRegionName, int& iShmSegFd, SysCall& oSysCall)
{
  oSysCall.vClear();
  
  // open shared memory region and get a file descriptor
  int    iFlags = O_RDWR;
  mode_t iMode  = S_IRUSR | S_IWUSR;
  iShmSegFd = shm_open(sRegionName.aChar(), iFlags, iMode);
  
  if (iShmSegFd == -1) {
    oSysCall.vError(errno);
    //vWarn("Shm::bAttach: shm_open() error; errno=%d %s\n"_Stz,errno, strerror(errno));
    return false;
  }
  
  gxResourceTracker.vIncShmOpen();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* map address onto a shared memory segment                                   */
/*----------------------------------------------------------------------------*/
void* Shm::aMap(int iFd, void* aAddress, i8 iBytes, bool bHugeTlb, SysCall& SysCall)
{
  SysCall.vClear();
  
  bool bShared = true;
  
  int iProt = PROT_READ | PROT_WRITE;
  
#ifdef __APPLE__
  int iFlags =   (bShared  ? MAP_SHARED : MAP_PRIVATE | MAP_ANONYMOUS)
               | (aAddress ? MAP_FIXED  : 0);
#else
  int iFlags =   (bShared  ? MAP_SHARED : MAP_PRIVATE)
               | (aAddress ? MAP_FIXED  : 0)
               | MAP_NORESERVE
               | (bHugeTlb ? MAP_HUGETLB | MAP_ANONYMOUS : 0);
#endif
  
  void* aRegion = mmap(aAddress, // starting address
                       iBytes,   // length of the region
                       iProt,    // region protection
                       iFlags,   // mapping flags
                       iFd,      // file descriptor
                       0         // no offset in file
                       );
  
  if (MAP_FAILED == aRegion) {
    SysCall.vError(errno);
    vWarn("Shm::aMap: mmap() error; errno=%d %s line=%d\n"_Stz,
          errno,strerror(errno), __LINE__);
    return nullptr;
  }
  
  SysCall.vSuccess();
  
  return aRegion;
}

/*----------------------------------------------------------------------------*/
/* test for region                                                            */
/*----------------------------------------------------------------------------*/
bool Shm::bTest(s32& sRegionName)
{
  int iFD;
  SysCall oSysCall;
  
  // open existing shm region
  bool bResult = Shm::bOpenExisting(sRegionName,iFD,oSysCall);
  if ( ! bResult )
    return false;
  
  bResult = Shm::bClose(iFD,oSysCall);
  RASSERT(bResult);
  
  return true;
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
