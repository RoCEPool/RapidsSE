/*============================================================================*/
/* LogEntryJob.cpp - Log Entry for Job                                        */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set variable entry                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryJob::vSet(LogEntry* pLogEntry, i8 iJobId, i8 iForeign, char* aText)
{  
  pLogEntry->m_eKind = EK_JOB;

  m_eLogNote = LN_TEXT;
  
  m_iJobId   = iJobId;
  m_iForeign = iForeign;
  
  StrLib::vCopyLen(m_caText,aText,sizeof(m_caText) - 1);
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryJob::vWrite(File* pFile, LogEntry* pLogEntry,
                           i8 iTickDeltaUsec, i8& iTotalDelta)
{
  pFile->vPrint("%lld,%lld,%lld,%lld,"
                "%s,%s,%s,"
                "%lld,%lld,"
                "%s\n"_Stz,
                
                pLogEntry->m_i_Entry,
                pLogEntry->m_iTick,
                iTickDeltaUsec,
                iTotalDelta,
              
                pLogEntry->m_caNode,
                aEK[pLogEntry->m_eKind],
                aLN[m_eLogNote],
                
                m_iJobId,
                m_iForeign,
                
                m_caText);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
