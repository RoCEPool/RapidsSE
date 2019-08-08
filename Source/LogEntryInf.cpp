/*============================================================================*/
/* LogEntryInf.cpp - Log Entry Information                                    */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntryInf::vSet(LogEntry* pLogEntry)
{  
  pLogEntry->m_eKind = EK_INF;

  m_eLogNote = LN_TEXT;
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryInf::vWriteCsv(File* pFile, LogEntry* pLogEntry,
                             i8 iTickDeltaUsec, i8& iTotalDelta)
{
  pFile->vPrint("%lld,%lld,%lld,%lld,"
                "%s,%s,%s,"
                "%s\n"_Stz,
                
                pLogEntry->m_i_Entry,
                pLogEntry->m_iTick,
                iTickDeltaUsec,
                iTotalDelta,
              
                pLogEntry->m_caNode,
                aEK[pLogEntry->m_eKind],
                aLN[m_eLogNote],
                
                pLogEntry->m_caText);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
