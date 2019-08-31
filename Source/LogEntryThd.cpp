/*============================================================================*/
/* LogEntryThd.cpp - Log Entry for Thread                                     */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set thread entry                                                           */
/*----------------------------------------------------------------------------*/
void _LogEntryThd::vSet(LogEntry* pLogEntry, Thread* pThread, eHA eThdAct)
{
  pLogEntry->m_eKind = EK_THD;
  pLogEntry->m_cPrefix = 'H';

  m_iThdId  = pThread->m_iUniqueId;
  m_eThdAct = eThdAct;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryThd::vOutline(s4K& sOutline)
{
  SP_Id spId;
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
  SP_Id spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_THREAD,m_iThdId),
                aHA[m_eThdAct]);
}

/*----------------------------------------------------------------------------*/
/* write to text file                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryThd::vWriteTxt(s256& sInf, File* pFile, LogEntry* pEntry)
{
  SP_Id spId;
  
  pFile->vPrint(C_ENTRY_FMT_1s,aHA[m_eThdAct]);
  
  pFile->vPrint("%s"_Stz,pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
