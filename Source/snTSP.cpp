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
  
  i8 iYear, iMonth,  iDay;
  i8 iHour, iMinute, iSecond, iMicro;
  
  if ( ! StrLib::bExtractTimestamp2(aacCrs,
                                    iYear, iMonth, iDay,
                                    iHour, iMinute, iSecond, iMicro,
                                    pScnFmt->m_eDateComponent,
                                    pScnFmt->m_cDateSeparator) )
    throw C_UNKNOWN;
  
  m_xTsp.xTsp.m_uYearDelta = iYear - C_YEAR_BASE;
  m_xTsp.xTsp.m_uMonth     = iMonth;
  m_xTsp.xTsp.m_uDay       = iDay;
  m_xTsp.xTsp.m_uHour      = iHour;
  m_xTsp.xTsp.m_uMinute    = iMinute;
  m_xTsp.xTsp.m_uSecond    = iSecond;
  m_xTsp.xTsp.m_uMicro     = iMicro;
  m_xTsp.xTsp.m_uSpare     = 0;
  
  return m_xTsp.m_iValue;
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
