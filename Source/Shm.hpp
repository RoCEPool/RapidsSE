/*============================================================================*/
/* Shm.h -- shared memory                                                     */
/*============================================================================*/
#pragma once

#include "System.h"

namespace System
{
  static const bool C_SHARED_REGION = true;
  static const bool C_HUGE_PAGES = true;
  
  class Shm
  {
  public:
    static bool  bAttach(StrVal& sName, int& iShmFD, SysCall& SysCall);
    static bool  bClose(int iShmFD, SysCall& xSysCall);
    static bool  bCreate(StrVal& sName, i8 iSize, int& iShmFD,
                           bool bRecreate, SysCall& SysCall);
    static void* pMap(int iFd, void* aAddress, i8 iBytes, bool bShared,
                           bool bHugeTlb, SysCall& SysCall);
    static bool  bUnmap(void* aAddress, i8 iBytes, SysCall& xSysCall);
    static bool  bUnlink(StrVal& sName, SysCall& xSysCall);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
