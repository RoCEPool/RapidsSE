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

  i8 nPartitions = AM::PTNMAP::nPtnsInGrid();

  m_iActive = 0;
  m_paPtnId = m_taPtnId_0;
  m_eKind   = eKind;

  // assign buckets to logical partitions in a round-robin fashion
  
  PtnId tPtnId = 0;

  for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS_IN_GRID; i_Bkt++) {

    tPtnId = i_Bkt % nPartitions;
    
    m_taPtnId_0[i_Bkt] = tPtnId;
    //vTrace("Internal table i_Bkt:%lld tPtnId:%lld\n"_Stz, i_Bkt, tPtnId.iAsI8());
  }

//  if (TT_NORMAL == eKind)
//  {
//
//    vRandomMap(nPartitions);
//
//  }
//  else {
//    for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS_IN_GRID; i_Bkt++) {
//      m_taPtnId_0[i_Bkt] = tPtnId;
//      iPtn = tPtnId.iAsI8();
//      tPtnId = (++iPtn) % nPartitions;
//      //vTrace("Internal table i_Bkt:%lld tPtnId:%lld\n"_Stz, i_Bkt, tPtnId.iAsI8());
//    }
//  }
 
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
  UNUSED(pBucketMap);
  //pBucketMap->vResetPointers();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location                                                               */
/*----------------------------------------------------------------------------*/
//Location* BucketMap_dyn::pLocation(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);
//
//  return & m_oLocation;
//}

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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);
  i8 iBkt=-1;
  for (i8 i_Bkt=0; i_Bkt < C_MAX_BUCKETS_IN_GRID; i_Bkt++) {
    vTrace("   i_Bkt:%lld tPtnId:%lld\n"_Stz, i_Bkt, m_paPtnId[i_Bkt].iAsI8());
    //if (m_paPtnId[i_Bkt].iAsI8() == tPtnId.iAsI8()) {
      if (i_Bkt > iBktNo) {
        iBkt = i_Bkt;
        break;
      }
    //}
  }

  return iBkt;
}

i8 BucketMap_dyn::iPtnIdToBkt(PtnId tPtnId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);

  for (i8 i_Bkt=0; i_Bkt < C_MAX_BUCKETS_IN_GRID; i_Bkt++) {
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


void BucketMap_dyn::vSetTableNm(Stz* szTableNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);
  m_sTableNm = szTableNm;

  /*
  if (m_eKind == TT_NORMAL) {
    vTrace("Table %s \n"_Stz, m_sTableNm.azChar());
    for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS_IN_GRID; i_Bkt++) {

      vTrace("   i_Bkt:%lld tPtnId:%lld\n"_Stz, i_Bkt, m_taPtnId_0[i_Bkt].iAsI8());
    }
    vTrace("======================\n"_Stz);
  }*/
}

/*----------------------------------------------------------------------------*/
/* see the bucket map                                                         */
/*----------------------------------------------------------------------------*/
void BucketMap_dyn::vSee(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);
  
  vTrace("BktMap::\n"_Stz);

  PtnId tPtnId;
  
  vTrace("  m_iActive  %2lld\n"_Stz,m_iActive);
  for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS_IN_GRID; i_Bkt++) {
    
    if (0 == m_iActive)
      tPtnId = m_taPtnId_0[i_Bkt];
    else
      tPtnId = m_taPtnId_1[i_Bkt];

    vTrace("  m_taPtnId_%lld[i_Bkt %02lld] { tPtnId=%lld }\n"_Stz
           ,m_iActive
           ,i_Bkt
           ,(i8)tPtnId.uPtnNo()
           );
  }
  
  vTrace(";;\n"_Stz);

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
/* Function to simulate random mapping update                                 */
/*----------------------------------------------------------------------------*/
#if 1
void BucketMap_dyn::vRandomMap (i8 i)
{
  i8 nPartitions = AM::PTNMAP::nPtnsInGrid();
  PtnId tPtnId;
  i8 iPtn=0;
  for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS_IN_GRID; i_Bkt++) {
    m_taPtnId_0[i_Bkt] = tPtnId;
    iPtn = tPtnId.iAsI8();
    tPtnId = (++iPtn) % nPartitions;
    //vTrace("Internal table i_Bkt:%lld tPtnId:%lld\n"_Stz, i_Bkt, tPtnId.iAsI8());
  }
}

/*----------------------------------------------------------------------------*/
#else
void BucketMap_dyn::vRandomMap (i8 i)
{
  if (i== 7) {
    // 1 to 1 mapping
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
  } else   if (i == 6) { // random
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
    m_taPtnId_0[15] = 4;
  } else   if (i == 8) { // synthetic
    // Synthetic mapping, assign buckets to logical partitions
    /*
    PtnId tPtnId;
    i8 i_Bkt = 0;

    while (i_Bkt < C_MAX_BUCKETS_IN_GRID)
    {
      m_taPtnId_0[i_Bkt] = tPtnId;
      m_taPtnId_0[++i_Bkt] = tPtnId;

      i_Bkt++;
      ++tPtnId;

    } */

    i8 nPartitions = AM::PTNMAP::nPtnsInGrid();
    PtnId tPtnId;
    i8 iPtn=0;
    for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS_IN_GRID; i_Bkt++) {
      m_taPtnId_0[i_Bkt] = tPtnId;
      iPtn = tPtnId.iAsI8();
      tPtnId = (++iPtn) % nPartitions;
      //vTrace("Internal table i_Bkt:%lld tPtnId:%lld\n"_Stz, i_Bkt, tPtnId.iAsI8());
    }


  } else   if (i==1 || i == 4) {
    i8 nPartitions = AM::PTNMAP::nPtnsInGrid();
    PtnId tPtnId;
    i8 iPtn=0;
    for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS_IN_GRID; i_Bkt++) {
      m_taPtnId_0[i_Bkt] = tPtnId;
      iPtn = tPtnId.iAsI8();
      tPtnId = (++iPtn) % nPartitions;
      //vTrace("Internal table i_Bkt:%lld tPtnId:%lld\n"_Stz, i_Bkt, tPtnId.iAsI8());
    }
  }

  /*
  for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS_IN_GRID; i_Bkt++) {
    vTrace("   i_Bkt:%lld tPtnId:%lld\n"_Stz, i_Bkt, m_taPtnId_0[i_Bkt].iAsI8());
  }*/
  */
  
}

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
