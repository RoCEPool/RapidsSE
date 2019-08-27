/*============================================================================*/
/* LogEntryTsk.cpp - Log Entry for Task                                       */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set resource entry                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryTsk::vSet(LogEntry* pLogEntry, Task* pTask, eTA eTskAct)
{
  pLogEntry->m_eKind = EK_TSK;
  
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
void _LogEntryTsk::vWriteCsv(File* pFile)
{
  SP_Id spId;
  
  pFile->vPrint(",%s,%s"_Stz,
                
                spId.sz(IK_TASK,m_iTskId),
                aTA[m_eTskAct]);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
