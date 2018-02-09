/*============================================================================*/
/* EntryBuffer.cpp -- Entry Buffer                                            */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
EntryBuffer* DBobjects::_pEntryBuffer(void* aAddr)
{
  EntryBuffer* pEntryBuffer = reinterpret_cast<EntryBuffer*>(aAddr);
  
  RASSERT( !pEntryBuffer || CN_ENTRYBUFFER == pEntryBuffer->eClass());
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* EntryBuffer::operator new(size_t iSize)
{
  RASSERT(iSize);
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_ENTRYBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* EntryBuffer::operator new(size_t iSize, void* aAddr)
{
  RASSERT(iSize);
  
  return aAddr;
}

/*----------------------------------------------------------------------------*/
/* return a row buffer to the buffer pool                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer::operator delete(void* pRowBufferAddress)
{
  Buffer* pBuffer = reinterpret_cast<Buffer*>(pRowBufferAddress);
  
  Heap::vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer::EntryBuffer(void)
: ItemBuffer(CN_ENTRYBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_eDurability   = BD_PERMANENT;
  m_bIsLast       = false;
  
  m_pHigherBuffer = nullptr;
  m_pNextBuffer   = nullptr;
  m_pPrevBuffer   = nullptr;

  m_pIndex        = nullptr;
  m_nPartitionNo  = -1;
  m_nBufferLevel  = 0;
  
  m_pEntryOffset    =
    reinterpret_cast<i4*>(aAllotSpace(sizeof(i4) * C_ENTRIES_0));
  m_iFreeOffsetHead = C_ENTRY_NONE;
  vFreeChains();
  
  m_pHeadEntry  = nullptr;
  m_pLastEntry  = nullptr;
  m_pFirstFree  = nullptr;
  
  vHeadEntryNew();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_ENTRYBUFFER);
}
/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer::EntryBuffer(i8 iDataBytes)
: ItemBuffer(CN_ENTRYBUFFER)
{
  RASSERT(0);
}

EntryBuffer::EntryBuffer(Index* pIndex, i8 nLevel, i8 nPartitionNo)
: ItemBuffer(CN_ENTRYBUFFER) {
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntryBuffer::~EntryBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump entry buffer                                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(CN_ATTACHMAP == eClass);
  
  EntryBuffer* pEntryBuffer = _pEntryBuffer(pObject);

  pEntryBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set buffer level                                                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vBufferLevel(i8 nBufferLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_nBufferLevel = nBufferLevel;
}

/*----------------------------------------------------------------------------*/
/* get the buffer level                                                       */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::nBufferLevel(void)
{
  return m_nBufferLevel;
}

/*----------------------------------------------------------------------------*/
/* get durability                                                             */
/*----------------------------------------------------------------------------*/
eBD EntryBuffer::eDurability(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_eDurability;
}

/*----------------------------------------------------------------------------*/
/* set durability                                                             */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDurability(eBD eDurability)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_eDurability = eDurability;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
FreeEntry* EntryBuffer::pFirstFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pFirstFree;
}

/*----------------------------------------------------------------------------*/
/* get head entry                                                             */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pHeadEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pHeadEntry;
}

/*----------------------------------------------------------------------------*/
/* get the higher level index buffer                                          */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pHigherBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pHigherBuffer;
}

/*----------------------------------------------------------------------------*/
/* set the higher level index                                                 */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vHigherBuffer(EntryBuffer* pHigherBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //ASSERT( ! m_pHigherBuffer);
  
  m_pHigherBuffer = pHigherBuffer;
}

