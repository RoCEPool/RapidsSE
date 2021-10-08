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

    PtnId m_taPtnId_0[C_MAX_BUCKETS]; // which logical partition a bucket is in
  
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

    i8   iWhichBucket(PtnId tPtnId);
    i8   iWhichBucketNext(PtnId tPtnId, i8 iBktNo);

    PtnId tWhichLglPtnId(i8 iBucketId);

  
  private:
    Footer* aFooter(void);
  };
  
  BucketMap* _pBucketMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
