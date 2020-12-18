/*============================================================================*/
/* LogEntryMgt.cpp - Log Entry for Management Actions                         */
/*============================================================================*/

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntryMgt::vSet(LogEntry* pLogEntry, Job* pJob, eMA eMgtAct)
{
  pLogEntry->m_eKind   = LG_MGT;
  pLogEntry->m_cPrefix = 'M';
  pLogEntry->m_iAct    = pJob->iStepNo();
  //pLogEntry->m_xReqTuple.vSet(&pJob->m_xReqTuple);

  m_iJobId  = pJob->m_iFocalId;
  m_eMgtAct = eMgtAct;
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
void _LogEntryMgt::vToTxt(s4K& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,
               aCN[pEntry->m_eClass],
               aMA[m_eMgtAct],
               pEntry->szIndicator(),
               aMA_short[m_eMgtAct],
               pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
