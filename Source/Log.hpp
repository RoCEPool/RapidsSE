/*============================================================================*/
/* Log.hpp - Log                                                              */
/*============================================================================*/
#pragma once

namespace Logging
{
  static const char* C_LOG_REGION = "/SE_Log";
  static const char* C_LOG_FILE   = "/tmp/SE_Log.csv";

  static const i8    C_JOB_GROUP = 0;
  static const i8    C_INF_GROUP = -1;
  static const i8    C_MARGIN = 150;
  
  static const char* C_LOG_LINE =
    "-------------------------"
    "-------------------------"
    "-------------------------"
    "-------------------------"
    "-------------------------"
    "-------------------------";


  
  class Log : public ShmLog
  {
  private:
  
    // circular array of log entries
    LogEntry m_oaEntry[C_LOG_ENTRIES];
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t, Log* pActivityLog);
    void  operator delete(void*);

    Log(void);
   ~Log(void) noexcept(false);

  private:
    static Log* pAttachCreate(void);
    static Log* pAttachExisting(void);

  public:
    static void vNoteString(i8 iJobId, s48K& sString);
  
    static void vMonitorPortrait(s48K& sPortrait);
    static void vOpenByApi(void);
    static void vOpenBySE(bool bStarting);
    static void vWriteCsv(bool bShortForm);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  

  //private:
    i8 iEntryFyi(eCN eClass, Stz* szFormat, va_list xArgs);
  
    i8 iEntryJob(Job*       pJob, i8 iJobId, eJA eJobAct, Stz* szFormat, va_list xArgs);
    i8 iEntryLck(Lock*      pLck, i8 iLckId, eLA eLckAct, Stz* szFormat, va_list xArgs);
    i8 iEntryLog(Log*       pLog, i8 iLogId, eGA eLogAct, Stz* szFormat, va_list xArgs);
    i8 iEntryOpr(Op*        pOpr, i8 iOprId, eOA eOprAct, Stz* szFormat, va_list xArgs);
    i8 iEntryPrm(SePrompt*  pPrm, i8 iPrmId, ePA ePrmAct, Stz* szFormat, va_list xArgs);
    i8 iEntryQue(Queues*    pQue, i8 iQueId, eQA eQueAct, Stz* szFormat, va_list xArgs);
    i8 iEntryRes(Resource*  pRes, i8 iResId, eRA eResAct, Stz* szFormat, va_list xArgs);
    i8 iEntrySsn(Session*   pSsn, i8 iApiId, eAA eApiAct, Stz* szFormat, va_list xArgs);
    i8 iEntrySvc(Job*       pJob, i8 iSvcId, eVA eSvcAct, Stz* szFormat, va_list xArgs);
    i8 iEntrySys(AttachMap* pSys, i8 iSysId, eYA eSysAct, Stz* szFormat, va_list xArgs);
    i8 iEntryThd(Thread*    pThd, i8 iThdId, eHA eThdAct, Stz* szFormat, va_list xArgs);
    i8 iEntryTsk(Task*      pTsk, i8 iTskId, eTA eTskAct, Stz* szFormat, va_list xArgs);
    i8 iEntryTxn(Tx*        pTxn, i8 iTxnId, eXA eTxnAct, Stz* szFormat, va_list xArgs);

    LogEntry* pNewEntry(eCN eClass, Stz* szFormat, va_list xVaArgs);
    LogEntry* pNewEntry(_Loggable* pLoggable, Stz* szFormat, va_list xVaArgs);
    LogEntry* pNewEntry_NL(void);
    void vFindRange_NL(i8& i_MinEntry, i8& i_MaxEntry);
    i8   iPercentFull_NL(void);
    i8   iSize(void);
    void vWriteToFile(bool bShortForm);

    Footer* aFooter(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
