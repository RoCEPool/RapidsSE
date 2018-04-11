/*============================================================================*/
/* IndexTree.cpp - index data portion of table partition                      */
/*============================================================================*/

#include "DBobjects.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IndexTree* DBobjects::_pIndexTree(void* pAddr)
{
  IndexTree* pIndexTree = reinterpret_cast<IndexTree*>(pAddr);
  
  RASSERT( !pIndexTree || CN_INDEXTREE == pIndexTree->eClass() );
  
  return pIndexTree;
}

/*----------------------------------------------------------------------------*/
/* dump data                                                                  */
/*----------------------------------------------------------------------------*/
void IndexTree::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_INDEXTREE);
  RASSERT(pObject->eClass() == CN_INDEXTREE);
  
}

/*----------------------------------------------------------------------------*/
/* finalize                                                                   */
/*----------------------------------------------------------------------------*/
void IndexTree::vFinalize(eCN eClass, Object* pObject)
{
  ASSERT(eClass == CN_INDEXTREE);
  ASSERT(pObject->eClass() == CN_INDEXTREE);
  
  IndexTree* pIndexTree = reinterpret_cast<IndexTree*>(pObject);
  ASSERT(pIndexTree->bWordBody());
  
  pIndexTree->vTidyUp();
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* IndexTree::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_INDEXTREE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void IndexTree::operator delete(void* pDataAddress)
{
  RASSERT(pDataAddress);
  IndexTree* pIndexTree = reinterpret_cast<IndexTree*>(pDataAddress);
  
  Heap::vFree(pIndexTree);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IndexTree::IndexTree(Index* pIndex, i8 iPartitionNo)
: Object(CN_INDEXTREE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_zLock.vInitLock(LK_INDEX_DATA);
  m_pIndex = pIndex;
  m_iPartitionNo = iPartitionNo;
  
  m_pRoot = nullptr;
  m_pCurrentBuffer = nullptr;
  
  _checkInitialised_(CN_INDEXTREE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IndexTree::~IndexTree(void)
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
  
  vTrace("IndexTree::vCompact\n"_Stz);
  
  for (EntryBuffer* pL1 = pFirstL1Buffer(); pL1; pL1 = pL1->pNextBuffer()) {
    
    vTrace("  Compacting L1 %p\n"_Stz,pL1);
    
    EntryBuffer* pLeftL0 = pL1->pFirstChildBuffer();
    
    EntryBuffer* pSubList = pLeftL0->pUnlinkSubList();
    
    while (EntryBuffer* pRightL0 = pPopSubList(&pSubList)) {
   
      vCopyLeft(&pLeftL0,pRightL0);
      
      delete pRightL0;
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* copy left (from src to dst)                                                */
/*----------------------------------------------------------------------------*/
void IndexTree::vCopyLeft(EntryBuffer** ppLeftL0, EntryBuffer* pRightL0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* dump the index                                                             */
/*----------------------------------------------------------------------------*/
void IndexTree::vDump(bool bSameLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  if (bSameLevel) {
    EntryBuffer* pEntryBuffer = m_pRoot;
    
    while (pEntryBuffer) {
      pEntryBuffer->vShow();
      pEntryBuffer = pEntryBuffer->pNextBuffer();
    }
  }
  else {
    // start with the root
    if (m_pRoot)
      m_pRoot->vDump();
  }
}

/*----------------------------------------------------------------------------*/
/* get the first L1 buffer                                                    */
/*----------------------------------------------------------------------------*/
EntryBuffer* IndexTree::pFirstL1Buffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  EntryBuffer* pEntryBuffer = m_pRoot;
  
  if ( ! pEntryBuffer )
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
  
  EntryBuffer* pEntryBuffer = m_pRoot;
  
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
  if (m_pRoot)
    m_pRoot->vBufferCount(nBufferCount);
  
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
  
  if (m_pRoot == nullptr) {
    ASSERT( ! m_pCurrentBuffer )
    m_pRoot = pEntryBuffer;
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
void IndexTree::vAppendEntriesFromBuffer(i8 iTxId,
                                         WorkingBuffer* pWorkingBuffer,
                                         EntryBuffer** ppFirstBuffer,
                                         Entry** ppFirstEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  EntryVec xPriorVec;
  Entry* pNewEntry;
  
  *ppFirstBuffer = nullptr;
  *ppFirstEntry = nullptr;
  
  m_zLock.vGrabWrite();
  
  WorkingRow* pRow;

  if (m_pRoot == nullptr) {
    ASSERT( ! m_pCurrentBuffer )
    m_pRoot = new EntryBuffer();
    m_pRoot->vIndex(m_pIndex);
    m_pRoot->vPartitionNo(m_iPartitionNo);
    m_pCurrentBuffer = m_pRoot;
  }
  
  for ( pRow = pWorkingBuffer->pFirstRow() ; pRow;
       pRow = pWorkingBuffer->pNextRow(pRow)) {
    
    //pRow->vShow(m_pIndex->pTable()->pColumnVec());
    RowWrk xRowWrk;
    
    xRowWrk.vTxId(iTxId);
    xRowWrk.vEmbedded((char*)pRow->aRow());
    xRowWrk.vRowSize(pRow->nRowSize());
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
    
    if ( ! *ppFirstBuffer )
      *ppFirstBuffer = m_pCurrentBuffer;
    
    if ( ! *ppFirstEntry )
      *ppFirstEntry = pNewEntry;
    
  }
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* append an entry into this index at level 0                                 */
/*----------------------------------------------------------------------------*/
//void IndexTree::vAppendEntry(KeyWrk& xKeyWrk,
//                             EntryVec& xPriorVec)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
//
//  // this is invoked during ld while building non-hidden indexes
//  ASSERT( ! m_pIndex->bIsHidden() );
//
//  m_zLock.vGrabWrite();
//
//  //m_pCurrentBuffer = pAppendL0Entry(m_pCurrentBuffer,xKeyWrk,xId);
//
//  if ( ! m_pRoot ) {
//    m_pRoot = new EntryBuffer;
//    m_pRoot->vIndex(m_pIndex);
//    m_pRoot->vPartitionNo(m_iPartitionNo);
//  }
//
//  //vTrace("\nInserting key: "_Stz);
//  //xKeyWrk.aKey()->vShow(m_pIndex->pIdxColVec());
//
//  Entry* pKeyEntry =
//    EntryBuffer::pInsertKeyTree(xKeyWrk,
//                                &m_pRoot);
//
//  //m_pRoot->vShowTree(true);
//
//  m_zLock.vDropWrite();
//}

/*----------------------------------------------------------------------------*/
/* insert entries from entryBuffer into this index at level 0                 */
/*----------------------------------------------------------------------------*/
void IndexTree::vInsertEntriesFromBuffer(EntryBuffer* pEntryBuffer,
                                         Entry* pRowEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  //m_zLock.vGrabWrite();
  
  //pEntryBuffer->vShow();
  
  for ( Entry* pEntry = pRowEntry ;
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
  //  m_pRoot->vShowTree();
  //  int iDebug = 1;
  //}
  
  if ( ! m_pRoot ) {
    m_pRoot = new EntryBuffer;
    m_pRoot->vIndex(m_pIndex);
    m_pRoot->vPartitionNo(m_iPartitionNo);
  }
  
  //bool bInserted = bInsertEntry(m_pRoot, xKeyWrk, pRow->xId(), 0);
  
  Entry* pKeyEntry =
    EntryBuffer::pInsertKeyTree(xKeyWrk,
                                &m_pRoot);
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
  
  m_pRoot->vLazyBuild();
}

/*----------------------------------------------------------------------------*/
/* pop entry buffer from sub list                                             */
/*----------------------------------------------------------------------------*/
EntryBuffer* IndexTree::pPopSubList(EntryBuffer** ppSubList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  EntryBuffer* pTopBuffer = *ppSubList;
  
  *ppSubList = (*ppSubList)->pNextBuffer();
  
  pTopBuffer->vNextBuffer(nullptr);
  pTopBuffer->vPrevBuffer(nullptr);
  
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
  
  if (m_pRoot)
    return m_pRoot->pPositionLevel(xKeyWrk.aKey(),nKeyCols,eJuxtaCompare,iLevel);
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get root entry                                                             */
/*----------------------------------------------------------------------------*/
EntryBuffer* IndexTree::pRoot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  return m_pRoot;
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
  
  if (m_pRoot)
    m_pRoot->vTruncate();
  
  m_pRoot = nullptr;
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* verify                                                                     */
/*----------------------------------------------------------------------------*/
bool IndexTree::bVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  return m_pRoot->bVerify();
}

/*----------------------------------------------------------------------------*/
/* verify index from root                                                     */
/*----------------------------------------------------------------------------*/
bool IndexTree::bVerifyIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  bool bValid = m_pRoot->bVerify();
  
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
  EntryBuffer* pEntryBuffer = m_pRoot;
  
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
