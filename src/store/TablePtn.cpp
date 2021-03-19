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
#include "ReadApi.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"

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
  
  m_pRootBuffer  = nullptr;
  m_pCurrentBuffer = nullptr;
  
  m_pIndexTreeVec = new IndexTreeVec;
  
  m_pReadApi = new ReadApi;
  
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
  delete m_pIndexTreeVec;
  
  // delete the readApi
  delete m_pReadApi;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump partition                                                             */
/*----------------------------------------------------------------------------*/
void TablePtn::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  UNUSED(eClass);
  UNUSED(aTab);

  TablePtn* pTablePtn = _pTablePtn(pObject);
  
  vTrace("m_tPtnNo %lld, DataVec %p\n"_Stz,
         pTablePtn->m_tPtnNo, pTablePtn->m_pIndexTreeVec);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add index                                                                  */
/*----------------------------------------------------------------------------*/
void TablePtn::vAddIndex(IndexTree* pIndexTree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(m_pIndexTreeVec->nItems() < C_MAX_INDEXES);
  m_pIndexTreeVec->vAdd(pIndexTree);
}

/*----------------------------------------------------------------------------*/
/* allot a row (from a table defined with a fixed row size)                   */
/*----------------------------------------------------------------------------*/
Crw* TablePtn::aAllotFixedSizeCrw(Rrf& xRowRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  Crw* aCrw = nullptr;
  
  StreamDef* pStreamDef = m_pTable->pStreamDef();
  i8 jCrwSize = pStreamDef->m_nFixedSized;
  RASSERT(jCrwSize > 0);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // new the first buffer?
  
  if (! m_pRootBuffer)
    RASSERT(pNewCurrentCrwBuffer());                  //! this needs to be fixed

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allot from latest buffer
  
  RowNub* pRowNub = m_pCurrentBuffer->pRowTray()->pNub();
  i8 i_Head = pRowNub->iAllotHead(jCrwSize);
  if (C_UNKNOWN != i_Head) {
    aCrw = (Crw*)pRowNub->aGetHead(i_Head);
    xRowRef.vBuffer(m_pCurrentBuffer->iBufferNo());
    xRowRef.vItem(i_Head);
    return aCrw;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allot from a new buffer

  RASSERT(pNewCurrentCrwBuffer());                    //! this needs to be fixed
  pRowNub = m_pCurrentBuffer->pRowTray()->pNub();
  i_Head = pRowNub->iAllotHead(jCrwSize);
  aCrw = (Crw*)pRowNub->aGetHead(i_Head);
  RASSERT(aCrw);
  
  xRowRef.vBuffer(m_pCurrentBuffer->iBufferNo());
  xRowRef.vItem(i_Head);
  
  return aCrw;
}

/*----------------------------------------------------------------------------*/
/* append a crw buffer into the TablePtn                                      */
/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
/* append a row buffer into the TablePtn                                      */
/*----------------------------------------------------------------------------*/
void TablePtn::vAppendBuffer(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* get the number of buffers in this partition                                */
/*----------------------------------------------------------------------------*/
i8 TablePtn::nBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  i8 nBufferCount = 0;
  
  if (m_pRootBuffer)
    nBufferCount = m_pRootBuffer->nLinkedBufferCount();
  
  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* delete an existing row                                                     */
/*----------------------------------------------------------------------------*/
void TablePtn::vDeleteCrw(IndexTree* pIndexTree, EntBuffer* pEntBuffer, Ent* aEnt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete the crw
  bool bBufferEmpty = CrwBuffer::bDeleteRefdCrw(aEnt->rxRef());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is the crw buffer empty ?
  if ( bBufferEmpty) {
    
    // get the referenced buffer
    CrwBuffer* pCrwBuffer = CrwBuffer::pRefdCrwBuffer(aEnt->rxRef());
    
    // unlink the buffer
    pCrwBuffer->vUnLink();
    
    // is it the currentBuffer ?
    if (m_pCurrentBuffer == pCrwBuffer)
      m_pCurrentBuffer = m_pCurrentBuffer->pPrevCrwBuffer();
    
    // is it the root buffer ?
    if (m_pRootBuffer == pCrwBuffer) {
      m_pRootBuffer = m_pRootBuffer->pNextCrwBuffer();
    }
    
    // now delete the buffer
    delete pCrwBuffer;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now purge the entry
  pIndexTree->vDelete(pEntBuffer, aEnt);
  
}

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
/* dump the table                                                             */
/*----------------------------------------------------------------------------*/
void TablePtn::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  CrwBuffer* pCrwBuffer = m_pRootBuffer;
    
  while (pCrwBuffer) {
    pCrwBuffer->vShow();
    pCrwBuffer = pCrwBuffer->pNextCrwBuffer();
  }

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
/* get IndexTree                                                              */
/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
/* get index data vector                                                      */
/*----------------------------------------------------------------------------*/
IndexTreeVec* TablePtn::pIndexTreeVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_pIndexTreeVec;
}

/*----------------------------------------------------------------------------*/
/* insert entries into all indexes for all rows in given buffer               */
/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
/* is the table partition locked?                                             */
/*----------------------------------------------------------------------------*/
bool TablePtn::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_okLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* new a CrwBuffer and make it the current                                    */
/*----------------------------------------------------------------------------*/
CrwBuffer* TablePtn::pNewCurrentCrwBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.IndexTree.m_nAppendBuffers++;
#endif
  
  RASSERT(m_okLock.bIsLocked());
  
  if (m_pRootBuffer == nullptr) {
    ASSERT( ! m_pCurrentBuffer );
    
    m_pRootBuffer = new CrwBuffer;
    m_pRootBuffer->vTable(m_pTable);
    m_pRootBuffer->vPtnNo(m_tPtnNo);
    m_pRootBuffer->vIsPermanent(true);
    
    m_pCurrentBuffer = m_pRootBuffer;
    
    return m_pCurrentBuffer;
  }
  
  CrwBuffer* pCrwBuffer = new CrwBuffer;
  pCrwBuffer->vTable(m_pTable);
  pCrwBuffer->vPtnNo(m_tPtnNo);
  pCrwBuffer->vIsPermanent(true);

  pCrwBuffer->vNextLinkedBuffer(m_pCurrentBuffer->pNextLinkedBuffer());
  pCrwBuffer->vPrevLinkedBuffer(m_pCurrentBuffer);
  m_pCurrentBuffer->vNextLinkedBuffer(pCrwBuffer);
  
  m_pCurrentBuffer = pCrwBuffer;
    
  return m_pCurrentBuffer;
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
/* insert entries into given index for all rows in this partition             */
/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
/* purge the table buffers                                                    */
/*----------------------------------------------------------------------------*/
bool TablePtn::bPurge(i8 iOlderThanSeconds)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  static i8 iMaxAge = 0;
  static i8 nMaxBuffers = 0;
  
  if (! m_pRootBuffer)
    return false;
  
  m_okLock.vGrabWrite();
  
  i8 iAge = m_pRootBuffer->iAgeSec();
  
  //  if (iAge <= 1) {
  //    vTrace("Purge: age is %llds\n"_Stz,iMaxAge);
  //  }
  
  if (iAge > iMaxAge) {
    iMaxAge = iAge;
    //    vTrace("Purge: max age now %llds\n"_Stz,iMaxAge);
  }
  
  i8 iLengthList = m_pRootBuffer->iLengthList();
  if (iLengthList > nMaxBuffers) {
    nMaxBuffers = iLengthList;
    //vTrace("Purge: max buffer chain now %lld buffers\n"_Stz,nMaxBuffers);
  }
  
  
  if (iAge <= iOlderThanSeconds) {
    m_okLock.vDropWrite();
    return false;
  }
  
  // is currentBuffer same as rootBuffer ?
  if (m_pCurrentBuffer == m_pRootBuffer)
    m_pCurrentBuffer = nullptr;
  
  CrwBuffer* pCrwBuffer = m_pRootBuffer;
  
  // get next entry buffer
  m_pRootBuffer = m_pRootBuffer->pNextCrwBuffer();
  
  // set prev pointer to null
  if (m_pRootBuffer) {
    ASSERT(pCrwBuffer ==  m_pRootBuffer->pPrevLinkedBuffer());
    m_pRootBuffer->vPrevLinkedBuffer(nullptr);
  }
  
  m_okLock.vDropWrite();
  
  // delete the root buffer
  delete pCrwBuffer;

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.IndexTree.m_nBuffersPurged++;
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get read api                                                               */
/*----------------------------------------------------------------------------*/
ReadApi* TablePtn::pReadApi(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_pReadApi;
}

/*----------------------------------------------------------------------------*/
/* get the root row buffer                                                    */
/*----------------------------------------------------------------------------*/
CrwBuffer* TablePtn::pRootCrwBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_pRootBuffer;
}

/*----------------------------------------------------------------------------*/
/* take a read-consistent snaphot of buffer                                   */
/*----------------------------------------------------------------------------*/
CrwBuffer* TablePtn::pSnapshot(CrwBuffer* pCrwBufferSrc, t8 tPerspective)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  CrwBuffer* pCrwBuffer = new CrwBuffer;
  
  pCrwBuffer->vTable(pTable());
  pCrwBuffer->vPtnNo(tPtnNo());
  pCrwBuffer->vIsPermanent(false);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  i8 nCrw = pCrwBufferSrc->nRowCount();
  for (i8 i_Crw = 0 ; i_Crw < nCrw; i_Crw++) {
    
    Crw* aCrw = pCrwBufferSrc->aCrw(i_Crw);
    Rrf xwRef;
    xwRef.vBuffer(pCrwBufferSrc->iBufferNo());
    xwRef.vItem(i_Crw);

    Crw* aCrw_snapshot = m_pReadApi->aGetCrwVersion(xwRef,aCrw,tPerspective);
    
    if (! aCrw_snapshot)
      continue;
    
    pCrwBuffer->bAppendCrw(aCrw_snapshot);
  }
  
  return pCrwBuffer;
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
/* remove index                                                               */
/*----------------------------------------------------------------------------*/
void TablePtn::vRemoveIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  IndexTree* pIndexTree = this->pIndexTree(iIndexNo);
  
  // remove the index from the IdxDataVec
  m_pIndexTreeVec->vRemove(pIndexTree);
  delete pIndexTree;
}

/*----------------------------------------------------------------------------*/
/* truncate the table data                                                    */
/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
/* truncate the index data for given index                                    */
/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
/* verify the table data of this partition                                    */
/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
/* visualize the table data of this partition                                 */
/*----------------------------------------------------------------------------*/
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
