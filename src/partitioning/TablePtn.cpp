/*============================================================================*/
/* TablePtn.cpp -- Table Partition                                            */
/*============================================================================*/

#include "Partitioning.h"



/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TablePtn* Partitioning::_pTablePtn(void* aAddr)
{
  TablePtn* pTablePtn = reinterpret_cast<TablePtn*>(aAddr);
  
  RASSERT_TYPE(pTablePtn,CN_TABLEPTN);
  
  return pTablePtn;
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TablePtn::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_TABLEPTN);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TablePtn::operator delete(void* pBufferAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pBufferAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TablePtn::TablePtn(Table* pTable, tPN tPtnNo)
: Object(CN_TABLEPTN)
, m_okLock(LK_PARTITION,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_pTable = pTable;
  m_tPtnNo = tPtnNo;

  m_bLowMemLoad = false;
  
  m_pRootCrwBuffer  = nullptr;
  m_pCurrentCrwBuffer = nullptr;
  
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
  
  if (! m_pRootCrwBuffer)
    RASSERT(pNewCurrentCrwBuffer());                  //! this needs to be fixed

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allot from latest buffer
  
  RowNub* pRowNub = m_pCurrentCrwBuffer->m_oRowTray.pNub();
  i8 i_Head = pRowNub->iAllotHead(jCrwSize);
  if (C_UNKNOWN != i_Head) {
    aCrw = (Crw*)pRowNub->aGetHead(i_Head);
    xRowRef.vBuffer(m_pCurrentCrwBuffer->iBufferNo());
    xRowRef.vItem(i_Head);
    return aCrw;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allot from a new buffer

  RASSERT(pNewCurrentCrwBuffer());                    //! this needs to be fixed
  pRowNub = m_pCurrentCrwBuffer->m_oRowTray.pNub();
  i_Head = pRowNub->iAllotHead(jCrwSize);
  aCrw = (Crw*)pRowNub->aGetHead(i_Head);
  RASSERT(aCrw);
  
  xRowRef.vBuffer(m_pCurrentCrwBuffer->iBufferNo());
  xRowRef.vItem(i_Head);
  
  return aCrw;
}

/*----------------------------------------------------------------------------*/
/* append a row buffer into the TablePtn                                      */
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
  if (m_pRootCrwBuffer == nullptr) {
    ASSERT( ! m_pCurrentCrwBuffer )
    m_pRootCrwBuffer = pCrwBuffer;
  }
  else {
    pCrwBuffer->vNextBuffer(m_pCurrentCrwBuffer->pNextBuffer());
    pCrwBuffer->vPrevBuffer(m_pCurrentCrwBuffer);
    m_pCurrentCrwBuffer->vNextBuffer(pCrwBuffer);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the root buffer
  m_pCurrentCrwBuffer = pCrwBuffer;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now insert entries from the buffer into the indexes
  vInsertIndexEntries(pCrwBuffer);
  
}

/*----------------------------------------------------------------------------*/
/* append a row to the row buffer into the TablePtn                           */
/*----------------------------------------------------------------------------*/
void TablePtn::vAppendRow(RowCompactor* pRowCompactor, RowSingle* pSrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.IndexTree.m_nAppendBuffers++;
#endif
  
  RASSERT(m_okLock.bIsLocked());
  //m_okLock.vGrabWrite();
  
  if (! m_pRootCrwBuffer)
    RASSERT(pNewCurrentCrwBuffer());                  //! this needs to be fixed
  
  m_pCurrentCrwBuffer->vAppendOpen(pRowCompactor, false);
  bool bAppended = m_pCurrentCrwBuffer->bAppendSrw(pSrw);
  
  if (! bAppended) {
    RASSERT(pNewCurrentCrwBuffer());                  //! this needs to be fixed

    m_pCurrentCrwBuffer->vAppendOpen(pRowCompactor, false);
    bAppended = m_pCurrentCrwBuffer->bAppendSrw(pSrw);
    RASSERT(bAppended);
  }
  
  m_pCurrentCrwBuffer->vAppendClose();
  
  // get the row from the buffer
  Rrf xRowRef;
  i8 iRow = m_pCurrentCrwBuffer->nRowCount() - 1;
  xRowRef.vItem(iRow);
  
  // get no of indexes of the table
  i8 nIndexes = m_pIndexTreeVec->nItems();
  
  // iterate all indexes of the table
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    
    // get index
    IndexTree* pIndexTree = m_pIndexTreeVec->pGet(i_Index);
    
    // insert entry into other indexes
    bool bResult = pIndexTree->bInsertEntry(m_pCurrentCrwBuffer,iRow,xRowRef);
    RASSERT(bResult);
  }
  
  //m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* get the row buffers used                                                   */
/*----------------------------------------------------------------------------*/
i8 TablePtn::nCrwBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  i8 nBufferCount = 0;
  
  if (m_pRootCrwBuffer)
    m_pRootCrwBuffer->vBufferCount(nBufferCount);
  
  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* delete an existing row                                                     */
/*----------------------------------------------------------------------------*/
void TablePtn::vDeleteRow(IndexTree* pIndexTree, EntBuffer* pEntBuffer, Ent* aEnt)
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
    if (m_pCurrentCrwBuffer == pCrwBuffer)
      m_pCurrentCrwBuffer = m_pCurrentCrwBuffer->pPrevCrwBuffer();
    
    // is it the root buffer ?
    if (m_pRootCrwBuffer == pCrwBuffer) {
      m_pRootCrwBuffer = m_pRootCrwBuffer->pNextCrwBuffer();
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
  
  CrwBuffer* pCrwBuffer = m_pRootCrwBuffer;
    
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
  
  if (m_pRootCrwBuffer == nullptr) {
    ASSERT( ! m_pCurrentCrwBuffer );
    
    m_pRootCrwBuffer = new CrwBuffer;
    m_pRootCrwBuffer->vTable(m_pTable);
    m_pRootCrwBuffer->vPtnNo(m_tPtnNo);
    m_pRootCrwBuffer->vIsPermanent(true);
    
    m_pCurrentCrwBuffer = m_pRootCrwBuffer;
    
    return m_pCurrentCrwBuffer;
  }
  
  CrwBuffer* pCrwBuffer = new CrwBuffer;
  pCrwBuffer->vTable(m_pTable);
  pCrwBuffer->vPtnNo(m_tPtnNo);
  pCrwBuffer->vIsPermanent(true);

  pCrwBuffer->vNextBuffer(m_pCurrentCrwBuffer->pNextBuffer());
  pCrwBuffer->vPrevBuffer(m_pCurrentCrwBuffer);
  m_pCurrentCrwBuffer->vNextBuffer(pCrwBuffer);
  
  m_pCurrentCrwBuffer = pCrwBuffer;
    
  return m_pCurrentCrwBuffer;
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
  
  CrwBuffer* pCrwBuffer = m_pRootCrwBuffer;
  
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
  
  if (! m_pRootCrwBuffer)
    return false;
  
  m_okLock.vGrabWrite();
  
  i8 iAge = m_pRootCrwBuffer->iAgeSec();
  
  //  if (iAge <= 1) {
  //    vTrace("Purge: age is %llds\n"_Stz,iMaxAge);
  //  }
  
  if (iAge > iMaxAge) {
    iMaxAge = iAge;
    //    vTrace("Purge: max age now %llds\n"_Stz,iMaxAge);
  }
  
  i8 iLengthList = m_pRootCrwBuffer->iLengthList();
  if (iLengthList > nMaxBuffers) {
    nMaxBuffers = iLengthList;
    //vTrace("Purge: max buffer chain now %lld buffers\n"_Stz,nMaxBuffers);
  }
  
  
  if (iAge <= iOlderThanSeconds) {
    m_okLock.vDropWrite();
    return false;
  }
  
  // is currentBuffer same as rootBuffer ?
  if (m_pCurrentCrwBuffer == m_pRootCrwBuffer)
    m_pCurrentCrwBuffer = nullptr;
  
  CrwBuffer* pCrwBuffer = m_pRootCrwBuffer;
  
  // get next entry buffer
  m_pRootCrwBuffer = m_pRootCrwBuffer->pNextCrwBuffer();
  
  // set prev pointer to null
  if (m_pRootCrwBuffer) {
    ASSERT(pCrwBuffer ==  m_pRootCrwBuffer->pPrevBuffer());
    m_pRootCrwBuffer->vPrevBuffer(nullptr);
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
  
  return m_pRootCrwBuffer;
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
  
  if (m_pRootCrwBuffer)
    m_pRootCrwBuffer->vTruncate();
  
  m_pRootCrwBuffer    = nullptr;
  m_pCurrentCrwBuffer = nullptr;
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
  
  CrwBuffer* pCrwBuffer = m_pRootCrwBuffer;
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
  
  CrwBuffer* pCrwBuffer = m_pRootCrwBuffer;
  bool bVerify = true;
  
  while (pCrwBuffer) {
    // print the decile of the this buffer
    vTrace("%lld"_Stz, pCrwBuffer->iDecile());
    pCrwBuffer = pCrwBuffer->pNextCrwBuffer();
  }
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(TablePtn);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
