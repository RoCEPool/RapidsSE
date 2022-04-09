/*============================================================================*/
/* Steps_BucketGet.cpp -- metaGet step group                                    */
/*============================================================================*/


#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "BucketMap.hpp"
#include "Bucket.hpp"
#include "TablePtn.hpp"
#include "PtnStream.hpp"
#include "Table.hpp"
#include "Steps.hpp"
#include "BucketGet.hpp"
//#include "BucketInfo.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

class IndexTree;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_BucketGet(void)
{
  vAddStep("BucketGet::new"_Stz,          &Step::vBucketGetNew);
  vAddStep("BucketGet[]"_Stz,                  &Step::vBucketGet);
  vAddStep("BucketGet[]\\clear"_Stz,            &Step::vBucketGetAndClear);
  vAddStep("BucketGet[].delete"_Stz,            &Step::vBucketDelete);
  vAddStep("BucketGet[].isNotNull"_Stz,         &Step::vBucketIsNotNull);
  vAddStep("BucketGet[].isNull"_Stz,            &Step::vBucketIsNull);
  vAddStep("BucketGet[].set"_Stz,               &Step::vBucketSet);

  vAddStep("BucketGet[].getBucketCrw"_Stz,                  &Step::vBucketGetCrw);

  vAddStep("BucketGet::getBucketData"_Stz,&Step::vBucketGetGetBucketData);
  vAddStep("BucketGet::getBucketOfTable"_Stz,&Step::vBucketGetGetBucketTable);
  vAddStep("BucketGet::getBucket"_Stz,&Step::vBucketGetGetBucket);
  vAddStep("BucketGet.getBucketNumber"_Stz,&Step::vBucketGetGetBucketNumber);
  vAddStep("BucketGet.getTablePtn"_Stz,&Step::vBucketGetGetTablePtn);

}

