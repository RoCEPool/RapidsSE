/*============================================================================*/
/* EntryBuffer.cpp -- Item Buffer                                            */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

bool mbWatch = false;


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
EntryBuffer* Data::_pEntryBuffer(void* aAddress)
{
  EntryBuffer* pEntryBuffer = reinterpret_cast<EntryBuffer*>(aAddress);
  
  RASSERT_TYPE(pEntryBuffer,CN_ENTRYBUFFER);
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* EntryBuffer::operator new(size_t)
{
  EntryBuffer* pEntryBuffer =
    static_cast<EntryBuffer*>(HeapMem::pAllocate(CN_ENTRYBUFFER));
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* EntryBuffer::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void EntryBuffer::operator delete(void* aAddress)
{
  EntryBuffer* pEntryBuffer = static_cast<EntryBuffer*>(aAddress);
  
  HeapMem::vFree(pEntryBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer::EntryBuffer(void)
: ItemHeap(CN_ENTRYBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_okLock.vInitLock(LK_ENTRYBUFFER);
  
  m_pIndex        = nullptr;
  
  //m_aHeadEntry  = nullptr;
  m_aLastEntry  = nullptr;
  
  m_aLazyChain  = nullptr;
  
  //vHeadEntryNew();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_ENTRYBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntryBuffer::~EntryBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump entry buffer                                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  EntryBuffer* pEntryBuffer = _pEntryBuffer(pObject);
  
  pEntryBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/* get the entry buffer that given id references                              */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pRefdEntryBuffer(Item* pItem)
{
  Id xId = pItem->xId();
  
  EntryBuffer* pEntryBuffer = EntryBuffer::pRefdEntryBuffer(xId);
    
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the entry buffer that given id references                              */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pRefdEntryBuffer(Id& xId)
{
  bn nBufferNo = xId.iBuffer();
  
  HeapBuffer* pHeapBuffer =
  reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(nBufferNo));
  
  EntryBuffer* pEntryBuffer =
  reinterpret_cast<EntryBuffer*>(pHeapBuffer->aFirstChunk());
  
  ASSERT(pEntryBuffer);
  ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
  
  return pEntryBuffer;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* assert valid entry buffer                                                  */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vAssertEntryBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDropWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vGrabWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* set the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pIndex = pIndex;
  
  if (! m_pIndex->bIsHidden())
    return;
  
  vInitFreeChain();
}

/*----------------------------------------------------------------------------*/
/* get the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
Index* EntryBuffer::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pIndex;
}


////////////////////////////////////////////////////////////////////////////////
//                             buffer functions                               //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* analyze this buffer                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vAnalyze(s48K& sAnalysis, bool bDetailed,
                           i8& iBuffers, i8& iEntries)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (m_pIndex->bIsHidden()) {
    
    ASSERT(m_iHeightNo == 0);
    EntryBuffer* pNextBuffer = this;
    
    do {
      ASSERT(pNextBuffer->iHeightNo() == 0);
      
      iBuffers++;
      iEntries += (pNextBuffer->m_nItems);
      
      if (bDetailed)
        sAnalysis.vCatSprint("row: (%s, %lld, %5lld, 'L%lld', %lld )\n"_Stz,
                             gpAttachMap->m_pGrid->pThisNode()->aNodeName(),
                             pNextBuffer->m_iPartitionNo,
                             iBuffers,
                             pNextBuffer->m_iHeightNo,
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
                         m_iHeightNo,
                         m_nItems);
  
  // recursively dump the lower level buffers
  if (m_iHeightNo != 0) {
    
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
/* return number of index buffers under this buffer                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vBufferCount(i8& nBuffers)
{
  EntryBuffer* pNextBuffer = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // hidden index ?
  
  if (m_pIndex->bIsHidden()) {
    ASSERT(m_iHeightNo == 0);
    pNextBuffer = this->pNextBuffer();
    nBuffers++;
    
    while (pNextBuffer) {
      ASSERT(pNextBuffer->iHeightNo() == 0);
      
      //EntryBuffer* pThisBuffer = pNextBuffer;
      pNextBuffer = pNextBuffer->pNextBuffer();
      nBuffers++;
    }
    
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // other index
  
  // count this buffer
  nBuffers++;
  
  // recursively count the lower level buffers
  if (! m_iHeightNo)
    return;
    
  for (Item* aItem = aLeadEntry();
       aItem != nullptr;
       aItem = aNextEntry(aItem, 0)) {
    
    // get the index buffer from the entry
    pNextBuffer = EntryBuffer::pRefdEntryBuffer(aItem);
    
    ASSERT(pNextBuffer);
    ASSERT(pNextBuffer->eClass() == CN_ENTRYBUFFER);
    
    pNextBuffer->vBufferCount(nBuffers);
    
  }
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
  if (m_iHeightNo != 0) {
    
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
/* get the higher level index buffer                                          */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pHigherBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return _pEntryBuffer(m_pHigherBuffer);
}

/*----------------------------------------------------------------------------*/
/* set the higher level index                                                 */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vHigherBuffer(EntryBuffer* pHigherBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pHigherBuffer = pHigherBuffer;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return _pEntryBuffer(m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vNextBuffer(EntryBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
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
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return _pEntryBuffer(m_pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vPrevBuffer(EntryBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* reset/empty the buffer                                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT(nullptr == m_i4aItemOffsets);
  RASSERT(! m_pIndex->bIsHidden());
  RASSERT(1 == m_iHeightNo);
  
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
    StrLib::wToString((i8)((EntryBuffer*)m_pHigherBuffer)->m_iHeightNo,
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
  
  vTrace("  index: %s partition: %lld level: %lld\n"_Stz,
         m_pIndex ? m_pIndex->aIndexName() : "NULL"_Stz,m_iPartitionNo,m_iHeightNo);
  
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
    
    if (m_iHeightNo)
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
    vTrace("\nIndex Tree:\n"_Stz,m_iHeightNo);
  
  vTrace("L%lld"_Stz,m_iHeightNo);
  
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
  
  if ( ! m_iHeightNo )
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
/* truncate this buffer by removing all entries                               */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  EntryBuffer* pNextBuffer;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // hidden index buffers are just one level
  
  if (m_pIndex->bIsHidden()) {
    ASSERT(m_iHeightNo == 0);
    pNextBuffer = this->pNextBuffer();
    
    while (pNextBuffer) {
      ASSERT(pNextBuffer->iHeightNo() == 0);
      
      EntryBuffer* pThisBuffer = pNextBuffer;
      pNextBuffer = pNextBuffer->pNextBuffer();
      delete pThisBuffer;
    }
    delete this;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // other index
  if (m_iHeightNo != 0) {
    
    // recursively delete the lower level buffers
    for (Item* aItem = aLeadEntry();
         aItem != nullptr;
         aItem = aNextEntry(aItem, 0)) {
      
      // get the index buffer from the entry
      pNextBuffer = EntryBuffer::pRefdEntryBuffer(aItem);
      
      ASSERT(pNextBuffer);
      ASSERT(pNextBuffer->eClass() == CN_ENTRYBUFFER);
      pNextBuffer->vTruncate();
    }
  }
  
  delete this;
}

////////////////////////////////////////////////////////////////////////////////
//                             entry functions                                //
////////////////////////////////////////////////////////////////////////////////


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
  
  KeyData* aKey = _aKeyData(aItem->aDataArea());
  
  vPositionPriorVecEntry(aKey, xPriorVec, aItem);
  
  vCutEntry(aItem,xPriorVec);
  
  vFreeItem(aItem);
  
  // entry no longer exists, set the pointer to be null
  *aaItem = nullptr;
}

/*----------------------------------------------------------------------------*/
/* get an entry by number                                                     */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aGetEntry_Item(i8 i_Item)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (i_Item == C_NO_MORE_ITEMS)
    return nullptr;
  
  RASSERT(i_Item >= 0 && i_Item < C_MAX_ITEMS);
  i4 iOffset = m_i4aItemOffsets[i_Item];
  
  Item* aItem = aAdd<Item*>(aItemArea(), iOffset);
  
  return aItem;
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
/* get lead entry (the entry after head entry                                 */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aLeadEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
  
  //if (m_aLazyChain)
  //  vBuildSkipList();
#endif
  
  Item* aFirst = aNextEntry(aSkipRoots(),0);
  
  RASSERT(! aFirst || aFirst->bValidItem());
  
  return aFirst;
}

/*----------------------------------------------------------------------------*/
/* get last entry                                                             */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aLastEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
  
  //if (m_aLazyChain)
  //  vBuildSkipList();
#endif
  
  Item* aLast = aSkipRoots();
  
  for (;;) {
    Item* aNext = aNextEntry(aLast,0);
    if (! aNext)
      break;
    aLast = aNext;
  }
  
  return aLast;
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
/* get next entry                                                             */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aNextEntry(Item* aCurrent, i8 i_Level)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT(aCurrent->bValidItem());
  
  i4 iOffset = aCurrent->iNEO(i_Level);
  
  if (-1 == iOffset)
    return nullptr;
  
  Item* aItem = aAdd<Item*>(aItemArea(), iOffset);
  
  RASSERT(! aItem || aItem->bValidItem());
  
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
/* show entry                                                                 */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vShowEntry(Item* aItem)
{
  KeyData* aKey = _aKeyData(aItem->aDataArea());
  i8 iOffset = iToOffset(aItem);
  
  vTrace("%lld: Item: %p Size: %llu Levels: %llu "_Stz,
         iOffset,
         aItem,
         aItem->nSizeItem_WithoutOffsets(),
         aItem->nLevels()
         );
  
  vTrace("Id: %3lli.%03lli "_Stz,
         aItem->iBufferNo(),
         aItem->iItemNo()
         );
  
  //vTrace("Tx: %lld.%03lld "_Stz,
  //       Tx::iTxNode(m_iTxId),
  //       Tx::iTxNo(m_iTxId)
  //       );
  
  if (! m_pIndex->bIsHidden())
    aKey->vShow(m_pIndex->pIdxColVec(), false);
  
  if (m_pIndex->bIsHidden()) {
    RowData* aRowData = _aRowData(aItem->aDataArea());
    vTrace("RowData: "_Stz);
    i8 iRowSize = aItem->nSizeItem_WithoutOffsets();
    if (iRowSize > 0) {
      aRowData->vShow(m_pIndex->pTable()->pColumnVec(), true);
    }
  }
  
  
  vTrace(" offsets:"_Stz);
  for (i8 i_Level = 0; i_Level < aItem->nLevels(); i_Level++)
    vTrace(" %d"_Stz,aItem->iNEO(i_Level));
  
  vTrace("\n"_Stz);
}



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot an entry in this buffer                                              */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aAllotEntry(i8 nBytes_Without_Offsets)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Item* aItem = aAllotItem(CN_ENTRY,nBytes_Without_Offsets);
  if (! aItem)
    return nullptr;
    
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* allot a row entry in this buffer                                           */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aAllotRowEntry(RowWrk& xRowWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // rows only ever have one next offset
  RASSERT(1 == xRowWrk.nLevels());

  i8 nRowEntrySize = xRowWrk.nRowEntrySize();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if this buffer has enough space and has free item no's

  if (! bHasSpace(nRowEntrySize))
    return nullptr;
  
  if (! bHasFreeItem()) {
    vTrace("No free item no's\n"_Stz);
    return nullptr;
  }

  RASSERT(nItems() < C_MAX_ITEMS - 16);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allot an entry and its neo in this entry buffer
  
  Item* aNewRowEntry = aAllotEntry(nRowEntrySize - 4);
  RASSERT(aNewRowEntry);
  
  IGNORE_RETURN(aAllotNEO(4));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the item no for this row
  
  i8 i_RowItemNo = iGetItemNo();
  RASSERT(C_NO_MORE_ITEMS != i_RowItemNo);
  RASSERT(i_RowItemNo >= 0 and i_RowItemNo < C_MAX_ITEMS);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the item id
  
  xRowWrk.vItemNo(i_RowItemNo);
  aNewRowEntry->vId(xRowWrk.xId());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // record the offset for this row entry
  
  i4 iNewEntryOffset = iToOffset(aNewRowEntry);
  RASSERT(i_RowItemNo >= 0 && i_RowItemNo < C_MAX_ITEMS);
  m_i4aItemOffsets[i_RowItemNo] = iNewEntryOffset;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set neo in head entry for the first time
  
  if (aSkipRoots()->iNEO(0) == -1)
    aSkipRoots()->vNEO(0,iNewEntryOffset);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // a last item should now link to this new item
  if (m_aLastEntry)
    m_aLastEntry->vNEO(0,iNewEntryOffset);
  m_aLastEntry = aNewRowEntry;
  aNewRowEntry->vNEO(0,-1);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  return aNewRowEntry;
}

/*----------------------------------------------------------------------------*/
/* get root of skip list (was aka head entry                                  */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aSkipRoots(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //return m_aHeadEntry;
  return &m_xSkipRoots;
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
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* EntryBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
