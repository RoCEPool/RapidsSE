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
TablePtn::TablePtn(Table* pTable)
: Object(CN_TABLEPTN)
, m_okLock(LK_PARTITION,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_iPartitionId = -1;
  m_pTable = pTable;
  
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
  TablePtn* pPartition = _pTablePtn(pObject);
  
  vTrace("m_iPartitionId %lld, DataVec %p\n"_Stz,
         pPartition->m_iPartitionId, pPartition->m_pIndexTreeVec);
}

/*----------------------------------------------------------------------------*/
/* convert partition number to node number                                    */
/*----------------------------------------------------------------------------*/
i8 TablePtn::iPartitionToNode(i8 iPartition)
{
  i8 iNode = iPartition % AM::nDefinedNodes();
  
  return iNode;
}

/*----------------------------------------------------------------------------*/
/* convert partition number to slot number                                    */
/*----------------------------------------------------------------------------*/
i8 TablePtn::iPartitionToSlot(i8 iPartition)
{
  i8 iSlot = iPartition / AM::nDefinedNodes();
  
  return iSlot;
}

/*----------------------------------------------------------------------------*/
/* convert slot number to partition number                                    */
/*----------------------------------------------------------------------------*/
i8 TablePtn::iSlotToPartition(i8 iSlot)
{
  i8 iPartition =
  (iSlot * AM::nDefinedNodes()) + AM::iThisNodeNo();
  
  return iPartition;
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
Crw* TablePtn::aAllotFixedSizeCrw(Ref& xRowRef)
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
  Ref xRowRef;
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
void TablePtn::vDeleteRow(Crw* aCrw_exists, Ent* aEnt_found)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  StreamDef* pStreamDef = m_pTable->pStreamDef();
  
  HV_Row hvRow_exists(pStreamDef,C_ALL);
  CrwBuffer::vToRow(hvRow_exists,aCrw_exists);
  SP_Record spRec;
  TRACE_UPSERT( vTrace("Exists   : %s\n"_Stz,spRec.sz(hvRow_exists)); )
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
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
/* WIP: insdel entries from a row buffer                                      */
/*----------------------------------------------------------------------------*/
void TablePtn::vInsdelBuffer(CrwBuffer* pCrwBuffer_upsert, t8 tPerspective)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  EntWrk xEntWrk;
  
  SP_Record spRecord;
  
  StreamDef* pStreamDef = pCrwBuffer_upsert->pStreamDef();
  
  IndexTree* pIndexTree = m_pIndexTreeVec->pGet(0);
  IndexDef* pIndexDef = pIndexTree->pIndex()->pIndexDef();
  
  HV_Row hvRow_upsert(pStreamDef,C_ALL);
  
  Encoding* pEncoding_upsert = new Encoding(pStreamDef);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iteration of rows from the incoming crw buffer
  
  i8 nRows = pCrwBuffer_upsert->nRowCount();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // get upsert row, re-encode it for this table, and extract key
    
    Crw* aCrw_upsert = pCrwBuffer_upsert->aCrw(i_Row);
    
    pCrwBuffer_upsert->vGetRow(hvRow_upsert,i_Row);
    TRACE_UPSERT( vTrace("Upsert %lld : %s\n"_Stz,i_Row,spRecord.sz(hvRow_upsert)); )
    
    pEncoding_upsert->vEncodeSrw(hvRow_upsert);

    xEntWrk.vElmSeqSize(0);
    pIndexTree->vExtractKey(&xEntWrk,pCrwBuffer_upsert,i_Row);
    SP_Ent spEnt;
    TRACE_UPSERT( vTrace("EntWrk : %s\n"_Stz,spEnt.sz(xEntWrk.aEnt(),pIndexDef)); )

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // possible insert (can we find the row using primary key)
    
    EntBuffer* pEntH0Buf = pIndexTree->pPositionH0Buffer(xEntWrk.aKey());
    if (! pEntH0Buf) {
      TRACE_UPSERT( vTrace("No pEntBuf @%lld; inserting...\n"_Stz,(i8)__LINE__); )
      vUpsertRow_insert(aCrw_upsert,&xEntWrk,pEncoding_upsert,tPerspective);
      continue;
    }
    
    Ent* aEnt_prior = pEntH0Buf->aPositionPriorEntry(xEntWrk.aKey());
    if (! aEnt_prior) {
      TRACE_UPSERT( vTrace("No aEnt_prior @%lld; inserting...\n"_Stz,(i8)__LINE__); )
      vUpsertRow_insert(aCrw_upsert,&xEntWrk,pEncoding_upsert,tPerspective);
      continue;
    }
    
    SP_Key spKey;
    TRACE_UPSERT( vTrace("Key Prior: %s\n"_Stz,spKey.sz(aEnt_prior->aKey(),pIndexDef)); )
    vDebugPoint();
    
    Ent* aEnt_found = pEntH0Buf->aNextEntry(aEnt_prior,0);
    if (! aEnt_found) {
      TRACE_UPSERT( vTrace("No aEnt_found @%lld; inserting...\n"_Stz,(i8)__LINE__); )
      vUpsertRow_insert(aCrw_upsert,&xEntWrk,pEncoding_upsert,tPerspective);
     continue;
    }
    
    TRACE_UPSERT( vTrace("Key Found: %s\n"_Stz,spKey.sz(aEnt_found->aKey(),pIndexDef)); )
    
    if (KC_SAME != Key::eCompare(aEnt_found->aKey(),xEntWrk.aKey())) {
      TRACE_UPSERT( vTrace("NOT SAME @%lld; inserting...\n"_Stz,(i8)__LINE__); )
      vUpsertRow_insert(aCrw_upsert,&xEntWrk,pEncoding_upsert,tPerspective);
      continue;
    }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // upsert (we have a crw matching the key)
    
    SP_Ent spRef;
    TRACE_UPSERT( vTrace("SAME : %s\n"_Stz,spRef.sz(aEnt_found,pIndexDef)); )
    
    Crw* aCrw_exists = CrwBuffer::pRefdCrw(aEnt_found->rxRef());
    if (! aCrw_exists) {
      TRACE_UPSERT( vTrace("No aCrw @%lld; skipping...\n"_Stz,(i8)__LINE__); )
      continue;
    }
    
    vUpsertRow_update(aCrw_exists,aEnt_found,pEncoding_upsert,tPerspective);
    
    vDebugPoint();

  } // for i_Row
  
  delete pEncoding_upsert;
}

