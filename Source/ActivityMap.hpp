/*============================================================================*/
/* ActivityMap.hpp - Activity Map                                             */
/*============================================================================*/
#pragma once

namespace Measurement
{
  static const i8 C_ACTIVITY_MARKS = C_1M;
  
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
    static bool bClose(void);
    static bool bCreate(void);
    static bool bOpen(void);
  
  public:
    static void vMark(Stz* aFormat, ...);
    static void vDump(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    void vMarkShm(Stz* aFormat, va_list xArgs);
    void vDumpShm(void);

    void vDropMutex(void);
    void vGrabMutex(void);
  
    ActivityMark* aNewActivityMark(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
