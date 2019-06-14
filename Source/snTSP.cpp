/*============================================================================*/
/* snTSP.cpp - Tsp scan helper                                                */
/*============================================================================*/

#include "Transform.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
snTSP::snTSP(void)
: Scan(CN_SNTSP)
{
  _precondition_(_MAGIC_,CN_SNTSP,_FOOTER_);
  
  _checkInitialised_(CN_SNTSP);
  
  _postcondition_(_MAGIC_,CN_SNTSP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
snTSP::~snTSP(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SNTSP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* scan                                                                       */
/*----------------------------------------------------------------------------*/
i8 snTSP::i(char** aacCrs, Column* pCol, Formatters* pScnFmt)
{
  _precondition_(_MAGIC_,CN_SNTSP);
  
  i8 iYear, iMonth, iDay;
  i8 iHour, iMinute, iSecond, iMicro;
  
  if ( ! StrLib::bExtractTimestamp2(aacCrs,
                                    iYear, iMonth, iDay,
                                    iHour, iMinute, iSecond, iMicro,
                                    pScnFmt->m_eDateComponent,
                                    pScnFmt->m_cDateSeparator) )
    throw C_UNKNOWN;
  
  Tsp xTsp;
  
  xTsp.x.m_uYearDelta   = iYear - C_BASE_YEAR;
  xTsp.x.m_uMonth  = iMonth;
  xTsp.x.m_uDay    = iDay;
  xTsp.x.m_uHour   = iHour;
  xTsp.x.m_uMinute = iMinute;
  xTsp.x.m_uSecond = iSecond;
  xTsp.x.m_uMicro  = iMicro;
  xTsp.x.m_uSpare  = 0;

  m_iValue = xTsp.iAsI8();
  
  return m_iValue;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* snTSP::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SNTSP);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
