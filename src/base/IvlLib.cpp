/*============================================================================*/
/* IvlLib.cpp -- Interval Library                                             */
/*============================================================================*/

#include "Resources.h"

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add Ivl2 to Ivl1 and return the new interval                               */
/*----------------------------------------------------------------------------*/
Ivl IvlLib::wAdd(Ivl wIvl1, Ivl wIvl2)
{
  // convert wIvl1 to microseconds
  u8 uMicro1 = (wIvl1.m_uSecond * C_1M) + wIvl1.m_uMicro;
  
  // convert wIvl2 to microseconds
  u8 uMicro2 = (wIvl2.m_uSecond * C_1M) + wIvl2.m_uMicro;
  
  // add the two intervals
  u8 uMicro = uMicro1 + uMicro2;
  
  // convert microseconds to Ivl
  Dtm xIvl;
  
  xIvl.m_wIvl = wToIvl(uMicro);
  
  return xIvl.m_wIvl;
}

/*----------------------------------------------------------------------------*/
/* add Ivl2 to Ivl1 and return the new interval                               */
/*----------------------------------------------------------------------------*/
Ivl IvlLib::wAdd(Ivl wIvl, u8 uMicro)
{
  // convert wIvl1 to microseconds
  u8 uIvlMicro = (wIvl.m_uSecond * C_1M) + wIvl.m_uMicro;
  
  // add the two intervals
  u8 uMicroTotal = uIvlMicro + uMicro;
  
  // convert microseconds to Ivl
  Dtm xIvl;
  
  xIvl.m_wIvl = wToIvl(uMicroTotal);
  
  return xIvl.m_wIvl;
}

/*----------------------------------------------------------------------------*/
/* get current system/epoch interval                                          */
/*----------------------------------------------------------------------------*/
Ivl IvlLib::wGetCurrIvl(void)
{
  Dtm wDtm;
  
  // get seconds & microseconds since Posix Epoch January 1, 1970
  struct timeval xTimeVal;
  i8 rCode = gettimeofday(&xTimeVal,NULL);
  
  if (rCode) {
    vWarn("wGetCurrIvl: gettimeofday() error; errno=%d"_Stz,errno);
    wDtm.vSetUnknown();
    return wDtm.m_wIvl;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  wDtm.m_wIvl.m_uSecond = xTimeVal.tv_sec;
  wDtm.m_wIvl.m_uMicro  = xTimeVal.tv_usec;
  
  ASSERT(wDtm.m_wIvl.m_uMicro < C_1M); // logic check

  return wDtm.m_wIvl;
}

/*----------------------------------------------------------------------------*/
/* convert timestamp to interval                                              */
/*----------------------------------------------------------------------------*/
Ivl IvlLib::wToIvl(Dtm wTsp)
{
  Ivl wIvl;
  
  struct tm xDateTime;
  
  xDateTime.tm_year = wTsp.m_wTsp.m_uYearDelta;
  xDateTime.tm_mon  = wTsp.m_wTsp.m_uMonth - 1;
  xDateTime.tm_mday = wTsp.m_wTsp.m_uDay;
  xDateTime.tm_hour = wTsp.m_wTsp.m_uHour;
  xDateTime.tm_min  = wTsp.m_wTsp.m_uMinute;
  xDateTime.tm_sec  = wTsp.m_wTsp.m_uSecond;
  
  RASSERT(wTsp.m_wTsp.m_uLocal == C_TSP_LOCAL);
  
  time_t iSecondsSincePosixEpoch = timelocal(&xDateTime);
  
  wIvl.m_uSecond = iSecondsSincePosixEpoch;
  wIvl.m_uMicro = wTsp.m_wTsp.m_uMicro;
  
  return wIvl;
}

/*----------------------------------------------------------------------------*/
/* convert microseconds to interval                                           */
/*----------------------------------------------------------------------------*/
Ivl IvlLib::wToIvl(u8 uMicroseconds)
{
  Ivl wIvl;
  
  wIvl = 0;
  
  if (uMicroseconds < C_1M) {
    wIvl.m_uMicro = uMicroseconds;
    return wIvl;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // correction needed
  
  u8 uMultiples = uMicroseconds;
  
  wIvl.m_uMicro = uMultiples % C_1M;
  
  u8 uSecondsDelta = uMultiples / C_1M;
  
  wIvl.m_uSecond += uSecondsDelta;
  
  return wIvl;
}

/*----------------------------------------------------------------------------*/
/* subtract Ivl2 from Ivl1 and return the microseconds                        */
/*----------------------------------------------------------------------------*/
u8 IvlLib::uSubtract(Ivl wIvl1, Ivl wIvl2)
{
  // convert wIvl1 to microseconds
  u8 uMicro1 = (wIvl1.m_uSecond * C_1M) + wIvl1.m_uMicro;
  
  // convert wIvl2 to microseconds
  u8 uMicro2 = (wIvl2.m_uSecond * C_1M) + wIvl1.m_uMicro;
  
  // subtract
  u8 uMicro = uMicro1 - uMicro2;
  
  return uMicro;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