/*----------------------------------------------------------------------------*/
/* set the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pIndex = pIndex;
  
  // create a head entry if we have a non-hidden index
  //if (! m_pIndex->bIsHidden() )
}

/*----------------------------------------------------------------------------*/
/* get the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
Index* EntryBuffer::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pIndex;
}

/*----------------------------------------------------------------------------*/
/* set buffer is last                                                         */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vIsLast(bool bIsLast)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_bIsLast = bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get if buffer is last                                                      */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bIsLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vNextBuffer(EntryBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set partition no                                                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vPartitionNo(i8 nPartitionNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_nPartitionNo = nPartitionNo;
}

/*----------------------------------------------------------------------------*/
/* get partition no                                                           */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::nPartitionNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_nPartitionNo;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vPrevBuffer(EntryBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Complex Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyze this buffer                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries) {
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (m_pIndex->bIsHidden()) {
    
    ASSERT(m_nBufferLevel == 0);
    EntryBuffer* pNextBuffer = this;
    
    do {
      ASSERT(pNextBuffer->nBufferLevel() == 0);
      
      iBuffers++;
      iEntries += (pNextBuffer->m_nItems);
      
      if (bDetailed)
        sAnalysis.vCatSprint("     buffer %5lld (L%lld) : %lld entries\n"_Stz,
                             iBuffers,
                             pNextBuffer->m_nBufferLevel,
                             pNextBuffer->m_nItems);
      
      pNextBuffer = pNextBuffer->pNextBuffer();
    } while (pNextBuffer);
    
    return;
  }
  
  iBuffers++;
  iEntries += m_nItems;
  
  if (bDetailed)
    sAnalysis.vCatSprint("     buffer %5lld (L%lld) : %lld entries\n"_Stz,
                         iBuffers,
                         m_nBufferLevel,
                         m_nItems);
  
  // recursively dump the lower level buffers
  if (m_nBufferLevel != 0) {
    
    for (Entry* pEntry = pFirstEntry(); pEntry != nullptr;
         pEntry = pNextEntry(pEntry, 0)) {
      
      // get the index buffer from the entry
      EntryBuffer* pEntryBuffer = pEntry->pEntryBuffer();;
      
      ASSERT(pEntryBuffer);
      ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
      
      pEntryBuffer->vAnalyze(sAnalysis, bDetailed, iBuffers, iEntries);
      
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* append new entry into this index buffer                                    */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pAppendEntry(EntWrk& xEntWrk,EntryVec& xPriorVec)
{
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* append an entry for {key,id} into an index buffer at level 0               */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bAppendEntry(Entry* pRowEntry, Entry** ppEntry)     //# critical
{
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* attempt to append a new row entry into this buffer                         */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pAppendRow(i8 iTxId, Stz* aRow, i8 iRowLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(nBufferLevel() == 0);
  
  i8 nOffsets = 1;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if this buffer has enough space
  i8 nOffsetBytes = sizeof(i4);
  i8 nEntryBytes = Entry::nBytesRequired(nOffsets, iRowLength);
  i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;
  
  if ( ! bHasSpace(nEntryPlusOffsets) )
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the rowId
  
  bn nBufferNo = this->nBufferNo();
  i8 nRowNo = iGetFreeEntry(); //nItems();
  Id xRowId(*nBufferNo, nRowNo);
  
  //xRowWrk.vId(xRowId);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate space in this buffer
  
  Entry* pRowEntry = pAllotEntry(nEntryBytes);
  ASSERT(pRowEntry);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the buffer entry
  
  pRowEntry->vSetEntry(iTxId, xRowId, nOffsets, aRow, iRowLength);
  //pRowEntry->vShow(0, m_pIndex, false);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set entry offsets
  
  i8 iEntryOffset = iToOffset(pRowEntry);
  m_pEntryOffset[nRowNo] = iEntryOffset;
  
  NEO* aNEO = aAllotNEO(nOffsetBytes);
  if (m_pLastEntry)
    m_pLastEntry->vNEO(0,iEntryOffset);
  pRowEntry->vNEO(0,-1);
  
  m_pLastEntry = pRowEntry;
  
  // set head entry offset
  if (m_pHeadEntry->iNEO(0) == -1)
    m_pHeadEntry->vNEO(0, iEntryOffset);
  
  return pRowEntry;
}

/*----------------------------------------------------------------------------*/
/* return number of index buffers under this buffer                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vBufferCount(i8& nBuffers)
{
  EntryBuffer* pNextBuffer = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // hidden index ?
  
  if (m_pIndex->bIsHidden()) {
    ASSERT(m_nBufferLevel == 0);
    pNextBuffer = this->pNextBuffer();
    nBuffers++;
    
    while (pNextBuffer) {
      ASSERT(pNextBuffer->nBufferLevel() == 0);
      
      EntryBuffer* pThisBuffer = pNextBuffer;
      pNextBuffer = pNextBuffer->pNextBuffer();
      nBuffers++;
    }
    
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // other index
  
  // count this buffer
  nBuffers++;
  
  // recursively dump the lower level buffers
  if (m_nBufferLevel != 0) {
    
    for (Entry* pEntry = pFirstEntry(); pEntry != nullptr;
         pEntry = pNextEntry(pEntry, 0)) {
      
      // get the index buffer from the entry
      pNextBuffer = pEntry->pEntryBuffer();;
      
      ASSERT(pNextBuffer);
      ASSERT(pNextBuffer->eClass() == CN_ENTRYBUFFER);
      
      pNextBuffer->vBufferCount(nBuffers);
      
    }
  }
}

/*----------------------------------------------------------------------------*/
/* compact this buffer                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vCompact(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  FreeEntry* pFreeEntry;
  Entry*     pMoveEntry;
  Entry*     xPriorVec[3];
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  // we can only compact if there are free entries
  if (! m_pFirstFree)
    return;

  // loose the free list so vShow() works correctly
  m_pFirstFree = nullptr;

  // scan item is the working pointer
  Item* pScanItem = pFirstItem();
  
  // loop while we can still reclaim more space
  for (;;) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // find the next free entry in buffer
    
    pFreeEntry = nullptr;
    for (;
         pScanItem != nullptr;
         pScanItem = pNextItem(pScanItem)) {
      
      eCN eClass = pScanItem->eClass();
      
      if (CN_FREEENTRY == eClass) {
        pFreeEntry = static_cast<FreeEntry*>(pScanItem);
        break;
      }
    }
    
    // return if no more free entries
    if (! pFreeEntry )
      break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // find the next entry in buffer after the free entry
    
    pMoveEntry = nullptr;
    for (Item* pItem = pNextItem(pFreeEntry);
         pItem != nullptr;
         pItem = pNextItem(pItem)) {
      
      eCN eClass = pItem->eClass();
      
      if (CN_ENTRY == eClass) {
        pMoveEntry = static_cast<Entry*>(pItem);
        break;
      }
    }
    
    // return if no more entries after the free entry
    if (! pMoveEntry )
      break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the prior vector to the entry being moved
    
    Key* aKey = pMoveEntry->aKey();
    aKey->vShow(pIdxColVec);
    vTrace("\n"_Stz);
    
    vPositionPrior(aKey, xPriorVec);
    //vDumpPriorVec(xPriorVec);
    
    i8 iWasOffset = iToOffset(pMoveEntry);
    i8 iNowOffset = iToOffset(pFreeEntry);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // now copy move entry left onto free entry
    
    // calculate the number of bytes to copy
    i8 nCopyBytes = pMoveEntry->nSizeEntry() + pMoveEntry->iNEOsize();
    
    // copy the move entry over the free entry
    MemLib::vMove(pFreeEntry, pMoveEntry, nCopyBytes, CN_ENTRYBUFFER);
    
    // give the reclaimed space back to the buffer
    i8 nReclaimedBytes = nSub(pMoveEntry,pFreeEntry);
    //vGiveSpace(nReclaimedBytes);
    
    // convert the moved entry to a free entry
    FreeEntry* pOpenedFreeEntry = pAdd<FreeEntry*>(pFreeEntry,nCopyBytes);
    new(pOpenedFreeEntry) FreeEntry(nReclaimedBytes);
    
    Entry* pCheckEntry = reinterpret_cast<Entry*>(pFreeEntry);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the prior vector to the entry being moved
    
    for (i8 i_Offset = 0; i_Offset < 3; i_Offset++)
      if (iWasOffset == xPriorVec[i_Offset]->iNEO(i_Offset))
        xPriorVec[i_Offset]->vNEO(i_Offset,iNowOffset);
    
    vShow();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move scan
    
    pScanItem = pFreeEntry;
    
  } // for
  
  ASSERT(CN_FREEENTRY == pScanItem->eClass());
  
  vResetUsed(pScanItem);
  
  vSetWilderness();
  
  vShow();
}

/*----------------------------------------------------------------------------*/
/* get decile full of the buffer                                              */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::iDecile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 iDecile;
  
  if (gpAttachMap->m_nIndexEntries == 0)
    iDecile = IntLib::iDecile(m_xWsp.nUsed(), C_SIZE_POOL_BUFFER);
  else
    iDecile = IntLib::iDecile(m_nItems, gpAttachMap->m_nIndexEntries);
  
  
  return iDecile;
}

/*----------------------------------------------------------------------------*/
/* delete the given entry                                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDeleteEntry(Entry* pEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // currently we only delete from non-hidden indexes
  ASSERT(! m_pIndex->bIsHidden());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // remove entry from next offset chains
  
  Entry* xPriorVec[3];

  Key* aKey = pEntry->aKey();
  vPositionPrior(aKey, xPriorVec);
  //vDumpPriorVec(xPriorVec);
  
  i8 nOffsets = pEntry->nOffsets();
  for (i8 i_Offset = 0; i_Offset < nOffsets; i_Offset++) {
    i4 iNEO = pEntry->iNEO(i_Offset);
    xPriorVec[i_Offset]->vNEO(i_Offset,iNEO);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add entry to head of free entry list
  
  i8 nSizeEntry = pEntry->nSizeEntry();
  i8 nSizeNEO   = pEntry->iNEOsize();
  i8 nSizeFree  = nSizeEntry + nSizeNEO;
  
  FreeEntry* pFreeEntry = new(pEntry) FreeEntry(nSizeFree);
  
  if (m_pFirstFree) {
    m_pFirstFree->vPrevFree(pFreeEntry);
    pFreeEntry->vNextFree(m_pFirstFree);
  }
  
  m_pFirstFree = pFreeEntry;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // decement the number of items in buffer
  vDecItems();
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDump(void)                            //ALKA - LEVEL 1 ??
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // show this buffer
  vShow();
  
  // recursively dump the lower level buffers
  if (m_nBufferLevel != 0) {
    
    for (Entry* pEntry = pFirstEntry();
         pEntry != nullptr;
         pEntry = pNextEntry(pEntry, 0)) {
      
      // get the index buffer from the entry
      EntryBuffer* pEntryBuffer = pEntry->pEntryBuffer();
      
      ASSERT(pEntryBuffer);
      ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
      
      pEntryBuffer->vDump();
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get first entry                                                            */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pFirstEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //m_pHeadEntry->vNEOdump();
  
  Entry* pFirst = pNextEntry(m_pHeadEntry,0);
  
  return pFirst;
}

/*----------------------------------------------------------------------------*/
/* get an entry by number                                                     */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pGetEntryNo(i8 iEntryNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (iEntryNo == C_ENTRY_NONE)
    return nullptr;
  
  i4 iOffset = m_pEntryOffset[iEntryNo];
  char* aAddress = pAdd<char*>(aDataArea(), iOffset);
  
  Entry* pEntry = reinterpret_cast<Entry*>(aAddress);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bHasSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nFree = m_xWsp.nFree();
  
  if (gpAttachMap->m_nIndexEntries > 0) {
    // has space calculation based on no of entries
    if (m_nItems >= gpAttachMap->m_nIndexEntries)
      return false;
    else
      return true;
  }
  else {
    // has space calculation based on free space
    if (nFree < nBytes)
      return false;
    else
      return true;
  }
}

/*----------------------------------------------------------------------------*/
/* insert new entry into this index buffer                                    */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bInsert(EntWrk& xEntWrk)                        //# critical
{
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* attempt to insert a new key entry into this buffer                         */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pInsertKey(i8 iTxId, Key* aKey, i8 iKeyLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  vTrace("Inserting key:\n"_Stz);
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  //aKey->vShow(pIdxColVec);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert a new entry into the skip list
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1 offset
  //i8 nOffsets = 1;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2 offsets
  //i8 nOffsets = Random::iLogLevel(gpAttachMap->m_nIndexOffsets,C_FLAT_FREQ + 1);
  i8 nOffsets = Random::iLogLevel(3,C_FLAT_FREQ + 1) + 1;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 3 offsets
  //i8 nOffsets = Random::iLogLevel(gpAttachMap->m_nIndexOffsets+1,C_FLAT_FREQ) + 1;
  
  Entry* xPriorVec[3];
  
  // find the entry after which the new entry.key will be inserted
  //Key* aKey = xKeyWrk.aKey();
  vPositionPrior(aKey, xPriorVec);
  //vDumpPriorVec(xPriorVec);
  
  // set no of offsets in working key
  //xKeyWrk.vOffsets(nOffsets); // 1,2 or 3 for the moment
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if this buffer has enough space
  i8 nEntryBytes = Entry::nBytesRequired(nOffsets, iKeyLength);
  
  i8 nOffsetBytes = nOffsets*sizeof(i4); //xKeyWrk.nOffsetSize();
  
  i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;
  if ( ! bHasSpace(nEntryPlusOffsets) )
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the rowId
  
  bn nBufferNo = this->nBufferNo();
  i8 nRowNo = iGetFreeEntry(); //nItems();
  Id xRowId(*nBufferNo, nRowNo);
  
  //xKeyWrk.vId(xRowId);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate space in this buffer
  
  Entry* pKeyEntry = pAllotEntry(nEntryBytes);
  ASSERT(pKeyEntry);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the key entry
  
  //pKeyEntry->vSetEntry(xKeyWrk);
  pKeyEntry->vSetEntry(iTxId, xRowId, nOffsets, (Stz*)aKey, iKeyLength);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set prior offsets
  
  i8 iKeyEntryOffset = iToOffset(pKeyEntry);
  
  NEO* aNEO = aAllotNEO(nOffsetBytes);
  
  for (i8 iNextOffset = 0; iNextOffset < nOffsets; iNextOffset++) {
    
    i8 iPriorOffset = xPriorVec[iNextOffset]->iNEO(iNextOffset);
    
    pKeyEntry->vNEO(iNextOffset,iPriorOffset);
    
    xPriorVec[iNextOffset]->vNEO(iNextOffset,iKeyEntryOffset);
  }
  
  //vDump();
  
  return pKeyEntry;
}

/*----------------------------------------------------------------------------*/
/* insert row into this buffer (invoked from entryBuffer.insertRow)           */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bInsertRow(Entry* pOldRow, Entry** ppRow) {
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* get next entry                                                             */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pNextEntry(Entry* pCurrent, i8 nOffsetNo)
{
  
  i4 iOffset = pCurrent->iNEO(nOffsetNo);
  
  if (-1 == iOffset)
    return nullptr;
  
  char* aAddress = pAdd<char*>(aDataArea(), iOffset);
  Entry* pEntry = reinterpret_cast<Entry*>(aAddress);

  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* position at key as per relational operator                                 */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pPositionQuery(Key* aGivenKey, eRO eRelnOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  bool bIsEqual;
  Entry* pIndexEntry = pPositionQuery(aGivenKey, 1, JC_AFTER);
  
  bool bAfter;
  if (pIndexEntry) {
    do {
      bool bIsEqual = false;
      bAfter = pIndexEntry->aKey()->bJuxtaCompare(JC_AFTER, aGivenKey,
                                                  m_pIndex->pIdxColVec(), 1,
                                                  bIsEqual, eRelnOp);
      if (! bAfter) {
        pIndexEntry = pNextEntry(pIndexEntry, nBufferLevel());
      }
    } while (pIndexEntry && ! bAfter);
  }
  
  return pIndexEntry;
}

/*----------------------------------------------------------------------------*/
/* position at an index buffer at level iLevel starting from this buffer      */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pPositionLevel(Key* aGivenKey, i8 nKeyCols, eJC eJuxtaCompare, i8 iLevel)  //# critical
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  EntryBuffer* pEntryBuffer = this;
  
  i8 iBufferLevel;
  Entry* pEntry;
  
  while (true) {
    iBufferLevel = pEntryBuffer->nBufferLevel();
    
    if (iBufferLevel == iLevel)
      break;
    
    // get the entry
    pEntry = pEntryBuffer->pPositionQuery(aGivenKey, nKeyCols, eJuxtaCompare);
    ASSERT(pEntry);
    
    // get the index buffer of the entry
    pEntryBuffer = pEntry->pEntryBuffer();
    ASSERT(pEntryBuffer);
    ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
  }
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* position at given key                                                      */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pPositionQuery(Key* aGivenKey,i8 nKeyCols, eJC eJuxtaCompare) //# critical
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   * positioning is based on eJuxtaCompare
   *   JC_BEFORE: never called
   *   JC_SAME:   scan for entry which is the same as given key
   *   JC_AFTER:  scan for entry which the given key would be inserted after
   */
  
  ASSERT(JC_SAME == eJuxtaCompare || JC_AFTER == eJuxtaCompare);
  
  bool bIsJuxta;
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  Entry* pScanEntry_sl = m_pHeadEntry;
  bool bIsEqual;
  
  ln nScanLevel;
  for (nScanLevel = 2; *nScanLevel >= 0; nScanLevel--) {
    
    Entry* pNextEntry_sl = pNextEntry(pScanEntry_sl,*nScanLevel);
    while (pNextEntry_sl) {
      
      Key* aNextKey = pNextEntry_sl->aKey();
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aGivenKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
      bIsJuxta =
      aGivenKey->bJuxtaCompare(JC_AFTER,aNextKey,pIdxColVec,nKeyCols,bIsEqual);
      
      // have we found to key during a query?
      if (bIsEqual) {
        if (0 == nScanLevel)
          return pNextEntry_sl;
        else
          break;
      }
      
      // if given key is not same/after next key then break to outer loop
      if (! bIsJuxta)
        break;
      
      pScanEntry_sl = pNextEntry_sl;
      pNextEntry_sl = pNextEntry(pNextEntry_sl,*nScanLevel);
    }
    //pPriorVec[*nScanLevel] = pScanEntry_sl;
  }
  
  if (pScanEntry_sl == pHeadEntry()) {
    pScanEntry_sl = pNextEntry(pScanEntry_sl, 0);
    // we currently think it should be null
    // RASSERT( ! pScanEntry_sl );
  }
  
  // JC_SAME : return the entry which is same as given key
  // JC_AFTER: return the entry which the given key would be inserted after
  return pScanEntry_sl;
}

/*----------------------------------------------------------------------------*/
/* scan thru skip list and positition on entry prior to key                   */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vPositionPrior(Key* aFindKey, Entry* pPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  for (ln nLevel = 0_ln; *nLevel < gpAttachMap->m_nIndexLevels; nLevel++)
    pPriorVec[*nLevel] = m_pHeadEntry;
  
  if (nItems() == 0)
    return;
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  i8 nKeyCols = pIdxColVec->nItems();
  
  bool bIsJuxta;
  bool bIsEqual;

  Entry* pScanEntry_sl = m_pHeadEntry;
  
  ln nScanLevel;
  for (nScanLevel = 2; *nScanLevel >= 0; nScanLevel--) {

    Entry* pNextEntry_sl = pNextEntry(pScanEntry_sl,*nScanLevel);
    while (pNextEntry_sl) {
    
      Key* aNextKey = pNextEntry_sl->aKey();
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
      bIsJuxta =
        aNextKey->bJuxtaCompare(JC_AFTER,aFindKey,pIdxColVec,nKeyCols,bIsEqual);
      
      if (bIsJuxta)
        break;
    
      pScanEntry_sl = pNextEntry_sl;
      pNextEntry_sl = pNextEntry(pNextEntry_sl,*nScanLevel);
    }
    pPriorVec[*nScanLevel] = pScanEntry_sl;
  }
  
}

/*----------------------------------------------------------------------------*/
/* set the prior vector to the head entry                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vPriorVecHead(EntryVec& xPriorVec)
{
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* restore the buffer after being received over a socket                      */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vRestore(void)
{
  vHeadEntryRestore();
  vRowCountRestore();
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vShow(void)
{
  s256 sHigherBuffer;
  if (m_pHigherBuffer)
    StrLib::wToString(((EntryBuffer*)m_pHigherBuffer)->m_nBufferLevel,
                      sHigherBuffer.aChar());
  else
    sHigherBuffer = "..."_Stz;
  
  vTrace("\n"_Stz);
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  vTrace("EntryBuffer: %p (buffer %lld): items: %lld used: %lld freespace %lld\n"_Stz,
         this,*nBufferNo(),nItems(),nUsedBytes(),nFreeBytes());
  
  vTrace("  durability: %s last: %s\n"_Stz,
         aBD[m_eDurability],m_bIsLast ? "true" : "false");
  
  vTrace("  higherBuffer %p (L%s) prevBuffer %p nextBuffer %p\n"_Stz,
         m_pHigherBuffer,sHigherBuffer.aStr(),m_pPrevBuffer,m_pNextBuffer);

  vTrace("  index: %s partition: %lld level: %lld\n"_Stz,
         m_pIndex ? m_pIndex->aIndexName() : "NULL"_Stz,m_nPartitionNo,m_nBufferLevel);
  
  vTrace("  entryOffset: %p freeOffsetHead: %lld\n"_Stz,
         m_pEntryOffset,m_iFreeOffsetHead);

  vTrace("  headEntry: %p[%lld,%lld,%lld] lastEntry: %p firstFree: %p\n"_Stz,
         m_pHeadEntry,m_pHeadEntry ? m_pHeadEntry->iNEO(0) : 0,
                      m_pHeadEntry ? m_pHeadEntry->iNEO(1) : 0,
                      m_pHeadEntry ? m_pHeadEntry->iNEO(2) : 0,
         m_pLastEntry,m_pFirstFree);
  
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  if (m_pHeadEntry) {
    
    vTrace("\n"_Stz);
    vTrace("Entries (by headEntry vector):\n"_Stz);
   
    i8 iOffset = m_pHeadEntry->iNEO(0);
    while (iOffset > -1) {
      Entry* pEntry = pGetEntryOffset(iOffset);
      pEntry->vShow(iOffset,m_pIndex,true);
      iOffset = pEntry->iNEO(0);
    }
  }
  else {
    vTrace("\n"_Stz);
    vTrace("Entries (by EntryOffset array):\n"_Stz);
    for (i8 i_Entry = 0; i_Entry != C_ENTRIES_0; i_Entry++) {

      if (m_pEntryOffset[i_Entry] <= 0)
        continue;

      Entry* pEntry = pGetEntryNo(i_Entry);
      vTrace("%4lld %8lld: "_Stz,i_Entry, nSub(pEntry,m_xWsp.aArea()));
      i8 iOffset = iToOffset(pEntry);
      
      pEntry->vShow(iOffset,m_pIndex,m_pIndex->bIsHidden() ? false : true);
    }
    
    vTrace("\n"_Stz);
    vTrace("Entries (by next offset):\n"_Stz);
    
    for (Entry* pEntry = pFirstEntry(); pEntry; ) {
      
      i8 iOffset = iToOffset(pEntry);
      vTrace("%8lld: "_Stz,iOffset);
      pEntry->vShow(iOffset,m_pIndex,m_pIndex->bIsHidden() ? false : true);
      
      pEntry = pNextEntry(pEntry,0);
    }
  }
  
  if (gpAttachMap->m_nIndexEntries) {
    vTrace("\n"_Stz);
    vTrace("Free Entry Chain:\n"_Stz);
    for (i4 i_Entry = m_iFreeOffsetHead;
         i_Entry != C_ENTRY_NONE;
         i_Entry = - m_pEntryOffset[i_Entry]) {
      vTrace("  %lld: %lld\n"_Stz,
             i_Entry,m_pEntryOffset[i_Entry]);
    }
  }
  
  if (m_pFirstFree) {
    vTrace("\n"_Stz);
    vTrace("FreeEntries:\n"_Stz);
    for (FreeEntry* pFreeEntry = pFirstFree();
         pFreeEntry != nullptr;
         pFreeEntry = pFreeEntry->pNextFree()) {
      vTrace("  %p bytes: %lld next: %p prev: %p\n"_Stz,
             pFreeEntry,pFreeEntry->nSizeItem(),
             pFreeEntry->pNextFree(),pFreeEntry->pPrevFree());
    }
  }
  
  if (m_pHeadEntry) {
    vTrace("\n"_Stz);
    vTrace("By item:\n"_Stz);
    for (Item* pItem = pFirstItem();
         pItem != nullptr;
         pItem = pNextItem(pItem)) {
      
      vTrace(" %p: "_Stz,pItem);
      eCN eClass = pItem->eClass();
      i8 iSize;
      
      switch (eClass) {
        case CN_ENTRY: {
          Entry* pEntryItem = static_cast<Entry*>(pItem);
          iSize = pEntryItem->nSizeEntry() + pEntryItem->iNEOsize();
          vTrace("Entry %lld bytes"_Stz,iSize);
          break;
        }
          
        case CN_FREEENTRY: {
          FreeEntry* pFreeEntryItem = static_cast<FreeEntry*>(pItem);
          iSize = pFreeEntryItem->nSizeItem();
          vTrace("FreeEntry %lld bytes"_Stz,iSize);
          break;
        }
          
        case CN_WILDERNESS:
          iSize = nFreeBytes();
          vTrace("Wilderness (%lld bytes free)"_Stz,iSize);
          break;
          
        default:
          break;
      }
      vTrace("\n"_Stz);
    }
  }
  

  vTrace("\n"_Stz);
  vTrace("=================================================="
         "==================================================\n"_Stz);
  
}

/*----------------------------------------------------------------------------*/
/* split this buffer into two: this original buffer plus a new buffer         */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pSplit(void)
{
  //RASSERT(0);
 
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  EntryBuffer* pNewBuffer = pNewNextBuffer();
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find split entry
  
  i8 iSplitAtEntry = m_nItems/2;
  Entry* pSplitEntry = pGetEntrySplit(iSplitAtEntry);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the prior vector to the split entry
  
  Key* aKey = pSplitEntry->aKey();
  i8 iKeyLength = aKey->nKeySize(pIdxColVec);
  
  Entry* xPriorVec[3];
  vPositionPrior(aKey, xPriorVec);
  //vDumpPriorVec(xPriorVec);
  for (i8 i_Offset = 0; i_Offset < 3; i_Offset++)
    xPriorVec[i_Offset]->vNEO(i_Offset,-1);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // move entries to new buffer
  
  Entry* pMoveEntry = pSplitEntry;
  
  while (pMoveEntry) {
    Entry* pNextEntry = this->pNextEntry(pMoveEntry,0);
    
    i8 iTxId = pMoveEntry->iTxId();
    Key* aKey = pMoveEntry->aKey();
    i8 nKeySize = aKey->nKeySize(pIdxColVec);
    
    Entry* pNewEntry = pNewBuffer->pInsertKey(iTxId,aKey,nKeySize);
    
    vDeleteEntry(pMoveEntry);
    
    pMoveEntry = pNextEntry;
  }
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compact the buffer
  vCompact();
  
  return pNewBuffer;
}

/*----------------------------------------------------------------------------*/
/* truncate this buffer by removing all entries                               */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  EntryBuffer* pNextBuffer;
  
  //----------------------------------------------------------------------------
  // hidden index buffers are just one level
  if (m_pIndex->bIsHidden()) {
    ASSERT(m_nBufferLevel == 0);
    pNextBuffer = this->pNextBuffer();
    
    while (pNextBuffer) {
      ASSERT(pNextBuffer->nBufferLevel() == 0);
      
      EntryBuffer* pThisBuffer = pNextBuffer;
      pNextBuffer = pNextBuffer->pNextBuffer();
      delete pThisBuffer;
    }
    delete this;
    return;
  }
  
  //----------------------------------------------------------------------------
  // other index
  if (m_nBufferLevel != 0) {
    
    // recursively delete the lower level buffers
    for (Entry* pEntry = pFirstEntry(); pEntry != nullptr;
         pEntry = pNextEntry(pEntry, 0)) {
      
      // get the index buffer from the entry
      pNextBuffer = pEntry->pEntryBuffer();;
      
      ASSERT(pNextBuffer);
      ASSERT(pNextBuffer->eClass() == CN_ENTRYBUFFER);
      pNextBuffer->vTruncate();
    }
  }
  
  delete this;
}

/*----------------------------------------------------------------------------*/
/* verify the structure of this buffer                                        */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerify(void) {
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot an entry in this buffer                                              */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pAllotEntry(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Entry* pEntry = reinterpret_cast<Entry*>(pAllotItem(nBytes));
  if ( ! pEntry )
    return nullptr;
  
  new(pEntry) Entry(CN_ENTRY,nBytes,0);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* allot an offset array in this buffer                                       */
/*----------------------------------------------------------------------------*/
NEO* EntryBuffer::aAllotNEO(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  NEO* aNEO = reinterpret_cast<NEO*>(aAllotSpace(nBytes));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return aNEO;
}

/*----------------------------------------------------------------------------*/
/* dump the given prior vector                                                */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDumpPriorVec(Entry* pPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  vTrace("Prior Vector\n"_Stz);
  
    vTrace("  0:%p 1:%p 2:%p\n"_Stz,
           pPriorVec[0],pPriorVec[1],pPriorVec[2]);
}

/*----------------------------------------------------------------------------*/
/* get first item after head (could be entry, free entry or wilderness)       */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::pFirstItem(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Item* pFirstItem = pNextItem(m_pHeadEntry);
  
  return pFirstItem;
}

/*----------------------------------------------------------------------------*/
/* set up the initial offset free chains                                      */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vFreeChains(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 L_ENTRIES_0 = gpAttachMap->m_nIndexEntries ?
  gpAttachMap->m_nIndexEntries : C_ENTRIES_0;
  
  // L0 free chain
  for (i4 i_Entry = 0; i_Entry < (L_ENTRIES_0 - 1); i_Entry++)
    m_pEntryOffset[i_Entry] = - (i_Entry+1);
  m_pEntryOffset[L_ENTRIES_0 - 1] = - C_ENTRY_NONE;
  
  m_iFreeOffsetHead = 0;
}

/*----------------------------------------------------------------------------*/
/* get an entry by offset                                                     */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pGetEntryOffset(i8 iOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  char* aAddress = pAdd<char*>(aDataArea(), iOffset);
  Entry* pEntry = reinterpret_cast<Entry*>(aAddress);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* get split entry by split number                                            */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pGetEntrySplit(i8 iSplitEntryNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(m_pHeadEntry);
  
  i8 iEntryNo = 0;
  for (Entry* pSplitEntry = pNextEntry(m_pHeadEntry,0);
       pSplitEntry;
       pSplitEntry = pNextEntry(pSplitEntry,0)) {
    
    // found split point?
    if (iEntryNo++ == iSplitEntryNo)
      return pSplitEntry;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a free entry (or C_ENTRY_NONE if none)                                 */
/*----------------------------------------------------------------------------*/
i4 EntryBuffer::iGetFreeEntry(void)
{
  if (C_ENTRY_NONE == m_iFreeOffsetHead)
    return C_ENTRY_NONE;
  
  i4 iFreeEntry = m_iFreeOffsetHead;
  
  m_iFreeOffsetHead = - m_pEntryOffset[iFreeEntry];
  
  return iFreeEntry;
}

/*----------------------------------------------------------------------------*/
/* create a head entry                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vHeadEntryNew(void)
{
  RowWrk xHeadWrk;
  
  ASSERT( ! m_pHeadEntry );
  
  xHeadWrk.vOffsets(3);
  
  i8 nEntryBytes = Entry::nBytesRequired(xHeadWrk);
  
  i8 nOffsetBytes = xHeadWrk.nOffsetSize();
  
  i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;
  ASSERT(bHasSpace(nEntryPlusOffsets));
  
  m_pHeadEntry = reinterpret_cast<Entry*>(aAllotSpace(nEntryBytes));
  ASSERT(m_pHeadEntry);
  char* aData = aDataArea();
  char* aHead = pAdd<char *>(aData,sizeof(i4) * C_ENTRIES_0);
  
  ASSERT((char*)m_pHeadEntry == aHead);
  
  new(m_pHeadEntry) Entry(CN_ENTRY,nEntryBytes,0);
  
  m_pHeadEntry->vSetEntry(0,xHeadWrk.xId(),3,nullptr,0);

  NEO* aNEO = aAllotNEO(nOffsetBytes);
  m_pHeadEntry->vNEO(0,-1);
  m_pHeadEntry->vNEO(1,-1);
  m_pHeadEntry->vNEO(2,-1);
}

/*----------------------------------------------------------------------------*/
/* restore the head entry                                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vHeadEntryRestore(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pHeadEntry = pAdd<Entry *>(aDataArea(),sizeof(i4) * C_ENTRIES_0);
}

/*----------------------------------------------------------------------------*/
/* create a new next buffer                                                   */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pNewNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // create new buffer at the same index level and link the two buffers
  EntryBuffer* pNewBuffer = new EntryBuffer();
  
  pNewBuffer->vIndex(m_pIndex);
  pNewBuffer->vBufferLevel(m_nBufferLevel);
  pNewBuffer->vPartitionNo(m_nPartitionNo);
  
  pNewBuffer->vPrevBuffer(this);
  vNextBuffer(pNewBuffer);
  
  return pNewBuffer;
}

/*----------------------------------------------------------------------------*/
/* get next item in buffer (could be entry, free entry, wilderness)           */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::pNextItem(Item* pCurrent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Item* pItemNext = nullptr;
  
  eCN eClass = pCurrent->eClass();
  
  switch (eClass) {
    case CN_ENTRY: {
      Entry* pEntryCurr = static_cast<Entry*>(pCurrent);
      pItemNext = reinterpret_cast<Item*>(pEntryCurr->aAddressAfter());
      break;
    }
      
    case CN_FREEENTRY: {
      FreeEntry* pFreeEntryCurr = static_cast<FreeEntry*>(pCurrent);
      pItemNext = reinterpret_cast<Item*>(pFreeEntryCurr->aAddressAfter());
      break;
    }
      
    case CN_WILDERNESS:
      break;
      
    default:
      //ASSERT(0);
      break;
  }
  
  return pItemNext;
}

/*----------------------------------------------------------------------------*/
/* restore row count                                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vRowCountRestore(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_nItems = 0;
  for (Entry* pRow = pFirstEntry(); pRow; pRow = pNextEntry(pRow, 0))
    m_nItems++;
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_ENTRYBUFFER);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* EntryBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Unit Test ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

void EntryBuffer::vUnitTest(Table* pTable)
{
  
  ColumnVec* pColVec = pTable->pColumnVec();
  Index* pHiddenIndex = pTable->pHiddenIndex();
  
  IdxVec* pIdxVec = pTable->pIdxVec();
  Index* pIndex1 = pIdxVec->pGet(1);
  IdxColVec* pIdxColVec1 = pIndex1->pIdxColVec();
  
  Formatters* pFormatters = &gpAttachMap->m_xFormatters;
  
  EntryBuffer* pFirstBuffer = new EntryBuffer;
  EntryBuffer* pCurrentBuffer = pFirstBuffer;
  pCurrentBuffer->vIndex(pHiddenIndex);
  pCurrentBuffer->vPartitionNo(0);
  //pCurrentBuffer->vShow();
  
  EntryBuffer* pFirstIndexBuffer = new EntryBuffer;
  EntryBuffer* pCurrentIndexBuffer = pFirstIndexBuffer;
  pCurrentIndexBuffer->vIndex(pIndex1);
  pCurrentIndexBuffer->vPartitionNo(0);
  pCurrentIndexBuffer->vShow();
  
  s256 sRecData;
  i8 iRecLength;
  RowWrk xRowEntWrk;
  i8 iRowLength;
  bool bFormatted;
  bool bBlankLine;
  KeyWrk xKeyWrk;
  
  Entry* xKeyEntries[1000];

  // keys in ascending order
  for (i8 iInt = 0; iInt <= 10; iInt++) {
    
  // keys in descending order
  //for (i8 iInt = 100; iInt >= 0; iInt--) {
    
  // keys in random order
  //for (i8 iCount = 0; iCount <= 100; iCount++) {
  //  i8 iInt = Random::iRandom(10);
    
    i8 iKey = iInt;
  
    sprintf(sRecData.aChar(),"%lld,%s,1900-01-01 00:00:%lld",
            iKey,IntLib::sAsText(iInt),iInt);
    vTrace("Record: %s\n"_Stz,sRecData.aChar());
  
    char* aRowData = xRowEntWrk.aData();
    iRecLength = sRecData.iLength();
    
    bFormatted = Row::bFormatRow(pColVec,pFormatters,
                                 sRecData,iRecLength,
                                 (Stz*)aRowData,iRowLength,
                                 bBlankLine);

    xRowEntWrk.vDataSize(iRowLength);
    //((Row*)aRowData)->vDump(pColVec);

    //Entry* pRowEntry = pCurrentBuffer->pAppendRow_v3(xRowEntWrk);
    
    Entry* pRowEntry = pCurrentBuffer->pAppendRow(0, (Stz*)aRowData, iRowLength);
    
    if ( ! pRowEntry ) {
      pCurrentBuffer = pCurrentBuffer->pNewNextBuffer();
      pRowEntry = pCurrentBuffer->pAppendRow(0, (Stz*)aRowData, iRowLength);
      ASSERT(pRowEntry);
    }
    
    xKeyWrk.vReset();
    pRowEntry->vExtractIndexKey(xKeyWrk,pIdxColVec1);
    //xKeyWrk.vShow(pIdxColVec1);
    
    //Entry* pKeyEntry = pCurrentIndexBuffer->pInsertKey_v3(xKeyWrk);
    
    Entry* pKeyEntry = pCurrentIndexBuffer->pInsertKey(0, xKeyWrk.aKey(), xKeyWrk.nKeySize());
    
    xKeyEntries[iKey] = pKeyEntry;
    
//    if ( ! pKeyEntry ) {
//      pCurrentIndexBuffer = pCurrentIndexBuffer->pNewNextBuffer();
//      pKeyEntry = pCurrentIndexBuffer->pInsertKey_v3(xKeyWrk);
//      ASSERT(pKeyEntry);
//    }
    pCurrentIndexBuffer->vShow();
    
  }
  
  pCurrentBuffer->vShow();

  pCurrentIndexBuffer->vShow();
  EntryBuffer* pSplitIndexBuffer = pCurrentIndexBuffer->pSplit();
  pCurrentIndexBuffer->vShow();
  pSplitIndexBuffer->vShow();
  
//  // delete some entries
//  Entry* pEntry;
//  pCurrentIndexBuffer->vShow();
//  
//  pEntry = xKeyEntries[0];
//  pCurrentIndexBuffer->vDeleteEntry(pEntry);
//  pCurrentIndexBuffer->vShow();
//
////  pEntry = xKeyEntries[1];
////  pCurrentIndexBuffer->vDeleteEntry(pEntry);
////  pCurrentIndexBuffer->vShow();
//  
//  pEntry = xKeyEntries[2];
//  pCurrentIndexBuffer->vDeleteEntry(pEntry);
//  pCurrentIndexBuffer->vShow();
//  
////  pEntry = xKeyEntries[4];
////  pCurrentIndexBuffer->vDeleteEntry(pEntry);
////  pCurrentIndexBuffer->vShow();
//
//  pCurrentIndexBuffer->vCompactBuffer();
//  pCurrentIndexBuffer->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
