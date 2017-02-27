/*============================================================================*/
/* IndexBuffer.cpp -- Index Buffer                                            */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IndexBuffer* DBobjects::_pIndexBuffer(void* pAddr)
{
  IndexBuffer* pIndexBuffer = reinterpret_cast<IndexBuffer*>(pAddr);
  
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
  m_pFreeSpace  = pDataArea();
  
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
/* dump index buffer                                                          */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_INDEXBUFFER);
  ASSERT(pObject->eClass() == CN_INDEXBUFFER);
  
  IndexBuffer* pIndexBuffer = reinterpret_cast<IndexBuffer*>(pObject);
  
  vTrace("entries %lld freeSpace %p (%lld bytes) prevBuffer %p nextBuffer %p\n",
         pIndexBuffer->m_nEntries,
         pIndexBuffer->m_pFreeSpace,
         pIndexBuffer->nFreeBytes(),
         pIndexBuffer->m_pPrevBuffer,
         pIndexBuffer->m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append entries of arg buffer into this buffer                              */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vAppendBuffer(IndexBuffer* pArgBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // copy arg entries into this buffer
  void* pDst = pAdd<void*>(pDataArea(),nBytesUsed());
  void* pSrc = pArgBuffer->pDataArea();
  i8 nBytes  = pArgBuffer->nBytesUsed();
  Memory::vCopy(pDst,pSrc,nBytes);
  
  m_pFreeSpace = pAdd<char*>(m_pFreeSpace,nBytes);
  
  // inc the number of entries in this buffer
  i8 nArgEntries = pArgBuffer->nEntries();
  m_nEntries += nArgEntries;
}

/*----------------------------------------------------------------------------*/
/* copy given buffer                                                          */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vCopyBuffer(IndexBuffer* pFromBuffer)
{                                            //! i'm not sure about this fn -ray
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 nBytes = pFromBuffer->nBytesUsed();
  
  Memory::vCopy(pDataArea(),pFromBuffer->pDataArea(),nBytes);
  
  char* pRevised = pAdd<char*>(pDataArea(),nBytes);
  
  m_pFreeSpace = reinterpret_cast<char*>(pRevised);
  
  vEntryCount();
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_INDEXBUFFER);
}

/*----------------------------------------------------------------------------*/
/* get number of entries in buffer                                            */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::nEntries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return m_nEntries;
}

/*----------------------------------------------------------------------------*/
/* set entry count                                                            */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vEntryCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  m_nEntries = 0;
  for (Entry* pEntry = pFirstEntry(); pEntry; pEntry = pNextEntry(pEntry))
    m_nEntries++;
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_INDEXBUFFER);
}

