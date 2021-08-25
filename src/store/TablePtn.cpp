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

__pCastFunction(STORE,TablePtn,CN_TABLEPTN)
__aNewFunction(TablePtn,CN_TABLEPTN)
__vDeleteFunction(TablePtn,CN_TABLEPTN)
__aFooterFunction(TablePtn)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TablePtn::TablePtn(Table* pTable, tPN tPtnNo)
: Object(CN_TABLEPTN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);

  m_pTable = pTable;
  m_tPtnNo = tPtnNo;

  m_pBucketVec = new BucketVec;

  vAddBucket(pTable, tPtnNo);
#ifdef WITH_BKTMAP_2N4P
  // add second bucket here
  vAddBucket(pTable, tPtnNo);
#endif

  // Temporary: Be used for now as the only 1 bucket per partition location
  m_pBucket = pGetBucket(0);

  _checkInitialised_(CN_TABLEPTN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TablePtn::~TablePtn(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);

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
void TablePtn::vDropRead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(m_pBucket->bIsLocked());
  m_pBucket->vDropRead();
}

/*----------------------------------------------------------------------------*/
/* drop a write lock on the partition                                         */
/*----------------------------------------------------------------------------*/
void TablePtn::vDropWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(m_pBucket->bIsLocked());

  return m_pBucket->vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* attempt at grabbing a read lock on the partition                           */
/*----------------------------------------------------------------------------*/
bool TablePtn::bGrabRead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  return m_pBucket->bGrabRead();
}

/*----------------------------------------------------------------------------*/
/* attempt at grabbing a write lock on the partition                          */
/*----------------------------------------------------------------------------*/
bool TablePtn::bGrabWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  return m_pBucket->bGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is the table partition locked?                                             */
/*----------------------------------------------------------------------------*/
bool TablePtn::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  return m_pBucket->bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* return tablePtn kind                                                       */
/*----------------------------------------------------------------------------*/
eCN TablePtn::eKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  return m_pBucket->eKind();
}

/*----------------------------------------------------------------------------*/
/* get the partition number (in grid) of this table partition                 */
/*----------------------------------------------------------------------------*/
tPN TablePtn::tPtnNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  return m_pBucket->tPtnNo();
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* TablePtn::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  return m_pBucket->pTable();
}


/*----------------------------------------------------------------------------*/
/*                                CRW/ROW                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add bucket                                                                 */
/*----------------------------------------------------------------------------*/
void TablePtn::vAddBucket(Table* pTable, tPN tPtnNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  Bucket* pBucket = new Bucket(pTable, tPtnNo);
  m_pBucketVec->vAdd(pBucket);
}

/*----------------------------------------------------------------------------*/
/* remove bucket                                                              */
/*----------------------------------------------------------------------------*/
void TablePtn::vRemoveBucket(i8 i_Bucket)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  Bucket* pBucket = m_pBucketVec->pGet(i_Bucket);

  if (pBucket) {
    m_pBucketVec->vRemove(pBucket);
    delete pBucket;
  }
}


/*----------------------------------------------------------------------------*/
/* set a bucket by number to be used                                          */
/*----------------------------------------------------------------------------*/
void TablePtn::vSetBucket(i8 iBucket)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);

  // Set the bucket to be used
}

/*----------------------------------------------------------------------------*/
/* get a bucket by number                                                     */
/*----------------------------------------------------------------------------*/
Bucket* TablePtn::pGetBucket(i8 i_Bucket)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);

  return m_pBucketVec->pGet(i_Bucket);
}

/*----------------------------------------------------------------------------*/
/* get number of bucket                                                       */
/*----------------------------------------------------------------------------*/
i8 TablePtn::nBuckets(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  return m_pBucketVec->nItems();
}

/*----------------------------------------------------------------------------*/
/* add index                                                                  */
/*----------------------------------------------------------------------------*/
void TablePtn::vAddIndex(IndexTree* pIndexTree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  m_pBucket->vAddIndex(pIndexTree);
}

/*----------------------------------------------------------------------------*/
/* append a buffer into the TablePtn                                          */
/*----------------------------------------------------------------------------*/
void TablePtn::vAppendBuffer(DataBuffer* pDataBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);


