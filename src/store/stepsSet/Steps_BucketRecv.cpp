/*============================================================================*/
/* Steps_BktMapGet.cpp -- BktMapGet  step group                               */
/*============================================================================*/


#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eTT.h"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Parcel.hpp"
#include "BucketInfo.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_BucketRecv(void)
{
  vAddStep("bktRecv::putParts"_Stz,&Step::vBucketRecvPutParts);
}

/*----------------------------------------------------------------------------*/
/* bktRecv::putParts -- put parts of rows data                                */
/*----------------------------------------------------------------------------*/
void Step::vBucketRecvPutParts(void)
{
  ENTRY();

  Table* pTable = nullptr;
  Table* pUpdateTable = nullptr;
  BucketInfo* pBucketInfo = nullptr;

  // get arguments
  Parcel* pParcel = (Parcel*)m_pDataStack->pPopPtr(__func__);

  i8 nObjParts = pParcel->nItems();

  //vTrace("C++ gets called: bktRecv::putParts. Received pParcel:%p with %lld parts\n"_Stz, pParcel, nObjParts);

  for (i8 i_Item = 0; i_Item < nObjParts; i_Item++) {
    Object *pObject = pParcel->pGetItem(i_Item);

    if (pObject && (pObject->eGetClassNo() == CN_BUCKETINFO)) {
      pBucketInfo = (BucketInfo *) pObject;
      vTrace("C++ gets called: bktRecv::putParts. %s Got  CN_BUCKETINFO from node:%s\n"_Stz, AM::szNodeNm()->azChar(), pBucketInfo->sNodeName().azChar());
      pUpdateTable = AM::pTableFind(pBucketInfo->sTableName());
      if (pUpdateTable) {
        //vTrace("C++ gets called: bktRecv::putParts. Got  CN_BUCKETINFO for table: %s OK\n"_Stz, pUpdateTable->szTableNm()->azChar());
        if (pUpdateTable->bUpdateBucket(pParcel))
          vTrace("C++ gets called: bktRecv::putParts. Updated row buffer for table: %s OK\n"_Stz, pUpdateTable->szTableNm()->azChar());
        break;
      }
    }
  }

  // Do we have to push the table here?
  //m_pDataStack->vPushPtr(pUpdateTable);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/