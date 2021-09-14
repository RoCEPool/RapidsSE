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
  
    PtnId* m_paPtnId;
    i8     m_iActive;

    PtnId m_taPtnId_0[C_MAX_BUCKETS]; // bucketId -> PtnId
    PtnId m_taPtnId_1[C_MAX_BUCKETS]; // bucketId -> Ptn

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    BucketMap_dyn(void);
   ~BucketMap_dyn(void) noexcept(false);
  
    static void vPostRecv(void* aBucketMap_dyn);
  
    Location* pLocation(void);
  
    //?? void vResetPointers(void);
    PtnId  tToPtnId(i8 iBucketId);
  
  private:
    Footer* aFooter(void);
  };
  
  BucketMap_dyn* _pBucketMap_dyn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