/*----------------------------------------------------------------------------*/
/* BucketGet::new -- new BucketGet with table name                       */
/*----------------------------------------------------------------------------*/
void Step::vBucketGetNew(void)
{
  ENTRY();

  s32 sBktNo;
  m_pDataStack->vPopStr(sBktNo, __func__);
  i8 iBucket = sBktNo.iToI8();
  //vTrace("C++ gets called: BucketGet::new  bucket:%lld\n"_Stz, iBucket);

  s32 sTableNm;
  m_pDataStack->vPopStr(sTableNm, __func__);

  Table* pTable = AM::pTableFind(sTableNm);

  if (pTable) {
    //vTrace("C++ gets called: BucketGet::new  Found the table:%s\n"_Stz, sTableNm.azChar());

    BucketGet *pBucketGet = new BucketGet(sTableNm,iBucket);

    m_pDataStack->vPushPtr(pBucketGet);

  } else  {
    m_pDataStack->vPushPtr(nullptr);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BucketGet[] -- get bucket                                                     */
/*----------------------------------------------------------------------------*/
void Step::vBucketGet(void)
{
  ENTRY();

  //vTrace("C++ gets called: BucketGet[] vBucketGet\n"_Stz);
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpBucketGet);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BucketGet[].getAndClear -- fetch planOperand and set to nullptr                 */
/*----------------------------------------------------------------------------*/
void Step::vBucketGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpBucketGet);

  m_pLogicUnit->m_xpBucket = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BucketGet[].set -- set planOperand                                              */
/*----------------------------------------------------------------------------*/
void Step::vBucketSet(void)
{
  ENTRY();

  //vTrace("C++ gets called: vBucketSet\n"_Stz);
  BucketGet* pBucketGet = _pBucketGet(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pBucketGet || m_pLogicUnit->m_xpBucketGet.bIsNull());

  m_pLogicUnit->m_xpBucketGet = pBucketGet;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BucketGet[].delete -- planOperand delete                                      */
/*----------------------------------------------------------------------------*/
void Step::vBucketDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_pLogicUnit->m_xpBucketGet);

  m_pLogicUnit->m_xpBucketGet = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BucketGet[].isNotNull -- is planOperand not null ?                            */
/*----------------------------------------------------------------------------*/
void Step::vBucketIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpBucketGet.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BucketGet[].isNull -- is planOperand null ?                                   */
/*----------------------------------------------------------------------------*/
void Step::vBucketIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpBucketGet.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BucketGet[].getBucketCrw -- fetch given bucket crw                            */
/*----------------------------------------------------------------------------*/
void Step::vBucketGetCrw(void)
{
  ENTRY();

  //vTrace("C++ gets called: vBucketGetCrw\n"_Stz);

  BucketGet *pBucketGet = _pBucketGet(*m_pLogicUnit->m_xpBucketGet);

  //if (pBucketGet) {
  //  vTrace("BucketGet[].getBucketCrw %s bkt:%lld \n"_Stz, pBucketGet->sTableName().azChar(), pBucketGet->iBucketNo());
  //}

  Table* pTable = AM::pTableFind(pBucketGet->sTableName());

  //if (pTable) {
  //  vTrace("C++ gets called: BucketGet[].getBucketCrw  Found the table:%s\n"_Stz, pBucketGet->sTableName().azChar());
  //}
  // Here we need to map the bucket to the partition
  LglPtn tLglPtn =  pTable->tLglPtnOf(pBucketGet->iBucketNo());
  //vTrace(">>>  vBucketGetCrw bkt:%lld ptn:%lld\n"_Stz, pBucketGet->iBucketNo(), tLglPtn.uPtnNo());

  TablePtn *pTablePtn = pTable->pTablePtn(tLglPtn.iAsI8());

  pTablePtn->vSetBucket(tLglPtn.iAsI8()); //not needed

  Bucket* pBucket = pTablePtn->pGetBucket(pBucketGet->iBucketNo());

  CrwBuffer* pCrwBuffer = pBucket->pRootCrwBuffer();

  // Experimenting... if we can not find the rows in the bucket, search all buckets for this partition
  if (!pCrwBuffer) {

      // DEBUG search all the buckets
      for (i8 i_Bkt = 0; i_Bkt < C_MAX_KEYBUCKETS; i_Bkt++) {
        //vTrace("    vBucketGetCrw bkt:%lld ptn:%lld\n"_Stz, iBkt, tLglPtn.uPtnNo());
        pBucket = pTablePtn->pGetBucket(i_Bkt);
        pCrwBuffer = pBucket->pRootCrwBuffer();
        if (pCrwBuffer) {
          vTrace("vBucketGetCrw got data parcel bkt:%lld ptn:%lld\n"_Stz, i_Bkt, tLglPtn.iAsI8());
          break;
        }
    }
    // if we still can not find rows in this partition, search all partitions
    if (!pCrwBuffer) {
      for (i8 i_Ptn = 0; i_Ptn < AM::PTNMAP::nPtnsPerNode(); i_Ptn++) {
        pTablePtn = pTable->pTablePtn(i_Ptn);
        // DEBUG search all the buckets
        for (i8 i_Bkt = 0; i_Bkt < C_MAX_KEYBUCKETS; i_Bkt++) {
          //vTrace("    vBucketGetCrw bkt:%lld ptn:%lld\n"_Stz, i_Bkt, tLglPtn.uPtnNo());
          pBucket = pTablePtn->pGetBucket(i_Bkt);
          pCrwBuffer = pBucket->pRootCrwBuffer();
          if (pCrwBuffer) {
            vTrace("vBucketGetCrw got data parcel bkt:%lld ptn:%lld (different ptn than originally looked for)\n"_Stz, i_Bkt, tLglPtn.iAsI8());
            break;
          }
        }
        if (pCrwBuffer) break;

      }
    }
  }

  Parcel *pParcel = new Parcel(true);


  BucketInfo* pBucketInfo = new BucketInfo(pBucketGet->sTableName(), tLglPtn, pBucketGet->iBucketNo());
  //pBucketInfo->vCrwBuffer(pCrwBuffer); leave data buffer separately
  pBucketInfo->vNodeName(AM::szNodeNm());

  pParcel->vAddItem(pBucketInfo);

  pParcel->vAddItem(pCrwBuffer);

  if (pCrwBuffer) {
    vTrace("vBucketGetCrw bkt:%lld sent the parcel with data\n"_Stz, pBucketGet->iBucketNo());
  } else {
    vTrace("vBucketGetCrw bkt:%lld sent the parcel without data\n"_Stz, pBucketGet->iBucketNo());
  }

  m_pDataStack->vPushPtr(pParcel);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BucketGet::getBucketData -- start get parts of bucket-object               */
/*----------------------------------------------------------------------------*/
void Step::vBucketGetGetBucketData(void)
{
  ENTRY();

  // ... code which initiates the bucket transfer ...
  vTrace("C++ gets called: BucketGet::getBucketData\n"_Stz);
  i8 iBucket = m_pDataStack->iPopInt(__func__);

  BucketGet *pBucketGet = new BucketGet(""_Stz,iBucket);

  m_pDataStack->vPushPtr(pBucketGet);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BucketGet.getTablePtn -- start get parts of bucket-object               */
/*----------------------------------------------------------------------------*/
void Step::vBucketGetGetTablePtn(void)
{
  ENTRY();

  // ... code which initiates the bucket transfer ...
  vTrace("C++ gets called: vBucketGetGetTablePtn\n"_Stz);
  BucketGet* pBucketGet = _pBucketGet(m_pDataStack->pPopPtr(__func__));

  i8 iBucket = m_pDataStack->iPopInt(__func__);

  TablePtn* pTablePtn = pBucketGet->pTablePtn();

  m_pDataStack->vPushPtr(pTablePtn);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BucketGet::getBucketOfTable -- start get parts of bucket-object               */
/*----------------------------------------------------------------------------*/
void Step::vBucketGetGetBucketTable(void)
{
  ENTRY();

  // ... code which initiates the bucket transfer ...
  //vTrace("C++ gets called: BucketGet::getBucketOfTable\n"_Stz);


  //i8 iBucket = m_pDataStack->iPopInt(__func__);
  s32 sBktNo;
  m_pDataStack->vPopStr(sBktNo, __func__);
  i8 iBucket = sBktNo.iToI8();
  vTrace("C++ gets called: BucketGet::getBucketOfTable bucket:%lld\n"_Stz, iBucket);

  s32 sTableNm;
  m_pDataStack->vPopStr(sTableNm, __func__);

  Table* pTable = AM::pTableFind(sTableNm);

  //Bucket *pBucket = nullptr;
  if (pTable) {
    vTrace("C++ gets called: BucketGet::getBucketOfTable Found the table:%s\n"_Stz, sTableNm.azChar());

    // Here we need to map the bucket to the partition
    LglPtn tLglPtn =  pTable->tLglPtnOf(iBucket);
    TablePtn *pTablePtn = pTable->pTablePtn(tLglPtn.iAsI8());

    //pBucket = pTablePtn->pGetBucket(0);

    // We have to tell tablePtn which bucket to use
    pTablePtn->vSetBucket(iBucket);
    //m_pDataStack->vPushPtr(pTablePtn);

    BucketGet *pBucketGet = new BucketGet(pTablePtn);

    vTrace("C++ gets called: BucketGet::getBucketOfTable Found the table:%s\n"_Stz, sTableNm.azChar());
    m_pDataStack->vPushPtr(pBucketGet);

  } else  {
    m_pDataStack->vPushPtr(nullptr);
  }



  EXIT();
}


/*----------------------------------------------------------------------------*/
/* BucketGet::getBucket -- start get parts of bucket-object               */
/*----------------------------------------------------------------------------*/
void Step::vBucketGetGetBucket(void)
{
  ENTRY();

  // ... code which initiates the bucket transfer ...
  vTrace("C++ gets called: BucketGet::getBucket\n"_Stz);


  //i8 iBucket = m_pDataStack->iPopInt(__func__);
  s32 sBktNo;
  m_pDataStack->vPopStr(sBktNo, __func__);
  i8 iBucket = sBktNo.iToI8();
  vTrace("C++ gets called: BucketGet::getBucket bucket:%lld\n"_Stz, iBucket);

  s32 sTableNm;
  m_pDataStack->vPopStr(sTableNm, __func__);

  Table* pTable = AM::pTableFind(sTableNm);

  Bucket *pBucket = nullptr;
  if (pTable) {
    // Here we need to map the bucket to the partition
    //pTable->pBucketMap()->vSee();
    LglPtn tLglPtn =  pTable->tLglPtnOf(iBucket);
    TablePtn *pTablePtn = pTable->pTablePtn(tLglPtn.iAsI8());

    vTrace("C++ gets called: BucketGet::getBucket Found the table:%s ptn:%lld for bucket:%lld\n"_Stz, sTableNm.azChar(), tLglPtn.iAsI8(), iBucket);

    pBucket = pTablePtn->pGetBucket(iBucket);
    //m_pDataStack->vPushPtr(pBucket);


    // We have to tell tablePtn which bucket to use
    pTablePtn->vSetBucket(iBucket);
    //m_pDataStack->vPushPtr(pTablePtn);

    // Update bucket map here
    BucketMap* pBucketMap = pTable->pBucketMap();
    pBucketMap->vUpdateMap();

    BucketGet *pBucketGet = new BucketGet(pTablePtn, pBucket);

    m_pDataStack->vPushPtr(pBucketGet);

  } else  {
    m_pDataStack->vPushPtr(nullptr);
  }



  EXIT();
}

/*--------------------------------------------------------------------------------*/
/* BucketGet.getBucketNumber           -- get bucket number from BucketGet object */
/*--------------------------------------------------------------------------------*/
void Step::vBucketGetGetBucketNumber(void)
{
  ENTRY();
  vTrace("C++ gets called: vBucketGetGetBucketNumber\n"_Stz);

  BucketGet* pBucketGet = _pBucketGet(m_pDataStack->pPopPtr(__func__));

  i8 iBucket = pBucketGet->iBucketNo();

  m_pDataStack->vPushInt(iBucket);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

