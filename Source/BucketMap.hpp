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
  
    i8   m_iaInPartition[C_MAX_BUCKETS]; // which partition is a bucket in
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    BucketMap(i8 nPartitions);
   ~BucketMap(void) noexcept(false);
  
    i8 i_WhichPartition(i8 i_Bkt);
  
  private:
    Footer* aFooter(void);
  };
  
  BucketMap* _pBucketMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
