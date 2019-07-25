/*============================================================================*/
/* ResLog.cpp -- Resource log helper                                          */
/*============================================================================*/

#include "System.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;
extern Log* gpLog;

/*----------------------------------------------------------------------------*/
/* add a log entry for resource                                               */
/*----------------------------------------------------------------------------*/
void ResLog::vNote(eSR eSysRes,Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if ( ! gpLog )
    return;
  
  if ( ! gpAttachMap->m_bMarkResource[eSysRes] &&
       ! gpAttachMap->m_bMarkResourceAll )
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
