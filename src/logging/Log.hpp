/*============================================================================*/
/* Log.hpp - Log                                                              */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
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
    
    bool m_bLogFyi;
  
    bool m_bLogDdl;
    bool m_bLogJob;
    bool m_bLogLck;
    bool m_bLogLog;
    bool m_bLogOpr;
    bool m_bLogMgt;
    bool m_bLogPrm;
    bool m_bLogQue;
    bool m_bLogRes;
    bool m_bLogSsn;
    bool m_bLogSvc;
    bool m_bLogSys;
    bool m_bLogThd;
    bool m_bLogTsk;
    bool m_bLogTxn;
  
//    bool m_bLogResDir;
//    bool m_bLogResFifo;
//    bool m_bLogResFile;
//    bool m_bLogResMutex;
//    bool m_bLogResPsxThd;
//    bool m_bLogResSocket;

    bool m_baLogClass[CN_ENUMS];
  
    // circular array of log entries
    LogEntry m_oaEntry[C_MAX_LOG_ENTRIES];
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t, Log* pActivityLog);
    void  operator delete(void*);

    Log(void);
   ~Log(void) noexcept(false);
  
    void vHow(i8 iReqId, i8 iDetail);
    void vHow(i8 iReqId, i8 iDetail, LogEntry* pLogEntry);

  private:
    static Log* pAttachCreate(void);
    static Log* pAttachExisting(void);

  public:
    static void vNoteString(i8 iJobId, s48K& sString);
  
    static bool bEntryExists(i8 i_Entry);
    static i8   iEntryPrev(i8 i_Entry);
    static void vEntrySprint(s4K& sString, i8 i_Entry);
    static void vMonitorPortrait(s48K& sPortrait);
    static void vOpenByApi(void);
    static void vOpenBySE(bool bStarting);
    static void vWriteCsv(bool bShortForm);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  

  //private:
    i8 iEntryFyi(eCN eClass, Stz* szFormat, va_list xArgs);
  
    i8 iEntryDdl(eSL, Job*       pJob, eDA eDdlAct, Stz* szFormat, va_list xArgs);
    i8 iEntryJob(eSL, Job*       pJob, eJA eJobAct, Stz* szFormat, va_list xArgs);
    i8 iEntryLck(eSL, Lock*      pLck, eLA eLckAct, Stz* szFormat, va_list xArgs);
    i8 iEntryLog(eSL, Log*       pLog, eGA eLogAct, Stz* szFormat, va_list xArgs);
    i8 iEntryOpr(eSL, Op*        pOpr, eOA eOprAct, Stz* szFormat, va_list xArgs);
    i8 iEntryMgt(eSL, Job*       pJob, eMA eMgtAct, Stz* szFormat, va_list xArgs);
    i8 iEntryPrm(eSL, SePrompt*  pPrm, ePA ePrmAct, Stz* szFormat, va_list xArgs);
    i8 iEntryQue(eSL, Queues*    pQue, eQA eQueAct, Stz* szFormat, va_list xArgs);
    i8 iEntryRes(eSL, Resource*  pRes, eRA eResAct, Stz* szFormat, va_list xArgs);
    i8 iEntrySsn(eSL, Session*   pSsn, eAA eApiAct, Stz* szFormat, va_list xArgs);
    i8 iEntrySvc(eSL, Job*       pJob, eVA eSvcAct, Stz* szFormat, va_list xArgs);
    i8 iEntrySys(eSL, AttachMap* pSys, eYA eSysAct, Stz* szFormat, va_list xArgs);
    i8 iEntryThd(eSL, Thread*    pThd, eHA eThdAct, Stz* szFormat, va_list xArgs);
    i8 iEntryTsk(eSL, Task*      pTsk, eTA eTskAct, Stz* szFormat, va_list xArgs);
    i8 iEntryTxn(eSL, Tx*        pTxn, eXA eTxnAct, Stz* szFormat, va_list xArgs);

    void vFocalOutline(s4K& sOutline);

    LogEntry* pGetEntry(i8 i_Entry);
  
    LogEntry* pNewEntry(eSL, eCN eClass, Stz* szFormat, va_list xVaArgs);
    LogEntry* pNewEntry(eSL, Focal* pFocal, Stz* szFormat, va_list xVaArgs);
    LogEntry* pNewEntry_NL(void);
    void vFindRange_NL(i8& i_MinEntry, i8& i_MaxEntry);
    i8   iPercentFull_NL(void);
    i8   iSize(void);
    void vWriteToFile(bool bCsvFile, bool bShortForm);
  
    // -- -- -- -- -- --
  
    void vLog(TokenVec* pTknVec);
    void vLogList(TokenVec* pTknVec);
    void vLogMark(TokenVec* pTknVec);
  
    void vMarkAll(bool bAll);
  
    bool bIsLoggable(Object* pObject);

    Footer* aFooter(void);
  };
  
  Log* _pLog(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
