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

  sOutline.vCatSprint(".%s,%24s"_Stz,
                
                      spId.sz("G"_Stz,m_iLogId),
                      spString.szRight(aGA[m_eLogAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryLog::vWriteCsv(File* pFile)
{
  SP_Id spId;
  
  pFile->vPrint(",%s,%s"_Stz,
                
                spId.sz("G"_Stz,m_iLogId),
                aGA[m_eLogAct]);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
