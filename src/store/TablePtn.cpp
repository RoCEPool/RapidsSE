/*============================================================================*/
/* TablePtn.cpp -- Table Partition                                            */
/*============================================================================*/

#include "BASE.h"

//#include "RowCoder.hpp"
//#include "HeadLine.hpp"

//#include "Crw.hpp"
//#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"

//#include "CrvBuffer.hpp"

#include "TablePtn.hpp"
#include "Table.hpp"
#include "CrwTablePtn.hpp"
#include "RowTablePtn.hpp"
#include "RowBuffer.hpp"
#include "Bucket.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, TablePtn, CN_TABLEPTN)

__aNewFunction(TablePtn, CN_TABLEPTN)

__vDeleteFunction(TablePtn, CN_TABLEPTN)

__aFooterFunction(TablePtn)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TablePtn::TablePtn(Table *pTable, tPN tPtnNo)
    : Object(CN_TABLEPTN), m_okLock(this) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  m_pTable = pTable;
  m_tPtnNo = tPtnNo;

  m_pBucketVec = new BucketVec;

#ifdef WITH_BKTMAP_2N4P
  for (i8 i_Bkt=0; i_Bkt < C_MAX_BKTS_PER_PTN; i_Bkt++)
  {
    vAddBucket(pTable, tPtnNo);
  }
#else
  vAddBucket(pTable, tPtnNo);
#endif

  _checkInitialised_(CN_TABLEPTN);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TablePtn::~TablePtn(void) noexcept(false) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  // delete the Buckets
  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    delete m_pBucketVec->pGet(i_Bucket);
  }
  m_pBucketVec->vClear();
  // delete bucket vector
  delete m_pBucketVec;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* drop a read lock on the partition                                          */
/*----------------------------------------------------------------------------*/
void TablePtn::vDropRead(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  ASSERT(m_okLock.bIsLocked());
  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* drop a write lock on the partition                                         */
/*----------------------------------------------------------------------------*/
void TablePtn::vDropWrite(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  ASSERT(m_okLock.bIsLocked());
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* attempt at grabbing a read lock on the partition                           */
/*----------------------------------------------------------------------------*/
bool TablePtn::bGrabRead(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  return m_okLock.bGrabRead();

}

/*----------------------------------------------------------------------------*/
/* attempt at grabbing a write lock on the partition                          */
/*----------------------------------------------------------------------------*/
bool TablePtn::bGrabWrite(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  return m_okLock.bGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is the table partition locked?                                             */
/*----------------------------------------------------------------------------*/
bool TablePtn::bIsLocked(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  return m_okLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* return tablePtn kind                                                       */
/*----------------------------------------------------------------------------*/
eCN TablePtn::eKind(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  return m_eKind;
}

/*----------------------------------------------------------------------------*/
/* get the partition number (in grid) of this table partition                 */
/*----------------------------------------------------------------------------*/
tPN TablePtn::tPtnNo(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  return m_tPtnNo;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table *TablePtn::pTable(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  return m_pTable;
}


/*----------------------------------------------------------------------------*/
/*                                CRW/ROW                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add bucket                                                                 */
/*----------------------------------------------------------------------------*/
void TablePtn::vAddBucket(Table *pTable, tPN tPtnNo) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  Bucket *pBucket = new Bucket(pTable, tPtnNo);
  m_pBucketVec->vAdd(pBucket);
}

/*----------------------------------------------------------------------------*/
/* remove bucket                                                              */
/*----------------------------------------------------------------------------*/
void TablePtn::vRemoveBucket(i8 i_Bucket) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);

  if (pBucket) {
    m_pBucketVec->vRemove(pBucket);
    delete pBucket;
  }
}


/*----------------------------------------------------------------------------*/
/* set a bucket by number to be used                                          */
/*----------------------------------------------------------------------------*/
void TablePtn::vSetBucket(i8 iBucket) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  // Set the bucket to be used
}

/*----------------------------------------------------------------------------*/
/* get a bucket by number                                                     */
/*----------------------------------------------------------------------------*/
Bucket *TablePtn::pGetBucket(i8 i_Bucket) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  return m_pBucketVec->pGet(i_Bucket);
}

