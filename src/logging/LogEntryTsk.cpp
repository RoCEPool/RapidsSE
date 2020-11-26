/*============================================================================*/
/* LogEntryTsk.cpp - Log Entry for Task                                       */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set resource entry                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryTsk::vSet(LogEntry* pLogEntry, Task* pTask, eTA eTskAct)
{
  pLogEntry->m_eKind   = LG_TSK;
  pLogEntry->m_cPrefix = 'F';
  pLogEntry->m_iAct    = pTask->iStepNo();
  //pLogEntry->m_xReqTuple.vSet(pTask->aReqTuple());

  m_iTskId  = pTask->m_iFocalId;
  m_eTskAct = eTskAct;
  
       if (TA_TSK_CONSTRUCT == eTskAct)     pLogEntry->m_iIndicator = OC_CONSTRUCT;
  else if (TA_TSK_DESTRUCT == eTskAct)      pLogEntry->m_iIndicator = OC_DESTRUCT;
  else if (TA_TSK_ASSIGN_THREAD == eTskAct)   pLogEntry->m_iIndicator = OC_PAIR;
  else if (TA_TSK_ASSIGNED_THREAD == eTskAct) pLogEntry->m_iIndicator = OC_PAIR;
  else if (TA_TSK_THREAD == eTskAct)        pLogEntry->m_iIndicator = OC_PAIR;
  else if (TA_TSK_EXECUTE == eTskAct)       pLogEntry->m_iIndicator = OC_RUN;
  else if (TA_TSK_RESUME == eTskAct)        pLogEntry->m_iIndicator = OC_RESUME;
  else if (TA_TSK_ABLE == eTskAct)          pLogEntry->m_iIndicator = OC_TRANSITION;
  else if (TA_TSK_YIELD == eTskAct)         pLogEntry->m_iIndicator = OC_TRANSITION;
  else if (TA_TSK_DESIST == eTskAct)        pLogEntry->m_iIndicator = OC_TRANSITION;
  else if (TA_TSK_SLEEP == eTskAct)         pLogEntry->m_iIndicator = OC_TRANSITION;
  else if (TA_TSK_EXIT == eTskAct)          pLogEntry->m_iIndicator = OC_EXIT;
  else if (TA_TSK_SCHEDULE == eTskAct)      pLogEntry->m_iIndicator = OC_SCHEDULE;
  else if (TA_TSK_REQUIRES == eTskAct)      pLogEntry->m_iIndicator = OC_REQUIRES;
  else if (TA_TSK_GRAB == eTskAct)          pLogEntry->m_iIndicator = OC_SECURE;
  else if (TA_TSK_GRABBED == eTskAct)       pLogEntry->m_iIndicator = OC_SECURE;
  else if (TA_TSK_DROP == eTskAct)          pLogEntry->m_iIndicator = OC_UNSECURE;
  else if (TA_TSK_ACK == eTskAct)           pLogEntry->m_iIndicator = OC_ACK;
  else if (TA_TSK_WAIT == eTskAct)          pLogEntry->m_iIndicator = OC_MOVE;
  else if (TA_TSK_TRIGGER == eTskAct)       pLogEntry->m_iIndicator = OC_TRIGGER;
  else if (TA_TSK_BIDE == eTskAct)          pLogEntry->m_iIndicator = OC_BIDE;
  else if (TA_TSK_SPAWN_DEDICATED == eTskAct) pLogEntry->m_iIndicator = OC_SPAWN;

  else if (TA_DDL_READ_GRAB == eTskAct)     pLogEntry->m_iIndicator = OC_CLARITY;
  else if (TA_DDL_READ_GRABBED == eTskAct)  pLogEntry->m_iIndicator = OC_CLARITY;
  else if (TA_DDL_WRITE_GRAB == eTskAct)    pLogEntry->m_iIndicator = OC_SOLIDITY;
  else if (TA_DDL_WRITE_GRABBED == eTskAct) pLogEntry->m_iIndicator = OC_SOLIDITY;
  else if (TA_DDL_READ_DROPPED == eTskAct)  pLogEntry->m_iIndicator = OC_CONFUSION;
  else if (TA_DDL_WRITE_DROPPED == eTskAct) pLogEntry->m_iIndicator = OC_TRIGGER;

  else
    if (TA_TSK_WAKEUP == eTskAct)
      pLogEntry->m_iIndicator = OC_SERVICE;
  
  SP_Task spTask;
  StrLib::vSprintCat((Stz*)pLogEntry->m_caNote," %s"_Stz,spTask.sz_log(pTask));
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryTsk::vOutline(s4K& sOutline)
{
  SP_Gid spId;
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
  SP_Gid spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_TASK,m_iTskId),
                aTA[m_eTskAct]);
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntryTsk::vToTxt(s4K& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,
               aCN[pEntry->m_eClass],
               aTA[m_eTskAct],
               pEntry->szIndicator(),
               aTA_short[m_eTskAct],
               pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
