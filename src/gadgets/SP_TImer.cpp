/*============================================================================*/
/* SP_Timer.cpp - Timer sprint helper                            (on c stack) */
/*============================================================================*/

#include "Resources.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Timer::SP_Timer(void)
: SP(CN_SPID)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  _checkInitialised_(CN_SPID);
  
  _postcondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Timer::~SP_Timer(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint SP_Timer and return Stz* to result                                  */
/*----------------------------------------------------------------------------*/
Stz* SP_Timer::sz(i8 iNanoseconds)
{
  /*
  
   This function determines the magnitude of the elapsed time of the timer
   and formats a string where the elapsed time is presented in the form x.yyyu
   
   If the elapsed time can be measured in 1 or more seconds then its
   formatted as seconds(x).milliseconds(y), with units(u) being "s"
   
   If the elapsed time can be measured in 1 or more milliseconds then its
   formatted as milliseconds(x).microseconds(y), with units(u) being "ms"
   
   If the elapsed time can be measured in 1 or more nanoseconds then its
   formatted as microseconds(x).nanoseconds(y), with units(u) being "us"
   
   Otherwise the elapsed time is zero then its
   formatted as 0.000s
   
   */
  
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  // get elapsed time in nanoseconds
  i8 nElpased_nsec = iNanoseconds;
  
  // convert elapsed nanoseconds to seconds and nanoseconds
  i8 nSeconds      = nElpased_nsec / C_1G;
  i8 nNanoseconds  = nElpased_nsec - (nSeconds * C_1G);
  
  // calculate the fractions
  i8 iThousandths  = (nNanoseconds       ) / C_1M;
  i8 iMillionths   = (nNanoseconds % C_1M) / C_1K;
  i8 iBillionths   = (nNanoseconds % C_1K) / 1;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // format according to magnitude
  
  if (nSeconds)
    StrLib::vSprint(m_caResult,
                    "%lld.%03llds"_Stz, // units are seconds
                    nSeconds,iThousandths);
  
  else if (iThousandths)
    StrLib::vSprint(m_caResult,
                    "%lld.%03lldms"_Stz, // units are milliseconds
                    iThousandths,iMillionths);
  
  else if (iMillionths || iBillionths)
    StrLib::vSprint(m_caResult,
                    "%lld.%03lldus"_Stz, // units are microseconds
                    iMillionths,iBillionths);
  
  else
    StrLib::vSprint(m_caResult,
                    "0.000s"_Stz);

  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint SP_Timer and return Stz* to result                                  */
/*----------------------------------------------------------------------------*/
Stz* SP_Timer::sz(Timer* pTimer)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  return sz(pTimer->iNanoseconds());
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_Timer)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
