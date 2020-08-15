/*============================================================================*/
/* IndexTree.cpp -- Index Partition                                            */
/*============================================================================*/

#include "Indexing.h"

bool mbCheckPriorVec = false;

extern DebugFlags gxDebugFlags;;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IndexTree* Indexing::_pIndexTree(void* pAddr)
{
  IndexTree* pIndexTree = reinterpret_cast<IndexTree*>(pAddr);
  
  RASSERT_TYPE(pIndexTree,CN_INDEXTREE);
  
  return pIndexTree;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* IndexTree::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_INDEXTREE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void IndexTree::operator delete(void* pDataAddress)
{
  IndexTree* pIndexTree = reinterpret_cast<IndexTree*>(pDataAddress);
  
  HeapMem::vFree(pIndexTree);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IndexTree::IndexTree(Index* pIndex, IndexDef* pIndexDef, i8 iPartitionNo)
: Object(CN_INDEXTREE)
, m_okLock(LK_INDEX_TREE,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_pIndex = pIndex;
  m_pIndexDef = pIndexDef ? new IndexDef(pIndexDef) : nullptr;
  m_iPartitionNo = iPartitionNo;
  m_pRootEntBuffer  = nullptr;
  
  _checkInitialised_(CN_INDEXTREE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IndexTree::~IndexTree(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  vTruncate();
  delete m_pIndexDef;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump data                                                                  */
/*----------------------------------------------------------------------------*/
void IndexTree::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
}

/*----------------------------------------------------------------------------*/
/* monitor portrait                                                           */
/*----------------------------------------------------------------------------*/
void IndexTree::vMonitorPortrait(s48K& sPortrait)
{
  Statistics* pStats = AM::pStatistics();

  sPortrait.vSprint("IndexTree: appendBuffers %lld appendRows %lld "
                               "buffersPurged %lld\n"_Stz,
                    pStats->IndexTree.m_nAppendBuffers,
                    pStats->IndexTree.m_nAppendRows,
                    pStats->IndexTree.m_nBuffersPurged
                    );
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyze table                                                              */
/*----------------------------------------------------------------------------*/
Stz* IndexTree::pAnalyze(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  i8 nRowsTotal = 0;
  i8 nBuffersTotal = 0;
  i8 nStrandsTotal = 0;
  s48K sAnalysis;
  
  sAnalysis.vCatSprint("N%lld: Analysis of Index %s of table %s\n"_Stz,
                        AM::iThisNodeNo(),
                        m_pIndex->szIndexNm(),
                        m_pIndex->pTable()->szTableNm());
  
  return sAnalysis;
}

/*----------------------------------------------------------------------------*/
/* compact the index                                                          */
/*----------------------------------------------------------------------------*/
void IndexTree::vCompact(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  TRACE_COMPACT( vTrace("IndexTree::vCompact\n"_Stz); )
  
  mbCheckPriorVec = true;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the number of H1 buffers
  
#ifdef WITH_IT_COMPACT_TRACE
  EntBuffer* pH1First = pFirstH1Buffer();
  i8 nP1Buffers = pH1First ? pH1First->iLengthList() : 0;
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // step thru each H1 buffer in the index
  
  i8 iH1BufferNo = -1;
  
  for (EntBuffer* pH1 = pFirstH1Buffer(); pH1; pH1 = pH1->pNextBuffer()) {
    
    iH1BufferNo++;
    
#ifdef WITH_IT_COMPACT_TRACE
     if (0 == iH1BufferNo % 10)
      vTrace("Compaction Progress: Partition %lld: H1 Buffer %lld of %lld\n"_Stz,
             m_iPartitionNo,iH1BufferNo,nP1Buffers);
#endif
    
    TRACE_COMPACT( vTrace("  Compacting H1 %lld\n"_Stz,pH1->iBufferNo()); )
    //vTrace("  Compacting H1 %lld\n"_Stz,pH1->iBufferNo());
    
    //ASSERT(pH1->bVerifySkipListEntry());
    
    //continue;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the first H0 and the sublist which follows
    
    // get the first H0 of current H1
    EntBuffer* pFirstH0 = pH1->pFirstChildBuffer();
    
    TRACE_COMPACT( pFirstLeftH0->vShowSubList(); )
    
    // get the sublist after the first H0 of current H1
    EntBuffer* pH0SubList = pFirstH0->pUnlinkSubList();
    if (! pH0SubList)
      continue;
    
    i8 iSubListLength = pH0SubList->iLengthSubList();
    
    TRACE_COMPACT( pH0SubList->vShowSubList(); )
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // copy each key in each H0 of sublist leftwards into (new) H0 buffers
    
    // first reinit the H1 buffer
    EntBuffer::vCopyReInitH1(pH1,pFirstH0);
    
    // foreach right H0 in sublist
    EntBuffer* pLeftH0 = pFirstH0;
    while (EntBuffer* pRightH0 = pPopSubList(&pH0SubList)) {
      
      TRACE_COMPACT( if (pSubList) pSubList->vShowSubList(); )
      
      //vTrace("Sublist: %lld\n"_Stz,--iSubListLength);
      
      // copy contents of right H0 leftwards
      EntBuffer::vCopyLeftH0(&pLeftH0,pRightH0);
      
      // delete the right H0
      delete pRightH0;
      
    }
    // build the skip list for dangling left H0
    pLeftH0->vBuildSkipListSorted();
    
    //pH1->bVerifyKeyBufferOrder();
  }
  
}

/*----------------------------------------------------------------------------*/
/* count the index entries at H0 level                                        */
/*----------------------------------------------------------------------------*/
i8 IndexTree::iCountH0Entries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  i8 iCount = 0;
  EntBuffer* pEntBuffer = pFirstH0Buffer();

  while (pEntBuffer) {
    iCount += pEntBuffer->nItems();
    pEntBuffer = pEntBuffer->pNextBuffer();
  }
  
  return iCount;
}

/*----------------------------------------------------------------------------*/
/* delete an entry from an entryBuffer                                        */
/*----------------------------------------------------------------------------*/
void IndexTree::vDelete(EntBuffer* pEntryBuffer, Ent* pEnt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // purge the entry
  pEntryBuffer->vPurgeUpwards(pEnt, &m_pRootEntBuffer);
}

/*----------------------------------------------------------------------------*/
/* dump the index                                                             */
/*----------------------------------------------------------------------------*/
void IndexTree::vDump(bool bSameHeight)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  if (bSameHeight) {
    EntBuffer* pEntBuffer = m_pRootEntBuffer;
    
    while (pEntBuffer) {
      //pEntBuffer->vShow();
      pEntBuffer = pEntBuffer->pNextBuffer();
    }
  }
  else {
    // start with the root
    if (m_pRootEntBuffer)
      m_pRootEntBuffer->vDump(true);
  }
}

/*----------------------------------------------------------------------------*/
/* get the first H1 buffer                                                    */
/*----------------------------------------------------------------------------*/
EntBuffer* IndexTree::pFirstH1Buffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  EntBuffer* pEntBuffer = m_pRootEntBuffer;
  
  if ( ! pEntBuffer )
    return nullptr;
  
  if (pEntBuffer->iBufferHeight() == 0)
    return nullptr;
  
  if (pEntBuffer->iBufferHeight() == 1)
    return pEntBuffer;
  
  while (pEntBuffer) {
    
    Ent* aFirstEntry = pEntBuffer->aLeadEntry();
    pEntBuffer = EntBuffer::pRefdEntBuffer(aFirstEntry);
    
    if (pEntBuffer->iBufferHeight() == 1) {
      return pEntBuffer;
    }
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get the first H0 buffer                                                    */
/*----------------------------------------------------------------------------*/
EntBuffer* IndexTree::pFirstH0Buffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  EntBuffer* pEntBuffer = m_pRootEntBuffer;
  
  if ( ! pEntBuffer )
    return nullptr;
  
  if (pEntBuffer->iBufferHeight() == 0)
    return pEntBuffer;
  
  while (pEntBuffer) {
    
    Ent* aFirstEntry = pEntBuffer->aLeadEntry();
    
    pEntBuffer = EntBuffer::pRefdEntBuffer(aFirstEntry);
    
    if (pEntBuffer->iBufferHeight() == 0) {
      return pEntBuffer;
    }
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get reference given key                                                    */
/*----------------------------------------------------------------------------*/
Ref IndexTree::xwGetRef(Key* aFindKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  Ref xwNilRef;
  
  EntBuffer* pEntBuffer = pPositionH0Buffer(aFindKey);
  if (! pEntBuffer)
    return xwNilRef;

  Ent* aEntPos = pEntBuffer->aFindEntryBefore(aFindKey);
  if (! aEntPos)
    return xwNilRef;

  aEntPos = EntBuffer::pMoveToNextEntry(aEntPos,&pEntBuffer);
  
  //! ALKA added:
  // could be the last entry ...
  if ( ! aEntPos ) {
    return xwNilRef;;
  }
  
  eKC eCompare = Key::eCompare(aEntPos->aKey(),aFindKey);
  if (KC_SAME != eCompare)
    return xwNilRef;

  return aEntPos->rxRef();
}

/*----------------------------------------------------------------------------*/
/* get the index                                                              */
/*----------------------------------------------------------------------------*/
Index* IndexTree::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  return m_pIndex;
}

/*----------------------------------------------------------------------------*/
/* get the indexDef                                                           */
/*----------------------------------------------------------------------------*/
IndexDef* IndexTree::pIndexDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  return m_pIndexDef;
}

/*----------------------------------------------------------------------------*/
/* get the index buffers used                                                 */
/*----------------------------------------------------------------------------*/
i8 IndexTree::nEntBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  i8 nBufferCount = 0;
  if (m_pRootEntBuffer)
    m_pRootEntBuffer->vBufferCount(nBufferCount);
  
  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* insert entries for pCrwBuffer into this index at level 0                   */
/*----------------------------------------------------------------------------*/
void IndexTree::vInsertEntries(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  //pEntBuffer->vShow();

  Ref xRowRef;
  xRowRef.vBuffer(pCrwBuffer->iBufferNo());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru the buffer till no more entries
  for (i8 i_Row = 0; i_Row < pCrwBuffer->nRowCount(); i_Row++) {

    xRowRef.vItem(i_Row);
    
    bool bInserted = bInsertEntry(pCrwBuffer,i_Row,xRowRef);
    
    //bVerify();
    ASSERT(bInserted);
  }
}

/*----------------------------------------------------------------------------*/
/* insert a given entry into this index tree starting at leaf / height 0      */
/*----------------------------------------------------------------------------*/
bool IndexTree::bInsertEntry(Ent* aEnt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  m_okLock.vGrabWrite();                                           //! GRAB LOCK
  
  if (! m_pRootEntBuffer) {
    EntBuffer* pRootBuffer = pNewRootBuffer();
  }
  
  Ent* aInsertedEntry = EntBuffer::aInsertEntTree(aEnt,&m_pRootEntBuffer);
  ASSERT(aInsertedEntry);
  
  //m_pRootEntBuffer->bVerify();
  
  m_okLock.vDropWrite();                                           //! DROP LOCK
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* insert an entry for given row into this index at level 0                   */
/*----------------------------------------------------------------------------*/
bool IndexTree::bInsertEntry(Crw* aCrw, Ref& xRowRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  StreamDef* pStreamDef = m_pIndex->pTable()->pStreamDef();

  EntWrk xEntWrk;
  xEntWrk.vStreamDef(pStreamDef);
  vExtractKey(&xEntWrk,aCrw);
  xEntWrk.vRef(xRowRef);

  m_okLock.vGrabWrite();                                           //! GRAB LOCK
  
  if (! m_pRootEntBuffer) {
    EntBuffer* pRootBuffer = pNewRootBuffer();
  }
  
  Ent* aInsertedEntry = EntBuffer::aInsertEntTree(xEntWrk.aEnt(),&m_pRootEntBuffer);
  ASSERT(aInsertedEntry);
  
  //m_pRootEntBuffer->bVerify();
  
  m_okLock.vDropWrite();                                           //! DROP LOCK
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* insert an entry for given row into this index at level 0                   */
/*----------------------------------------------------------------------------*/
bool IndexTree::bInsertEntry(CrwBuffer* pCrwBuffer, i8 i_Row, Ref& xRowRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_okLock.vGrabWrite();
  
  EntWrk xEntWrk;
  vExtractKey(&xEntWrk,pCrwBuffer,i_Row);
  xEntWrk.vRef(xRowRef);
  
  if (! m_pRootEntBuffer) {
    EntBuffer* pRootBuffer = pNewRootBuffer();
  }
  
  Ent* aInsertedEntry = EntBuffer::aInsertEntTree(xEntWrk.aEnt(),&m_pRootEntBuffer);
  ASSERT(aInsertedEntry);
  
  //m_pRootEntBuffer->bVerify();
  
  m_okLock.vDropWrite();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* insert an entry for given row into this index at level 0                   */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aInsertPayloadEntry(EntWrk& xEntWrk, i8 nPayloadBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_okLock.vGrabWrite();
  
  Pay* aPay = nullptr;
  
  xEntWrk.vPayload(true);
  xEntWrk.vPayloadBytes(nPayloadBytes);
  
  byte* ayPayLoad = xEntWrk.ayPayload();
  RndLib::vBytes(ayPayLoad,nPayloadBytes);
  
  aPay = xEntWrk.aPay();

  if (! m_pRootEntBuffer) {
    EntBuffer* pRootBuffer = pNewRootBuffer();
  }
  
  Ent* aInsertedEntry = EntBuffer::aInsertEntTree(xEntWrk.aEnt(),&m_pRootEntBuffer);
  ASSERT(aInsertedEntry);
  
  //m_pRootEntBuffer->bVerify();
  
  m_okLock.vDropWrite();
  
  return aInsertedEntry;
}

/*----------------------------------------------------------------------------*/
/* insert an entry for given row into this index at level 0                   */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aInsertEntry(Ent* pEnt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_okLock.vGrabWrite();
  
  if (! m_pRootEntBuffer) {
    EntBuffer* pRootBuffer = pNewRootBuffer();
  }
  
  Ent* aInsertedEntry = EntBuffer::aInsertEntTree(pEnt,&m_pRootEntBuffer);
  ASSERT(aInsertedEntry);
  
  //m_pRootEntBuffer->bVerify();
  
  m_okLock.vDropWrite();
  
  return aInsertedEntry;
}

/*----------------------------------------------------------------------------*/
/* build skip lists for the index                                             */
/*----------------------------------------------------------------------------*/
void IndexTree::vLazyBuild(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_pRootEntBuffer->vLazyBuild();
}

/*----------------------------------------------------------------------------*/
/* pop entry buffer from sub list                                             */
/*----------------------------------------------------------------------------*/
EntBuffer* IndexTree::pPopSubList(EntBuffer** ppSubList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  EntBuffer* pTopBuffer = *ppSubList;
  
  if (! pTopBuffer)
    return nullptr;
  
  *ppSubList = (*ppSubList)->pNextBuffer();
  
  pTopBuffer->vNextBuffer(nullptr);
  pTopBuffer->vPrevBuffer(nullptr);
  
  if (*ppSubList)
    (*ppSubList)->vPrevBuffer(nullptr);
  
  return pTopBuffer;
}

/*----------------------------------------------------------------------------*/
/* position to the first H0 buffer which has given key                        */
/*----------------------------------------------------------------------------*/
EntBuffer* IndexTree::pPositionH0Buffer(Key* aFindKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  if (m_pRootEntBuffer)
    return m_pRootEntBuffer->pFindH0Buffer(aFindKey);
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* position to the first H0 buffer which has given key                        */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aPositionPriorEntry(Key* aFindKey, EntBuffer** ppEntBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  EntBuffer* pEntBuffer;
  
  // root buffer ?
  if ( ! m_pRootEntBuffer )
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - -
  // position at H0 buffer
  pEntBuffer = m_pRootEntBuffer->pFindH0Buffer(aFindKey);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - -
  // position at prior entry
  Ent* aEnt_prior = pEntBuffer->aFindEntryBefore(aFindKey);
  
  // entry ?
  if ( ! aEnt_prior )
    return nullptr;
  
  // return the prior entry and the buffer
  *ppEntBuffer = pEntBuffer;
  return aEnt_prior;

}

/*----------------------------------------------------------------------------*/
/* create a new root buffer                                                   */
/*----------------------------------------------------------------------------*/
EntBuffer* IndexTree::pNewRootBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  ASSERT(! m_pRootEntBuffer);
  
  m_pRootEntBuffer = new EntBuffer(m_pIndex ? m_pIndex->pIndexDef()->eKeyKind()
                                   : m_pIndexDef->eKeyKind());
  m_pRootEntBuffer->vIndex(m_pIndex);
  m_pRootEntBuffer->vPartitionNo(m_iPartitionNo);
  
  return m_pRootEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* get root buffer                                                            */
/*----------------------------------------------------------------------------*/
EntBuffer* IndexTree::pRootBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  return m_pRootEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* set root buffer                                                            */
/*----------------------------------------------------------------------------*/
void IndexTree::vRootBuffer(EntBuffer* pRootBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_pRootEntBuffer = pRootBuffer;
}

/*----------------------------------------------------------------------------*/
/* truncate the index                                                         */
/*----------------------------------------------------------------------------*/
void IndexTree::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_okLock.vGrabWrite();
  
  if (m_pRootEntBuffer)
    m_pRootEntBuffer->vTruncate();
  
  m_pRootEntBuffer    = nullptr;
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* verify                                                                     */
/*----------------------------------------------------------------------------*/
bool IndexTree::bVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  if (m_pRootEntBuffer)
    return m_pRootEntBuffer->bVerify();

  return true;
}

/*----------------------------------------------------------------------------*/
/* verify index from root                                                     */
/*----------------------------------------------------------------------------*/
bool IndexTree::bVerifyIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  if (m_pRootEntBuffer)
    return m_pRootEntBuffer->bVerify();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a key from given row into EntWrk                                   */
/*----------------------------------------------------------------------------*/
void IndexTree::vExtractKey(EntWrk* pEntWrk, Crw* aCrw)
{
  // get stream def
  StreamDef* pStreamDef = pEntWrk->pStreamDef();
  
  // inflate row
  HV_Row hvRow(pStreamDef,C_ALL);
  
  RowInflator::vInflateRow(hvRow,aCrw);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IndexDef* pIndexDef = m_pIndex->pIndexDef();
  
  i8 nIdxCols = pIndexDef->nColumns();
  hvRow->m_bGetAll = false;
  
  for (i8 i_IdxCol = 0; i_IdxCol < nIdxCols; i_IdxCol++) {
    
    // get the column from the index column vector
    Column* pColumn = pIndexDef->pGetColumn(i_IdxCol);
    
    // mark column to be retrieved
    i8 i_Col = pColumn->iColumnNo();
    hvRow->m_abGetCol[i_Col] = true;
    hvRow->m_iLastCol = (i_Col > hvRow->m_iLastCol)
                          ? i_Col
                          : hvRow->m_iLastCol;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (i8 i_Col = 0; i_Col < nIdxCols; i_Col++) {
    
    Column* pColumn = pIndexDef->pGetColumn(i_Col);
    
    // add the column value to the key
    pEntWrk->vAppendElement(hvRow,pColumn);
  }
}

/*----------------------------------------------------------------------------*/
/* extract a key from given row into EntWrk                                   */
/*----------------------------------------------------------------------------*/
void IndexTree::vExtractKey(EntWrk* pEntWrk, Rsb* pRsb)
{
  // get stream def
  StreamDef* pStreamDef = pEntWrk->pStreamDef();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 nIdxCols = m_pIndexDef->nColumns();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  for (i8 i_Col = 0; i_Col < nIdxCols; i_Col++) {
    
    // add the column value to the key
    pEntWrk->vAppendElement(pRsb, m_pIndexDef->pGetColumn(i_Col));
  }
}

/*----------------------------------------------------------------------------*/
/* extract a key from row into EntWrk                                         */
/*----------------------------------------------------------------------------*/
void IndexTree::vExtractKey(EntWrk* pEntWrk, CrwBuffer* pCrwBuffer, i8 i_Row)
{
  // create a row
  HV_Row hRow(pCrwBuffer->pStreamDef(),C_ALL);
  
  IndexDef* pIndexDef = m_pIndex->pIndexDef();
  
  i8 nIdxCols = pIndexDef->nColumns();
  hRow->m_bGetAll = false;
  
  for (i8 i_IdxCol = 0; i_IdxCol < nIdxCols; i_IdxCol++) {
    
    // get the column from the index column vector
    Column* pColumn = pIndexDef->pGetColumn(i_IdxCol);
    
    // mark column to be retrieved
    i8 iColumn = pColumn->iColumnNo();
    hRow->m_abGetCol[iColumn] = true;
    hRow->m_iLastCol = iColumn > hRow->m_iLastCol ? iColumn :  hRow->m_iLastCol;
  }
  
  // get the inflated compact row
  pCrwBuffer->vGetRow(hRow, i_Row);
  
  for (i8 i_Column = 0; i_Column < nIdxCols; i_Column++) {
    
    Column* pColumn = pIndexDef->pGetColumn(i_Column);
    
    // add the column value to the key
    pEntWrk->vAppendElement(hRow, pColumn);
  }
  
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* visualize the index                                                        */
/*----------------------------------------------------------------------------*/
void IndexTree::vDumpLevel(EntBuffer* pEntBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  // get the first entry
  Ent* aEnt = pEntBuffer->aLeadEntry();
  
  while (aEnt) {
    
    // get the index buffer the first enetry poinst to
    EntBuffer* pEntryEntBuffer = EntBuffer::pRefdEntBuffer(aEnt);
    
    // print the decile of the buffer
    vTrace("%lld"_Stz, pEntryEntBuffer->iDecile());
    
    // insert spaces for no of entries in buffer
    if (pEntryEntBuffer->iBufferHeight() > 0) {
      
      // exclude the head entry
      i8 iEntries = pEntryEntBuffer->nItems() - 1;
      
      while (iEntries--)
        vTrace(" "_Stz);
    }
    
    // move to the next entry
    aEnt = pEntBuffer->aNextEntry(aEnt, 0);
  }
}

/*----------------------------------------------------------------------------*/
/* visualize the index                                                        */
/*----------------------------------------------------------------------------*/
void IndexTree::vVisualize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  // start with the root buffer
  EntBuffer* pEntBuffer = m_pRootEntBuffer;
  
  if ( ! pEntBuffer)
    return;

  // print the decile of the root buffer
  vTrace("%lld\n"_Stz, pEntBuffer->iDecile());
  
  i8 iBufferHeight = pEntBuffer->iBufferHeight();
  
  // if there is only one buffer i.e root is at level 0, we are done
  if (iBufferHeight == 0) {
    vTrace("\n"_Stz);
    return;
  }
  
  // iterate through the buffers
  while (pEntBuffer) {
    
    // dump this buffer
    vDumpLevel(pEntBuffer);

    // if we are at level 1, iterate thru next buffers
    if (pEntBuffer->iBufferHeight() == 1) {
      pEntBuffer = (EntBuffer*)pEntBuffer->pNextBuffer();
    }
    else {
      vTrace("\n"_Stz);
      
      // get the first entry of the buffer
      Ent* aEnt = pEntBuffer->aLeadEntry();
      
      // get the index buffer this entry points to
      pEntBuffer = EntBuffer::pRefdEntBuffer(aEnt);
    }
  }
  
  vTrace("\n"_Stz);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(IndexTree);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
