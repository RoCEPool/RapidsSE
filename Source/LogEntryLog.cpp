/*============================================================================*/
/* LogEntryLog.cpp - Log Entry for Log                                        */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntryLog::vSet(LogEntry* pLogEntry, Log* pLog, eGA eLogAct)
{
  pLogEntry->m_eKind = EK_LOG;
  pLogEntry->m_cPrefix = 'G';

  m_iLogId  = pLog->m_iUniqueId;
  m_eLogAct = eLogAct;
  
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryLog::vOutline(s4K& sOutline)
{
  SP_Id spId;
  SP_String spString;

  sOutline.vCatSprint(".%s %24s"_Stz,
                
                      spId.sz(IK_LOG,m_iLogId),
                      spString.szRight(aGA[m_eLogAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryLog::vWriteCsv(File* pFile, LogEntry* pEntry)
{
  SP_Id spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_LOG,m_iLogId),
                aGA[m_eLogAct]);
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntryLog::vToTxt(s256& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,
               aCN[pEntry->m_eClass],
               aGA[m_eLogAct],
               pEntry->szIndicator(),
               pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
