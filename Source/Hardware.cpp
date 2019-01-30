/*============================================================================*/
/* Hardware -- machine harware                                                */
/*============================================================================*/

#include "System.h"

#include "Debugging.h"

/*----------------------------------------------------------------------------*/
/* get clock frequency of this machine                                        */
/*----------------------------------------------------------------------------*/
i8 Hardware::iClockFrequency(void)
{
  
#ifdef __APPLE__

  struct clockinfo clockrate;
  
  int mib[2];
  unsigned int freq;
  size_t len;
  
  mib[0] = CTL_KERN;
  mib[1] = KERN_CLOCKRATE;
  len = sizeof clockrate;
  sysctl(mib, 2, &clockrate, &len, NULL, 0);

  //vTrace("Kern ClockRate %d\n"_Stz,clockrate.hz);

  // - - -

  
  mib[0] = CTL_HW;
  mib[1] = HW_CPU_FREQ;
  len = sizeof(freq);
  sysctl(mib, 2, &freq, &len, NULL, 0);
  
  return freq;
  
#else
  const i8 C_BUFSIZ=512;
  char caBuf[C_BUFSIZ];
  double dFreq;
  i8 iFreq = 0;
  
  // open the /Proc/cpuinfo file
  FILE* aFile = fopen("/proc/cpuinfo", "r");
  if (! aFile) {
    vWarn("File::bOpen: fopen(/proc/cpuinfo,r) error; errno=%d"_Stz, errno);
    return 0;
  }
  
  // look for cpu MHz
  while (fgets(caBuf, C_BUFSIZ, aFile) != NULL) {
    if (strstr(caBuf, "cpu MHz") == NULL)
      continue;
    
    char* p = strchr(caBuf,':');
    if (p == NULL)
      continue;
    p++;
    while (isspace(*p))
      p++;
    sscanf(p,"%lf", &dFreq);
    iFreq = static_cast<i8>(dFreq * (double)C_1M);
  }

  //vTrace("Clock frequency: %lld\n"_Stz, iFreq);

  return iFreq;

#endif
  
}

/*----------------------------------------------------------------------------*/
/* get number of logical cores                                                */
/*----------------------------------------------------------------------------*/
i8 Hardware::iCoreCount(void)
{
  
  i8 iCount = sysconf(_SC_NPROCESSORS_ONLN);
  if (iCount == -1) {
    vWarn("Error while getting cpu count %lld\n"_Stz, iCount);
    return 0;
  }
 
  return iCount;
}

/*----------------------------------------------------------------------------*/
/* get the no of cou's on this machine                                        */
/*----------------------------------------------------------------------------*/
i8 Hardware::iCpuCount(void)
{
 
#ifdef __APPLE__
  
  i8     iCount = 0;
  size_t len = sizeof(iCount);
  
  i8 iReturn = sysctlbyname("machdep.cpu.core_count", &iCount, &len, 0, 0);
  if (iReturn) {
    vWarn("Error while getting cpu count %lld\n"_Stz, iReturn);
    return 0;
  }
  return iCount;
  
#else
  
  //vWarn("iCpuCount() not implemented on LINUX!\n");
  return 0;

#endif
  
}

/*----------------------------------------------------------------------------*/
/* get amount of physical memory                                              */
/*----------------------------------------------------------------------------*/
i8 Hardware::iPhysicalMemory(void)
{
  i8 iNumPages = sysconf(_SC_PHYS_PAGES);
  if (iNumPages == -1) {
    vWarn("Error while getting physical memory %lld\n"_Stz, iNumPages);
    return 0;
  }
  i8 iPageSize = sysconf(_SC_PAGESIZE);
  if (iPageSize == -1) {
    vWarn("Error while getting physical memory %lld\n"_Stz, iPageSize);
    return 0;
  }

  i8 iMemQty = iNumPages * iPageSize;
  
  return iMemQty;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

