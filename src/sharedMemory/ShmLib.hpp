/*============================================================================*/
/* ShmLib.h -- shared memory                                                  */
/*============================================================================*/
#pragma once

namespace Resources
{
  static const bool C_HUGE_PAGES    = true;
  static const bool C_NO_HUGE_PAGES = false;

  class ShmLib
  {
  public:
    static bool  bClose(int iShmFd, SysCall& oSysCall);
    static bool  bExtend(int iShmFd, i8 iSize, SysCall& oSysCall);
    static bool  bLock(void* aAddress, i8 iSize, SysCall& SysCall);
    static void* aMap(int iShmFd, void* aAddress, i8 iSize,
                      bool bHugeTlb, SysCall& SysCall);
    static bool  bOpenCreate(int& iShmFd, s32& sShmNm, SysCall& SysCall);
    static bool  bOpenExisting(int& iShmFd, s32& sShmNm, SysCall& SysCall);
    static bool  bTest(s32& sShmNm);
    static bool  bUnmap(void* aAddress, i8 iSize, SysCall& oSysCall);
    static bool  bUnlink(s32& sShmNm, SysCall& oSysCall);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
