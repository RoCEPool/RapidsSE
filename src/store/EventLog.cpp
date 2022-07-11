

#include "EventLog.hpp"



/*----------------------------------------------------------------------------*/
/* report an event message                                                    */
/*----------------------------------------------------------------------------*/
void BASE::vEventLog(Stz* aFormat, ...)
{
  va_list xVarArgs;

  va_start(xVarArgs,aFormat);
  AM::vWriteEvent_vargs(aFormat,xVarArgs);
  va_end(xVarArgs);
}

