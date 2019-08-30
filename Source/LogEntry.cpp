/*============================================================================*/
/* LogEntry.cpp - Shared Log Entry                                            */
/*============================================================================*/

#include "Logging.h"
#include "AM.hpp"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LogEntry* Logging::_pLogEntry(void* aAddr)
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
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void LogEntry::vInit(eSL eSeverity, eCN eClassNo, Stz* szText)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  m_i_Prev = C_UNKNOWN;
  
  m_iTick = Tick::iRdClock();
  m_iNodeNo = AM::iThisNodeNo();
  StrLib::vCopyLen(m_caNode,AM::szNodeName()->aChar(),sizeof(m_caNode) - 1);
  
  m_caNote[0] = '\0';
  StrLib::vCopyLen(m_caNote,szText->aChar(),C_LOG_NOTE - 1);

  m_iAct   = C_UNKNOWN;
  m_eClass = eClassNo;
  m_pFocal = nullptr;
  
  m_eSeverity = eSeverity;
  
  m_bLine = false;
  m_cPrefix = '?';
  m_iUniqueId = C_UNKNOWN;
  m_iForId = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void LogEntry::vInit(eSL eSeverity, Focal *pFocal, Stz* szText)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  m_i_Prev = pFocal->m_i_Log;
  pFocal->m_i_Log = m_i_Entry;
  
  m_iTick = Tick::iRdClock();
  m_iNodeNo = AM::iThisNodeNo();
  StrLib::vCopyLen(m_caNode,AM::szNodeName()->aChar(),sizeof(m_caNode) - 1);
  
  m_caNote[0] = '\0';
  StrLib::vCopyLen(m_caNote,szText->aChar(),C_LOG_NOTE - 1);
  
  m_iAct    = pFocal->iActNo();
  m_eClass  = pFocal->eClass();
  m_pFocal  = pFocal;
  m_i_Focal = pFocal->m_i_Focal;
  
  m_eSeverity = eSeverity;
  
  m_bLine = false;
  m_cPrefix = '?';
  m_iUniqueId = pFocal->m_iUniqueId;
  m_iForId = pFocal->m_iForId;
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
    case EK_FYI: m_xFyi.vOutline(sString); break;
    case EK_JOB: m_xJob.vOutline(sString); break;
    case EK_LCK: m_xLck.vOutline(sString); break;
    case EK_LOG: m_xLog.vOutline(sString); break;
    case EK_OPR: m_xOpr.vOutline(sString); break;
    case EK_PRM: m_xPrm.vOutline(sString); break;
    case EK_QUE: m_xQue.vOutline(sString); break;
    case EK_REQ: m_xReq.vOutline(sString); break;
    case EK_RES: m_xRes.vOutline(sString); break;
    case EK_SSN: m_xSsn.vOutline(sString); break;
    case EK_SVC: m_xSvc.vOutline(sString); break;
    case EK_SYS: m_xSys.vOutline(sString); break;
    case EK_TSK: m_xTsk.vOutline(sString); break;
    case EK_THD: m_xThd.vOutline(sString); break;
    case EK_TXN: m_xTxn.vOutline(sString); break;
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

                    aEK[m_eKind]
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
    case EK_FYI: m_xFyi.vWriteCsv(pFile,this); break;
    case EK_JOB: m_xJob.vWriteCsv(pFile,this); break;
    case EK_LCK: m_xLck.vWriteCsv(pFile,this); break;
    case EK_LOG: m_xLog.vWriteCsv(pFile,this); break;
    case EK_OPR: m_xOpr.vWriteCsv(pFile,this); break;
    case EK_PRM: m_xPrm.vWriteCsv(pFile,this); break;
    case EK_QUE: m_xQue.vWriteCsv(pFile,this); break;
    case EK_REQ: m_xReq.vWriteCsv(pFile,this); break;
    case EK_RES: m_xRes.vWriteCsv(pFile,this); break;
    case EK_SSN: m_xSsn.vWriteCsv(pFile,this); break;
    case EK_SVC: m_xSvc.vWriteCsv(pFile,this); break;
    case EK_SYS: m_xSys.vWriteCsv(pFile,this); break;
    case EK_TSK: m_xTsk.vWriteCsv(pFile,this); break;
    case EK_THD: m_xThd.vWriteCsv(pFile,this); break;
    case EK_TXN: m_xTxn.vWriteCsv(pFile,this); break;
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
/* write log to csv file                                                      */
/*----------------------------------------------------------------------------*/
void LogEntry::vWriteTxt(File* pFile)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  vWriteTxtEntry(pFile);
  
  switch (m_eKind) {
    case EK_FYI: m_xFyi.vWriteTxt(pFile,this); break;
    case EK_JOB: m_xJob.vWriteTxt(pFile,this); break;
    case EK_LCK: m_xLck.vWriteTxt(pFile,this); break;
    case EK_LOG: m_xLog.vWriteTxt(pFile,this); break;
    case EK_OPR: m_xOpr.vWriteTxt(pFile,this); break;
    case EK_PRM: m_xPrm.vWriteTxt(pFile,this); break;
    case EK_QUE: m_xQue.vWriteTxt(pFile,this); break;
    case EK_REQ: m_xReq.vWriteTxt(pFile,this); break;
    case EK_RES: m_xRes.vWriteTxt(pFile,this); break;
    case EK_SSN: m_xSsn.vWriteTxt(pFile,this); break;
    case EK_SVC: m_xSvc.vWriteTxt(pFile,this); break;
    case EK_SYS: m_xSys.vWriteTxt(pFile,this); break;
    case EK_TSK: m_xTsk.vWriteTxt(pFile,this); break;
    case EK_THD: m_xThd.vWriteTxt(pFile,this); break;
    case EK_TXN: m_xTxn.vWriteTxt(pFile,this); break;
    default:
      NEVER_REACH;
  }
  
  pFile->vPrint("\n"_Stz,m_caNote);
}

/*----------------------------------------------------------------------------*/
/* write entry to csv file                                                    */
/*----------------------------------------------------------------------------*/
void LogEntry::vWriteTxtEntry(File* pFile)
{
  SP_Elapsed spElapsed;

  if (m_bLine)
    pFile->vPrint("%s\n"_Stz,C_LOG_LINE);
  
  SP_Id spId;
  SP_Id spForId;
  SP_Id spReqId;

  pFile->vPrint("[%04lld,%04lld,%s+%s] %014p "
                "%16s:%s[%s:%s]%03lld:%-16s "
                ""_Stz,
                
                m_i_Entry,
                m_i_Prev,
                m_caNode,
                spElapsed.szNsec(m_iTick - AM::iAMTimePoint()),
                m_pFocal,

                m_xReqTuple.m_sPurpose.aStz(),
                spReqId.sz(EK_REQ,m_xReqTuple.m_iReqId),
                spForId.sz(m_iForId),
                spId.sz(m_eKind,m_iUniqueId),
                m_iAct,
                aCN[m_eClass],

                C_ENDARGS);
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
  
  m_eKind      = EK_FYI;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(LogEntry)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
