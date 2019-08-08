/*============================================================================*/
/* LogEntryTsk.cpp - Log Entry for Task                                       */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set resource entry                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryTsk::vSet(LogEntry* pLogEntry, i8 iTskId)
{
  pLogEntry->m_eKind = EK_THD;
  
  m_iTskId = iTskId;
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryTsk::vWriteCsv(File* pFile, LogEntry* pLogEntry,
                             i8 iTickDeltaUsec, i8& iTotalDelta)
{
  pFile->vPrint("%lld,%lld,%lld,%lld,"
                "%s,%s,"
                "%lld,%s\n"_Stz,

                pLogEntry->m_i_Entry,
                pLogEntry->m_iTick,
                iTickDeltaUsec,
                iTotalDelta,
                
                pLogEntry->m_caNode,
                aEK[pLogEntry->m_eKind],
                
                m_iTskId,
                pLogEntry->m_caText);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
