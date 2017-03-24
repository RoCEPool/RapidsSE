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
/* dump index buffer                                                          */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_INDEXBUFFER);
  ASSERT(pObject->eClass() == CN_INDEXBUFFER);
  
  IndexBuffer* pIndexBuffer = reinterpret_cast<IndexBuffer*>(pObject);
  
  vTrace("entries %lld free %lld bytes prevBuffer %p nextBuffer %p"_Str,
         pIndexBuffer->m_nEntries,
         pIndexBuffer->m_xWsp.nFree(),
         pIndexBuffer->m_pPrevBuffer,
         pIndexBuffer->m_pNextBuffer);
  for (i8 iIndex=0; iIndex < C_FLAT_LEVELS; iIndex++)
    vTrace("skipHead[%lld] %lld\n"_Str,
           iIndex,
           pIndexBuffer->m_nSkipHead[iIndex]);
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
  void* pDst = pAdd<void*>(m_xWsp.aArea(),m_xWsp.nUsed());
  void* pSrc = pArgBuffer->m_xWsp.aArea();
  i8 nBytes  = pArgBuffer->m_xWsp.nUsed();
  MemLib::vCopy(pDst,pSrc,nBytes);
  
  //m_pFreeSpace = pAdd<char*>(m_pFreeSpace,nBytes);
  
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
  
  i8 nBytes = pFromBuffer->m_xWsp.nUsed();
  
  MemLib::vCopy(m_xWsp.aArea(),pFromBuffer->m_xWsp.aArea(),nBytes);
  
  m_xWsp.vTake(nBytes);
  
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
  //if (m_xWsp.nWspFree() < nBytes) {
    // split the buffer
  //  vSplitBuffer();
  //}
  
  // get the pDst as the starting of the free space in the buffer
  void* pDst = reinterpret_cast<void*>(m_xWsp.aLatter());
  
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
      i8 nSize = nSub(m_xWsp.aLatter(),reinterpret_cast<char*>(pEntry));
      
      // calculate new postion to shuffle to
      void* pShuffleTo = pAdd<void*>(pEntry,nBytes);
      
      // do the shuffle
      MemLib::vCopy(pShuffleTo,pEntry,nSize);
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
  
  // we have taken nBytes from the wsp
  m_xWsp.vTake(nBytes);
  
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
  if (m_xWsp.nFree() < (C_1MB - 5000)) {
  //if (m_xWsp.nWspFree() < nBytes) {
    i8 iFree = m_xWsp.nFree();
    i8 iBytesUsed = m_xWsp.nUsed();
    
    // split the buffer
    vSplitBuffer();
    
    return false;
  }
  
  // get the pDst as the starting of the free space in the buffer
  void* pDst = reinterpret_cast<void*>(m_xWsp.aLatter());

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
      i8 nSize = nSub(m_xWsp.aLatter(),reinterpret_cast<char*>(pEntry));
      
      // calculate new postion to shuffle to
      void* pShuffleTo = pAdd<void*>(pEntry,nBytes);
      
      // do the shuffle
      MemLib::vCopy(pShuffleTo,pEntry,nSize);
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
  
  // we have taken nBytes from the wsp
  m_xWsp.vTake(nBytes);
  
  // inc the number of entries in this buffer
  m_nEntries++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vShow(void)
{
  vTrace("IndexBuffer %p: entries %lld FreeSpace %lld bytes bytesUsed %lld"_Str
         " prevBuffer %p nextBuffer %p "_Str,
         this,
         m_nEntries,
         m_xWsp.nFree(),
         m_xWsp.nUsed(),
         m_pPrevBuffer,
         m_pNextBuffer);
  
  for (i8 iIndex=0; iIndex < C_FLAT_LEVELS; iIndex++)
    vTrace("skipHead[%lld] %lld "_Str,
           iIndex,
           m_nSkipHead[iIndex]);
  
  vTrace("\n"_Str);

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
  
  // calculate bytes to copy to new buffer
  i8 nSize = nSub(m_xWsp.aLatter(), pSplitAt);
  
  // allocate a new buffer
  IndexBuffer* pNewBuffer = new IndexBuffer;
  
  // calculate new postion to shuffle to
  void* pNewDst = pNewBuffer->m_xWsp.aArea();
  
  // do the shuffle
  MemLib::vCopy(pNewDst,pEntry,nSize);
  
  pNewBuffer->m_xWsp.vTake(nSize);
  
  //pNewBuffer->m_pFreeSpace = pAdd<char*>(pNewBuffer->aWspLatter(),nSize);
  pNewBuffer->m_nEntries = iSplitAtEntry;
  
  // give back nSize bytes to wsp in original buffer
  m_xWsp.vGive(nSize);
  
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
  
    //pKeyA->vShow();
    //pKeyB->vShow();
    
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
  
  Entry* pFirst = reinterpret_cast<Entry*>(m_xWsp.aArea());
  
  if ((char*)pFirst < m_xWsp.aLatter())
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
  
  if ((char*)pNext < m_xWsp.aLatter())
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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//
//                              skip list support                             //
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//

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
/* get head entry                                                             */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pHeadEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  Entry* pHead = reinterpret_cast<Entry*>(m_xWsp.aArea());
  
  return pHead;
}

/*----------------------------------------------------------------------------*/
/* allot new entry                                                            */
/*----------------------------------------------------------------------------*/
Entry* IndexBuffer::pNewEntry(i8 nBytes, i8 nLevels)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  Entry* pEntry = reinterpret_cast<Entry*>(m_xWsp.aAllot(nBytes));
  
  return pEntry;
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
  
  //u8* aRightVec = pEntry->aRightVec();
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* move portion of the wsp left                                               */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vMoveLeft(void* aDst, void* aSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* move portion of the wsp right by given bytes                               */
/*----------------------------------------------------------------------------*/
void IndexBuffer::vMoveRight(void* aDst, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
