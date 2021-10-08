
#if 1



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
BucketMap_dyn::BucketMap_dyn(eTT eKind)
: Operand(CN_BUCKETMAP_dyn)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);

  m_iActive = 0;
  m_paPtnId = m_taPtnId_0;


#if 0
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
  i8 iPtn=0;
  for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS; i_Bkt++) {
    m_taPtnId_0[i_Bkt] = tPtnId;
    iPtn = tPtnId.iAsI8();
    //tPtnId = (++iPtn) % nPartitions;
    //vTrace("i_Bkt:%lld tPtnId:%lld\n"_Stz, i_Bkt, tPtnId);
  }
#endif

  _checkInitialised_(CN_BUCKETMAP_dyn);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BucketMap_dyn::BucketMap_dyn(i8 nPartitions, eTT eKind)
: Operand(CN_BUCKETMAP_dyn)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);
  RASSERT(nPartitions);


  m_iActive = 0;
  m_paPtnId = m_taPtnId_0;

#if 0
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
  i8 iPtn=0;

  if (TT_NORMAL == eKind)
  {
    // Synthetic mapping, assign buckets to logical partitions
    tPtnId = 0;
    i8 i_Bkt = 0;

    while (i_Bkt < C_MAX_BUCKETS)
    {
      m_taPtnId_0[i_Bkt] = tPtnId;
      m_taPtnId_0[++i_Bkt] = tPtnId;

      i_Bkt++;
      tPtnId++;
      //tPtnId = (++tPtnId) % nPartitions;

    }

    for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS; i_Bkt++) {
      vTrace("%s  i_Bkt:%lld tPtnId:%lld\n"_Stz, aTT[eKind], i_Bkt, m_taPtnId_0[i_Bkt].iAsI8());
    }
  } else {
    for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS; i_Bkt++) {
      m_taPtnId_0[i_Bkt] = tPtnId;
      iPtn = tPtnId.iAsI8();
      tPtnId = (++iPtn) % nPartitions;
      //vTrace("Internal table i_Bkt:%lld tPtnId:%lld\n"_Stz, i_Bkt, tPtnId.iAsI8());
    }
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

  //pBucketMap->vResetPointers();
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
/*void BucketMap_dyn::vResetPointers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);

  // no pointers need to be reset
}*/

/*----------------------------------------------------------------------------*/
/* get which logical partition a bucket is currently mapped to                */
/*----------------------------------------------------------------------------*/

PtnId BucketMap_dyn::tBktToPtnId(i8 iBucketId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);
  
  PtnId tPtnId = m_paPtnId[iBucketId];
  
  return tPtnId;

}

/*----------------------------------------------------------------------------*/
/* get next bucket logical partition is currently mapped to                   */
/* We can have a partition mapped into multiple buckets                       */
/*----------------------------------------------------------------------------*/
i8 BucketMap_dyn::iWhichBucketNext(PtnId tPtnId, i8 iBktNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  i8 iBkt=-1;
  for (i8 i_Bkt=0; i_Bkt < C_MAX_BUCKETS; i_Bkt++) {
    if (m_paPtnId[i_Bkt].iAsI8() == tPtnId.iAsI8()) {
      if (i_Bkt > iBktNo) {
        iBkt = i_Bkt;
        break;
      }
    }
  }

  return iBkt;
}

i8 BucketMap_dyn::iPtnIdToBkt(PtnId tPtnId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);

  for (i8 i_Bkt=0; i_Bkt < C_MAX_BUCKETS; i_Bkt++) {
    if (m_paPtnId[i_Bkt].iAsI8() == tPtnId.iAsI8())
      return i_Bkt;
  }

  return -1;
}


i8 BucketMap_dyn::tWhichLglPtnId(i8 iBucketId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);
  return tBktToPtnId(iBucketId).iAsI8();
}

/*----------------------------------------------------------------------------*/
/* Function to simulate swapping                */
/*----------------------------------------------------------------------------*/
void BucketMap_dyn::vSwapMap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);
  if (m_iActive == 0) {
    m_iActive = 1;
    m_paPtnId = m_taPtnId_1;
  }
  else {
    m_iActive = 0;

    m_paPtnId = m_taPtnId_0;
  }
}

/*----------------------------------------------------------------------------*/
/* Function to simulate map update                */
/*----------------------------------------------------------------------------*/
void BucketMap_dyn::vUpdateMap (void)
{
  PtnId tSave = m_taPtnId_0[0];

  m_taPtnId_0[0] = m_taPtnId_0[1];

  m_taPtnId_0[1] = tSave;

}

/*----------------------------------------------------------------------------*/
/* Function to simulate random mapping update                */
/*----------------------------------------------------------------------------*/
void BucketMap_dyn::vRandomMap (i8 i)
{
  if (i== 1) {
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
  } else   if (i == 2) {
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
  } else {
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
  }

}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/



#endif

