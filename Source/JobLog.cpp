/*============================================================================*/
/* JobLog.cpp -- job log helper                                               */
/*============================================================================*/

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;
extern Log* gpLog;

/*----------------------------------------------------------------------------*/
/* add a log entry for job                                                    */
/*----------------------------------------------------------------------------*/
void JobLog::vNote(Jobs::Job* pJob, Stz* szFormat, ...)
{
#ifdef WITH_ACTIVITY_MAP
  if ( ! gpLog )
    return;
  
  va_list xArgs;
  
  va_start(xArgs,szFormat);
  
  s4K szCombinedFormat;
  
  if (pJob->m_bWasReceived) {
    
    s32 sNodeNm;
    sNodeNm = pJob->szNodeNm();
    
    StrLib::vSprint(szCombinedFormat,
                    "%s (recv from %s)"_Stz,
                    
                    szFormat,
                    sNodeNm.aStz()
                    );
  }
  else {
    StrLib::vSprint(szCombinedFormat,
                    "%s"_Stz,
                    
                    szFormat
                    );
  }
  
  i8 iJobId = pJob->iJobId();
  bool bForeign = pJob->m_bWasReceived;
  pJob->m_i_Activity = gpLog->iAddNoteArg(C_JOB_GROUP,iJobId,bForeign,
                                          szCombinedFormat,xArgs);
  
  va_end(xArgs);
#endif
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
