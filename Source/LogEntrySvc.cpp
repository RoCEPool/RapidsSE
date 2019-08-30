/*============================================================================*/
/* LogEntrySvc.cpp - Log Entry for Service                                    */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set resource entry                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntrySvc::vSet(LogEntry* pLogEntry, Job* pJob, eVA eSvcAct)
{
  pLogEntry->m_eKind = EK_SVC;
  
  m_iSvcId  = pJob->m_iUniqueId;
  m_eSvcAct = eSvcAct;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntrySvc::vOutline(s4K& sOutline)
{
  SP_Id spId;
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
  SP_Id spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_SERVICE,m_iSvcId),
                aVA[m_eSvcAct]);
}

/*----------------------------------------------------------------------------*/
/* write to text file                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntrySvc::vWriteTxt(File* pFile, LogEntry* pEntry)
{
  SP_Id spId;
  
  pFile->vPrint(C_ENTRY_FMT_2s,
                
                aVA[m_eSvcAct],
                spId.sz(IK_SERVICE,m_iSvcId)
                );
  
  pFile->vPrint("%s"_Stz,pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
