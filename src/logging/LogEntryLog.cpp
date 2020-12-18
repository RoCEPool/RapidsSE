/*============================================================================*/
/* LogEntryLog.cpp - Log Entry for Log                                        */
/*============================================================================*/

#include "StorageEngine.h"

#include "File.hpp" //?

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntryLog::vSet(LogEntry* pLogEntry, Log* pLog, eGA eLogAct)
{
  pLogEntry->m_eKind = LG_LOG;
  pLogEntry->m_cPrefix = 'G';

  m_iLogId  = pLog->m_iFocalId;
  m_eLogAct = eLogAct;
  
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryLog::vOutline(s4K& sOutline)
{
  SP_Gid spId;
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
  SP_Gid spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_LOG,m_iLogId),
                aGA[m_eLogAct]);
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntryLog::vToTxt(s4K& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,
               aCN[pEntry->m_eClass],
               aGA[m_eLogAct],
               pEntry->szIndicator(),
               aGA_short[m_eLogAct],
               pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
