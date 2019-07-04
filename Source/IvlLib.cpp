/*============================================================================*/
/* IvlLib.cpp -- Interval Library                                             */
/*============================================================================*/

#include "System.h"

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add Ivl2 to Ivl1 and return the new interval                               */
/*----------------------------------------------------------------------------*/
Dtm IvlLib::wAdd(Dtm wIvl1, Dtm wIvl2)
{
  // convert wIvl1 to microseconds
  u8 uMicro1 = (wIvl1.xIvl.m_uSecond * C_1M) + wIvl1.xIvl.m_uMicro;
  
  // convert wIvl2 to microseconds
  u8 uMicro2 = (wIvl2.xIvl.m_uSecond * C_1M) + wIvl2.xIvl.m_uMicro;
  
  // add the two intervals
  u8 uMicro = uMicro1 + uMicro2;
  
  // convert microseconds to Ivl
  Dtm xIvl = wToIvl(uMicro);
  
  return xIvl;
}

/*----------------------------------------------------------------------------*/
/* add Ivl2 to Ivl1 and return the new interval                               */
/*----------------------------------------------------------------------------*/
Dtm IvlLib::wAdd(Dtm wIvl, u8 uMicro)
{
  // convert wIvl1 to microseconds
  u8 uIvlMicro = (wIvl.xIvl.m_uSecond * C_1M) + wIvl.xIvl.m_uMicro;
  
  // add the two intervals
  u8 uMicroTotal = uIvlMicro + uMicro;
  
  // convert microseconds to Ivl
  Dtm xIvl = wToIvl(uMicroTotal);
  
  return xIvl;
}

/*----------------------------------------------------------------------------*/
/* get current system/epoch interval                                          */
/*----------------------------------------------------------------------------*/
Dtm IvlLib::wGetCurrIvl(void)
{
  Dtm wDtm;
  
  // get seconds & microseconds since Posix Epoch January 1, 1970
  struct timeval xTimeVal;
  i8 rCode = gettimeofday(&xTimeVal,NULL);
  
  if (rCode) {
    vWarn("wGetCurrIvl: gettimeofday() error; errno=%d"_Stz,errno);
    wDtm.vSetUnknown();
    return wDtm;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  wDtm.xIvl.m_uSecond = xTimeVal.tv_sec;
  wDtm.xIvl.m_uMicro  = xTimeVal.tv_usec;
  
  ASSERT(wDtm.xIvl.m_uMicro < C_1M); // logic check

  return wDtm;
}

/*----------------------------------------------------------------------------*/
/* convert timestamp to interval                                              */
/*----------------------------------------------------------------------------*/
Dtm IvlLib::wToIvl(Dtm wTsp)
{
  Dtm wIvl;
  
  struct tm xDateTime;
  
  xDateTime.tm_year = wTsp.xTsp.m_uYearDelta;
  xDateTime.tm_mon  = wTsp.xTsp.m_uMonth - 1;
  xDateTime.tm_mday = wTsp.xTsp.m_uDay;
  xDateTime.tm_hour = wTsp.xTsp.m_uHour;
  xDateTime.tm_min  = wTsp.xTsp.m_uMinute;
  xDateTime.tm_sec  = wTsp.xTsp.m_uSecond;
  
  RASSERT(wTsp.xTsp.m_uLocal == C_TSP_LOCAL);
  
  time_t iSecondsSincePosixEpoch = timelocal(&xDateTime);
  
  wIvl.xIvl.m_uSecond = iSecondsSincePosixEpoch;
  wIvl.xIvl.m_uMicro = wTsp.xTsp.m_uMicro;
  
  return wIvl;
}

/*----------------------------------------------------------------------------*/
/* convert microseconds to interval                                           */
/*----------------------------------------------------------------------------*/
Dtm IvlLib::wToIvl(u8 uMicroseconds)
{
  Dtm wIvl;
  
  if (uMicroseconds < C_1M) {
    wIvl.xIvl.m_uMicro = uMicroseconds;
    return wIvl;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // correction needed
  
  u8 uMultiples = uMicroseconds;
  
  wIvl.xIvl.m_uMicro = uMultiples % C_1M;
  
  u8 uSecondsDelta = uMultiples / C_1M;
  
  wIvl.xIvl.m_uSecond += uSecondsDelta;
  
  return wIvl;
}

/*----------------------------------------------------------------------------*/
/* subtract Ivl2 from Ivl1 and return the microseconds                        */
/*----------------------------------------------------------------------------*/
u8 IvlLib::uSubtract(Dtm wIvl1, Dtm wIvl2)
{
  // convert wIvl1 to microseconds
  u8 uMicro1 = (wIvl1.xIvl.m_uSecond * C_1M) + wIvl1.xIvl.m_uMicro;
  
  // convert wIvl2 to microseconds
  u8 uMicro2 = (wIvl2.xIvl.m_uSecond * C_1M) + wIvl1.xIvl.m_uMicro;
  
  // subtract
  u8 uMicro = uMicro1 - uMicro2;
  
  return uMicro;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
