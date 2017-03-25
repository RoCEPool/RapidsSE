/*============================================================================*/
/* IndexBufferSeqSeq.cpp -- Index Buffer (sequential)                            */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IndexBufferSeq* DBobjects::_pIndexBufferSeq(void* aAddr)
{
  IndexBufferSeq* pIndexBufferSeq = reinterpret_cast<IndexBufferSeq*>(aAddr);
  
  RASSERT( !pIndexBufferSeq || CN_INDEXBUFFER == pIndexBufferSeq->eClass() );
  
  return pIndexBufferSeq;
}

/*----------------------------------------------------------------------------*/
/* allocate an index buffer from the heap                                     */
/*----------------------------------------------------------------------------*/
void* IndexBufferSeq::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_INDEXBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* free an index buffer to the heap                                           */
/*----------------------------------------------------------------------------*/
void IndexBufferSeq::operator delete(void* pIndexBufferSeqAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pIndexBufferSeqAddress);
  
  Heap::vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IndexBufferSeq::IndexBufferSeq(void)
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
IndexBufferSeq::~IndexBufferSeq(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* index buffer factory                                                       */
/*----------------------------------------------------------------------------*/
IndexBufferSeq* IndexBufferSeq::pNewIndexBufferSeq(void)
{
  // should we be testing low buffers for index?
  if (gpBufferPool->iFreeCount() <= C_BUFFER_THRESHOLD)
    return nullptr;
  
  IndexBufferSeq* pIndexBufferSeq = new IndexBufferSeq;
  
  return pIndexBufferSeq;
}

/*----------------------------------------------------------------------------*/
/* dump index buffer                                                          */
/*----------------------------------------------------------------------------*/
void IndexBufferSeq::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_INDEXBUFFER);
  ASSERT(pObject->eClass() == CN_INDEXBUFFER);
  
  IndexBufferSeq* pIndexBufferSeq = reinterpret_cast<IndexBufferSeq*>(pObject);
  
  vTrace("entries %lld free %lld bytes prevBuffer %p nextBuffer %p"_Str,
         pIndexBufferSeq->m_nEntries,
         pIndexBufferSeq->m_xWsp.nFree(),
         pIndexBufferSeq->m_pPrevBuffer,
         pIndexBufferSeq->m_pNextBuffer);
  for (i8 iIndex=0; iIndex < C_FLAT_LEVELS; iIndex++)
    vTrace("skipHead[%lld] %lld\n"_Str,
           iIndex,
           pIndexBufferSeq->m_nSkipHead[iIndex]);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append entries of arg buffer into this buffer                              */
/*----------------------------------------------------------------------------*/
void IndexBufferSeq::vAppendBuffer(IndexBufferSeq* pArgBuffer)
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
void IndexBufferSeq::vCopyBuffer(IndexBufferSeq* pFromBuffer)
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
i8 IndexBufferSeq::nEntries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return m_nEntries;
}

/*----------------------------------------------------------------------------*/
/* set entry count                                                            */
/*----------------------------------------------------------------------------*/
void IndexBufferSeq::vEntryCount(void)
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
IndexBufferSeq* IndexBufferSeq::pFindBuffer(EntWsp& xEntWsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  Entry* pNewEntry;
  
  // get the size of the EntWsp
  i8 nBytes = xEntWsp.nSize();
  
  // get the value of the EntWsp key
  Key* pNewKey = xEntWsp.pKey();
  
  IndexBufferSeq* pIndexBufferSeq = this;
  IndexBufferSeq* pLastBuffer = nullptr;
  
  while (pIndexBufferSeq) {
    
    // find the position for the new entry in the index buffer
    Entry* pEntry = pIndexBufferSeq->pFirstEntry();
    
    while (pEntry) {
      
      Key* pKey = pEntry->pKey();
      
      //pKey->vShow();
      
      if (*pNewKey < pKey ) {
        return pIndexBufferSeq;
      }
      
      // get next entry
      pEntry = pIndexBufferSeq->pNextEntry(pEntry);
    }
    
    pLastBuffer = pIndexBufferSeq;
    pIndexBufferSeq = pIndexBufferSeq->pNextBuffer();
  }
  
  return pLastBuffer;
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the index buffer                                      */
/*----------------------------------------------------------------------------*/
void IndexBufferSeq::vInsertEntry(EntWsp& xEntWsp)
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
bool IndexBufferSeq::bInsertEntry(EntWsp& xEntWsp)
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
void IndexBufferSeq::vShow(void)
{
  vTrace("IndexBufferSeq %p: entries %lld FreeSpace %lld bytes bytesUsed %lld"_Str
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
void IndexBufferSeq::vSplitBuffer(void)
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
  IndexBufferSeq* pNewBuffer = new IndexBufferSeq;
  
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
void IndexBufferSeq::vVerify(void)
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
Entry* IndexBufferSeq::pFirstEntry(void)
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
Entry* IndexBufferSeq::pNextEntry(Entry* pCurr)
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
IndexBufferSeq* IndexBufferSeq::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return m_pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void IndexBufferSeq::vNextBuffer(IndexBufferSeq* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
IndexBufferSeq* IndexBufferSeq::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return m_pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void IndexBufferSeq::vPrevBuffer(IndexBufferSeq* pPrevBuffer)
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
bool IndexBufferSeq::bInsert(Key* pKey, RowId xRowId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* remove existing entry                                                      */
/*----------------------------------------------------------------------------*/
bool IndexBufferSeq::bRemove(Key* pKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* positition to entry (optional set revise vector)                           */
/*----------------------------------------------------------------------------*/
Entry* IndexBufferSeq::pPosition(Key* pKey, bool& bEqualTo, Entry** pReviseVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* generate a new random level                                                */
/*----------------------------------------------------------------------------*/
i8 IndexBufferSeq::iLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  i8 iRndLevel = Random::uLogLevel(C_FLAT_LEVELS,C_FLAT_FREQ);
  ASSERT(iRndLevel >= 0 && iRndLevel < C_FLAT_LEVELS);
  
  return 0;
}

/*----------------------------------------------------------------------------*/
/* get head entry of the skip list in this index buffer                       */
/*----------------------------------------------------------------------------*/
Entry* IndexBufferSeq::pHeadEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXBUFFER,_FOOTER_);
  
  // the head entry is the first entry in wsp
  Entry* pHead = reinterpret_cast<Entry*>(m_xWsp.aArea());
  
  return pHead;
}

/*----------------------------------------------------------------------------*/
/* allot new entry                                                            */
/*----------------------------------------------------------------------------*/
Entry* IndexBufferSeq::pNewEntry(Entry* pAfterEntry, i8 nBytes, i8 nLevels)
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
Entry* IndexBufferSeq::pRightEntry(Entry* pEntry, i8 iLevel)
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
Entry* IndexBufferSeq::pNextEntrySL(Entry* pEntry)
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
void IndexBufferSeq::vMoveLeft(void* aDst, void* aSrc)
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
void IndexBufferSeq::vMoveRight(void* aSrc, i8 nBytes)
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
