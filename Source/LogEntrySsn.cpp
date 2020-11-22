/*============================================================================*/
/* LogEntryItf.cpp - Log Entry for Api                                        */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntrySsn::vSet(LogEntry* pLogEntry, Session* pSession, eAA eSsnAct)
{
  pLogEntry->m_eKind = LG_SSN;
  pLogEntry->m_cPrefix = 'S';

  m_iSsnId  = pSession->m_iFocalId;
  m_eSsnAct = eSsnAct;
  
       if (AA_SSN_CONSTRUCT == eSsnAct) pLogEntry->m_iIndicator = OC_CONSTRUCT;
  else if (AA_SSN_DESTRUCT == eSsnAct)  pLogEntry->m_iIndicator = OC_DESTRUCT;
  else if (AA_API_SPAWN_JOB == eSsnAct) pLogEntry->m_iIndicator = OC_SPAWN;
  
  SP_Session spSession;
  StrLib::vSprintCat((Stz*)pLogEntry->m_caNote," %s"_Stz,spSession.sz_log(pSession));
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntrySsn::vOutline(s4K& sOutline)
{
  SP_Gid spId;
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
  SP_Gid spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_SESSION,m_iSsnId),
                aAA[m_eSsnAct]);
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntrySsn::vToTxt(s4K& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,
               aCN[pEntry->m_eClass],
               aAA[m_eSsnAct],
               pEntry->szIndicator(),
               aAA_short[m_eSsnAct],
               pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
