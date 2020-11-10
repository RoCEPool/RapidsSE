/*============================================================================*/
/* BucketMap.hpp -- Bucket (to logical partition) Map                         */
/*============================================================================*/
#pragma once

namespace Partitioning
{
  class BucketMap : public Object
  {
  private:
    Lock m_okLock;
  
    tPN m_taInLglPtn[C_MAX_BUCKETS]; // which logical partition a bucket is in
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    BucketMap(i8 nPartitions);
   ~BucketMap(void) noexcept(false);
  
    tPN tWhichLglPtnNo(i8 i_Bkt);
  
  private:
    Footer* aFooter(void);
  };
  
  BucketMap* _pBucketMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
