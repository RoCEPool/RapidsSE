/*============================================================================*/
/* Today -- time of day (seconds.microseconds)                                */
/*============================================================================*/

#include "Today.hpp"

/*----------------------------------------------------------------------------*/
/* get time of day (current realtime clock)                                   */
/*----------------------------------------------------------------------------*/
void Today::vGet(i8* pSeconds, i8* pMicroseconds, SysCall& SysCall)
{
  struct timeval xTimeVal;

  SysCall.vClear();
  
  i8 rCode = gettimeofday(&xTimeVal,NULL);
  
  if (rCode) {
    SysCall.vError(errno);
    vError("gettimeofday() error; errno=%d",errno);
    return;
  }
  
  SysCall.vSuccess();
  
  *pSeconds      = xTimeVal.tv_sec;
  *pMicroseconds = xTimeVal.tv_usec;
  
  ASSERT(*pMicroseconds < 1000000); // logic check
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/