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
#include "BucketMap_dyn.hpp"
#include "Bucket.hpp"
#include "TablePtn.hpp"
#include "PtnStream.hpp"
#include "Table.hpp"
#include "Steps.hpp"
#include "BucketGet.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

class IndexTree;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_BucketGet(void)
{
  vAddStep("BucketGet::getBucketData"_Stz,&Step::vBucketGetGetBucketData);
  vAddStep("BucketGet::getBucketOfTable"_Stz,&Step::vBucketGetGetBucketTable);
  vAddStep("BucketGet::getBucket"_Stz,&Step::vBucketGetGetBucket);
  vAddStep("BucketGet.getBucketNumber"_Stz,&Step::vBucketGetGetBucketNumber);
  vAddStep("BucketGet.getTablePtn"_Stz,&Step::vBucketGetGetTablePtn);
  vAddStep("Bucket[]"_Stz,                  &Step::vBucketGet);
  vAddStep("Bucket[]\\clear"_Stz,            &Step::vBucketGetAndClear);
  vAddStep("Bucket[].set"_Stz,               &Step::vBucketSet);
  vAddStep("Bucket[].getBucketCrw"_Stz,                  &Step::vBucketGetCrw);
}

/*----------------------------------------------------------------------------*/
/* Bucket[] -- get bucket                                                     */
/*----------------------------------------------------------------------------*/
void Step::vBucketGet(void)
{
  ENTRY();

  vTrace("C++ gets called: vBucketGet\n"_Stz);
  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpBucket);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Bucket[].getAndClear -- fetch planOperand and set to nullptr                 */
/*----------------------------------------------------------------------------*/
void Step::vBucketGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpBucket);

  m_p_TaskLogicUnit->m_xpBucket = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Bucket[].set -- set planOperand                                              */
/*----------------------------------------------------------------------------*/
void Step::vBucketSet(void)
{
  ENTRY();

  vTrace("C++ gets called: vBucketSet\n"_Stz);
  Bucket* pBucket = _pBucket(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pBucket || m_p_TaskLogicUnit->m_xpBucket.bIsNull());

  m_p_TaskLogicUnit->m_xpBucket = pBucket;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Bucket[].getBucketCrw -- fetch given bucket crw                            */
/*----------------------------------------------------------------------------*/
void Step::vBucketGetCrw(void)
{
  ENTRY();

  vTrace("C++ gets called: vBucketGetCrw\n"_Stz);

  m_p_TaskLogicUnit->m_xpBucket;


  // messing around to debug some weird problem
  //Bucket* pBucket = _pBucket(*m_p_TaskLogicUnit->m_xpBucket);
  BucketGet *pBucket = _pBucketGet(m_pDataStack->pPopPtr(__func__));

  //BucketGet *pBucket = _pBucketGet(m_pDataStack->pPopPtr(__func__));

  //PtnId tPtnId;
  //tPtnId = m_pDataStack->iPopInt(__func__);
  //i8 i_TablePtn = pTable->iPtnNoToTablePtn(tPtnId);
  //TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
  //m_pDataStack->vPushPtr(pTablePtn);

  if (pBucket) {
    //vTrace("vBucketGetCrw %s\n"_Stz, pBucket->pTable()->szTableNm()->azChar());
    vTrace("vBucketGetCrw %s\n"_Stz, pBucket->sTableName().azChar());
  }


  //CrwBuffer* pCrwBuffer = pBucket->pRootCrwBuffer();
  CrwBuffer* pCrwBuffer = pBucket->pTablePtn()->pGetBucket(pBucket->iBucketNo())->pRootCrwBuffer();
  m_pDataStack->vPushPtr(pCrwBuffer);



  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BucketGet::getBucketData -- start get parts of bucket-object               */
/*----------------------------------------------------------------------------*/
void Step::vBucketGetGetBucketData(void)
{
  ENTRY();

  // ... code which initiates the bucket transfer ...
  vTrace("C++ gets called: vBucketGetGetBucketdata\n"_Stz);
  i8 iBucket = m_pDataStack->iPopInt(__func__);

  BucketGet *pBucketGet = new BucketGet(iBucket);

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
  vTrace("C++ gets called: vBucketGetGetBucketTable\n"_Stz);


  //i8 iBucket = m_pDataStack->iPopInt(__func__);
  s32 sBktNo;
  m_pDataStack->vPopStr(sBktNo, __func__);
  i8 iBucket = sBktNo.iToI8();
  vTrace("C++ gets called: vBucketGetGetBucketTable bucket:%lld\n"_Stz, iBucket);

  s32 sTableNm;
  m_pDataStack->vPopStr(sTableNm, __func__);

  Table* pTable = AM::pTableFind(sTableNm);

  //Bucket *pBucket = nullptr;
  if (pTable) {
    vTrace("C++ gets called: vBucketGetGetBucketTable Found the table:%s\n"_Stz, sTableNm.azChar());

    // Here we need to map the bucket to the partition
    PtnId tPtnId =  pTable->tBktToLglPtn(iBucket);
    TablePtn *pTablePtn = pTable->pTablePtn(tPtnId.uPtnNo());

    //pBucket = pTablePtn->pGetBucket(0);

    // We have to tell tablePtn which bucket to use
    pTablePtn->vSetBucket(iBucket);
    //m_pDataStack->vPushPtr(pTablePtn);

    BucketGet *pBucketGet = new BucketGet(pTablePtn);

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
  vTrace("C++ gets called: vBucketGetGetBucket\n"_Stz);


  //i8 iBucket = m_pDataStack->iPopInt(__func__);
  s32 sBktNo;
  m_pDataStack->vPopStr(sBktNo, __func__);
  i8 iBucket = sBktNo.iToI8();
  vTrace("C++ gets called: vBucketGetGetBucket bucket:%lld\n"_Stz, iBucket);

  s32 sTableNm;
  m_pDataStack->vPopStr(sTableNm, __func__);

  Table* pTable = AM::pTableFind(sTableNm);

  Bucket *pBucket = nullptr;
  if (pTable) {
    vTrace("C++ gets called: vBucketGetGetBucket Found the table:%s\n"_Stz, sTableNm.azChar());

    // Here we need to map the bucket to the partition
    //pTable->pBucketMap()->vSee();
    PtnId tPtnId =  pTable->tBktToLglPtn(iBucket);
    TablePtn *pTablePtn = pTable->pTablePtn(tPtnId.uPtnNo());


    pBucket = pTablePtn->pGetBucket(iBucket);
    //m_pDataStack->vPushPtr(pBucket);


    // We have to tell tablePtn which bucket to use
    pTablePtn->vSetBucket(iBucket);
    //m_pDataStack->vPushPtr(pTablePtn);

    BucketGet *pBucketGet = new BucketGet(pTablePtn, pBucket);

    // Update bucket map here
    BucketMap_dyn* pBucketMap = pTable->pBucketMap();
    pBucketMap->vUpdateMap();

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

