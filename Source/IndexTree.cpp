/*============================================================================*/
/* IndexTree.cpp - index data portion of table partition                      */
/*============================================================================*/

#include "Data.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

//#define TRACE_COMPACT(x) x
#define TRACE_COMPACT(x)

bool mbCheckPriorVec = false;

#define USE_IT_COPYLEFTV3

#define USE_IT_COMPACT_TRACE 10

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
IndexTree::IndexTree(Index* pIndex, i8 iPartitionNo)
: Object(CN_INDEXTREE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_okLock.vInitLock(LK_INDEX_TREE);
  m_pIndex = pIndex;
  m_iPartitionNo = iPartitionNo;
  m_pRootBuffer  = nullptr;
  
  _checkInitialised_(CN_INDEXTREE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IndexTree::~IndexTree(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  vTidyUp();
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IndexTree* Data::_pIndexTree(void* pAddr)
{
  IndexTree* pIndexTree = reinterpret_cast<IndexTree*>(pAddr);
  
  RASSERT_TYPE(pIndexTree,CN_INDEXTREE);
  
  return pIndexTree;
}

/*----------------------------------------------------------------------------*/
/* dump data                                                                  */
/*----------------------------------------------------------------------------*/
void IndexTree::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
}

/*----------------------------------------------------------------------------*/
/* finalize                                                                   */
/*----------------------------------------------------------------------------*/
void IndexTree::vFinalize(eCN eClass, Object* pObject)
{
  ASSERT(eClass == CN_INDEXTREE);
  ASSERT(pObject->eClass() == CN_INDEXTREE);
  
  IndexTree* pIndexTree = _pIndexTree(pObject);
  ASSERT(pIndexTree->bWordBody());
  
  pIndexTree->vTidyUp();
}

/*----------------------------------------------------------------------------*/
/* monitor portrait                                                           */
/*----------------------------------------------------------------------------*/
void IndexTree::vMonitorPortrait(s48K& sPortrait)
{
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  
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
                        gpAttachMap->m_pGrid->iThisNode(),
                        m_pIndex->aIndexName()->aChar(),
                        m_pIndex->pTable()->aTableName());
  
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
  // get the number of L1 buffers 
  
#ifdef USE_IT_COMPACT_TRACE
  EntBuffer* pL1First = pFirstL1Buffer();
  i8 nP1Buffers = pL1First ? pL1First->iLengthList() : 0;
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // step thru each L1 buffer in the index
  
  i8 iL1BufferNo = -1;
  
  for (EntBuffer* pL1 = pFirstL1Buffer(); pL1; pL1 = pL1->pNextBuffer()) {
    
    iL1BufferNo++;
    
#ifdef USE_IT_COMPACT_TRACE
     if (0 == iL1BufferNo % USE_IT_COMPACT_TRACE)
      vTrace("Compaction Progress: Partition %lld: L1 Buffer %lld of %lld\n"_Stz,
             m_iPartitionNo,iL1BufferNo,nP1Buffers);
#endif
    
    TRACE_COMPACT( vTrace("  Compacting L1 %lld\n"_Stz,pL1->iBufferNo()); )
    //vTrace("  Compacting L1 %lld\n"_Stz,pL1->iBufferNo());
    
    //RASSERT(pL1->bVerifySkipListEntry());
    
    //continue;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the first L0 and the sublist which follows
    
    // get the first L0 of current L1
    EntBuffer* pFirstL0 = pL1->pFirstChildBuffer();
    
    TRACE_COMPACT( pFirstLeftL0->vShowSubList(); )
    
    // get the sublist after the first L0 of current L1
    EntBuffer* pL0SubList = pFirstL0->pUnlinkSubList();
    if (! pL0SubList)
      continue;
    
    i8 iSubListLength = pL0SubList->iLengthSubList();
    
    TRACE_COMPACT( pL0SubList->vShowSubList(); )
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // copy each key in each L0 of sublist leftwards into (new) L0 buffers
    
    // first reinit the L1 buffer
    vCopyReInitL1(pL1,pFirstL0);
    
    // foreach right L0 in sublist
    EntBuffer* pLeftL0 = pFirstL0;
    while (EntBuffer* pRightL0 = pPopSubList(&pL0SubList)) {
      
      TRACE_COMPACT( if (pSubList) pSubList->vShowSubList(); )
      
      //vTrace("Sublist: %lld\n"_Stz,--iSubListLength);
      
      // copy contents of right L0 leftwards
#ifdef USE_IT_COPYLEFTV3
      vCopyLeftV3(&pLeftL0,pRightL0);
#else
      vCopyLeftV2(&pLeftL0,pRightL0);
#endif
      
      // delete the right L0
      delete pRightL0;
      
    }
#ifdef USE_IT_COPYLEFTV3
    // build the skip list for dangling left L0
    pLeftL0->vBuildSkipListSorted();
#endif
    
    //pL1->bVerifyKeyBufferOrder();
  }
  
}

/*----------------------------------------------------------------------------*/
/* reinitialise the L1 buffer with just the first key of the first L0 buffer  */
/*----------------------------------------------------------------------------*/
void IndexTree::vCopyReInitL1(EntBuffer* pL1, EntBuffer* pFirstL0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  ASSERT(0 == pFirstL0->iBufferHeight());
  ASSERT(1 == pL1->iBufferHeight());
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // reset the L1 buffer
  
  pL1->vReset();
  //pL1->vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // reinsert the first key of the first L0 buffer back into the L1 buffer
  
  Ent* aFirstL0_Key = pFirstL0->aLeadEntry();
  
  Id xId = aFirstL0_Key->xId();
  byte* aKeyBytes = aFirstL0_Key->ayKeyBytes();
  //i8 nKeySize = aKey->nKeySize(pIdxColVec);
  i8 nKeySize = aFirstL0_Key->nSizeKey();
  i8 nLevels = aFirstL0_Key->nLevels();
  
  i8 iFirstL0_BufferNo = pFirstL0->iPoolBufferNo();
  xId.vBuffer(iFirstL0_BufferNo);
  
  EntWrk xL1_FirstKey;
  
  xL1_FirstKey.vId(xId);
  xL1_FirstKey.vEmbedded(aKeyBytes);
  xL1_FirstKey.vKeySize(nKeySize);
  xL1_FirstKey.vLevels(nLevels);
  
  Ent* aFirstL1_Key = pL1->aInsertKeyAbs(xL1_FirstKey);
  
  //pL1->vShow();
}

/*----------------------------------------------------------------------------*/
/* copy all keys of right buffer to left buffer; create new left when needed  */
/*----------------------------------------------------------------------------*/
void IndexTree::vCopyLeftV3(EntBuffer** ppLeftL0, EntBuffer* pRightL0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  TRACE_COMPACT( vTrace("IndexTree::vCopyLeft (right %lld)\n"_Stz,
                        pRightL0->iBufferNo()); )
  
  EntWrk xLeftEntWrk;
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // assert the parameters are sensible
  
  EntBuffer* pLeftL0 = *ppLeftL0;
  ASSERT(0 == pLeftL0->iBufferHeight());
  ASSERT(0 == pRightL0->iBufferHeight());
  EntBuffer* pL1 = pLeftL0->pHigherBuffer();
  ASSERT(1 == pL1->iBufferHeight());
  ASSERT(pRightL0->pHigherBuffer() == pLeftL0->pHigherBuffer());
  
  //vTrace("IndexTree::vCopyLeft (left %lld right %lld)\n"_Stz,
  //       pLeftL0->iBufferNo(),pRightL0->iBufferNo());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set up for using pInsertKeyAbsL0
  
  Ent* aPriorEntry = pLeftL0->aLastEntry();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru each key in right L0
  
  TRACE_COMPACT( pRightL0->vShow(); )

  //pLeftL0->vShow();
  
  for (Ent* aRightL0_Key = pRightL0->aLeadEntry();
       aRightL0_Key;
       aRightL0_Key = pRightL0->aNextEntry(aRightL0_Key,0)) {
    
    TRACE_COMPACT( pRightKeyEntryL0->vShow(0, m_pIndex, true); )
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the elements of the right key
    
    Id xId = aRightL0_Key->xId();
    byte* aKeyBytes = aRightL0_Key->ayKeyBytes();
    
    //i8 nKeySize = aKey->nKeySize(pIdxColVec);
    i8 nKeySize = aRightL0_Key->nSizeKey();
    i8 nLevels = aRightL0_Key->nLevels();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // form the working version of the left key
    
    xLeftEntWrk.vId(xId);
    xLeftEntWrk.vEmbedded(aKeyBytes);
    xLeftEntWrk.vKeySize(nKeySize);
    xLeftEntWrk.vLevels(nLevels);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // compute the size of the left key
    
    i8 nEntryBytes = xLeftEntWrk.nEntKeySize();
    i8 nOffsetBytes = xLeftEntWrk.nOffsetsSize();
    i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get a new left buffer if not enough space in current left buffer
    
    if (! pLeftL0->bHasSpace(nEntryPlusOffsets)) {
      
      // build the skip list for left L0 thats now full
      pLeftL0->vBuildSkipListSorted();
      
      //pLeftL0->vShow();
      
      // new left buffer
      EntBuffer* pNewLeftL0 = new EntBuffer;
      pNewLeftL0->vIndex(m_pIndex);
      pNewLeftL0->vHigherBuffer(pL1);
      
      // splice the new left buffer into the L0 list
      pLeftL0->vSpliceAfter(pNewLeftL0);
      
      // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
      
      // adjust variables
      *ppLeftL0 = pNewLeftL0;
      pLeftL0 = pNewLeftL0;
      
      aPriorEntry = pLeftL0->aLastEntry();
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // insert a copy of the right key into left buffer
    
    TRACE_COMPACT( pLeftL0->vShow(); )
    Ent* aLeftKeyEntryL0 = pLeftL0->aInsertKeyAbsL0(xLeftEntWrk,&aPriorEntry);
    TRACE_COMPACT( pLeftKeyEntryL0->vShow(0, m_pIndex, true); )
    TRACE_COMPACT( pLeftL0->vShow(); )
    
    //pLeftL0->vShow();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // insert a first L0 key into L1
    
    if (1 == pLeftL0->nItems()) {
      
      i8 iLeftL0BufferNo = pLeftL0->iPoolBufferNo();
      xId.vBuffer(iLeftL0BufferNo);
      xLeftEntWrk.vId(xId);
      
      TRACE_COMPACT( pL1->vShow(); )
      //RASSERT(pL1->bVerifySkipListEntry());
      Ent* aLeftEntryL1 = pL1->aInsertKeyAbs(xLeftEntWrk);
      //RASSERT(pL1->bVerifySkipListEntry());
      TRACE_COMPACT( pLeftEntryL1->vShow(0, m_pIndex, true); )
      TRACE_COMPACT( pL1->vShow(); )
      
      // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
      
      //pL1->vShowShort();
      
      //pL1->vShow();
      
      //pL1->vShowTree();
      
      //pL1->bVerifyKeyBufferOrder();
      
      // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
    }
    
    //pL1->bVerifyKeyBufferOrder();
    
  } // for
  
}
/*----------------------------------------------------------------------------*/
void IndexTree::vCopyLeftV2(EntBuffer** ppLeftL0, EntBuffer* pRightL0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  TRACE_COMPACT( vTrace("IndexTree::vCopyLeft (right %lld)\n"_Stz,
                        pRightL0->iBufferNo()); )
  
  EntWrk xLeftEntWrk;
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // assert the parameters are sensible
  
  EntBuffer* pLeftL0 = *ppLeftL0;
  ASSERT(0 == pLeftL0->iBufferHeight());
  ASSERT(0 == pRightL0->iBufferHeight());
  EntBuffer* pL1 = pLeftL0->pHigherBuffer();
  ASSERT(1 == pL1->iBufferHeight());
  ASSERT(pRightL0->pHigherBuffer() == pLeftL0->pHigherBuffer());
  
  //vTrace("IndexTree::vCopyLeft (left %lld right %lld)\n"_Stz,
  //       pLeftL0->iBufferNo(),pRightL0->iBufferNo());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru each key in right L0
  
  TRACE_COMPACT( pRightL0->vShow(); )
  
  for (Ent* aRightL0_Key = pRightL0->aLeadEntry();
       aRightL0_Key;
       aRightL0_Key = pRightL0->aNextEntry(aRightL0_Key,0)) {
    
    TRACE_COMPACT( pRightKeyEntryL0->vShow(0, m_pIndex, true); )
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the elements of the right key
    
    Id xId = aRightL0_Key->xId();
    byte* ayKeyBytes = aRightL0_Key->ayKeyBytes();
    
    //i8 nKeySize = aKey->nKeySize(pIdxColVec);
    i8 nKeySize = aRightL0_Key->nSizeKey();
    i8 nLevels = aRightL0_Key->nLevels();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // form the working version of the left key
    
    xLeftEntWrk.vId(xId);
    xLeftEntWrk.vEmbedded(ayKeyBytes);
    xLeftEntWrk.vKeySize(nKeySize);
    xLeftEntWrk.vLevels(nLevels);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // compute the size of the left key
    
    i8 nEntryBytes = xLeftEntWrk.nEntKeySize();
    i8 nOffsetBytes = xLeftEntWrk.nOffsetsSize();
    i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get a new left buffer if not enough space in current left buffer
    
    if (! pLeftL0->bHasSpace(nEntryPlusOffsets)) {
      
      // new left buffer
      EntBuffer* pNewLeftL0 = new EntBuffer;
      pNewLeftL0->vIndex(m_pIndex);
      pNewLeftL0->vHigherBuffer(pL1);
      
      // splice the new left buffer into the L0 list
      pLeftL0->vSpliceAfter(pNewLeftL0);
      
      // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
      
      
      
      // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
      
      // adjust variables
      *ppLeftL0 = pNewLeftL0;
      pLeftL0 = pNewLeftL0;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // insert a copy of the right key into left buffer
    
    TRACE_COMPACT( pLeftL0->vShow(); )
    Ent* aLeftKeyEntryL0 = pLeftL0->aInsertKeyAbs(xLeftEntWrk);
    TRACE_COMPACT( pLeftKeyEntryL0->vShow(0, m_pIndex, true); )
    TRACE_COMPACT( pLeftL0->vShow(); )
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // insert a first L0 key into L1
    
    if (1 == pLeftL0->nItems()) {
      
      i8 iLeftL0BufferNo = pLeftL0->iPoolBufferNo();
      xId.vBuffer(iLeftL0BufferNo);
      xLeftEntWrk.vId(xId);
      
      TRACE_COMPACT( pL1->vShow(); )
      //RASSERT(pL1->bVerifySkipListEntry());
      Ent* aLeftEntryL1 = pL1->aInsertKeyAbs(xLeftEntWrk);
      //RASSERT(pL1->bVerifySkipListEntry());
      TRACE_COMPACT( pLeftEntryL1->vShow(0, m_pIndex, true); )
      TRACE_COMPACT( pL1->vShow(); )
      
      // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
      
      //pL1->vShowShort();
      
      //pL1->vShow();
      
      //pL1->vShowTree();
      
      //pL1->bVerifyKeyBufferOrder();
      
      // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
    }
    
    //pL1->bVerifyKeyBufferOrder();
    
  } // for
  
}

/*----------------------------------------------------------------------------*/
/* dump the index                                                             */
/*----------------------------------------------------------------------------*/
void IndexTree::vDump(bool bSameHeight)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  if (bSameHeight) {
    EntBuffer* pEntBuffer = m_pRootBuffer;
    
    while (pEntBuffer) {
      pEntBuffer->vShow();
      pEntBuffer = pEntBuffer->pNextBuffer();
    }
  }
  else {
    // start with the root
    if (m_pRootBuffer)
      m_pRootBuffer->vDump();
  }
}

/*----------------------------------------------------------------------------*/
/* get the first L1 buffer                                                    */
/*----------------------------------------------------------------------------*/
EntBuffer* IndexTree::pFirstL1Buffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  EntBuffer* pEntBuffer = m_pRootBuffer;
  
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
/* get the first L0 buffer                                                    */
/*----------------------------------------------------------------------------*/
EntBuffer* IndexTree::pFirstH0Buffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  EntBuffer* pEntBuffer = m_pRootBuffer;
  
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
/* get the index                                                              */
/*----------------------------------------------------------------------------*/
Index* IndexTree::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  return m_pIndex;
}

/*----------------------------------------------------------------------------*/
/* get the index buffers used                                                 */
/*----------------------------------------------------------------------------*/
i8 IndexTree::nEntBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  i8 nBufferCount = 0;
  if (m_pRootBuffer)
    m_pRootBuffer->vBufferCount(nBufferCount);
  
  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* insert entries from entryBuffer into this index at level 0                 */
/*----------------------------------------------------------------------------*/
void IndexTree::vInsertEntriesFromBuffer(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  //m_okLock.vGrabWrite();

  //pEntBuffer->vShow();

  Id xId;
  xId.vBuffer(*pRowBuffer->nPoolBufferNo());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru the buffer till no more entries
  for (i8 i_Row = 0; i_Row < pRowBuffer->nRowCount(); i_Row++) {

    xId.vItem(i_Row);
    
    bool bInserted = bInsertEntry(pRowBuffer,i_Row,xId);
    
    //bVerify();
    ASSERT(bInserted);
  }
  
  //m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* insert an entry for given row into this index at level 0                   */
/*----------------------------------------------------------------------------*/
bool IndexTree::bInsertEntry(RowBuffer* pRowBuffer, i8 i_Row, Id& xId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_okLock.vGrabWrite();
  
  EntWrk xEntWrk;
  vExtractKey(&xEntWrk,pRowBuffer,i_Row);
  xEntWrk.vId(xId);
  
  //xEntWrk.vShow(m_pIndex->pIdxColVec());
  
  if ( ! m_pRootBuffer ) {
    m_pRootBuffer = new EntBuffer;
    m_pRootBuffer->vIndex(m_pIndex);
    m_pRootBuffer->vPartitionNo(m_iPartitionNo);
  }
  
  Ent* aKeyEntry = EntBuffer::aInsertKeyTree(xEntWrk,&m_pRootBuffer);
  ASSERT(aKeyEntry);
  
  //m_pRootBuffer->bVerify();
  
  m_okLock.vDropWrite();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* build skip lists for the index                                             */
/*----------------------------------------------------------------------------*/
void IndexTree::vLazyBuild(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_pRootBuffer->vLazyBuild();
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
/* position the IndexTree at a particular height of index buffer              */
/*----------------------------------------------------------------------------*/
EntBuffer* IndexTree::pPositionH0Buffer(EntWrk& xEntWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  if (m_pRootBuffer)
    return m_pRootBuffer->pPositionH0Buffer(xEntWrk);
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get root entry                                                             */
/*----------------------------------------------------------------------------*/
EntBuffer* IndexTree::pRoot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  return m_pRootBuffer;
}

/*----------------------------------------------------------------------------*/
/* tidyup                                                                     */
/*----------------------------------------------------------------------------*/
void IndexTree::vTidyUp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  // nothing to do
}

/*----------------------------------------------------------------------------*/
/* truncate the index                                                         */
/*----------------------------------------------------------------------------*/
void IndexTree::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_okLock.vGrabWrite();
  
  if (m_pRootBuffer)
    m_pRootBuffer->vTruncate();
  
  m_pRootBuffer    = nullptr;
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* verify                                                                     */
/*----------------------------------------------------------------------------*/
bool IndexTree::bVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  if (m_pRootBuffer)
    return m_pRootBuffer->bVerify();

  return true;
}

/*----------------------------------------------------------------------------*/
/* verify index from root                                                     */
/*----------------------------------------------------------------------------*/
bool IndexTree::bVerifyIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  if (m_pRootBuffer)
    return m_pRootBuffer->bVerify();
  
  return true;
}



/*----------------------------------------------------------------------------*/
/* extract an index key into EntWrk from this row using given index columns   */
/*----------------------------------------------------------------------------*/
void IndexTree::vExtractKey(EntWrk* pEntWrk, RowBuffer* pRowBuffer, i8 i_Row)
{
  // create a row
  HV_Row hRow(pRowBuffer->pColumnVec());
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  i8 nIdxCols = pIdxColVec->nItems();
  hRow->m_bGetAll = false;
  
  for (i8 i_IdxCol = 0; i_IdxCol < nIdxCols; i_IdxCol++) {
    
    // get the column from the index column vector
    Column* pColumn = pIdxColVec->pGet(i_IdxCol);
    
    // mark column to be retrieved
    i8 iColumn = pColumn->iColumnNo();
    hRow->m_abGetCol[iColumn] = true;
    hRow->m_iLastCol = iColumn > hRow->m_iLastCol ? iColumn :  hRow->m_iLastCol;
  }
  
  // get the inflated compact row
  pRowBuffer->vGetRow(hRow, i_Row);
  
  for (i8 i_Column = 0; i_Column < nIdxCols; i_Column++) {
    
    Column* pColumn = pIdxColVec->pGet(i_Column);
    
    // add the column value to the key
    pEntWrk->vAddElement(hRow, pColumn);
    
    //vShow(pIdxColVec);
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
  EntBuffer* pEntBuffer = m_pRootBuffer;
  
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

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* IndexTree::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
