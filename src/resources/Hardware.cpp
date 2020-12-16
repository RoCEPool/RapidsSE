/*============================================================================*/
/* Hardware -- machine harware                                                */
/*============================================================================*/

#include "Resources.h"

/*----------------------------------------------------------------------------*/
/* get byte order                                                             */
/*----------------------------------------------------------------------------*/
i8 Hardware::iByteOrder(void)
{
  _static_

  i8 test = 0;
  char *bytes = (char *) &test;
  *bytes = 0x1;
  
  if (test == 1){
    return 1234;
  }
  else {
    return 4321;
  }

}

/*----------------------------------------------------------------------------*/
/* get size of cache line                                                     */
/*----------------------------------------------------------------------------*/
i8 Hardware::iCacheLine(void)
{
  _static_

#ifdef WITH_APPLE
  
  i8     iSize = 0;
  size_t len = sizeof(iSize);
  
  i8 iReturn = sysctlbyname("hw.cachelinesize", &iSize, &len, 0, 0);
  if (iReturn) {
    vWarn("Error while getting cache line size %lld\n"_Stz, iReturn);
    return 0;
  }
  return iSize;
  
#else
  
  i8 iSize = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);

  if (iSize == -1) {
    vWarn("sysconf(_SC_LEVEL1_DCACHE_LINESIZE)\n"_Stz, iSize);
    return 0;
  }

  return iSize;

#endif
  
}

/*----------------------------------------------------------------------------*/
/* get clock resolution (in nanoseconds)                                      */
/*----------------------------------------------------------------------------*/
i8 Hardware::iClockResNsec(void)
{
  _static_
  
  struct timespec tp;
  
  int iResult = clock_getres(CLOCK_REALTIME,&tp);
  
  return tp.tv_nsec;
}

/*----------------------------------------------------------------------------*/
/* get number of logical cores                                                */
/*----------------------------------------------------------------------------*/
i8 Hardware::iCoreCount(void)
{
  _static_
  
  i8 iCount = sysconf(_SC_NPROCESSORS_ONLN);
  if (iCount == -1) {
    vWarn("Error while getting cpu core count %lld\n"_Stz, iCount);
    return 0;
  }
 
  return iCount;
}

/*----------------------------------------------------------------------------*/
/* get the no of cou's on this machine                                        */
/*----------------------------------------------------------------------------*/
i8 Hardware::iCpuCount(void)
{
  _static_

#ifdef WITH_APPLE
  
  i8     iCount = 0;
  size_t len = sizeof(iCount);
  
  i8 iReturn = sysctlbyname("machdep.cpu.core_count", &iCount, &len, 0, 0);
  if (iReturn) {
    vWarn("Error while getting cpu count %lld\n"_Stz, iReturn);
    return 0;
  }
  return iCount;
  
#else
 
  i8 iCount = sysconf(_SC_NPROCESSORS_CONF);
   if (iCount == -1) {
     vWarn("Error while getting cpu count %lld\n"_Stz, iCount);
     return 0;
   }
  
   return iCount;
 
#endif
  
}

/*----------------------------------------------------------------------------*/
/* get cpu frequency of this machine                                          */
/*----------------------------------------------------------------------------*/
i8 Hardware::iCpuFrequency(void)
{
  _static_

#ifdef WITH_APPLE

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
  static const i8 C_BUFSIZ=512;
  char caBuf[C_BUFSIZ];
  double dFreq;
  i8 iFreq = 0;

  // First try to get the frequency from the file:
  // /sys/devices/system/cpu/cpu*/cpufreq/scaling_max_freq
  FILE* aFile = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq", "r");
  
  if (aFile) {
    if (fgets(caBuf, C_BUFSIZ, aFile) != NULL) {
      sscanf(caBuf, "%lld", &iFreq);
      iFreq *= C_1K; // value returned is in kHz
      fclose(aFile);
      return iFreq;
    }
    
    fclose(aFile);
  }

 
  // open the /Proc/cpuinfo file
  aFile = fopen("/proc/cpuinfo", "r");
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
  fclose(aFile);
  return iFreq;

#endif
  
}

/*----------------------------------------------------------------------------*/
/* get size of L1 cache per core on this machine                              */
/*----------------------------------------------------------------------------*/
i8 Hardware::iL1Cache(void)
{
  _static_

#ifdef WITH_APPLE
  
  i8     iSize = 0;
  size_t len = sizeof(iSize);
  
  i8 iReturn = sysctlbyname("hw.l1dcachesize", &iSize, &len, 0, 0);
  if (iReturn) {
    vWarn("Error while getting L1 cache size %lld\n"_Stz, iReturn);
    return 0;
  }
  return iSize;
  
#else

  i8 iSize = sysconf(_SC_LEVEL1_DCACHE_SIZE); 

  if (iSize == -1) {
    vWarn("sysconf(_SC_LEVEL1_DCACHE_SIZE)\n"_Stz, iSize);
    return 0;
  }

  return iSize;

#endif
  
}

