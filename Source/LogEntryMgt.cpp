/*============================================================================*/
/* LogEntryMgt.cpp - Log Entry for Management Actions                         */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntryMgt::vSet(LogEntry* pLogEntry, Job* pJob, eDA eDdlAct)
{
  pLogEntry->m_eKind   = EK_LCK;
  pLogEntry->m_cPrefix = 'D';
  pLogEntry->m_iAct    = pJob->iActNo();
  pLogEntry->m_xReqTuple.vSet(&pJob->m_xReqTuple);

  m_iJobId  = pJob->m_iUniqueId;
  m_eDdlAct = eDdlAct;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryMgt::vOutline(s4K& sOutline)
{
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryMgt::vWriteCsv(File* pFile, LogEntry* pEntry)
{
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntryMgt::vToTxt(s256& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,
               aCN[pEntry->m_eClass],
               aDA[m_eDdlAct],
               pEntry->szIndicator(),
               aDA_short[m_eDdlAct],
               pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