/*----------------------------------------------------------------------------*/
/* WIP: insdel entries from a row buffer                                      */
/*----------------------------------------------------------------------------*/
void TablePtn::vInsdelBuffer(RsbBuffer* pRsbBuffer, t8 tPerspective)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
//  SP_DataBuffer spDataBuffer;
//
//  vTrace("vInsdelBuffer() :- [[RsbBuffer %s ; Rsbs=%lld]]\n"_Stz,
//         spDataBuffer.sz(pRsbBuffer),
//         pRsbBuffer->nRsb());
  
  //pRsbBuffer->vPrintRows();

  EntWrk xEntWrk;
  
  IndexTree* pIndexTree = m_pIndexTreeVec->pGet(0);
  IndexDef* pIndexDef = pIndexTree->pIndex()->pIndexDef();
  
  StreamDef* pStreamDef = pIndexTree->pIndex()->pTable()->pStreamDef();
  Encoding* pEncoding_upsert = new Encoding(pStreamDef);
  
  RowSlab* pRowSlab = pRsbBuffer->pRowSlab();
  SP_RowPix spRowPix(pRowSlab->nCols(),pRowSlab->axaColPix());
  
  i8 nRows = pRowSlab->nRows();
  SP_Ent spEnt;
  
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // get upsert row, re-encode it for this table, and extract key
    
    Rsb* aRsb_upsert = pRowSlab->aRsbAdr(i_Row);
    pEncoding_upsert->vEncodeRsb(aRsb_upsert);
    
    Stz* szRsb = spRowPix.sz(aRsb_upsert);
    TRACE_UPSERT( vTrace("Upsert(P%lld) %lld: %s\n"_Stz,m_iPartitionId,i_Row,szRsb); )
    
    xEntWrk.vElmSeqSize(0);
    pIndexTree->vExtractKey(&xEntWrk,aRsb_upsert);
    TRACE_UPSERT( vTrace("EntWrk : %s\n"_Stz,spEnt.sz(xEntWrk.aEnt(),pIndexDef)); )

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // possible insert (can we find the row using primary key)
    EntBuffer* pEntH0Buf = pIndexTree->pPositionH0Buffer(xEntWrk.aKey());
    if (! pEntH0Buf) {
      TRACE_UPSERT( vTrace("No pEntBuf @%lld; inserting...\n"_Stz,(i8)__LINE__); )
      vUpsertRow_insert(pRowSlab,i_Row,&xEntWrk,pEncoding_upsert,tPerspective);
      continue;
    }

    Ent* aEnt_prior = pEntH0Buf->aPositionPriorEntry(xEntWrk.aKey());
    if (! aEnt_prior) {
      TRACE_UPSERT( vTrace("No aEnt_prior @%lld; inserting...\n"_Stz,(i8)__LINE__); )
      vUpsertRow_insert(pRowSlab,i_Row, &xEntWrk,pEncoding_upsert,tPerspective);
      continue;
    }
    
    SP_Key spKey;
    TRACE_UPSERT( vTrace("Key Prior: %s\n"_Stz,spKey.sz(aEnt_prior->aKey(),pIndexDef)); )
    vDebugPoint();
    
    Ent* aEnt_found = pEntH0Buf->aNextEntry(aEnt_prior,0);
    if (! aEnt_found) {
      TRACE_UPSERT( vTrace("No aEnt_found @%lld; inserting...\n"_Stz,(i8)__LINE__); )
      vUpsertRow_insert(pRowSlab,i_Row,&xEntWrk,pEncoding_upsert,tPerspective);
      continue;
    }
    
    TRACE_UPSERT( vTrace("Key Found: %s\n"_Stz,spKey.sz(aEnt_found->aKey(),pIndexDef)); )
    
    if (KC_SAME != Key::eCompare(aEnt_found->aKey(),xEntWrk.aKey())) {
      TRACE_UPSERT( vTrace("NOT SAME @%lld; inserting...\n"_Stz,(i8)__LINE__); )
      vUpsertRow_insert(pRowSlab,i_Row,&xEntWrk,pEncoding_upsert,tPerspective);
      continue;
    }
   
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // upsert (we have a crw matching the key)
  
    SP_Ent spRef;
    TRACE_UPSERT( vTrace("SAME : %s\n"_Stz,spRef.sz(aEnt_found,pIndexDef)); )
    
    Crw* aCrw_exists = CrwBuffer::pRefdCrw(aEnt_found->rxRef());
    if (! aCrw_exists) {
      TRACE_UPSERT( vTrace("No aCrw @%lld; skipping...\n"_Stz,(i8)__LINE__); )
      continue;
    }
    
    vDeleteRow(aCrw_exists,aEnt_found);
    
    vDebugPoint();
  }
  
  // delete the encoding
  delete pEncoding_upsert;
}

