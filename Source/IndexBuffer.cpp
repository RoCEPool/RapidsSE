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
  
  for (i8 iIndex=0; iIndex < C_FLAT_LEVELS; iIndex++)
    m_nSkipHead[iIndex] = C_NIL;
  
  m_xWsp.vZero();
  
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
/* insert new entry                                                           */
/*----------------------------------------------------------------------------*/
bool IndexBuffer::bInsert(Key* pKey, RowId xRowId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* remove existing entry                                                      */
/*----------------------------------------------------------------------------*/
bool IndexBuffer::bRemove(Key* pKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* positition to entry (optional set revise vector)                           */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pPosition(Key* pKey, bool& bEqualTo, Entry** pReviseVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return nullptr;
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
/* get head entry of the skip list in this index buffer                       */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pHeadEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // the head entry is the first entry in wsp
  Entry* pHead = reinterpret_cast<Entry*>(m_xWsp.aArea());
  
  return pHead;
}

/*----------------------------------------------------------------------------*/
/* allot new entry                                                            */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pNewEntry(Entry* pAfterEntry, i8 nBytes, i8 nLevels)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);

  m_xWsp.vTake(nBytes);

  Entry* pNewEntry = pAdd<Entry*>(pAfterEntry,nBytes);

  vMoveRight(pNewEntry,nBytes);
  
  new (pNewEntry) Entry(nBytes,nLevels);
  
  return pNewEntry;
}

/*----------------------------------------------------------------------------*/
/* get right entry at given level                                             */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pRightEntry(Entry* pEntry, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  ASSERT(iLevel >= 0 && iLevel < C_FLAT_LEVELS);

  Entry* pRightEntry;
  
  i8 nEntrySize = pEntry->nSize();
  
  if (0 == iLevel) {
    pRightEntry = pAdd<Entry*>(pEntry,nEntrySize);
    return pRightEntry;
  }
  
  pRightEntry = pEntry->pRightEntry(iLevel);
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get next entry after given entry                                           */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pNextEntrySL(Entry* pEntry)
{
  i8 nBytes = pEntry->nSize();
  
  Entry* pNext = pAdd<Entry*>(pEntry,nBytes);
  
  if ((char*)pNext < m_xWsp.aLatter())
    return pNext;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* move portion of the wsp left                                               */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vMoveLeft(void* aDst, void* aSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  ASSERT(aDst < aSrc);
  
  // compute the number of bytes being moved
  i8 nBytes = nSub(aSrc,aDst);
  
  // copy the bytes left
  MemLib::vMove(aDst,aSrc,nBytes);
  
  // reduce used in wsp
  m_xWsp.vGive(nBytes);
}

/*----------------------------------------------------------------------------*/
/* move part of the wsp right by given bytes                                  */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vMoveRight(void* aSrc, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // compute the destination address
  void* aDst = pAdd<void*>(aSrc,nBytes);
  
  // increase used in wsp
  m_xWsp.vTake(nBytes);
  
  // move the bytes right
  MemLib::vMove(aDst,aSrc,nBytes);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
