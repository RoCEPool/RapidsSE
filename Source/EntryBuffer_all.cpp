/*============================================================================*/
/* EntryBuffer_all.cpp -- working file to sort out the fun's of Item PoolBuffer  */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* assert valid entry buffer                                                  */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vAssertEntryBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* test for valid entry buffer                                                */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bValidEntryBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (! bValidMagic()) {
    vTrace("Not valid magic\n"_Stz);
    return false;
  }
  
  if (CN_ENTRYBUFFER != eClass()) {
    vTrace("Not valid class\n"_Stz);
    return false;
  }
  
  if (! aFooter()->bValidFooter()) {
    vTrace("Not valid footer\n"_Stz);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Complex Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyze this buffer                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vAnalyze(s48K& sAnalysis, bool bDetailed,
                           i8& iBuffers, i8& iEntries)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (m_pIndex->bIsHidden()) {
    
    ASSERT(m_iBufferHeight == 0);
    EntryBuffer* pNextBuffer = this;
    
    do {
      ASSERT(pNextBuffer->iBufferHeight() == 0);
      
      iBuffers++;
      iEntries += (pNextBuffer->m_nItems);
      
      if (bDetailed)
        sAnalysis.vCatSprint("row: (%s, %lld, %5lld, 'L%lld', %lld )\n"_Stz,
                             gpAttachMap->m_pGrid->pThisNode()->aNodeName(),
                             pNextBuffer->m_iPartitionNo,
                             iBuffers,
                             pNextBuffer->m_iBufferHeight,
                             pNextBuffer->m_nItems);
      
      pNextBuffer = pNextBuffer->pNextBuffer();
    } while (pNextBuffer);
    
    return;
  }
  
  iBuffers++;
  iEntries += m_nItems;
  
  if (bDetailed)
    sAnalysis.vCatSprint(" row: (%s, %lld, %5lld,' L%lld', %lld)\n"_Stz,
                         gpAttachMap->m_pGrid->pThisNode()->aNodeName(),
                         m_iPartitionNo,
                         iBuffers,
                         m_iBufferHeight,
                         m_nItems);
  
  // recursively dump the lower level buffers
  if (m_iBufferHeight != 0) {
    
    for (Item* aItem = aLeadEntry();
         aItem != nullptr;
         aItem = aNextEntry(aItem, 0)) {
      
      // get the index buffer from the entry
      EntryBuffer* pEntryBuffer = EntryBuffer::pRefdEntryBuffer(aItem);
      
      ASSERT(pEntryBuffer);
      ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
      
      pEntryBuffer->vAnalyze(sAnalysis, bDetailed, iBuffers, iEntries);
      
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* delete the given entry                                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDeleteEntry(Item** aaItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // currently we only delete from non-hidden indexes
  ASSERT(! m_pIndex->bIsHidden());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Item* aItem = *aaItem;
  
  Item* xPriorVec[gpAttachMap->m_nIndexLevels];
  
  //KeyData* aKey = _aKeyData(aItem->aDataArea());
  
  vPositionPriorVecEntry(aItem, xPriorVec, aItem);
  
  vCutEntry(aItem,xPriorVec);
  
  vFreeItem(aItem);
  
  // entry no longer exists, set the pointer to be null
  *aaItem = nullptr;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // show this buffer
  vShow();
  
  // recursively dump the lower level buffers
  if (m_iBufferHeight != 0) {
    
    for (Item* aItem = aLeadEntry();
         aItem != nullptr;
         aItem = aNextEntry(aItem, 0)) {
      
      // get the index buffer from the entry
      EntryBuffer* pEntryBuffer = EntryBuffer::pRefdEntryBuffer(aItem);
      
      ASSERT(pEntryBuffer);
      ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
      
      pEntryBuffer->vDump();
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get first child buffer                                                     */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pFirstChildBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
  
  //if (m_aLazyChain)
  //  vBuildSkipList();
#endif
  
  Item* aFirstEntry = this->aLeadEntry();
  EntryBuffer* pChildEntryBuffer = EntryBuffer::pRefdEntryBuffer(aFirstEntry);
  
  return pChildEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* get length of list of buffers                                               */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::iLengthList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 iLength = 0;
  for (EntryBuffer* pLn = this; pLn; pLn = pLn->pNextBuffer())
    iLength++;
  
  return iLength;
}

/*----------------------------------------------------------------------------*/
/* get length of sublist of buffers                                           */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::iLengthSubList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 iLength = 0;
  
  EntryBuffer* pThisL0 = this;
  EntryBuffer* pThisL1 = pThisL0->pHigherBuffer();
  
  EntryBuffer* pL0;
  for (pL0 = pThisL0;
       pL0 && pL0->pHigherBuffer() == pThisL1;
       pL0 = pL0->pNextBuffer()) {
    
    iLength++;
  }
  
  return iLength;
}

/*----------------------------------------------------------------------------*/
/* get next entry                                                             */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aNextEntry(Item* aCurrent, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT(aCurrent->bValidItem());
  
  i4 i_Offset = aCurrent->iNEO(iLevel);
  
  if (-1 == i_Offset)
    return nullptr;
  
  Item* aItem = aAdd<Item*>(aItemArea(), i_Offset);
  
  RASSERT(! aItem || aItem->bValidItem());

  return aItem;
}

/*----------------------------------------------------------------------------*/
/* reset/empty the buffer                                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT(nullptr == m_i4aItemOffsets);
  RASSERT(! m_pIndex->bIsHidden());
  RASSERT(1 == m_iBufferHeight);
  
  vResetUsed();
  
  m_xSkipRoots.m_iaNextOffset[0] = -1;
  m_xSkipRoots.m_iaNextOffset[1] = -1;
  m_xSkipRoots.m_iaNextOffset[2] = -1;

  m_i_FreeOffsetChain = C_NO_MORE_ITEMS;
  
  //m_aHeadEntry  = nullptr;
  m_aLastEntry  = nullptr;
  m_aFreeItemChain  = nullptr;
  
  m_aLazyChain  = nullptr;
  
  //vHeadEntryNew();
}

/*----------------------------------------------------------------------------*/
/* restore the buffer after being received over a socket                      */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vRestore(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT( ! m_pHigherBuffer );
  RASSERT( ! m_pNextBuffer );
  RASSERT( ! m_pPrevBuffer );
  
  // set the index to nullptr (this pointer is not valid any more)
  m_pIndex = nullptr;
  
  
  // get the original buffer address
  //  nb: the given address has to be an address in the orginal buffer
  PoolBuffer* pOriginalBuffer =
    pBitAnd<PoolBuffer*>(m_i4aItemOffsets,C_OBJECT_TO_BUFFER_MASK);
  
  // get this buffer address
  PoolBuffer* pThisBuffer =
    pBitAnd<PoolBuffer*>(this,C_OBJECT_TO_BUFFER_MASK);
  
  // calculate offset for m_i4aItemOffsets and set it
  if (m_i4aItemOffsets) {
    PoolBuffer* pOriginalBuffer =
      pBitAnd<PoolBuffer*>(m_i4aItemOffsets,C_OBJECT_TO_BUFFER_MASK);
    u8 uOffset = nSub(m_i4aItemOffsets, pOriginalBuffer);
    m_i4aItemOffsets = aBitOr<i4*>(pThisBuffer,uOffset);
  }
  
  // calculate offset for m_aHeadEntry and set it
  //uOffset = nSub(m_aHeadEntry, pOriginalBuffer);
  //m_aHeadEntry = pBitOr<Item*>(pThisBuffer,uOffset);
  
  // calculate offset for m_aLastEntry and set it
  if (m_aLastEntry) {
    PoolBuffer* pOriginalBuffer =
      pBitAnd<PoolBuffer*>(m_aLastEntry,C_OBJECT_TO_BUFFER_MASK);
    u8 uOffset = nSub(m_aLastEntry, pOriginalBuffer);
    m_aLastEntry = pBitOr<Item*>(pThisBuffer,uOffset);
  }
  
  // calculate offset for m_aFreeItemChain and set it
  if (m_aFreeItemChain) {
    PoolBuffer* pOriginalBuffer =
      pBitAnd<PoolBuffer*>(m_aFreeItemChain,C_OBJECT_TO_BUFFER_MASK);
    u8 uOffset = nSub(m_aFreeItemChain, pOriginalBuffer);
    m_aFreeItemChain = pBitOr<FreeItem*>(pThisBuffer,uOffset);
  }
  
  // calculate offset for m_aLazyChain and set it
  if (m_aLazyChain) {
    PoolBuffer* pOriginalBuffer =
      pBitAnd<PoolBuffer*>(m_aLazyChain,C_OBJECT_TO_BUFFER_MASK);
    u8 uOffset = nSub(m_aLazyChain, pOriginalBuffer);
    m_aLazyChain = pBitOr<Item*>(pThisBuffer,uOffset);
  }
  
  // get the buffer no
  bn nBuffer = nPoolBufferNo();
  
  // restore the bufferNo
  m_iBufferNo = *nBuffer;
  
  // now restore the row id's to point to this buffer no
  for (Item* aItem = aLeadEntry();
       aItem != nullptr;
       aItem = aNextEntry(aItem, 0)) {
    
    Id xId(*nBuffer, aItem->iItemNo(), aItem->nLevels());
    aItem->vId(xId);
  }
  
}

/*----------------------------------------------------------------------------*/
/* restore the index of the index buffer                                      */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vRestoreIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT( ! m_pIndex );
  m_pIndex = pIndex;
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vShow(Item* aMarkEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  s256 sHigherBuffer;
  if (m_pHigherBuffer)
    StrLib::wToString((i8)((EntryBuffer*)m_pHigherBuffer)->m_iBufferHeight,
                      sHigherBuffer.aChar());
  else
    sHigherBuffer = "..."_Stz;
  
  vTrace("\n"_Stz);
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  vTrace("EntryBuffer: %p (buffer %lld): items: %lld used: %lld freespace %lld\n"_Stz,
         this,*nPoolBufferNo(),nItems(),nUsedBytes(),nFreeBytes());
  
  vTrace("  higherBuffer %p (L%s) prevBuffer %p nextBuffer %p\n"_Stz,
         m_pHigherBuffer,sHigherBuffer.aStz(),m_pPrevBuffer,m_pNextBuffer);
  
  vTrace("  index: %s partition: %lld height: %lld\n"_Stz,
         m_pIndex ? m_pIndex->aIndexName() : "NULL"_Stz,m_iPartitionNo,m_iBufferHeight);
  
  vTrace("  entryOffset: %p freeOffsetHead: %lld\n"_Stz,
         m_i4aItemOffsets,m_i_FreeOffsetChain);
  
  vTrace("  headEntry: %p[%d,%d,%d] lastEntry: %p firstFree: %p\n"_Stz,
         aSkipRoots(),aSkipRoots() ? aSkipRoots()->iNEO(0) : 0,
         aSkipRoots() ? aSkipRoots()->iNEO(1) : 0,
         aSkipRoots() ? aSkipRoots()->iNEO(2) : 0,
         m_aLastEntry,m_aFreeItemChain);
  
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  ASSERT(aSkipRoots());
  
  vTrace("\n"_Stz);
  vTrace("Entries (by headEntry vector[0]):\n"_Stz);
  
  i8 iOffset = aSkipRoots()->iNEO(0);
  while (iOffset > -1) {
    Item* aItem = aGetEntryOffset(iOffset);
    vShowEntry(aItem);
    if (aMarkEntry == aItem)
      vTrace("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
             "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n"_Stz);
    
    iOffset = aItem->iNEO(0);
  }
  
  vTrace("\n"_Stz);
  vTrace("=================================================="
         "==================================================\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* show short form of buffer                                                  */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vShowShort(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  vTrace("[t%lld h%lld: "_Stz,
         *nPoolBufferNo(),
         pHigherBuffer() ? *pHigherBuffer()->nPoolBufferNo() : 0);
  
  i8 iOffset = aSkipRoots()->iNEO(0);
  while (iOffset > -1) {
    Item* aItem = aGetEntryOffset(iOffset);
    
    KeyData* aKey = _aKeyData(aItem->aDataArea())
    ;
    i8 iVal = aKey->iGetInt();
    
    if (m_iBufferHeight)
      vTrace("%lld(%lld)"_Stz,iVal,*EntryBuffer::pRefdEntryBuffer(aItem)->nPoolBufferNo());
    else
      vTrace("%lld|%lld|"_Stz,iVal,*EntryBuffer::pRefdEntryBuffer(aItem)->nPoolBufferNo());
    
    iOffset = aItem->iNEO(0);
    if (iOffset > -1)
      vTrace(","_Stz);
  }
  vTrace("]"_Stz);
}

/*----------------------------------------------------------------------------*/
/* show sublist of buffers                                                    */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vShowSubList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  EntryBuffer* pThisL0 = this;
  EntryBuffer* pThisL1 = pThisL0->pHigherBuffer();
  
  vTrace("{"_Stz);
  
  if (pThisL0->pPrevBuffer())
    vTrace("<-"_Stz);
  
  EntryBuffer* pL0;
  for (pL0 = pThisL0;
       pL0 && pL0->pHigherBuffer() == pThisL1;
       pL0 = pL0->pNextBuffer()) {
    
    if (pL0 != pThisL0)
      vTrace(","_Stz);
    
    vTrace("%lld"_Stz,pL0->iPoolBufferNo());
  }
  
  if (pL0)
    vTrace("->"_Stz);
  
  vTrace("}\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* show contents of all buffers in index tree                                 */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vShowTree(bool bDumpBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (! m_pHigherBuffer )
    vTrace("\nIndex Tree:\n"_Stz,m_iBufferHeight);
  
  vTrace("L%lld"_Stz,m_iBufferHeight);
  
  for (EntryBuffer* pBuffer = this;
       pBuffer;
       pBuffer = pBuffer->pNextBuffer()) {
    
    pBuffer->vShowShort();
    
    ASSERT(! pBuffer->pPrevBuffer() || pBuffer == pBuffer->pPrevBuffer()->pNextBuffer());
    ASSERT(! pBuffer->pNextBuffer() || pBuffer == pBuffer->pNextBuffer()->pPrevBuffer());
  }
  
  vTrace("\n"_Stz);
  
  if (bDumpBuffer) {
    for (EntryBuffer* pBuffer = this;
         pBuffer;
         pBuffer = pBuffer->pNextBuffer())
      pBuffer->vShow();
  }
  
  if ( ! m_iBufferHeight )
    return;
  
  Item* aFirstEntry = this->aLeadEntry();
  if (! aFirstEntry)
    return;
  
  Id xId;
  xId = aFirstEntry->xId();
  bn nBuffer = xId.iBuffer();
  
  HeapBuffer* pHeapBuffer =
  reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(nBuffer));
  
  EntryBuffer* pChildBuffer =
  reinterpret_cast<EntryBuffer*>(pHeapBuffer->aFirstChunk());
  
  pChildBuffer->vShowTree(bDumpBuffer);
}

/*----------------------------------------------------------------------------*/
/* splice the given buffer after this buffer                                  */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vSpliceAfter(EntryBuffer* pSplicedBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  EntryBuffer* pThisBuffer = this;
  EntryBuffer* pNextBuffer = pThisBuffer->pNextBuffer();
  
  // the spliced buffer is always inserted after this buffer
  pThisBuffer->vNextBuffer(pSplicedBuffer);
  pSplicedBuffer->vNextBuffer(pNextBuffer);
  pSplicedBuffer->vPrevBuffer(pThisBuffer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // circumstance (a), no next buffer
  
  if (! pNextBuffer)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // circumstance (b), next buffer
  
  pNextBuffer->vPrevBuffer(pSplicedBuffer);
}

/*----------------------------------------------------------------------------*/
/* unlink the sublist of EB's following this L0 EB within this L1             */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pUnlinkSubList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(0 == m_iBufferHeight);
  
  EntryBuffer* pThisL0 = this;
  EntryBuffer* pThisL1 = pThisL0->pHigherBuffer();
  EntryBuffer* pNextL0 = pThisL0->pNextBuffer();
  EntryBuffer* pNextL1 = pThisL1->pNextBuffer();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // circumstance (a) no more L0
  
  if (! pNextL0)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // circumstance (b) only one L0 under L1
  
  if (pNextL0->pHigherBuffer() != pHigherBuffer())
    return nullptr;
  
  //----------------------------------------------------------------------------
  //
  // - there is a sublist
  //
  //----------------------------------------------------------------------------
  
  EntryBuffer* pThisL0SubList = pNextL0;
  
  // the sublist no longer points back to this L0
  pThisL0SubList->vPrevBuffer(nullptr);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // circumstance (c) last L1, there is a sublist
  
  if (! pNextL1) {
    
    // last L1, therefore the next chain terminates here
    pThisL0->vNextBuffer(nullptr);
    
    return pThisL0SubList;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // circumstance (d) middle L1, there is a sublist
  
  EntryBuffer* pFirstL0ofNextL1 = pNextL1->pFirstChildBuffer();
  EntryBuffer* pLastL0ofThisL1 = pFirstL0ofNextL1->pPrevBuffer();
  
  // the next chain points to first L0 buffer of next L1
  pThisL0->vNextBuffer(pFirstL0ofNextL1);
  
  // the prev chain of first L0 of next L1 is this L0
  pFirstL0ofNextL1->vPrevBuffer(pThisL0);
  
  // the prev chain of last L0 of sublist terminates
  pLastL0ofThisL1->vNextBuffer(nullptr);
  
  return pThisL0SubList;
}

/*----------------------------------------------------------------------------*/
/* verify the structure of this buffer (and its children)                     */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  bool bValid = false;
  
  if (m_iBufferHeight) {
    bValid = bVerifyLnAny();
    return bValid;
  }
  
  if (! m_pIndex->bIsHidden()) {
    bValid = bVerifyL0Other();
    return bValid;
  }
  
  for (EntryBuffer* EBhiddenL0 = this;
       EBhiddenL0;
       EBhiddenL0 = EBhiddenL0->pNextBuffer()) {
    bValid = EBhiddenL0->bVerifyL0Hidden();
    if (! bValid)
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compact this buffer                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vCompact(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  //RASSERT( ! m_aLazyChain );
  
  if (m_aLazyChain)
    vBuildSkipList();
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  FreeItem* aFreeEntry;
  Item*     aMoveEntry;
  Item*     xPriorVec[gpAttachMap->m_nIndexLevels];
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  // we can only compact if there are free entries
  if (! m_aFreeItemChain)
    return;
  
  // loose the free list so vShow() works correctly
  m_aFreeItemChain = nullptr;
  
  // scan item is the working pointer
  Item* aScanItem = aFirstItem();
  //ASSERT(aScanItem->eClass() == CN_ENTRY);
  
  // loop while we can still reclaim more space
  for (;;) {
    
    if (m_iBufferHeight) {
      //vShow();
      int iDebug = 1;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // find the next free entry in buffer
    
    aFreeEntry = nullptr;
    for (;
         aScanItem != nullptr;
         aScanItem = aAdjacentItem(aScanItem)) {
      
      eCN eClass = aScanItem->eClass();
      
      if (CN_FREEENTRY == eClass) {
        aFreeEntry = _aFreeItem(aScanItem);
        break;
      }
    }
    
    // return if no more free entries
    if (! aFreeEntry)
      break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // find the next entry in buffer after the free entry
    
    aMoveEntry = nullptr;
    for (Item* aItem = aAdjacentItem(aFreeEntry);
         aItem != nullptr;
         aItem = aAdjacentItem(aItem)) {
      
      eCN eClass = aItem->eClass();
      
      RASSERT ( (eClass == CN_ENTRY) ||
               (eClass == CN_FREEENTRY) ||
               (eClass == CN_WILDERNESS));
      
      
      if (CN_ENTRY == eClass) {
        aMoveEntry = static_cast<Item*>(aItem);
        break;
      }
    }
    
    // return if no more entries after the free entry
    if (! aMoveEntry)
      break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the prior vector to the entry being moved
    
    //KeyData* aMoveKey = _aKeyData(aMoveEntry->aDataArea());
    
    //aKey->vShow(pIdxColVec);
    //vTrace("\n"_Stz);
    
    vPositionPriorVecEntry(aMoveEntry, xPriorVec, aMoveEntry);
    
    if (m_iBufferHeight) {
      //vDumpPriorVec(xPriorVec);
      int iDebug = 1;
    }
    
    i4 iWasOffset = iToOffset(aMoveEntry);
    i4 iNowOffset = iToOffset(aFreeEntry);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // now copy move entry left onto free entry
    
    // calculate the number of bytes to copy
    i8 nCopyBytes = aMoveEntry->nSizeItem_WithoutOffsets()
                      + aMoveEntry->iNEOsize();
    
    // copy the move entry over the free entry
    MemLib::vMove(aFreeEntry, aMoveEntry, nCopyBytes, CN_ENTRYBUFFER);
    
    // give the reclaimed space back to the buffer
    i8 nReclaimedBytes = nSub(aMoveEntry,aFreeEntry);
    //vGiveSpace(nReclaimedBytes);
    
    // convert the moved entry to a free entry
    FreeItem* aOpenedFreeEntry = aAdd<FreeItem*>(aFreeEntry,nCopyBytes);
    new(aOpenedFreeEntry) FreeItem(nReclaimedBytes);
    
    Item* aCheckEntry = reinterpret_cast<Item*>(aFreeEntry);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the prior vector to the entry being moved
    
    //vDumpPriorVec(xPriorVec);
    for (i8 i_Offset = 0; i_Offset < 3; i_Offset++) {
      Item* aPriorEntry = xPriorVec[i_Offset];
      i4 iPriorOffset = aPriorEntry->iNEO(i_Offset);
      if (iWasOffset == iPriorOffset)
        aPriorEntry->vNEO(i_Offset,iNowOffset);
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move scan
    
    aScanItem = aFreeEntry;
    
  } // for
  
  ASSERT( ! aScanItem || CN_FREEENTRY == aScanItem->eClass() );
  
  if (aScanItem)
    vSetUsed(aScanItem);
  
  //vShow();
}

/*----------------------------------------------------------------------------*/
/* cut entry from skip list                                                   */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vCutEntry(Item* aItem, Item* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nLevels = aItem->nLevels();
  
  for (i8 i_Level = 0; i_Level < nLevels; i_Level++) {
    i4 iNEO = aItem->iNEO(i_Level);
    aPriorVec[i_Level]->vNEO(i_Level,iNEO);
  }
}

/*----------------------------------------------------------------------------*/
/* dump the given prior vector                                                */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDumpPriorVec(Item* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  vTrace("Prior Vector\n"_Stz);
  
  vTrace("  0:%p 1:%p 2:%p\n"_Stz,
         aPriorVec[0],aPriorVec[1],aPriorVec[2]);
  
  for (i8 i = 0; i < 3; i++) {
    Item* aItem = aPriorVec[i];
    for (i8 i_Level = 0; i_Level < aItem->nLevels(); i_Level++) {
      vTrace("  i : %lld Item : %p i_Offset : %lld offset : %lld pointsTo : %p\n"_Stz,
             i,
             aItem,
             i_Level,
             aItem->iNEO(i_Level),
             aGetEntryOffset(aItem->iNEO(i_Level)));
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get an entry by offset                                                     */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aGetEntryOffset(i8 iOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Item* aItem = aAdd<Item*>(aItemArea(), iOffset);
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* get split entry by split number                                            */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aGetEntrySplit(i8 iSplitEntryNo,Item** aaPriorEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(aSkipRoots());
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  *aaPriorEntry = nullptr;
  
  i8 iEntryNo = 0;
  for (Item* aSplitEntry = aNextEntry(aSkipRoots(),0);
       aSplitEntry;
       aSplitEntry = aNextEntry(aSplitEntry,0)) {
    
    // found split point?
    if (iEntryNo++ == iSplitEntryNo)
      return aSplitEntry;
    
    *aaPriorEntry = aSplitEntry;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* restore item count                                                         */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vItemCountRestore(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_nItems = 0;
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem, 0))
    m_nItems++;
}

/*----------------------------------------------------------------------------*/
/* convert given entry into free entry                                        */
/*----------------------------------------------------------------------------*/
FreeItem* EntryBuffer::aMakeFreeEntry(Item* aItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nSizeItem = aItem->nSizeItem_WithoutOffsets();
  i8 nSizeNEO  = aItem->iNEOsize();
  i8 nSizeFree = nSizeItem + nSizeNEO;
  
  FreeItem* aFreeItem = new(aItem) FreeItem(nSizeFree);
  
  return aFreeItem;
}

/*----------------------------------------------------------------------------*/
/* move to exact key entry                                                    */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::pMoveToExactEntry(Item* aItem, KeyWrk& xKeyWrk, EntryBuffer** paEntryBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (aItem == aSkipRoots())
    aItem = aNextEntry(aItem,0);
    
  eKC eCompare = KeyData::eCompare(aItem, xKeyWrk);
  
  // have we reached the key?
  if (eCompare == KC_SAME)
    return aItem;
  
  // have we moved beyond ?
  if (eCompare == KC_AFTER)
    return nullptr;
  
  aItem = aNextEntry(aItem,0);
  
  if ( ! aItem ) {
    
    // move to the next buffer
    *paEntryBuffer = pNextBuffer();
    
    if (*paEntryBuffer) {
      return (*paEntryBuffer)->pMoveToExactEntry((*paEntryBuffer)->aSkipRoots(), xKeyWrk, paEntryBuffer);
    }
    
  }
  

  return aItem;
}

/*----------------------------------------------------------------------------*/
/* move to next key entry                                                     */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::pMoveToGreaterThanEntry(Item* aItem, KeyWrk& xKeyWrk, EntryBuffer** paEntryBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (aItem == aSkipRoots())
    aItem = aNextEntry(aItem,0);
  
  while (aItem) {
    eKC eCompare = KeyData::eCompare(aItem, xKeyWrk);
    
    // have we moved beyond ?
    if (eCompare == KC_AFTER)
      break;
    
    aItem = aNextEntry(aItem,0);
    
    if ( ! aItem) {
      
      // move to the next buffer
      *paEntryBuffer = pNextBuffer();
      
      if (*paEntryBuffer) {
        return (*paEntryBuffer)->pMoveToGreaterThanEntry((*paEntryBuffer)->aSkipRoots(), xKeyWrk, paEntryBuffer);
      }
      else
        break;
    }
  }
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* move to next key entry                                                     */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::pMoveToGreaterEqualEntry(Item* aItem, KeyWrk& xKeyWrk, EntryBuffer** paEntryBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (aItem == aSkipRoots())
    aItem = aNextEntry(aItem,0);
  
  while (aItem) {
    eKC eCompare = KeyData::eCompare(aItem, xKeyWrk);
    
    // have we reached the key?
    if (eCompare == KC_SAME)
      break;
    
    // have we moved beyond ?
    if (eCompare == KC_AFTER)
      break;
    
    aItem = aNextEntry(aItem,0);
    
    if (!aItem) {
      
      // move to the next buffer
      *paEntryBuffer = pNextBuffer();
      
      if (*paEntryBuffer) {
        return (*paEntryBuffer)->pMoveToGreaterEqualEntry((*paEntryBuffer)->aSkipRoots(), xKeyWrk, paEntryBuffer);
      }
    }
    else
      break;
  }
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* move to next key entry                                                     */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::pMoveToNextEntry(Item* aItem, KeyWrk& xKeyWrk, EntryBuffer** paEntryBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  aItem = aNextEntry(aItem,0);
  
  if (aItem)
    return aItem;
  
  while( ! aItem) {
    
    // move to the next buffer
    *paEntryBuffer = pNextBuffer();
    
    if (*paEntryBuffer) {
      return (*paEntryBuffer)->pMoveToNextEntry((*paEntryBuffer)->aSkipRoots(), xKeyWrk, paEntryBuffer);
    }
    else
      break;
  }
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* pop entry from lazy chain                                                  */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aPopLazyChain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (! m_aLazyChain )
    return nullptr;
  
  Item* aPoppedLazyEntry = m_aLazyChain;
  
  i8 iNextLazyOffset = aPoppedLazyEntry->iNEO(0);
  
  if (-1 == iNextLazyOffset)
    m_aLazyChain = nullptr;
  else
    m_aLazyChain = aGetEntryOffset(iNextLazyOffset);
  
  return aPoppedLazyEntry;
}

/*----------------------------------------------------------------------------*/
/*-- Verify ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* check prior vec ordering                                                   */
/*----------------------------------------------------------------------------*/
#ifdef WITH_DEBUG_VERSION

bool EntryBuffer::bCheckPriorVec(Item* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  vNumberEntries();
  
  Item* aPriorEntry_0 = aPriorVec[0];
  Item* aPriorEntry_1 = aPriorVec[1];
  Item* aPriorEntry_2 = aPriorVec[2];
  
  Item* aNextEntry_0  = aNextEntry(aPriorEntry_0,0);
  Item* aNextEntry_1  = aNextEntry(aPriorEntry_1,1);
  Item* aNextEntry_2  = aNextEntry(aPriorEntry_2,2);
  
  if (aNextEntry_0 && aNextEntry_1) {
    if (aNextEntry_0->m_iSkipNo > aNextEntry_1->m_iSkipNo) {
      vVerifyFault();
      return false;
    }
  }
  
  if (aNextEntry_1 && aNextEntry_2) {
    if (aNextEntry_1->m_iSkipNo > aNextEntry_2->m_iSkipNo) {
      vVerifyFault();
      return false;
    }
  }
  
  if (aNextEntry_0 && aNextEntry_2) {
    if (aNextEntry_0->m_iSkipNo > aNextEntry_2->m_iSkipNo) {
      vVerifyFault();
      return false;
    }
  }
  
  return true;
}
#endif

/*----------------------------------------------------------------------------*/
/* number entries                                                             */
/*----------------------------------------------------------------------------*/
#ifdef WITH_DEBUG_VERSION
void EntryBuffer::vNumberEntries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // set entry no
  
  i8 iEntryNo = 0;
  
  for (Item* aTestEntry_0 = aNextEntry(aSkipRoots(),0);
       aTestEntry_0;
       aTestEntry_0 = aNextEntry(aTestEntry_0,0))
    
    aTestEntry_0->m_iSkipNo = iEntryNo++;
  
}
#endif

/*----------------------------------------------------------------------------*/
/* quick verify                                                               */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vQuickVerify(Item* aSeeEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  for (Item* aItem = aFirstItem();
       aItem != nullptr;
       aItem = aAdjacentItem(aItem))
    {;}
  
  for (Item* aTestEntry = aNextEntry(aSkipRoots(),0);
       aTestEntry;
       aTestEntry = aNextEntry(aTestEntry,0)) {
    
    if (aSeeEntry)
      vShowEntry(aTestEntry);
    
    if (aTestEntry == aSeeEntry)
      int iDebug = 1;
    
    for (i8 i_ScanLevel = aTestEntry->nLevels()-1; i_ScanLevel >= 0; i_ScanLevel--) {
      Item* aCheckEntry = aNextEntry(aTestEntry,i_ScanLevel);
      if (! aCheckEntry)
        continue;
      
      if (CN_ENTRY != aCheckEntry->eClass())
        vShow();
      ASSERT(CN_ENTRY == aCheckEntry->eClass());
    }
    
  }
  
  //  ln i_ScanLevel;
  //  for (i_ScanLevel = 2; *i_ScanLevel >= 0; i_ScanLevel--) {
  //
  //    for (Item* aTestEntry = aNextEntry(aSkipRoots(),*i_ScanLevel);
  //         pTestEntry;
  //         pTestEntry = aNextEntry(pTestEntry,*i_ScanLevel)) {
  //
  //      if (pSeeEntry) {
  //        vTrace("|scan: %lld| "_Stz,*i_ScanLevel);
  //        pTestEntry->vShow(iToOffset(pTestEntry),m_pIndex,true);
  //      }
  //
  //      if (pTestEntry == pSeeEntry)
  //        int iDebug = 1;
  //
  //      if (CN_ENTRY != pTestEntry->eClass())
  //        vShow();
  //      ASSERT(CN_ENTRY == pTestEntry->eClass());
  //    }
  //  }
  
}

/*----------------------------------------------------------------------------*/
/* quick verify                                                               */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bQuickVerify(Item* aNotSeeEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  for (Item* aTestEntry = aNextEntry(aSkipRoots(),0);
       aTestEntry;
       aTestEntry = aNextEntry(aTestEntry,0)) {
    
    for (i8 i_ScanLevel = aTestEntry->nLevels()-1; i_ScanLevel >= 0; i_ScanLevel--) {
      
      Item* aCheckEntry = aNextEntry(aTestEntry,i_ScanLevel);
      if (! aCheckEntry)
        continue;
      
      if (aCheckEntry == aNotSeeEntry) {
        vShowEntry(aTestEntry);
        vShowEntry(aCheckEntry);
        return false;
      }
      
      if (CN_ENTRY != aCheckEntry->eClass())
        return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the structure of this L0 buffer (hidden index)                      */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyL0Hidden(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(! m_aLazyChain);
  
  RASSERT(0 == m_iBufferHeight);
  RASSERT(m_pIndex->bIsHidden());
  
  //  vTrace("VerifyL0 %s: bufno: %lld level %lld\n"_Stz,
  //         m_pIndex->aIndexName(),m_iBufferNo,m_iBufferHeight);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify the contents of this L0 hidden index buffer buffer
  
  bool bValid;
  
  bValid = bVerifyNextPrev();
  if (! bValid)
    return false;
  
  bValid = bVerifyItems();
  if (! bValid) {
    return false;
  }
  
  bValid = bVerifyFreeList();
  if (! bValid) {
    return false;
  }
  
  bValid = bVerifyEntryOffset();
  if (! bValid) {
    return false;
  }
  
  bValid = bVerifyRows();
  if (! bValid) {
    return false;
  }
  
  bValid = bVerifySkipListEntry();
  if (! bValid)
    return false;
  
#ifdef WITH_DEBUG_VERSION
  bValid = bVerifySkipListLevels();
  if (! bValid)
    return false;
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the structure of this L0 buffer (non-hidden index)                  */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyL0Other(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT(0 == m_iBufferHeight);
  
  //  vTrace("VerifyL0 %s: bufno: %lld level %lld\n"_Stz,
  //         m_pIndex->aIndexName(),m_iBufferNo,m_iBufferHeight);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify the contents of this L0 non-hidden index buffer
  
  bool bValid;
  
  bValid = bVerifyNextPrev();
  if (! bValid)
    return false;
  
  bValid = bVerifyItems();
  if (! bValid)
    return false;
  
  bValid = bVerifyFreeList();
  if (! bValid)
    return false;
  
  bValid = bVerifyKeyOrder();
  if (! bValid)
    return false;
  
  bValid = bVerifyRowRefs();
  if (! bValid)
    return false;
  
  bValid = bVerifySkipListEntry();
  if (! bValid)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the structure of this Ln buffer (any index)                         */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyLnAny(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT(0 != m_iBufferHeight);
  
  //  vTrace("VerifyLn %s: bufno: %lld level %lld\n"_Stz,
  //         m_pIndex->aIndexName(),m_iBufferNo,m_iBufferHeight);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify the contents of this buffer
  
  bool bValid;
  
  bValid = bVerifyNextPrev();
  if (! bValid)
    return false;
  
  bValid = bVerifyItems();
  if (! bValid)
    return false;
  
  bValid = bVerifyFreeList();
  if (! bValid)
    return false;
  
  bValid = bVerifyKeyOrder();
  if (! bValid)
    return false;
  
  bValid = bVerifyBufferRefs();
  if (! bValid)
    return false;
  
  bValid = bVerifyAdjacent();
  if (!bValid)
    return false;
  
  bValid = bVerifyKeyBufferOrder();
  if (! bValid)
    return false;
  
  bValid = bVerifyFirstKeys();
  if (! bValid)
    return false;
  
  bValid = bVerifySkipListEntry();
  if (! bValid)
    return false;
  
#ifdef WITH_DEBUG_VERSION
  bValid = bVerifySkipListLevels();
  if (! bValid)
    return false;
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify the contents of child buffers
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
    
    EntryBuffer* pRefEntBuf = EntryBuffer::pRefdEntryBuffer(aItem);
    if (pRefEntBuf->m_iBufferHeight) {
      bValid = pRefEntBuf->bVerifyLnAny();
    }
    else {
      if (m_pIndex->bIsHidden())
        bValid = pRefEntBuf->bVerifyL0Hidden();
      else
        bValid = pRefEntBuf->bVerifyL0Other();
    }
    if (! bValid)
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the adjacent entries not pointing to the same buffer                */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyAdjacent(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   Every entry in an Ln buffer would point to a different buffer.  This
   routine performs a simple whereby two adjacent entries can't point to
   the same buffer.
   
   A more complete test would be to mark all buffers with some reference
   count.
   */
  
  RASSERT(m_iBufferHeight >= 1);
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
    
    // get the buffer that this entry references
    EntryBuffer* pChildEntryBuffer = EntryBuffer::pRefdEntryBuffer(aItem);
    
    // get the adjacent entry
    Item* aAdjacentEntry = aNextEntry(aItem,0);
    
    if (aAdjacentEntry) {
      // get the buffer that the adjacent entry references
      EntryBuffer* pAdjacentChildEntryBuffer =
        EntryBuffer::pRefdEntryBuffer(aAdjacentEntry);
      
      // the two child buffers should not be the same
      if ( pChildEntryBuffer == pAdjacentChildEntryBuffer) {
        vShow();
        vVerifyFault();
        return false;
      }
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the buffer references in this Ln index buffer                       */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyBufferRefs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   Every entry in an Ln buffer should reference an Ln-1 buffer. And
   the Ln-1 buffer should have its parent buffer be that Ln buffer
   */
  
  RASSERT(m_iBufferHeight >= 1);
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
    
    EntryBuffer* pChildEntBuf = EntryBuffer::pRefdEntryBuffer(aItem);
    
    if ( ! pChildEntBuf->bValidEntryBuffer() ) {
      vVerifyFault();
      return false;
    }
    
    if (pChildEntBuf->iBufferHeight() != m_iBufferHeight - 1) {
      vVerifyFault();
      return false;
    }
    
    if (pChildEntBuf->pHigherBuffer() != this) {
      vVerifyFault();
      return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the entry offset array of this L0 hidden index buffer               */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyEntryOffset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   For the hidden index each buffer has an offset array which indicates
   where the row is located in the buffer.
   
   Entries are only maintained for entries 0..gpAttachMap->m_nIndexEntries-1.
   
   In all cases the hidden index is a list of L0 buffers.
   */
  
  RASSERT(0 == m_iBufferHeight);
  RASSERT(m_pIndex->bIsHidden());
  
  //vShow();
  
  for (i8 i_RowNo = 0; i_RowNo < gpAttachMap->m_nIndexEntries; i_RowNo++) {
    
    RASSERT(i_RowNo < C_MAX_ITEMS);
    i8 i_RowOffset = m_i4aItemOffsets[i_RowNo];
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // validate free offset element
    
    if (i_RowOffset < 0) {
      
      // last offset in free chain
      if (C_NO_MORE_ITEMS == - i_RowOffset)
        continue;
      
      // get the next offset in free chain
      i8 i_NextRowEntryOffset = m_i4aItemOffsets[ - i_RowOffset ];
      
      // next offset should be free (or end of free chain)
      if (i_NextRowEntryOffset >= 0) {
        vVerifyFault();
        return false;
      }
      
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // validate offset element which is the offset to an entry
    
    Item* aItem = aGetEntryOffset(i_RowOffset);
    if (! aItem->bValidItem())
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the first key order of child buffers                                */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyFirstKeys(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   The for each entry in an Ln buffer will match the first key in an
   Ln-1 buffer
   */
  
  RASSERT(m_iBufferHeight >= 1);
  
  //vShow();
  
  //IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
    
    // get the entry buffer that this entry references
    EntryBuffer* pChildEntryBuffer = EntryBuffer::pRefdEntryBuffer(aItem);
    
    //pChildEntryBuffer->vShow();
    
    
    // get the first entry of the child buffer
    Item* aChildFirstEntry = pChildEntryBuffer->aLeadEntry();
    
    // get keys of the two entries
    //KeyData* aKeyParent = _aKeyData(aItem->aDataArea());
    //KeyData* aKeyChild  = _aKeyData(aChildFirstEntry->aDataArea());
    
    // the two keys should match
    //bool bEqual = aKeyChild->bEqualTo(aKeyParent,pIdxColVec);
    bool bEqual = KeyData::bEqualTo(aChildFirstEntry, aItem);
    if (! bEqual) {
      vVerifyFault();
      return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the free list of this buffer                                        */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyFreeList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   The chain of free entries in any buffer must consist of solely
   of FreeEntry's.  No other class is permited.
   */
  
  for (FreeItem* aFreeEntry = m_aFreeItemChain;
       aFreeEntry != nullptr;
       aFreeEntry = aFreeEntry->aNextFree()) {
    
    if (! aFreeEntry->bValidFreeEntry()) {
      vVerifyFault();
      return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the items of this buffer                                            */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   The items in and entry buffer are some sequence of:
   
   {[Item]|[FreeItem]...}[Wilderness]
   
   The number of Entries must match the number of items in the buffer
   */
  
  i8 nEntryItems = 0;
  
  for (Item* aItem = aFirstItem();
       aItem != nullptr;
       aItem = aAdjacentItem(aItem)) {
    
    //vTrace("Verify item %p\n"_Stz,pItem);
    
    eCN eClass = aItem->eClass();
    
    switch (eClass) {
        
      case CN_ENTRY: {
        if (! aItem->bValidItem()) {
          vShow();
          vVerifyFault();
          return false;
        }
        nEntryItems++;
        break;
      }
        
      case CN_FREEENTRY: {
        FreeItem* aFreeEntry = _aFreeItem(aItem);
        if (! aFreeEntry->bValidFreeEntry()) {
          vShow();
          vVerifyFault();
          return false;
        }
        break;
      }
        
      case CN_WILDERNESS: {
        Wilderness* aWilderness = _aWilderness(aItem);
        if (! aWilderness->bValidWilderness()) {
          vShow();
          vVerifyFault();
          return false;
        }
        break;
      }
        
      default:
        vVerifyFault();
        return false;
    }
  }
  
  // check the number of entries match
  if (nEntryItems != nItems()) {
    vShow();
    vVerifyFault();
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify that keys are in buffer order of the child buffers                  */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyKeyBufferOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   The entries in an Ln buffer should be in order of the child buffer list
   */
  
  RASSERT(m_iBufferHeight >= 1);
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
    
    // get next entry
    Item* aNextEntry = this->aNextEntry(aItem,0);
    if (! aNextEntry)
      break;
    
    // get the child entry buffers
    EntryBuffer* pChildBufferA = EntryBuffer::pRefdEntryBuffer(aItem);
    EntryBuffer* pChildBufferB = EntryBuffer::pRefdEntryBuffer(aNextEntry);
    
    // child A should point to child B
    if (pChildBufferA->pNextBuffer() != pChildBufferB) {
      vDump();
      vShow(aItem);
      vVerifyFault();
      return false;
    }
    
    // child B should point to child A
    if (pChildBufferB->pPrevBuffer() != pChildBufferA) {
      vVerifyFault();
      return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the key order of this buffer                                        */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyKeyOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   Any Ln buffer or non-hidden L0 buffer should have its keys in ascending
   order.
   */
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
    
    // get next entry
    Item* aNextEntry = this->aNextEntry(aItem,0);
    if (! aNextEntry)
      break;
    
    // get keys of the two entries
    //KeyData* aKeyA = _aKeyData(aItem->aDataArea());
    //KeyData* aKeyB = _aKeyData(aNextEntry->aDataArea());
    
    // next keyB should be greater than or equal to keyA
    //bool bGreaterEqual = aKeyB->bGreaterEqual(aKeyA,pIdxColVec);
    bool bGreaterEqual = KeyData::bGreaterEqual(aNextEntry, aItem);
    if (! bGreaterEqual) {
      vVerifyFault();
      return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the next, previous and higher pointers                              */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyNextPrev(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   Any buffer should have a next whose prev point back to it; any prev should
   point forward to it; a child buffer should point back to its parent
   */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_pPrevBuffer) {
    EntryBuffer* pPrevBuffer = _pEntryBuffer(m_pPrevBuffer);
    if (pPrevBuffer->pNextBuffer() != this) {
      vVerifyFault();
      return false;
    }
    if (pPrevBuffer->iBufferHeight() != m_iBufferHeight) {
      vVerifyFault();
      return false;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_pNextBuffer) {
    EntryBuffer* pNextBuffer = _pEntryBuffer(m_pNextBuffer);
    if (pNextBuffer->pPrevBuffer() != this) {
      vVerifyFault();
      return false;
    }
    if (pNextBuffer->iBufferHeight() != m_iBufferHeight) {
      vVerifyFault();
      return false;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_pHigherBuffer) {
    EntryBuffer* pHigherBuffer = _pEntryBuffer(m_pHigherBuffer);
    if (pHigherBuffer->iBufferHeight() != m_iBufferHeight + 1) {
      vVerifyFault();
      return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the rows of this buffer                                             */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyRows(void)                        //# discuss with Alka
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT(0 == m_iBufferHeight);
  RASSERT(m_pIndex->bIsHidden());
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the row references in this L0 non-hidden index buffer               */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyRowRefs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   For this L0 non-hidden index verify that its key entries referen a
   valid row in an L0 hidden index buffer
   */
  
  RASSERT(0 == m_iBufferHeight);
  RASSERT(! m_pIndex->bIsHidden());
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
    
    // get the entry buffer that this L0 key leaf references
    EntryBuffer* pEntryBuffer = EntryBuffer::pRefdEntryBuffer(aItem);
    
    // that referenced entry buffer should be an L0 buffer
    if (0 != pEntryBuffer->iBufferHeight()) {
      vVerifyFault();
      return false;
    }
    
    // that referenced entry buffer should be for a hidden index
    if (! pEntryBuffer->pIndex()->bIsHidden()) {
      vVerifyFault();
      return false;
    }
    
    // get the row entry from referenced entry buffer
    i8 i_Item = aItem->iItemNo();
    Item* aRowEntry = pEntryBuffer->aGetEntry_Item(i_Item);
    
    // validate the row entry                                          //# more?
    if (1 != aRowEntry->nLevels()) {
      vVerifyFault();
      return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the skip list only contains only entries                            */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifySkipListEntry(bool bShowKeys)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   Run thru each scan list level and verify we discover entries with
   the same number of scan entries
   */
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  Item* aNextEntry2;
  Item* aNextEntry1;
  Item* aNextEntry0;
  
  for (i8 i_ScanLevel = gpAttachMap->m_nIndexLevels - 1;
       i_ScanLevel >= 0;
       i_ScanLevel--) {
    
    if (bShowKeys)
      vTrace("i_ScanLevel %lld\n"_Stz,i_ScanLevel);
    
    Item* aTestEntry_sl = aNextEntry(aSkipRoots(),i_ScanLevel);
    while (aTestEntry_sl) {
      
      if (CN_ENTRY != aTestEntry_sl->eClass()) {
        vVerifyFault();
        return false;
      }
      
      switch (i_ScanLevel) {
        case 2:
          aNextEntry2 = aNextEntry(aTestEntry_sl,2);
          if (aNextEntry2 && CN_ENTRY != aNextEntry2->eClass()) {
            vVerifyFault();
            return false;
          }
        case 1:
          aNextEntry1 = aNextEntry(aTestEntry_sl,1);
          if (aNextEntry1 && CN_ENTRY != aNextEntry1->eClass()) {
            vVerifyFault();
            return false;
          }
        case 0:
          aNextEntry0 = aNextEntry(aTestEntry_sl,0);
          if (aNextEntry0 && CN_ENTRY != aNextEntry0->eClass()) {
            vVerifyFault();
            return false;
          }
          break;
      }
      
      if (bShowKeys) {
        KeyData* aTestKey = _aKeyData(aTestEntry_sl->aDataArea());
        vTrace("Test %p: "_Stz,aTestEntry_sl);
        aTestKey->vShow(pIdxColVec, m_pIndex->bIsHidden() ? true : false);
        vTrace("\n"_Stz);
      }
      
      aTestEntry_sl = aNextEntry(aTestEntry_sl,i_ScanLevel);
    }
  }
  
#ifdef WITH_DEBUG_VERSION
  if (! bVerifySkipListLevels())
    return false;
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the skip list levels                                                */
/*----------------------------------------------------------------------------*/
#ifdef WITH_DEBUG_VERSION

bool EntryBuffer::bVerifySkipListLevels(bool bShowKeys)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   Verify entries with skip offsets greater than one that they
   can be found by following the  skip chain at that level, and
   that the entries all move forward (not backward).
   */
  
  // set entry no's
  vNumberEntries();
  
  // run through all entries at level 0
  Item* aTestEntry_0 = aNextEntry(aSkipRoots(),0);
  while (aTestEntry_0) {
    
    // check if scan entry has more than one offset
    i8 nOffsets = aTestEntry_0->nLevels();
    if (1 == nOffsets) {
      aTestEntry_0 = aNextEntry(aTestEntry_0,0);
      continue;
    }
    
    // for each level see if we can find the test entry
    for (i8 i_SkipLevel = nOffsets - 1;
         i_SkipLevel > 0;
         i_SkipLevel--) {
      
      Item* aScanEntry_sl = aNextEntry(aSkipRoots(),i_SkipLevel);
      while (aScanEntry_sl) {
        
        if (aScanEntry_sl == aTestEntry_0)
          break;
        
        Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_SkipLevel);
        
        // check we dont go backwwards
        if (aNextEntry_sl->m_iSkipNo < aScanEntry_sl->m_iSkipNo) {
          vShow(aScanEntry_sl);
          vVerifyFault();
          return false;
        }
        
        aScanEntry_sl = aNextEntry_sl;
      }
      
      if (aScanEntry_sl != aTestEntry_0) {
        vShow(aTestEntry_0);
        vVerifyFault();
        return false;
      }
      
    }
    
    aTestEntry_0 = aNextEntry(aTestEntry_0,0);
  }
  
  return true;
}
#endif

/*----------------------------------------------------------------------------*/
/* verify has detected a fault                                                */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vVerifyFault(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  vTrace("VerifyFault\n"_Stz);
  RASSERT(0);
  return;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
