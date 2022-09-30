/*============================================================================*/
/* IvlLib.cpp -- Interval Library                                             */
/*============================================================================*/

#include "BASE.h"
#include "eIV.h"

#include <sys/sysctl.h>
#include <sys/time.h>

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add Ivl2 to Ivl1 and return the new interval                               */
/*----------------------------------------------------------------------------*/
Ivl IvlLib::wAdd(Ivl wIvl1, Ivl wIvl2)
{
  // add the two intervals
  u8 uMicro = wIvl1.T.m_xIvl.u.m_xIvlDT.m_uMicros + wIvl2.T.m_xIvl.u.m_xIvlDT.m_uMicros;
  
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
  // add the two intervals
  u8 uMicroTotal = wIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros + uMicro;
  
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
    vTrace("wGetCurrIvl: gettimeofday() error; errno=%d"_Stz,errno);
    wDtm.vSetUnknown();
    return wDtm.m_wIvl;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  wDtm.m_wIvl.T.m_xIvl.m_eType = IV_SECOND;
  wDtm.m_wIvl.T.m_xIvl.m_bIsNeg = false;
  wDtm.m_wIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros  = (xTimeVal.tv_sec * C_1M) + xTimeVal.tv_usec;

  return wDtm.m_wIvl;
}

/*----------------------------------------------------------------------------*/
/* convert timestamp to interval                                              */
/*----------------------------------------------------------------------------*/
Ivl IvlLib::wToIvl(Dtm wTsp)
{
  Ivl wIvl(IV_SECOND);
  
  struct tm xDateTime;
  
  xDateTime.tm_year = wTsp.m_wTsp.T.m_uYearDelta;
  xDateTime.tm_mon  = wTsp.m_wTsp.T.m_uMonth - 1;
  xDateTime.tm_mday = wTsp.m_wTsp.T.m_uDay;
  xDateTime.tm_hour = wTsp.m_wTsp.T.m_uHour;
  xDateTime.tm_min  = wTsp.m_wTsp.T.m_uMinute;
  xDateTime.tm_sec  = wTsp.m_wTsp.T.m_uSecond;
  
  RASSERT(wTsp.m_wTsp.T.m_uLocal == C_TSP_LOCAL);
  
  time_t iSecondsSincePosixEpoch = timelocal(&xDateTime);
  
  wIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros  = (iSecondsSincePosixEpoch * C_1M) + wTsp.m_wTsp.T.m_uMicro;
  
  return wIvl;
}

/*----------------------------------------------------------------------------*/
/* convert microseconds to interval                                           */
/*----------------------------------------------------------------------------*/
Ivl IvlLib::wToIvl(u8 uMicroseconds)
{
  Ivl wIvl(IV_SECOND);

  wIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros = uMicroseconds;
  return wIvl;
}

/*----------------------------------------------------------------------------*/
/* subtract Ivl2 from Ivl1 and return the microseconds                        */
/*----------------------------------------------------------------------------*/
u8 IvlLib::uSubtract(Ivl wIvl1, Ivl wIvl2)
{
  // subtract
  u8 uMicro = wIvl1.T.m_xIvl.u.m_xIvlDT.m_uMicros - wIvl2.T.m_xIvl.u.m_xIvlDT.m_uMicros;
  
  return uMicro;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
