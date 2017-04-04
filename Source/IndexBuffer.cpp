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
  
  return false;
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
  m_xWsp.vTake(nBytes);
  
  // compute the destination address
  void* aDst = pAdd<void*>(aSrc,nBytes);
  
  // move F..Z right
  MemLib::vMove(aDst,aSrc,nBytes);
  
  // for the moment we'll zero the bytes of entry E
  MemLib::vZero(aDst,nBytes);
}

/*----------------------------------------------------------------------------*/
/* allot new entry after given entry (return null if insufficient space)      */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::aNewEntry(i8 nBytes, i8 nLevels, Entry* aAfterEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // check there is enough space for the new entry
  if (m_xWsp.nFree() < nBytes)
    return nullptr;
  
  // allot space for the new entry
  m_xWsp.vTake(nBytes);

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
  new (aNewEntry) Entry(nBytes,nLevels);
  
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
  i8 nBytes = Entry::nBytesRequired(0,C_FLAT_LEVELS);
  
  // allot the head
  Entry* aHead = aNewEntry(nBytes,C_FLAT_LEVELS,nullptr);
  
  // initialize the head
  for (i8 iLevel = 0; iLevel < C_FLAT_LEVELS; iLevel++)
    ;
  //aHead->m_iRight[iLevel] = C_NIL_OFFSET;
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
  if (aNext < _aEntry(m_xWsp.aLatter()))
    return aNext;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* positition to entry (optional set revise vector)                           */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::aPosition(Key* aKey, bool& bEqualTo, Entry** aReviseVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return nullptr;
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
  ASSERT(iLevel >= 0 && iLevel <= aEntry->nLevels());

  if (0 == iLevel) {
    // for level 0, the right entry is the next entry
    aRightEntry = aNextEntry(aEntry);
  }
  else {
    // for levels 1 & 2 the right entry is m_xWsp + rightOffset[iRight]
    i8 iRightOffset = aEntry->uRightOffset(iLevel);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
