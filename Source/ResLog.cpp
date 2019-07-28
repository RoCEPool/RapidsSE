/*============================================================================*/
/* ResLog.cpp -- Resource log helper                                          */
/*============================================================================*/

#include "Correctness.h"
#include "StorageEngine.h"
#include "AM.hpp"

extern Log* gpLog;

/*----------------------------------------------------------------------------*/
/* add a log entry for resource                                               */
/*----------------------------------------------------------------------------*/
void ResLog::vNote(eSR eSysRes,Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (AM::pMarkResource()->bEnum(eSysRes) ||
      AM::pMarkResource()->bAll())
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  gpLog->iAddNoteArg(_EG_SR,eSysRes,false,szFormat,xVaArgs);
  
  va_end(xVaArgs);
#endif
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