#ifdef WITH_BKTMAP_2N4P
  // eventually, we have to append the rowbuffer into the correct bucket
  i8 iBktNo = pDataBuffer->iBucketNo();
  vTrace("TablePtn::vAppendBuffer  Bucket:%lld   Ptn:%lld\n"_Stz, iBktNo, pDataBuffer->tPtnNo());
  Bucket* pBucket = pGetBucket(iBktNo );
  pBucket->vAppendBuffer(pDataBuffer);
  // let's insert the same data into the two buckets, because we don't have a way of
  // getting data from which bucket
  //m_pBucketVec->pGet(0)->vAppendBuffer(pDataBuffer);
  //m_pBucketVec->pGet(1)->vAppendBuffer(pDataBuffer);

#else
  m_pBucket->vAppendBuffer(pDataBuffer);
#endif
}

/*----------------------------------------------------------------------------*/
/* get the number of buffers in this partition                                */
/*----------------------------------------------------------------------------*/
i8 TablePtn::nBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  i8 nBufferCount = m_pBucket->nBuffers();

  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* dump the table                                                             */
/*----------------------------------------------------------------------------*/
void TablePtn::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  m_pBucket->vDump();
}

/*----------------------------------------------------------------------------*/
/* get IndexTree                                                              */
/*----------------------------------------------------------------------------*/
IndexTree* TablePtn::pIndexTree(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  IndexTree* pIndexTree = m_pBucket->pIndexTree(iIndexNo);

  return pIndexTree;
}

/*----------------------------------------------------------------------------*/
/* get index data vector                                                      */
/*----------------------------------------------------------------------------*/
IndexTreeVec* TablePtn::pIndexTreeVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  IndexTreeVec* pIndexTreeVec = m_pBucket->pIndexTreeVec();

  return pIndexTreeVec;
}

/*----------------------------------------------------------------------------*/
/* insert entries into all indexes for all rows in given buffer               */
/*----------------------------------------------------------------------------*/
void TablePtn::vInsertIndexEntries(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  m_pBucket->vInsertIndexEntries(pCrwBuffer);
}

/*----------------------------------------------------------------------------*/
/* insert entries into given index for all rows in this partition             */
/*----------------------------------------------------------------------------*/
bool TablePtn::bPopulateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  bool b = m_pBucket->bPopulateIndex(iIndexNo);

  return b;
}

/*----------------------------------------------------------------------------*/
/* get the root crw buffer                                                    */
/*----------------------------------------------------------------------------*/
CrwBuffer* TablePtn::pRootCrwBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  CrwBuffer* pRootBuffer = m_pBucket->pRootCrwBuffer();

  return pRootBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the root buffer                                                        */
/*----------------------------------------------------------------------------*/
DataBuffer* TablePtn::pRootBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);

  DataBuffer* pRootBuffer = m_pBucket->pRootBuffer();

  return pRootBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the root row buffer                                                    */
/*----------------------------------------------------------------------------*/
RowBuffer* TablePtn::pRootRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);

  RowBuffer* pRootBuffer = m_pBucket->pRootRowBuffer();

  return pRootBuffer;
}

/*----------------------------------------------------------------------------*/
/* remove index                                                               */
/*----------------------------------------------------------------------------*/
void TablePtn::vRemoveIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  m_pBucket->vRemoveIndex(iIndexNo);

}

/*----------------------------------------------------------------------------*/
/* truncate the table data                                                    */
/*----------------------------------------------------------------------------*/
void TablePtn::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);

  m_pBucket->vTruncate();
}

/*----------------------------------------------------------------------------*/
/* truncate the index data for given index                                    */
/*----------------------------------------------------------------------------*/
void TablePtn::vTruncateIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);

  m_pBucket->vTruncateIndex(pIndex);
}

/*----------------------------------------------------------------------------*/
/* verify the table data of this partition                                    */
/*----------------------------------------------------------------------------*/
bool TablePtn::bVerifyTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  bool bVerify = m_pBucket->bVerifyTable();

  return bVerify;
}

/*----------------------------------------------------------------------------*/
/* visualize the table data of this partition                                 */
/*----------------------------------------------------------------------------*/
void TablePtn::vVisualizeTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  m_pBucket->vVisualizeTable();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