/*----------------------------------------------------------------------------*/
/* get size of L2 cache per core on this machine                              */
/*----------------------------------------------------------------------------*/
i8 Hardware::iL2Cache(void)
{
  _static_

#ifdef WITH_APPLE
  
  i8     iSize = 0;
  size_t len = sizeof(iSize);
  
  i8 iReturn = sysctlbyname("hw.l2cachesize", &iSize, &len, 0, 0);
  if (iReturn) {
    vWarn("Error while getting L2 cache size %lld\n"_Stz, iReturn);
    return 0;
  }
  return iSize;
  
#else
  
  i8 iSize = sysconf(_SC_LEVEL2_CACHE_SIZE);

  if (iSize == -1) {
    vWarn("sysconf(_SC_LEVEL2_CACHE_SIZE)\n"_Stz, iSize);
    return 0;
  }

  return iSize;

#endif
  
}

/*----------------------------------------------------------------------------*/
/* get size of L3 cache on this machine                                       */
/*----------------------------------------------------------------------------*/
i8 Hardware::iL3Cache(void)
{
  _static_

#ifdef WITH_APPLE
  
  i8     iSize = 0;
  size_t len = sizeof(iSize);
  
  i8 iReturn = sysctlbyname("hw.l3cachesize", &iSize, &len, 0, 0);
  if (iReturn) {
    vWarn("Error while getting L3 cache size %lld\n"_Stz, iReturn);
    return 0;
  }
  return iSize;
  
#else
  
  i8 iSize = sysconf(_SC_LEVEL3_CACHE_SIZE);

  if (iSize == -1) {
    vWarn("sysconf(_SC_LEVEL3_CACHE_SIZE)\n"_Stz, iSize);
    return 0;
  }

  return iSize;

#endif
  
}

/*----------------------------------------------------------------------------*/
/* get amount of physical memory                                              */
/*----------------------------------------------------------------------------*/
i8 Hardware::iPhysicalMemory(void)
{
  _static_

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

/*----------------------------------------------------------------------------*/
/* read ticks from clock                                                      */
/*----------------------------------------------------------------------------*/
i8 Hardware::iReadClock(void)
{
  _static_

#ifdef WITH_APPLE
  struct timespec tp;
  int iResult = clock_gettime(CLOCK_MONOTONIC_RAW_APPROX,&tp);
  //int iResult = clock_gettime(CLOCK_REALTIME,&tp);

  i8 iTicks = tp.tv_sec * C_1G;
  iTicks += tp.tv_nsec;
  
#elif defined(WITH_LINUX)
  struct timeval iTimeval;
  
  if (gettimeofday(&iTimeval, NULL) < 0) {
    return 0;
  }
  i8 iTicks = iTimeval.tv_sec;
  iTicks *= C_1M;  // convert to micros
  iTicks += iTimeval.tv_usec;
  iTicks *= C_1K;  // convert to nanos
#endif
  
  return iTicks;
}

/*----------------------------------------------------------------------------*/
/* read ticks from hardware timestamp counter                                 */
/*----------------------------------------------------------------------------*/
i8 Hardware::iReadTscp(void)
{
  _static_
  
  /*
   Intel SDM says:
   The processor monotonically increments the time-stamp counter MSR
   every clock cycle and resets it to 0 whenever the processor is reset.
   -----------------
   
   The time stamp counter in newer processors may support an enhancement,
   referred to as invariant TSC. Processor’s support for invariant TSC is
   indicated by CPUID.80000007H:EDX[8].
   
   The invariant TSC will run at a constant rate in all ACPI P-, C-. and
   T-states. This is the architectural behavior moving forward.
   On processors with invariant TSC support, the OS may use the TSC for
   wall clock timer services (instead of ACPI or HPET timers). TSC reads
   are much more efficient and do not incur the overhead associated with
   a ring transition or access to a platform resource.
   */
  
  i8 iTscp = Resources::iRdtscp();
  return iTscp;
  
}


/*----------------------------------------------------------------------------*/
/* get get host name of this machine                                          */
/*----------------------------------------------------------------------------*/
void Hardware::vHostName(s32& sName)
{
  _static_
  
  i8 iResult = gethostname(sName.azChar(), 32);
  if (iResult)
    perror("gethostname");
}

/*----------------------------------------------------------------------------*/
/* get get login name                                                         */
/*----------------------------------------------------------------------------*/
void Hardware::vLoginName(s32& sName)
{
  _static_
  
  i8 iResult = getlogin_r(sName.azChar(), 32);
  if (iResult)
    perror("getlogin_r");
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

