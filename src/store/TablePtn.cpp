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
  m_pIndexTreeVec  = new IndexTreeVec;

  eTT eTableKind = m_pTable->eKind();
  switch (eTableKind) {
    case TT_INFOSET:
      NEVER_REACH;
      break;
    case TT_INTERNAL:
      m_eKind = CN_ROWTABLEPTN;
      break;
    case TT_NORMAL:
      m_eKind = CN_CRWTABLEPTN;
      break;
    case TT_REPLICATED:
      NEVER_REACH;
      break;
    default:
      NEVER_REACH;
  }

#ifdef WITH_BKTMAP_2N4P
  for (i8 i_Bkt=0; i_Bkt < C_MAX_BKTS_PER_PTN; i_Bkt++)
  {
    vAddBucket(pTable, tPtnNo, m_eKind);
  }
#else
  vAddBucket(pTable, tPtnNo, m_eKind);
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

  // delete the index data vector
  delete m_pIndexTreeVec;
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
//eCN TablePtn::eKind(void) {
//  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
//  return m_eKind;
//}

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
void TablePtn::vAddBucket(Table *pTable, tPN tPtnNo, eCN eKind) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  Bucket *pBucket = new Bucket(pTable, tPtnNo, eKind);
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

#ifdef WITH_MULTI_BUCKET
  ASSERT(m_pIndexTreeVec->nItems() < C_MAX_INDEXES);
  m_pIndexTreeVec->vAdd(pIndexTree);
#else

  // Add index for all buckets
  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    pBucket->vAddIndex(pIndexTree);
  }
#endif

}

/*----------------------------------------------------------------------------*/
/* append a buffer into the TablePtn                                          */
/*----------------------------------------------------------------------------*/
void TablePtn::vAppendBuffer(DataBuffer *pDataBuffer) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  // Note: After append buffer we need to insert into index
  // We need code for append buffer here

#ifdef WITH_BKTMAP_2N4P
  // >>> Need to come back to this
  // now insert entries from the buffer into the indexes
  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      vInsertIndexEntries(_pCrwBuffer(pDataBuffer));
      break;
    case CN_ROWTABLEPTN:
      //m_pRowTablePtn->vAppendBuffer(_pRowBuffer(pDataBuffer));
      //vInsertIndexEntries(_pRowBuffer(pDataBuffer));
      break;
    default:
      NEVER_REACH;
  }




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
  IndexTree* pIndexTree = nullptr;
  i8 nIndexes = m_pIndexTreeVec->nItems();

  for (i8 i_Index=0; i_Index < nIndexes; i_Index++) {
    pIndexTree = m_pIndexTreeVec->pGet(i_Index);
    Index* pIndex = pIndexTree->pIndex();
    if (pIndex->iIndexNo() == iIndexNo)
      break;
  }

  return pIndexTree;
#else
  IndexTree *pIndexTree = m_pBucketVec->pGet(0)->pIndexTree(iIndexNo);

  return pIndexTree;
#endif
}

/*----------------------------------------------------------------------------*/
/* get index data vector                                                      */
/*----------------------------------------------------------------------------*/
IndexTreeVec *TablePtn::pIndexTreeVec(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

#ifdef WITH_MULTI_BUCKET
  return m_pIndexTreeVec;
#else
  IndexTreeVec *pIndexTreeVec = m_pBucketVec->pGet(0)->pIndexTreeVec();

  return pIndexTreeVec;
#endif
}

/*----------------------------------------------------------------------------*/
/* insert entries into all indexes for all rows in given buffer               */
/*----------------------------------------------------------------------------*/
void TablePtn::vInsertIndexEntries(CrwBuffer *pCrwBuffer) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  // get no of indexes of the table
  i8 nIndexes = m_pIndexTreeVec->nItems();

  // iterate all indexes of the table
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    // get index
    IndexTree* pIndexTree = m_pIndexTreeVec->pGet(i_Index);

    // insert entries from EntBuffer into the index
    pIndexTree->vInsertEntries(pCrwBuffer);
  }
}

/*----------------------------------------------------------------------------*/
/* insert entries into given index for all rows in this partition             */
/*----------------------------------------------------------------------------*/
bool TablePtn::bPopulateIndex(i8 iIndexNo) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

#ifdef WITH_MULTI_BUCKET
  IndexTree* pIndexTree = this->pIndexTree(iIndexNo);

  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    pBucket->bPopulateIndexTree(pIndexTree);
  }

  return true;
#else
  bool b = m_pBucketVec->pGet(0)->bPopulateIndex(iIndexNo);

  return b;
#endif
}