/*----------------------------------------------------------------------------*/
/* insert entries into all indexes for given row                              */
/*----------------------------------------------------------------------------*/
void TablePtn::vInsertIndexEntries(Crw* aCrw, Ref& xRowRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  // get no of indexes of the table
  i8 nIndexes = m_pIndexTreeVec->nItems();
  
  // iterate all indexes of the table
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    
    // get index
    IndexTree* pIndexTree = m_pIndexTreeVec->pGet(i_Index);
    
    // insert entry from row into the index
    pIndexTree->bInsertEntry(aCrw,xRowRef);
  }
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
    m_pRootCrwBuffer->vPartitionNo(m_iPartitionId);
    m_pRootCrwBuffer->vIsPermanent(true);
    
    m_pCurrentCrwBuffer = m_pRootCrwBuffer;
    
    return m_pCurrentCrwBuffer;
  }
  
  CrwBuffer* pCrwBuffer = new CrwBuffer;
  pCrwBuffer->vTable(m_pTable);
  pCrwBuffer->vPartitionNo(m_iPartitionId);
  pCrwBuffer->vIsPermanent(true);

  pCrwBuffer->vNextBuffer(m_pCurrentCrwBuffer->pNextBuffer());
  pCrwBuffer->vPrevBuffer(m_pCurrentCrwBuffer);
  m_pCurrentCrwBuffer->vNextBuffer(pCrwBuffer);
  
  m_pCurrentCrwBuffer = pCrwBuffer;
    
  return m_pCurrentCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* get this partitions partitionId                                            */
