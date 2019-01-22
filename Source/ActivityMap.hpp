/*============================================================================*/
/* ActivityMap.hpp - Activity Map                                             */
/*============================================================================*/
#pragma once

namespace Measurement
{
  static const i8    C_ACTIVITY_MARKS  = C_1M * 10;
  static const char* C_ACTIVITY_REGION = "/SE_ActivityMap";

  class ActivityMap : public Object
  {
  private:
    Lock m_oLock;
    i8   m_i_Mark;  // current mark subscript
    i8   m_nMarks;  // number of available marks in array
  
    // circular array of activity marks
    ActivityMark m_oaMark[C_ACTIVITY_MARKS];
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ActivityMap(void);
    ~ActivityMap(void) noexcept(false);

  private:
    static bool bAttachExisting(void);
    static bool bCreateNew(void);
  
  public:
    static void vMark(i8 iJobId, Stz* aFormat, ...);
    static void vMarkApi(i8 eApiCall, i8 iJobId, Stz* aFormat, ...);
    static void vMonitorPortrait(s48K& sPortrait);
    static void vOpenByApi(void);
    static void vOpenBySE(void);
    static i8   iSize(void);
    static void vWriteCsv(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  

  private:
    ActivityMark* pNewActivityMark(void);
    void vDropLock(void);
    void vFindRange_NL(i8& i_MinMark, i8& i_MaxMark);
    void vGrabLock(void);
    i8   iPercentFull_NL(void);
    void vWriteMark(i8 iJobId, Stz* aFormat, va_list xArgs);
    void vWriteToFile(void);

    Footer* aFooter(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
