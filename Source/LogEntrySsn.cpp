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
  pLogEntry->m_cPrefix = 'S';

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
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntrySsn::vToTxt(s256& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,aAA[m_eSsnAct],pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
