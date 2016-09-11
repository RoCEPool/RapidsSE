/*============================================================================*/
/* Shm.h -- shared memory                                                     */
/*============================================================================*/
#pragma once

#include "Common.h"

namespace Common
{
  class Shm : public Static
  {
  public:
    static bool  bAttach(const char* aName, int& iDescriptor, SysCall& SysCall);
    static bool  bClose(int iDescriptor, SysCall& xSysCall);
    static bool  bCreate(const char* aName, i8 iSize, int& iDescriptor, SysCall& SysCall, bool bRecreate=false);
    static void* pMap(int iFd, void* pAddress, i8 iBytes, bool bShared, SysCall& SysCall);
    static bool  bUnmap(void* pAddress, i8 iBytes, SysCall& xSysCall);
    static bool  bUnlink(const char* aName, SysCall& xSysCall);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/