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

  // ... code which pipesi in all the parts of a meta-object and
  //     gets the meta-object to reassemble itself ...


  // get arguments
  Table* pTable = nullptr;
  Table* pUpdateTable = nullptr;
  BucketInfo* pBucketInfo = nullptr;

  Parcel* pParcel = (Parcel*)m_pDataStack->pPopPtr(__func__);

  i8 nObjParts = pParcel->nItems();

  vTrace("C++ gets called: vBucketRecvPutParts. Received pParcel:%p with %lld parts\n"_Stz, pParcel, nObjParts);

  for (i8 i_Item = 0; i_Item < nObjParts; i_Item++) {
    Object *pObject = pParcel->pGetItem(i_Item);
    if (pObject && (pObject->eGetClassNo() == CN_TABLE)) {
      pTable = (Table *) pObject;

      // get the local table then call update bktmap code
      pUpdateTable = AM::pTableFind(pTable->szTableNm());
      if (pUpdateTable) {
          vTrace("C++ gets called: vBucketRecvPutParts.  for table: %s OK\n"_Stz, pUpdateTable->szTableNm()->azChar());
      }

    } else if (pObject && (pObject->eGetClassNo() == CN_CRWBUFFER)) {
      vTrace("C++ gets called: vBucketRecvPutParts. CN_CRWBUFFER\n"_Stz);
      if (pUpdateTable) {
        if (pUpdateTable->bUpdateBucket(pParcel))
          vTrace("C++ gets called: vBucketRecvPutParts. Updated row buffer for table: %s OK\n"_Stz, pUpdateTable->szTableNm()->azChar());
      }
    }  else if (pObject && (pObject->eGetClassNo() == CN_BUCKETINFO)) {
      vTrace("C++ gets called: vBucketRecvPutParts. Got  CN_BUCKETINFO OK\n"_Stz);
      pBucketInfo = (BucketInfo *) pObject;
      pUpdateTable = AM::pTableFind(pBucketInfo->sTableName());
      if (pUpdateTable) {
        vTrace("C++ gets called: vBucketRecvPutParts. Got  CN_BUCKETINFO for table: %s OK\n"_Stz, pUpdateTable->szTableNm()->azChar());
        if (pUpdateTable->bUpdateBucket(pParcel))
          vTrace("C++ gets called: vBucketRecvPutParts. Updated row buffer for table: %s OK\n"_Stz, pUpdateTable->szTableNm()->azChar());
        break;
      }
    }
  }

  // Do we have to push the table here?
  m_pDataStack->vPushPtr(pUpdateTable);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/