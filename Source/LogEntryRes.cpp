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
/* write to text file                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryRes::vWriteTxt(s256& sInf, File* pFile, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_N,aRA[m_eResAct],pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
