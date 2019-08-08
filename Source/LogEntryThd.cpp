/*============================================================================*/
/* LogEntryThd.cpp - Log Entry for Operator                                    */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set resource entry                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryThd::vSet(LogEntry* pLogEntry, i8 iThdId)
{
  pLogEntry->m_eKind = EK_THD;
  
  m_iThdId = iThdId;
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryThd::vWriteCsv(File* pFile, LogEntry* pLogEntry,
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
                
                m_iThdId,
                pLogEntry->m_caText);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
