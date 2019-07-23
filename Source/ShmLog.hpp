/*============================================================================*/
/* ShmLog.hpp -- Log in shared memory                                         */
/*============================================================================*/
#pragma once

namespace Logging
{
  class ShmLog : public Object
  {
  public:
    void* operator new(size_t, ShmLog* pShmLog);
    void  operator delete(void*);
  
    ShmLog(eCN eClassNo);
    ~ShmLog(void) noexcept(false);
  
  protected:
    static void* aCreateNew(s32 sRegionNm, i8 nRegionSz);
    static void* aOpenExisting(s32 sRegionNm, i8 nRegionSz);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
