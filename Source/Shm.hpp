/*============================================================================*/
/* Shm.h -- shared memory                                                     */
/*============================================================================*/
#pragma once

namespace System
{
  static const bool C_SHARED_REGION = true;
  static const bool C_HUGE_PAGES = true;
  
  class Shm
  {
  public:
    static bool  bAttach(s32& sName, int& iShmFD, SysCall& SysCall);
    static bool  bClose(int iShmFD, SysCall& oSysCall);
    static bool  bCreate(s32& sName, i8 iSize, int& iShmFD,
                           bool bRecreate, SysCall& SysCall);
    static bool  bLock(void* pAddress, i8 iBytes, SysCall& SysCall);
    static void* aMap(int iFd, void* aAddress, i8 iBytes, bool bShared,
                           bool bHugeTlb, SysCall& SysCall);
    static bool  bUnmap(void* aAddress, i8 iBytes, SysCall& oSysCall);
    static bool  bUnlink(s32& sName, SysCall& oSysCall);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
