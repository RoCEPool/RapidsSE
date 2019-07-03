/*============================================================================*/
/* IvlLib.cpp -- Interval Library                                             */
/*============================================================================*/

#include "System.h"

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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

  time_t iSecondsSincePosixEpoch = timegm(&xDateTime);
  
  wIvl.xIvl.m_uSecond = iSecondsSincePosixEpoch;
  wIvl.xIvl.m_uSecond = wTsp.xTsp.m_uMicro;
  
  return wIvl;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
