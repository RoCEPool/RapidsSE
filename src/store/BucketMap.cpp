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
  if (nPartitions == 8) {
    // Synthetic mapping, assign buckets to logical partitions
    PtnId tPtnId = 0;
    i8 i_Bkt = 0;

    while (i_Bkt < C_MAX_BUCKETS)
    {
      m_taInLglPtn[i_Bkt] = tPtnId;
      m_taInLglPtn[++i_Bkt] = tPtnId;

      i_Bkt++;
      tPtnId++;
      //tPtnId = (++tPtnId) % nPartitions;

    }

    for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS; i_Bkt++) {
      vTrace("i_Bkt:%lld tPtnId:%lld\n"_Stz, i_Bkt, m_taInLglPtn[i_Bkt]);
    }

  } else {
    // this to take care of internal tables ?
    PtnId tPtnId = 0;
    for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS; i_Bkt++) {
      m_taInLglPtn[i_Bkt] = tPtnId;
      tPtnId = (++tPtnId) % nPartitions;
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
    m_taInLglPtn[i_Bkt] = tPtnId;
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
PtnId BucketMap::tWhichLglPtnNo(i8 iBucketId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  PtnId tPtnId;
  tPtnId = m_taInLglPtn[iBucketId];

  return tPtnId;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
