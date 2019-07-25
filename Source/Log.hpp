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
    i8   iAddNoteArg(i8 iEnumGroup, i8 iEnum, i8 iForeign,
                   Stz* aFormat, va_list xArgs);
    i8   iAddNoteVbl(i8 iEnumGroup, i8 iEnum, i8 iForeign,
                   Stz* aFormat, ...);
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
