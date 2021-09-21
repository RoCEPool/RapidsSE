#if 0



/*============================================================================*/
/* BucketMap_dyn.cpp -- Dynamic Bucket (to logical partition) Map             */
/*============================================================================*/

#include "BASE.h"

#include "BucketMap_dyn.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,BucketMap_dyn,CN_BUCKETMAP_dyn)
__aNewFunction(BucketMap_dyn,CN_BUCKETMAP_dyn)
__vDeleteFunction(BucketMap_dyn,CN_BUCKETMAP_dyn)
__aFooterFunction(BucketMap_dyn);

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BucketMap_dyn::BucketMap_dyn(i8 nPartitions)
: Operand(CN_BUCKETMAP_dyn)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);

  RASSERT(nPartitions);

#ifdef WITH_BKTMAP_2N4P
  if (nPartitions == 8) {
    // Synthetic mapping, assign buckets to logical partitions
    tPN tPtnNo = 0;
    i8 i_Bkt = 0;

    while (i_Bkt < C_MAX_BUCKETS)
    {
      m_taInLglPtn[i_Bkt] = tPtnNo;
      m_taInLglPtn[++i_Bkt] = tPtnNo;

      i_Bkt++;
      tPtnNo++;
      //tPtnNo = (++tPtnNo) % nPartitions;

    }

    for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS; i_Bkt++) {
      vTrace("i_Bkt:%lld tPtnNo:%lld\n"_Stz, i_Bkt, m_taInLglPtn[i_Bkt]);
    }

  } else {
    // this to take care of internal tables ?
    tPN tPtnNo = 0;
    for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS; i_Bkt++) {
      m_taInLglPtn[i_Bkt] = tPtnNo;
      tPtnNo = (++tPtnNo) % nPartitions;
      //vTrace("i_Bkt:%lld tPtnNo:%lld\n"_Stz, i_Bkt, tPtnNo);
    }
  }


#else
// assign buckets to logical partitions in a round-robin fashion
tPN tPtnNo = 0;
  for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS; i_Bkt++) {
    m_taInLglPtn[i_Bkt] = tPtnNo;
    tPtnNo = (++tPtnNo) % nPartitions;
    //vTrace("i_Bkt:%lld tPtnNo:%lld\n"_Stz, i_Bkt, tPtnNo);
  }
#endif

  _checkInitialised_(CN_BUCKETMAP_dyn);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BucketMap_dyn::~BucketMap_dyn(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void BucketMap_dyn::vPostRecv(void* aBucketMap)
{
  _static_

  BucketMap_dyn* pBucketMap = _pBucketMap_dyn(aBucketMap);

  pBucketMap->vResetPointers();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location                                                               */
/*----------------------------------------------------------------------------*/
Location* BucketMap_dyn::pLocation(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);

  return & m_oLocation;
}

/*----------------------------------------------------------------------------*/
/* reset members which are pointers (after being received)                    */
/*----------------------------------------------------------------------------*/
void BucketMap_dyn::vResetPointers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);

  // no pointers need to be reset
}

/*----------------------------------------------------------------------------*/
/* get which logical partition a bucket is currently mapped to                */
/*----------------------------------------------------------------------------*/
tPN BucketMap_dyn::tWhichLglPtnNo(i8 iBucketId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);

  tPN tPtnNo = m_taInLglPtn[iBucketId];

  return tPtnNo;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/



#endif


