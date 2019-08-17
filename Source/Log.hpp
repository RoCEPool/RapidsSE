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
  static const i8    C_MARGIN = 143;
  
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
    static void vNote(Stz* aFormat, ...);
    static void vNoteSection(Stz* szSectionNm);
    static void vNoteString(i8 iJobId, s48K& sString);
  
    static void vMonitorPortrait(s48K& sPortrait);
    static void vOpenByApi(void);
    static void vOpenBySE(bool bStarting);
    static void vWriteCsv(bool bShortForm);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  

  //private:
    i8 iEntryFyi(Stz* szFormat, va_list xArgs);
  
    i8 iEntryApi(i8 iApiId, eAA eApiAct, Stz* szFormat, va_list xArgs);
    i8 iEntryJob(i8 iJobId, eJA eJobAct, Stz* szFormat, va_list xArgs);
    i8 iEntryLck(i8 iJckId, eLA eLckAct, Stz* szFormat, va_list xArgs);
    i8 iEntryObj(Object* pObject, eCA eObjAct, Stz* szFormat, va_list xArgs);
    i8 iEntryOpr(i8 iOprId, eOA eOprAct, Stz* szFormat, va_list xArgs);
    i8 iEntryPrm(i8 iPrmId, ePA ePrmAct, Stz* szFormat, va_list xArgs);
    i8 iEntryRes(i8 iResId, eRA eResAct, eRK eResKnd,   Stz* szFormat, va_list xArgs);
    i8 iEntrySvc(i8 iThdId, eVA eSvcAct, Stz* szFormat, va_list xArgs);
    i8 iEntryThd(i8 iThdId, eHA eThdAct, Stz* szFormat, va_list xArgs);
    i8 iEntryTsk(i8 iTskId, eTA eTskAct, Stz* szFormat, va_list xArgs);
    i8 iEntryTxn(i8 iTxnId, eXA eTxnAct, Stz* szFormat, va_list xArgs);

    i8 iEntryJob(i8 iJobId, i8 iForeign, Stz* aFormat, ...);
    i8 iEntryJob_arg(i8 iJobId, i8 iForeign, Stz* szFormat, va_list xArgs);

    LogEntry* pNewEntry(void);
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
