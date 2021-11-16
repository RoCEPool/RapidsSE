/*============================================================================*/
/* BucketMap_dyn.hpp -- Dynamic Bucket (to logical partition) Map             */
/*============================================================================*/
#pragma once

#include "eTT.h"

namespace STORE
{
  class BucketMap_dyn : public Operand

  {
  private:
    Lock m_okLock;
  
    Location m_oLocation; // logical location of BucketMap_dyn
  
    PtnId* m_paPtnId;
    i8     m_iActive;

    PtnId m_taPtnId_0[C_MAX_BUCKETS_IN_GRID]; // bucketId -> PtnId
    PtnId m_taPtnId_1[C_MAX_BUCKETS_IN_GRID]; // bucketId -> PtnId

    s32 m_sTableNm; // table name (leave for debug)
    eTT m_eKind;    // table kind (leave for debug)

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    BucketMap_dyn(eTT eKind);
   ~BucketMap_dyn(void) noexcept(false);
  
    static void vPostRecv(void* aBucketMap_dyn);
  
    Location* pLocation(void);
  
    //?? void vResetPointers(void);
    PtnId tBktToPtnId(i8 iBucketId);
    i8 iPtnIdToBkt(PtnId tPtnId);
    i8 iWhichBucketNext(PtnId tPtnId, i8 iBktNo);
    i8 tWhichLglPtnId(i8 iBucketId);

    void vSwapMap(void);
    void vUpdateMap(void);
    void vRandomMap(i8 i);
    void vSetTableNm(Stz* szTableNm);
  
  private:
    Footer* aFooter(void);
  };
  
  BucketMap_dyn* _pBucketMap_dyn(void* aAddr);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
