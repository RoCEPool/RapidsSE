/*============================================================================*/
/* BucketMap.hpp -- Bucket (to partition) Map                                 */
/*============================================================================*/
#pragma once

namespace Partitioning
{
  class BucketMap : public Object
  {
  private:
    Lock m_okLock;
  
    tPN m_taToWhichPtn[C_MAX_BUCKETS]; // which partition is a bucket in
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    BucketMap(i8 nPartitions);
   ~BucketMap(void) noexcept(false);
  
    tPN tWhichPtnNo(i8 i_Bkt);
  
  private:
    Footer* aFooter(void);
  };
  
  BucketMap* _pBucketMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
