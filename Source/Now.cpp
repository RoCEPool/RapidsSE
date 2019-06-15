/*============================================================================*/
/* Now.cpp - Now sprint helper                                                */
/*============================================================================*/

#include "Measurement.h"
#include "Values.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Now::Now(void)
: Object(CN_NOW)
{
  _precondition_(_MAGIC_,CN_NOW,_FOOTER_);
  
  // get current epoch time
  time_t iSecondsSincePosixEpoch = time(0);
  
  // convert current epoch time into LOCAL:date&time
  struct tm* aLocalDateTime = localtime(&iSecondsSincePosixEpoch);
  
  if (! aLocalDateTime)
    vWarn("localtime() error; errno=%d"_Stz,errno);
  
  _Tsp xTsp;
  
  xTsp.m_uYearDelta = aLocalDateTime->tm_year;
  xTsp.m_uMonth     = aLocalDateTime->tm_mon + 1;
  xTsp.m_uDay       = aLocalDateTime->tm_mday;
  xTsp.m_uHour      = aLocalDateTime->tm_hour;
  xTsp.m_uMinute    = aLocalDateTime->tm_min;
  xTsp.m_uSecond    = aLocalDateTime->tm_sec;
  xTsp.m_uMicro     = 0;
  
  m_iTsp = Dtm::iTsp(xTsp.m_uYearDelta+  C_YEAR_BASE,
                     xTsp.m_uMonth,
                     xTsp.m_uDay,
                     xTsp.m_uHour,
                     xTsp.m_uMinute,
                     xTsp.m_uSecond,
                     xTsp.m_uMicro);
  
  StrLib::vSprint(m_caResult,
                  (Stz*)F_TSP,
                  
                  (i8)xTsp.m_uYearDelta + C_YEAR_BASE,
                  (i8)xTsp.m_uMonth,
                  (i8)xTsp.m_uDay,
                  
                  (i8)xTsp.m_uHour,
                  (i8)xTsp.m_uMinute,
                  (i8)xTsp.m_uSecond);

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
/* sprint Tme and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* Now::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Now::aFooter(void)
{
  _precondition_(_MAGIC_,CN_NOW);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
