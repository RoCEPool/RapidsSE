/*============================================================================*/
/* Hardware.cpp -- machine harware                                            */
/*============================================================================*/

#include "BASE.h"

#ifdef WITH_APPLE
#include <sys/sysctl.h>
#else
#include <linux/sysctl.h>
#endif

#include <sys/utsname.h>
#include <sys/time.h>
#include <x86intrin.h>


// The variable that holds the default clock type to be used on this machine
static clockid_t S_clockSource = CLOCK_MONOTONIC_RAW;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
    vTrace("Error while getting cache line size %lld\n"_Stz, iReturn);
    return 0;
  }
  return iSize;
  
#else
  
  i8 iSize = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);

  if (iSize == -1) {
    vTrace("sysconf(_SC_LEVEL1_DCACHE_LINESIZE)\n"_Stz, iSize);
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
  UNUSED(iResult);

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
    vTrace("Error while getting cpu core count %lld\n"_Stz, iCount);
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
    vTrace("Error while getting cpu count %lld\n"_Stz, iReturn);
    return 0;
  }
  return iCount;
  
#else
 
  i8 iCount = sysconf(_SC_NPROCESSORS_CONF);
   if (iCount == -1) {
     vTrace("Error while getting cpu count %lld\n"_Stz, iCount);
     return 0;
   }
  
   return iCount;
 
#endif
  
}

/*----------------------------------------------------------------------------*/
/* get cpu frequency of this machine                                          */
/* Note: Do not use for trying to convert TSC (from rdtscp) to nanoseconds.   */
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
  //
  // Please note that this file does not exist in virtualised environments such as AWS.
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

 
  // Open the /Proc/cpuinfo file
  //
  // Please note that this frequency changes dynamically depending on the load
  // on the machine and any thermal throttling that may be going on.
  aFile = fopen("/proc/cpuinfo", "r");
  if (! aFile) {
    vTrace("File::bOpen: fopen(/proc/cpuinfo,r) error; errno=%d"_Stz, errno);
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
    vTrace("Error while getting L1 cache size %lld\n"_Stz, iReturn);
    return 0;
  }
  return iSize;
  
#else

  i8 iSize = sysconf(_SC_LEVEL1_DCACHE_SIZE); 

  if (iSize == -1) {
    vTrace("sysconf(_SC_LEVEL1_DCACHE_SIZE)\n"_Stz, iSize);
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
    vTrace("Error while getting L2 cache size %lld\n"_Stz, iReturn);
    return 0;
  }
  return iSize;
  
#else
  
  i8 iSize = sysconf(_SC_LEVEL2_CACHE_SIZE);

  if (iSize == -1) {
    vTrace("sysconf(_SC_LEVEL2_CACHE_SIZE)\n"_Stz, iSize);
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
    // doesnt seem to work on Mac ARM
    //vTrace("Error while getting L3 cache size %lld\n"_Stz, iReturn);
    return 0;
  }
  return iSize;
  
#else
  
  i8 iSize = sysconf(_SC_LEVEL3_CACHE_SIZE);

  if (iSize == -1) {
    vTrace("sysconf(_SC_LEVEL3_CACHE_SIZE)\n"_Stz, iSize);
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
    vTrace("Error while getting physical memory %lld\n"_Stz, iNumPages);
    return 0;
  }
  i8 iPageSize = sysconf(_SC_PAGESIZE);
  if (iPageSize == -1) {
    vTrace("Error while getting physical memory %lld\n"_Stz, iPageSize);
    return 0;
  }

  i8 iMemQty = iNumPages * iPageSize;
  
  return iMemQty;
}

/*----------------------------------------------------------------------------*/
/* read nanoseconds from clock                                                */
/*----------------------------------------------------------------------------*/
u8 Hardware::uReadClock(void)
{
  _static_

  struct timespec ts{0, 0};

  // Note: We have already chosen the type of clock to use for this machine.
  // Typically we want CLOCK_MONOTONIC_RAW, but in some cases we need to use
  // CLOCK_MONOTONIC instead. See the function xChooseClock() at the top of file.

  int iResult = clock_gettime(S_clockSource, &ts);
  UNUSED(iResult);

  u8 uNanos = (ts.tv_sec * (u8)C_1G) + ts.tv_nsec;

  return uNanos;
}

