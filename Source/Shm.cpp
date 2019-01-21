/*============================================================================*/
/* Shm.cpp -- shared memory                                                   */
/*============================================================================*/

#include "System.h"

#include "StorageEngine.h"

extern ResourceTracker gxResourceTracker;

/*----------------------------------------------------------------------------*/
/* close an existing shared memory segment                                    */
/*----------------------------------------------------------------------------*/
bool Shm::bClose(int iShmFd, SysCall& oSysCall)
{
  oSysCall.vClear();
  
  int iResult = close(iShmFd);
  
  if (0 != iResult) {
    oSysCall.vError(errno);
    return false;
  }
  
  gxResourceTracker.vDecShmOpen();

  oSysCall.vSuccess();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extend newely created shared memory segment to given size                  */
/*----------------------------------------------------------------------------*/
bool Shm::bExtend(int iShmFd, i8 iSize, SysCall& oSysCall)
{
  // extend segment to given length (will be filled with zero's)
  int iResult = ftruncate(iShmFd, iSize);
  
  if (iResult == -1) {
    oSysCall.vError(errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* lock a shm region into memory                                              */
/*----------------------------------------------------------------------------*/
bool Shm::bLock(void* aAddress, i8 iBytes, SysCall& SysCall)
{
  SysCall.vClear();
  
  if (mlock(aAddress,iBytes) != 0) {
    SysCall.vError(errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create and open a new shared memory segment                                */
/*----------------------------------------------------------------------------*/
bool Shm::bOpenCreate(s32& sShmNm, int& iShmFd, SysCall& oSysCall)
{
  oSysCall.vClear();
  
  // open shared memory region and get a file descriptor
  int    iFlags = O_CREAT | O_EXCL | O_RDWR;
  mode_t iMode  = S_IRUSR | S_IWUSR;
  iShmFd = shm_open(sShmNm.aChar(),iFlags,iMode);

  if (iShmFd == -1) {
    oSysCall.vError(errno);
    return false;
  }
  
  gxResourceTracker.vIncShmOpen();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* open an existing shared memory sement                                      */
/*----------------------------------------------------------------------------*/
bool Shm::bOpenExisting(s32& sShmNm, int& iShmFd, SysCall& oSysCall)
{
  oSysCall.vClear();
  
  // open shared memory region and get a file descriptor
  int    iFlags = O_RDWR;
  mode_t iMode  = S_IRUSR | S_IWUSR;
  iShmFd = shm_open(sShmNm.aChar(), iFlags, iMode);
  
  if (iShmFd == -1) {
    oSysCall.vError(errno);
    return false;
  }
  
  gxResourceTracker.vIncShmOpen();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* map address onto a shared memory segment                                   */
/*----------------------------------------------------------------------------*/
void* Shm::aMap(int iShmFd, void* aAddress, i8 iBytes, bool bHugeTlb, SysCall& SysCall)
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
                       iShmFd,      // file descriptor
                       0         // no offset in file
                       );
  
  if (MAP_FAILED == aRegion) {
    SysCall.vError(errno);
    return nullptr;
  }
  
  SysCall.vSuccess();
  
  return aRegion;
}

/*----------------------------------------------------------------------------*/
/* test for region                                                            */
/*----------------------------------------------------------------------------*/
bool Shm::bTest(s32& sShmNm)
{
  int iShmFd;
  SysCall oSysCall;
  
  // open existing shm region
  bool bResult = Shm::bOpenExisting(sShmNm,iShmFd,oSysCall);
  if ( ! bResult )
    return false;
  
  bResult = Shm::bClose(iShmFd,oSysCall);
  RASSERT(bResult);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* unmap an existing memory region                                            */
/*----------------------------------------------------------------------------*/
bool Shm::bUnmap(void *aAddress, i8 iBytes, SysCall& oSysCall)
{
  oSysCall.vClear();
  
  int iResult = munmap(aAddress, // starting address
                     iBytes    // length of the region
                     );
  
  if (0 != iResult) {
    oSysCall.vError(errno);
    return false;
  }
  
  oSysCall.vSuccess();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* unlink an existing memory region                                           */
/*----------------------------------------------------------------------------*/
bool Shm::bUnlink(s32& sShmNm, SysCall& oSysCall)
{
  oSysCall.vClear();
  
  int iResult = shm_unlink(sShmNm.aChar());

  if (0 != iResult) {
    oSysCall.vError(errno);
    return false;
  }
  
  oSysCall.vSuccess();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
