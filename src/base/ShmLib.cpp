/*============================================================================*/
/* ShmLib.cpp -- shared memory library                                        */
/*============================================================================*/

#include "BASE.h"

#include <sys/mman.h>

#include "TagConstants.h"

#include "CmdLineArgs.hpp"

extern CmdLineArgs gxCmdLineArgs;

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* close an existing shared memory segment                                    */
/*----------------------------------------------------------------------------*/
bool ShmLib::bClose(int iShmFd, SysCall& oSysCall)
{
  oSysCall.vClear();
  
  if (gxCmdLineArgs.m_xSysArgs.m_oArgTags.bHasName(C_SEE_SHM))
    vTrace("ShmLib::bClose : %lld\n"_Stz,iShmFd);

  int iResult = close(iShmFd);
  
  if (0 != iResult) {
    oSysCall.vError(errno);
    return false;
  }
  
  //if (AM::bAttached()) AM::pResources()->vDecShmOpen();

  oSysCall.vSuccess();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extend newely created shared memory segment to given size                  */
/*----------------------------------------------------------------------------*/
bool ShmLib::bExtend(int iShmFd, i8 iSize, SysCall& oSysCall)
{
  
  if (gxCmdLineArgs.m_xSysArgs.m_oArgTags.bHasName(C_SEE_SHM))
    vTrace("ShmLib::bExtend : %lld to %lldB\n"_Stz,iShmFd,iSize);

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
bool ShmLib::bLock(void* aAddress, i8 iBytes, SysCall& SysCall)
{
  SysCall.vClear();
  
  if (gxCmdLineArgs.m_xSysArgs.m_oArgTags.bHasName(C_SEE_SHM))
    vTrace("ShmLib::bLock: %p of %lldB\n"_Stz,aAddress,iBytes);

  if (mlock(aAddress,iBytes) != 0) {
    SysCall.vError(errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create and open a new shared memory segment                                */
/*----------------------------------------------------------------------------*/
bool ShmLib::bOpenCreate(int& iShmFd, s32& sShmNm, SysCall& oSysCall)
{
  oSysCall.vClear();
 
  if (gxCmdLineArgs.m_xSysArgs.m_oArgTags.bHasName(C_SEE_SHM))
    vTrace("ShmLib::bOpenCreate : %s\n"_Stz,sShmNm.azChar());

  // open shared memory region and get a file descriptor
  int    iFlags = O_CREAT | O_EXCL | O_RDWR;
  mode_t iMode  = S_IRUSR | S_IWUSR;
  iShmFd = shm_open(sShmNm.azChar(),iFlags,iMode);

  if (iShmFd == -1) {
    oSysCall.vError(errno);
    return false;
  }
  
  //if (AM::bAttached()) AM::pResources()->vIncShmOpen();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* open an existing shared memory sement                                      */
/*----------------------------------------------------------------------------*/
bool ShmLib::bOpenExisting(int& iShmFd, s32& sShmNm, SysCall& oSysCall)
{
  oSysCall.vClear();
  
  if (gxCmdLineArgs.m_xSysArgs.m_oArgTags.bHasName(C_SEE_SHM))
    vTrace("ShmLib::bOpenExisting : %s\n"_Stz,sShmNm.azChar());
  
  // open shared memory region and get a file descriptor
  int    iFlags = O_RDWR;
  mode_t iMode  = S_IRUSR | S_IWUSR;
  iShmFd = shm_open(sShmNm.azChar(), iFlags, iMode);
  
  if (iShmFd == -1) {
    oSysCall.vError(errno);
    return false;
  }
  
  //if (AM::bAttached()) AM::pResources()->vIncShmOpen();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* map address onto a shared memory segment                                   */
/*----------------------------------------------------------------------------*/
void* ShmLib::aMap(int iShmFd, void* aAddress, i8 iBytes, bool bHugeTlb, SysCall& SysCall)
{
  SysCall.vClear();
  
  if (gxCmdLineArgs.m_xSysArgs.m_oArgTags.bHasName(C_SEE_SHM))
    vTrace("ShmLib::aMap : %lld to %p %lldB\n"_Stz,iShmFd,aAddress,iBytes);

  bool bShared = true;
  
  int iProt = PROT_READ | PROT_WRITE;
  
#ifdef WITH_APPLE
  int iFlags =   (bShared  ? MAP_SHARED : MAP_PRIVATE | MAP_ANONYMOUS)
               | (aAddress ? MAP_FIXED  : 0);
  UNUSED(bHugeTlb);
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
bool ShmLib::bTest(s32& sShmNm)
{
  int iShmFd;
  SysCall oSysCall;
  
  if (gxCmdLineArgs.m_xSysArgs.m_oArgTags.bHasName(C_SEE_SHM))
    vTrace("ShmLib::bTest : %s\n"_Stz,sShmNm.azChar());

  // open existing shm region
  bool bResult = ShmLib::bOpenExisting(iShmFd,sShmNm,oSysCall);
  if ( ! bResult )
    return false;
  
  bResult = ShmLib::bClose(iShmFd,oSysCall);
  RASSERT(bResult);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* unmap an existing memory region                                            */
/*----------------------------------------------------------------------------*/
bool ShmLib::bUnmap(void *aAddress, i8 iBytes, SysCall& oSysCall)
{
  oSysCall.vClear();
  
  if (gxCmdLineArgs.m_xSysArgs.m_oArgTags.bHasName(C_SEE_SHM))
    vTrace("ShmLib::bUnmap: %p of %lldB\n"_Stz,aAddress,iBytes);

  
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
bool ShmLib::bUnlink(s32& sShmNm, SysCall& oSysCall)
{
  oSysCall.vClear();
  
  if (gxCmdLineArgs.m_xSysArgs.m_oArgTags.bHasName(C_SEE_SHM))
    vTrace("ShmLib::bUnlink : %s\n"_Stz,sShmNm.azChar());

  int iResult = shm_unlink(sShmNm.azChar());

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
