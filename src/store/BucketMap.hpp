/*============================================================================*/
/* BucketMap.hpp -- Bucket (to logical partition) Map                         */
/*============================================================================*/
#pragma once

#include "eTT.h"

#include "KeyBkt.hpp"

namespace STORE
{
  class BucketMap : public Operand

  {
  private:
    Lock m_okLock;
  
    //Location m_oLocation; // logical location of BucketMap_dyn
  
    LglPtn* m_paPtnId;
    i8     m_iActive;

    LglPtn m_taPtnId_0[C_MAX_KEYBUCKETS]; // KeyBkt -> PtnId
    LglPtn m_taPtnId_1[C_MAX_KEYBUCKETS]; // KeyBkt -> PtnId

    s32 m_sTableNm; // table name (leave for debug)
    eTT m_eKind;    // table kind (leave for debug)

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    BucketMap(eTT eKind);
   ~BucketMap(void) noexcept(false);
  
    static void vPostRecv(void* aBucketMap);

    LglPtn tBktToLglPtnId(KeyBkt tKeyBkt);
    i8    iPtnIdToBkt(LglPtn tLglPtn);

    void vAdjustMap(i8 nPartitions);
    void vAdjustSecondMap(i8 nCurrentPartitions);

    void vUpdateMap(void);
    void vRandomMap(i8 i);
    void vSee(void);
    void vSetTableNm(Stz* szTableNm);

    Stz* szDumpMap(void);

    void vSwapMap(void);

    i8 iWhichBucketNext(LglPtn tLglPtn, i8 iBktNo);
    i8 tWhichLglPtnId(i8 iBucketId);

  private:
    Footer* aFooter(void);
  };
  
  BucketMap* _pBucketMap(void* aAddr);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
