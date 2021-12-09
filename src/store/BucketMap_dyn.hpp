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
  
    //Location m_oLocation; // logical location of BucketMap_dyn
  
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
    
    PtnId tBktToLglPtnId(i8 iBucketId);
    i8    iPtnIdToBkt(PtnId tPtnId);

    void vAdjustMap(i8 nnPartitions);

    void vUpdateMap(void);
    void vRandomMap(i8 i);
    void vSee(void);
    void vSetTableNm(Stz* szTableNm);

    Stz* szDumpMap(void);

    void vSwapMap(void);

    i8 iWhichBucketNext(PtnId tPtnId, i8 iBktNo);
    i8 tWhichLglPtnId(i8 iBucketId);

  private:
    Footer* aFooter(void);
  };
  
  BucketMap_dyn* _pBucketMap_dyn(void* aAddr);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
