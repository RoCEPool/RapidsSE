/*============================================================================*/
/* LogEntryLck.cpp - Log Entry for Lock                                       */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntryLck::vSet(LogEntry* pLogEntry, Lock* pLock, eLA eLckAct)
{
  pLogEntry->m_eKind = EK_LCK;
  
  m_iLckId  = pLock->m_iUniqueId;
  m_eLckAct = eLckAct;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryLck::vOutline(s4K& sOutline)
{
  SP_Id spId;
  SP_String spString;

  sOutline.vCatSprint(".%s %s"_Stz,
                
                      spId.sz(IK_LOCK,m_iLckId),
                      spString.szRight(aLA[m_eLckAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryLck::vWriteCsv(File* pFile)
{
  SP_Id spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_LOCK,m_iLckId),
                aLA[m_eLckAct]);
}

/*----------------------------------------------------------------------------*/
/* write to text file                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryLck::vWriteTxt(File* pFile)
{
  SP_Id spId;
  
  pFile->vPrint(",%s,%s"_Stz,
                
                spId.sz(IK_LOCK,m_iLckId),
                aLA[m_eLckAct]);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
