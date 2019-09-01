/*============================================================================*/
/* LogEntrySys.cpp - Log Entry for System                                     */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntrySys::vSet(LogEntry* pLogEntry, AttachMap* pAttachMap, eYA eSysAct)
{
  pLogEntry->m_eKind = EK_SYS;
  pLogEntry->m_cPrefix = 'Y';

  m_iSysId  = pAttachMap->m_iUniqueId;
  m_eSysAct = eSysAct;
  
  if (JA_JOB_CONSTRUCT == eSysAct)
    pLogEntry->m_iIndicator = OC_CONSTRUCT;
  else
    if (JA_JOB_DESTRUCT == eSysAct)
      pLogEntry->m_iIndicator = OC_DESTRUCT;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntrySys::vOutline(s4K& sOutline)
{
  SP_Id spId;
  SP_String spString;

  sOutline.vCatSprint(".%s %24s"_Stz,
                
                      spId.sz(IK_SYSTEM,m_iSysId),
                      spString.szRight(aYA[m_eSysAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntrySys::vWriteCsv(File* pFile, LogEntry* pEntry)
{
  SP_Id spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_SYSTEM,m_iSysId),
                aYA[m_eSysAct]);
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntrySys::vToTxt(s256& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,aCN[pEntry->m_eClass],aYA[m_eSysAct],pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
