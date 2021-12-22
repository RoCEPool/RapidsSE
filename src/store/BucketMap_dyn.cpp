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
  m_eKind   = eKind;

  // assign buckets to logical partitions in a round-robin fashion
  i8 nCurrentPartitions = AM::PTNMAP::nPtnsInGrid();
  vAdjustMap(nCurrentPartitions);
  vAdjustSecondMap(nCurrentPartitions);
 
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
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* adjust map for given number of partitions                                  */
/*----------------------------------------------------------------------------*/
void BucketMap_dyn::vAdjustMap(i8 nCurrentPartitions)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);
  
  for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS_IN_GRID; i_Bkt++) {
    
    PtnId tPtnId = i_Bkt % nCurrentPartitions;
    
    m_taPtnId_0[i_Bkt] = tPtnId;
  }
}

/*----------------------------------------------------------------------------*/
/* adjust map for given number of partitions for the second map               */
/*----------------------------------------------------------------------------*/
void BucketMap_dyn::vAdjustSecondMap(i8 nCurrentPartitions)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);

  for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS_IN_GRID; i_Bkt++) {

    PtnId tPtnId = i_Bkt % nCurrentPartitions;

    m_taPtnId_1[i_Bkt] = tPtnId;
  }
}

/*----------------------------------------------------------------------------*/
/* get which logical partition a bucket is currently mapped to                */
/*----------------------------------------------------------------------------*/
PtnId BucketMap_dyn::tBktToLglPtnId(i8 iBucketId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);

  PtnId tPtnId;
  if (0 == m_iActive)
    tPtnId = m_taPtnId_0[iBucketId];
  else
    tPtnId = m_taPtnId_1[iBucketId];
  
  return tPtnId;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 BucketMap_dyn::iPtnIdToBkt(PtnId tPtnId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);

  for (i8 i_Bkt=0; i_Bkt < C_MAX_BUCKETS_IN_GRID; i_Bkt++) {
    if (m_paPtnId[i_Bkt].iAsI8() == tPtnId.iAsI8())
      return i_Bkt;
  }

  return -1;
}

/*----------------------------------------------------------------------------*/
/* Function to simulate random mapping update                                 */
/*----------------------------------------------------------------------------*/
void BucketMap_dyn::vRandomMap(i8 i)
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void BucketMap_dyn::vSetTableNm(Stz* szTableNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);
  m_sTableNm = szTableNm;
}


Stz* BucketMap_dyn::szDumpMap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);
  s4K sDumpBuffer = ""_Stz;

  for (i8 i_Bkt = 0; i_Bkt < C_MAX_BUCKETS_IN_GRID; i_Bkt++) {
    sDumpBuffer.vCatSprint("   i_Bkt:%lld tPtnId:%lld\n"_Stz, i_Bkt, m_taPtnId_0[i_Bkt].iAsI8());
  }
  sDumpBuffer.vConcat("======================\n"_Stz);
  return sDumpBuffer;
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
/* Function to simulate swapping                                              */
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
/* Function to change map after map updated                                   */
/*----------------------------------------------------------------------------*/
/*
void BucketMap_dyn::vSimulateUpdateMap (void)
{
  PtnId tSave = m_taPtnId_0[0];

  m_taPtnId_0[0] = m_taPtnId_0[1];

  m_taPtnId_0[1] = tSave;

}*/



/*----------------------------------------------------------------------------*/
/* Function to update map after send the data to new node                     */
/*----------------------------------------------------------------------------*/
void BucketMap_dyn::vUpdateMap (void)
{
  vAdjustMap(AM::PTNMAP::nPtnsInGrid());
}

/*----------------------------------------------------------------------------*/
/* Function to simulate random mapping update                                 */
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

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 BucketMap_dyn::tWhichLglPtnId(i8 iBucketId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP_dyn,_FOOTER_);
  return tBktToLglPtnId(iBucketId).iAsI8();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
