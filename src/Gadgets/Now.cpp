/*============================================================================*/
/* Now.cpp - Now sprint helper                                                */
/*============================================================================*/

#include "Gadgets.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Gadgets,Now,CN_NOW)
__aNewFunction(Now,CN_NOW)
__vDeleteFunction(Now,CN_NOW)
__aFooterFunction(Now)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Now::Now(void)
: Object(CN_NOW)
{
  _precondition_(_MAGIC_,CN_NOW,_FOOTER_);
  
  MemLib::vZero(m_caResult,sizeof(m_caResult),CN_NOW);
  
  vSetNow();
  
  _checkInitialised_(CN_NOW);
  
  _postcondition_(_MAGIC_,CN_NOW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Now::~Now(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_NOW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
void Now::vSetNow(void)
{
  _precondition_(_MAGIC_,CN_NOW,_FOOTER_);
  
  // get current epoch time
  time_t iSecondsSincePosixEpoch = time(0);
  
  m_iSinceEpoch = iSecondsSincePosixEpoch;
  
  // convert current epoch time into LOCAL:date&time
  struct tm* aLocalDateTime = localtime(&iSecondsSincePosixEpoch);
  
  if (! aLocalDateTime)
    vWarn("localtime() error; errno=%d"_Stz,errno);
  
  m_wTsp.m_uYearDelta = aLocalDateTime->tm_year;
  m_wTsp.m_uMonth     = aLocalDateTime->tm_mon + 1;
  m_wTsp.m_uDay       = aLocalDateTime->tm_mday;
  m_wTsp.m_uHour      = aLocalDateTime->tm_hour;
  m_wTsp.m_uMinute    = aLocalDateTime->tm_min;
  m_wTsp.m_uSecond    = aLocalDateTime->tm_sec;
  m_wTsp.m_uLocal     = C_TSP_LOCAL;
  m_wTsp.m_uMicro     = 0;
  
  StrLib::vSprint(m_caResult,
                  (Stz*)F_NOW,
                  
                  (i8)m_wTsp.m_uYearDelta + C_YEAR_BASE,
                  (i8)m_wTsp.m_uMonth,
                  (i8)m_wTsp.m_uDay,
                  
                  (i8)m_wTsp.m_uHour,
                  (i8)m_wTsp.m_uMinute,
                  (i8)m_wTsp.m_uSecond);
  
  _checkInitialised_(CN_NOW);
  
  _postcondition_(_MAGIC_,CN_NOW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
void Now::vSetNowUTC(void)
{
  _precondition_(_MAGIC_,CN_NOW,_FOOTER_);
  
  // get current epoch time
  time_t iSecondsSincePosixEpoch = time(0);
  
  m_iSinceEpoch = iSecondsSincePosixEpoch;
  
  // convert secondsSince into a tm structure for UTC/GMT time zone
  struct tm* aUtcDateTime = gmtime(&iSecondsSincePosixEpoch);
  
  if (! aUtcDateTime)
    vWarn("gmtime() error; errno=%d"_Stz,errno);
  
  m_wTsp.m_uYearDelta = aUtcDateTime->tm_year;
  m_wTsp.m_uMonth     = aUtcDateTime->tm_mon + 1;
  m_wTsp.m_uDay       = aUtcDateTime->tm_mday;
  m_wTsp.m_uHour      = aUtcDateTime->tm_hour;
  m_wTsp.m_uMinute    = aUtcDateTime->tm_min;
  m_wTsp.m_uSecond    = aUtcDateTime->tm_sec;
  m_wTsp.m_uLocal     = C_TSP_UTC;
  m_wTsp.m_uMicro     = 0;
  
  StrLib::vSprint(m_caResult,
                  (Stz*)F_NOW,
                  
                  (i8)m_wTsp.m_uYearDelta + C_YEAR_BASE,
                  (i8)m_wTsp.m_uMonth,
                  (i8)m_wTsp.m_uDay,
                  
                  (i8)m_wTsp.m_uHour,
                  (i8)m_wTsp.m_uMinute,
                  (i8)m_wTsp.m_uSecond);
  
  _checkInitialised_(CN_NOW);
  
  _postcondition_(_MAGIC_,CN_NOW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* sprint Tme and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
i8 Now::iSinceEpoch(void)
{
  _precondition_(_MAGIC_,CN_NOW,_FOOTER_);
  
  return m_iSinceEpoch;
}

/*----------------------------------------------------------------------------*/
/* sprint Tme and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* Now::sz(void)
{
  _precondition_(_MAGIC_,CN_NOW,_FOOTER_);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Tme and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* Now::szPath(void)
{
  _precondition_(_MAGIC_,CN_NOW,_FOOTER_);
  
  // get current epoch time
   time_t iSecondsSincePosixEpoch = time(0);
   
   m_iSinceEpoch = iSecondsSincePosixEpoch;
   
   // convert current epoch time into LOCAL:date&time
   struct tm* aLocalDateTime = localtime(&iSecondsSincePosixEpoch);
   
   if (! aLocalDateTime)
     vWarn("localtime() error; errno=%d"_Stz,errno);
   
   m_wTsp.m_uYearDelta = aLocalDateTime->tm_year;
   m_wTsp.m_uMonth     = aLocalDateTime->tm_mon + 1;
   m_wTsp.m_uDay       = aLocalDateTime->tm_mday;
   m_wTsp.m_uHour      = aLocalDateTime->tm_hour;
   m_wTsp.m_uMinute    = aLocalDateTime->tm_min;
   m_wTsp.m_uSecond    = aLocalDateTime->tm_sec;
   m_wTsp.m_uLocal     = C_TSP_LOCAL;
   m_wTsp.m_uMicro     = 0;

  StrLib::vSprint(m_caResult,
                  (Stz*)F_NOW_path,
                  
                  (i8)m_wTsp.m_uYearDelta + C_YEAR_BASE,
                  (i8)m_wTsp.m_uMonth,
                  (i8)m_wTsp.m_uDay,
                  
                  (i8)m_wTsp.m_uHour,
                  (i8)m_wTsp.m_uMinute,
                  (i8)m_wTsp.m_uSecond);

  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* set time using tsp                                                         */
/*----------------------------------------------------------------------------*/
Stz* Now::sz(Dtm* aTsp)
{
  _precondition_(_MAGIC_,CN_NOW,_FOOTER_);
  
  StrLib::vSprint(m_caResult,
                  (Stz*)F_NOW,
                  
                  (i8)aTsp->m_wTsp.m_uYearDelta + C_YEAR_BASE,
                  (i8)aTsp->m_wTsp.m_uMonth,
                  (i8)aTsp->m_wTsp.m_uDay,
                  
                  (i8)aTsp->m_wTsp.m_uHour,
                  (i8)aTsp->m_wTsp.m_uMinute,
                  (i8)aTsp->m_wTsp.m_uSecond);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* get time as 8B word                                                        */
/*----------------------------------------------------------------------------*/
i8 Now::wTsp(void)
{
  _precondition_(_MAGIC_,CN_NOW,_FOOTER_);
  
  return m_wTsp.m_iAsI8;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
