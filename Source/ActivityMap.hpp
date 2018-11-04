/*============================================================================*/
/* ActivityMap.hpp - Activity Map                                             */
/*============================================================================*/
#pragma once

namespace Measurement
{
  static const i8    C_ACTIVITY_MARKS  = C_1M;
  static const char* C_ACTIVITY_REGION = "/SE_ActivityMap";

  class ActivityMap
  {
  private:
    pthread_mutex_t     m_xMutex;     // mutex variable
    pthread_mutexattr_t m_xMutexAttr; // mutex shared attribute
  
    i8 m_nMarks;
    ActivityMark m_xActivity[C_ACTIVITY_MARKS];

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ActivityMap(void);
    ~ActivityMap(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  private:
    static bool bCloseShm(void);
    static bool bCreateNewShm(void);
    static bool bOpenExistingShm(void);
  
  public:
    static void vMark(i8 iJobId, Stz* aFormat, ...);
    static void vMarkApi(i8 eApiCall, i8 iJobId, Stz* aFormat, ...);
    static void vOpenByApi(void);
    static void vOpenBySE(void);
    static void vDump(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    void vWriteMark(i8 iJobId, Stz* aFormat, va_list xArgs);
    void vDumpMap(void);

    void vLockMutex(void);
    void vUnlockMutex(void);
  
    ActivityMark* aNewActivityMark(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