/*----------------------------------------------------------------------------*/
i8 TablePtn::iPartitionId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_iPartitionId;
}

/*----------------------------------------------------------------------------*/
/* set this partitions partitionId                                            */
/*----------------------------------------------------------------------------*/
void TablePtn::vPartitionId(i8 iPartitionId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_iPartitionId = iPartitionId;
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
/* publish the index buffers                                                  */
/*----------------------------------------------------------------------------*/
void TablePtn::vPublish(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  if ( ! m_pRootCrwBuffer)
    return;
  
  m_okLock.vGrabWrite();
  
  // is there a current buffer ?
  if ( ! m_pCurrentCrwBuffer) {
    m_okLock.vDropWrite();
    return;
  }
  
  StreamDef* pStreamDef = m_pCurrentCrwBuffer->pStreamDef();
  HV_Rec hvRec(pStreamDef, C_ALL);
  
  // create a row for retrieving the row
  HV_Row hRow(pStreamDef, C_ALL);
  
  // publish the current buffer (last buffer that was added)
  i8 nRowCount = m_pCurrentCrwBuffer->nRowCount();
  for (i8 i_Row = 0; i_Row < nRowCount; i_Row++) {
    
    // get the inflated compact row
    m_pCurrentCrwBuffer->vGetRow(hRow, i_Row);
    
    // append the row to the record
    hvRec->vAppend(hRow);
    
    vTrace("row: (%s)\n"_Stz,hvRec->szRecStr());
  }
  
  vTrace("Partition%lld: published %lld rows\n"_Stz,m_iPartitionId,
         m_pCurrentCrwBuffer->nRowCount());
  
  m_okLock.vDropWrite();
  
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
  pCrwBuffer->vPartitionNo(iPartitionId());
  pCrwBuffer->vIsPermanent(false);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  i8 nCrw = pCrwBufferSrc->nRowCount();
  for (i8 i_Crw = 0 ; i_Crw < nCrw; i_Crw++) {
    
    Crw* aCrw = pCrwBufferSrc->aCrw(i_Crw);
    Ref xwRef;
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
/* WIP: upsert entries from a row buffer                                      */
/*----------------------------------------------------------------------------*/
void TablePtn::vUpsertBuffer(CrwBuffer* pCrwBuffer_upsert, t8 tPerspective)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  EntWrk xEntWrk;
  
  SP_Record spRecord;
  
  StreamDef* pStreamDef = pCrwBuffer_upsert->pStreamDef();
  
  IndexTree* pIndexTree = m_pIndexTreeVec->pGet(0);
  IndexDef* pIndexDef = pIndexTree->pIndex()->pIndexDef();
  
  HV_Row hvRow_upsert(pStreamDef,C_ALL);
  
  Encoding* pEncoding_upsert = new Encoding(pStreamDef);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iteration of rows from the incoming crw buffer
  
  i8 nRows = pCrwBuffer_upsert->nRowCount();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // get upsert row, re-encode it for this table, and extract key
    
    Crw* aCrw_upsert = pCrwBuffer_upsert->aCrw(i_Row);
    
    pCrwBuffer_upsert->vGetRow(hvRow_upsert,i_Row);
    TRACE_UPSERT( vTrace("Upsert %lld : %s\n"_Stz,i_Row,spRecord.sz(hvRow_upsert)); )
    
    pEncoding_upsert->vEncodeSrw(hvRow_upsert);

    xEntWrk.vElmSeqSize(0);
    pIndexTree->vExtractKey(&xEntWrk,pCrwBuffer_upsert,i_Row);
    SP_Ent spEnt;
    TRACE_UPSERT( vTrace("EntWrk : %s\n"_Stz,spEnt.sz(xEntWrk.aEnt(),pIndexDef)); )

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // possible insert (can we find the row using primary key)
    
    EntBuffer* pEntH0Buf = pIndexTree->pPositionH0Buffer(xEntWrk.aKey());
    if (! pEntH0Buf) {
      TRACE_UPSERT( vTrace("No pEntBuf @%lld; inserting...\n"_Stz,(i8)__LINE__); )
      vUpsertRow_insert(aCrw_upsert,&xEntWrk,pEncoding_upsert,tPerspective);
      continue;
    }
    
    Ent* aEnt_prior = pEntH0Buf->aPositionPriorEntry(xEntWrk.aKey());
    if (! aEnt_prior) {
      TRACE_UPSERT( vTrace("No aEnt_prior @%lld; inserting...\n"_Stz,(i8)__LINE__); )
      vUpsertRow_insert(aCrw_upsert,&xEntWrk,pEncoding_upsert,tPerspective);
      continue;
    }
    
    SP_Key spKey;
    TRACE_UPSERT( vTrace("Key Prior: %s\n"_Stz,spKey.sz(aEnt_prior->aKey(),pIndexDef)); )
    vDebugPoint();
    
    Ent* aEnt_found = pEntH0Buf->aNextEntry(aEnt_prior,0);
    if (! aEnt_found) {
      TRACE_UPSERT( vTrace("No aEnt_found @%lld; inserting...\n"_Stz,(i8)__LINE__); )
      vUpsertRow_insert(aCrw_upsert,&xEntWrk,pEncoding_upsert,tPerspective);
     continue;
    }
    
    TRACE_UPSERT( vTrace("Key Found: %s\n"_Stz,spKey.sz(aEnt_found->aKey(),pIndexDef)); )
    
    if (KC_SAME != Key::eCompare(aEnt_found->aKey(),xEntWrk.aKey())) {
      TRACE_UPSERT( vTrace("NOT SAME @%lld; inserting...\n"_Stz,(i8)__LINE__); )
      vUpsertRow_insert(aCrw_upsert,&xEntWrk,pEncoding_upsert,tPerspective);
      continue;
    }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // upsert (we have a crw matching the key)
    
    SP_Ent spRef;
    TRACE_UPSERT( vTrace("SAME : %s\n"_Stz,spRef.sz(aEnt_found,pIndexDef)); )
    
    Crw* aCrw_exists = CrwBuffer::pRefdCrw(aEnt_found->rxRef());
    if (! aCrw_exists) {
      TRACE_UPSERT( vTrace("No aCrw @%lld; skipping...\n"_Stz,(i8)__LINE__); )
      continue;
    }
    
    vUpsertRow_update(aCrw_exists,aEnt_found,pEncoding_upsert,tPerspective);
    
    vDebugPoint();

  } // for i_Row
  
  delete pEncoding_upsert;
}

/*----------------------------------------------------------------------------*/
/* WIP: insert a new row (no such row yet exists in the index)                */
/*----------------------------------------------------------------------------*/
void TablePtn::vUpsertRow_insert(Crw* aCrw_nosuch, EntWrk* aEntWrk,
                                 Encoding* pEncoding_upsert, t8 tPerspective)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  StreamDef* pStreamDef = m_pTable->pStreamDef();
  
  HV_Row hvRow_nosuch(pStreamDef,C_ALL);
  CrwBuffer::vToRow(hvRow_nosuch,aCrw_nosuch);
  SP_Record spRec;
  TRACE_UPSERT( vTrace("NoSuch   : %s\n"_Stz,spRec.sz(hvRow_nosuch)); )
  
  SP_Encoding spEncoding;
  TRACE_UPSERT( vTrace("Encoding : \n%s\n"_Stz,spEncoding.sz(pEncoding_upsert)); )
  
  Crw* aCrw_insert = aAllotFixedSizeCrw(aEntWrk->rxRef());
  Packing::vPackCrw(aCrw_insert,pEncoding_upsert);
  
  // set perspective
  aCrw_insert->vPerspective(tPerspective);
  
  SP_Ref spRef;
  //TRACE_UPSERT( vTrace("Insert Entries to : %s\n"_Stz,spRef.sz(aEntWrk->xRef())); )

  vInsertIndexEntries(aCrw_insert,aEntWrk->rxRef());
  
  vDebugPoint();
}
/*----------------------------------------------------------------------------*/
/* WIP: insert a new row (no such row yet exists in the index)                */
/*----------------------------------------------------------------------------*/
void TablePtn::vUpsertRow_insert(RowSlab* pRowSlab, i8 i_Row, EntWrk* aEntWrk,
                                 Encoding* pEncoding_upsert, t8 tPerspective)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  //StreamDef* pStreamDef = m_pTable->pStreamDef();
  
  //HV_Row hvRow_nosuch(pStreamDef,C_ALL);
  //CrwBuffer::vToRow(hvRow_nosuch,aCrw_nosuch);
  //SP_Record spRec;
  //TRACE_UPSERT( vTrace("NoSuch   : %s\n"_Stz,spRec.sz(hvRow_nosuch)); )
  
  //SP_Encoding spEncoding;
  //TRACE_UPSERT( vTrace("Encoding : \n%s\n"_Stz,spEncoding.sz(pEncoding_upsert)); )
  
  Crw* aCrw_insert = aAllotFixedSizeCrw(aEntWrk->rxRef());
  Packing::vPackCrw(aCrw_insert,pEncoding_upsert);
  Rsb* aRsb = pRowSlab->aRsbAdr(i_Row);
  
  
  // set perspective
  //aCrw_insert->vPerspective(tPerspective);
  
  SP_Ref spRef;
  //TRACE_UPSERT( vTrace("Insert Entries to : %s\n"_Stz,spRef.sz(aEntWrk->xRef())); )
  
  vInsertIndexEntries(aCrw_insert,aEntWrk->rxRef());
  
  vDebugPoint();
}
/*----------------------------------------------------------------------------*/
/* WIP: update an existing row                                                */
/*----------------------------------------------------------------------------*/
void TablePtn::vUpsertRow_update(Crw* aCrw_exists, Ent* aEnt_found,
                                 Encoding* pEncoding_upsert, t8 tPerspective)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  StreamDef* pStreamDef = m_pTable->pStreamDef();
  
  HV_Row hvRow_exists(pStreamDef,C_ALL);
  CrwBuffer::vToRow(hvRow_exists,aCrw_exists);
  SP_Record spRec;
  TRACE_UPSERT( vTrace("Exists   : %s\n"_Stz,spRec.sz(hvRow_exists)); )
  
  SP_Encoding spEncoding;
  TRACE_UPSERT( vTrace("Encoding : \n%s\n"_Stz,spEncoding.sz(pEncoding_upsert)); )
  
  // record prior version
  m_pReadApi->vAddCrwVersion(aEnt_found->rxRef(),aCrw_exists);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Packing::vPackCrw(aCrw_exists,pEncoding_upsert);
  
  // set perspective
  aCrw_exists->vPerspective(tPerspective);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // read-check
  
//  Crw* aCrwPrior = m_pReadApi->aGetCrwVersion(aEnt_found->xRef(),tPerspective - 2);
//  if (aCrwPrior) {
//    SP_Crw spCrw;
//    //vTrace("CrwExists : %s\n"_Stz,spCrw.sz(aCrw_exists,pStreamDef));
//    //vTrace("aCrwPrior : %s\n"_Stz,spCrw.sz(aCrwPrior,pStreamDef));
//    vDebugPoint();
//  }
//  else {
//    vDebugPoint();
//  }
  
  vDebugPoint();
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
