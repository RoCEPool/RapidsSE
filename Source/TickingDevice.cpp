/*============================================================================*/
/* TickingDevice.cpp -- TickingDevice reader (Clock or Timestamp Counter)     */
/*============================================================================*/

#include "Gadgets.h"

#include "Asm.h"

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get ticks from the ticking device                                          */
/*----------------------------------------------------------------------------*/
i8 TickingDevice::iGetTick(void)
{
  _static_
  
  i8 iTicks;
  
#ifdef WITH_APPLE
  iTicks = iReadTscp();
#elif defined(WITH_LINUX)
  iTicks = iReadClock();
#else
  NEVER_REACH;
#endif
  
  return iTicks;
}

/*----------------------------------------------------------------------------*/
/* get tick frequency                                                         */
/*----------------------------------------------------------------------------*/
i8 TickingDevice::iTicksPerSecond(void)
{
  _static_
  
  
#ifdef WITH_APPLE
  i8 iFrequency = Hardware::iCpuFrequency();
#elif defined(WITH_LINUX)
  i8 iFrequency = Hardware::iClockFrequency();
#else
  NEVER_REACH;
#endif

  return iFrequency;
}

/*----------------------------------------------------------------------------*/
/* convert tick delta to milliseconds                                         */
/*----------------------------------------------------------------------------*/
i8 TickingDevice::iToMsec(i8 iTickDelta)
{
  _static_
  
  i8 iTicksPerMillisecond = iTicksPerSecond() / C_1K;
  
  i8 iMilliseconds = iTickDelta / iTicksPerMillisecond;
  
  return iMilliseconds;
}

/*----------------------------------------------------------------------------*/
/* convert tick delta to nanoseconds                                          */
/*----------------------------------------------------------------------------*/
i8 TickingDevice::iToNsec(i8 iTickDelta)
{
  _static_
  
  i8 iTicksPerNanosecond = iTicksPerSecond() / C_1G;
  
  i8 iNanoseconds = iTickDelta / iTicksPerNanosecond;
  
  return iNanoseconds;
}

/*----------------------------------------------------------------------------*/
/* convert tick delta to microseconds                                         */
/*----------------------------------------------------------------------------*/
i8 TickingDevice::iToUsec(i8 iTickDelta)
{
  _static_
  
  i8 iTicksPerMicrosecond = iTicksPerSecond() / C_1M;
  
  i8 iMicroseconds = iTickDelta / iTicksPerMicrosecond;
  
  return iMicroseconds;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* read ticks from clock                                                      */
/*----------------------------------------------------------------------------*/
i8 TickingDevice::iReadClock(void)
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
i8 TickingDevice::iReadTscp(void)
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
  
#ifdef WITH_APPLE
  
  i8 iTscp = Assembler::iRdtscp();
  return iTscp;
  
#else
  
  return 0;
  
#endif
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
