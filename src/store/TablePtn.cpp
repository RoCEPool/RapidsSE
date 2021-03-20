/*============================================================================*/
/* TablePtn.cpp -- Table Partition                                            */
/*============================================================================*/

#include "BASE.h"

#include "RowCoder.hpp"
#include "HeadLine.hpp"

#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"

#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "CrwTablePtn.hpp"

#define __CRWTABLEPTN

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
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_pTable = pTable;
  m_tPtnNo = tPtnNo;
  
  m_pCrwTablePtn = new CrwTablePtn(m_pTable,m_tPtnNo,this);
  
  m_pRootBuffer  = nullptr;
  m_pCurrentBuffer = nullptr;
  
  m_pIndexTreeVec = new IndexTreeVec;
    
  _checkInitialised_(CN_TABLEPTN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TablePtn::~TablePtn(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  // delete the index data vector
  
  delete m_pCrwTablePtn;
  delete m_pIndexTreeVec;
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
  
  ASSERT(m_okLock.bIsLocked());
  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* drop a write lock on the partition                                          */
/*----------------------------------------------------------------------------*/
void TablePtn::vDropWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* attempt at grabbing a read lock on the partition                           */
/*----------------------------------------------------------------------------*/
bool TablePtn::bGrabRead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);

  return m_okLock.bGrabRead();
}

/*----------------------------------------------------------------------------*/
/* attempt at grabbing a write lock on the partition                          */
/*----------------------------------------------------------------------------*/
bool TablePtn::bGrabWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_okLock.bGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is the table partition locked?                                             */
/*----------------------------------------------------------------------------*/
bool TablePtn::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_okLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* get the partition number (in grid) of this table partition                 */
/*----------------------------------------------------------------------------*/
tPN TablePtn::tPtnNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_tPtnNo;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* TablePtn::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_pTable;
}



/*----------------------------------------------------------------------------*/
/*                                    CRW                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add index                                                                  */
/*----------------------------------------------------------------------------*/
#ifdef __CRWTABLEPTN
void TablePtn::vAddIndex(IndexTree* pIndexTree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_pCrwTablePtn->vAddIndex(pIndexTree);
}
#else
//??????????????????????????????????????????????????????????????????????????????
void TablePtn::vAddIndex(IndexTree* pIndexTree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(m_pIndexTreeVec->nItems() < C_MAX_INDEXES);
  m_pIndexTreeVec->vAdd(pIndexTree);
}
#endif

/*----------------------------------------------------------------------------*/
/* append a crw buffer into the TablePtn                                      */
/*----------------------------------------------------------------------------*/
#ifdef __CRWTABLEPTN
void TablePtn::vAppendBuffer(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_pCrwTablePtn->vAppendBuffer(pCrwBuffer);
}
#else
//??????????????????????????????????????????????????????????????????????????????
void TablePtn::vAppendBuffer(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.IndexTree.m_nAppendBuffers++;
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // if buffer has 0 entries (only head) then should be last buffer
  if (pCrwBuffer->nRowCount() == 0) {
    delete pCrwBuffer;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark crwBuffer as permanent
  pCrwBuffer->vIsPermanent(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the table in the buffer
  pCrwBuffer->vTable(m_pTable);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  RASSERT(m_okLock.bIsLocked());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the root buffer
  if (m_pRootBuffer == nullptr) {
    ASSERT( ! m_pCurrentBuffer )
    m_pRootBuffer = pCrwBuffer;
  }
  else {
    pCrwBuffer->vNextLinkedBuffer(m_pCurrentBuffer->pNextLinkedBuffer());
    pCrwBuffer->vPrevLinkedBuffer(m_pCurrentBuffer);
    m_pCurrentBuffer->vNextLinkedBuffer(pCrwBuffer);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the current buffer
  m_pCurrentBuffer = pCrwBuffer;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now insert entries from the buffer into the indexes
  vInsertIndexEntries(pCrwBuffer);
  
}
#endif

/*----------------------------------------------------------------------------*/
/* append a row buffer into the TablePtn                                      */
/*----------------------------------------------------------------------------*/
#ifdef __CRWTABLEPTN
void TablePtn::vAppendBuffer(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  NEVER_REACH;
}
#else
//??????????????????????????????????????????????????????????????????????????????
void TablePtn::vAppendBuffer(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  NEVER_REACH;
}
#endif

/*----------------------------------------------------------------------------*/
/* get the number of buffers in this partition                                */
/*----------------------------------------------------------------------------*/
#ifdef __CRWTABLEPTN
i8 TablePtn::nBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  i8 nBufferCount = m_pCrwTablePtn->nBuffers();
    
  return nBufferCount;
}
#else
//??????????????????????????????????????????????????????????????????????????????
i8 TablePtn::nBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  i8 nBufferCount = 0;
  
  if (m_pRootBuffer)
    nBufferCount = m_pRootBuffer->nLinkedBufferCount();
  
  return nBufferCount;
}
#endif

/*----------------------------------------------------------------------------*/
/* dump the table                                                             */
/*----------------------------------------------------------------------------*/
#ifdef __CRWTABLEPTN
void TablePtn::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_pCrwTablePtn->vDump();
}
#else
//??????????????????????????????????????????????????????????????????????????????
void TablePtn::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  CrwBuffer* pCrwBuffer = m_pRootBuffer;
    
  while (pCrwBuffer) {
    pCrwBuffer->vShow();
    pCrwBuffer = pCrwBuffer->pNextCrwBuffer();
  }

}
#endif

