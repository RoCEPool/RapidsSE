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
  
    i8 iEntrySsn(Session*   pSsn, eCN eClass, i8 iApiId, eAA eApiAct, i8 i_Prev, Stz* szFormat, va_list xArgs);
    i8 iEntryJob(Job*       pJob, eCN eClass, i8 iJobId, eJA eJobAct, i8 i_Prev, Stz* szFormat, va_list xArgs);
    i8 iEntryLck(Lock*      pLck, eCN eClass, i8 iLckId, eLA eLckAct, i8 i_Prev, Stz* szFormat, va_list xArgs);
    i8 iEntryLog(Log*       pLog, eCN eClass, i8 iLogId, eGA eLogAct, i8 i_Prev, Stz* szFormat, va_list xArgs);
    i8 iEntryObj(Object* pObject, eCA eObjAct, i8 i_Prev, Stz* szFormat, va_list xArgs);
    i8 iEntryOpr(Op*        pOpr, eCN eClass, i8 iOprId, eOA eOprAct, i8 i_Prev, Stz* szFormat, va_list xArgs);
    i8 iEntryPrm(SePrompt*  pPrm, eCN eClass, i8 iPrmId, ePA ePrmAct, i8 i_Prev, Stz* szFormat, va_list xArgs);
    i8 iEntryQue(Queues*    pQue, eCN eClass, i8 iQueId, eQA eQueAct, i8 i_Prev, Stz* szFormat, va_list xArgs);
    i8 iEntryRes(Resource*  pRes, eCN eClass, i8 iResId, eRA eResAct, eRK eResKnd,   i8 i_Prev, Stz* szFormat, va_list xArgs);
    i8 iEntrySvc(Job*       pJob, eCN eClass, i8 iSvcId, eVA eSvcAct, i8 i_Prev, Stz* szFormat, va_list xArgs);
    i8 iEntrySys(AttachMap* pSys, eCN eClass, i8 iSysId, eYA eSysAct, i8 i_Prev, Stz* szFormat, va_list xArgs);
    i8 iEntryThd(Thread*    pThd, eCN eClass, i8 iThdId, eHA eThdAct, i8 i_Prev, Stz* szFormat, va_list xArgs);
    i8 iEntryTsk(Task*      pTsk, eCN eClass, i8 iTskId, eTA eTskAct, i8 i_Prev, Stz* szFormat, va_list xArgs);
    i8 iEntryTxn(Tx*        pTxn, eCN eClass, i8 iTxnId, eXA eTxnAct, i8 i_Prev, Stz* szFormat, va_list xArgs);

    i8 iEntryJob(i8 iJobId, i8 iForeign, Stz* aFormat, ...);
    i8 iEntryJob_arg(i8 iJobId, i8 iForeign, Stz* szFormat, va_list xArgs);

    LogEntry* pNewEntry(eCN eClass, Stz* szFormat, va_list xVaArgs);
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
