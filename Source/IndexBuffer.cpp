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
  vAllotHead();
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
    
    for (Entry* aEntry = aNextEntry(aHead()); aEntry != nullptr; aEntry = aNextEntry(aEntry)) {
      
      // get the index buffer from the entry
      IndexBuffer* pIndexBuffer = aEntry->pIndexBuffer();;
      
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
 
   
  //if (iFree < nBytes)
  // return false;
   
  
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
  Entry* aHead = _aEntry(m_xWsp.aArea());
  
  return aHead;
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
/* get the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
Index* IndexBuffer::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return m_pIndex;
}

/*----------------------------------------------------------------------------*/
/* insert new entry into this index buffer                                    */
/*----------------------------------------------------------------------------*/

#ifndef FIX_PRIOR

bool IndexBuffer::bInsert(KeyWsp& xKeyWsp, Id xId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  //vTrace("Inserting key:\n"_Str);
  //xKeyWsp.vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert a new entry into the skip list
  
  // find the entry after which the new entry.key will be inserted
  Entry* aReviseVec[C_FLAT_LEVELS];
  Entry* aAfterEntry = aPosition(xKeyWsp.aKey(), JC_AFTER, aReviseVec);
  
  // create a new entry after the prior entry and set it
  EntWsp xEntWsp(xId,xKeyWsp);
  i8 nEntryBytes = Entry::nBytesRequired(xEntWsp);
  Entry* aNewEntry = aAllotEntry(nEntryBytes, xEntWsp.nOffsets(), aAfterEntry);
  aNewEntry->vSetEntry(xEntWsp);
  
  //vTrace("\n\n---------INDEX BUFFER AFTER MOVING --------------\n"_Str);
  //vShow();
  //vTrace("-----------------------\n\n"_Str);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // revise the offsets of the prior entries from the revise vector
  
  // fix the revise vector after the move
  vFixRevise(aReviseVec, aNewEntry);
  
  // get the absolute offset of the new entry
  i8 iNewEntryOffsetAbs = iToOffsetAbs(aNewEntry);
  
  // step thru each offset to be revised
  for (i8 iOffsetNo = aNewEntry->nOffsets() - 1; iOffsetNo >= 0; iOffsetNo--) {
    
    // get prior entry being revised
    i8 iLevelNo = iOffsetNo + 1;
    Entry* aPriorEntry = aReviseVec[iLevelNo];
    
    // copy the right offset from the prior entry into the new entry
    i8 iRightOffset = aPriorEntry->uRightOffset(iOffsetNo);
    aNewEntry->vRightOffset(iOffsetNo, iRightOffset);
    
    // set the right offset in the prior entry to offset of new entry
    i8 iPriorOffsetAbs = iToEndOffset(aPriorEntry);
    i8 iNewEntryOffset = iNewEntryOffsetAbs - iPriorOffsetAbs;
    aPriorEntry->vRightOffset(iOffsetNo, iNewEntryOffset);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fix higher level buffers
  // this is needed in case we inserted a new first key in the buffer
  
  vFixHigherLevel(aNewEntry);
  
  return true;
}

#else //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif

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
Entry* IndexBuffer::aPosition(KeyWsp& xKeyWsp, eJC eJuxtaCompare)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  Entry* aEntry;
  
  aEntry = aPosition(xKeyWsp.aKey(), eJuxtaCompare, nullptr);
  
  if (aEntry == aHead())
    aEntry = aNextEntry(aEntry);
  
  return aEntry;
}

/*----------------------------------------------------------------------------*/
/* position at an index buffer at level iLevel starting from this buffer      */
/*--------------------ƒ--------------------------------------------------------*/
IndexBuffer* IndexBuffer::pPositionLevel(KeyWsp& xKeyWsp, eJC eJuxtaCompare, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  IndexBuffer* pIndexBuffer = this;
  
  i8 iBufferLevel;
  Entry* aEntry;
  
  while (true) {
    iBufferLevel = pIndexBuffer->nBufferLevel();
    
    if (iBufferLevel == iLevel)
      break;
    
    // get the entry
    aEntry = pIndexBuffer->aPosition(xKeyWsp, eJuxtaCompare);
    ASSERT( aEntry );
    
    // get the index buffer of the entry
    pIndexBuffer = aEntry->pIndexBuffer();
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
  s256 sHigherLevel;
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
  
  for (Entry* aEntry = aHead(); aEntry != nullptr; aEntry = aNextEntry(aEntry)) {
    vTrace("%8lld: "_Str, nSub(aEntry, m_xWsp.aArea()));
    aEntry->vShow(iToOffsetAbs(aEntry), m_pIndex->pIdxColVec());
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
  Entry* aLastEntry = pGetSplitEntry(iSplitAtEntry);
  
  // the last entry in the buffer after which the split will happen
  //pLastEntry->vShow(0,m_pIndex->pIdxColVec());
  
  Entry* aReviseVec[C_FLAT_LEVELS];
  
  // find the position of the last entry in the buffer
  KeyWsp xKeyWsp(aLastEntry->aKey(), m_pIndex->pIdxColVec());
  
  Entry* aAfterEntry = aPosition(xKeyWsp.aKey(), JC_AFTER, aReviseVec);
  
  // Note this ASSERT fails if we have dulplicates in the table     //ALKA: TODO
  // Need to fix when we cater for non-unique indexes
  //pAfterEntry->vShow(0,m_pIndex->pIdxColVec());
  ASSERT(aAfterEntry == aLastEntry);
  
  i8 iLastEntryOffset = iToOffsetAbs(aLastEntry);
  i8 iOffsets = C_FLAT_LEVELS - 1;
  
  // Revise the offsets of the enteries using the ReviseVector
  while (--iOffsets >= 0) {
    
    // calculate level into reviseVector for this offset
    i8 iLevel = iOffsets + 1;
    
    // get the right offset from the revise vector
    i8 iRightOffset = aReviseVec[iLevel]->uRightOffset(iOffsets);
    i8 iEntryOffset = iToOffsetAbs(aReviseVec[iLevel]);
    i8 iEntrySize   = aReviseVec[iLevel]->nSize();
    
    if (iRightOffset + iEntryOffset + iEntrySize >= iLastEntryOffset)
      aReviseVec[iLevel]->vRightOffset(iOffsets, C_NIL_OFFSET);
  }
  
  // get no of offsets of the last entry
  i8 nEntryOffsets = aLastEntry->nOffsets();
  iOffsets = nEntryOffsets;
  
  while (--iOffsets >= 0) {
    // set the right offet in the last entry to C_NIL_OFFSET
    aLastEntry->vRightOffset(iOffsets, C_NIL_OFFSET);
  }
  
  // get the split entry from the lastEntry
  Entry* aSplitEntry = aNextEntry(aLastEntry);
  
  // get the address of the entry to split at
  char* pSplitAt = reinterpret_cast<char*>(aSplitEntry);
  
  // calculate bytes to copy to new buffer
  i8 nSize = nSub(m_xWsp.aLatter(), pSplitAt);
  
  // allocate a new buffer at the same index level
  IndexBuffer* pNewBuffer = new IndexBuffer(m_pIndex, m_nBufferLevel);
  
  // copy entries starting at the splitEntry from this buffer to the new buffer
  pNewBuffer->vCopyEntries(this, aSplitEntry);
  
  // give back nSize bytes to wsp in original buffer
  m_xWsp.vGive(nSize);
  ASSERT(m_xWsp.aLatter() == pAdd<void*>((void*)aLastEntry,aLastEntry->nSize()));
  
  m_nEntries -= iSplitAtEntry;
  
  // now link the two buffers
  pNewBuffer->m_pNextBuffer = pNextBuffer();
  pNewBuffer->m_pPrevBuffer = this;
  
  m_pNextBuffer = pNewBuffer;
  
  return pNewBuffer;
}

/*----------------------------------------------------------------------------*/
/* verify                                                                     */
/*----------------------------------------------------------------------------*/
bool IndexBuffer::bVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // verify the entries at each level
  i8 nLevel = C_FLAT_LEVELS - 1;
  
  do {
    
    Entry* aEntryA = aHead();
    if ( ! aEntryA )
      return true;
    
    if (aEntryA->nOffsets() < (u8)nLevel)
      return true;
    
    Entry* aEntryB = aRightEntry(aEntryA, nLevel);
    if ( ! aEntryB )
      return true;
  
    while (aEntryB) {
      
      Key* pKeyA = aEntryA->aKey();
      Key* pKeyB = aEntryB->aKey();
      
      if (aEntryA != aHead()) {
        bool bAfter = aEntryB->aKey()->bJuxtaAfter(aEntryA->aKey(), m_pIndex->pIdxColVec());
        if ( ! bAfter )
          return false;
      }
      
      aEntryA = aEntryB;
      aEntryB = aNextEntry(aEntryA);
    }
  } while (--nLevel >= 0 );
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy entries from buffer from fromEntry                                                                     */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vCopyEntries(IndexBuffer* pFromBuffer, Entry* aFromEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  Entry* aEntry = aFromEntry;
  i8 nLevel = pFromBuffer->nBufferLevel();
  while (aEntry) {
    
    KeyWsp xKeyWsp;
    
    xKeyWsp.vAddKey(aEntry->aKey(), m_pIndex->pIdxColVec());
    
    bInsert(xKeyWsp, aEntry->xId());
    
    if (nLevel > 0) {
      IndexBuffer* pIndexBuffer = aEntry->pIndexBuffer();
      pIndexBuffer->m_pHigherLevel = this;
    }
    aEntry = pFromBuffer->aNextEntry(aEntry);
    
  }
}

/*----------------------------------------------------------------------------*/
/* dump the revise vector                                                     */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vDumpRevise(Entry** ppReviseVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  for (i8 iIndex = 0; iIndex < C_FLAT_OFFSETS; iIndex++) {
    i8 iLevel = iIndex + 1;
    i8 iReviseOffset = ppReviseVec[iLevel]->uRightOffset(iIndex);
    vTrace("%p pReviseVec[%lld]->uRightOffset(%lld) = %lld\n"_Str,
           ppReviseVec[iLevel], iLevel, iIndex, iReviseOffset);
  }
}

/*----------------------------------------------------------------------------*/
/* fix the higher level                                                       */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vFixHigherLevel(Entry* aNewEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  /*
   
   If a new entry is inserted in a buffer and its the first entry (not the head)
   then if this buffer has a higher level buffer above it, then we need to
   change the entry pointing to this buffer in that buffer
   
  */
  
  // fix higher levels only if there are any. Duh!
  if ( ! m_pHigherLevel )
    return;
  
  // get the first entry right of head at offset 0
  Entry* aFirstEntry = aRightEntry(aHead(), 0);
  
  // if the entry we just added is not the first entry , nothing to fix
  if (aNewEntry != aFirstEntry)
    return;
  
  /*
   
  -------------
  |Ka Kb ......|               L(N+1) buffer
  -------------
  |   |
  |    -----------
  |               |
  V               V
  ------------- -------------
  |Ka........ | | Kb.........|  LN bufer
  ------------- -------------
  
   
   Kc gets inserted before Ka, we need to remove Ka and insert Kc instead in L(N+1) buffer
   
   -------------
   |Kc Kb .....|                 L(N+1) buffer
   -------------
   |   |
   |    -----------
   |               |
   V               V
   ------------- -------------
   |Kc Ka...... | | Kb.........|  LN bufer
   ------------- -------------

  */
  
  // we need to update the higher level buffer entry key
  Entry* aOldFirstEntry = aNextEntry(aNewEntry);

  KeyWsp xOldKeyWsp;
  xOldKeyWsp.vAddKey(aOldFirstEntry->aKey(), m_pIndex->pIdxColVec());
  
  // create akeyWsp with the newEntry key
  KeyWsp xNewKeyWsp;
  xNewKeyWsp.vAddKey(aNewEntry->aKey(), m_pIndex->pIdxColVec());
  
  IndexBuffer* pHigherLevel = m_pHigherLevel;
  
  while (pHigherLevel) {
    pHigherLevel->vReplace(xOldKeyWsp, xNewKeyWsp);
    ASSERT(pHigherLevel->bVerify());
    pHigherLevel = pHigherLevel->m_pHigherLevel;
  }
}

/*----------------------------------------------------------------------------*/
/* replace old key with new key in the index buffer                           */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vReplace(KeyWsp& xOldKeyWsp, KeyWsp& xNewKeyWsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);

  // find the oldEntry in the higher level buffer
  Entry* aFoundEntry = aPosition(xOldKeyWsp, JC_SAME);

  // assert that we found one
  ASSERT(aFoundEntry);

  // assert the key of the higher level entry and the old entry match
  ASSERT(aFoundEntry->aKey()->bJuxtaCompare(JC_SAME, xOldKeyWsp.aKey(), m_pIndex->pIdxColVec()));

  // get the size of the old key
  u8 uOldKeySize = xOldKeyWsp.nWspUsed();

  // get the size of the new key
  u8 uNewKeySize = xNewKeyWsp.nWspUsed();

  i8 nOffsets = aFoundEntry->nOffsets();
  Id xId = aFoundEntry->xId();

  // create an entry wsp from the pFoundEntry and the new key
  EntWsp xEntWsp(nOffsets);
  xEntWsp.vAddKey(xNewKeyWsp);
  xEntWsp.vId(xId);
  for (i8 i=0; i<nOffsets; i++) {
    xEntWsp.vRightOffset(i, aFoundEntry->uRightOffset(i));
  }

  i8 iSize = aFoundEntry->nSize();

  if (uOldKeySize == uNewKeySize) {
    
    /*
     The old and new key are the same size, just change the key in the entry
     
     -------------    -------------
     |Ka(m bytes) |   |Kc(m bytes) |
     -------------    -------------
     
     We keep the same offsets etc. and just replace the key in the entry
     */
    
    // just replace the key in the buffer
    aFoundEntry->vKey(xEntWsp);
    return;
  }

  // create a newEntry at the same location as the foundEntry
  Entry* aNewEntry = _aEntry(aFoundEntry);
  i8 nDifference;

  if (uOldKeySize > uNewKeySize) {
    /*
     The new key is shorter than the old key
     
     ---------------     ------------
     |Ka(m bytes)    |   |Kc(m-nbytes)|
     ---------------     ------------
     
     We will moveLeft in the buffer reclaiming n bytes
     */
    
    nDifference = uOldKeySize - uNewKeySize;
    
    // calculate the pDst address
    void* pDst = pAdd<void*>(aFoundEntry, uNewKeySize);
    
    // move left nDifference bytes
    vMoveLeft(pDst, nDifference);
    m_xWsp.vGive(nDifference);
  }
  else {
    
    /*
     The new key is longer than the old key
     
     ---------------     --------------------
     |Ka(m bytes)    |   |Kc(m+n bytes)       |
     ---------------     --------------------
     
     We will moveRight in the buffer making space for n bytes
     */

    ASSERT(uOldKeySize < uNewKeySize);
    nDifference = uNewKeySize - uOldKeySize;
    ASSERT(nDifference);
    
    // calculate the pDst address
    void* pDst = pAdd<void*>(aFoundEntry, iSize);
    
    // need to ensure we have space in the buffer
    if (m_xWsp.nFree() < nDifference) {
      ASSERT(0);                                                   //ALKA: TODO
    }
    
    ASSERT(m_xWsp.nFree() >= nDifference);
    
    // move right nDifference bytes
    vMoveRight(pDst, nDifference);
    m_xWsp.vTake(nDifference);
  }

  i8 nBytes =  iSize - (uOldKeySize - uNewKeySize);
  new (aNewEntry) Entry(nBytes, nOffsets);
  aNewEntry->vSetEntry(xEntWsp);
  ASSERT(aNewEntry->nSize() == nBytes);

  // fix the right offsets in the head
  Entry* aHeadEntry = aHead();
  
  for (u8 i = 0; i < aHeadEntry->nOffsets(); i++) {
    
    // here we adjust the iRightOffset if offset is after oldEntry
    u8 uOldEntrySize = aFoundEntry->nSize();
    u8 uRightOffset = aHeadEntry->uRightOffset(i);
    nDifference = uOldKeySize - uNewKeySize;
    
    if ( uRightOffset != C_NIL_OFFSET) {
      if ((i8)uRightOffset - nDifference >= (i8)uOldEntrySize) {
        aHeadEntry->vRightOffset(i,(i8)(aHeadEntry->uRightOffset(i)) - nDifference);
      }
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* fix the revise vector                                                      */
/*----------------------------------------------------------------------------*/
#ifndef FIX_PRIOR

void IndexBuffer::vFixRevise(Entry** aaReviseVec, Entry* aNewEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  //vTrace("REVISE VECTOR BEFORE FIXING\n"_Str);
  //vDumpRevise(ppReviseVec);
  //vTrace("\n"_Str);

  // get no of offsets, entry size and offset for the new entry
  u8 nNewEntryOffsets = aNewEntry->nOffsets();
  i8 nNewEntryBytes = aNewEntry->nSize();
  u8 uNewEntryOffsetAbs = iToOffsetAbs(aNewEntry);

  // iterate thru each possible offset
  for (u8 uOffsetNo = 0; uOffsetNo < C_FLAT_OFFSETS; uOffsetNo++) {
    
    // get prior entry being revised
    u8 uLevelNo = uOffsetNo + 1;
    Entry* aPriorEntry = aaReviseVec[uLevelNo];
    
    // get prior entry's right offset and check there is an offset to fix
    u8 uPriorRightOffset = aPriorEntry->uRightOffset(uOffsetNo);
    if (C_NIL_OFFSET == uPriorRightOffset)
      continue;
    
    u8 uPriorEntryOffsetAbs = iToOffsetAbs(aPriorEntry);
    u8 uPriorEntryBytes = aPriorEntry->nSize();

    // check if newEntry is between the reviseEntry and postEntry
    u8 uPostOffsetAbs = uPriorEntryOffsetAbs + uPriorEntryBytes + uPriorRightOffset;
    if (uNewEntryOffsetAbs > uPostOffsetAbs)
      continue;
    
    // no of offsets of reviseVector entry is >= no of new entry offsets
    if (uOffsetNo >= nNewEntryOffsets) {
      // adjust the reviseVector offset with the newEntrySize
      u8 uPriorRightOffsetAdj = uPriorRightOffset + nNewEntryBytes;
      aPriorEntry->vRightOffset(uOffsetNo,uPriorRightOffsetAdj);
    }
    else {
      // no of offsets in prior entry is >= no of new entry offsets
      i8 nSize = uNewEntryOffsetAbs - (uPriorEntryBytes + uPriorEntryOffsetAbs);
      
      // adjust the prior entry to point to the new entry
      aPriorEntry->vRightOffset(uOffsetNo,uPriorRightOffset - nSize);
    }
  
  } // for
  
  //vTrace("REVISE VECTOR AFTER FIXING\n"_Str);
  //vDumpRevise(ppReviseVec);
  //vTrace("\n"_Str);
}

#else //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif

/*----------------------------------------------------------------------------*/
/* get the split entry                                                        */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pGetSplitEntry(i8 iSplitAtEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // start with the head
  Entry* aSplitEntry = aHead();
  
  i8 iEntries = 0;
  while (aSplitEntry) {
    
    iEntries++;
    
    // found split point
    if (iEntries == iSplitAtEntry)
      break;
    
    // get next entry
    aSplitEntry = this->aNextEntry(aSplitEntry);
  }
  
  return aSplitEntry;
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
  
  // bytes to copy
  i8 nBytesToCopy = nSub(m_xWsp.aLatter(), aSrc);
  
  // move the bytes left
  MemLib::vMove(aDst,aSrc,nBytesToCopy);
  
  // release space back to wsp
  //m_xWsp.vGive(nBytes);
  
  // for the moment we'll zero the bytes put back into the free area
  //void* aLatter = m_xWsp.aLatter();
  //MemLib::vZero(aLatter,nBytes);
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
  //ASSERT(m_xWsp.nFree() >= nBytes);
  
  // allot space for entry E in wsp
  //m_xWsp.vTake(nBytes);
  
  // compute the destination address
  void* aDst = pAdd<void*>(aSrc,nBytes);
  
  void* x = m_xWsp.aLatter();
  i8 y = nSub(m_xWsp.aLatter(), aSrc);
  // bytes to copy
  i8 nBytesToCopy = nSub(m_xWsp.aLatter(), aSrc); // - nBytes;
  
  // move F..Z right
  MemLib::vMove(aDst,aSrc,nBytesToCopy);
  
  // for the moment we'll zero the bytes of entry E
  MemLib::vZero(aSrc,nBytes);
}

/*----------------------------------------------------------------------------*/
/* allot new entry after given entry (return null if insufficient space)      */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::aAllotEntry(i8 nBytes, i8 nOffsets, Entry* aAfterEntry)
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
  
  // increment the no of entries
  m_nEntries++;
  
  return aNewEntry;
}

/*----------------------------------------------------------------------------*/
/* allot head entry (during constructor)                                      */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vAllotHead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);

  // must be the first item in wsp
  ASSERT( ! m_xWsp.nUsed() );
  ASSERT( ! m_nEntries );
  
  // how many bytes are required
  i8 nBytes = Entry::nBytesRequired(C_FLAT_OFFSETS, 0);
  
  // allot the head
  
  Entry* aHead = aAllotEntry(nBytes,C_FLAT_OFFSETS,nullptr);

  // initialize the head
  for (i8 iOffset = 0; iOffset < C_FLAT_OFFSETS; iOffset++)
    aHead->vRightOffset(iOffset, C_NIL_OFFSET);
}


/*----------------------------------------------------------------------------*/
/* scan thru skip list and positition to entry (optionally set revise vector) */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::aPosition(Key* aGivenKey, eJC eJuxtaCompare, Entry** aaReviseVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  /*
   * positioning is based on eJuxtaCompare
   *   JC_BEFORE: never called
   *   JC_SAME:   scan for entry which is the same as given key
   *   JC_AFTER:  scan for entry which the given key would be inserted after
   */

  ASSERT(JC_SAME == eJuxtaCompare || JC_AFTER == eJuxtaCompare);

  bool bIsJuxta;
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  // start scanning from head entry and topmost level
  Entry* aScanEntry = aHead();
  i8 iScanLevel = C_FLAT_LEVELS - 1;
  
  do {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inner loop: scan right a number of entries at current scan level
    //             until we find a key that is same/after given key
    
    while (Entry* aNextEntry = aRightEntry(aScanEntry, iScanLevel)) {
      
      // get next key after scan key
      Key* aNextKey = aNextEntry->aKey();

      // trace:
      //aGivenKey->vShow(m_pIndex->pIdxColVec());
      //aNextKey->vShow(m_pIndex->pIdxColVec());
    
      // juxta-compare given key with next key at this level
      bIsJuxta = aGivenKey->bJuxtaCompare(eJuxtaCompare, aNextKey, pIdxColVec);

      //vTrace(" %s\n"_Str,bAfter ? "after" : "before");
    
      // if given key is not same/after next key then break to outer loop
      if (! bIsJuxta)
        break;

      // given key is same/after next key, go right one entry at this level
      aScanEntry = aNextEntry;                          //! to discuss with Alka
      //aScanEntry = aRightEntry(aScanEntry, iScanLevel);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // outer loop: having gone right as far as we can, go down one level
    //             note that aNextEntry may be null at this point

    // opt record scan entry as an insert-after revision point at this level
    if (aaReviseVec)
      aaReviseVec[iScanLevel] = aScanEntry;
   
    // now go down one level
    iScanLevel--;
    
    // and repeat till we have scanned down thru all levels
  } while (iScanLevel >= 0);
  
  // JC_SAME : return the entry which is same as given key
  // JC_AFTER: return the entry which the given key would be inserted after
  return aScanEntry;
}

/*----------------------------------------------------------------------------*/
/* get right entry at given level (returns null if no right)                  */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::aRightEntry(Entry* aEntry, u8 uLevel)
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
  ASSERT(uLevel >= 0 && uLevel < C_FLAT_LEVELS);

  if (0 == uLevel) {
    // for level 0, the right entry is the next entry
    aRightEntry = aNextEntry(aEntry);
  }
  else {
    
    u8 uOffset = uLevel - 1;
    ASSERT(uOffset >= 0 && uOffset < aEntry->nOffsets());
    
    // for levels 1 & 2 the right entry is m_xWsp + rightOffset[iRight]
    u8 iBytes = aEntry->nSize();
    u8 uRightOffset = aEntry->uRightOffset(uOffset);
    if (C_NIL_OFFSET == uRightOffset)
      aRightEntry = nullptr;
    else
      aRightEntry = pAdd<Entry*>(aEntry,uRightOffset+iBytes);
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
/* calculates the absolute offset of aEntry in the workspace                  */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::iToOffsetAbs(Entry* aEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 iOffset = nSub(aEntry,m_xWsp.aArea());
  
  ASSERT(iOffset >= 0 && iOffset < m_xWsp.nSize());
  
  return iOffset;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
