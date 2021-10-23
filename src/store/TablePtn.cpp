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
TablePtn::TablePtn(Table *pTable, PtnId tPtnId)
: Object(CN_TABLEPTN), m_okLock(this) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  m_pTable = pTable;
  m_tPtnId = tPtnId;

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


  //for (i8 i_Bkt=0; i_Bkt < C_MAX_BKTS_PER_PTN; i_Bkt++)
  for (i8 i_Bkt=0; i_Bkt < C_MAX_BUCKETS; i_Bkt++)
  {
    vAddBucket(pTable, tPtnId, m_eKind);
    //vTrace("Bkt:%lld \n"_Stz, i_Bkt);
  }

//  vAddBucket(pTable, tPtnId, m_eKind);


  _checkInitialised_(CN_TABLEPTN);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TablePtn::~TablePtn(void) noexcept(false) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  // delete the Buckets
  i8 nItems = m_pBucketVec->nItems();
  for (i8 i_Bucket = 0; i_Bucket < nItems; i_Bucket++) {
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
eCN TablePtn::eKind(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  return m_eKind;
}


#if 0
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  return m_tPtnId;
}

/*----------------------------------------------------------------------------*/
/* set a mapped ptn number to be used                                         */
/*----------------------------------------------------------------------------*/
void TablePtn::vSetMappedPtn(PtnId tPtn) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  // Set the ptn to be used
  //m_tMappedPtn = tPtn;
}

/*----------------------------------------------------------------------------*/
/* get the partition number (in bucket map) of this table partition           */
/*----------------------------------------------------------------------------*/
PtnId  TablePtn::tMappedPtnNo(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  return m_tMappedPtn;
#endif

/*----------------------------------------------------------------------------*/
/* get the partition number (in grid) of this table partition                 */
/*----------------------------------------------------------------------------*/
PtnId TablePtn::tPtnId(void) {
_precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
return m_tPtnId;

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
void TablePtn::vAddBucket(Table *pTable, PtnId tPtnId, eCN eKind) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  Bucket *pBucket = new Bucket(pTable, tPtnId, eKind);
  m_pBucketVec->vAppend(pBucket);
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

  UNUSED(iBucket);
  // Set the bucket to be used
}

/*----------------------------------------------------------------------------*/
/* get a bucket by number                                                     */
/*----------------------------------------------------------------------------*/
Bucket *TablePtn::pGetBucket(i8 i_Bucket) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  //vTrace("TablePtn::pGetBucket from bucketvec %p \n"_Stz, m_pBucketVec);
  if (i_Bucket >= 0 && i_Bucket < m_pBucketVec->nItems())
    return m_pBucketVec->pGet(i_Bucket);
  else
    return nullptr;
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
void TablePtn::vAddIndex(IndexTree *pIndexTree)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);


  ASSERT(m_pIndexTreeVec->nItems() < C_MAX_INDEXES);
  m_pIndexTreeVec->vAdd(pIndexTree);
/*

  // Add index for all buckets
  for (i8 i_Bucket = 0; i_Bucket < m_pBucketVec->nItems(); i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    pBucket->vAddIndex(pIndexTree);
  }
*/

}

/*----------------------------------------------------------------------------*/
/* append a buffer into the TablePtn                                          */
/*----------------------------------------------------------------------------*/
void TablePtn::vAppendBuffer(DataBuffer *pDataBuffer)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  // append buffer first, then indexes
  // Get bucket number
  i8 iBktNo = pDataBuffer->iBucketNo();
  //vTrace("TablePtn::vAppendBuffer BufferNo:%lld  BucketNo:%lld   Ptn:%lld\n"_Stz,
  //       pDataBuffer->iBufferNo(), iBktNo, pDataBuffer->tPtnId().iAsI8());

  Bucket *pBucket = nullptr;
  if (iBktNo >= 0 && iBktNo < m_pBucketVec->nItems()) {
    pBucket = m_pBucketVec->pGet(iBktNo);
  }

  if (pBucket) {
    pBucket->vAppendBuffer(pDataBuffer);
    //vTrace(">>>>>>>>>>>>>>>>> Append row to vec:%p bucket:%p Bkt:%lld\n"_Stz, m_pBucketVec , pBucket, iBktNo );
  } else {
    vTrace(">>>>> ERROR >>>>>>>>>>>> Table:%s No bucket for BktNo:%lld available \n"_Stz, m_pTable->szTableNm()->azChar(),iBktNo);
  }


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

}

