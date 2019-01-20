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
    pthread_mutex_t     m_xMutex;     // mutex variable
    pthread_mutexattr_t m_xMutexAttr; // mutex shared attribute
  
    i8 m_i_Mark;  // current mark subscript
    i8 m_nMarks;  // number of marks in array
    ActivityMark m_xActivity[C_ACTIVITY_MARKS];
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ActivityMap(void);
    ~ActivityMap(void) noexcept(false);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  private:
    static bool bCloseShm(void);
    static bool bCreateNewShm(void);
    static bool bOpenExistingShm(void);
  
  public:
    static void vMark(i8 iJobId, Stz* aFormat, ...);
    static void vMarkApi(i8 eApiCall, i8 iJobId, Stz* aFormat, ...);
    static void vMonitorPortrait(s48K& sPortrait);
    static void vOpenByApi(void);
    static void vOpenBySE(void);
    static void vDump(void);
    static i8   iSize(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    void vWriteMark(i8 iJobId, Stz* aFormat, va_list xArgs);
    void vDumpMap(void);
    void vFindRange(i8& i_MinMark, i8& i_MaxMark);
    i8   iPercentFull_NL(void);

    void vLockMutex(void);
    void vUnlockMutex(void);
  
    ActivityMark* aNewActivityMark(void);
  
    Footer* aFooter(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
