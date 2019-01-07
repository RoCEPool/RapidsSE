/*============================================================================*/
/* IndexTree.cpp - index data portion of table partition                      */
/*============================================================================*/

#include "DBobjects.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

//#define TRACE_COMPACT(x) x
#define TRACE_COMPACT(x)

bool mbCheckPriorVec = false;

#define IT_COPYLEFTV3

#define IT_COMPACT_P1_TRACE 10

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IndexTree* DBobjects::_pIndexTree(void* pAddr)
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
  
  m_zLock.vInitLock(LK_INDEX_TREE);
  m_pIndex = pIndex;
  m_iPartitionNo = iPartitionNo;
  m_pPartition   = nullptr;
  m_pRootBuffer  = nullptr;
  m_pCurrentBuffer = nullptr;
  
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
  
  if (m_pIndex->bIsHidden())
      sAnalysis.vCatSprint("N%lld: Analysis of table %s\n"_Stz,
                           gpAttachMap->m_pGrid->iThisNode(),
                           m_pIndex->pTable()->aTableName());
  else
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
  
#ifdef IT_COMPACT_P1_TRACE
  EntryBuffer* pL1First = pFirstL1Buffer();
  i8 nP1Buffers = pL1First ? pL1First->iLengthList() : 0;
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // step thru each L1 buffer in the index
  
  i8 iL1BufferNo = -1;
  
  for (EntryBuffer* pL1 = pFirstL1Buffer(); pL1; pL1 = pL1->pNextBuffer()) {
    
    iL1BufferNo++;
    
#ifdef IT_COMPACT_P1_TRACE
     if (0 == iL1BufferNo % IT_COMPACT_P1_TRACE)
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
    EntryBuffer* pFirstL0 = pL1->pFirstChildBuffer();
    
    TRACE_COMPACT( pFirstLeftL0->vShowSubList(); )
    
    // get the sublist after the first L0 of current L1
    EntryBuffer* pL0SubList = pFirstL0->pUnlinkSubList();
    if (! pL0SubList)
      continue;
    
    i8 iSubListLength = pL0SubList->iLengthSubList();
    
    TRACE_COMPACT( pL0SubList->vShowSubList(); )
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // copy each key in each L0 of sublist leftwards into (new) L0 buffers
    
    // first reinit the L1 buffer
    vCopyReInitL1(pL1,pFirstL0);
    
    // foreach right L0 in sublist
    EntryBuffer* pLeftL0 = pFirstL0;
    while (EntryBuffer* pRightL0 = pPopSubList(&pL0SubList)) {
      
      TRACE_COMPACT( if (pSubList) pSubList->vShowSubList(); )
      
      //vTrace("Sublist: %lld\n"_Stz,--iSubListLength);
      
      // copy contents of right L0 leftwards
#ifdef IT_COPYLEFTV3
      vCopyLeftV3(&pLeftL0,pRightL0);
#else
      vCopyLeftV2(&pLeftL0,pRightL0);
#endif
      
      // delete the right L0
      delete pRightL0;
      
    }
#ifdef IT_COPYLEFTV3
    // build the skip list for dangling left L0
    pLeftL0->vBuildSkipListSorted();
#endif
    
    //pL1->bVerifyKeyBufferOrder();
  }
  
}

/*----------------------------------------------------------------------------*/
/* reinitialise the L1 buffer with just the first key of the first L0 buffer  */
/*----------------------------------------------------------------------------*/
void IndexTree::vCopyReInitL1(EntryBuffer* pL1, EntryBuffer* pFirstL0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  ASSERT(0 == pFirstL0->iBufferLevel());
  ASSERT(1 == pL1->iBufferLevel());
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // reset the L1 buffer
  
  pL1->vReset();
  //pL1->vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // reinsert the first key of the first L0 buffer back into the L1 buffer
  
  Entry* pFirstL0_Key = pFirstL0->pFirstEntry();
  
  i8 iTxId = pFirstL0_Key->iTxId();
  Id xId = pFirstL0_Key->xId();
  Key* aKey = pFirstL0_Key->aKey();
  i8 nKeySize = aKey->nKeySize(pIdxColVec);
  i8 nOffsets = pFirstL0_Key->nOffsets();
  
  i8 iFirstL0_BufferNo = pFirstL0->iBufferNo();
  xId.vBuffer(iFirstL0_BufferNo);
  
  KeyWrk xL1_FirstKey;
  
  xL1_FirstKey.vTxId(iTxId);
  xL1_FirstKey.vId(xId);
  xL1_FirstKey.vEmbedded((char*)aKey);
  xL1_FirstKey.vKeySize(nKeySize);
  xL1_FirstKey.vOffsets(nOffsets);
  
  Entry* pFirstL1_Key = pL1->pInsertKeyAbs(xL1_FirstKey);
  
  //pL1->vShow();
}

