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
BucketMap::BucketMap(eTT eKind)
: Operand(CN_BUCKETMAP)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);

  m_iActive = 0;
  m_paPtnId = m_taPtnId_0;
  m_eKind   = eKind;

  // assign buckets to logical partitions in a round-robin fashion
  i8 nCurrentPartitions = AM::PTNMAP::nPtnsInGrid();
  vAdjustMap(nCurrentPartitions);
  vAdjustSecondMap(nCurrentPartitions);
 
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
  UNUSED(pBucketMap);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* adjust map for given number of partitions                                  */
/*----------------------------------------------------------------------------*/
void BucketMap::vAdjustMap(i8 nCurrentPartitions)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  
  for (i8 i_Bkt = 0; i_Bkt < C_MAX_KEYBUCKETS; i_Bkt++) {
    
    LglPtn tLglPtn = i_Bkt % nCurrentPartitions;
    
    m_taPtnId_0[i_Bkt] = tLglPtn;
  }
}

/*----------------------------------------------------------------------------*/
/* adjust map for given number of partitions for the second map               */
/*----------------------------------------------------------------------------*/
void BucketMap::vAdjustSecondMap(i8 nCurrentPartitions)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);

  for (i8 i_Bkt = 0; i_Bkt < C_MAX_KEYBUCKETS; i_Bkt++) {

    LglPtn tLglPtn = i_Bkt % nCurrentPartitions;

    m_taPtnId_1[i_Bkt] = tLglPtn;
  }
}

/*----------------------------------------------------------------------------*/
/* get which logical partition a bucket is currently mapped to                */
/*----------------------------------------------------------------------------*/
LglPtn BucketMap::tLglPtnOf(KeyBkt tKeyBkt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);

  LglPtn tLglPtn;
  if (0 == m_iActive)
    tLglPtn = m_taPtnId_0[tKeyBkt];
  else
    tLglPtn = m_taPtnId_1[tKeyBkt];
  
  return tLglPtn;
}

/*----------------------------------------------------------------------------*/
/* Function to simulate random mapping update                                 */
/*----------------------------------------------------------------------------*/
void BucketMap::vRandomMap(i8 i)
{
  i8 nPartitions = AM::PTNMAP::nPtnsInGrid();
  LglPtn tLglPtn;
  i8 iPtn=0;
  for (i8 i_Bkt = 0; i_Bkt < C_MAX_KEYBUCKETS; i_Bkt++) {
    m_taPtnId_0[i_Bkt] = tLglPtn;
    iPtn = tLglPtn.iAsI8();
    tLglPtn = (++iPtn) % nPartitions;
    //vTrace("Internal table i_Bkt:%lld tLglPtn:%lld\n"_Stz, i_Bkt, tLglPtn.iAsI8());
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void BucketMap::vSetTableNm(Stz* szTableNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  m_sTableNm = szTableNm;
}


Stz* BucketMap::szDumpMap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  s4K sDumpBuffer = ""_Stz;

  for (i8 i_Bkt = 0; i_Bkt < C_MAX_KEYBUCKETS; i_Bkt++) {
    sDumpBuffer.vCatSprint("   i_Bkt:%lld tLglPtn:%lld\n"_Stz, i_Bkt, m_taPtnId_0[i_Bkt].iAsI8());
  }
  sDumpBuffer.vConcat("======================\n"_Stz);
  return sDumpBuffer;
}

/*----------------------------------------------------------------------------*/
/* see the bucket map                                                         */
/*----------------------------------------------------------------------------*/
void BucketMap::vSee(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  
  vTrace("BktMap::\n"_Stz);

  LglPtn tLglPtn;
  
  vTrace("  m_iActive  %2lld\n"_Stz,m_iActive);
  for (i8 i_Bkt = 0; i_Bkt < C_MAX_KEYBUCKETS; i_Bkt++) {
    
    if (0 == m_iActive)
      tLglPtn = m_taPtnId_0[i_Bkt];
    else
      tLglPtn = m_taPtnId_1[i_Bkt];

    vTrace("  m_taPtnId_%lld[i_Bkt %02lld] { tLglPtn=%lld }\n"_Stz
           ,m_iActive
           ,i_Bkt
           ,(i8)tLglPtn.iAsI8()
           );
  }
  
  vTrace(";;\n"_Stz);

}

/*----------------------------------------------------------------------------*/
/* Function to simulate swapping                                              */
/*----------------------------------------------------------------------------*/
void BucketMap::vSwapMap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
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
void BucketMap::vUpdateMap (void)
{
  vAdjustMap(AM::PTNMAP::nPtnsInGrid());
}

/*----------------------------------------------------------------------------*/
/* Function to simulate random mapping update                                 */
/* get next bucket logical partition is currently mapped to                   */
/* We can have a partition mapped into multiple buckets                       */
/*----------------------------------------------------------------------------*/
i8 BucketMap::iWhichBucketNext(LglPtn tLglPtn, i8 iBktNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  i8 iBkt=-1;
  for (i8 i_Bkt=0; i_Bkt < C_MAX_KEYBUCKETS; i_Bkt++) {
    vTrace("   i_Bkt:%lld tLglPtn:%lld\n"_Stz, i_Bkt, m_paPtnId[i_Bkt].iAsI8());
    //if (m_paPtnId[i_Bkt].iAsI8() == tLglPtn.iAsI8()) {
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
i8 BucketMap::tWhichLglPtnId(i8 iBucketId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKETMAP,_FOOTER_);
  return tLglPtnOf(iBucketId).iAsI8();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
