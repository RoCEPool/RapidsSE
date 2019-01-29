/*============================================================================*/
/* ActivityMap.hpp - Activity Map                                             */
/*============================================================================*/
#pragma once

namespace Measurement
{
  static const i8    C_ACTIVITY_MARKS  = C_1M * 10;
  static const char* C_ACTIVITY_REGION = "/tmp/SE_ActivityMap.csv";

  class ActivityMap : public Object
  {
  private:
    Lock m_okLock;
    i8   m_i_Mark;  // current mark subscript
    i8   m_nMarks;  // number of available marks in array
  
    // circular array of activity marks
    ActivityMark m_oaMark[C_ACTIVITY_MARKS];
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void* operator new(size_t, ActivityMap* pActivityMap);
    void  operator delete(void*);

    ActivityMap(void);
    ~ActivityMap(void) noexcept(false);

  private:
    static bool bOpenCreate(void);
    static bool bOpenExisting(void);

  public:
    static void vMark(i8 iJobId, Stz* aFormat, ...);
    static void vMarkApi(i8 eApiCall, i8 iJobId, Stz* aFormat, ...);
    static void vMarkApiEntry(i8 eApiCall, i8 iJobId=-1);
    static void vMarkApiError(i8 eApiCall, i8 iError, i8 iJobId=-1);
    static void vMarkApiExit(i8 eApiCall, i8 iJobId=-1);
    static void vMarkString(i8 iJobId, s48K& sString);
    static void vMonitorPortrait(s48K& sPortrait);
    static void vOpenByApi(void);
    static void vOpenBySE(void);
    static i8   iSize(void);
    static void vWriteCsv(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  

  private:
    ActivityMark* pNewActivityMark(void);
    ActivityMark* pNewActivityMark_NL(void);
    void vDropLock(void);
    void vFindRange_NL(i8& i_MinMark, i8& i_MaxMark);
    void vGrabLock(void);
    i8   iPercentFull_NL(void);
    void vWriteMark(i8 iJobId, Stz* aFormat, ...);
    void vWriteMark(i8 iJobId, Stz* aFormat, va_list xArgs);
    void vWriteToFile(void);

    Footer* aFooter(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
