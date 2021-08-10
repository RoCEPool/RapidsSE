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
#include "TablePtn.hpp"
#include "PtnStream.hpp"
#include "Table.hpp"
#include "Steps.hpp"
#include "BucketGet.hpp"

class IndexTree;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_BucketGet(void)
{
  vAddStep("BucketGet::getBucketData"_Stz,&Step::vBucketGetGetBucketData);
  vAddStep("BucketGet::getBucketOfTable"_Stz,&Step::vBucketGetGetBucketTable);
  vAddStep("BucketGet.getBucketNumber"_Stz,&Step::vBucketGetGetBucketNumber);
}

/*----------------------------------------------------------------------------*/
/* BucketGet::getBucketData -- start get parts of bucket-object               */
/*----------------------------------------------------------------------------*/
void Step::vBucketGetGetBucketData(void)
{
  ENTRY();

  // ... code which initiates the metadata transfer ...
  vTrace("C++ gets called: vBucketGetGetBucketdata\n"_Stz);
  i8 iBucket = m_pDataStack->iPopInt(__func__);

  // HACK


  BucketGet *pBucketGet = new BucketGet(iBucket);

  m_pDataStack->vPushPtr(pBucketGet);

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
  s32 sTableNm;
  m_pDataStack->vPopStr(sTableNm, __func__);

  Table* pTable = AM::pTableFind(sTableNm);

  // HACK: hardcode to buffer 0 for this table
  Bucket *pBucket = nullptr;
  if (pTable) {
    vTrace("C++ gets called: vBucketGetGetBucketTable Found the table:%s\n"_Stz, sTableNm.azChar());
    TablePtn *pTablePtn = pTable->pTablePtn(0);
    // Forget about bucket for now, just send the ptnstream
    pBucket = pTablePtn->pGetBucket(0);
    m_pDataStack->vPushPtr(pBucket);

    //BucketGet *pBucketGet = new BucketGet(iBucket);

    //PtnStream* pPtnStream = new PtnStream(pTablePtn);
    //m_pDataStack->vPushPtr(pPtnStream);
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

