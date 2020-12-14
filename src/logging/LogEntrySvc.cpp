/*============================================================================*/
/* LogEntrySvc.cpp - Log Entry for Service                                    */
/*============================================================================*/

#include "Logging.h"

#include "File.hpp" //?

/*----------------------------------------------------------------------------*/
/* set resource entry                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntrySvc::vSet(LogEntry* pLogEntry, Job* pJob, eVA eSvcAct)
{
  pLogEntry->m_eKind = LG_SVC;
  pLogEntry->m_cPrefix = 'V';

  m_iSvcId  = pJob->m_iFocalId;
  m_eSvcAct = eSvcAct;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntrySvc::vOutline(s4K& sOutline)
{
  SP_Gid spId;
  SP_String spString;

  sOutline.vCatSprint(".%s %24s"_Stz,
                
                      spId.sz(IK_SERVICE,m_iSvcId),
                      spString.szRight(aVA[m_eSvcAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntrySvc::vWriteCsv(File* pFile, LogEntry* pEntry)
{
  SP_Gid spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_SERVICE,m_iSvcId),
                aVA[m_eSvcAct]);
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntrySvc::vToTxt(s4K& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,
               aCN[pEntry->m_eClass],
               aVA[m_eSvcAct],
               pEntry->szIndicator(),
               aVA_short[m_eSvcAct],
               pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
