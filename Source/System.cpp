/*============================================================================*/
/* PosixThread - a posix thread                                               */
/*============================================================================*/

#include "Common.h"

/*----------------------------------------------------------------------------*/
/* get the no of cou's on this machine                                        */
/*----------------------------------------------------------------------------*/
i8 System::iCpuCount(void)
{
  
#ifdef __APPLE__
  
  i8     iCount = 0;
  size_t len = sizeof(iCount);
  
  i8 iReturn = sysctlbyname(SYSCTL_CORE_COUNT, &iCount, &len, 0, 0);
  if (iReturn) {
    vWarn("Error while getting cpu count %lld\n", iReturn);
    return 0;
  }
  return iCount;

#else
  vWarn("Cpu count not implemented on LINUX!");
  return 0;
#endif
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

