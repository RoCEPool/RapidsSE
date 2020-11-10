/*============================================================================*/
/* BucketMap.cpp -- Bucket (to logical partition) Map                         */
/*============================================================================*/

#include "Partitioning.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Partitioning,BucketMap,CN_BUCKETMAP)
__aNewFunction(BucketMap,CN_BUCKETMAP)
__vDeleteFunction(BucketMap,CN_BUCKETMAP)
__aFooterFunction(BucketMap);

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BucketMap::BucketMap(i8 nPartitions)
: Object(CN_BUCKETMAP)
, m_okLock(LK_PARTITION,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  
  RASSERT(nPartitions);
  
  // assign buckets to logical partitions in a round-robin fashion
  tPN tPtnNo = 0;
  for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS; i_Bkt++) {
    m_taInLglPtn[i_Bkt] = tPtnNo;
    tPtnNo = (++tPtnNo) % nPartitions;
  }
 
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get which logical partition a bucket is currently mapped to                */
/*----------------------------------------------------------------------------*/
tPN BucketMap::tWhichLglPtnNo(i8 i_Bkt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  
  tPN tPtnNo = m_taInLglPtn[i_Bkt];
  
  return tPtnNo;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
