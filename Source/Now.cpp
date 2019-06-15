/*============================================================================*/
/* Now.cpp - Now sprint helper                                                */
/*============================================================================*/

#include "Measurement.h"

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
  
  m_oTsp.xTsp.m_uYearDelta = aLocalDateTime->tm_year;
  m_oTsp.xTsp.m_uMonth     = aLocalDateTime->tm_mon + 1;
  m_oTsp.xTsp.m_uDay       = aLocalDateTime->tm_mday;
  m_oTsp.xTsp.m_uHour      = aLocalDateTime->tm_hour;
  m_oTsp.xTsp.m_uMinute    = aLocalDateTime->tm_min;
  m_oTsp.xTsp.m_uSecond    = aLocalDateTime->tm_sec;
  m_oTsp.xTsp.m_uMicro     = 0;
  
  if (m_oTsp.xTsp.m_uMicro) {
    
    char caFraction[16];
    StrLib::vFormatFraction(caFraction,m_oTsp.xTsp.m_uMicro,6);
    
    StrLib::vSprint(m_caResult,
                    (Stz*)F_TSP_frac,
                    
                    (i8)m_oTsp.xTsp.m_uYearDelta + C_YEAR_BASE,
                    (i8)m_oTsp.xTsp.m_uMonth,
                    (i8)m_oTsp.xTsp.m_uDay,
                    
                    (i8)m_oTsp.xTsp.m_uHour,
                    (i8)m_oTsp.xTsp.m_uMinute,
                    (i8)m_oTsp.xTsp.m_uSecond,
                    
                    caFraction);
  }
  else {
    StrLib::vSprint(m_caResult,
                    (Stz*)F_TSP,
                    
                    (i8)m_oTsp.xTsp.m_uYearDelta + C_YEAR_BASE,
                    (i8)m_oTsp.xTsp.m_uMonth,
                    (i8)m_oTsp.xTsp.m_uDay,
                    
                    (i8)m_oTsp.xTsp.m_uHour,
                    (i8)m_oTsp.xTsp.m_uMinute,
                    (i8)m_oTsp.xTsp.m_uSecond);
  }


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
