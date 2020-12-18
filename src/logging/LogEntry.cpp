/*============================================================================*/
/* LogEntry.cpp - Shared Log Entry                                            */
/*============================================================================*/

#include "StorageEngine.h"

#include "eOC.h"

#include "File.hpp" //?

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LogEntry* StorageEngine::_pLogEntry(void* aAddr)
{
  LogEntry* pLogEntry = reinterpret_cast<LogEntry*>(aAddr);
  
  RASSERT_TYPE(pLogEntry,CN_LOGENTRY);
 
  return pLogEntry;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* LogEntry::operator new(size_t)
{
  void* pObject = nullptr;
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void LogEntry::operator delete(void* pDataAddress)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LogEntry::LogEntry(void)
: Object(CN_LOGENTRY)
, m_xReqTuple(nullptr)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  _checkInitialised_(CN_LOGENTRY);
  
  _postcondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LogEntry::~LogEntry(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get entry as text                                                          */
/*----------------------------------------------------------------------------*/
void LogEntry::vAsTxt(s4K& sString)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  SP_Gid spId;
  SP_Gid spForId;
  SP_Elapsed spElapsed;

  sString.vSprint("[%04lld,%04lld,%s+%s] %014p⬜️"_Stz,
               m_i_Entry,
               m_i_Prev,
               m_caNode,
               spElapsed.szNsec(m_iTick - AM::iAMTimePoint()),
               m_pFocal,
               C_VARGS_END
               );

  sString.vCatSprint("%20s ⬜️ %s:%s#%03lld"_Stz,
               m_xReqTuple.m_sPurpose.szStz(),
               
               spForId.sz(m_iForId),
               spId.sz(m_eKind,m_iFocalId),
               m_iAct,
               C_VARGS_END
               );
  
  sString.vCatSprint("%-16s"_Stz,
                    aCN[m_eClass]
                   );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  s4K sInf;

  switch (m_eKind) {
    case LG_DDL: m_xDdl.vToTxt(sInf,this); break;
    case LG_FYI: m_xFyi.vToTxt(sInf,this); break;
    case LG_JOB: m_xJob.vToTxt(sInf,this); break;
    case LG_LCK: m_xLck.vToTxt(sInf,this); break;
    case LG_LOG: m_xLog.vToTxt(sInf,this); break;
    case LG_OPR: m_xOpr.vToTxt(sInf,this); break;
    case LG_PRM: m_xPrm.vToTxt(sInf,this); break;
    case LG_QUE: m_xQue.vToTxt(sInf,this); break;
    case LG_RES: m_xRes.vToTxt(sInf,this); break;
    case LG_SSN: m_xSsn.vToTxt(sInf,this); break;
    case LG_SVC: m_xSvc.vToTxt(sInf,this); break;
    case LG_SYS: m_xSys.vToTxt(sInf,this); break;
    case LG_TSK: m_xTsk.vToTxt(sInf,this); break;
    case LG_THD: m_xThd.vToTxt(sInf,this); break;
    case LG_TXN: m_xTxn.vToTxt(sInf,this); break;
    default:
      NEVER_REACH;
  }

  sString.vCatSprint(" %s"_Stz,sInf.szStz());
}

/*----------------------------------------------------------------------------*/
/* get indicator                                                              */
/*----------------------------------------------------------------------------*/
Stz* LogEntry::szIndicator(void)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  if (m_iIndicator == C_UNKNOWN)
    return aOC[0];
  else
    return aOC[m_iIndicator];
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void LogEntry::vInit(eSL eSeverity, eCN eClassNo, Stz* szText)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);

  m_i_Prev = C_UNKNOWN;
  
  m_iTick = TickingDevice::iGetTick();
  m_iNodeNo = AM::iThisNodeNo();
  StrLib::vCopyLen(m_caNode,AM::szNodeNm()->azChar(),sizeof(m_caNode) - 1);
  
  m_caNote[0] = '\0';
  StrLib::vCopyLen(m_caNote,szText->azChar(),C_MAX_LOG_NOTE - 1);

  m_iAct   = C_UNKNOWN;
  m_eClass = eClassNo;
  m_pFocal = nullptr;
  
  m_eSeverity = eSeverity;
  
  m_bLine = false;
  m_cPrefix = '?';
  m_iFocalId = C_UNKNOWN;
  m_iForId = C_UNKNOWN;
  
  m_iIndicator = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void LogEntry::vInit(eSL eSeverity, Focal *pFocal, Stz* szText)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  m_i_Prev = pFocal->m_i_Log;
  pFocal->m_i_Log = m_i_Entry;
  
  m_iTick = TickingDevice::iGetTick();
  m_iNodeNo = AM::iThisNodeNo();
  StrLib::vCopyLen(m_caNode,AM::szNodeNm()->azChar(),sizeof(m_caNode) - 1);
  
  m_caNote[0] = '\0';
  StrLib::vCopyLen(m_caNote,szText->azChar(),C_MAX_LOG_NOTE - 1);
  
  m_iAct    = pFocal->iStepNo();
  m_eClass  = pFocal->eClass();
  m_pFocal  = pFocal;
  m_i_Focal = pFocal->m_i_Focal;
  
  m_eSeverity = eSeverity;
  
  m_bLine = false;
  m_cPrefix = '?';
  m_iFocalId = pFocal->m_iFocalId;
  m_iForId = pFocal->m_iForId;
  
  m_iIndicator = C_UNKNOWN;
  
  ReqTuple* aReqTuple_tls = AM::aReqTuple_tls();
  if (aReqTuple_tls)
    m_xReqTuple.vSet(aReqTuple_tls);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void LogEntry::vSprint(s4K& sString, i8 iEntryDeltaUsec, i8& iTotalDeltaUsec)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  iTotalDeltaUsec += iEntryDeltaUsec;
  
  //if (StrLib::bSame(m_xFyi.m_caNote,"delta reset"))
  //  iTotalDeltaUsec = 0;
  
  vSprintCsv(sString,iEntryDeltaUsec,iTotalDeltaUsec);
  
  switch (m_eKind) {
    case LG_FYI: m_xFyi.vOutline(sString); break;
    case LG_JOB: m_xJob.vOutline(sString); break;
    case LG_LCK: m_xLck.vOutline(sString); break;
    case LG_LOG: m_xLog.vOutline(sString); break;
    case LG_OPR: m_xOpr.vOutline(sString); break;
    case LG_PRM: m_xPrm.vOutline(sString); break;
    case LG_QUE: m_xQue.vOutline(sString); break;
    case LG_RES: m_xRes.vOutline(sString); break;
    case LG_SSN: m_xSsn.vOutline(sString); break;
    case LG_SVC: m_xSvc.vOutline(sString); break;
    case LG_SYS: m_xSys.vOutline(sString); break;
    case LG_TSK: m_xTsk.vOutline(sString); break;
    case LG_THD: m_xThd.vOutline(sString); break;
    case LG_TXN: m_xTxn.vOutline(sString); break;
    default:
      NEVER_REACH;
  }
  
  sString.vCatSprint(" %s"_Stz,m_caNote);
}

