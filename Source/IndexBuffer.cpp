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
/* get decile full of the buffer                                              */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::iDecile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 iDecile;
  
  if (C_SPACE_BASED_ON_ENTRIES)
    iDecile = Int::iDecile(m_nEntries, C_MAX_ENTRIES);
  else
    iDecile = Int::iDecile(m_xWsp.nUsed(), C_SIZE_POOL_BUFFER);
  
  return iDecile;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vDump(void)
{
  // show this buffer
  vShow();
  
  // recursively dump the lower level buffers
  if (m_nBufferLevel != 0) {
    
    for (Entry* pEntry = pNextEntry(pHead()); pEntry != nullptr;
         pEntry = pNextEntry(pEntry)) {
      
      // get the index buffer from the entry
      IndexBuffer* pIndexBuffer = pEntry->pIndexBuffer();;
      
      ASSERT(pIndexBuffer);
      ASSERT(pIndexBuffer->eClass() == CN_INDEXBUFFER);
      
      pIndexBuffer->vDump();
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* analyze                                                                    */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries)
{
  
  iBuffers++;
  
  // analyze this buffer
  vAnalyzeThis(sAnalysis, bDetailed, iBuffers, iEntries);
  
  // recursively dump the lower level buffers
  if (m_nBufferLevel != 0) {
  
    for (Entry* pEntry = pNextEntry(pHead()); pEntry != nullptr;
         pEntry = pNextEntry(pEntry)) {
      
      // get the index buffer from the entry
      IndexBuffer* pIndexBuffer = pEntry->pIndexBuffer();;
      
      ASSERT(pIndexBuffer);
      ASSERT(pIndexBuffer->eClass() == CN_INDEXBUFFER);
      
      pIndexBuffer->vAnalyze(sAnalysis, bDetailed, iBuffers, iEntries);
      
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* analyze                                                                    */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vAnalyzeThis(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries)
{

  iEntries += (m_nEntries - 1);
  
  if (bDetailed)
    sAnalysis.vCatSprint("     buffer %5lld (L%lld) : %lld entries\n"_Str,
                         iBuffers,
                         m_nBufferLevel,
                         m_nEntries - 1);
  
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
/* get first entry (after the head) of skip list in this index buffer         */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pFirstEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // the head entry will have been alloted during constructor
  ASSERT(m_xWsp.nUsed());
  
  // first get the head
  Entry* pHead = _pEntry(m_xWsp.aArea());
  
  // return the next entry
  return pNextEntry(pHead);
}

/*----------------------------------------------------------------------------*/
/* does the buffer have space for nBytes ?                                    */
/*----------------------------------------------------------------------------*/
bool IndexBuffer::bHasSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 iFree = m_xWsp.nFree();
 
  // free space calculation based on no of enteries
  if (C_SPACE_BASED_ON_ENTRIES) {
    if (m_nEntries > C_MAX_ENTRIES)
      return false;
   }
  else {
    if (iFree < nBytes)
      return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* get head entry of skip list in this index buffer                           */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pHead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // the head entry will have been alloted during constructor
  ASSERT(m_xWsp.nUsed());
  
  // the head entry is the first entry in wsp
  Entry* pHead = _pEntry(m_xWsp.aArea());
  
  return pHead;
}

/*----------------------------------------------------------------------------*/
/* get the higher level index buffer                                          */
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

/*----------------------------------------------------------------------------*/
/* insert new entry into this index buffer                                    */
/*----------------------------------------------------------------------------*/
bool IndexBuffer::bInsert(EntWsp& xEntWsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  //vTrace("Inserting key:\n"_Str);
  //xKeyWsp.vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert a new entry into the skip list
  
  // find the entry after which the new entry.key will be inserted
  EntryVec xPriorVec;
  Entry* pAfterEntry = pPosition(xEntWsp.aKey(), JC_AFTER, &xPriorVec);
  
  // create a new entry after the prior entry and set it
  //EntWsp xEntWsp(xId,xKeyWsp);
  i8 nEntryBytes = Entry::nBytesRequired(xEntWsp);
  Entry* pNewEntry = pAllotEntry(nEntryBytes, xEntWsp.nOffsets(), pAfterEntry);
  pNewEntry->vSetEntry(xEntWsp);
  
  //vTrace("\n\n---------INDEX BUFFER AFTER MOVING --------------\n"_Str);
  //vShow();
  //vTrace("-----------------------\n\n"_Str);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // revise the offsets of the prior entries from the prior vector
  
  // fix the prior vector after the move
  vFixPriorOffset(&xPriorVec, pNewEntry);
 
  // get the absolute offset of the new entry
  i8 iNewEntryOffsetAbs = iToOffsetAbs(pNewEntry);
  
  // step thru each offset to be revised
  for (on nOffsetNo = pNewEntry->nHighestOffset(); nOffsetNo.bInRange(); nOffsetNo--) {
    
    // get prior entry being revised
    ln nLevelNo = nOffsetNo.nToLevelNo();
    Entry* aPriorEntry = xPriorVec[nLevelNo];

    i8 iPriorsRightOffsetAbs = iToRightOffsetAbs(aPriorEntry,nOffsetNo);
    
    // is the prior's right offset after new entry : [PURPLE] (grey)-> [GREEN]
    if (iPriorsRightOffsetAbs > iNewEntryOffsetAbs) {
      
      // get post entry : [GREEN]
      Entry* aPostEntry = pRightEntry(aPriorEntry,nLevelNo);
      
      // calculate new's right offset : [RED ARROW]
      i8 iNewRightOffset = uOffsetBetween(pNewEntry,aPostEntry);
      
      // set right offset on new : [RED.hasoffset]
      pNewEntry->vRightOffset(nOffsetNo,iNewRightOffset);

      // calculate prior's revised offset : [PURPLE ARROW]
      i8 iPriorRightOffset = uOffsetBetween(aPriorEntry,pNewEntry);
      
      aPriorEntry->vRightOffset(nOffsetNo,iPriorRightOffset);
    }
    else {
      
      // this is the case where the prior entry now needs to point to the new
      // prior was at C_NIL_OFFSET and now needs to point to the newEntry
      ASSERT(aPriorEntry->iRightOffset(nOffsetNo) == C_NIL_OFFSET);
      ASSERT(pNewEntry->iRightOffset(nOffsetNo) == C_NIL_OFFSET);
      
      // calculate new right offset
      i8 iNewRightOffset = uOffsetBetween(pNewEntry,aPriorEntry);
      
      // set the right offset in the prior entry to offset of new entry
      aPriorEntry->vRightOffset(nOffsetNo, iNewRightOffset);
    }
    
  } // for
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fix higher level buffers
  // this is needed in case we inserted a new first key in the buffer
  //vShow();
  vFixHigherLevel(pNewEntry);
  
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
Entry* IndexBuffer::pNextEntry(Entry* pEntry)
{
  // get the size of given entry
  i8 nBytes = pEntry->nSize();
  
  // compute the address of possible next entry
  Entry* pNext = pAdd<Entry*>(pEntry,nBytes);
  
  // check for valid entry
  Entry* pLatter = _pEntry(m_xWsp.aLatter());
  if (pNext < pLatter)
    return pNext;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* position at entry with lower bound at key                                  */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pPosition(KeyWsp& xKeyWsp, eJC eJuxtaCompare)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  Entry* pEntry;
  
  pEntry = pPosition(xKeyWsp.aKey(), eJuxtaCompare, nullptr);
  
  if (pEntry == pHead())
    pEntry = pNextEntry(pEntry);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* position at an index buffer at level iLevel starting from this buffer      */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexBuffer::pPositionLevel(KeyWsp& xKeyWsp, eJC eJuxtaCompare, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  IndexBuffer* pIndexBuffer = this;
  
  i8 iBufferLevel;
  Entry* pEntry;
  
  while (true) {
    iBufferLevel = pIndexBuffer->nBufferLevel();
    
    if (iBufferLevel == iLevel)
      break;
    
    // get the entry
    pEntry = pIndexBuffer->pPosition(xKeyWsp, eJuxtaCompare);
    ASSERT(pEntry);
    
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
  s256 sHigherLevel;
  if (m_pHigherLevel)
    StrLib::wToString(m_pHigherLevel->m_nBufferLevel, sHigherLevel.s());
  else
    sHigherLevel = "..."_Str;
  
  vTrace("IndexBuffer %p (buffer %lld): level L%lld HigerLevel %p (L%s) "
         "entries %lld prevBuffer %p nextBuffer %p \n"_Str,
         this,
         *nBufferNo(),
         m_nBufferLevel,
         m_pHigherLevel,
         sHigherLevel.aStr(),
         m_nEntries,
         m_pPrevBuffer,
         m_pNextBuffer);
  
  for (Entry* pEntry = pHead(); pEntry != nullptr; pEntry = pNextEntry(pEntry)) {
    vTrace("%8lld: "_Str, nSub(pEntry, m_xWsp.aArea()));
    pEntry->vShow(iToOffsetAbs(pEntry), m_pIndex->pIdxColVec());
  }
  
  vTrace("\n"_Str);
  
}

/*----------------------------------------------------------------------------*/
/* split this buffer into two: this original buffer plus a new buffer         */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexBuffer::pSplitBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  EntryVec xPriorVec;
  
  // get split entry to remain in this buffer and
  // move entry (and those that follow) which will be moved into new buffer
  i8 iSplitAtEntry   = m_nEntries/2;
  Entry* pSplitEntry = pGetEntry(iSplitAtEntry);
  Entry* aMoveEntry  = pNextEntry(pSplitEntry);
  //aSplitEntry->vShow(0,m_pIndex->pIdxColVec());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set offsets to entries after the split entry in this buffer to nil
  
  // get vector of prior entries which have offsets to the split entry
  // show this buffer
  //vShow();
  
  Key* aSplitKey= pSplitEntry->aKey();
  Entry* pCrossCheck = pPosition(aSplitKey, JC_AFTER, &xPriorVec);
  
  // Note this ASSERT fails if we have dulplicates in the table     //ALKA: TODO
  // Need to fix when we cater for non-unique indexes
  //aCrossCheck->vShow(0,m_pIndex->pIdxColVec());
  ASSERT(pCrossCheck == pSplitEntry);
  
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  
  EntryVec xPrevVec;
  vPriorVec(aMoveEntry,&xPrevVec);

//  vTrace("Split %p\n"_Str,pSplitEntry);
//  vTrace("Move  %p\n"_Str,aMoveEntry);
//  for (on nOffsetNo; nOffsetNo.bInRange(); nOffsetNo++) {
//    ln nLevel = nOffsetNo.nToLevelNo();
//    vTrace("%p v %p\n"_Str,xPrevVec[nLevel],xPriorVec[nLevel]);
//    ASSERT(xPrevVec[nLevel] == xPriorVec[nLevel]);
//  }
  
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  
  // set any prior offset to nil if its an offset to an entry in split half
  i8 iSplitEntryOffsetAbs = iToOffsetAbs(pSplitEntry);
  
  for (on nOffsetNo; nOffsetNo.bInRange(); nOffsetNo++) {
    
    // calculate level into prior vector for this offset and get entry
    ln nLevel = nOffsetNo.nToLevelNo();
    Entry* pEntry = xPriorVec[nLevel];
    
    if (iToRightOffsetAbs(pEntry,nOffsetNo) > iSplitEntryOffsetAbs)
      pEntry->vRightOffset(nOffsetNo, C_NIL_OFFSET);
  }
  
  // set all right offsets in the split entry to C_NIL_OFFSET
  for (on nOffsetNo = pSplitEntry->nHighestOffset(); nOffsetNo.bInRange(); nOffsetNo--)
    pSplitEntry->vRightOffset(nOffsetNo, C_NIL_OFFSET);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create new buffer and move the entries into it
  
  // create new buffer at the same index level and link the two buffers
  IndexBuffer* pNewBuffer = new IndexBuffer(m_pIndex, m_nBufferLevel);
  pNewBuffer->m_pNextBuffer = pNextBuffer();
  pNewBuffer->m_pPrevBuffer = this;
  m_pNextBuffer = pNewBuffer;
  
  // copy entries from this buffer starting at splitEntry to the new buffer
  m_nEntries -= pNewBuffer->iCopyEntries(this, aMoveEntry);
  
  // give copied bytes back to wsp of this original buffer
  i8 nBytesToGive = nSub(m_xWsp.aLatter(), aMoveEntry);
  m_xWsp.vGive(nBytesToGive);
  ASSERT(m_xWsp.aLatter() == pAdd<void*>((void*)pSplitEntry,pSplitEntry->nSize()));
  
  return pNewBuffer;
}

/*----------------------------------------------------------------------------*/
/* verify                                                                     */
/*----------------------------------------------------------------------------*/
bool IndexBuffer::bVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   
   Entries in an index buffer
   
    EntryA                                                         EntryH
    -------                                                        -------
   |       | ---------------------------------------------------->|       |
L2 |       |                                                      |       |
    -------                    EntryD                              -------
    -------                    -------                             -------
   |       |                  |       |                           |       |
L1 |       |----------------->|       |-------------------------->|       |
    -------  EntryB    EntryC  -------   EntryE   EntryF  EntryG   -------
    -------  -------  -------  -------  -------  -------  -------  -------
   |       ||       ||       ||       ||       ||       ||       ||       |
L0 |  KeyA || KeyB  || KeyC  ||  KeyD || KeyE  || KeyF  || KeyG  || KeyH  |
    -------  -------  -------  -------  -------  -------  -------  -------
   
   
   We start at L2
   EntryOne is EntryA
   EntryTwo is EntryH
   
   keyH is after keyA
   
   We reach the end of Level2
   
   Move to L1
   EntryOne is EntryA
   EntryTwo is EntryD
   keyD is after jeyA
   
   EntryOne becomes EntryD
   EntryTwo is EntryH
   KeyH is after keyD
   
   We reach the end of Level1
   
   Move to L0
   EntryOne is EntryA
   EntryTwo is EntryB
   KeyB is after KeyA
   ....
   
   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  
  ln nLevelNo;
  
  // start at the highest level in the index buffer's skip list (L2)
  nLevelNo = C_FLAT_LEVELS - 1;
  
  do {
    
    // get the head entry of the buffer
    Entry* pEntryOne = pHead();
    if ( ! pEntryOne )
      return true;
    
    // If we are scanning at a level greater than the level of the entry
    // just move down one level
    if (nLevelNo > (i8)(pEntryOne->nOffsets())) {
      nLevelNo--;
      continue;
    }
    
    // we have a right entry at this level entryA
    // get the right entry
    Entry* pEntryTwo = pRightEntry(pEntryOne, nLevelNo);
    
    // if no right entry at this level, move down one level
    if ( ! pEntryTwo ) {
      nLevelNo--;
      continue;
    }
  
    // iterate through the entries at this level
    while (pEntryTwo) {
      
      // get keys of the two entries
      Key* pKeyA = pEntryOne->aKey();
      Key* pKeyB = pEntryTwo->aKey();
      
      // if entryA is not the head, compare the keys
      if (pEntryOne != pHead()) {
        
        // KeyB should should be after keyA
        bool bAfter = pKeyB->bJuxtaAfter(pKeyA, m_pIndex->pIdxColVec());
        
        // if keyB is not after keyA , we have a problem
        if ( ! bAfter )
          return false;
      }
      
      // get next entry
      pEntryOne = pEntryTwo;
      pEntryTwo = pNextEntry(pEntryOne);
    }
    
    // move down one level and start with the head again
    nLevelNo--;
    
  } while (nLevelNo >= 0_ln );
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* adjust the head and first entry offsets                                    */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vAdjustOffsets(Entry* pEntry, i8 nAdjustAmount)
{

  for (on nOffsetNo; nOffsetNo < pEntry->nOffsets(); nOffsetNo++) {
    
    //
    i8 iRightOffset = pEntry->iRightOffset(nOffsetNo);
    
    // here we adjust the iRightOffset if iRightOffset is > 0
    if (iRightOffset > 0) {
      i8 iNewRightOffset = iRightOffset + nAdjustAmount;
      pEntry->vRightOffset(nOffsetNo,iNewRightOffset);
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* allot new entry after given entry (return null if insufficient space)      */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pAllotEntry(i8 nBytes, i8 nOffsets, Entry* aAfterEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // There should be  enough space for the new entry
  if (m_xWsp.nFree() < nBytes)
    return nullptr;
  
  // allot space for the new entry
  m_xWsp.vTake(nBytes);
  Entry* pLatter = _pEntry(m_xWsp.aLatter());
  
  Entry* pNewEntry;
  
  if (aAfterEntry) {
    // compute address of new entry
    i8 nAfterBytes = aAfterEntry->nSize();
    pNewEntry = pAdd<Entry*>(aAfterEntry,nAfterBytes);
    
    // move subset of entries to right to make room for new entry
    vMoveRight(pNewEntry,nBytes);
  }
  else {
    pNewEntry = _pEntry(m_xWsp.aArea());
  }
  
  // run the constructor of the new entry
  new (pNewEntry) Entry(nBytes,nOffsets);
  
  // increment the no of entries
  m_nEntries++;
  
  return pNewEntry;
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
  
  Entry* pHead = pAllotEntry(nBytes,C_FLAT_OFFSETS,nullptr);
  
  // initialize the head
  for (on nOffsetNo; nOffsetNo.bInRange(); nOffsetNo++)
    pHead->vRightOffset(nOffsetNo, C_NIL_OFFSET);
}

/*----------------------------------------------------------------------------*/
/* copy entries from fromBuffer starting at fromEntry to this buffer          */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::iCopyEntries(IndexBuffer* pFromBuffer, Entry* aFromEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 nBufferLevel = pFromBuffer->nBufferLevel();
  
  i8 nCopied = 0;
  for (Entry* pEntry = aFromEntry;
       pEntry;
       pEntry = pFromBuffer->pNextEntry(pEntry)) {
    
    // form keywsp from the entry
    KeyWsp xKeyWsp(pEntry->aKey(), m_pIndex->pIdxColVec());
    
    // form the entwsp from the entry
    EntWsp xEntWsp(pEntry->xId(), xKeyWsp);
    
    // insert entry into this buffer
    bInsert(xEntWsp);
    
    // if this buffer is a higher level buffer,
    // set this buffer as the higher level buffer
    // for the buffer the entry is pointing to
    if (nBufferLevel > 0) {
      
      /*----------------------------------------------------------------------//
       
      We are splitting a LN buffer BN at entry kl
       
       BN
      --------------------
      |ka kj ..kl kp...   |  Level LN buffer
      --------------------
      ^
      |
      BM
      -------------
      |ka kb   ...|  Level LN-1 buffer
      -------------

       At this point if LN is not L0 we have a buffer BM pointing to BN.

       We are copying entries kl, kp ... from buffer BN to BN+1.
       
       BN             BN+1
      -------------    -------------
      |ka kj   ... |-->| kl kp...   |  Level LN buffer
      -------------    -------------
      ^                ^
      |                |
      |                |
      |                |
      BM              BM+1
      -------------   -------------
      |ka kb   ...|-->| kj kk  ...|   Level LN-1 buffer
      -------------   -------------
       
       Now we need to make buffer BM+1 have its higher level buffer point to BN
       
      //----------------------------------------------------------------------*/

      // get the lower level buffer the entry is pointing to
      IndexBuffer* pIndexBuffer = pEntry->pIndexBuffer();
      
      // this index buffer should be at a level lower than nBufferLevel
      ASSERT(pIndexBuffer->nBufferLevel() < nBufferLevel);
      
      // this lower level buffers higher level is this buffer
      pIndexBuffer->m_pHigherLevel = this;
    }
    
    nCopied++;
  }
  
  return nCopied;
}

/*----------------------------------------------------------------------------*/
/* dump the prior vector                                                      */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vDumpPrior(Entry* xPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  for (on nOffsetNo; nOffsetNo.bInRange(); nOffsetNo++) {
    i8 iLevel = *nOffsetNo + 1;
    i8 iReviseOffset = xPriorVec[iLevel]->iRightOffset(nOffsetNo);
    vTrace("%p xPriorVec[%lld]->uRightOffset(%lld) = %lld\n"_Str,
           xPriorVec[iLevel], iLevel, *nOffsetNo, iReviseOffset);
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
  
  // get the first entry right of head at level 0
  Entry* aFirstEntry = pRightEntry(pHead(), ln::nZeroLevel());
  
  // if the entry we just added is not the first entry , nothing to fix
  if (aNewEntry != aFirstEntry)
    return;
  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  // we need to update the higher level buffer entry key
  Entry* aOldFirstEntry = pNextEntry(aNewEntry);

  KeyWsp xOldKeyWsp;
  xOldKeyWsp.vAddKey(aOldFirstEntry->aKey(), m_pIndex->pIdxColVec());
  
  // create akeyWsp with the newEntry key
  KeyWsp xNewKeyWsp;
  xNewKeyWsp.vAddKey(aNewEntry->aKey(), m_pIndex->pIdxColVec());
  
  IndexBuffer* pHigherLevel = m_pHigherLevel;
  
  while (pHigherLevel) {
    //pHigherLevel->vShow();
    //xOldKeyWsp.vShow();
    //xNewKeyWsp.vShow();
    pHigherLevel->vReplace(xOldKeyWsp, xNewKeyWsp);
    //pHigherLevel->vShow();
    //ASSERT(pHigherLevel->bVerify());
    pHigherLevel = pHigherLevel->m_pHigherLevel;
  }
}

/*----------------------------------------------------------------------------*/
/* replace old key with new key in the index buffer                           */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vReplace(KeyWsp& xOldKeyWsp, KeyWsp& xNewKeyWsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);

  // get the size of the old key
  u8 uOldKeySize = xOldKeyWsp.nWspUsed();

  // get the size of the new key
  u8 uNewKeySize = xNewKeyWsp.nWspUsed();

  if (uOldKeySize == uNewKeySize) {
    vReplaceEqualKey(xOldKeyWsp, xNewKeyWsp);
    return;
  }
  
  if (uOldKeySize > uNewKeySize) {
    vReplaceSmallereKey(xOldKeyWsp, xNewKeyWsp);
    return;
  }
  
  ASSERT(uOldKeySize < uNewKeySize);
  vReplaceLargerKey(xOldKeyWsp, xNewKeyWsp);
}

/*----------------------------------------------------------------------------*/
/* replace old key with new key of same size in the index buffer              */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vReplaceEqualKey(KeyWsp& xOldKeyWsp, KeyWsp& xNewKeyWsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   The old and new key are the same size, just change the key in the entry
   
   -------------    -------------
   |Ka(m bytes) |   |Kc(m bytes) |
   -------------    -------------
   
   We keep the same offsets etc. and just replace the key in the entry
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  // find the oldEntry in the higher level buffer
  Entry* pOldEntry = pPosition(xOldKeyWsp, JC_SAME);
  
  // assert that we found one
  ASSERT(pOldEntry);
  
  // assert the key of the higher level entry and the old entry match
  ASSERT(pOldEntry->aKey()->bJuxtaCompare(JC_SAME, xOldKeyWsp.aKey(), m_pIndex->pIdxColVec()));
  
  EntWsp xEntWsp(pOldEntry);
  xEntWsp.vAddKey(xNewKeyWsp);
  
  // just replace the key in the buffer
  pOldEntry->vKey(xEntWsp);
}

/*----------------------------------------------------------------------------*/
/* replace old key with new key in the index buffer                           */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vReplaceLargerKey(KeyWsp& xOldKeyWsp, KeyWsp& xNewKeyWsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   The new key is longer than the old key
   
   ---------------     --------------------
   |Ka(m bytes)    |   |Kc(m+n bytes)       |
   ---------------     --------------------
   
   We will moveRight in the buffer making space for n bytes
   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  // find the oldEntry in the higher level buffer
  Entry* pOldEntry = pPosition(xOldKeyWsp, JC_SAME);
  
  // assert that we found the old key
  ASSERT(pOldEntry);
  
  // assert the key of the higher level entry and the old entry match
  ASSERT(pOldEntry->aKey()->bJuxtaCompare(JC_SAME, xOldKeyWsp.aKey(), m_pIndex->pIdxColVec()));
  
  // create a newEntry at the same location as the pOldEntry
  Entry* pNewEntry = _pEntry(pOldEntry);
  
  // get the size of the old key
  u8 uOldKeySize = xOldKeyWsp.nWspUsed();
  
  // get the size of the new key
  u8 uNewKeySize = xNewKeyWsp.nWspUsed();
  
  // the old key is smaller than the new key
  i8 nSizeDifference = uNewKeySize - uOldKeySize;
  ASSERT(nSizeDifference > 0);
  

  // ensure we have space in the buffer
  if (m_xWsp.nFree() < nSizeDifference) {
    ASSERT(0);                                                   //ALKA: TODO
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create an entwsp from the old entry, with offsets et all
  EntWsp xEntWsp(pOldEntry);
  xEntWsp.vAddKey(xNewKeyWsp);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // we need to move right nSizeDifference bytes, making room for the bigger key
  
  // calculate the pDst address
  i8 iOldEntrySize = pOldEntry->nSize();
  void* pDst = pAdd<void*>(pOldEntry, iOldEntrySize);
  
  ASSERT(m_xWsp.nFree() >= nSizeDifference);
  
  // move right
  m_xWsp.vTake(nSizeDifference);
  vMoveRight(pDst, nSizeDifference);
  
  // calculate size of new entry
  i8 nBytes =  iOldEntrySize + nSizeDifference;
  
  // create a new entry at pNewEntry using EntWsp
  new (pNewEntry) Entry(nBytes, xEntWsp.nOffsets());
  pNewEntry->vSetEntry(xEntWsp);
  ASSERT(pNewEntry->nSize() == nBytes);
  
  //pOldEntry->vShow(pNewEntry->nOffsets(), m_pIndex->pIdxColVec());
  //pNewEntry->vShow(pNewEntry->nOffsets(), m_pIndex->pIdxColVec());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // we have replaced the first entry in this buffer
  // hence we need to fix the right offsets in the head and the first entry
  vAdjustOffsets(pHead(), nSizeDifference);
  vAdjustOffsets(pNextEntry(pHead()), nSizeDifference);
}

/*----------------------------------------------------------------------------*/
/* replace old key with new key in the index buffer                           */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vReplaceSmallereKey(KeyWsp& xOldKeyWsp, KeyWsp& xNewKeyWsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  /*
   The new key is shorter than the old key
   
   old key             new key
   ----------------     ------------
   |Ka(m bytes)    |   |Kc(m-nbytes)|
   ----------------    ------------
   
   We will moveLeft in the buffer reclaiming n bytes
   */
  
  // find the oldEntry in the higher level buffer
  Entry* pOldEntry = pPosition(xOldKeyWsp, JC_SAME);
  
  // assert that we found one
  ASSERT(pOldEntry);
  
  // assert the key of the higher level entry and the old entry match
  ASSERT(pOldEntry->aKey()->bJuxtaCompare(JC_SAME, xOldKeyWsp.aKey(), m_pIndex->pIdxColVec()));
  
  // create a newEntry at the same location as the foundEntry
  Entry* pNewEntry = _pEntry(pOldEntry);
  
  // get the size of the old key
  u8 uOldKeySize = xOldKeyWsp.nWspUsed();
  
  // get the size of the new key
  u8 uNewKeySize = xNewKeyWsp.nWspUsed();
  
  i8 iSize = pOldEntry->nSize();
  
  // the old key is smaller than the new key
  i8 nSizeDifference = uOldKeySize - uNewKeySize;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create an entwsp from the old entry, with offsets et all
  EntWsp xEntWsp(pOldEntry);
  xEntWsp.vAddKey(xNewKeyWsp);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // we need to move left nSizeDifference bytes, making room for the bigger key
  
  // calculate the pDst address
  void* pDst = pAdd<void*>(pOldEntry, uNewKeySize);
  
  // move left
  vMoveLeft(pDst, nSizeDifference);
  m_xWsp.vGive(nSizeDifference);
  
  // calculate size of new entry
  i8 nBytes =  iSize - (uOldKeySize - uNewKeySize);
  
  // create a new entry at pNewEntry using EntWsp
  new (pNewEntry) Entry(nBytes, xEntWsp.nOffsets());
  pNewEntry->vSetEntry(xEntWsp);
  ASSERT(pNewEntry->nSize() == nBytes);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // we have replaced the first entry in this buffer
  // hence we need to fix the right offsets in the head
  nSizeDifference = uNewKeySize - uOldKeySize;
  ASSERT( nSizeDifference < 0 );
  vAdjustOffsets(pHead(), nSizeDifference);
  vAdjustOffsets(pNextEntry(pHead()), nSizeDifference);
}

/*----------------------------------------------------------------------------*/
/* fix offsets of the entries in the prior vector to account for new entry    */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vFixPriorOffset(EntryVec* pPriorVec, Entry* aNewEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  //vTrace("PRIOR VECTOR BEFORE FIXING\n"_Str);
  //vDumpPrior(xPriorVec);
  //vTrace("\n"_Str);
  
  // get entry size
  i8 nNewEntryBytes = aNewEntry->nSize();
  
  // iterate thru each possible offset
  for (on nOffsetNo; nOffsetNo.bInRange(); nOffsetNo++) {
    
    //on nOffsetNo;
    //nOffsetNo= uOffsetNo;
    
    // get prior entry being fixed
    ln nLevelNo = nOffsetNo.nToLevelNo();
    Entry* aPriorEntry = (*pPriorVec)[nLevelNo];
    
    // compute and set corrected offset
    i8 iPriorsRightOffset = aPriorEntry->iRightOffset(nOffsetNo);
    
    // ignore cases where no right offset
    if (C_NIL_OFFSET == iPriorsRightOffset)
      continue;
    
    // correct the right offset by adding in the size of the new entry
    i8 iCorrectedRightOffset = iPriorsRightOffset + nNewEntryBytes;
    aPriorEntry->vRightOffset(nOffsetNo,iCorrectedRightOffset);
    
  } // for
  
  //vTrace("PRIOR VECTOR AFTER FIXING\n"_Str);
  //vDumpPrior(xPriorVec);
  //vTrace("\n"_Str);
}

/*----------------------------------------------------------------------------*/
/* get an entry by number                                                     */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pGetEntry(i8 iEntryNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // start with the head
  Entry* pSplitEntry = pHead();
  
  i8 iEntries = 0;
  while (pSplitEntry) {
    
    iEntries++;
    
    // found split point
    if (iEntries == iEntryNo)
      break;
    
    // get next entry
    pSplitEntry = this->pNextEntry(pSplitEntry);
  }
  
  return pSplitEntry;
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
  i8 nBytesToCopy = nSub(m_xWsp.aLatter(), aSrc) - nBytes;
  
  // move F..Z right
  MemLib::vMove(aDst,aSrc,nBytesToCopy);
  
  // for the moment we'll zero the bytes of entry E
  MemLib::vZero(aSrc,nBytes);
}


/*----------------------------------------------------------------------------*/
/* scan thru skip list and positition to entry (optionally set prior vector)  */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pPosition(Key* aGivenKey, eJC eJuxtaCompare, EntryVec* pPriorVec)
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
  ln nScanLevel;
  nScanLevel = C_FLAT_LEVELS - 1;
  
  //aGivenKey->vShow(m_pIndex->pIdxColVec());
  //vTrace("\n"_Str);
  //vShow();
  
  Entry* pScanEntry_sl = pHead();
  Entry* pNextEntry_sl = nullptr;
  //Entry* pMoreEntry_sl = nullptr;
  
  do {

    pNextEntry_sl = pRightEntry(pScanEntry_sl,nScanLevel);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inner loop: scan right a number of entries at current scan level
    //             until we find a key that is same/after given key
    
    
    while (pNextEntry_sl) {
      
      // get next key after scan key
      Key* aNextKey = pNextEntry_sl->aKey();

      // trace:
      //aGivenKey->vShow(m_pIndex->pIdxColVec());
      //aNextKey->vShow(m_pIndex->pIdxColVec());
    
      // juxta-compare given key with next key at this level
      bIsJuxta = aGivenKey->bJuxtaCompare(eJuxtaCompare,aNextKey,pIdxColVec);

      //vTrace(" %s\n"_Str,bAfter ? "after" : "before");
    
      // if given key is not same/after next key then break to outer loop
      if (! bIsJuxta)
        break;
      
      pScanEntry_sl = pNextEntry_sl;
      pNextEntry_sl = pRightEntry(pNextEntry_sl,nScanLevel);;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // outer loop: having gone right as far as we can, go down one level
    //             note that aNextEntry may be null at this point

    // opt record scan entry as an insert-after revision point at this level
    if (pPriorVec)
      (*pPriorVec)[nScanLevel] = pScanEntry_sl;
   
    // now go down one level
    nScanLevel--;
    

    // and repeat till we have scanned down thru all levels
  } while (*nScanLevel >= 0);
  
  // JC_SAME : return the entry which is same as given key
  // JC_AFTER: return the entry which the given key would be inserted after
  return pScanEntry_sl;
}

/*----------------------------------------------------------------------------*/
/* get the prior vector of given entry                                        */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vPriorVec(Entry* pEntry, EntryVec* pPriorVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  ln nLevel;
  
  // set all entries to null
  Entry* pHeadEntry = pHead();
  for (nLevel = 0_on; *nLevel < C_FLAT_LEVELS; nLevel++)
    (*pPriorVec)[nLevel] = nullptr;
  
  // scan till we find the entry
  for (Entry* pScan = pHead(); pScan; pScan = pNextEntry(pScan)) {
    
    if (pScan == pEntry)
      return;
    
    for (on nOffset; *nOffset < pScan->nOffsets(); nOffset++) {
      nLevel = nOffset.nToLevelNo();
      if (C_NIL_OFFSET == pScan->iRightOffset(nOffset) ||
          pRightEntry(pScan,nLevel) >= pEntry)
        (*pPriorVec)[nLevel] = pScan;
    }
    
  }
  
  ASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* get right entry at given level (returns null if no right)                  */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pRightEntry(Entry* pEntry, ln nLevelNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  Entry* pRightEntry;
  
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
  ASSERT(nLevelNo >= 0_ln && *nLevelNo < C_FLAT_LEVELS);

  if (nLevelNo == 0_ln) {
    // for level 0, the right entry is the next entry
    pRightEntry = pNextEntry(pEntry);
  }
  else {
    
    on nOffsetNo = nLevelNo.nToOffsetNo();
    ASSERT(nOffsetNo >= 0_on && *nOffsetNo < pEntry->nOffsets());
    
    // for levels 1 & 2 the right entry is m_xWsp + rightOffset[iRight]
    i8 iBytes = pEntry->nSize();
    i8 iRightOffset = pEntry->iRightOffset(nOffsetNo);
    if (C_NIL_OFFSET == iRightOffset)
      pRightEntry = nullptr;
    else
      pRightEntry = pAdd<Entry*>(pEntry,iRightOffset+iBytes);
  }
 
  ASSERT( ! pRightEntry ||
          ( pRightEntry > pHead() &&
            pRightEntry < _pEntry(m_xWsp.aLatter()) ) );
  
  return pRightEntry;
}

/*----------------------------------------------------------------------------*/
/* converts given entry to absolute offset to end of entry                    */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::iToEndOffsetAbs(Entry* pEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 iOffset = nSub(pEntry,m_xWsp.aArea());
  
  ASSERT(iOffset >= 0 && iOffset < m_xWsp.nSize());
  
  return iOffset + pEntry->nSize();
}

/*----------------------------------------------------------------------------*/
/* calculate the absolute offset within workspace for given entry             */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::iToOffsetAbs(Entry* pEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 iOffset = nSub(pEntry,m_xWsp.aArea());
  
  ASSERT(iOffset >= 0 && iOffset < m_xWsp.nSize());
  
  return iOffset;
}

/*----------------------------------------------------------------------------*/
/* calculate the absolute right offset for a given entry and offset no        */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::iToRightOffsetAbs(Entry* pEntry, on nOffsetNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 iEntryOffsetAbs = iToOffsetAbs(pEntry);
  i8 iRightOffset = pEntry->iRightOffset(nOffsetNo);
  i8 iEntrySize = pEntry->nSize();
  
  if (iRightOffset == C_NIL_OFFSET)
    iRightOffset = 0;
  
  i8 iRightOffsetAbs = iEntryOffsetAbs + iRightOffset + iEntrySize;
  
  return iRightOffsetAbs;
}

/*----------------------------------------------------------------------------*/
/* calculate the relative offset between two entries                          */
/*----------------------------------------------------------------------------*/
u8 IndexBuffer::uOffsetBetween(Entry* pEntryA, Entry* pEntryB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  u8 uOffset = 0;
  
  u8 uEntryOffsetA = iToOffsetAbs(pEntryA);
  u8 uEntryEndOffsetA = iToEndOffsetAbs(pEntryA);
  
  u8 uEntryOffsetB = iToOffsetAbs(pEntryB);
  u8 uEntryEndOffsetB = iToEndOffsetAbs(pEntryB);
  
  if (uEntryOffsetA < uEntryOffsetB)
    uOffset = uEntryOffsetB - uEntryEndOffsetA;
  else
    uOffset = uEntryOffsetA - uEntryEndOffsetB;
  
  return uOffset;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
