/*============================================================================*/
/* EntBuffer.cpp -- Ent Buffer                                            */
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
EntBuffer* Data::_pEntBuffer(void* aAddress)
{
  EntBuffer* pEntBuffer = reinterpret_cast<EntBuffer*>(aAddress);
  
  RASSERT_TYPE(pEntBuffer,CN_ENTBUFFER);
  
  return pEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* EntBuffer::operator new(size_t)
{
  EntBuffer* pEntBuffer =
    static_cast<EntBuffer*>(HeapMem::pAllocate(CN_ENTBUFFER));
  
  return pEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* EntBuffer::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void EntBuffer::operator delete(void* aAddress)
{
  EntBuffer* pEntBuffer = static_cast<EntBuffer*>(aAddress);
  
  HeapMem::vFree(pEntBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntBuffer::EntBuffer(void)
: DataBuffer(CN_ENTBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  m_pIndex      = nullptr;
  //m_xSkipRoots
  m_aLazyChain  = nullptr;
  
  if (gpAttachMap->m_nIndexEntries != 0)
    vMaxItems(gpAttachMap->m_nIndexEntries);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_ENTBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntBuffer::~EntBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump entry buffer                                                          */
/*----------------------------------------------------------------------------*/
void EntBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  EntBuffer* pEntBuffer = _pEntBuffer(pObject);
  
  pEntBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/* aGetNextLeft -- get the next left entry                                     */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aGetNextEntry(EntBuffer** ppEntBuffer, Ent* aPrevEntry)
{
  EntBuffer* pEntBuffer = *ppEntBuffer;
  
  Ent *aNextEntry = pEntBuffer->aNextEntry(aPrevEntry, 0);
  if (! aNextEntry ) {
    pEntBuffer = pEntBuffer->pNextBuffer();
    *ppEntBuffer = pEntBuffer;
    if (pEntBuffer)
      aNextEntry = pEntBuffer->aLeadEntry();
  }
  
  return aNextEntry;
}

/*----------------------------------------------------------------------------*/
/* get the entry buffer that given id references                              */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pRefdEntBuffer(Ent* pItem)
{
  Id xId = pItem->xId();
  
  EntBuffer* pEntBuffer = EntBuffer::pRefdEntBuffer(xId);
    
  return pEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the entry buffer that given id references                              */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pRefdEntBuffer(Id& xId)
{
  bn nBufferNo = xId.iBuffer();
  
  HeapBuffer* pHeapBuffer =
  reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(nBufferNo));
  
  EntBuffer* pEntBuffer =
  reinterpret_cast<EntBuffer*>(pHeapBuffer->aFirstChunk());
  
  ASSERT(pEntBuffer);
  ASSERT(pEntBuffer->eClass() == CN_ENTBUFFER);
  
  return pEntBuffer;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot an entry in this buffer                                              */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aAllotEntry(i8 nBytes_Without_Offsets)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  Ent* aEnt = aAllotItem(CN_ENT,nBytes_Without_Offsets);
  if (! aEnt)
    return nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* analyze this buffer                                                        */
/*----------------------------------------------------------------------------*/
void EntBuffer::vAnalyze(s48K& sAnalysis, bool bDetailed,
                           i8& iBuffers, i8& iEntries)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
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
    
    for (Ent* aEnt = aLeadEntry();
         aEnt != nullptr;
         aEnt = aNextEntry(aEnt, 0)) {
      
      // get the index buffer from the entry
      EntBuffer* pEntBuffer = EntBuffer::pRefdEntBuffer(aEnt);
      
      ASSERT(pEntBuffer);
      ASSERT(pEntBuffer->eClass() == CN_ENTBUFFER);
      
      pEntBuffer->vAnalyze(sAnalysis, bDetailed, iBuffers, iEntries);
      
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* assert valid entry buffer                                                  */
/*----------------------------------------------------------------------------*/
void EntBuffer::vAssertEntBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* return number of index buffers under this buffer                           */
/*----------------------------------------------------------------------------*/
void EntBuffer::vBufferCount(i8& nBuffers)
{
  EntBuffer* pNextBuffer = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // other index
  
  // count this buffer
  nBuffers++;
  
  // recursively count the lower level buffers
  if (! m_iBufferHeight)
    return;
  
  for (Ent* aEnt = aLeadEntry();
       aEnt != nullptr;
       aEnt = aNextEntry(aEnt, 0)) {
    
    // get the index buffer from the entry
    pNextBuffer = EntBuffer::pRefdEntBuffer(aEnt);
    
    ASSERT(pNextBuffer);
    ASSERT(pNextBuffer->eClass() == CN_ENTBUFFER);
    
    pNextBuffer->vBufferCount(nBuffers);
    
  }
}

/*----------------------------------------------------------------------------*/
/* cut entry from skip list                                                   */
/*----------------------------------------------------------------------------*/
void EntBuffer::vCutEntry(Ent* aEnt, Ent* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  i8 nLevels = aEnt->nLevels();
  
  for (i8 i_Level = 0; i_Level < nLevels; i_Level++) {
    i4 iNEO = aEnt->iNEO(i_Level);
    aPriorVec[i_Level]->vNEO(i_Level,iNEO);
  }
}

/*----------------------------------------------------------------------------*/
/* delete the given entry                                                     */
/*----------------------------------------------------------------------------*/
void EntBuffer::vDeleteEntry(Ent** aaEnt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent* aEnt = *aaEnt;
  
  Ent* xPriorVec[gpAttachMap->m_nIndexLevels];
  
  vPositionPriorVecEntry(aEnt, xPriorVec, aEnt);
  
  vCutEntry(aEnt,xPriorVec);
  
  i8 nBytes = aEnt->nSizeItem();
  vFreeItem(aEnt,nBytes);
  
  // entry no longer exists, set the pointer to be null
  *aaEnt = nullptr;
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void EntBuffer::vDropWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void EntBuffer::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  // show this buffer
  vShow();
  
  // recursively dump the lower level buffers
  if (m_iBufferHeight != 0) {
    
    for (Ent* aEnt = aLeadEntry();
         aEnt != nullptr;
         aEnt = aNextEntry(aEnt, 0)) {
      
      // get the index buffer from the entry
      EntBuffer* pEntBuffer = EntBuffer::pRefdEntBuffer(aEnt);
      
      ASSERT(pEntBuffer);
      ASSERT(pEntBuffer->eClass() == CN_ENTBUFFER);
      
      pEntBuffer->vDump();
    }
  }
}

/*----------------------------------------------------------------------------*/
/* dump the given prior vector                                                */
/*----------------------------------------------------------------------------*/
void EntBuffer::vDumpPriorVec(Ent* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  vTrace("Prior Vector\n"_Stz);
  
  vTrace("  0:%p 1:%p 2:%p\n"_Stz,
         aPriorVec[0],aPriorVec[1],aPriorVec[2]);
  
  for (i8 i = 0; i < 3; i++) {
    Ent* aEnt = aPriorVec[i];
    for (i8 i_Level = 0; i_Level < aEnt->nLevels(); i_Level++) {
      vTrace("  i : %lld Ent : %p i_Offset : %lld offset : %lld pointsTo : %p\n"_Stz,
             i,
             aEnt,
             i_Level,
             aEnt->iNEO(i_Level),
             aGetEntryOffset(aEnt->iNEO(i_Level)));
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get first child buffer                                                     */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pFirstChildBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  Ent* aFirstEntry = this->aLeadEntry();
  EntBuffer* pChildEntBuffer = EntBuffer::pRefdEntBuffer(aFirstEntry);
  
  return pChildEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* get an entry by offset                                                     */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aGetEntryOffset(i8 iOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  Ent* aEnt = aAdd<Ent*>(aItemArea(), iOffset);
  
  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* get split entry by split number                                            */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aGetEntrySplit(i8 iSplitEntryNo,Ent** aaPriorEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  ASSERT(aSkipRoots());
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  *aaPriorEntry = nullptr;
  
  i8 iEntryNo = 0;
  for (Ent* aSplitEntry = aNextEntry(aSkipRoots(),0);
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
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void EntBuffer::vGrabWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* get the higher level index buffer                                          */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pHigherBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  return _pEntBuffer(m_pHigherBuffer);
}

/*----------------------------------------------------------------------------*/
/* set the higher level index                                                 */
/*----------------------------------------------------------------------------*/
void EntBuffer::vHigherBuffer(EntBuffer* pHigherBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  m_pHigherBuffer = pHigherBuffer;
}

/*----------------------------------------------------------------------------*/
/* set the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
void EntBuffer::vIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  m_pIndex = pIndex;
}

/*----------------------------------------------------------------------------*/
/* get the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
Index* EntBuffer::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  return m_pIndex;
}

/*----------------------------------------------------------------------------*/
/* get last entry                                                             */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aLastEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
  
  //if (m_aLazyChain)
  //  vBuildSkipList();
#endif
  
  Ent* aLast = aSkipRoots();
  
  for (;;) {
    Ent* aNext = aNextEntry(aLast,0);
    if (! aNext)
      break;
    aLast = aNext;
  }
  
  return aLast;
}

/*----------------------------------------------------------------------------*/
/* get lead entry (the entry after head entry                                 */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aLeadEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  Ent* aFirst = aNextEntry(aSkipRoots(),0);
  
  RASSERT(! aFirst || aFirst->bValidItem());
  
  return aFirst;
}

/*----------------------------------------------------------------------------*/
/* get length of list of buffers                                               */
/*----------------------------------------------------------------------------*/
i8 EntBuffer::iLengthList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  i8 iLength = 0;
  for (EntBuffer* pLn = this; pLn; pLn = pLn->pNextBuffer())
    iLength++;
  
  return iLength;
}

/*----------------------------------------------------------------------------*/
/* get length of sublist of buffers                                           */
/*----------------------------------------------------------------------------*/
i8 EntBuffer::iLengthSubList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  i8 iLength = 0;
  
  EntBuffer* pThisL0 = this;
  EntBuffer* pThisL1 = pThisL0->pHigherBuffer();
  
  EntBuffer* pL0;
  for (pL0 = pThisL0;
       pL0 && pL0->pHigherBuffer() == pThisL1;
       pL0 = pL0->pNextBuffer()) {
    
    iLength++;
  }
  
  return iLength;
}

/*----------------------------------------------------------------------------*/
/* convert given entry into free entry                                        */
/*----------------------------------------------------------------------------*/
FreeItem* EntBuffer::aMakeFreeEntry(Ent* aEnt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  i8 nSizeItem = aEnt->nSizeItem_WithoutOffsets();
  i8 nSizeNEO  = aEnt->iNEOsize();
  i8 nSizeFree = nSizeItem + nSizeNEO;
  
  FreeItem* aFreeItem = new(aEnt) FreeItem(nSizeFree);
  
  return aFreeItem;
}

/*----------------------------------------------------------------------------*/
/* move to exact key entry                                                    */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::pMoveToExactEntry(Ent* aEnt, KeyWrk& xKeyWrk, EntBuffer** paEntBuffer)
{
  EntBuffer* pEntBuffer = *paEntBuffer;
  
  if (aEnt == pEntBuffer->aSkipRoots())
    aEnt = pEntBuffer->aNextEntry(aEnt,0);
  
  eKC eCompare = Key::eCompare(aEnt, xKeyWrk);
  
  // have we reached the key?
  if (eCompare == KC_SAME)
    return aEnt;
  
  // have we moved beyond ?
  if (eCompare == KC_AFTER)
    return nullptr;
  
  aEnt = pEntBuffer->aNextEntry(aEnt,0);
  
  if ( ! aEnt ) {
    
    // move to the next buffer
    *paEntBuffer = pEntBuffer->pNextBuffer();
    
    if (*paEntBuffer) {
      return pMoveToExactEntry((*paEntBuffer)->aSkipRoots(), xKeyWrk, paEntBuffer);
    }
    
  }
  
  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* move to next key entry                                                     */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::pMoveToGreaterThanEntry(Ent* aEnt, KeyWrk& xKeyWrk, EntBuffer** paEntBuffer)
{
  EntBuffer* pEntBuffer = *paEntBuffer;
  
  if (aEnt == pEntBuffer->aSkipRoots())
    aEnt = pEntBuffer->aNextEntry(aEnt,0);
  
  while (aEnt) {
    eKC eCompare = Key::eCompare(aEnt, xKeyWrk);
    
    // have we moved beyond ?
    if (eCompare == KC_AFTER)
      break;
    
    aEnt = pEntBuffer->aNextEntry(aEnt,0);
    
    if ( ! aEnt) {
      
      // move to the next buffer
      *paEntBuffer = pEntBuffer->pNextBuffer();
      
      if (*paEntBuffer) {
        return (*paEntBuffer)->pMoveToGreaterThanEntry((*paEntBuffer)->aSkipRoots(), xKeyWrk, paEntBuffer);
      }
      else
        break;
    }
  }
  
  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* move to next key entry                                                     */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::pMoveToGreaterEqualEntry(Ent* aEnt, KeyWrk& xKeyWrk, EntBuffer** paEntBuffer)
{
  EntBuffer* pEntBuffer = *paEntBuffer;
  
  if (aEnt == pEntBuffer->aSkipRoots())
    aEnt = pEntBuffer->aNextEntry(aEnt,0);
  
  while (aEnt) {
    eKC eCompare = Key::eCompare(aEnt, xKeyWrk);
    
    // have we reached the key?
    if (eCompare == KC_SAME)
      break;
    
    // have we moved beyond ?
    if (eCompare == KC_AFTER)
      break;
    
    aEnt = pEntBuffer->aNextEntry(aEnt,0);
    
    if (!aEnt) {
      
      // move to the next buffer
      *paEntBuffer = pEntBuffer->pNextBuffer();
      
      if (*paEntBuffer) {
        return pMoveToGreaterEqualEntry((*paEntBuffer)->aSkipRoots(), xKeyWrk, paEntBuffer);
      }
    }
    else
      break;
  }
  
  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* move to next key entry                                                     */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::pMoveToNextEntry(Ent* aEnt, KeyWrk& xKeyWrk, EntBuffer** paEntBuffer)
{
  EntBuffer* pEntBuffer = *paEntBuffer;
  
  aEnt = pEntBuffer->aNextEntry(aEnt,0);
  
  if (aEnt)
    return aEnt;
  
  while( ! aEnt) {
    
    // move to the next buffer
    *paEntBuffer = pEntBuffer->pNextBuffer();
    
    if (*paEntBuffer) {
      return pMoveToNextEntry((*paEntBuffer)->aSkipRoots(), xKeyWrk, paEntBuffer);
    }
    else
      break;
  }
  
  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* move to next key entry                                                     */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::pMoveToPrevEntry(Ent* aEnt, KeyWrk& xKeyWrk, EntBuffer** paEntBuffer)
{
  EntBuffer* pEntBuffer = *paEntBuffer;
  
  if (aEnt == pEntBuffer->aSkipRoots())
    aEnt = pEntBuffer->aNextEntry(aEnt,0);
  
  Ent* aEntPrev = aEnt;
  
  while (aEnt) {
    eKC eCompare = Key::eCompare(aEnt, xKeyWrk);
    
    // have we reached the key?
    if (eCompare == KC_SAME)
      break;
    
    // have we moved beyond ?
    if (eCompare == KC_AFTER)
      return aEntPrev;
    
    aEntPrev = aEnt;
    aEnt = pEntBuffer->aNextEntry(aEnt,0);
    
    if ( ! aEnt )
      return aEntPrev;
  }
  
  return aEntPrev;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return _pEntBuffer(m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntBuffer::vNextBuffer(EntBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get next entry                                                             */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aNextEntry(Ent* aCurrent, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  RASSERT(aCurrent->bValidItem());
  
  i4 i_Offset = aCurrent->iNEO(iLevel);
  
  if (-1 == i_Offset)
    return nullptr;
  
  Ent* aEnt = aAdd<Ent*>(aItemArea(), i_Offset);
  
  RASSERT(! aEnt || aEnt->bValidItem());
  
  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* pop entry from lazy chain                                                  */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aPopLazyChain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  if (! m_aLazyChain )
    return nullptr;
  
  Ent* aPoppedLazyEntry = m_aLazyChain;
  
  i8 iNextLazyOffset = aPoppedLazyEntry->iNEO(0);
  
  if (-1 == iNextLazyOffset)
    m_aLazyChain = nullptr;
  else
    m_aLazyChain = aGetEntryOffset(iNextLazyOffset);
  
  return aPoppedLazyEntry;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  return _pEntBuffer(m_pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPrevBuffer(EntBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* reset/empty the buffer                                                     */
/*----------------------------------------------------------------------------*/
void EntBuffer::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  RASSERT(1 == m_iBufferHeight);
  
  vResetUsed();

  m_xSkipRoots.vNEO(0,-1);
  m_xSkipRoots.vNEO(1,-1);
  m_xSkipRoots.vNEO(2,-1);
  
  //m_aHeadEntry  = nullptr;
  m_aLazyChain  = nullptr;
  
  //vHeadEntryNew();
}

/*----------------------------------------------------------------------------*/
/* restore the buffer after being received over a socket                      */
/*----------------------------------------------------------------------------*/
void EntBuffer::vRestore(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  RASSERT( ! m_pHigherBuffer );
  RASSERT( ! m_pNextBuffer );
  RASSERT( ! m_pPrevBuffer );
  
  // set the index to nullptr (this pointer is not valid any more)
  m_pIndex = nullptr;
  
  // get this buffer address
  PoolBuffer* pThisBuffer =
  pBitAnd<PoolBuffer*>(this,C_OBJECT_TO_BUFFER_MASK);

  // calculate offset for m_aLazyChain and set it
  if (m_aLazyChain) {
    PoolBuffer* pOriginalBuffer =
    pBitAnd<PoolBuffer*>(m_aLazyChain,C_OBJECT_TO_BUFFER_MASK);
    u8 uOffset = nSub(m_aLazyChain, pOriginalBuffer);
    m_aLazyChain = pBitOr<Ent*>(pThisBuffer,uOffset);
  }
  
  // get the buffer no
  bn nBuffer = nPoolBufferNo();
    
  // now restore the row id's to point to this buffer no
  for (Ent* aEnt = aLeadEntry();
       aEnt != nullptr;
       aEnt = aNextEntry(aEnt, 0)) {
    
    Id xId(*nBuffer, aEnt->iItemNo(), aEnt->nLevels());
    aEnt->vId(xId);
  }
  
}

/*----------------------------------------------------------------------------*/
/* restore the index of the index buffer                                      */
/*----------------------------------------------------------------------------*/
void EntBuffer::vRestoreIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  //RASSERT( ! m_pIndex );
  m_pIndex = pIndex;
}

/*----------------------------------------------------------------------------*/
/* show entry                                                                 */
/*----------------------------------------------------------------------------*/
void EntBuffer::vShowEntry(Ent* aEnt)
{
  Key* aKey = _aKeyData(aEnt->aDataArea());
  i8 iOffset = iToOffset(aEnt);
  
  vTrace("%lld: Ent: %p Size: %llu Levels: %llu "_Stz,
         iOffset,
         aEnt,
         aEnt->nSizeItem_WithoutOffsets(),
         aEnt->nLevels()
         );
  
  vTrace("Id: %3lli.%03lli "_Stz,
         aEnt->iBufferNo(),
         aEnt->iItemNo()
         );
  
  //vTrace("Tx: %lld.%03lld "_Stz,
  //       Tx::iTxNode(m_iTxId),
  //       Tx::iTxNo(m_iTxId)
  //       );
  
  aKey->vShow(m_pIndex->pIdxColVec());
  
  vTrace(" offsets:"_Stz);
  for (i8 i_Level = 0; i_Level < aEnt->nLevels(); i_Level++)
    vTrace(" %d"_Stz,aEnt->iNEO(i_Level));
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void EntBuffer::vShow(Ent* aMarkEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  s256 sHigherBuffer;
  if (m_pHigherBuffer)
    StrLib::wToString((i8)((EntBuffer*)m_pHigherBuffer)->m_iBufferHeight,
                      sHigherBuffer.aChar());
  else
    sHigherBuffer = "..."_Stz;
  
  vTrace("\n"_Stz);
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  vTrace("EntBuffer: %p (buffer %lld): items: %lld used: %lld freespace %lld\n"_Stz,
         this,*nPoolBufferNo(),nItems(),nUsedBytes(),nFreeBytes());
  
  vTrace("  higherBuffer %p (L%s) prevBuffer %p nextBuffer %p\n"_Stz,
         m_pHigherBuffer,sHigherBuffer.aStz(),m_pPrevBuffer,m_pNextBuffer);
  
  vTrace("  index: %s partition: %lld height: %lld\n"_Stz,
         m_pIndex ? m_pIndex->aIndexName() : "NULL"_Stz,m_iPartitionNo,m_iBufferHeight);
  
  vTrace("  skipRoots: %p[%d,%d,%d] lastEntry: %p\n"_Stz,
         aSkipRoots(),
         aSkipRoots() ? aSkipRoots()->iNEO(0) : 0,
         aSkipRoots() ? aSkipRoots()->iNEO(1) : 0,
         aSkipRoots() ? aSkipRoots()->iNEO(2) : 0);
  
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  ASSERT(aSkipRoots());
  
  vTrace("\n"_Stz);
  vTrace("Entries (by headEntry vector[0]):\n"_Stz);
  
  i8 iOffset = aSkipRoots()->iNEO(0);
  while (iOffset > -1) {
    Ent* aEnt = aGetEntryOffset(iOffset);
    vShowEntry(aEnt);
    if (aMarkEntry == aEnt)
      vTrace("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
             "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n"_Stz);
    
    iOffset = aEnt->iNEO(0);
  }
  
  vTrace("\n"_Stz);
  vTrace("=================================================="
         "==================================================\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* show short form of buffer                                                  */
/*----------------------------------------------------------------------------*/
void EntBuffer::vShowShort(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  vTrace("[t%lld h%lld: "_Stz,
         *nPoolBufferNo(),
         pHigherBuffer() ? *pHigherBuffer()->nPoolBufferNo() : 0);
  
  i8 iOffset = aSkipRoots()->iNEO(0);
  while (iOffset > -1) {
    Ent* aEnt = aGetEntryOffset(iOffset);
    
    Key* aKey = _aKeyData(aEnt->aDataArea())
    ;
    i8 iVal = aKey->iGetInt();
    
    if (m_iBufferHeight)
      vTrace("%lld(%lld)"_Stz,iVal,*EntBuffer::pRefdEntBuffer(aEnt)->nPoolBufferNo());
    else
      vTrace("%lld|%lld|"_Stz,iVal,*EntBuffer::pRefdEntBuffer(aEnt)->nPoolBufferNo());
    
    iOffset = aEnt->iNEO(0);
    if (iOffset > -1)
      vTrace(","_Stz);
  }
  vTrace("]"_Stz);
}

/*----------------------------------------------------------------------------*/
/* show sublist of buffers                                                    */
/*----------------------------------------------------------------------------*/
void EntBuffer::vShowSubList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  EntBuffer* pThisL0 = this;
  EntBuffer* pThisL1 = pThisL0->pHigherBuffer();
  
  vTrace("{"_Stz);
  
  if (pThisL0->pPrevBuffer())
    vTrace("<-"_Stz);
  
  EntBuffer* pL0;
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
void EntBuffer::vShowTree(bool bDumpBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  if (! m_pHigherBuffer )
    vTrace("\nIndex Tree:\n"_Stz,m_iBufferHeight);
  
  vTrace("L%lld"_Stz,m_iBufferHeight);
  
  for (EntBuffer* pBuffer = this;
       pBuffer;
       pBuffer = pBuffer->pNextBuffer()) {
    
    pBuffer->vShowShort();
    
    ASSERT(! pBuffer->pPrevBuffer() || pBuffer == pBuffer->pPrevBuffer()->pNextBuffer());
    ASSERT(! pBuffer->pNextBuffer() || pBuffer == pBuffer->pNextBuffer()->pPrevBuffer());
  }
  
  vTrace("\n"_Stz);
  
  if (bDumpBuffer) {
    for (EntBuffer* pBuffer = this;
         pBuffer;
         pBuffer = pBuffer->pNextBuffer())
      pBuffer->vShow();
  }
  
  if ( ! m_iBufferHeight )
    return;
  
  Ent* aFirstEntry = this->aLeadEntry();
  if (! aFirstEntry)
    return;
  
  Id xId;
  xId = aFirstEntry->xId();
  bn nBuffer = xId.iBuffer();
  
  HeapBuffer* pHeapBuffer =
  reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(nBuffer));
  
  EntBuffer* pChildBuffer =
  reinterpret_cast<EntBuffer*>(pHeapBuffer->aFirstChunk());
  
  pChildBuffer->vShowTree(bDumpBuffer);
}
/*----------------------------------------------------------------------------*/
/* get root of skip list (was aka head entry                                  */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aSkipRoots(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  //return m_aHeadEntry;
  return &m_xSkipRoots;
}

/*----------------------------------------------------------------------------*/
/* splice the given buffer after this buffer                                  */
/*----------------------------------------------------------------------------*/
void EntBuffer::vSpliceAfter(EntBuffer* pSplicedBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  EntBuffer* pThisBuffer = this;
  EntBuffer* pNextBuffer = pThisBuffer->pNextBuffer();
  
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
/* truncate this buffer by removing all entries                               */
/*----------------------------------------------------------------------------*/
void EntBuffer::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  EntBuffer* pNextBuffer;
  
  if (m_iBufferHeight != 0) {
    
    // recursively delete the lower level buffers
    for (Ent* aEnt = aLeadEntry();
         aEnt != nullptr;
         aEnt = aNextEntry(aEnt, 0)) {
      
      // get the index buffer from the entry
      pNextBuffer = EntBuffer::pRefdEntBuffer(aEnt);
      
      ASSERT(pNextBuffer);
      ASSERT(pNextBuffer->eClass() == CN_ENTBUFFER);
      pNextBuffer->vTruncate();
    }
  }
  
  delete this;
}

/*----------------------------------------------------------------------------*/
/* unlink the sublist of EB's following this L0 EB within this L1             */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pUnlinkSubList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  ASSERT(0 == m_iBufferHeight);
  
  EntBuffer* pThisL0 = this;
  EntBuffer* pThisL1 = pThisL0->pHigherBuffer();
  EntBuffer* pNextL0 = pThisL0->pNextBuffer();
  EntBuffer* pNextL1 = pThisL1->pNextBuffer();
  
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
  
  EntBuffer* pThisL0SubList = pNextL0;
  
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
  
  EntBuffer* pFirstL0ofNextL1 = pNextL1->pFirstChildBuffer();
  EntBuffer* pLastL0ofThisL1 = pFirstL0ofNextL1->pPrevBuffer();
  
  // the next chain points to first L0 buffer of next L1
  pThisL0->vNextBuffer(pFirstL0ofNextL1);
  
  // the prev chain of first L0 of next L1 is this L0
  pFirstL0ofNextL1->vPrevBuffer(pThisL0);
  
  // the prev chain of last L0 of sublist terminates
  pLastL0ofThisL1->vNextBuffer(nullptr);
  
  return pThisL0SubList;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* EntBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
