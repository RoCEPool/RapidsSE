/*============================================================================*/
/* LogEntryThd.cpp - Log Entry for Thread                                     */
/*============================================================================*/

#include "Logging.h"

#include "eOC.h"

#include "File.hpp" //?

/*----------------------------------------------------------------------------*/
/* set thread entry                                                           */
/*----------------------------------------------------------------------------*/
void _LogEntryThd::vSet(LogEntry* pLogEntry, Thread* pThread, eHA eThdAct)
{
  pLogEntry->m_eKind = LG_THD;
  pLogEntry->m_cPrefix = 'H';

  if (pThread->m_pTask && pThread->pTask()->pJob()) {
    //pLogEntry->m_xReqTuple.vSet(&pThread->pTask()->pJob()->m_xReqTuple);
  }
  else {
    SP_Gid spId;
    
    pLogEntry->m_xReqTuple.m_sPurpose.vSprint("~~THREAD-%s~~"_Stz,
                                              spId.sz(pThread->m_iFocalId));
  };
  m_iThdId  = pThread->m_iFocalId;
  m_eThdAct = eThdAct;
  
       if (HA_THD_CONSTRUCT   == eThdAct)   pLogEntry->m_iIndicator = OC_CONSTRUCT;
  else if (HA_THD_DESTRUCT    == eThdAct)   pLogEntry->m_iIndicator = OC_DESTRUCT;
  else if (HA_THD_ADD_DEDICATED == eThdAct) pLogEntry->m_iIndicator = OC_ADD;
  else if (HA_THD_ADD_GENERAL == eThdAct)   pLogEntry->m_iIndicator = OC_ADD;
  else if (HA_THD_APPEND_IDLE == eThdAct)   pLogEntry->m_iIndicator = OC_IDLE;
  else if (HA_THD_UNASSIGN_TASK == eThdAct) pLogEntry->m_iIndicator = OC_UNASSIGN;
  else if (HA_THD_GET_TASK == eThdAct)      pLogEntry->m_iIndicator = OC_GET;
  else if (HA_THD_POP_READY == eThdAct)     pLogEntry->m_iIndicator = OC_POP;
  else if (HA_THD_END_LOOP == eThdAct)      pLogEntry->m_iIndicator = OC_END;
  else if (HA_THD_BEGIN_LOOP == eThdAct)    pLogEntry->m_iIndicator = OC_BEGIN;
  else if (HA_THD_EXECUTE_DEDICATED == eThdAct) pLogEntry->m_iIndicator = OC_RUN;
  else if (HA_THD_REMOVE_DEDICATED == eThdAct)  pLogEntry->m_iIndicator = OC_REMOVE;
  
  SP_Thread spThread;
  StrLib::vSprintCat((Stz*)pLogEntry->m_caNote," %s"_Stz,spThread.sz_log(pThread));
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryThd::vOutline(s4K& sOutline)
{
  SP_Gid spId;
  SP_String spString;

  sOutline.vCatSprint(".%s %24s"_Stz,
                
                      spId.sz(IK_THREAD,m_iThdId),
                      spString.szRight(aHA[m_eThdAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryThd::vWriteCsv(File* pFile, LogEntry* pEntry)
{
  SP_Gid spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_THREAD,m_iThdId),
                aHA[m_eThdAct]);
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntryThd::vToTxt(s4K& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,
               aCN[pEntry->m_eClass],
               aHA[m_eThdAct],
               pEntry->szIndicator(),
               aHA_short[m_eThdAct],
               pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