/*----------------------------------------------------------------------------*/
/* get IndexTree                                                              */
/*----------------------------------------------------------------------------*/
#ifdef __CRWTABLEPTN
IndexTree* TablePtn::pIndexTree(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  IndexTree* pIndexTree = m_pCrwTablePtn->pIndexTree(iIndexNo);
  
  return pIndexTree;
}
#else
//??????????????????????????????????????????????????????????????????????????????
IndexTree* TablePtn::pIndexTree(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  IndexTree* pIndexTree = nullptr;
  i8 nIndexes = m_pIndexTreeVec->nItems();
  
  for (i8 i_Index=0; i_Index < nIndexes; i_Index++) {
    pIndexTree = m_pIndexTreeVec->pGet(i_Index);
    Index* pIndex = pIndexTree->pIndex();
    if (pIndex->iIndexNo() == iIndexNo)
      break;
  }
  
  return pIndexTree;
}
#endif

/*----------------------------------------------------------------------------*/
/* get index data vector                                                      */
/*----------------------------------------------------------------------------*/
#ifdef __CRWTABLEPTN
IndexTreeVec* TablePtn::pIndexTreeVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  IndexTreeVec* pIndexTreeVec = m_pCrwTablePtn->pIndexTreeVec();
  
  return pIndexTreeVec;
}
#else
//??????????????????????????????????????????????????????????????????????????????
IndexTreeVec* TablePtn::pIndexTreeVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_pIndexTreeVec;
}
#endif

/*----------------------------------------------------------------------------*/
/* insert entries into all indexes for all rows in given buffer               */
/*----------------------------------------------------------------------------*/
#ifdef __CRWTABLEPTN
void TablePtn::vInsertIndexEntries(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_pCrwTablePtn->vInsertIndexEntries(pCrwBuffer);
}
#else
//??????????????????????????????????????????????????????????????????????????????
void TablePtn::vInsertIndexEntries(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
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
#endif

/*----------------------------------------------------------------------------*/
/* insert entries into given index for all rows in this partition             */
/*----------------------------------------------------------------------------*/
#ifdef __CRWTABLEPTN
bool TablePtn::bPopulateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  bool b = m_pCrwTablePtn->bPopulateIndex(iIndexNo);

  return b;
}
#else
//??????????????????????????????????????????????????????????????????????????????
bool TablePtn::bPopulateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  IndexTree* pIndexTree = this->pIndexTree(iIndexNo);
  
  CrwBuffer* pCrwBuffer = m_pRootBuffer;
  
  while (pCrwBuffer) {
    pIndexTree->vInsertEntries(pCrwBuffer);
    pCrwBuffer = pCrwBuffer->pNextCrwBuffer();
  }

  return true;
}
#endif

