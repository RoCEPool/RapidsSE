/*============================================================================*/
/* TickingDevice.cpp -- TickingDevice reader (Clock or Timestamp Counter)     */
/*============================================================================*/

#include "Resources.h"

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get ticks from the ticking device                                          */
/*----------------------------------------------------------------------------*/
i8 TickingDevice::iGetTick(void)
{
  _static_
  
  i8 iTicks = Hardware::iReadTscp();
  
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
  
  i8 iFrequency = Hardware::iCpuFrequency();

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
