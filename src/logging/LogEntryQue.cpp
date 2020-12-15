/*============================================================================*/
/* LogEntryQue.cpp - Log Entry for Queues                                     */
/*============================================================================*/

#include "Logging.h"

#include "eOC.h"

#include "File.hpp" //?

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntryQue::vSet(LogEntry* pLogEntry, Queues* pQueues, eQA eQueAct)
{
  pLogEntry->m_eKind = LG_QUE;
  pLogEntry->m_cPrefix = 'Q';

  m_iQueId  = pQueues->m_iFocalId;
  m_eQueAct = eQueAct;
  
       if (QA_QUE_FIND_ORIG  == eQueAct)     pLogEntry->m_iIndicator = OC_FIND;
  else if (QA_QUE_FOUND_ORIG  == eQueAct)    pLogEntry->m_iIndicator = OC_FOUND;
  else if (QA_QUE_NOT_ORIG  == eQueAct)      pLogEntry->m_iIndicator = OC_NOTFOUND;
  
  else if (QA_QUE_FIND_PARTG  == eQueAct)    pLogEntry->m_iIndicator = OC_FIND;
  else if (QA_QUE_FOUND_PARTG  == eQueAct)   pLogEntry->m_iIndicator = OC_FOUND;
  else if (QA_QUE_NOT_PARTG  == eQueAct)     pLogEntry->m_iIndicator = OC_NOTFOUND;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryQue::vOutline(s4K& sOutline)
{
  SP_Gid spId;
  SP_String spString;

  sOutline.vCatSprint(".%s %24s"_Stz,
                
                      spId.sz(IK_QUEUE,m_iQueId),
                      spString.szRight(aQA[m_eQueAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryQue::vWriteCsv(File* pFile, LogEntry* pEntry)
{
  SP_Gid spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_QUEUE,m_iQueId),
                aQA[m_eQueAct]);
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntryQue::vToTxt(s4K& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,
               aCN[pEntry->m_eClass],
               aQA[m_eQueAct],
               pEntry->szIndicator(),
               aQA_short[m_eQueAct],
               pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
