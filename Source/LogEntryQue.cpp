/*============================================================================*/
/* LogEntryQue.cpp - Log Entry for Queues                                     */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntryQue::vSet(LogEntry* pLogEntry, Queues* pQueues, eQA eQueAct)
{
  pLogEntry->m_eKind = EK_QUE;
  
  m_iQueId  = pQueues->m_iUniqueId;
  m_eQueAct = eQueAct;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryQue::vOutline(s4K& sOutline)
{
  SP_Id spId;
  SP_String spString;

  sOutline.vCatSprint(".%s %24s"_Stz,
                
                      spId.sz("Q"_Stz,m_iQueId),
                      spString.szRight(aQA[m_eQueAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryQue::vWriteCsv(File* pFile)
{
  SP_Id spId;
  
  pFile->vPrint(",%s,%s"_Stz,
                
                spId.sz("Q"_Stz,m_iQueId),
                aQA[m_eQueAct]);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
