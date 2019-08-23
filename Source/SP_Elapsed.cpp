/*============================================================================*/
/* SP_Elapsed.cpp - Elapsed sprint helper                        (on c stack) */
/*============================================================================*/

#include "Gadgets.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Elapsed::SP_Elapsed(void)
: SP(CN_SPELAPSED)
{
  _precondition_(_MAGIC_,CN_SPELAPSED,_FOOTER_);
  
  _checkInitialised_(CN_SPELAPSED);
  
  _postcondition_(_MAGIC_,CN_SPELAPSED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Elapsed::~SP_Elapsed(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPELAPSED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint SP_Elapsed and return Stz* to result                                */
/*----------------------------------------------------------------------------*/
Stz* SP_Elapsed::szNsec(i8 iNsec)
{
  _precondition_(_MAGIC_,CN_SPELAPSED,_FOOTER_);
  
  i8 iSeconds = iNsec / C_1G;
  i8 iNanoseconds = iNsec - (iSeconds * C_1G);
  
  i8 iThousandths = (iNanoseconds       ) / C_1M;
  i8 iMillionths  = (iNanoseconds % C_1M) / C_1K;
  i8 iBillionths  = (iNanoseconds % C_1K) / 1;

  // days and seconds
  i8 iDays = iSeconds / (24 * 60 * 60);
  iSeconds -= iDays * (24 * 60 * 60);
  
  // days, hours and seconds
  i8 iHours = iSeconds / (60 * 60);
  iSeconds -= iHours * (60 * 60);
  
  // days, hours, minutes and seconds
  i8 iMinutes = iSeconds / (60);
  iSeconds -= iMinutes * 60;

  if (iDays)
    StrLib::vSprint(m_caResult,
                    "%02lld %02lld:%02lld:%02lld.%03lld_%03lld_%03lld"_Stz,
                    iDays,iHours,iMinutes,iSeconds,
                    iThousandths,iMillionths,iBillionths);
  else if (iHours)
    StrLib::vSprint(m_caResult,
                    "%02lld:%02lld:%02lld.%03lld_%03lld_%03lld"_Stz,
                    iHours,iMinutes,iSeconds,
                    iThousandths,iMillionths,iBillionths);
  else if (iMinutes)
    StrLib::vSprint(m_caResult,
                    "%02lld:%02lld.%03lld_%03lld_%03lld"_Stz,
                    iMinutes,iSeconds,
                    iThousandths,iMillionths,iBillionths);
  else
    StrLib::vSprint(m_caResult,
                    "%02lld.%03lld_%03lld_%03lld"_Stz,
                    iSeconds,
                    iThousandths,iMillionths,iBillionths);

  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_Elapsed)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
