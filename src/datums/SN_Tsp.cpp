/*============================================================================*/
/* SN_Tsp.cpp - Tsp scan helper                                               */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SN_Tsp::SN_Tsp(void)
: Scan(CN_SNTSP)
{
  _precondition_(_MAGIC_,CN_SNTSP,_FOOTER_);
  
  _checkInitialised_(CN_SNTSP);
  
  _postcondition_(_MAGIC_,CN_SNTSP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SN_Tsp::~SN_Tsp(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SNTSP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* scan                                                                       */
/*----------------------------------------------------------------------------*/
i8 SN_Tsp::i(char** aacCrs, Format* pFmt)
{
  _precondition_(_MAGIC_,CN_SNTSP);
  
  i8 iYear, iMonth,  iDay;
  i8 iHour, iMinute, iSecond, iMicro;
  
  char* aBegin = *aacCrs;
  
  if ( ! StrLib::bExtractTimestamp2(aacCrs,
                                    iYear, iMonth, iDay,
                                    iHour, iMinute, iSecond, iMicro,
                                    pFmt->m_eDateComponent,
                                    pFmt->m_cDateSeparator) )
    THROW_USER("StrLib::bExtractTimestamp2"_Stz);

  m_wDtm.m_wTsp.m_uYearDelta = iYear - C_YEAR_BASE;
  m_wDtm.m_wTsp.m_uMonth     = iMonth;
  m_wDtm.m_wTsp.m_uDay       = iDay;
  m_wDtm.m_wTsp.m_uHour      = iHour;
  m_wDtm.m_wTsp.m_uMinute    = iMinute;
  m_wDtm.m_wTsp.m_uSecond    = iSecond;
  m_wDtm.m_wTsp.m_uMicro     = iMicro;
  m_wDtm.m_wTsp.m_uLocal     = C_TSP_LOCAL;
  m_wDtm.m_wTsp.m_uSpare     = 0;
  
  return m_wDtm;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SN_Tsp::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SNTSP);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