/*----------------------------------------------------------------------------*/
/* append a buffer into the TablePtn                                          */
/*----------------------------------------------------------------------------*/
void TablePtn::vAppendBuffer_replicated(DataBuffer *pDataBuffer)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  // append buffer first, then indexes
  // Get bucket number
  //i8 iBktNo = pDataBuffer->iBucketNo();
  //vTrace("TablePtn::vAppendBuffer BufferNo:%lld  BucketNo:%lld   Ptn:%lld\n"_Stz,
  //       pDataBuffer->iBufferNo(), iBktNo, pDataBuffer->tPtnId().iAsI8());

  Bucket *pBucket = m_pBucketVec->pGet(0);


  if (pBucket) {
    pBucket->vAppendBuffer(pDataBuffer);
    //vTrace(">>>>>>>>>>>>>>>>> Append row to vec:%p bucket:%p Bkt:%lld\n"_Stz, m_pBucketVec , pBucket, iBktNo );
  } else {
    vTrace(">>>>> ERROR >>>>>>>>>>>> Table:%s No bucket available \n"_Stz, m_pTable->szTableNm()->azChar());
  }


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

}

/*----------------------------------------------------------------------------*/
/* get the number of buffers in this partition                                */
/*----------------------------------------------------------------------------*/
i8 TablePtn::nBuffers(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  i8 nBufferCount = 0;
  i8 nItems = m_pBucketVec->nItems();
  for (i8 i_Bucket = 0; i_Bucket < nItems; i_Bucket++) {
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
#ifdef WITH_MULTI_BUCKET
  // get no of indexes of the table
  i8 nIndexes = m_pIndexTreeVec->nItems();

  // iterate all indexes of the table
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    // get index
    IndexTree* pIndexTree = m_pIndexTreeVec->pGet(i_Index);

    // insert entries from EntBuffer into the index
    pIndexTree->vInsertEntries(pCrwBuffer);
  }
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
  IndexTree* pIndexTree = this->pIndexTree(iIndexNo);
  i8 nItems = m_pBucketVec->nItems();
  for (i8 i_Bucket = 0; i_Bucket < nItems; i_Bucket++) {
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
  i8 nItems = m_pBucketVec->nItems();
  for (i8 i_Bucket = 0; i_Bucket < nItems; i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    pBucket->vAnalyze(nBuffers, nRows);
  }

  nRowsTotal += nRows;
  nBuffersTotal += nBuffers;
}







/*----------------------------------------------------------------------------*/
/* remove index                                                               */
/*----------------------------------------------------------------------------*/
void TablePtn::vRemoveIndex(i8 iIndexNo) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
#ifdef WITH_MULTI_BUCKET
  IndexTree* pIndexTree = this->pIndexTree(iIndexNo);

  // remove the index from the IdxDataVec
  m_pIndexTreeVec->vRemove(pIndexTree);
  delete pIndexTree;
#else
  m_pBucketVec->pGet(0)->vRemoveIndex(iIndexNo);
#endif
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
      {
        i8 nItems = m_pIndexTreeVec->nItems();
        for (i8 iIdxTre = 0; iIdxTre < nItems; iIdxTre++) {
          IndexTree *pIndexTree = m_pIndexTreeVec->pGet(iIdxTre);
          if (pIndexTree) pIndexTree->vTruncate();
        }
      }
      break;
    case CN_ROWTABLEPTN:
      // Row buffer doesn't have index
      break;
    default:
      NEVER_REACH;
  }



  // Now, truncate all data
  i8 nItems = m_pBucketVec->nItems();
  for (i8 i_Bucket = 0; i_Bucket < nItems; i_Bucket++) {
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
  i8 nItems = m_pIndexTreeVec->nItems();
  for (i8 iIdxTre = 0; iIdxTre < nItems; iIdxTre++) {
    IndexTree *pIndexTree = m_pIndexTreeVec->pGet(iIdxTre);

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
  i8 nItems = m_pBucketVec->nItems();
  for (i8 i_Bucket = 0; i_Bucket < nItems; i_Bucket++) {
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

  // visualize all buckets
  i8 nItems = m_pBucketVec->nItems();
  for (i8 i_Bucket = 0; i_Bucket < nItems; i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    pBucket->vVisualizeTable();
  }
}

/*----------------------------------------------------------------------------*/
/* dump the table data of this partition                                      */
/*----------------------------------------------------------------------------*/
Stz* TablePtn::szDumpContents(void)
{
  s4K sDumpBuffer = ""_Stz;
  i8 nItems = m_pBucketVec->nItems();
  for (i8 i_Bucket = 0; i_Bucket < nItems; i_Bucket++) {
    Bucket *pBucket = m_pBucketVec->pGet(i_Bucket);
    sDumpBuffer.vConcat(pBucket->szDumpContents(i_Bucket));
  }

  return sDumpBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
