/*============================================================================*/
/* spTSP.cpp - Tsp sprint helper                                              */
/*============================================================================*/

#include "Sprinting.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
spTSP::spTSP(void)
: Sprint(CN_SPTSP)
{
  _precondition_(_MAGIC_,CN_SPTSP,_FOOTER_);
  
  // set default Tsp format
  vFormat(F_TSP);
  
  _checkInitialised_(CN_SPTSP);
  
  _postcondition_(_MAGIC_,CN_SPTSP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
spTSP::~spTSP(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPTSP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint Tme and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* spTSP::sz(i8 iTsp)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  Word wTsp = iTsp;
  
  if (wTsp.xTsp.m_uMicro) {
    
    char caFraction[16];
    StrLib::vFormatFraction(caFraction,wTsp.xTsp.m_uMicro,6);
    
    StrLib::vSprint(m_caResult,
                    (Stz*)F_TSP_frac,

                    (i8)wTsp.xTsp.m_uYearDelta + C_BASE_YEAR,
                    (i8)wTsp.xTsp.m_uMonth,
                    (i8)wTsp.xTsp.m_uDay,
                    
                    (i8)wTsp.xTsp.m_uHour,
                    (i8)wTsp.xTsp.m_uMinute,
                    (i8)wTsp.xTsp.m_uSecond,
                    
                    caFraction);
  }
  else {
    StrLib::vSprint(m_caResult,
                    (Stz*)F_TSP,

                    (i8)wTsp.xTsp.m_uYearDelta + C_BASE_YEAR,
                    (i8)wTsp.xTsp.m_uMonth,
                    (i8)wTsp.xTsp.m_uDay,
                    
                    (i8)wTsp.xTsp.m_uHour,
                    (i8)wTsp.xTsp.m_uMinute,
                    (i8)wTsp.xTsp.m_uSecond);
  }

  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* spTSP::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SPTSP);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
