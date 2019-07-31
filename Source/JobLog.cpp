/*============================================================================*/
/* JobLog.cpp -- job log helper                                               */
/*============================================================================*/

#include "Jobs.h"
#include "AM.hpp"
#include "eTS.h"

extern Log* gpLog;

/*----------------------------------------------------------------------------*/
/* add a log entry for job                                                    */
/*----------------------------------------------------------------------------*/
void JobLog::vNote(Jobs::Job* pJob, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if ( ! gpLog )
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
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
                                          szCombinedFormat,xVaArgs);
  
  va_end(xVaArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a log entry for job state at some point in the code                    */
/*----------------------------------------------------------------------------*/
void JobLog::vNoteState(Jobs::Job* pJob, Stz* szPoint)
{
#ifdef WITH_SHARED_LOG
  if (! gpLog)
    return;
  
  if (! pJob)
    return;

  if (! Jobs::_bIsValidJob(pJob->eClass()))
    return;
  
  if (! pJob->m_bMarkAction)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  s4K szEntryText;
  
  szEntryText.vReset();
  
  szEntryText.vSprint("%s %s %p "
                      "%s %s"_Stz,
                      
                      szPoint,
                      aCN[pJob->eClass()],
                      pJob,
                      
                      aTS[AM::iTaskState(pJob->m_pExecutingTask)],
                      aQK[AM::iTaskQueue(pJob->m_pExecutingTask)]
                      );
  
  if (pJob->m_bWasReceived) {
    
    s32 sNodeNm;
    sNodeNm = pJob->szNodeNm();
    
    szEntryText.vCatSprint(" (recv from %s)"_Stz,sNodeNm.aStz());
  }

  i8 iJobId = pJob->iJobId();
  bool bForeign = pJob->m_bWasReceived;
  pJob->m_i_Activity = gpLog->iAddNoteVbl(C_JOB_GROUP,iJobId,bForeign,
                                          szEntryText);

#endif
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
