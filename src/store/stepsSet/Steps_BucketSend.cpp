/*============================================================================*/
/* Steps_BucketSend.cpp -- bucketSend step group                              */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "SkipRoots.hpp"
#include "CrwBuffer.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "TablePtn.hpp"
#include "PtnStm_cursor.hpp"
#include "PtnStream.hpp"
#include "Table.hpp"
#include "Bucket.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_BucketSend(void)
{
  vAddStep("BucketSend.getBucketCrw"_Stz,&Step::vBucketSendGetBucketCrw);
  //vAddStep("BucketSend.getBucketPtn"_Stz,&Step::vBucketSendGetBucketPtn);
  vAddStep("BucketSend.getBucketPtnStream"_Stz,&Step::vBucketSendGetBucketPtnStream);
}

/*----------------------------------------------------------------------------*/
/* BucketSend.getBucketCrw -- get parts of bucket-object                      */
/*----------------------------------------------------------------------------*/
void Step::vBucketSendGetBucketCrw(void)
{
  ENTRY();

  vTrace("C++ gets called: vBucketSendGetBucketCrw\n"_Stz);

  Bucket* pBucket = _pBucket(m_pDataStack->pPopPtr(__func__));
  CrwBuffer *pCrwBuffer = nullptr;

  if (pBucket) {
    pCrwBuffer = pBucket->pRootCrwBuffer();
    vTrace("C++ gets called: Found BucketCrw\n"_Stz);
  }

  m_pDataStack->vPushPtr(pCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BucketSend.getBucketPtn -- get parts of bucket-object                      */
/*----------------------------------------------------------------------------*/
/*
void Step::vBucketSendGetBucketPtn(void)
{
  ENTRY();
what class
  vTrace("C++ gets called: vBucketSendGetBucketPtn\n"_Stz);

  TablePtn* pTablePtn = _pTablePtn(m_pDataStack->pPopPtr(__func__));



  m_pDataStack->vPushPtr(pTablePtn);

  EXIT();
}*/

/*----------------------------------------------------------------------------*/
/* BucketSend.getBucketPtnStream -- get parts of bucket-object          */
/*----------------------------------------------------------------------------*/
void Step::vBucketSendGetBucketPtnStream(void)
{
  ENTRY();

  vTrace("C++ gets called: vBucketSendGetBucketPtnStream\n"_Stz);

  PtnStream* pPtnStream = _pPtnStream(m_pDataStack->pPopPtr(__func__));

  DataBuffer* pDataBuffer = pPtnStream->pRead();

  ASSERT(! pDataBuffer || ! pDataBuffer->pNextItem());
  ASSERT(! pDataBuffer || ! pDataBuffer->pPrevItem());

  vTrace("C++ gets called: vBucketSendGetBucketCrw Got buffer\n"_Stz);
  m_pDataStack->vPushPtr(pDataBuffer);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/