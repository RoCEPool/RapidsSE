/*============================================================================*/
/* Log.cpp - Log                                                              */
/*============================================================================*/

#include "Logging.h"
#include "AM.hpp"

extern Log* gpLog;

/*----------------------------------------------------------------------------*/
/* -- LifeCycle --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Log::operator new(size_t, Log* pLog)
{
  RASSERT(! gpLog);
  
  return pLog;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Log::operator delete(void* aAddress)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Log::Log(void)
: ShmLog(CN_LOG)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  if (0 != AM::iThisNodeNo() || Pthread::bIsJavaThread() )
    RASSERT(0);
  
  for (i8 i_Entry = 0; i_Entry < m_nEntries; i_Entry++) {
    LogEntry* pLogEntry = &m_oaEntry[i_Entry];
    pLogEntry->vZero();
  }
  
  _checkInitialised_(CN_LOG);
  
  _postcondition_(_MAGIC_,CN_LOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Log::~Log(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* -- Private Static ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create a new shm segment for Log                                           */
/*----------------------------------------------------------------------------*/
Log* Log::pAttachCreate(void)
{
  int iRegionFd;
  bool bResult;
  SysCall oSysCall;

  s32 sLogRegionNm((Stz*)C_LOG_REGION);
  
  // unlink the name of any previous segment
  IGNORE_RETURN( Shm::bUnlink(sLogRegionNm,oSysCall) );
  
  // open a new shm region
  bResult = Shm::bOpenCreate(iRegionFd,sLogRegionNm,oSysCall);
  if (! bResult)
    vFatal("Log::bOpenCreate: failed to create region %s\n"_Stz,
           sLogRegionNm.aChar());
  
  // extend the shm region
  bResult = Shm::bExtend(iRegionFd,sizeof(Log),oSysCall);
  if (! bResult)
    vFatal("Log::bOpenCreate: failed to extend region %s\n"_Stz,
           sLogRegionNm.aChar());
  
  // map the shm region into a vm segment within this process address space
  void* aSegment = Shm::aMap(iRegionFd,
                             nullptr,
                             sizeof(Log),
                             C_NO_HUGE_PAGES,
                             oSysCall);
  if (! aSegment)
    vFatal("Log::bOpenCreate: failed to map region!\n"_Stz);
  
  Log* pLog = static_cast<Log*>(aSegment);
  
  return pLog;
}

/*----------------------------------------------------------------------------*/
/* attach to existing shm segment containing the Log                          */
/*----------------------------------------------------------------------------*/
Log* Log::pAttachExisting(void)
{
  int iFD;
  SysCall oSysCall;

  s32 sLogRegionNm((Stz*)C_LOG_REGION);

  // open existing shm region
  bool bResult = Shm::bOpenExisting(iFD,sLogRegionNm,oSysCall);
  if (! bResult)
    vFatal("Log::bOpenExisting: failed to open region: %s\n"_Stz,
           sLogRegionNm.aChar());
  
  // map the shm region into a vm segment within this process address space
  void* aSegment = Shm::aMap(iFD,
                             nullptr,
                             sizeof(Log),
                             C_NO_HUGE_PAGES,
                             oSysCall);
  if (! aSegment)  {
    vTrace("Log::bOpenExisting: failed to map\n"_Stz);
    Shm::bClose(iFD,oSysCall);
    NEVER_REACH;
  }
  
  Log* pLog = static_cast<Log*>(aSegment);
  
  return pLog;
}

/*----------------------------------------------------------------------------*/
/* -- Public Static ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a multi-line string entry                                              */
/*----------------------------------------------------------------------------*/
void Log::vNoteString(i8 iJobId, s48K& sString)
{
#ifdef WITH_SHARED_LOG
  if ( ! gpLog )
    return;
  
  s4K sLine;
  char* aCharAdr = sString.aChar();

  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  gpLog->vGrabLock();
  
  while (StrLib::bEachLine(&aCharAdr,sLine.aChar())) {
    
    LogEntry* pLogEntry = gpLog->pNewEntry_NL();
    pLogEntry->vInit(CN_NONE,sLine);
    
    pLogEntry->m_xJob.vSet(pLogEntry,iJobId,JA_NONE);
  }
  
  gpLog->vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲

#endif
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* monitor profile                                                            */
/*----------------------------------------------------------------------------*/
void Log::vMonitorPortrait(s48K& sPortrait)
{
#ifdef WITH_SHARED_LOG
  if (! gpLog) {
    sPortrait.vSprint("Log: activity map not configured\n"_Stz);
    return;
  }
  
  gpLog->vGrabLock();
  i8 iPercentFull = gpLog->iPercentFull_NL();
  gpLog->vDropLock();
  
  i8 iMapSize = gpLog->iSize();
  s32 sShmMag;
  MagLib::vByteMag(iMapSize, sShmMag);
  
  s32 sArrayMag;
  MagLib::vUnitMag(gpLog->m_nEntries, sArrayMag);

  sPortrait.vSprint("Log: shm %s array %s marks %lld; %lld%% full;\n"_Stz,
                    sShmMag.aStz(),
                    sArrayMag.aStz(),
                    gpLog->m_i_Entry,
                    iPercentFull
                     );
#else
  sPortrait.vSprint("Log: not compiled WITH_SHARED_LOG\n"_Stz);
#endif
}

/*----------------------------------------------------------------------------*/
/* open Log (called by API)                                                   */
/*----------------------------------------------------------------------------*/
void Log::vOpenByApi(void)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLog())
    return;
  
  if (gpLog)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Log* pLog = Log::pAttachExisting();
  if (AM::bVerbose())
    vTrace("Log %s : "
           "attached to existing map (size %lldB, max %lld marks)\n\n"_Stz,
           C_LOG_REGION,
           sizeof(Log),
           C_LOG_ENTRIES);
  
  gpLog = pLog;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  AM::vLogLog(gpLog,GA_LOG_OPENED_API,"Opened by API"_Stz);
  
  Now oNow;
  AM::vLogLog(gpLog,GA_LOG_TIME,"Time is: "_Stz,oNow.sz());