/*----------------------------------------------------------------------------*/
/* find index buffer for this entWsp                                          */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexBuffer::pFindBuffer(EntWsp& xEntWsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  Entry* pNewEntry;
  
  // get the size of the EntWsp
  i8 nBytes = xEntWsp.nSize();
  
  // get the value of the EntWsp key
  Key* pNewKey = xEntWsp.pKey();
  
  IndexBuffer* pIndexBuffer = this;
  IndexBuffer* pLastBuffer = nullptr;
  
  while (pIndexBuffer) {
    
    // find the position for the new entry in the index buffer
    Entry* pEntry = pIndexBuffer->pFirstEntry();
    
    while (pEntry) {
      
      Key* pKey = pEntry->pKey();
      
      //pKey->vShow();
      
      if (*pNewKey < pKey ) {
        return pIndexBuffer;
      }
      
      // get next entry
      pEntry = pIndexBuffer->pNextEntry(pEntry);
    }
    
    pLastBuffer = pIndexBuffer;
    pIndexBuffer = pIndexBuffer->pNextBuffer();
  }
  
  return pLastBuffer;
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the index buffer                                      */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vInsertEntry(EntWsp& xEntWsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  Entry* pNewEntry;
  
  // get the size of the EntWsp
  i8 nBytes = xEntWsp.nSize();
  
  // test sufficient free space
  //if (nFreeBytes() < nBytes) {
    // split the buffer
  //  vSplitBuffer();
  //}
  
  // get the pDst as the staring of the free space in the buffer
  void* pDst = pAdd<void*>(pDataArea(),nBytesUsed());
  
  // get the vale of the EntWsp key
  Key* pNewKey = xEntWsp.pKey();
  
  // find the position for the new entry in the index buffer
  Entry* pEntry = pFirstEntry();
  
  while (pEntry) {
    
    Key* pKey = pEntry->pKey();
    
    if (*pNewKey < pKey ) {
      
      // position the pDst at pEntry
      pDst = pEntry;
      
      // shuffle right pEntry making space for new entry
      i8 nSize = nBytesUsed() - nSub(pEntry,pDataArea());
      
      // calculate new postion to shuffle to
      void* pShuffleTo = pAdd<void*>(pEntry,nBytes);
      
      // do the shuffle
      Memory::vCopy(pShuffleTo,pEntry,nSize);
      break;
    }
    
    // get next entry
    pEntry = pNextEntry(pEntry);
  }
  
  // map the pDst to an Entry
  pNewEntry = reinterpret_cast<Entry*>(pDst);
  
  // copy EntWsp into Index buffer pNewEntry
  pNewEntry->vEntry(xEntWsp);
  
  // size of EntWsp whould be the same as this NewEntry we just created
  ASSERT(nBytes == pNewEntry->nSize());
  
  // recalculate beginning of free space
  m_pFreeSpace = pAdd<char*>(m_pFreeSpace,nBytes);
  
  // inc the number of entries in this buffer
  m_nEntries++;
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the index buffer                                      */
/*----------------------------------------------------------------------------*/
bool IndexBuffer::bInsertEntry(EntWsp& xEntWsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  Entry* pNewEntry;
  
  // get the size of the EntWsp
  i8 nBytes = xEntWsp.nSize();
  
  // test sufficient free space
  //if (nFreeBytes() < (C_1MB - 1000))
  if (nFreeBytes() < nBytes) {
    i8 iFree = nFreeBytes();
    i8 iBytesUsed = nBytesUsed();
    
    // split the buffer
    vSplitBuffer();
    
    return false;
  }
  
  // get the pDst as the staring of the free space in the buffer
  void* pDst = pAdd<void*>(pDataArea(),nBytesUsed());

  // get the vale of the EntWsp key
  Key* pNewKey = xEntWsp.pKey();
  
  // find the position for the new entry in the index buffer
  Entry* pEntry = pFirstEntry();
  
  while (pEntry) {
    
    Key* pKey = pEntry->pKey();
    
    if (*pNewKey < pKey ) {
      
      // position the pDst at pEntry
      pDst = pEntry;
      
      // shuffle right pEntry making space for new entry
      i8 nSize = nBytesUsed() - nSub(pEntry,pDataArea());
      
      // calculate new postion to shuffle to
      void* pShuffleTo = pAdd<void*>(pEntry,nBytes);
      
      // do the shuffle
      Memory::vCopy(pShuffleTo,pEntry,nSize);
      break;
    }
    
    // get next entry
    pEntry = pNextEntry(pEntry);
  }
  
  // map the pDst to an Entry
  pNewEntry = reinterpret_cast<Entry*>(pDst);
  
  // copy EntWsp into Index buffer pNewEntry
  pNewEntry->vEntry(xEntWsp);
  
  // size of EntWsp whould be the same as this NewEntry we just created
  ASSERT(nBytes == pNewEntry->nSize());
  
  // recalculate beginning of free space
  m_pFreeSpace = pAdd<char*>(m_pFreeSpace,nBytes);
  
  // inc the number of entries in this buffer
  m_nEntries++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vShow(void)
{
  vTrace("IndexBuffer %p: entries %lld freeSpace %p (%lld bytes) bytesUsed %lld"
         " prevBuffer %p nextBuffer %p\n",
         this,
         m_nEntries,
         m_pFreeSpace,
         nFreeBytes(),
         nBytesUsed(),
         m_pPrevBuffer,
         m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* split this index buffer into two                                           */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vSplitBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // split the buffer into two
  i8 iSplitAtEntry = m_nEntries/2;
  
  // find the position for the new entry in the index buffer
  Entry* pEntry = pFirstEntry();
  
  i8 iEntries = 0;
  while (pEntry) {
    
    iEntries++;
    
    if (iEntries == iSplitAtEntry)
      break;
    
    // get next entry
    pEntry = pNextEntry(pEntry);
  }
  
  // get the address of the entry to split at
  char* pSplitAt = reinterpret_cast<char*>(pEntry);
  
  // allocate a new buffer
  IndexBuffer* pNewBuffer = new IndexBuffer;
  
  // get size of entries to be copied to new buffer
  i8 nSize = nSub(m_pFreeSpace,pEntry);
  
  // calculate new postion to shuffle to
  void* pNewDst = pNewBuffer->pDataArea();
  
  // do the shuffle
  Memory::vCopy(pNewDst,pEntry,nSize);
  
  pNewBuffer->m_pFreeSpace = pAdd<char*>(pNewBuffer->m_pFreeSpace,nSize);
  pNewBuffer->m_nEntries = iSplitAtEntry;
  
  // reset freespace in original buffer
  m_pFreeSpace = pSplitAt;
  i8 iFreeSpace = nFreeBytes();
  i8 iNewFree = pNewBuffer->nFreeBytes();
  
  m_nEntries -= iSplitAtEntry;

  // now link the two buffers
  pNewBuffer->vNextBuffer(pNextBuffer());
  pNewBuffer->vPrevBuffer(this);
  
  vNextBuffer(pNewBuffer);
}

/*----------------------------------------------------------------------------*/
/* verify                                                                     */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vVerify(void)
{
  Entry* pEntryA = pFirstEntry();
  if ( ! pEntryA )
    return;

  Entry* pEntryB = pNextEntry(pEntryA);
  if ( ! pEntryB )
    return;
  
  while (pEntryB) {
    
    Key* pKeyA = pEntryA->pKey();
    Key* pKeyB = pEntryB->pKey();
  
    pKeyA->vShow();
    pKeyB->vShow();
    
    ASSERT(*pEntryA->pKey() <= pEntryB->pKey());
    
    pEntryA = pEntryB;
    pEntryB = pNextEntry(pEntryA);
  }
}

/*----------------------------------------------------------------------------*/
/* first entry                                                                */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pFirstEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  Entry* pFirst = reinterpret_cast<Entry*>(pDataArea());
  
  if ((char*)pFirst < (char*)m_pFreeSpace)
    return pFirst;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* next entry                                                                 */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pNextEntry(Entry* pCurr)
{
  i8 iSize = pCurr->nSize();
  
  Entry* pNext = pAdd<Entry*>(pCurr,iSize);
  
  if ((char*)pNext < (char*)m_pFreeSpace)
    return pNext;
  else
    return nullptr;
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
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vNextBuffer(IndexBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return m_pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vPrevBuffer(IndexBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calc bytes used                                                            */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::nBytesUsed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 nBytes = nSub(m_pFreeSpace,pDataArea());
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
char* IndexBuffer::pDataArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  char* pStart = pAdd<char*>(this,sizeof(IndexBuffer));
  
  return pStart;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in data area                           */
/*----------------------------------------------------------------------------*/
i8 IndexBuffer::nFreeBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 nBytes = nSub(m_pLastByte,m_pFreeSpace);

  nBytes -= sizeof(Footer);
  
  return nBytes;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