/*----------------------------------------------------------------------------*/
/* sprint                                                                     */
/*----------------------------------------------------------------------------*/
void LogEntry::vSprintCsv(s4K& sString, i8 iTickDeltaUsec, i8& iTotalDelta)
{
   SP_Elapsed spElapsed;

   sString.vSprint("[%06lld,%06lld] "
                   "%s %s+%s-"
                   "%s"_Stz,
                
                    m_i_Entry,
                    m_i_Prev,

                    aSL[m_eSeverity],
                    m_caNode,
                    spElapsed.szNsec(m_iTick - AM::iAMTimePoint()),

                    aLG[m_eKind]
                  );
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* write log to csv file                                                      */
/*----------------------------------------------------------------------------*/
void LogEntry::vWriteCsv(File* pFile, i8 iEntryDeltaUsec, i8& iTotalDeltaUsec)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  iTotalDeltaUsec += iEntryDeltaUsec;
  
  //if (StrLib::bSame(m_xFyi.m_caNote,"delta reset"))
  //  iTotalDeltaUsec = 0;
  
  vWriteCsvEntry(pFile,iEntryDeltaUsec,iTotalDeltaUsec);
  
  switch (m_eKind) {
    case LG_DDL: m_xDdl.vWriteCsv(pFile,this); break;
    case LG_FYI: m_xFyi.vWriteCsv(pFile,this); break;
    case LG_JOB: m_xJob.vWriteCsv(pFile,this); break;
    case LG_LCK: m_xLck.vWriteCsv(pFile,this); break;
    case LG_LOG: m_xLog.vWriteCsv(pFile,this); break;
    case LG_OPR: m_xOpr.vWriteCsv(pFile,this); break;
    case LG_PRM: m_xPrm.vWriteCsv(pFile,this); break;
    case LG_QUE: m_xQue.vWriteCsv(pFile,this); break;
    case LG_RES: m_xRes.vWriteCsv(pFile,this); break;
    case LG_SSN: m_xSsn.vWriteCsv(pFile,this); break;
    case LG_SVC: m_xSvc.vWriteCsv(pFile,this); break;
    case LG_SYS: m_xSys.vWriteCsv(pFile,this); break;
    case LG_TSK: m_xTsk.vWriteCsv(pFile,this); break;
    case LG_THD: m_xThd.vWriteCsv(pFile,this); break;
    case LG_TXN: m_xTxn.vWriteCsv(pFile,this); break;
    default:
      NEVER_REACH;
  }
  
  pFile->vPrint(",  %s\n"_Stz,m_caNote);
}