/*----------------------------------------------------------------------------*/
/* analyze table                                                              */
/*----------------------------------------------------------------------------*/
void TablePtn::vAnalyze(i8& nBuffers, i8& nRows)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  i8 nRowsTotal    = 0;
  i8 nBuffersTotal = 0;

  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    pBucket->vAnalyze(nBuffers, nRows);
  }

  nRowsTotal += nRows;
  nBuffersTotal += nBuffers;
}

/*----------------------------------------------------------------------------*/
/* get the root crw buffer                                                    */
/*----------------------------------------------------------------------------*/
CrwBuffer *TablePtn::pRootCrwBuffer(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  //vTrace("TablePtn::pRootCrwBuffer \n"_Stz);
  //NEVER_REACH;

  CrwBuffer *pRootBuffer = nullptr;

  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    pRootBuffer = pBucket->pRootCrwBuffer();
    if (pRootBuffer) break;
  }
  return pRootBuffer;

}

/*----------------------------------------------------------------------------*/
/* get the root buffer                                                        */
/*----------------------------------------------------------------------------*/
DataBuffer *TablePtn::pRootBuffer(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  //vTrace("TablePtn::pRootBuffer \n"_Stz);
  //NEVER_REACH;




#ifdef WITH_MULTI_BUCKET
  // Which bucket ? RAY ?
  // this concept doesn't work anymore, will be removed from the system
  // new api to access all rows from ptn stream
  DataBuffer *pRootBuffer = nullptr;

  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    pRootBuffer = pBucket->pRootBuffer();
    if (pRootBuffer) break;
  }
  return pRootBuffer;
#else
  DataBuffer* pRootBuffer = m_pBucketVec->pGet(0)->pRootBuffer();

  return pRootBuffer;
#endif
}

/*----------------------------------------------------------------------------*/
/* get the root row buffer                                                    */
/*----------------------------------------------------------------------------*/
RowBuffer *TablePtn::pRootRowBuffer(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  //vTrace("TablePtn::pRootRowBuffer \n"_Stz);
  //NEVER_REACH;


#ifdef WITH_MULTI_BUCKET
  // Which bucket ? RAY
  // this concept doesn't work anymore, will be removed from the system
  // new api to access all rows from ptn stream
  RowBuffer *pRootBuffer = nullptr;

  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    pRootBuffer = pBucket->pRootRowBuffer();
    if (pRootBuffer) break;
  }
  return pRootBuffer;
#else
  RowBuffer* pRootBuffer = m_pBucketVec->pGet(0)->pRootRowBuffer();

  return pRootBuffer;
#endif
}

/*----------------------------------------------------------------------------*/
/* remove index                                                               */
/*----------------------------------------------------------------------------*/
void TablePtn::vRemoveIndex(i8 iIndexNo) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  IndexTree* pIndexTree = this->pIndexTree(iIndexNo);

  // remove the index from the IdxDataVec
  m_pIndexTreeVec->vRemove(pIndexTree);
  delete pIndexTree;
}

/*----------------------------------------------------------------------------*/
/* truncate the table data                                                    */
/*----------------------------------------------------------------------------*/
void TablePtn::vTruncate(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  // truncate for all buckets

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      // first truncate all indexes for this table ptn
      for (i8 iIdxTre = 0; iIdxTre < m_pIndexTreeVec->nItems(); iIdxTre++) {
        IndexTree *pIndexTree = m_pIndexTreeVec->pGet(iIdxTre);
        pIndexTree->vTruncate();
      }
      break;
    case CN_ROWTABLEPTN:
      // Row buffer doesn't have index
      break;
    default:
      NEVER_REACH;
  }

  // Now, truncate all data
  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    pBucket->vTruncate();
  }

}

/*----------------------------------------------------------------------------*/
/* truncate the index data for given index                                    */
/*----------------------------------------------------------------------------*/
void TablePtn::vTruncateIndex(Index *pIndex) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

#ifdef WITH_MULTI_BUCKET
  // truncate all index, only for Crw buffer
  bool bFoundIndex = false;

  // find the index to be truncated for this table ptn
  for (i8 iIdxTre = 0; iIdxTre < m_pIndexTreeVec->nItems(); iIdxTre++) {
    IndexTree* pIndexTree = m_pIndexTreeVec->pGet(iIdxTre);

    if (pIndexTree->pIndex() == pIndex) {
      bFoundIndex = true;
      // truncate the index
      pIndexTree->vTruncate();
      break;
    }
  }

  ASSERT(bFoundIndex);


#else
  m_pBucketVec->pGet(0)->vTruncateIndex(pIndex);
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
    bVerify = pBucket->bVerifyTable();
    if (!bVerify)
      break;
  }
  return bVerify;
#else
  bool bVerify = m_pBucketVec->pGet(0)->bVerifyTable();

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
    pBucket->vVisualizeTable();
  }
#else
  m_pBucketVec->pGet(0)->vVisualizeTable();
#endif
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
