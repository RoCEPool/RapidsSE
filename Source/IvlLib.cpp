/*============================================================================*/
/* IvlLib.cpp -- Interval Library                                             */
/*============================================================================*/

#include "System.h"

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get current system interval                                                */
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
