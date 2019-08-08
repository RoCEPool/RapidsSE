/*============================================================================*/
/* LogEntryTxn.cpp - Log Entry for Transaction                                */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set resource entry                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryTxn::vSet(LogEntry* pLogEntry, i8 iTxnId)
{
  pLogEntry->m_eKind = EK_TXN;
  
  m_iTxnId = iTxnId;
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryTxn::vWriteCsv(File* pFile, LogEntry* pLogEntry,
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
                
                m_iTxnId,
                pLogEntry->m_caText);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
