/*============================================================================*/
/* LogEntryRes.cpp - Log Entry for Resource                                   */
/*============================================================================*/

#include "StorageEngine.h"

#include "eOC.h"

#include "File.hpp" //?

/*----------------------------------------------------------------------------*/
/* set resource entry                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryRes::vSet(LogEntry* pLogEntry, Resource* pResource, eRA eResAct)
{
  pLogEntry->m_eKind   = LG_RES;
  pLogEntry->m_cPrefix = 'R';
  pLogEntry->m_iAct    = pResource->iStepNo();
  //pLogEntry->m_xReqTuple.vSet(pResource->aReqTuple());

  m_eClass  = pResource->eClass();

  m_iResId  = pResource->m_iFocalId;
  m_eResAct = eResAct;
  
       if (RA_DIR_CONSTRUCT  == eResAct) pLogEntry->m_iIndicator = OC_CONSTRUCT;
  else if (RA_DIR_DESTRUCT   == eResAct) pLogEntry->m_iIndicator = OC_DESTRUCT;
  
  else if (RA_FIO_CONSTRUCT  == eResAct) pLogEntry->m_iIndicator = OC_CONSTRUCT;
  else if (RA_FIO_DESTRUCT   == eResAct) pLogEntry->m_iIndicator = OC_DESTRUCT;
  
  else if (RA_FIL_CONSTRUCT  == eResAct) pLogEntry->m_iIndicator = OC_CONSTRUCT;
  else if (RA_FIL_DESTRUCT   == eResAct) pLogEntry->m_iIndicator = OC_DESTRUCT;
  else if (RA_FIL_OPENED     == eResAct) pLogEntry->m_iIndicator = OC_OPEN;
  else if (RA_FIL_CLOSED     == eResAct) pLogEntry->m_iIndicator = OC_CLOSE;

  else if (RA_MTX_CONSTRUCT  == eResAct) pLogEntry->m_iIndicator = OC_CONSTRUCT;
  else if (RA_MTX_DESTRUCT   == eResAct) pLogEntry->m_iIndicator = OC_DESTRUCT;
  else if (RA_MTX_LOCK       == eResAct) pLogEntry->m_iIndicator = OC_BLOCKED;
  else if (RA_MTX_LOCKED     == eResAct) pLogEntry->m_iIndicator = OC_BLOCKED;
  else if (RA_MTX_SIGNAL     == eResAct) pLogEntry->m_iIndicator = OC_SIGNAL;
  else if (RA_MTX_SIGNALED   == eResAct) pLogEntry->m_iIndicator = OC_SIGNALED;
  else if (RA_MTX_UNLOCKED   == eResAct) pLogEntry->m_iIndicator = OC_UNBLOCKED;
  else if (RA_MTX_WAIT_SIGNAL == eResAct) pLogEntry->m_iIndicator = OC_INDEFINITE;
  
  else if (RA_SKT_ACCEPTED   == eResAct) pLogEntry->m_iIndicator = OC_ACCEPT;
  else if (RA_SKT_CONSTRUCT  == eResAct) pLogEntry->m_iIndicator = OC_CONSTRUCT;
  else if (RA_SKT_CONNECT    == eResAct) pLogEntry->m_iIndicator = OC_CONNECT;
  else if (RA_SKT_CONNECTED  == eResAct) pLogEntry->m_iIndicator = OC_CONNECT;
  else if (RA_SKT_DESTRUCT   == eResAct) pLogEntry->m_iIndicator = OC_DESTRUCT;
  else if (RA_SKT_RECEIVED   == eResAct) pLogEntry->m_iIndicator = OC_RECEIVE;
  else if (RA_SKT_SEND       == eResAct) pLogEntry->m_iIndicator = OC_SEND;
  else if (RA_SKT_SENT       == eResAct) pLogEntry->m_iIndicator = OC_SEND;
  else if (RA_SKT_GRAB       == eResAct) pLogEntry->m_iIndicator = OC_GRAB;
  else if (RA_SKT_GRABBED    == eResAct) pLogEntry->m_iIndicator = OC_GRAB;
  else if (RA_SKT_DROPPED    == eResAct) pLogEntry->m_iIndicator = OC_DROP;
  
  else if (RA_THD_CONSTRUCT  == eResAct) pLogEntry->m_iIndicator = OC_CONSTRUCT;
  else if (RA_THD_DESTRUCT   == eResAct) pLogEntry->m_iIndicator = OC_DESTRUCT;
  else if (RA_THD_ENTRY      == eResAct) pLogEntry->m_iIndicator = OC_ENTER;
  else if (RA_THD_EXIT       == eResAct) pLogEntry->m_iIndicator = OC_EXIT;
  else if (RA_THD_SLEEP      == eResAct) pLogEntry->m_iIndicator = OC_SLEEP;
  else if (RA_THD_AWAKE      == eResAct) pLogEntry->m_iIndicator = OC_AWAKE;
  else if (RA_THD_SPAWNED    == eResAct) pLogEntry->m_iIndicator = OC_SPAWN;
  
  if (CN_MUTEX == pResource->eClass()) {
    Mutex* pMutex = _pMutex(pResource);
    
    SP_Mutex spMutex;
    StrLib::vSprintCat((Stz*)pLogEntry->m_caNote," %s"_Stz,spMutex.sz_log(pMutex));
  }
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryRes::vOutline(s4K& sOutline)
{
  SP_Gid spId;
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
  SP_Gid spId;
  
  pFile->vPrint("%s,%s"_Stz,
              
                 spId.sz(IK_RESOURCE,m_iResId),
                aRA[m_eResAct]);
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntryRes::vToTxt(s4K& sInf, LogEntry* pEntry)
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
