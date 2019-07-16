/*============================================================================*/
/* ShmLog.hpp -- Log in shared memory                                         */
/*============================================================================*/
#pragma once

namespace Monitoring
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class ShmLog : public Object
  {
  public:
    void* operator new(size_t, ShmLog* pMap);
    void  operator delete(void*);
  
    ShmLog(eCN eClassNo);
    ~ShmLog(void) noexcept(false);
  
  protected:
    static void* aMapCreate(s32 sRegionNm, i8 nRegionSz);
    static void* aMapExisting(s32 sRegionNm, i8 nRegionSz);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
