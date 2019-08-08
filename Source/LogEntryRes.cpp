/*============================================================================*/
/* LogEntryRes.cpp - Log Entry for Resource                                   */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set resource entry                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryRes::vSet(LogEntry* pLogEntry, i8 iResId, eRK eResKnd, eRA eResAct)
{
  pLogEntry->m_eKind = EK_RES;
  
  m_iResId  = iResId;
  m_eResKnd = eResKnd;
  m_eResAct = eResAct;
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryRes::vWriteCsv(File* pFile, LogEntry* pLogEntry,
                             i8 iTickDeltaUsec, i8& iTotalDelta)
{
  pFile->vPrint("%lld,%lld,%lld,%lld,"
                "%lld,%s,"
                "%s,%lld,%s,%s,"
                "%s\n"_Stz,

                pLogEntry->m_i_Entry,
                pLogEntry->m_iTick,
                iTickDeltaUsec,
                iTotalDelta,
                
                pLogEntry->m_iNodeNo,
                pLogEntry->m_caNode,
                
                aEK[pLogEntry->m_eKind],
                m_iResId,
                aRK_Short[m_eResKnd],
                aRA[m_eResAct],

                pLogEntry->m_caText);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