/*----------------------------------------------------------------------------*/
/* get number of bucket                                                       */
/*----------------------------------------------------------------------------*/
i8 TablePtn::nBuckets(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  return m_pBucketVec->nItems();
}

/*----------------------------------------------------------------------------*/
/* add index                                                                  */
/*----------------------------------------------------------------------------*/
void TablePtn::vAddIndex(IndexTree *pIndexTree) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  // Add index for all buckets
  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    pBucket->vAddIndex(pIndexTree);
  }

}

/*----------------------------------------------------------------------------*/
/* append a buffer into the TablePtn                                          */
/*----------------------------------------------------------------------------*/
void TablePtn::vAppendBuffer(DataBuffer *pDataBuffer) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);


#ifdef WITH_BKTMAP_2N4P
  // eventually, we have to append the rowbuffer into the correct bucket

  i8 iBktNo = pDataBuffer->iBucketNo();
  vTrace("TablePtn::vAppendBuffer  Bucket:%lld   Ptn:%lld\n"_Stz, iBktNo, pDataBuffer->tPtnNo());

  Bucket *pBucket = pGetBucket(iBktNo);
  pBucket->vAppendBuffer(pDataBuffer);

#else
  pGetBucket(0)->vAppendBuffer(pDataBuffer);
#endif
}

/*----------------------------------------------------------------------------*/
/* get the number of buffers in this partition                                */
/*----------------------------------------------------------------------------*/
i8 TablePtn::nBuffers(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  i8 nBufferCount = 0;

  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    nBufferCount += pBucket->nBuffers();
  }
  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* dump the table                                                             */
/*----------------------------------------------------------------------------*/
void TablePtn::vDump(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    pBucket->vDump();
  }

}

/*----------------------------------------------------------------------------*/
/* get IndexTree                                                              */
/*----------------------------------------------------------------------------*/
IndexTree *TablePtn::pIndexTree(i8 iIndexNo) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
#ifdef WITH_MULTI_BUCKET
  IndexTree *pIndexTree = nullptr;
  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    pIndexTree = pBucket->pIndexTree(iIndexNo);
    if (pIndexTree)
      break;
  }
  return pIndexTree;
#else
  IndexTree *pIndexTree = m_pBucket->pIndexTree(iIndexNo);

  return pIndexTree;
#endif
}

/*----------------------------------------------------------------------------*/
/* get index data vector                                                      */
/*----------------------------------------------------------------------------*/
IndexTreeVec *TablePtn::pIndexTreeVec(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

#ifdef WITH_MULTI_BUCKET
  // OK, this one it tricky, which bucket ? RAY?

  IndexTreeVec *pIndexTreeVec = nullptr;
  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    pIndexTreeVec = pBucket->pIndexTreeVec();
    if (pIndexTreeVec)
      break;
  }
  return pIndexTreeVec;
#else
  IndexTreeVec *pIndexTreeVec = m_pBucket->pIndexTreeVec();

  return pIndexTreeVec;
#endif
}

/*----------------------------------------------------------------------------*/
/* insert entries into all indexes for all rows in given buffer               */
/*----------------------------------------------------------------------------*/
void TablePtn::vInsertIndexEntries(CrwBuffer *pCrwBuffer) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

#ifdef WITH_BKTMAP_2N4P

  /*for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket* pBucket = m_pBucketVec->pGet(i_Bucket);
    pBucket->vInsertIndexEntries(pCrwBuffer);
  }*/
  i8 iBktNo = pCrwBuffer->iBucketNo();
  Bucket *pBucket = pGetBucket(iBktNo);
  pBucket->vInsertIndexEntries(pCrwBuffer);

#else
  m_pBucketVec->pGet(0)->vInsertIndexEntries(pCrwBuffer);
#endif
}

/*----------------------------------------------------------------------------*/
/* insert entries into given index for all rows in this partition             */
/*----------------------------------------------------------------------------*/
bool TablePtn::bPopulateIndex(i8 iIndexNo) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

#ifdef WITH_MULTI_BUCKET
  // Should it be only one bucket has this indexNo ? RAY ?
  bool b = false;

  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    b = pBucket->bPopulateIndex(iIndexNo);
    if (b) break;
  }
  return b;