/*----------------------------------------------------------------------------*/
/* get the root row buffer                                                    */
/*----------------------------------------------------------------------------*/
#ifdef __CRWTABLEPTN
CrwBuffer* TablePtn::pRootCrwBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  CrwBuffer* pRootBuffer = m_pCrwTablePtn->pRootCrwBuffer();

  return pRootBuffer;
}
#else
//??????????????????????????????????????????????????????????????????????????????
CrwBuffer* TablePtn::pRootCrwBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_pRootBuffer;
}
#endif

/*----------------------------------------------------------------------------*/
/* remove index                                                               */
/*----------------------------------------------------------------------------*/
#ifdef __CRWTABLEPTN
void TablePtn::vRemoveIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_pCrwTablePtn->vRemoveIndex(iIndexNo);
}
#else
//??????????????????????????????????????????????????????????????????????????????
void TablePtn::vRemoveIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  IndexTree* pIndexTree = this->pIndexTree(iIndexNo);
  
  // remove the index from the IdxDataVec
  m_pIndexTreeVec->vRemove(pIndexTree);
  delete pIndexTree;
}
#endif

/*----------------------------------------------------------------------------*/
/* truncate the table data                                                    */
/*----------------------------------------------------------------------------*/
#ifdef __CRWTABLEPTN
void TablePtn::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_pCrwTablePtn->vTruncate();
}
#else
//??????????????????????????????????????????????????????????????????????????????
void TablePtn::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  // first truncate all indexes for this table ptn
  for (i8 iIdxTre = 0; iIdxTre < m_pIndexTreeVec->nItems(); iIdxTre++) {
    IndexTree* pIndexTree = m_pIndexTreeVec->pGet(iIdxTre);
    pIndexTree->vTruncate();
  }
  
  if (m_pRootBuffer)
    m_pRootBuffer->vTruncate();
  
  m_pRootBuffer    = nullptr;
  m_pCurrentBuffer = nullptr;
}
#endif

/*----------------------------------------------------------------------------*/
/* truncate the index data for given index                                    */
/*----------------------------------------------------------------------------*/
#ifdef __CRWTABLEPTN
void TablePtn::vTruncateIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_pCrwTablePtn->vTruncateIndex(pIndex);
}
#else
//??????????????????????????????????????????????????????????????????????????????
void TablePtn::vTruncateIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
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
}
#endif

/*----------------------------------------------------------------------------*/
/* verify the table data of this partition                                    */
/*----------------------------------------------------------------------------*/
#ifdef __CRWTABLEPTN
bool TablePtn::bVerifyTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  

  bool bVerify = m_pCrwTablePtn->bVerifyTable();
  
  return bVerify;
}
#else
//??????????????????????????????????????????????????????????????????????????????
bool TablePtn::bVerifyTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  CrwBuffer* pCrwBuffer = m_pRootBuffer;
  bool bVerify = true;
  
  while (pCrwBuffer) {
    bVerify = pCrwBuffer->bVerify();
    if (! bVerify )
      break;
    pCrwBuffer = pCrwBuffer->pNextCrwBuffer();
  }
  
  return bVerify;
}
#endif

/*----------------------------------------------------------------------------*/
/* visualize the table data of this partition                                 */
/*----------------------------------------------------------------------------*/
#ifdef __CRWTABLEPTN
void TablePtn::vVisualizeTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_pCrwTablePtn->vVisualizeTable();
}
#else
//??????????????????????????????????????????????????????????????????????????????
void TablePtn::vVisualizeTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  CrwBuffer* pCrwBuffer = m_pRootBuffer;
  
  while (pCrwBuffer) {
    // print the decile of the this buffer
    vTrace("%lld"_Stz, pCrwBuffer->iDecile());
    pCrwBuffer = pCrwBuffer->pNextCrwBuffer();
  }
  vTrace("\n"_Stz);
}
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
