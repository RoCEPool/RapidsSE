/*============================================================================*/
/* LogEntryJob.cpp - Log Entry for Job                                        */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set variable entry                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryJob::vSet(LogEntry* pLogEntry, Job* pJob, eJA eJobAct)
{  
  pLogEntry->m_eKind = EK_JOB;
  
  m_iJobId  = pJob->m_iUniqueId;
  m_eJobAct = eJobAct;
  
  m_iReqId  = pJob->m_xReqTuple.m_iReqId;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryJob::vOutline(s4K& sOutline)
{
  SP_Id spId;
  SP_Id spReqId;
  SP_String spString;

  sOutline.vCatSprint(".%s %24s %s"_Stz,
                
                      spId.sz("J"_Stz,m_iJobId),
                      spString.szRight(aJA[m_eJobAct],24),
                      spReqId.sz("M"_Stz,m_iReqId)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryJob::vWriteCsv(File* pFile)
{
  SP_Id spId;
  SP_Id spReqId;

  pFile->vPrint(",%s,%s,%s"_Stz,
                
                spId.sz("J"_Stz,m_iJobId),
                aJA[m_eJobAct],
                spReqId.sz("M"_Stz,m_iReqId)
                );
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
