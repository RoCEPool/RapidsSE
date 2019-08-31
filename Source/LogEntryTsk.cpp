/*============================================================================*/
/* LogEntryTsk.cpp - Log Entry for Task                                       */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set resource entry                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryTsk::vSet(LogEntry* pLogEntry, Task* pTask, eTA eTskAct)
{
  pLogEntry->m_eKind   = EK_TSK;
  pLogEntry->m_cPrefix = 'F';
  pLogEntry->m_iAct    = pTask->iActNo();
  pLogEntry->m_xReqTuple.vSet(pTask->aReqTuple());

  m_iTskId  = pTask->m_iUniqueId;
  m_eTskAct = eTskAct;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryTsk::vOutline(s4K& sOutline)
{
  SP_Id spId;
  SP_String spString;

  sOutline.vCatSprint(".%s %24s"_Stz,
                
                      spId.sz(IK_TASK,m_iTskId),
                      spString.szRight(aTA[m_eTskAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryTsk::vWriteCsv(File* pFile, LogEntry* pEntry)
{
  SP_Id spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_TASK,m_iTskId),
                aTA[m_eTskAct]);
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntryTsk::vToTxt(s256& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,aCN[pEntry->m_eClass],aTA[m_eTskAct],pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
