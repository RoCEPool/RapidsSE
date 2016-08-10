/*============================================================================*/
/* Shm.cpp -- shared memory                                                   */
/*============================================================================*/

#include "Env.hpp"

/*----------------------------------------------------------------------------*/
/* attach/open to an existing shared memory region                            */
/*----------------------------------------------------------------------------*/
bool Shm::bAttach(const char* aName, int& iFD, SysCall& xSysCall)
{
  xSysCall.vClear();
    
  // open shared memory region and get a fd
  iFD = shm_open(aName, O_RDWR, S_IRUSR | S_IWUSR);
  
  if (iFD == -1) {
    xSysCall.vError(errno);
    vTrace("shm_open() error; errno=%d %s\n",errno, strerror(errno));
    return false;
  }
  
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
    //vReportWarning("munmap() error; errno=%d",errno);
    return false;
  }
  
  xSysCall.vSuccess();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create/open a new shared memory region                                     */
/*----------------------------------------------------------------------------*/
bool Shm::bCreate(const char* aName, i8 iSize, int& iFD, SysCall& xSysCall)
{
  xSysCall.vClear();
  
  // unlink the shm region (in case created earlier; this needs work)
  int rCode = shm_unlink(aName);
  
  // we ignore the error
  
  // open shared memory region and get a fd
  iFD = shm_open(aName, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
  
  if (iFD == -1) {
    xSysCall.vError(errno);
    vTrace("shm_open() error; errno=%d %s\n",errno, strerror(errno));
    return false;
  }
  
  // fill with 0's
  rCode = ftruncate(iFD, iSize);
  
  if (rCode == -1) {
    xSysCall.vError(errno);
    vTrace("ftruncate() error; errno=%d %s\n",errno, strerror(errno));
    
    int rCode = shm_unlink(aName);
    
    if (0 != rCode) {
      xSysCall.vError(errno);
      //vReportWarning("munmap() error; errno=%d",errno);
      return false;
    }
    
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* map a new an anonymous region of memory (private or shared)                */
/*----------------------------------------------------------------------------*/
void* Shm::pMap(int iFd, void* pAddress, i8 iBytes, bool bShared, SysCall& SysCall)
{
  SysCall.vClear();
  
  int iProt = PROT_READ | PROT_WRITE;
  
  int iFlags =   (bShared  ? MAP_SHARED : MAP_PRIVATE | MAP_ANONYMOUS)
               | (pAddress ? MAP_FIXED  : 0);
  
  void* pRegion = mmap(pAddress, // starting address
                       iBytes,   // length of the region
                       iProt,    // region protection
                       iFlags,   // mapping flags
                       iFd,      // file descriptor
                       0         // no offset in file
                       );
  
  if (MAP_FAILED == pRegion) {
    SysCall.vError(errno);
    //vReportWarning("mmap() error; errno=%d",errno);
    return nullptr;
  }
  
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
    //vReportWarning("munmap() error; errno=%d",errno);
    return false;
  }
  
  xSysCall.vSuccess();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* unlink an existing memory region                                           */
/*----------------------------------------------------------------------------*/
bool Shm::bUnlink(const char* aName, SysCall& xSysCall)
{
  xSysCall.vClear();
  
  int rCode = shm_unlink(aName);

  if (0 != rCode) {
    xSysCall.vError(errno);
    vWarn("shm_unlink() error; errno=%d",errno);
    return false;
  }
  
  xSysCall.vSuccess();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/