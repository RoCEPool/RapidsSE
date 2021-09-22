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

#ifdef WITH_PTNID
    PtnId m_taInLglPtn[C_MAX_BUCKETS]; // which logical partition a bucket is in
#else
    tPN m_taInLglPtn[C_MAX_BUCKETS]; // which logical partition a bucket is in
#endif

  
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

#ifdef WITH_PTNID
    PtnId tWhichLglPtnNo(i8 iBucketId);
#else
    tPN  tWhichLglPtnNo(i8 iBucketId);
#endif
  
  private:
    Footer* aFooter(void);
  };
  
  BucketMap* _pBucketMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
