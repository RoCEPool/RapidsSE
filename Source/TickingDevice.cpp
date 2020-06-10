/*============================================================================*/
/* TickingDevice.cpp -- TickingDevice reader (Clock or Timestamp Counter)     */
/*============================================================================*/

#include "Gadgets.h"

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get ticks from the ticking device                                          */
/*----------------------------------------------------------------------------*/
i8 TickingDevice::iGetTick(void)
{
  _static_
  
#ifdef WITH_APPLE
  i8 iTicks = Hardware::iReadTscp();
#elif defined(WITH_LINUX)
  i8 iTicks = Hardware::iReadClock();
#else
  NEVER_REACH;
#endif
  
  return iTicks;
}

/*----------------------------------------------------------------------------*/
/* get tick frequency                                                         */
/*----------------------------------------------------------------------------*/
i8 TickingDevice::iTicksPerSec(void)
{
  _static_
 
  static i8 iFreq = 0;

  if (iFreq)
    return iFreq;
  
#ifdef WITH_APPLE
  i8 iFrequency = Hardware::iCpuFrequency();
#elif defined(WITH_LINUX)
  i8 iFrequency = Hardware::iClockResNsec();
#else
  NEVER_REACH;
#endif

  iFreq = iFrequency;
  
  return iFrequency;
}

/*----------------------------------------------------------------------------*/
/* convert tick delta to milliseconds                                         */
/*----------------------------------------------------------------------------*/
i8 TickingDevice::iToMsec(i8 iTickDelta)
{
  _static_
  
  i8 iTicksPerMillisecond = iTicksPerSec() / C_1K;
  
  i8 iMilliseconds = iTickDelta / iTicksPerMillisecond;
  
  return iMilliseconds;
}

/*----------------------------------------------------------------------------*/
/* convert tick delta to nanoseconds                                          */
/*----------------------------------------------------------------------------*/
i8 TickingDevice::iToNsec(i8 iTickDelta)
{
  _static_
  
  i8 iTicksPerNanosecond = iTicksPerSec() / C_1G;
  
  i8 iNanoseconds = iTickDelta / iTicksPerNanosecond;
  
  return iNanoseconds;
}

/*----------------------------------------------------------------------------*/
/* convert tick delta to microseconds                                         */
/*----------------------------------------------------------------------------*/
i8 TickingDevice::iToUsec(i8 iTickDelta)
{
  _static_
  
  i8 iTicksPerMicrosecond = iTicksPerSec() / C_1M;
  
  i8 iMicroseconds = iTickDelta / iTicksPerMicrosecond;
  
  return iMicroseconds;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
