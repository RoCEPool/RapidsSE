/*============================================================================*/
/* ShmLog.hpp -- Log in shared memory                                         */
/*============================================================================*/
#pragma once

namespace Logging
{
  static const i8  C_LOG_MARKS = C_1M * 1;
  
  class ShmLog : public Object
  {
  protected:
    Lock m_okLock;
    i8   m_i_Mark;  // current mark subscript
    i8   m_nMarks;  // number of available marks in array

  public:
    void* operator new(size_t, ShmLog* pShmLog);
    void  operator delete(void*);
  
    ShmLog(eCN eClassNo);
    ~ShmLog(void) noexcept(false);
  
  protected:
    static void* aCreateNew(s32 sRegionNm, i8 nRegionSz);
    static void* aOpenExisting(s32 sRegionNm, i8 nRegionSz);
  
    void vDropLock(void);
    void vGrabLock(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
