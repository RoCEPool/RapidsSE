/*============================================================================*/
/* LogEntryLck.cpp - Log Entry for Lock                                       */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntryLck::vSet(LogEntry* pLogEntry, Lock* pLock, eLA eLckAct)
{
  pLogEntry->m_eKind   = EK_LCK;
  pLogEntry->m_cPrefix = 'L';
  pLogEntry->m_iAct    = pLock->iActNo();
  pLogEntry->m_xReqTuple.vSet(pLock->aReqTuple());

  m_iLckId  = pLock->m_iUniqueId;
  m_eLckAct = eLckAct;
  
  if (LA_LCK_CONSTRUCT == eLckAct)
    pLogEntry->m_iIndicator = OC_CONSTRUCT;
  else
    if (LA_LCK_DESTRUCT == eLckAct)
      pLogEntry->m_iIndicator = OC_DESTRUCT;
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
void _LogEntryLck::vWriteCsv(File* pFile, LogEntry* pEntry)
{
  SP_Id spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_LOCK,m_iLckId),
                aLA[m_eLckAct]);
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntryLck::vToTxt(s256& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,
               aCN[pEntry->m_eClass],
               aLA[m_eLckAct],
               pEntry->szIndicator(),
               pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