/*----------------------------------------------------------------------------*/
/* Read ticks from hardware timestamp counter (TSC). x86 only                 */
/* Note: on a machine with more than one physical CPU, each CPU maintains its */
/*       own TSC. Comparing two TSC values may result in abnormal tick        */
/*       differences because the TSC on each CPU are not in sync. This can    */
/*       result in a negative result or a larger than expected positive       */
/*       difference.                                                          */
/*----------------------------------------------------------------------------*/
i8 Hardware::iReadTsc(u4* puLogicalProcessorId)
{
  _static_

  i8 iTscp = (i8) uReadClock();

  * puLogicalProcessorId = 0;

  return iTscp;
}
/*----------------------------------------------------------------------------*/
//i8 Hardware::iReadTsc(u4* puLogicalProcessorId)
//{
//  _static_
//
//  /*
//   Intel SDM says:
//   The processor monotonically increments the time-stamp counter MSR
//   every clock cycle and resets it to 0 whenever the processor is reset.
//   -----------------
//
//   Note however, that the frequency that the TSC is updated is not necessarily
//   related to the max or base frequency of the processor. It can be wildly
//   different. The Linux kernel will determine this TSC update frequency at
//   boot time experimentally if it cannot retrieve it from the processor
//   directly. This TSC update frequency is not available in userspace unfortunately
//   so it is not possible to accurately convert TSC differences to a time
//   difference. Instead, simply compare ticks with ticks.
//
//   The time stamp counter in newer processors may support an enhancement,
//   referred to as invariant TSC. Processor’s support for invariant TSC is
//   indicated by CPUID.80000007H:EDX[8]. On Linux you can also see this with
//   the flag "constant_tsc" in /proc/cpuinfo.
//
//   The invariant TSC will run at a constant rate in all ACPI P-, C-. and
//   T-states. This is the architectural behavior moving forward. This can also
//   be seen on Linux by the presence of the flag "nonstop_tsc" in /proc/cpuinfo.
//   On processors with invariant TSC support, the OS may use the TSC for
//   wall clock timer services (instead of ACPI or HPET timers). TSC reads
//   are much more efficient and do not incur the overhead associated with
//   a ring transition or access to a platform resource.
//   */
//
//  i8 iTscp = (i8) __rdtscp(puLogicalProcessorId);
//  _mm_lfence();
//  return iTscp;
//}

/*----------------------------------------------------------------------------*/
/* Choose the appropriate default clock type to use for all measurements      */
/*----------------------------------------------------------------------------*/
void Hardware::vChooseClockType(void)
{
  struct utsname xBuf{};
  i8 iResult = uname(&xBuf);

#ifdef WITH_APPLE
  UNUSED(iResult);
#endif
#ifdef WITH_LINUX
  if (iResult != 0) {
    perror("Unable to get the kernel version information while choosing a clock type. "
           "Falling back to CLOCK_MONOTONIC ");
    S_clockSource = CLOCK_MONOTONIC;
    return;
  }
#endif

#ifdef WITH_APPLE
  // On MacOs we always want to use CLOCK_MONOTONIC_RAW.
  // It's performance is adequate (~30ns/call)
#ifdef WITH_DEBUG_VERSION
  vTrace("Using clock type CLOCK_MONOTONIC_RAW on MacOS (kernel %s).\n"_Stz, xBuf.release);
#endif
  S_clockSource = CLOCK_MONOTONIC_RAW;

#endif

#ifdef WITH_LINUX
  // On Linux we need to be careful which clock we choose.
  // Kernel 2.6 introduced vDSO which eliminates many system calls when
  // calling gettimeofday() and clock_gettime(), however this was not
  // implemented for CLOCK_MONOTONIC_RAW until kernel 5.3.
  //
  // So if the kernel is less than 5.3 we will fall back to CLOCK_MONOTONIC
  // instead. The MONOTONIC clock doesn't step (e.g. DST) but it does slew
  // (e.g. NTP), but it has the advantage that it doesn't invoke a system call
  // so it is still pretty fast.
  //
  // If the kernel is >= 5.3  then we can use CLOCK_MONOTONIC_RAW as desired.

  char* psChar = xBuf.release;
  i8 iMajor = 0;
  i8 iMinor = 0;
  i8 nDigits = 0;
  bool bOk;

  bOk = StrLib::bExtractInteger(&psChar, &iMajor, nDigits);
  if (!bOk) {
    vTrace("Unable to extract Linux kernel major version while choosing a clock type (kernel %s). "
           "Falling back to CLOCK_MONOTONIC."_Stz, xBuf.release);
    S_clockSource = CLOCK_MONOTONIC;
    return;
  }

  RASSERT(*psChar == '.');
  ++psChar;

  bOk = StrLib::bExtractInteger(&psChar, &iMinor, nDigits);
  if (!bOk) {
    vTrace("Unable to extract Linux kernel minor version while choosing a clock type (kernel %s). "
           "Falling back to CLOCK_MONOTONIC."_Stz, xBuf.release);
    S_clockSource = CLOCK_MONOTONIC;
    return;
  }

  if (iMajor >= 5 && iMinor >= 3) {
#ifdef WITH_DEBUG_VERSION
    vTrace("Using clock type CLOCK_MONOTONIC_RAW on Linux kernel %lld.%lld.\n"_Stz, iMajor, iMinor);
#endif
    S_clockSource = CLOCK_MONOTONIC_RAW;
    return;
  }

  // The default safe case, though this does include slewing (e.g. NTP).
#ifdef WITH_DEBUG_VERSION
  vTrace("Using clock type CLOCK_MONOTONIC on Linux kernel %lld.%lld.\n"_Stz, iMajor, iMinor);
#endif
  S_clockSource = CLOCK_MONOTONIC;
#endif
}

/*----------------------------------------------------------------------------*/
/* get the host name of this machine                                          */
/*----------------------------------------------------------------------------*/
void Hardware::vHostName(s32& sName)
{
  _static_
  
  i8 iResult = gethostname(sName.azChar(), 32);
  if (iResult)
    perror("gethostname");
}

/*----------------------------------------------------------------------------*/
/* get the login name                                                         */
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

