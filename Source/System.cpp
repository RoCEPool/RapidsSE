/*============================================================================*/
/* PosixThread - a posix thread                                               */
/*============================================================================*/

#include "Common.h"

/*----------------------------------------------------------------------------*/
/* get clock frequency of this machine                                        */
/*----------------------------------------------------------------------------*/
i8 System::iClockFrequency(void)
{
  
#ifdef __APPLE__
  
  int mib[2];
  unsigned int freq;
  size_t len;
  
  mib[0] = CTL_HW;
  mib[1] = HW_CPU_FREQ;
  len = sizeof(freq);
  sysctl(mib, 2, &freq, &len, NULL, 0);
  
  return freq;
  
#else
  vWarn("System::iClockFrequency not implemented on LINUX!");
  return 0;
#endif
  
}

/*----------------------------------------------------------------------------*/
/* get number of logical cores                                                */
/*----------------------------------------------------------------------------*/
i8 System::iCoreCount(void)
{
  
#ifdef __APPLE__
  
  i8     iCount = 0;
  size_t len = sizeof(iCount);
  
  i8 iReturn = sysctlbyname("hw.logicalcpu", &iCount, &len, 0, 0);
  if (iReturn) {
    vWarn("Error while getting cpu count %lld\n", iReturn);
    return 0;
  }
  return iCount;

  
#else
  vWarn("System::iPhysicalMemory not implemented on LINUX!");
  return 0;
#endif
  
}

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
  vWarn("System::iCpuCount not implemented on LINUX!");
  return 0;
#endif
  
}

/*----------------------------------------------------------------------------*/
/* get amount of physical memory                                              */
/*----------------------------------------------------------------------------*/
i8 System::iPhysicalMemory(void)
{
  
#ifdef __APPLE__
  
  int mib[2];
  u8 iMemQty;
  size_t len;
  
  mib[0] = CTL_HW;
  mib[1] = HW_MEMSIZE;
  len = sizeof(iMemQty);
  sysctl(mib, 2, &iMemQty, &len, NULL, 0);
  
  return iMemQty;
  
#else
  vWarn("System::iPhysicalMemory not implemented on LINUX!");
  return 0;
#endif
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

