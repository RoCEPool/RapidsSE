/*============================================================================*/
/* IndexBuffer.cpp -- Index Buffer (skip list variant)                        */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IndexBuffer* DBobjects::_pIndexBuffer(void* aAddr)
{
  IndexBuffer* pIndexBuffer = reinterpret_cast<IndexBuffer*>(aAddr);
  
  RASSERT( !pIndexBuffer || CN_INDEXBUFFER == pIndexBuffer->eClass() );
  
  return pIndexBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate an index buffer from the heap                                     */
/*----------------------------------------------------------------------------*/
void* IndexBuffer::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_INDEXBUFFER);
  
  ASSERT(pBuffer);
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* free an index buffer to the heap                                           */
/*----------------------------------------------------------------------------*/
void IndexBuffer::operator delete(void* pIndexBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pIndexBufferAddress);
  
  Heap::vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IndexBuffer::IndexBuffer(Index* pIndex, i8 nBufferLevel)
: Buffer(CN_INDEXBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);

  m_pIndex       = pIndex;
  m_nBufferLevel = nBufferLevel;
  m_nEntries     = 0;
  m_pPrevBuffer  = nullptr;
  m_pNextBuffer  = nullptr;
  m_pHigherLevel = nullptr;
  
  m_xWsp.vZero();
  
  // new the head
  vNewHead();
  m_nEntries++;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);

  _checkInitialised_(CN_INDEXBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IndexBuffer::~IndexBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the buffer level                                                       */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::nBufferLevel(void)
{
  return m_nBufferLevel;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vDump(void)
{
  
  IndexBuffer* pIndexBuffer;
  
  // show this buffer
  vShow();
  
  // recursively dump the lower level buffers
  if (m_nBufferLevel != 0) {
    
    for (Entry* pEntry = aNextEntry(aHead()); pEntry != nullptr; pEntry = aNextEntry(pEntry)) {
      
      // get the index buffer from the entry
      IndexBuffer* pIndexBuffer = pEntry->pIndexBuffer();;
      
      ASSERT(pIndexBuffer);
      ASSERT(pIndexBuffer->eClass() == CN_INDEXBUFFER);
      
      pIndexBuffer->vDump();
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* get number of enteries in the buffer                                       */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::nEntries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return m_nEntries;
}

/*----------------------------------------------------------------------------*/
/* does the buffer have space for nBytes ?                                    */
/*----------------------------------------------------------------------------*/
bool IndexBuffer::bHasSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 iFree = m_xWsp.nFree();
  /*
   
   if (m_xWsp.nWspFree() < nBytes) {
   return false;
   */
  
  /* FOR TESTING SMALL TABLES
   if (m_nBufferLevel == 0) {
   if (m_nEntries > 10)
   return false;
   }
   else {
   if (m_nEntries > 5)
   return false;
   }
   */
  
  if (m_nEntries > 20)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get head entry of skip list in this index buffer                           */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::aHead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // the head entry will have been alloted during constructor
  ASSERT(m_xWsp.nUsed());
  
  // the head entry is the first entry in wsp
  Entry* pHead = _aEntry(m_xWsp.aArea());
  
  return pHead;
}

/*----------------------------------------------------------------------------*/
/* set the higher level index buffer                                          */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexBuffer::pHigherLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return m_pHigherLevel;
}

/*----------------------------------------------------------------------------*/
/* set the higher level index buffer                                          */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vHigherLevel(IndexBuffer* pHigherLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  //ASSERT( ! m_pHigherLevel);
  
  m_pHigherLevel = pHigherLevel;
}

/*----------------------------------------------------------------------------*/
/* insert new entry                                                           */
/*----------------------------------------------------------------------------*/
bool IndexBuffer::bInsert(KeyWsp& xKeyWsp, Id xId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  Entry* pReviseVec[C_FLAT_LEVELS];
  bool   bEqualTo;
  EntWsp xEntWsp;
  
  //vTrace("Inserting key:\n"_Str);
  //xKeyWsp.vShow();
  
  // initialize the EntWsp
  xEntWsp.vId(xId);
  xEntWsp.vAddKey(xKeyWsp);
  
  // get the size of the EntWsp
  i8 nBytes = Entry::nBytesRequired(xEntWsp);
  
  Key xKey(xKeyWsp);
  
  // create a realtional operator array
  RelOpAry* pOpAryPosition = new RelOpAry;
  pOpAryPosition->vAdd(RO_GREATER_EQUAL);
  
  // position the key in the buffer
  Entry* pAfterEntry = aPosition(xKey, pOpAryPosition, bEqualTo, pReviseVec);
  Entry* pNewEntry = aNewEntry(nBytes, xEntWsp.nOffsets(), pAfterEntry);
  
  // set the new entry from the EntWsp
  pNewEntry->vSetEntry(xEntWsp);
  
  //vTrace("\n\n---------INDEX BUFFER AFTER MOVING --------------\n"_Str);
  //vShow();
  //vTrace("-----------------------\n\n"_Str);
  
  // fix the revise vector after the move
  vFixRevise(pReviseVec, pNewEntry);
  
  // get no of offsets of the new entry
  i8 nEntryOffsets = pNewEntry->nOffsets();

  // get the offset of the new entry
  i8 iNewEntryOffset = iToOffset(pNewEntry);
  
  i8 iOffsets = nEntryOffsets;
  
  // Revise the offsets of the enteries using the ReviseVector
  while (--iOffsets >= 0) {
    
    // calculate level into reviseVector for this offset
    i8 iLevel = iOffsets + 1;
    
    // get the right offset from the revise vector
    i8 iRightOffset = pReviseVec[iLevel]->uRightOffset(iOffsets);
    
    // set the right offet in the new entry
    pNewEntry->vRightOffset(iOffsets, iRightOffset);
    
    // get the end offset of the revise vector entry at level
    i8 iRevEndOffset = iToEndOffset( pReviseVec[iLevel]);
    
    // set the right offset for the entry in the revise vector entry
    pReviseVec[iLevel]->vRightOffset(iOffsets,  iNewEntryOffset - iRevEndOffset);
  }
  
  // increment the no of entries
  m_nEntries++;
  
  // fix higher level buffers
  // this is needed in case we inserted a new first key in the buffer
  vFixHigherLevel(pNewEntry);
  
  delete pOpAryPosition;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return m_pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get next entry (eq to right L0) after given entry (null if no next)        */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::aNextEntry(Entry* aEntry)
{
  // get the size of given entry
  i8 nBytes = aEntry->nSize();
  
  // compute the address of possible next entry
  Entry* aNext = pAdd<Entry*>(aEntry,nBytes);
  
  // check for valid entry
  Entry* aLatter = _aEntry(m_xWsp.aLatter());
  if (aNext < aLatter)
    return aNext;
  else
    return nullptr;
}

#define goDownLevel(iLevel) iLevel--;

/*----------------------------------------------------------------------------*/
/* position at entry with lower bound at key                                  */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::aPosition(Key& xKey, RelOpAry* pRelOpAry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  bool   bEqualTo;
  Entry* pEntry;
  
  pEntry = aPosition(xKey, pRelOpAry, bEqualTo, nullptr);
  
  if (pEntry == aHead())
    pEntry = aNextEntry(pEntry);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* position at an index buffer at level iLevel starting from this buffer      */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexBuffer::aPositionLevel(KeyWsp& xKeyWsp, RelOpAry* pOpAryGreater, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  IndexBuffer* pIndexBuffer = this;
  
  i8 iBufferLevel;
  Entry* pEntry;
  
  Key xKey(xKeyWsp);
  
  while (true) {
    iBufferLevel = pIndexBuffer->nBufferLevel();
    
    if (iBufferLevel == iLevel)
      break;
    
    // get the entry
    pEntry = pIndexBuffer->aPosition(xKey, pOpAryGreater);
    ASSERT( pEntry );
    
    // get the index buffer of the entry
    pIndexBuffer = pEntry->pIndexBuffer();
    ASSERT(pIndexBuffer);
    ASSERT(pIndexBuffer->eClass() == CN_INDEXBUFFER);
  }
  
  return pIndexBuffer;
}

/*----------------------------------------------------------------------------*/
/* remove existing entry                                                      */
/*----------------------------------------------------------------------------*/
bool IndexBuffer::bRemove(Key* aKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vShow(void)
{
  s32 sHigherLevel;
  if (m_pHigherLevel)
    StrLib::wToString(m_pHigherLevel->m_nBufferLevel, sHigherLevel.s());
  else
    sHigherLevel = "..."_Str;
  
  vTrace("IndexBuffer %p (buffer %lld): level L%lld HigerLevel %p (L%s) entries %lld prevBuffer %p nextBuffer %p \n"_Str,
         this,
         *nBufferNo(),
         m_nBufferLevel,
         m_pHigherLevel,
         sHigherLevel.aStr(),
         m_nEntries,
         m_pPrevBuffer,
         m_pNextBuffer);
  
  for (Entry* pEntry = aHead(); pEntry != nullptr; pEntry = aNextEntry(pEntry)) {
    vTrace("%8lld: "_Str, nSub(pEntry, m_xWsp.aArea()));
    pEntry->vShow(iToOffset(pEntry));
  }
  
  vTrace("\n"_Str);
  
}

/*----------------------------------------------------------------------------*/
/* split the buffer                                                           */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexBuffer::pSplitBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // split the buffer into two
  i8 iSplitAtEntry = m_nEntries/2;
  
  // get the split entry
  Entry* pLastEntry = pGetSplitEntry(iSplitAtEntry);
  
  // the last entry in the buffer after which the split will happen
  //pLastEntry->vShow();
  
  Entry* pReviseVec[C_FLAT_LEVELS];
  bool   bEqualTo;
  
  // create a relational operator array
  RelOpAry* pOpAryFind = new RelOpAry;
  pOpAryFind->vAdd(RO_GREATER_EQUAL);
  
  // find the position of the last entry in the buffer
  Entry* pAfterEntry = aPosition(*pLastEntry->aKey(), pOpAryFind, bEqualTo, pReviseVec);
  ASSERT(pAfterEntry == pLastEntry);
  
  i8 iLastEntryOffset = iToOffset(pLastEntry);
  i8 iOffsets = C_FLAT_LEVELS - 1;
  
  // Revise the offsets of the enteries using the ReviseVector
  while (--iOffsets >= 0) {
    
    // calculate level into reviseVector for this offset
    i8 iLevel = iOffsets + 1;
    
    // get the right offset from the revise vector
    i8 iRightOffset = pReviseVec[iLevel]->uRightOffset(iOffsets);
    i8 iEntryOffset = iToOffset(pReviseVec[iLevel]);
    i8 iEntrySize   = pReviseVec[iLevel]->nSize();
    
    if (iRightOffset + iEntryOffset + iEntrySize >= iLastEntryOffset)
      pReviseVec[iLevel]->vRightOffset(iOffsets, C_NIL_OFFSET);
  }
  
  // get no of offsets of the last entry
  i8 nEntryOffsets = pLastEntry->nOffsets();
  iOffsets = nEntryOffsets;
  
  while (--iOffsets >= 0) {
    // set the right offet in the last entry to C_NIL_OFFSET
    pLastEntry->vRightOffset(iOffsets, C_NIL_OFFSET);
  }
  
  // get the split entry from the lastEntry
  Entry* pSplitEntry = aNextEntry(pLastEntry);
  
  // get the address of the entry to split at
  char* pSplitAt = reinterpret_cast<char*>(pSplitEntry);
  
  // calculate bytes to copy to new buffer
  i8 nSize = nSub(m_xWsp.aLatter(), pSplitAt);
  
  // allocate a new buffer at the same index level
  IndexBuffer* pNewBuffer = new IndexBuffer(m_pIndex, m_nBufferLevel);
  
  // copy entries starting at the splitEntry from this buffer to the new buffer
  pNewBuffer->vCopyEntries(this, pSplitEntry);
  
  // give back nSize bytes to wsp in original buffer
  m_xWsp.vGive(nSize);
  ASSERT(m_xWsp.aLatter() == pAdd<void*>((void*)pLastEntry,pLastEntry->nSize()));
  
  m_nEntries -= iSplitAtEntry;
  
  // now link the two buffers
  pNewBuffer->m_pNextBuffer = pNextBuffer();
  pNewBuffer->m_pPrevBuffer = this;
  
  m_pNextBuffer = pNewBuffer;
  
  delete pOpAryFind;
  
  return pNewBuffer;
}

/*----------------------------------------------------------------------------*/
/* verify                                                                     */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vVerify(void)
{
  
  //-------------------------------------------------------------------
  // need to rework....
  //-------------------------------------------------------------------
  
  Entry* pEntryA = aNextEntry(aHead());
  if ( ! pEntryA )
    return;
  
  Entry* pEntryB = aNextEntry(pEntryA);
  if ( ! pEntryB )
    return;
  
  while (pEntryB) {
    
    Key* pKeyA = pEntryA->aKey();
    Key* pKeyB = pEntryB->aKey();
    
    //pKeyA->vShow(CT_INTEGER);
    //pKeyB->vShow(CT_INTEGER);
    
    ASSERT(*pEntryA->aKey() <= pEntryB->aKey());
    
    pEntryA = pEntryB;
    pEntryB = aNextEntry(pEntryA);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy entries from buffer from fromEntry                                                                     */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vCopyEntries(IndexBuffer* pFromBuffer, Entry* pFromEntry)
{
  
  Entry* pEntry = pFromEntry;
  while (pEntry) {
    
    KeyWsp xKeyWsp;
    
    xKeyWsp.vAddKey(pEntry->aKey(), m_pIndex->pIdxColVec());
    Key xKey(xKeyWsp);
    
    bInsert(xKeyWsp, pEntry->xId());
    
    pEntry = pFromBuffer->aNextEntry(pEntry);
  }
}

/*----------------------------------------------------------------------------*/
/* dump the revise vector                                                     */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vDumpRevise(Entry** ppReviseVec)
{
  for (i8 iIndex = 0; iIndex < C_FLAT_OFFSETS; iIndex++) {
    i8 iLevel = iIndex + 1;
    i8 iReviseOffset = ppReviseVec[iLevel]->uRightOffset(iIndex);
    vTrace("%p pReviseVec[%lld]->uRightOffset(%lld) = %lld\n"_Str,
           ppReviseVec[iLevel], iLevel, iIndex, iReviseOffset);
  }
}

/*----------------------------------------------------------------------------*/
/* fix the higher level                                                        */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vFixHigherLevel(Entry* pNewEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  bool bMatches;
  
  // fix higher levels only if there are any. Duh!
  if ( ! m_pHigherLevel )
    return;
  
  // get the first entry right of head at offset 0
  Entry* pFirstEntry = aRightEntry(aHead(), 0);
  
  // if the entry we just added is not the first entry , nothing to fix
  if (pNewEntry != pFirstEntry)
    return;
  
  // we need to update the higher level buffer entry key
  Entry* pOldFirstEntry = aNextEntry(pNewEntry);
  
  //pOldFirstEntry->vShow(pOldFirstEntry->nOffsets());
  //pNewEntry->vShow(pNewEntry->nOffsets());
  
  // create a relational operator array
  RelOpAry* pOpAryFind = new RelOpAry;
  pOpAryFind->vAdd(RO_EQUAL);
  
  KeyWsp xOldKeyWsp;
  xOldKeyWsp.vAddKey(pOldFirstEntry->aKey(), m_pIndex->pIdxColVec());
  
  //pOldFirstEntry->aKey()->vShow(m_pIndex->pIdxColVec());
  
  Key xKey(xOldKeyWsp);
  //xKey.vShow(pColVec);
  
  // find the oldEntry in the higher level buffer
  Entry* pFoundEntry = m_pHigherLevel->aPosition(xKey, pOpAryFind);
  
  // assert that we found one
  ASSERT(pFoundEntry);
  //pFoundEntry->aKey()->vShow(m_pIndex->pIdxColVec());
  
  // assert the key of the higher level entry and the old entry match
  ASSERT(pFoundEntry->aKey()->bMatches(pOldFirstEntry->aKey(), m_pIndex->pIdxColVec(), pOpAryFind));
  
  // get the new entry key
  KeyWsp xNewKeyWsp;
  xNewKeyWsp.vAddKey(pNewEntry->aKey(), m_pIndex->pIdxColVec());
  
  EntWsp xEntWsp;
  xEntWsp.vAddKey(xNewKeyWsp);
  
  // now replace the key in the higher level buffer
  pFoundEntry->vSetKey(xEntWsp);
  
  //vTrace("**********AFTER**************\n"_Str);
  //pFoundEntry->vShow(pFoundEntry->nOffsets());
  //m_pHigherLevel->vDump();
  //vTrace("************************\n"_Str);
}

/*----------------------------------------------------------------------------*/
/* fix the revise vector                                                      */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vFixRevise(Entry** ppReviseVec, Entry* pNewEntry)
{
  
  //vTrace("REVISE VECTOR BEFORE FIXING\n"_Str);
  //vDumpRevise(ppReviseVec);
  //vTrace("\n"_Str);

  // get no of offsets of the new entry
  i8 nEntryOffsets = pNewEntry->nOffsets();
  
  // get entry size
  i8 nNewEntrySize = pNewEntry->nSize();
  
  // get the offset of the new entry
  i8 iNewEntryOffset = iToOffset(pNewEntry);

  for (i8 iOffset = 0; iOffset < C_FLAT_OFFSETS; iOffset++) {
    
    // get the level
    i8 iLevel = iOffset + 1;
    
    // get the reviseVector's rightOffset at this level
    i8 iReviseRightOffset = ppReviseVec[iLevel]->uRightOffset(iOffset);
    
    // if C_NIL_OFFSET, nothing to revise
    if (iReviseRightOffset == C_NIL_OFFSET )
      continue;
    
    i8 iReviseEntryOffset = iToOffset(ppReviseVec[iLevel]);
    i8 iReviseEntrySize = ppReviseVec[iLevel]->nSize();

    // is newEntry added between the the reviseEntry and reviseOffset ?
    if (iNewEntryOffset <= iReviseEntryOffset + iReviseEntrySize + iReviseRightOffset) {
      
      // no of offsets of reviseVector entry is >= no of new entry offsets
      if (iOffset >= nEntryOffsets) {
        // adjust the reviseVector offset with the newEntrySize
        ppReviseVec[iLevel]->vRightOffset(iOffset,iReviseRightOffset + nNewEntrySize);
      }
      else {
        // no of offsets of reviseVector entry is >= no of new entry offsets
        i8 nSize = iNewEntryOffset - (iReviseEntrySize + iReviseEntryOffset);
        
        // adjust the reviseVector to point to the newEntry
        ppReviseVec[iLevel]->vRightOffset(iOffset,iReviseRightOffset - nSize);
      }
    }

  }
  
  //vTrace("REVISE VECTOR AFTER FIXING\n"_Str);
  //vDumpRevise(ppReviseVec);
  //vTrace("\n"_Str);
}

/*----------------------------------------------------------------------------*/
/* get the split entry                                                        */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pGetSplitEntry(i8 iSplitAtEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // start with the head
  Entry* pSplitEntry = aHead();
  
  i8 iEntries = 0;
  while (pSplitEntry) {
    
    iEntries++;
    
    // found split point
    if (iEntries == iSplitAtEntry)
      break;
    
    // get next entry
    pSplitEntry = this->aNextEntry(pSplitEntry);
  }
  
  return pSplitEntry;
}

/*----------------------------------------------------------------------------*/
/* generate a new random level                                                */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::iLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 iRndLevel = Random::uLogLevel(C_FLAT_LEVELS,C_FLAT_FREQ);
  ASSERT(iRndLevel >= 0 && iRndLevel < C_FLAT_LEVELS);
  
  return 0;
}

/*----------------------------------------------------------------------------*/
/* move subset of entries left (thereby removing an entry)                    */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vMoveLeft(void* aDst, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  /*
   
   we are in the process of removing entry E from between A..D and F..Z
   
   1) we compute the source address
   2) we move the F..Z left
   3) we give space back into wsp free space
   
       aDst --+   +-- aSrc
              v   v
   ------------------------------------------+
      | A..D | E | F..Z |       free         |
   ------------------------------------------+
   ------------------------------------------+
      | A..D | F..Z |         free           |
   ------------------------------------------+
   
   aDst is the first byte of entry E
   nBytes is the size of entry E
   aSrc will be the first byte of F..Z
   
   note that MemLib::vMove() is used because the move/copy overlaps
   
   */
  
  // compute the source address
  void* aSrc = pAdd<void*>(aDst,nBytes);
  
  // move the bytes left
  MemLib::vMove(aDst,aSrc,nBytes);
  
  // release space back to wsp
  m_xWsp.vGive(nBytes);
  
  // for the moment we'll zero the bytes put back into the free area
  void* aLatter = m_xWsp.aLatter();
  MemLib::vZero(aLatter,nBytes);
}

/*----------------------------------------------------------------------------*/
/* move subset of entries right (thereby making room for a new entry)         */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vMoveRight(void* aSrc, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  /*
   
   we are in the process of inserting entry E between A..D and F..Z
   
   1) the caller has checked there is sufficient free space in wsp
   2) we first take space for E in wsp
   3) compute the destination address
   3) then move F..Z right by bytes alloted for E
   
       aSrc --+      +-- aDst
              v      v
   ------------------------------------------+
      | A..D | F..Z | alloted | free         |
   ------------------------------------------+
   ------------------------------------------+
      | A..D | alloted | F..Z | free         |
   ------------------------------------------+
   
   aSrc is the first byte where E is to be inserted (i.e. first byte of F)
   nBytes is the size of E
   aDst will be the first byte of alloted space
   
   note that MemLib::vMove() is used because the move/copy overlaps
   
   */
  
  // the caller has checked there is sufficient space
  ASSERT(m_xWsp.nFree() >= nBytes);
  
  // allot space for entry E in wsp
  //m_xWsp.vTake(nBytes);
  
  // compute the destination address
  void* aDst = pAdd<void*>(aSrc,nBytes);
  
  // bytes to copy
  i8 nBytesToCopy = nSub(m_xWsp.aLatter(), aSrc) - nBytes;
  
  // move F..Z right
  MemLib::vMove(aDst,aSrc,nBytesToCopy);
  
  // for the moment we'll zero the bytes of entry E
  MemLib::vZero(aSrc,nBytes);
}

/*----------------------------------------------------------------------------*/
/* allot new entry after given entry (return null if insufficient space)      */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::aNewEntry(i8 nBytes, i8 nOffsets, Entry* aAfterEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // There should be  enough space for the new entry
  if (m_xWsp.nFree() < nBytes)
    return nullptr;
  
  // allot space for the new entry
  m_xWsp.vTake(nBytes);
  Entry* aLatter = _aEntry(m_xWsp.aLatter());
  
  Entry* aNewEntry;
  
  if (aAfterEntry) {
  // compute address of new entry
    i8 nAfterBytes = aAfterEntry->nSize();
    aNewEntry = pAdd<Entry*>(aAfterEntry,nAfterBytes);

    // move subset of entries to right to make room for new entry
    vMoveRight(aNewEntry,nBytes);
  }
  else {
    aNewEntry = _aEntry(m_xWsp.aArea());
  }
  
  // run the constructor of the new entry
  new (aNewEntry) Entry(nBytes,nOffsets);
  
  return aNewEntry;
}

/*----------------------------------------------------------------------------*/
/* allot head entry (during constructor)                                      */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vNewHead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);

  // must be the first item in wsp
  ASSERT( ! m_xWsp.nUsed() );
  
  // how many bytes are required
  i8 nBytes = Entry::nBytesRequired(C_FLAT_OFFSETS, 0);
  
  // allot the head
  
  Entry* aHead = aNewEntry(nBytes,C_FLAT_OFFSETS,nullptr);

  // initialize the head
  for (i8 iOffset = 0; iOffset < C_FLAT_OFFSETS; iOffset++)
    aHead->vRightOffset(iOffset, C_NIL_OFFSET);
}


/*----------------------------------------------------------------------------*/
/* positition to entry (optional set revise vector)                           */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::aPosition(Key& xKey, RelOpAry* pOpAryPosition, bool& bEqualTo, Entry** aReviseVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // start from head at known highest level
  Entry* pCurrentEntry = aHead();
  i8 iCurrentLevel = C_FLAT_LEVELS - 1;
  
  do {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inner loop: go right a number of nodes until we find
    //   ObjectInsert < ObjectNext at current level
    while (Entry* pNextEntry = aRightEntry(pCurrentEntry, iCurrentLevel)) {
      
        // compare the inserting object with next object
        bool bGreaterEqual = xKey.bMatches(pNextEntry->aKey(), m_pIndex->pIdxColVec(), pOpAryPosition);
      
        if (bGreaterEqual) {
            // pObjectInsert is >= pObjectNext
            //vTrace("  %lld after %lld (level %lld)\n"_Str,
            //       (i8)pObjectInsert,(i8)pNextEntry->uRight(),iCurrentLevel);
    
           // go right one node
            pCurrentEntry = aRightEntry(pCurrentEntry, iCurrentLevel);
          }
       else {
            // pObjectInsert is < pObjectNext or pObjectNext is null
            //vTrace("  %lld before %lld (level %lld)\n"_Str,
            //       (i8)pObjectInsert,(i8)pNextEntry->uRight(),iCurrentLevel);
    
            break;
          }
      }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // outer loop: now having gone right as far as we go down one single level

    // first record current node as a revision point
    if (aReviseVec)
      aReviseVec[iCurrentLevel] = pCurrentEntry;
   
   // now go down one level
    goDownLevel(iCurrentLevel);
    
    // and repeat till we have a complete revision vector
  } while (iCurrentLevel >= 0);
  
  return pCurrentEntry;
}


/*----------------------------------------------------------------------------*/
/* get right entry at given level (returns null if no right)                  */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::aRightEntry(Entry* aEntry, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  Entry* aRightEntry;
  
  /*
   
  1) entries are in key order within a given buffer
  2) all entries have an implied level 0
  2) this means the right entry for level 0 for an entry is 
       the next entry in the buffer
  3) entries may also have level 1 or level 1 & level 2 right entries
  
  pEntry is the entry we want a right entry from
  iLevel is the level we want the right entry of
   
  */
  
  // the caller must request a valid right entry of pEntry
  ASSERT(iLevel >= 0 && iLevel < C_FLAT_LEVELS);

  if (0 == iLevel) {
    // for level 0, the right entry is the next entry
    aRightEntry = aNextEntry(aEntry);
  }
  else {
    
    i8 iOffset = iLevel - 1;
    ASSERT(iOffset >= 0 && iOffset < aEntry->nOffsets());
    
    // for levels 1 & 2 the right entry is m_xWsp + rightOffset[iRight]
    i8 iBytes = aEntry->nSize();
    i8 iRightOffset = aEntry->uRightOffset(iOffset);
    if (C_NIL_OFFSET == iRightOffset)
      aRightEntry = nullptr;
    else
      aRightEntry = pAdd<Entry*>(aEntry,iRightOffset+iBytes);
  }
 
  ASSERT( ! aRightEntry ||
          ( aRightEntry > aHead() &&
            aRightEntry < _aEntry(m_xWsp.aLatter()) ) );
  
  return aRightEntry;
}

/*----------------------------------------------------------------------------*/
/* converts iOffset to aEntry                                                 */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::aToEntry(i8 iOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // check the offset is within bounds
  ASSERT(iOffset >= 0 && iOffset < m_xWsp.nUsed());
  
  // calculate address of entry
  Entry* aEntry = pAdd<Entry*>(m_xWsp.aArea(),iOffset);
  
  return aEntry;
}

/*----------------------------------------------------------------------------*/
/* converts aEntry to iOffset at end of entry                                 */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::iToEndOffset(Entry* aEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 iOffset = nSub(aEntry,m_xWsp.aArea());
  
  ASSERT(iOffset >= 0 && iOffset < m_xWsp.nSize());
  
  return iOffset + aEntry->nSize();
}

/*----------------------------------------------------------------------------*/
/* converts aEntry to iOffset                                                 */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::iToOffset(Entry* aEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 iOffset = nSub(aEntry,m_xWsp.aArea());
  
  ASSERT(iOffset >= 0 && iOffset < m_xWsp.nSize());
  
  return iOffset;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