#else
  bool b = m_pBucket->bPopulateIndex(iIndexNo);

  return b;
#endif
}

/*----------------------------------------------------------------------------*/
/* get the root crw buffer                                                    */
/*----------------------------------------------------------------------------*/
CrwBuffer *TablePtn::pRootCrwBuffer(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

#ifdef WITH_MULTI_BUCKET
  // Which bucket ? RAY ?
  CrwBuffer *pRootBuffer = nullptr;


  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    pRootBuffer = pBucket->pRootCrwBuffer();
    if (pRootBuffer) break;
  }
  return pRootBuffer;
#else
  CrwBuffer* pRootBuffer = m_pBucket->pRootCrwBuffer();

  return pRootBuffer;
#endif
}

/*----------------------------------------------------------------------------*/
/* get the root buffer                                                        */
/*----------------------------------------------------------------------------*/
DataBuffer *TablePtn::pRootBuffer(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
#ifdef WITH_MULTI_BUCKET
  // Which bucket ? RAY ?
  DataBuffer *pRootBuffer = nullptr;

  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    pRootBuffer = pBucket->pRootBuffer();
    if (pRootBuffer) break;
  }
  return pRootBuffer;
#else
  DataBuffer* pRootBuffer = m_pBucket->pRootBuffer();

  return pRootBuffer;
#endif
}

/*----------------------------------------------------------------------------*/
/* get the root row buffer                                                    */
/*----------------------------------------------------------------------------*/
RowBuffer *TablePtn::pRootRowBuffer(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

#ifdef WITH_MULTI_BUCKET
  // Which bucket ? RAY
  RowBuffer *pRootBuffer = nullptr;

  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    pRootBuffer = pBucket->pRootRowBuffer();
    if (pRootBuffer) break;
  }
  return pRootBuffer;
#else
  RowBuffer* pRootBuffer = m_pBucket->pRootRowBuffer();

  return pRootBuffer;
#endif
}

/*----------------------------------------------------------------------------*/
/* remove index                                                               */
/*----------------------------------------------------------------------------*/
void TablePtn::vRemoveIndex(i8 iIndexNo) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
#ifdef WITH_MULTI_BUCKET
  // Should it be only one bucket has this indexNo ? RAY

  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    pBucket->vRemoveIndex(iIndexNo);
  }

#else
  m_pBucket->vRemoveIndex(iIndexNo);
#endif

}

/*----------------------------------------------------------------------------*/
/* truncate the table data                                                    */
/*----------------------------------------------------------------------------*/
void TablePtn::vTruncate(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

#ifdef WITH_MULTI_BUCKET
  // truncate all buckets

  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    pBucket->vTruncate();
  }

#else
  m_pBucket->vTruncate();
#endif
}

/*----------------------------------------------------------------------------*/
/* truncate the index data for given index                                    */
/*----------------------------------------------------------------------------*/
void TablePtn::vTruncateIndex(Index *pIndex) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

#ifdef WITH_MULTI_BUCKET
  // truncate all index

  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    // Make sure we don't crash at nullptr
    pBucket->vTruncateIndex(pIndex);
  }

#else
  m_pBucket->vTruncateIndex(pIndex);
#endif
}

/*----------------------------------------------------------------------------*/
/* verify the table data of this partition                                    */
/*----------------------------------------------------------------------------*/
bool TablePtn::bVerifyTable(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
#ifdef WITH_MULTI_BUCKET
  // verify all tables
  bool bVerify = false;
  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    // Make sure we don't crash at nullptr
    bVerify = pBucket->bVerifyTable();
    if (!bVerify)
      break;
  }
  return bVerify;
#else
  bool bVerify = m_pBucket->bVerifyTable();

  return bVerify;
#endif
}

/*----------------------------------------------------------------------------*/
/* visualize the table data of this partition                                 */
/*----------------------------------------------------------------------------*/
void TablePtn::vVisualizeTable(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

#ifdef WITH_MULTI_BUCKET
  // visualize all buckets

  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    // Make sure we don't crash at nullptr
    pBucket->vVisualizeTable();
  }
#else
  m_pBucket->vVisualizeTable();
#endif
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
