/*============================================================================*/
/* LogEntryLck.cpp - Log Entry for Lock                                       */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntryLck::vSet(LogEntry* pLogEntry, Lock* pLock, eLA eLckAct)
{
  pLogEntry->m_eKind   = LE_LCK;
  pLogEntry->m_cPrefix = 'L';
  pLogEntry->m_iAct    = pLock->iStepNo();
  //pLogEntry->m_xReqTuple.vSet(pLock->aReqTuple());

  m_iLckId  = pLock->m_iFocalId;
  m_eLckAct = eLckAct;
  
       if (LA_LCK_CONSTRUCT == eLckAct)  pLogEntry->m_iIndicator = OC_CONSTRUCT;
  else if (LA_LCK_DESTRUCT == eLckAct)   pLogEntry->m_iIndicator = OC_DESTRUCT;
  
  SP_Lock spLock;
  StrLib::vSprintCat((Stz*)pLogEntry->m_caNote," %s"_Stz,spLock.sz_log(pLock));
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryLck::vOutline(s4K& sOutline)
{
  SP_Gid spId;
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
  SP_Gid spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_LOCK,m_iLckId),
                aLA[m_eLckAct]);
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntryLck::vToTxt(s4K& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,
               aCN[pEntry->m_eClass],
               aLA[m_eLckAct],
               pEntry->szIndicator(),
               aLA_short[m_eLckAct],
               pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
