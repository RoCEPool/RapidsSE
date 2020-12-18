/*============================================================================*/
/* JobLog.cpp -- job log helper                                               */
/*============================================================================*/

#include "Plans.h"

#include "eQK.h"
#include "eTS.h"

extern Log* gpLog;

/*----------------------------------------------------------------------------*/
/* add a log entry for job                                                    */
/*----------------------------------------------------------------------------*/
void JobLog::vNote(Plans::Job* pJob, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if ( ! gpLog )
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  s4K szCombinedFormat;
  
  if (pJob->bWasReceived()) {
    
    s32 sNodeNm;
    sNodeNm = pJob->szNodeNm();
    
    StrLib::vSprint(szCombinedFormat,
                    "%s (recv from %s)"_Stz,
                    
                    szFormat,
                    sNodeNm.szStz()
                    );
  }
  else {
    StrLib::vSprint(szCombinedFormat,
                    "%s"_Stz,
                    
                    szFormat
                    );
  }
  
  i8 iJobId = pJob->iJobId();
  bool bForeign = pJob->bWasReceived();
//  pJob->m_i_Activity = gpLog->iEntryJob_arg(iJobId,bForeign,
//                                          szCombinedFormat,xVaArgs);
  
  va_end(xVaArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a log entry for job state at some point in the code                    */
/*----------------------------------------------------------------------------*/
void JobLog::vNoteState(Plans::Job* pJob, Stz* szPoint)
{
#ifdef WITH_SHARED_LOG
  if (! gpLog)
    return;
  
  if (! pJob)
    return;

  if (! Plans::_bIsValidJob(pJob->eClass()))
    return;
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  s4K szEntryText;
  
  szEntryText.vReset();
  
  szEntryText.vSprint("%s %s %p "
                      "%s %s"_Stz,
                      
                      szPoint,
                      aCN[pJob->eClass()],
                      pJob,
                      
                      aTS[AM::iTaskState(pJob->pTask())],
                      aQK[AM::iTaskQueue(pJob->pTask())]
                      );
  
  if (pJob->bWasReceived()) {
    
    s32 sNodeNm;
    sNodeNm = pJob->szNodeNm();
    
    szEntryText.vCatSprint(" (recv from %s)"_Stz,sNodeNm.szStz());
  }

  i8 iJobId = pJob->iJobId();
  bool bForeign = pJob->bWasReceived();
  //pJob->m_i_Activity = gpLog->iEntryJob(iJobId,bForeign,szEntryText);

#endif
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
