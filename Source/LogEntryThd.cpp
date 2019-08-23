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

  sOutline.vCatSprint(".%s,%24s"_Stz,
                
                      spId.sz("H"_Stz,m_iThdId),
                      spString.szRight(aHA[m_eThdAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryThd::vWriteCsv(File* pFile)
{
  SP_Id spId;
  
  pFile->vPrint(",%s,%s"_Stz,
                
                spId.sz("H"_Stz,m_iThdId),
                aHA[m_eThdAct]);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
