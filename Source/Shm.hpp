/*============================================================================*/
/* Shm.h -- shared memory                                                     */
/*============================================================================*/
#pragma once

#include "Common.h"

namespace Common
{
  static const bool C_SHARED_REGION = true;
  static const bool C_HUGE_PAGES = true;
  
  class Shm : public Static
  {
  public:
    static bool  bAttach(sRefactor& sName, int& iShmFD, SysCall& SysCall);
    static bool  bClose(int iShmFD, SysCall& xSysCall);
    static bool  bCreate(sRefactor& sName, i8 iSize, int& iShmFD,
                           bool bRecreate, SysCall& SysCall);
    static void* pMap(int iFd, void* pAddress, i8 iBytes, bool bShared,
                           bool bHugeTlb, SysCall& SysCall);
    static bool  bUnmap(void* pAddress, i8 iBytes, SysCall& xSysCall);
    static bool  bUnlink(sRefactor& sName, SysCall& xSysCall);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