#endif
}

/*----------------------------------------------------------------------------*/
/* create or open Log (called by SE)                                          */
/*----------------------------------------------------------------------------*/
void Log::vOpenBySE(bool bStarting)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLog())
    return;
  
  if (gpLog)
    vFatal("Log::vOpenBySE() -- activity map already opened\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Log* pLog = nullptr;
  
  if (bStarting) {
    pLog = Log::pAttachCreate();
    if (AM::bVerbose())
      vTrace("Log %s : "
             "created new map (size %lldB, max %lld marks)\n\n"_Stz,
             C_LOG_REGION,
             sizeof(Log),
             C_LOG_ENTRIES);
    gpLog = new(pLog) Log;
   }
   else {
    pLog = Log::pAttachExisting();
    if (AM::bVerbose())
      vTrace("Log %s : "
             "attached to existing map (size %lldB, max %lld marks)\n\n"_Stz,
             C_LOG_REGION,
             sizeof(Log),
             C_LOG_ENTRIES);
    
    gpLog = pLog;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s256 sWelcome;
  AM::vWelcomeText(sWelcome);
  AM::vLogLog(gpLog,GA_LOG_WELCOME,sWelcome);
  
  Now oNow;
  AM::vLogLog(gpLog,GA_LOG_TIME,"Time is: %s"_Stz,oNow.sz());

  s32 sMem;
  MagLib::vByteMag(Hardware::iPhysicalMemory(),sMem);
  
  s32 sClock;
  MagLib::vHzMag(Hardware::iClockFrequency(),sClock);

  s32 sHost;
  s32 sUser;
  Hardware::vHostName(sHost);
  Hardware::vLoginName(sUser);

  AM::vLogLog(gpLog,GA_LOG_HOST,
              "hw: host %s user %s; "
              "%lld cores; %s memory; %s clock frequency"_Stz,
             sHost.aChar(),
             sUser.aChar(),
              
             Hardware::iCoreCount(),
             sMem.aStz(),
             sClock.aStz()
             );
  
  AM::vLogArguments();
#endif
}

/*----------------------------------------------------------------------------*/
/* write Log to csv file                                                      */
/*----------------------------------------------------------------------------*/
void Log::vWriteCsv(bool bShortForm)
{
#ifdef WITH_SHARED_LOG
  if (! gpLog)
    return;
  
  gpLog->vWriteToFile(bShortForm);
#endif
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a information log entry                                                */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryFyi(eCN eClassNo,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eClassNo,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xFyi.vSet(pLogEntry);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a api log entry                                                        */
/*----------------------------------------------------------------------------*/
i8 Log::iEntrySsn(eCN eClassNo, i8 iSsnId, eAA eApiAct, i8 i_Prev,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eClassNo,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xSsn.vSet(pLogEntry,iSsnId,eApiAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a job log entry                                                        */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryJob(eCN eClassNo, i8 iJobId, eJA eJobAct, i8 i_Prev,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eClassNo,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xJob.vSet(pLogEntry,iJobId,eJobAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a lock log entry                                                       */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryLck(eCN eClassNo, i8 iLckId, eLA eLckAct, i8 i_Prev,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eClassNo,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xLck.vSet(pLogEntry,iLckId,eLckAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a log log entry                                                        */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryLog(eCN eClassNo, i8 iLogId, eGA eLogAct, i8 i_Prev,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eClassNo,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xLog.vSet(pLogEntry,iLogId,eLogAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a object log entry                                                     */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryObj(Object* pObj, eCA eObjAct, i8 i_Prev,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(pObj->eClass(),szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xObj.vSet(pLogEntry,pObj,eObjAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a operator log entry                                                   */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryOpr(eCN eClassNo, i8 iOprId, eOA eOprAct, i8 i_Prev,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eClassNo,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xOpr.vSet(pLogEntry,iOprId,eOprAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a prompt log entry                                                     */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryPrm(eCN eClassNo, i8 iPrmId, ePA ePrmAct, i8 i_Prev,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eClassNo,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xPrm.vSet(pLogEntry,iPrmId,ePrmAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a queues log entry                                                     */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryQue(eCN eClassNo, i8 iQueId, eQA eQueAct, i8 i_Prev,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eClassNo,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xQue.vSet(pLogEntry,iQueId,eQueAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a resource log entry                                                   */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryRes(eCN eClassNo, i8 iResId, eRA eResAct, eRK eResKnd, i8 i_Prev,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eClassNo,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xRes.vSet(pLogEntry,iResId,eResKnd,eResAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a service log entry                                                    */
/*----------------------------------------------------------------------------*/
i8 Log::iEntrySvc(eCN eClassNo, i8 iSvcId, eVA eSvcAct, i8 i_Prev,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eClassNo,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xSvc.vSet(pLogEntry,iSvcId,eSvcAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a system log entry                                                     */
/*----------------------------------------------------------------------------*/
i8 Log::iEntrySys(eCN eClassNo, i8 iSysId, eYA eSysAct, i8 i_Prev,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eClassNo,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xSys.vSet(pLogEntry,iSysId,eSysAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a thread log entry                                                     */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryThd(eCN eClassNo, i8 iThdId, eHA eThdAct, i8 i_Prev,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eClassNo,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xThd.vSet(pLogEntry,iThdId,eThdAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a task log entry                                                       */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryTsk(eCN eClassNo, i8 iTskId, eTA eTskAct, i8 i_Prev,
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eClassNo,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xTsk.vSet(pLogEntry,iTskId,eTskAct);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* add a thread log entry                                                     */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryTxn(eCN eClassNo, i8 iTxnId, eXA eTxnAct, i8 i_Prev, 
                  Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(eClassNo,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pLogEntry->m_xTxn.vSet(pLogEntry,iTxnId,eTxnAct);
  
  return pLogEntry->m_i_Entry;
}



/*----------------------------------------------------------------------------*/
/* write an LogEntry                                                          */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryJob_arg(i8 iJobId, i8 iForeign,
                      Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  LogEntry* pLogEntry = pNewEntry(CN_NONE,szFormat,xVaArgs);
  
  if (! pLogEntry)
    return C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  char caMark[1024];
  
  vsnprintf(caMark,sizeof(caMark)-1,szFormat->aChar(),xVaArgs);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  StrLib::vCopyLen(pLogEntry->m_caText,caMark,C_LOG_TEXT - 1);
  
  pLogEntry->m_xJob.vSet(pLogEntry,iJobId,JA_NONE);
  
  return pLogEntry->m_i_Entry;
}

/*----------------------------------------------------------------------------*/
/* write an LogEntry                                                          */
/*----------------------------------------------------------------------------*/
i8 Log::iEntryJob(i8 iJobId, i8 iForeign, Stz* aFormat, ...)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  va_list xVaArgs;
  
  va_start(xVaArgs,aFormat);
  iEntryJob_arg(iJobId,iForeign,aFormat,xVaArgs);
  va_end(xVaArgs);
  
  return C_UNKNOWN;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get a new mark in Log                                                      */
/*----------------------------------------------------------------------------*/
LogEntry* Log::pNewEntry(eCN eClassNo, Stz* szFormat, va_list xVaArgs)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);

  char caText[1024];
  vsnprintf(caText,sizeof(caText)-1,szFormat->aChar(),xVaArgs);

  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  vGrabLock();
  
  LogEntry* pLogEntry = pNewEntry_NL();
  
  vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲
  
  pLogEntry->vInit(eClassNo,(Stz*)caText);
  
  //StrLib::vCopyLen(pLogEntry->m_caText,caText,C_LOG_TEXT - 1);

  return pLogEntry;
}

/*----------------------------------------------------------------------------*/
/* get a new mark in Log (non-locking)                                        */
/*----------------------------------------------------------------------------*/
LogEntry* Log::pNewEntry_NL(void)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);

  m_okLock.vAssertWrite();
  
  LogEntry* pLogEntry = &m_oaEntry[m_i_Entry % m_nEntries];
  
  pLogEntry->m_i_Entry = m_i_Entry;
  
  m_i_Entry++;
  
  return pLogEntry;
}

/*----------------------------------------------------------------------------*/
/* find active range                                                          */
/*----------------------------------------------------------------------------*/
void Log::vFindRange_NL(i8& i_MinEntry, i8& i_MaxEntry)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  RASSERT(m_okLock.bIsLocked());
  
  // should be a quicker way to do this
  
  i_MinEntry = C_I8_MAX;
  i_MaxEntry = C_I8_MIN;
  
  for (i8 i_Entry = 0; i_Entry < m_nEntries; i_Entry++) {
    
    if (-1 == m_oaEntry[i_Entry].m_i_Entry)
      return;
    
    if (m_oaEntry[i_Entry].m_i_Entry < i_MinEntry)
      i_MinEntry = m_oaEntry[i_Entry].m_i_Entry;
    
    if (m_oaEntry[i_Entry].m_i_Entry > i_MaxEntry)
      i_MaxEntry = m_oaEntry[i_Entry].m_i_Entry;
  }
}

/*----------------------------------------------------------------------------*/
/* calculate how full the log is                                              */
/*----------------------------------------------------------------------------*/
i8 Log::iPercentFull_NL(void)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  RASSERT(m_okLock.bIsLocked());
  
  i8 nEntriesInUse = 0;
  i8 iPercent = 0;
  
  i8 i_MinEntry;
  i8 i_MaxEntry;
  vFindRange_NL(i_MinEntry,i_MaxEntry);
  
  if (i_MinEntry < i_MaxEntry) {
    //
    // Example: 0 .. 25 __ [64]
    //          i_MinEntry is 0
    //          i_MaxEntry is 48
    //          m_nEntries  is 64
    // then only (0..25) is in use out of 64
    //
    nEntriesInUse = (i_MaxEntry - i_MinEntry);
  }
  else {
    //
    // Example: .. 25 __ 48 .. [64]
    //          i_MinEntry is 48
    //          i_MaxEntry is 25
    //          m_nEntries  is 64
    // then (48..63) + (0..25) is in use
    //
    nEntriesInUse = (m_nEntries - i_MinEntry) + (i_MaxEntry);
  }
  
  iPercent = (nEntriesInUse * 100) / m_nEntries;
  
  return iPercent;
}

/*----------------------------------------------------------------------------*/
/* compute the size of Log given gpAttachMap->m_nActivityMarks                */
/*----------------------------------------------------------------------------*/
i8 Log::iSize(void)
{
  i8 iMapSize = sizeof(Log);
  
  return iMapSize;
}

/*----------------------------------------------------------------------------*/
/* write Log to a csv file                                                    */
/*----------------------------------------------------------------------------*/
void Log::vWriteToFile(bool bShortForm)
{
  _precondition_(_MAGIC_,CN_LOG,_FOOTER_);
  
  s256 sLogPath((Stz*)C_LOG_FILE);
  
  File* pLogFile = new File;
  
  bool bOpened = pLogFile->bOpenWrite(sLogPath);
  if (! bOpened) {
    vTrace("Unable to open log file: %s\n"_Stz,sLogPath.aChar());
    delete pLogFile;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Now oNow;
  AM::vLogLog(gpLog,GA_LOG_WRITING,"Writing Log at: %s"_Stz,oNow.sz());
  
  s256 sWelcome;
  AM::vWelcomeText(sWelcome);
  AM::vLogLog(gpLog,GA_LOG_WELCOME,sWelcome);
  
  i8 iUpTimeSsec = AM::iElapsedSsec();
  AM::vLogLog(gpLog,GA_LOG_UPTIME,"Uptime: %llds; Requests: %lld"_Stz,iUpTimeSsec,C_UNKNOWN);

  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  vGrabLock();
  
  i8 i_MinEntry;
  i8 i_MaxEntry;
  i8 iTotalDeltaUsec = 0;
  
  vFindRange_NL(i_MinEntry,i_MaxEntry);
  
  LogEntry* pLogEntry = &m_oaEntry[i_MinEntry % m_nEntries];
  pLogEntry->vWrite(pLogFile,0,iTotalDeltaUsec);
  
  for (i8 i_Entry = i_MinEntry + 1; i_Entry <= i_MaxEntry; i_Entry++) {
    
    pLogEntry = &m_oaEntry[i_Entry % m_nEntries];
    
    if (-1 == pLogEntry->m_i_Entry)
      break;
    
    LogEntry* pPrevEntry = &m_oaEntry[(i_Entry - 1) % m_nEntries];
    i8 iEntryDeltaUsec = Tick::iToUsec(pLogEntry->m_iTick - pPrevEntry->m_iTick);

    pLogEntry->vWrite(pLogFile,iEntryDeltaUsec,iTotalDeltaUsec);
  }
  
  vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲
  
  pLogFile->vClose();
  
  vTrace("Log written to: %s\n"_Stz,sLogPath.aChar());
  
  delete pLogFile;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Log);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
