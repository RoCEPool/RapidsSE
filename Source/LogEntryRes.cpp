/*============================================================================*/
/* LogEntryRes.cpp - Log Entry for Resource                                   */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set resource entry                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryRes::vSet(LogEntry* pLogEntry, Resource* pResource, eRA eResAct)
{
  pLogEntry->m_eKind   = EK_RES;
  pLogEntry->m_cPrefix = 'R';
  pLogEntry->m_iAct    = pResource->iActNo();
  pLogEntry->m_xReqTuple.vSet(pResource->aReqTuple());

  m_eClass  = pResource->eClass();

  m_iResId  = pResource->m_iUniqueId;
  m_eResAct = eResAct;
  
       if (RA_MTX_CONSTRUCT  == eResAct) pLogEntry->m_iIndicator = OC_CONSTRUCT;
  else if (RA_MTX_DESTRUCT   == eResAct) pLogEntry->m_iIndicator = OC_DESTRUCT;
  else if (RA_MTX_LOCK       == eResAct) pLogEntry->m_iIndicator = OC_BLOCKED;
  else if (RA_MTX_LOCKED     == eResAct) pLogEntry->m_iIndicator = OC_BLOCKED;
  else if (RA_MTX_SIGNAL     == eResAct) pLogEntry->m_iIndicator = OC_SIGNAL;
  else if (RA_MTX_SIGNALED   == eResAct) pLogEntry->m_iIndicator = OC_SIGNALED;
  else if (RA_MTX_UNLOCKED   == eResAct) pLogEntry->m_iIndicator = OC_UNBLOCKED;
  else if (RA_MTX_WAIT_INDEF == eResAct) pLogEntry->m_iIndicator = OC_INDEFINITE;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryRes::vOutline(s4K& sOutline)
{
  SP_Id spId;
  SP_String spString;

  sOutline.vCatSprint(".%s %24s"_Stz,
                
                      spId.sz(IK_RESOURCE,m_iResId),
                      spString.szRight(aRA[m_eResAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryRes::vWriteCsv(File* pFile, LogEntry* pEntry)
{
  SP_Id spId;
  
  pFile->vPrint("%s,%s"_Stz,
              
                 spId.sz(IK_RESOURCE,m_iResId),
                aRA[m_eResAct]);
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntryRes::vToTxt(s256& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,
               aCN[pEntry->m_eClass],
               aRA[m_eResAct],
               pEntry->szIndicator(),
               aRA_short[m_eResAct],
               pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