/*----------------------------------------------------------------------------*/
/* write entry to csv file                                                    */
/*----------------------------------------------------------------------------*/
void LogEntry::vWriteCsvEntry(File* pFile, i8 iTickDeltaUsec, i8& iTotalDelta)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  SP_Elapsed spElapsed;

  if (m_bLine)
    pFile->vPrint("%s\n"_Stz,C_LOG_LINE);
  

  pFile->vPrint(
                
                "%lld,%lld,  "
                "%lld,+%s,  "
                "%s,%s,%p,  "
                ""_Stz,
                
                m_i_Entry,
                m_i_Prev,

                m_iTick,
                spElapsed.szNsec(m_iTick - AM::iAMTimePoint()),

                m_caNode,
                aCN[m_eClass],
                m_pFocal
                
                );
}

/*----------------------------------------------------------------------------*/
/* write log entry to text file                                               */
/*----------------------------------------------------------------------------*/
void LogEntry::vWriteTxt(File* pFile)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  s4K sInf;
  
  vWriteTxtEntry(pFile);
  
  switch (m_eKind) {
    case LG_DDL: m_xDdl.vToTxt(sInf,this); break;
    case LG_FYI: m_xFyi.vToTxt(sInf,this); break;
    case LG_JOB: m_xJob.vToTxt(sInf,this); break;
    case LG_LCK: m_xLck.vToTxt(sInf,this); break;
    case LG_LOG: m_xLog.vToTxt(sInf,this); break;
    case LG_OPR: m_xOpr.vToTxt(sInf,this); break;
    case LG_PRM: m_xPrm.vToTxt(sInf,this); break;
    case LG_QUE: m_xQue.vToTxt(sInf,this); break;
    case LG_RES: m_xRes.vToTxt(sInf,this); break;
    case LG_SSN: m_xSsn.vToTxt(sInf,this); break;
    case LG_SVC: m_xSvc.vToTxt(sInf,this); break;
    case LG_SYS: m_xSys.vToTxt(sInf,this); break;
    case LG_TSK: m_xTsk.vToTxt(sInf,this); break;
    case LG_THD: m_xThd.vToTxt(sInf,this); break;
    case LG_TXN: m_xTxn.vToTxt(sInf,this); break;
    default:
      NEVER_REACH;
  }
  
  pFile->vPrint("%s\n"_Stz,sInf.szStz());
}

/*----------------------------------------------------------------------------*/
/* write log entry to text file                                               */
/*----------------------------------------------------------------------------*/
void LogEntry::vWriteTxtEntry(File* pFile)
{
  SP_Elapsed spElapsed;

  if (m_bLine)
    pFile->vPrint("%s\n"_Stz,C_LOG_LINE);
  
  SP_Gid spId;
  SP_Gid spForId;
  
  s256 sSub;
  s256 sMid;
  s256 sInf;
  
  sSub.vSprint("[%04lld,%04lld,%s+%s] %014p⬜️"_Stz,
               m_i_Entry,
               m_i_Prev,
               m_caNode,
               spElapsed.szNsec(m_iTick - AM::iAMTimePoint()),
               m_pFocal,
               C_VARGS_END
               );

  sMid.vSprint("%20s ⬜️ %s:%s#%03lld"_Stz,
               m_xReqTuple.m_sPurpose.szStz(),
               
               spForId.sz(m_iForId),
               spId.sz(m_eKind,m_iFocalId),
               m_iAct,
               C_VARGS_END
               );
  
  sInf.vSprint("%-16s"_Stz,
               aCN[m_eClass]
               );
  
  pFile->vPrint("%s %s ⬜️ "_Stz,
                sSub.szStz(),
                sMid.szStz());
}

/*----------------------------------------------------------------------------*/
/* zero                                                                       */
/*----------------------------------------------------------------------------*/
void LogEntry::vZero(void)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  m_i_Entry    = -1;
  m_iTick      = -1;
  m_caNode[0]  = '\0';
  
  m_eKind      = LG_FYI;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(LogEntry)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
