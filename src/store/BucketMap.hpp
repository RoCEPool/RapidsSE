/*============================================================================*/
/* BucketMap.hpp -- Bucket (to logical partition) Map                         */
/*============================================================================*/
#pragma once

namespace STORE
{
  class BucketMap : public Operand
  {
  private:
    Lock m_okLock;
  
    Location m_oLocation; // logical location of bucketMap

    tPN m_taInLglPtn[C_MAX_BUCKETS]; // which logical partition a bucket is in
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    BucketMap(i8 nPartitions);
   ~BucketMap(void) noexcept(false);
  
    static void vPostRecv(void* aBucketMap);
  
    Location* pLocation(void);
  
    void vResetPointers(void);
    tPN  tWhichLglPtnNo(i8 iBucketId);
    i8   iWhichBucket(tPN tPtnNo);
  
  private:
    Footer* aFooter(void);
  };
  
  BucketMap* _pBucketMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
