/*============================================================================*/
/* BucketMap.cpp -- Bucket (to logical partition) Map                         */
/*============================================================================*/

#include "BASE.h"

#include "BucketMap.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,BucketMap,CN_BUCKETMAP)
__aNewFunction(BucketMap,CN_BUCKETMAP)
__vDeleteFunction(BucketMap,CN_BUCKETMAP)
__aFooterFunction(BucketMap);

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BucketMap::BucketMap(i8 nPartitions)
: Operand(CN_BUCKETMAP)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  
  RASSERT(nPartitions);

#ifdef WITH_BKTMAP_2N4P
  if (nPartitions <= 8) {
    // Synthetic mapping, assign buckets to logical partitions

    PtnId tPtnId;

    i8 i_Bkt = 0;

    tPtnId = 0;
    while (i_Bkt < C_MAX_BUCKETS)
    {
      m_taPtnId_0[i_Bkt] = tPtnId;
      m_taPtnId_0[++i_Bkt] = tPtnId;

      i_Bkt++;
      tPtnId++;
      //tPtnId = (++tPtnId) % nPartitions;

    }

    // 1 to 1 mapping
    m_taPtnId_0[0] = 0;
    m_taPtnId_0[1] = 1;
    m_taPtnId_0[2] = 2;
    m_taPtnId_0[3] = 3;
    m_taPtnId_0[4] = 4;
    m_taPtnId_0[5] = 5;
    m_taPtnId_0[6] = 6;
    m_taPtnId_0[7] = 7;
    m_taPtnId_0[8] = 8;
    m_taPtnId_0[9] = 9;
    m_taPtnId_0[10] = 10;
    m_taPtnId_0[11] = 11;
    m_taPtnId_0[12] = 12;
    m_taPtnId_0[13] = 13;
    m_taPtnId_0[14] = 14;
    m_taPtnId_0[15] = 15;




   // Random 1
   /*
    m_taPtnId_0[0] = 0;
    m_taPtnId_0[1] = 1;
    m_taPtnId_0[2] = 2;
    m_taPtnId_0[3] = 3;
    m_taPtnId_0[4] = 4;
    m_taPtnId_0[5] = 5;
    m_taPtnId_0[6] = 6;
    m_taPtnId_0[7] = 7;
    m_taPtnId_0[8] = 0;
    m_taPtnId_0[9] = 1;
    m_taPtnId_0[10] = 2;
    m_taPtnId_0[11] = 3;
    m_taPtnId_0[12] = 4;
    m_taPtnId_0[13] = 5;
    m_taPtnId_0[14] = 6;
    m_taPtnId_0[15] = 7;
    */

   //Random 2
    /*
    m_taPtnId_0[0] = 0;
    m_taPtnId_0[1] = 7;
    m_taPtnId_0[2] = 2;
    m_taPtnId_0[3] = 3;
    m_taPtnId_0[4] = 1;
    m_taPtnId_0[5] = 5;
    m_taPtnId_0[6] = 4;
    m_taPtnId_0[7] = 7;
    m_taPtnId_0[8] = 0;
    m_taPtnId_0[9] = 1;
    m_taPtnId_0[10] = 2;
    m_taPtnId_0[11] = 3;
    m_taPtnId_0[12] = 6;
    m_taPtnId_0[13] = 5;
    m_taPtnId_0[14] = 6;
    m_taPtnId_0[15] = 7;
     */



    //for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS; i_Bkt++) {
      //vTrace("i_Bkt:%lld tPtnId:%lld\n"_Stz, i_Bkt, m_taPtnId_0[i_Bkt].iAsI8());
    //}

  } else {
    // this to take care of internal tables ?
    PtnId tPtnId;
    for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS; i_Bkt++) {
      m_taPtnId_0[i_Bkt] = tPtnId;
      //tPtnId = (++tPtnId) % nPartitions;

      //vTrace("i_Bkt:%lld tPtnId:%lld\n"_Stz, i_Bkt, tPtnId);
    }
  }


#else
  // assign buckets to logical partitions in a round-robin fashion
  PtnId tPtnId;
  tPtnId = 0;
  i8 iPtnNo = 0;
  for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS; i_Bkt++) {
    tPtnId = iPtnNo;
    m_taPtnId_0[i_Bkt] = tPtnId;
    iPtnNo = (++iPtnNo) % nPartitions;
    //vTrace("i_Bkt:%lld tPtnId:%lld\n"_Stz, i_Bkt, iPtnNo);
  }

#endif
 
  _checkInitialised_(CN_BUCKETMAP);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BucketMap::~BucketMap(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void BucketMap::vPostRecv(void* aBucketMap)
{
  _static_
  
  BucketMap* pBucketMap = _pBucketMap(aBucketMap);
  
  pBucketMap->vResetPointers();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location                                                               */
/*----------------------------------------------------------------------------*/
Location* BucketMap::pLocation(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  
  return & m_oLocation;
}

/*----------------------------------------------------------------------------*/
/* reset members which are pointers (after being received)                    */
/*----------------------------------------------------------------------------*/
void BucketMap::vResetPointers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  
  // no pointers need to be reset
}

/*----------------------------------------------------------------------------*/
/* get which logical partition a bucket is currently mapped to                */
/*----------------------------------------------------------------------------*/
PtnId BucketMap::tWhichLglPtnId(i8 iBucketId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  PtnId tPtnId;
  tPtnId = m_taPtnId_0[iBucketId];

  return tPtnId;

}
/*----------------------------------------------------------------------------*/
/* get which bucket logical partition is currently mapped to                  */
/*----------------------------------------------------------------------------*/
i8 BucketMap::iWhichBucket(PtnId tPtnId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  i8 iBkt=-1;
  for (i8 i_Bkt=0; i_Bkt < C_MAX_BUCKETS; i_Bkt++) {
    if (m_taPtnId_0[i_Bkt].iAsI8() == tPtnId.iAsI8()) {
      iBkt = i_Bkt;
      break;
    }
  }

  return iBkt;
}

/*----------------------------------------------------------------------------*/
/* get next bucket logical partition is currently mapped to                   */
/* We can have a partition mapped into multiple buckets                       */
/*----------------------------------------------------------------------------*/
i8 BucketMap::iWhichBucketNext(PtnId tPtnId, i8 iBktNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  i8 iBkt=-1;
  for (i8 i_Bkt=0; i_Bkt < C_MAX_BUCKETS; i_Bkt++) {
    if (m_taPtnId_0[i_Bkt].iAsI8() == tPtnId.iAsI8()) {
      if (i_Bkt != iBktNo) {
        iBkt = i_Bkt;
        break;
      }
    }
  }

  return iBkt;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
