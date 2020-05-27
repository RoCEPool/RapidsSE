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
  
  if (nPartitions) {
    // assign buckets to partitions in a round-robin fashion
    i8 i_Ptn = 0;
    for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS; i_Bkt++) {
      m_iaInPartition[i_Bkt] = i_Ptn;
      i_Ptn = (++i_Ptn) % nPartitions;
    }
  }
  // ray : 28 May 2020 : added this to get define table running
  else {
    for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS; i_Bkt++)
      m_iaInPartition[i_Bkt] = 0;
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
i8 BucketMap::i_WhichPartition(i8 i_Bkt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  
  i8 i_Ptn = m_iaInPartition[i_Bkt];
  
  return i_Ptn;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(BucketMap);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
