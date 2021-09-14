/*============================================================================*/
/* BucketMap_dyn.hpp -- Dynamic Bucket (to logical partition) Map             */
/*============================================================================*/
#pragma once

namespace STORE
{
  class BucketMap_dyn : public Operand
  {
  private:
    Lock m_okLock;
  
    Location m_oLocation; // logical location of BucketMap_dyn

    tPN m_taInLglPtn[C_MAX_BUCKETS]; // which logical partition a bucket is in
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    BucketMap_dyn(i8 nPartitions);
   ~BucketMap_dyn(void) noexcept(false);
  
    static void vPostRecv(void* aBucketMap);
  
    Location* pLocation(void);
  
    void vResetPointers(void);
    tPN  tWhichLglPtnNo(i8 iBucketId);
  
  private:
    Footer* aFooter(void);
  };
  
  BucketMap_dyn* _pBucketMap_dyn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
