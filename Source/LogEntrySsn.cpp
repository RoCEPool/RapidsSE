/*============================================================================*/
/* LogEntryItf.cpp - Log Entry for Api                                        */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntrySsn::vSet(LogEntry* pLogEntry, Session* pSession, eAA eSsnAct)
{
  pLogEntry->m_eKind = EK_SSN;
  
  m_iSsnId  = pSession->m_iUniqueId;
  m_eSsnAct = eSsnAct;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntrySsn::vOutline(s4K& sOutline)
{
  SP_Id spId;
  SP_String spString;

  sOutline.vCatSprint(".%s %24s"_Stz,
                
                      spId.sz(IK_SESSION,m_iSsnId),
                      spString.szRight(aAA[m_eSsnAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntrySsn::vWriteCsv(File* pFile, LogEntry* pEntry)
{
  SP_Id spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_SESSION,m_iSsnId),
                aAA[m_eSsnAct]);
}

/*----------------------------------------------------------------------------*/
/* write to text file                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntrySsn::vWriteTxt(File* pFile, LogEntry* pEntry)
{
  SP_Id spId;
  
  pFile->vPrint(C_ENTRY_FMT_2s,
                
                aAA[m_eSsnAct],
                spId.sz(IK_SESSION,m_iSsnId)
                );
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
