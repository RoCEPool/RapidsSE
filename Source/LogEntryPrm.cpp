/*============================================================================*/
/* LogEntryPrm.cpp - Log Entry for Prompt                                     */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntryPrm::vSet(LogEntry* pLogEntry, i8 iPrmId)
{
  pLogEntry->m_eKind = EK_PRM;
  
  m_iPrmId = iPrmId;
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryPrm::vWriteCsv(File* pFile, LogEntry* pLogEntry,
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
                
                m_iPrmId,
                pLogEntry->m_caText);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
