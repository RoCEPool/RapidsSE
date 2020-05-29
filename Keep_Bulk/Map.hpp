/*============================================================================*/
/* Map.hpp -- Map in shared memory                                            */
/*============================================================================*/
#pragma once

namespace Monitoring
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Map : public Object
  {
  public:
    void* operator new(size_t, Map* pMap);
    void  operator delete(void*);
  
    Map(eCN eClassNo);
    ~Map(void) noexcept(false);
  
  protected:
    static void* aMapCreate(s32 sRegionNm, i8 nRegionSz);
    static void* aMapExisting(s32 sRegionNm, i8 nRegionSz);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
