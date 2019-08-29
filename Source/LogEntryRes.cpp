/*============================================================================*/
/* LogEntryRes.cpp - Log Entry for Resource                                   */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set resource entry                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryRes::vSet(LogEntry* pLogEntry, Resource* pResource, eRA eResAct)
{
  pLogEntry->m_eKind = EK_RES;
  
  m_eClass  = pResource->eClass();
  pLogEntry->m_xReqTuple.vSet(pResource->aReqTuple());
  pLogEntry->m_iAct = pResource->iActNo();

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
void _LogEntryRes::vWriteTxt(File* pFile, LogEntry* pEntry)
{
  SP_Id spId;
  
  pFile->vPrint("%-20s %s:%03lld "_Stz, //C_ENTRY_FMT_2s,
                
                aRA[m_eResAct],
                spId.sz(IK_RESOURCE,m_iResId),
                
                pEntry->m_iAct,
                
                C_ENDARGS
                );
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
