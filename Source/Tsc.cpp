/*============================================================================*/
/* Tsc.cpp -- Time Stamp Counter                                              */
/*============================================================================*/

#include "System.h"

#include "Asm.h"

/*----------------------------------------------------------------------------*/
/* get ticks                                                                  */
/*----------------------------------------------------------------------------*/
i8 Tsc::iTicks(void)
{
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
/* get tick frequency                                                         */
/*----------------------------------------------------------------------------*/
i8 Tsc::iTicksPerSecond(void)
{
#if defined(WITH_APPLE) || defined(WITH_LINUX)
  i8 iFrequency = C_1G;
#else
  i8 iFrequency = System::iClockFrequency();
#endif
  
  return iFrequency;
}

/*----------------------------------------------------------------------------*/
/* convert tick delta to milliseconds                                         */
/*----------------------------------------------------------------------------*/
i8 Tsc::iToMsec(i8 iTickDelta)
{
  i8 iTicksPerMillisecond = iTicksPerSecond() / C_1K;
  
  i8 iMilliseconds = iTickDelta / iTicksPerMillisecond;
  
  return iMilliseconds;
}

/*----------------------------------------------------------------------------*/
/* convert tick delta to nanoseconds                                          */
/*----------------------------------------------------------------------------*/
i8 Tsc::iToNsec(i8 iTickDelta)
{
  i8 iTicksPerNanosecond = iTicksPerSecond() / C_1G;
  
  i8 iNanoseconds = iTickDelta / iTicksPerNanosecond;
  
  return iNanoseconds;
}

/*----------------------------------------------------------------------------*/
/* convert tick delta to microseconds                                         */
/*----------------------------------------------------------------------------*/
i8 Tsc::iToUsec(i8 iTickDelta)
{
  i8 iTicksPerMicrosecond = iTicksPerSecond() / C_1M;
  
  i8 iMicroseconds = iTickDelta / iTicksPerMicrosecond;
  
  return iMicroseconds;
}

/*----------------------------------------------------------------------------*/
/* read hardware timestamp counter                                            */
/*----------------------------------------------------------------------------*/
i8 Tsc::iRdTscp(void)
{
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
  
#ifdef WITH_APPLE
  
  i8 iTscp = iRdtscp();
  return iTscp;
  
#else
  
  return 0;
  
#endif
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
