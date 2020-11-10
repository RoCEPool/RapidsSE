/*============================================================================*/
/* BucketMap.cpp -- Bucket (to partition) Map                                 */
/*============================================================================*/

#include "Partitioning.h"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
BucketMap* Partitioning::_pBucketMap(void* aAddr)
{
  BucketMap* pBucketMap = reinterpret_cast<BucketMap*>(aAddr);
  
  RASSERT_TYPE(pBucketMap,CN_BUCKETMAP);
  
  return pBucketMap;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* BucketMap::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_BUCKETMAP);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void BucketMap::operator delete(void* pBufferAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pBufferAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BucketMap::BucketMap(i8 nPartitions)
: Object(CN_BUCKETMAP)
, m_okLock(LK_PARTITION,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  
  RASSERT(nPartitions);
  
  // assign buckets to partitions in a round-robin fashion
  tPN tPtnNo = 0;
  for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS; i_Bkt++) {
    m_taToWhichPtn[i_Bkt] = tPtnNo;
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
/* get which partition a bucket is currently mapped to                        */
/*----------------------------------------------------------------------------*/
tPN BucketMap::tWhichPtnNo(i8 i_Bkt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  
  tPN tPtnNo = m_taToWhichPtn[i_Bkt];
  
  return tPtnNo;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(BucketMap);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
