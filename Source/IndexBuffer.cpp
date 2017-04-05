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
IndexBuffer::IndexBuffer(void)
: Buffer(CN_INDEXBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);

  m_nEntries    = 0;
  m_pPrevBuffer = nullptr;
  m_pNextBuffer = nullptr;
  
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
/* index buffer factory                                                       */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexBuffer::pNewIndexBuffer(void)
{
  // should we be testing low buffers for index?
  if (gpBufferPool->iFreeCount() <= C_BUFFER_THRESHOLD)
    return nullptr;
  
  IndexBuffer* pIndexBuffer = new IndexBuffer;
  
  return pIndexBuffer;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* insert new entry                                                           */
/*----------------------------------------------------------------------------*/
bool IndexBuffer::bInsert(Key* aKey, RowId xRowId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  Entry* pReviseVec[C_FLAT_LEVELS];
  bool   bEqualTo;
  EntWsp xEntWsp;
  
  vTrace("\n\n---------INDEX BUFFER--------------\n"_Str);
  vShow();
  vTrace("-----------------------\n\n"_Str);
  
  // initialize the EntWsp
  xEntWsp.vRowId(xRowId);
  xEntWsp.vAddKey(aKey);
  xEntWsp.vShow();
  
  // get the size of the EntWsp
  i8 nBytes = Entry::nBytesRequired(xEntWsp);
  
  // get a positioned revise vector
  Entry* pAfterEntry = aPosition(aKey, bEqualTo, pReviseVec);
  Entry* pNewEntry = aNewEntry(nBytes, xEntWsp.nOffsets(), pAfterEntry);
  
  // set the new entry from the EntWsp
  pNewEntry->vSetEntry(xEntWsp);
  
  // get the offset of the entry
  i8 nEntryOffsets = pNewEntry->nOffsets();
  i8 iNewOffset = iToOffset(pNewEntry);
  
  i8 iOffset = nEntryOffsets;
  
  // put new entry into the index buffer according to the revise vector for iEntryOffset
  while (--iOffset >= 0) {
    
    i8 iRightOffset = pReviseVec[iOffset+1]->uRightOffset(iOffset);
    pNewEntry->vRightOffset(iOffset, iRightOffset);
    pReviseVec[iOffset+1]->vRightOffset(iOffset, iNewOffset);
    
  }

  pNewEntry->vShow();
  
  // increment the no of entries
  m_nEntries++;
  
  return true;
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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
  
  // check there is enough space for the new entry
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
/* positition to entry (optional set revise vector)                           */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::aPosition(Key* aKey, bool& bEqualTo, Entry** aReviseVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  aKey->vShow();
  
  // start from head at known highest level
  Entry* pCurrentEntry = aHead();
  i8 iCurrentLevel = C_FLAT_LEVELS - 1;
  
  do {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inner loop: go right a number of nodes until we find
    //   ObjectInsert < ObjectNext at current level
    while (Entry* pNextEntry = aRightEntry(pCurrentEntry, iCurrentLevel)) {
      
        // compare the inserting object with next object
        bool bGreaterEqual = bI8Compare(aKey, pNextEntry->aKey(), bEqualTo);
  
      vTrace("nextKey:  "_Str);
      pNextEntry->vShow();
      
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
    i8 iRightOffset = aEntry->uRightOffset(iOffset);
    if (C_NIL_OFFSET == iRightOffset)
      aRightEntry = nullptr;
    else
      aRightEntry = pAdd<Entry*>(&m_xWsp,iRightOffset);
  }
 
  ASSERT( ! aRightEntry ||
          ( aRightEntry > aHead() &&
            aRightEntry < _aEntry(m_xWsp.aLatter()) ) );
  
  return aRightEntry;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vShow(void)
{
  vTrace("IndexBuffer %p: entries %lld prevBuffer %p nextBuffer %p \n"_Str,
         this,
         m_nEntries,
         m_pPrevBuffer,
         m_pNextBuffer);
  
  for (Entry* pEntry = aHead(); pEntry != nullptr; pEntry = aNextEntry(pEntry))
    pEntry->vShow();
  
  vTrace("\n"_Str);
  
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
/* converts aEntry to iOffset                                                 */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::iToOffset(Entry* aEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 iOffset = nSub(aEntry,m_xWsp.aArea());
  
  ASSERT(iOffset >= 0 && iOffset < m_xWsp.nSize());
  
  return iOffset;
}

/*----------------------------------------------------------------------------*/
/* verify                                                                     */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vVerify(void)
{
  Entry* pEntryA = aNextEntry(aHead());
  if ( ! pEntryA )
    return;
  
  Entry* pEntryB = aNextEntry(pEntryA);
  if ( ! pEntryB )
    return;
  
  while (pEntryB) {
    
    Key* pKeyA = pEntryA->aKey();
    Key* pKeyB = pEntryB->aKey();
    
    pKeyA->vShow();
    pKeyB->vShow();
    
    ASSERT(*pEntryA->aKey() <= pEntryB->aKey());
    
    pEntryA = pEntryB;
    pEntryB = aNextEntry(pEntryA);
  }
}

/*----------------------------------------------------------------------------*/
bool DBobjects::bI8Compare(void* pKeyInsert, void* pKeyScan, bool& bMatch)
{
  i8 iInsertKey;
  i8 iScanKey;
  
  ValLib::vCopyIntDatum(&iInsertKey,pKeyInsert);
  ValLib::vCopyIntDatum(&iScanKey,pKeyScan);
  
  if ( ! pKeyScan )
    return false;
  
  if (iInsertKey == iScanKey)
    bMatch = true;
  else
    bMatch = false;
  
  if (iInsertKey > iScanKey)
    return true;
  else
    return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