/*----------------------------------------------------------------------------*/
/* copy all keys of right buffer to left buffer; create new left when needed  */
/*----------------------------------------------------------------------------*/
void IndexTree::vCopyLeftV3(EntryBuffer** ppLeftL0, EntryBuffer* pRightL0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  TRACE_COMPACT( vTrace("IndexTree::vCopyLeft (right %lld)\n"_Stz,
                        pRightL0->iBufferNo()); )
  
  KeyWrk xLeftKeyWrk;
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // assert the parameters are sensible
  
  EntryBuffer* pLeftL0 = *ppLeftL0;
  ASSERT(0 == pLeftL0->iBufferLevel());
  ASSERT(0 == pRightL0->iBufferLevel());
  EntryBuffer* pL1 = pLeftL0->pHigherBuffer();
  ASSERT(1 == pL1->iBufferLevel());
  ASSERT(pRightL0->pHigherBuffer() == pLeftL0->pHigherBuffer());
  
  //vTrace("IndexTree::vCopyLeft (left %lld right %lld)\n"_Stz,
  //       pLeftL0->iBufferNo(),pRightL0->iBufferNo());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set up for using pInsertKeyAbsL0
  
  Entry* pPriorEntry = pLeftL0->pLastEntry();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru each key in right L0
  
  TRACE_COMPACT( pRightL0->vShow(); )

  //pLeftL0->vShow();
  
  for (Entry* pRightL0_Key = pRightL0->pFirstEntry();
       pRightL0_Key;
       pRightL0_Key = pRightL0->pNextEntry(pRightL0_Key,0)) {
    
    TRACE_COMPACT( pRightKeyEntryL0->vShow(0, m_pIndex, true); )
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the elements of the right key
    
    i8 iTxId = pRightL0_Key->iTxId();
    Id xId = pRightL0_Key->xId();
    Key* aKey = pRightL0_Key->aKey();
    i8 nKeySize = aKey->nKeySize(pIdxColVec);
    i8 nOffsets = pRightL0_Key->nOffsets();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // form the working version of the left key
    
    xLeftKeyWrk.vTxId(iTxId);
    xLeftKeyWrk.vId(xId);
    xLeftKeyWrk.vEmbedded((char*)aKey);
    xLeftKeyWrk.vKeySize(nKeySize);
    xLeftKeyWrk.vOffsets(nOffsets);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // compute the size of the left key
    
    i8 nEntryBytes = xLeftKeyWrk.nEntrySize();
    i8 nOffsetBytes = xLeftKeyWrk.nOffsetSize();
    i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get a new left buffer if not enough space in current left buffer
    
    if (! pLeftL0->bHasSpace(nEntryPlusOffsets)) {
      
      // build the skip list for left L0 thats now full
      pLeftL0->vBuildSkipListSorted();
      
      //pLeftL0->vShow();
      
      // new left buffer
      EntryBuffer* pNewLeftL0 = new EntryBuffer;
      pNewLeftL0->vIndex(m_pIndex);
      pNewLeftL0->vHigherBuffer(pL1);
      
      // splice the new left buffer into the L0 list
      pLeftL0->vSpliceAfter(pNewLeftL0);
      
      // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
      
      // adjust variables
      *ppLeftL0 = pNewLeftL0;
      pLeftL0 = pNewLeftL0;
      
      pPriorEntry = pLeftL0->pLastEntry();
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // insert a copy of the right key into left buffer
    
    TRACE_COMPACT( pLeftL0->vShow(); )
    Entry* pLeftKeyEntryL0 = pLeftL0->pInsertKeyAbsL0(xLeftKeyWrk,&pPriorEntry);
    TRACE_COMPACT( pLeftKeyEntryL0->vShow(0, m_pIndex, true); )
    TRACE_COMPACT( pLeftL0->vShow(); )
    
    //pLeftL0->vShow();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // insert a first L0 key into L1
    
    if (1 == pLeftL0->nItems()) {
      
      i8 iLeftL0BufferNo = pLeftL0->iBufferNo();
      xId.vBuffer(iLeftL0BufferNo);
      xLeftKeyWrk.vId(xId);
      
      TRACE_COMPACT( pL1->vShow(); )
      //RASSERT(pL1->bVerifySkipListEntry());
      Entry* pLeftEntryL1 = pL1->pInsertKeyAbs(xLeftKeyWrk);
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
void IndexTree::vCopyLeftV2(EntryBuffer** ppLeftL0, EntryBuffer* pRightL0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  TRACE_COMPACT( vTrace("IndexTree::vCopyLeft (right %lld)\n"_Stz,
                        pRightL0->iBufferNo()); )
  
  KeyWrk xLeftKeyWrk;
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // assert the parameters are sensible
  
  EntryBuffer* pLeftL0 = *ppLeftL0;
  ASSERT(0 == pLeftL0->iBufferLevel());
  ASSERT(0 == pRightL0->iBufferLevel());
  EntryBuffer* pL1 = pLeftL0->pHigherBuffer();
  ASSERT(1 == pL1->iBufferLevel());
  ASSERT(pRightL0->pHigherBuffer() == pLeftL0->pHigherBuffer());
  
  //vTrace("IndexTree::vCopyLeft (left %lld right %lld)\n"_Stz,
  //       pLeftL0->iBufferNo(),pRightL0->iBufferNo());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru each key in right L0
  
  TRACE_COMPACT( pRightL0->vShow(); )
  
  for (Entry* pRightL0_Key = pRightL0->pFirstEntry();
       pRightL0_Key;
       pRightL0_Key = pRightL0->pNextEntry(pRightL0_Key,0)) {
    
    TRACE_COMPACT( pRightKeyEntryL0->vShow(0, m_pIndex, true); )
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the elements of the right key
    
    i8 iTxId = pRightL0_Key->iTxId();
    Id xId = pRightL0_Key->xId();
    Key* aKey = pRightL0_Key->aKey();
    i8 nKeySize = aKey->nKeySize(pIdxColVec);
    i8 nOffsets = pRightL0_Key->nOffsets();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // form the working version of the left key
    
    xLeftKeyWrk.vTxId(iTxId);
    xLeftKeyWrk.vId(xId);
    xLeftKeyWrk.vEmbedded((char*)aKey);
    xLeftKeyWrk.vKeySize(nKeySize);
    xLeftKeyWrk.vOffsets(nOffsets);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // compute the size of the left key
    
    i8 nEntryBytes = xLeftKeyWrk.nEntrySize();
    i8 nOffsetBytes = xLeftKeyWrk.nOffsetSize();
    i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get a new left buffer if not enough space in current left buffer
    
    if (! pLeftL0->bHasSpace(nEntryPlusOffsets)) {
      
      // new left buffer
      EntryBuffer* pNewLeftL0 = new EntryBuffer;
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
    Entry* pLeftKeyEntryL0 = pLeftL0->pInsertKeyAbs(xLeftKeyWrk);
    TRACE_COMPACT( pLeftKeyEntryL0->vShow(0, m_pIndex, true); )
    TRACE_COMPACT( pLeftL0->vShow(); )
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // insert a first L0 key into L1
    
    if (1 == pLeftL0->nItems()) {
      
      i8 iLeftL0BufferNo = pLeftL0->iBufferNo();
      xId.vBuffer(iLeftL0BufferNo);
      xLeftKeyWrk.vId(xId);
      
      TRACE_COMPACT( pL1->vShow(); )
      //RASSERT(pL1->bVerifySkipListEntry());
      Entry* pLeftEntryL1 = pL1->pInsertKeyAbs(xLeftKeyWrk);
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
/* copy all the elements of the right buffer to the left                      */
/*----------------------------------------------------------------------------*/
/*
void IndexTree::vCopyLeftV1(EntryBuffer** ppLeftL0, EntryBuffer* pRightL0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  TRACE_COMPACT( vTrace("IndexTree::vCopyLeft (right %lld)\n"_Stz,
                        pRightL0->iBufferNo()); )

  KeyWrk xLeftKeyWrk;
  
  EntryBuffer* pLeftL0 = *ppLeftL0;
  ASSERT(0 == pLeftL0->iBufferLevel());
  ASSERT(0 == pRightL0->iBufferLevel());
  EntryBuffer* pL1 = pLeftL0->pHigherBuffer();
  ASSERT(1 == pL1->iBufferLevel());
  ASSERT(pRightL0->pHigherBuffer() == pLeftL0->pHigherBuffer());
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  TRACE_COMPACT( pRightL0->vShow(); )
  
  // iterate thru each key entry in right L0
  for (Entry* pRightKeyEntryL0 = pRightL0->pFirstEntry();
       pRightKeyEntryL0;
       pRightKeyEntryL0 = pRightL0->pNextEntry(pRightKeyEntryL0,0)) {
    
    TRACE_COMPACT( pRightKeyEntryL0->vShow(0, m_pIndex, true); )
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the elements of the right key entry
    
    i8 iTxId = pRightKeyEntryL0->iTxId();
    Id xId = pRightKeyEntryL0->xId();
    Key* aKey = pRightKeyEntryL0->aKey();
    i8 nKeySize = aKey->nKeySize(pIdxColVec);
    i8 nOffsets = pRightKeyEntryL0->nOffsets();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // form the working version of the left key entry
    
    xLeftKeyWrk.vTxId(iTxId);
    xLeftKeyWrk.vId(xId);
    xLeftKeyWrk.vEmbedded((char*)aKey);
    xLeftKeyWrk.vKeySize(nKeySize);
    xLeftKeyWrk.vOffsets(nOffsets);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // compute the size of the left key entry
    
    i8 nEntryBytes = xLeftKeyWrk.nEntrySize();
    i8 nOffsetBytes = xLeftKeyWrk.nOffsetSize();
    i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get a new left buffer if not enough space in current left buffer
    
    if (! pLeftL0->bHasSpace(nEntryPlusOffsets)) {
      
      // new left buffer
      EntryBuffer* pNewLeftL0 = new EntryBuffer;
      pNewLeftL0->vIndex(m_pIndex);
      pNewLeftL0->vHigherBuffer(pL1);
      
      // splice the new left buffer into the L0 list
      pLeftL0->vSpliceAfter(pNewLeftL0);
      
      // adjust variables
      *ppLeftL0 = pNewLeftL0;
      pLeftL0 = pNewLeftL0;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // remove a first L0 key entry from L1
    
    if (pRightKeyEntryL0 == pRightL0->pFirstEntry()) {
      TRACE_COMPACT( pL1->vShow(); )
      bool bDeleted = pL1->bDeleteKey(aKey,pRightL0);
      RASSERT(bDeleted);
      TRACE_COMPACT( pL1->vShow(); )
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // insert a copy of the right key entry into left buffer
    
    TRACE_COMPACT( pLeftL0->vShow(); )
    Entry* pLeftKeyEntryL0 = pLeftL0->pInsertKeyAbs(xLeftKeyWrk);
    TRACE_COMPACT( pLeftKeyEntryL0->vShow(0, m_pIndex, true); )
    TRACE_COMPACT( pLeftL0->vShow(); )

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // insert a first L0 key entry into L1
    
    if (1 == pLeftL0->nItems()) {

      i8 iLeftL0BufferNo = pLeftL0->iBufferNo();
      xId.vBuffer(iLeftL0BufferNo);
      xLeftKeyWrk.vId(xId);
      
      TRACE_COMPACT( pL1->vShow(); )
      Entry* pLeftEntryL1 = pL1->pInsertKeyAbs(xLeftKeyWrk);
      TRACE_COMPACT( pLeftEntryL1->vShow(0, m_pIndex, true); )
      TRACE_COMPACT( pL1->vShow(); )
    }
    
  } // for

}
*/

/*----------------------------------------------------------------------------*/
/* dump the index                                                             */
/*----------------------------------------------------------------------------*/
void IndexTree::vDump(bool bSameLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  if (bSameLevel) {
    EntryBuffer* pEntryBuffer = m_pRootBuffer;
    
    while (pEntryBuffer) {
      pEntryBuffer->vShow();
      pEntryBuffer = pEntryBuffer->pNextBuffer();
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
EntryBuffer* IndexTree::pFirstL1Buffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  EntryBuffer* pEntryBuffer = m_pRootBuffer;
  
  if ( ! pEntryBuffer )
    return nullptr;
  
  if (pEntryBuffer->iBufferLevel() == 0)
    return nullptr;
  
  if (pEntryBuffer->iBufferLevel() == 1)
    return pEntryBuffer;
  
  while (pEntryBuffer) {
    
    Entry* pFirstEntry = pEntryBuffer->pFirstEntry();
    pEntryBuffer = pFirstEntry->pReferencedEntryBuffer();
    
    if (pEntryBuffer->iBufferLevel() == 1) {
      return pEntryBuffer;
    }
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get the first L0 buffer                                                    */
/*----------------------------------------------------------------------------*/
EntryBuffer* IndexTree::pFirstL0Buffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  EntryBuffer* pEntryBuffer = m_pRootBuffer;
  
  if ( ! pEntryBuffer )
    return nullptr;
  
  if (pEntryBuffer->iBufferLevel() == 0)
    return pEntryBuffer;
  
  while (pEntryBuffer) {
    
    Entry* pFirstEntry = pEntryBuffer->pFirstEntry();
    pEntryBuffer = pFirstEntry->pReferencedEntryBuffer();
    
    if (pEntryBuffer->iBufferLevel() == 0) {
      return pEntryBuffer;
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
i8 IndexTree::nEntryBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  i8 nBufferCount = 0;
  if (m_pRootBuffer)
    m_pRootBuffer->vBufferCount(nBufferCount);
  
  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* append a buffer into the IndexTree (should be hidden index IndexTree)      */
/*----------------------------------------------------------------------------*/
void IndexTree::vAppendBuffer(EntryBuffer* pEntryBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  // we should be appending buffers only for hidden index
  ASSERT(m_pIndex->bIsHidden());
  
  m_zLock.vGrabWrite();
  
  if (m_pRootBuffer == nullptr) {
    ASSERT( ! m_pCurrentBuffer )
    m_pRootBuffer = pEntryBuffer;
  }
  else {
    pEntryBuffer->vNextBuffer(m_pCurrentBuffer->pNextBuffer());
    pEntryBuffer->vPrevBuffer(m_pCurrentBuffer);
    m_pCurrentBuffer->vNextBuffer(pEntryBuffer);
  }
  
  m_pCurrentBuffer = pEntryBuffer;
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* insert an entry for {key,id} into an index buffer at given level           */
/*----------------------------------------------------------------------------*/
Entry* IndexTree::pAppendRow(i8 iTxId, Stz* aRow, i8 iRowLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  EntryVec xPriorVec;
  Entry* pNewEntry;
  
  m_zLock.vGrabWrite();
  
  if (m_pRootBuffer == nullptr) {
    ASSERT( ! m_pCurrentBuffer )
    m_pRootBuffer = new EntryBuffer();
    m_pRootBuffer->vIndex(m_pIndex);
    m_pRootBuffer->vPartitionNo(m_iPartitionNo);
    m_pCurrentBuffer = m_pRootBuffer;
  }
  
  RowWrk xRowWrk;
  
  xRowWrk.vTxId(iTxId);
  xRowWrk.vEmbedded((char*)aRow);
  xRowWrk.vRowSize(iRowLength);
  //xRowWrk.vShow(m_pIndex->pTable()->pColumnVec());
    
  pNewEntry = m_pCurrentBuffer->pAppendRow(xRowWrk);
  //m_pCurrentBuffer->vDump();
  if ( !pNewEntry ) {
    //----------------------------------------------------------------------------
    // create a new entry buffer and link with current buffer
    EntryBuffer* pNewBuffer = new EntryBuffer();
    pNewBuffer->vIndex(m_pIndex);
    pNewBuffer->vPartitionNo(m_iPartitionNo);
    pNewBuffer->vNextBuffer(m_pCurrentBuffer->pNextBuffer());
    pNewBuffer->vPrevBuffer(m_pCurrentBuffer);
    m_pCurrentBuffer->vNextBuffer(pNewBuffer);
    
    //----------------------------------------------------------------------------
    // toggle to the new buffer
    m_pCurrentBuffer = pNewBuffer;
    
    // now insert into new buffer
    pNewEntry = m_pCurrentBuffer->pAppendRow(xRowWrk);
    ASSERT(pNewEntry);
  }
  m_zLock.vDropWrite();
      
  return pNewEntry;
}

/*----------------------------------------------------------------------------*/
/* insert entries from entryBuffer into this index at level 0                 */
/*----------------------------------------------------------------------------*/
void IndexTree::vInsertEntriesFromBuffer(EntryBuffer* pEntryBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  //m_zLock.vGrabWrite();
  
  //pEntryBuffer->vShow();
  
  for ( Entry* pEntry = pEntryBuffer->pFirstEntry() ;
       pEntry;
       pEntry = pEntryBuffer->pNextEntry(pEntry, 0)) {
    
    bool bInserted = bInsertEntry(pEntry);
    //bVerify();
    ASSERT(bInserted);
  }
  
  //m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* insert an entry for given row into this index at level 0                   */
/*----------------------------------------------------------------------------*/
bool IndexTree::bInsertEntry(Entry* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_zLock.vGrabWrite();
  
  //static i8 nInserts = 0;
  
  //nInserts++;
  
  KeyWrk xKeyWrk;
  pRow->vExtractIndexKey(xKeyWrk, m_pIndex->pIdxColVec());
  
  xKeyWrk.vTxId(0);
  xKeyWrk.vId(pRow->xId());
  
  //if (2947 == nInserts) {
  //  xKeyWrk.vShow(m_pIndex->pIdxColVec());
  //  m_pRootBuffer->vShowTree();
  //  int iDebug = 1;
  //}
  
  if ( ! m_pRootBuffer ) {
    m_pRootBuffer = new EntryBuffer;
    m_pRootBuffer->vIndex(m_pIndex);
    m_pRootBuffer->vPartitionNo(m_iPartitionNo);
  }
  
  //bool bInserted = bInsertEntry(m_pRootBuffer, xKeyWrk, pRow->xId(), 0);
  
  Entry* pKeyEntry =
    EntryBuffer::pInsertKeyTree(xKeyWrk,
                                &m_pRootBuffer);
  ASSERT(pKeyEntry);
  
  m_zLock.vDropWrite();
  
  //if ( ! bVerify() ) {
  //  vTrace("Inserting row that caused verify to fail:\n"_Stz);
  //  pRow->vContentDump();
  //  vDump();
  //  NEVER_REACH;
  //}
  
  return true;                                                    //ALKA
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
EntryBuffer* IndexTree::pPopSubList(EntryBuffer** ppSubList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  EntryBuffer* pTopBuffer = *ppSubList;
  
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
/* position the IndexTree at a particular level of index buffer               */
/*----------------------------------------------------------------------------*/
EntryBuffer* IndexTree::pPositionLevel(KeyWrk& xKeyWrk, i8 nKeyCols,
                                       eJC eJuxtaCompare, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  if (m_pRootBuffer)
    return m_pRootBuffer->pPositionLevel(xKeyWrk.aKey(),nKeyCols,eJuxtaCompare,iLevel);
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* publish the index buffers                                                  */
/*----------------------------------------------------------------------------*/
void IndexTree::vPublish(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  // only allowing publishing of hidden index
  RASSERT(m_pIndex->bIsHidden());
  
  if ( ! m_pRootBuffer)
    return;
  
  m_zLock.vGrabWrite();
  
  // is there a current buffer ?
  if ( ! m_pCurrentBuffer)
    return;
  
  s48K sRowString;
  
  // publish the current buffer (last buffer that was added)
  for (Entry* pEntry = m_pCurrentBuffer->pFirstEntry(); pEntry != nullptr;
       pEntry = m_pCurrentBuffer->pNextEntry(pEntry, 0)) {
    Row* pRow = pEntry->aRow();
    pRow->vAsBinaryStringVLQ(sRowString, m_pIndex->pTable()->pColumnVec());
    vTrace("row: (%s)\n"_Stz, sRowString.aChar());
  }
  vTrace("Partition%lld: published %lld rows\n"_Stz,m_iPartitionNo, m_pCurrentBuffer->nItems());
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* attempt to purge oldest buffer of index tree                               */
/*----------------------------------------------------------------------------*/
bool IndexTree::bPurge(i8 iOlderThanSeconds)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  static i8 iMaxAge = 0;
  
  // only allowing purging of hidden index
  RASSERT(m_pIndex->bIsHidden());
  
  if (! m_pRootBuffer)
    return false;
  
  m_zLock.vGrabWrite();
  
  // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  // record the index tree for debugging purposes
  
  if (! gpAttachMap->m_xPurgeDebug.m_paIndexTree[m_iPartitionNo])
    gpAttachMap->m_xPurgeDebug.m_paIndexTree[m_iPartitionNo] = this;
  
  // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

  i8 iAge = m_pRootBuffer->iAgeInSeconds();
  
//  if (iAge <= 1) {
//    vTrace("Purge: age is %llds\n"_Stz,iMaxAge);
//  }
  
  if (iAge > iMaxAge) {
    iMaxAge = iAge;
    vTrace("Purge: max age now %llds\n"_Stz,iMaxAge);
  }
  
  if (iAge <= iOlderThanSeconds) {
    m_zLock.vDropWrite();
    return false;
  }
  
  // is currentBuffer same as rootBuffer ?
  if (m_pCurrentBuffer == m_pRootBuffer)
    m_pCurrentBuffer = nullptr;
  
  EntryBuffer* pEntryBuffer = m_pRootBuffer;
  
  // get next entry buffer
  m_pRootBuffer = m_pRootBuffer->pNextBuffer();
  
  // set prev pointer to null
  if (m_pRootBuffer) {
    ASSERT(pEntryBuffer ==  m_pRootBuffer->pPrevBuffer());
    m_pRootBuffer->vPrevBuffer(nullptr);
  }
  
  m_zLock.vDropWrite();
  
  // delete the roor buffer
  delete pEntryBuffer;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get root entry                                                             */
/*----------------------------------------------------------------------------*/
EntryBuffer* IndexTree::pRoot(void)
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
  
  m_zLock.vGrabWrite();
  
  if (m_pRootBuffer)
    m_pRootBuffer->vTruncate();
  
  m_pRootBuffer    = nullptr;
  m_pCurrentBuffer = nullptr;
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* verify                                                                     */
/*----------------------------------------------------------------------------*/
bool IndexTree::bVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  return m_pRootBuffer->bVerify();
}

/*----------------------------------------------------------------------------*/
/* verify index from root                                                     */
/*----------------------------------------------------------------------------*/
bool IndexTree::bVerifyIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  bool bValid = m_pRootBuffer->bVerify();
  
  return bValid;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* visualize the index                                                        */
/*----------------------------------------------------------------------------*/
void IndexTree::vDumpLevel(EntryBuffer* pEntryBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  // get the first entry
  Entry* pEntry = pEntryBuffer->pFirstEntry();
  
  while (pEntry) {
    
    // get the index buffer the first enetry poinst to
    EntryBuffer* pEntryEntryBuffer = pEntry->pReferencedEntryBuffer();
    
    // print the decile of the buffer
    vTrace("%lld"_Stz, pEntryEntryBuffer->iDecile());
    
    // insert spaces for no of entries in buffer
    if (pEntryEntryBuffer->iBufferLevel() > 0) {
      
      // exclude the head entry
      i8 iEntries = pEntryEntryBuffer->nItems() - 1;
      
      while (iEntries--)
        vTrace(" "_Stz);
    }
    
    // move to the next entry
    pEntry = pEntryBuffer->pNextEntry(pEntry, 0);
  }
}

/*----------------------------------------------------------------------------*/
/* visualize the index                                                        */
/*----------------------------------------------------------------------------*/
void IndexTree::vVisualize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  // start with the root buffer
  EntryBuffer* pEntryBuffer = m_pRootBuffer;
  
  if ( ! pEntryBuffer)
    return;
  
  // hidden index has all level 0 buffers
  if (m_pIndex->bIsHidden()) {
    
    do {
      ASSERT(pEntryBuffer->iBufferLevel() == 0);
      // print the decile of pNextBuffer
      vTrace("%lld"_Stz, pEntryBuffer->iDecile());
      
      pEntryBuffer = pEntryBuffer->pNextBuffer();
      
    } while (pEntryBuffer);
    
    vTrace("\n"_Stz);
    return;
  }

  // print the decile of the root buffer
  vTrace("%lld\n"_Stz, pEntryBuffer->iDecile());
  
  i8 iBufferLevel = pEntryBuffer->iBufferLevel();
  
  // if there is only one buffer i.e root is at level 0, we are done
  if (iBufferLevel == 0) {
    vTrace("\n"_Stz);
    return;
  }
  
  // iterate through the buffers
  while (pEntryBuffer) {
    
    // dump this buffer
    vDumpLevel(pEntryBuffer);

    // if we are at level 1, iterate thru next buffers
    if (pEntryBuffer->iBufferLevel() == 1) {
      pEntryBuffer = (EntryBuffer*)pEntryBuffer->pNextBuffer();
    }
    else {
      vTrace("\n"_Stz);
      
      // get the first entry of the buffer
      Entry* pEntry = pEntryBuffer->pFirstEntry();
      
      // get the index buffer this entry points to
      pEntryBuffer = pEntry->pReferencedEntryBuffer();
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
