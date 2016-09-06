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
  
  i8 iCount = sysconf(_SC_NPROCESSORS_ONLN);
  if (iCount == -1) {
    vWarn("Error while getting cpu count %lld\n", iCount);
    return 0;
  }
  
  return iCount;
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
  
  i8 iCount = get_nprocs();
  if (iCount == -1) {
    vWarn("Error while getting cpu count %lld\n", iCount);
    return 0;
  }
  
  return iCount;
#endif
  
}

/*----------------------------------------------------------------------------*/
/* get amount of physical memory                                              */
/*----------------------------------------------------------------------------*/
i8 System::iPhysicalMemory(void)
{
  i8 iNumPages = sysconf(_SC_PHYS_PAGES);
  if (iNumPages == -1) {
    vWarn("Error while getting physical memory %lld\n", iNumPages);
    return 0;
  }
  i8 iPageSize = sysconf(_SC_PAGESIZE);
  if (iPageSize == -1) {
    vWarn("Error while getting physical memory %lld\n", iPageSize);
    return 0;
  }

  i8 MemQty = iNumPages * iPageSize;
  return MemQty;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

