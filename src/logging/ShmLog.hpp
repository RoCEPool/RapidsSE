/*============================================================================*/
/* ShmLog.hpp -- Log in shared memory                                         */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class ShmLog : public Focal
  {
  protected:
    Lock m_okLock;
    i8   m_i_Entry;   // current entry subscript
    i8   m_nEntries;  // number of available entries in array

  public:
    void* operator new(size_t, ShmLog* pShmLog);
    void  operator delete(void*);
  
    ShmLog(eCN eClassNo);
   ~ShmLog(void) noexcept(false);
  
  protected:
    static void* aOpenCreate(s32 sRegionNm, i8 nRegionSz);
    static void* aOpenExisting(s32 sRegionNm, i8 nRegionSz);

    void vDropLock(void);
    void vGrabLock(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
