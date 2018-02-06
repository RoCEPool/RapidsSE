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
EntryBuffer_v3* DBobjects::_pEntryBuffer_v3(void* aAddr)
{
  EntryBuffer_v3* pEntryBuffer = reinterpret_cast<EntryBuffer_v3*>(aAddr);
  
  RASSERT( !pEntryBuffer || CN_ENTRYBUFFER == pEntryBuffer->eClass());
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* EntryBuffer_v3::operator new(size_t iSize)
{
  RASSERT(iSize);
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_ENTRYBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* EntryBuffer_v3::operator new(size_t iSize, void* aAddr)
{
  RASSERT(iSize);
  
  return aAddr;
}

/*----------------------------------------------------------------------------*/
/* return a row buffer to the buffer pool                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::operator delete(void* pRowBufferAddress)
{
  Buffer* pBuffer = reinterpret_cast<Buffer*>(pRowBufferAddress);
  
  Heap::vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer_v3::EntryBuffer_v3(void)
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
  
  m_pEntryOffset0   =
  reinterpret_cast<i4*>(aAllotSpace(sizeof(i4) * C_ENTRIES_0_V3));
  m_nFreeOffsets    = 0;
  m_iFreeOffsetHead = C_ENTRY_NONE_V3;
  vFreeChains();
  
  m_pHeadEntry  = nullptr;
  m_pFirstEntry = nullptr;
  m_pLastEntry  = nullptr;
  m_pFirstFree  = nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_ENTRYBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntryBuffer_v3::~EntryBuffer_v3(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump entry buffer                                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get durability                                                             */
/*----------------------------------------------------------------------------*/
eBD EntryBuffer_v3::eDurability(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_eDurability;
}

/*----------------------------------------------------------------------------*/
/* set durability                                                             */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vDurability(eBD eDurability)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_eDurability = eDurability;
}

/*----------------------------------------------------------------------------*/
/* set buffer is last                                                         */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vIsLast(bool bIsLast)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_bIsLast = bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get if buffer is last                                                      */
/*----------------------------------------------------------------------------*/
bool EntryBuffer_v3::bIsLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get the higher level index buffer                                          */
/*----------------------------------------------------------------------------*/
EntryBuffer_v3* EntryBuffer_v3::pHigherBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pHigherBuffer;
}

/*----------------------------------------------------------------------------*/
/* set the higher level index buffer                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vHigherBuffer(EntryBuffer_v3* pHigherBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //ASSERT( ! m_pHigherBuffer);
  
  m_pHigherBuffer = pHigherBuffer;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer_v3* EntryBuffer_v3::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vNextBuffer(EntryBuffer_v3* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer_v3* EntryBuffer_v3::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vPrevBuffer(EntryBuffer_v3* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set buffer level                                                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vBufferLevel(i8 nBufferLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_nBufferLevel = nBufferLevel;
}

/*----------------------------------------------------------------------------*/
/* get the buffer level                                                       */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer_v3::nBufferLevel(void)
{
  return m_nBufferLevel;
}

/*----------------------------------------------------------------------------*/
/* get first entry                                                            */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v3::pFirstEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pFirstEntry;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
FreeEntry* EntryBuffer_v3::pFirstFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pFirstFree;
}

/*----------------------------------------------------------------------------*/
/* get head entry                                                             */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v3::pHeadEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pHeadEntry;
}

/*----------------------------------------------------------------------------*/
/* set the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pIndex = pIndex;
  
  // create a head entry if we have a non-hidden index
  if (! m_pIndex->bIsHidden() )
    vHeadEntry();
}

/*----------------------------------------------------------------------------*/
/* get the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
Index* EntryBuffer_v3::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pIndex;
}

/*----------------------------------------------------------------------------*/
/* set partition no                                                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vPartitionNo(i8 nPartitionNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_nPartitionNo = nPartitionNo;
}

/*----------------------------------------------------------------------------*/
/* get partition no                                                           */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer_v3::nPartitionNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_nPartitionNo;
}

/*----------------------------------------------------------------------------*/
/*-- Complex Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add given entry to free list                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot an entry in this buffer                                              */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v3::pAllotEntryBytes(i8 nBytes)
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
NEO* EntryBuffer_v3::aAllotNEO(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  NEO* aNEO = reinterpret_cast<NEO*>(aAllotSpace(nBytes));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return aNEO;
}

/*----------------------------------------------------------------------------*/
/* analyze this buffer                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* attempt to append a new row entry into this buffer                         */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v3::pAppendRow_v3(EntWrk_v2& xRowWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(nBufferLevel() == 0);
  
  xRowWrk.vOffsets(1);
  
  //  if (3 == nOffsets)
  //    int iDebug = 1;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if this buffer has enough space
  
  i8 nEntryBytes = Entry::nBytesRequired_v3(xRowWrk);
  
  i8 nOffsetBytes = xRowWrk.nOffsetSize();

  i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;
  if ( ! bHasSpace(nEntryPlusOffsets) )
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the rowId
  
  bn nBufferNo = this->nBufferNo();
  i8 nRowNo = iGetFreeEntry(); //nItems();
  Id xRowId(*nBufferNo, nRowNo);
  
  xRowWrk.vId(xRowId);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate space in this buffer
  
  Entry* pRowEntry = pAllotEntryBytes(nEntryBytes);
  ASSERT(pRowEntry);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the buffer entry
  
  pRowEntry->vSetEntry(xRowWrk);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set entry offsets
  
  i8 iEntryOffset = iToOffsetAbs(pRowEntry);
  m_pEntryOffset0[nRowNo] = iEntryOffset;
  
  NEO* aNEO = aAllotNEO(nOffsetBytes);
  if (m_pLastEntry)
    m_pLastEntry->vNEO(0,iEntryOffset);
  pRowEntry->vNEO(0,-1);
  
  if (! m_pFirstEntry)
    m_pFirstEntry = pRowEntry;
  m_pLastEntry = pRowEntry;
  
  return pRowEntry;
}

/*----------------------------------------------------------------------------*/
/* attempt to append a new row entry into this buffer                         */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v3::pAppendRow_v3a(i8 iTxId, Stz* aRow, i8 iRowLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(nBufferLevel() == 0);
  
  i8 nOffsets = 1;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if this buffer has enough space
  i8 nOffsetBytes = sizeof(i4);
  i8 nEntryBytes = Entry::nBytesRequired_v3(nOffsets, iRowLength);
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
  
  Entry* pRowEntry = pAllotEntryBytes(nEntryBytes);
  ASSERT(pRowEntry);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the buffer entry
  
  pRowEntry->vSetEntry_v3(iTxId, xRowId, nOffsets, aRow, iRowLength);
  pRowEntry->vShow_v2(0, m_pIndex, true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set entry offsets
  
  i8 iEntryOffset = iToOffsetAbs(pRowEntry);
  m_pEntryOffset0[nRowNo] = iEntryOffset;
  
  NEO* aNEO = aAllotNEO(nOffsetBytes);
  if (m_pLastEntry)
    m_pLastEntry->vNEO(0,iEntryOffset);
  pRowEntry->vNEO(0,-1);
  
  if (! m_pFirstEntry)
    m_pFirstEntry = pRowEntry;
  m_pLastEntry = pRowEntry;
  
  return pRowEntry;
}

/*----------------------------------------------------------------------------*/
/* return number of index buffers under this buffer                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compact this buffer                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vCompactBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // scan item is the
  Item* pScanItem = pFirstItem();
  
  // loop while we can still reclaim more space
  for (;;) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // find the first free entry in buffer
    
    FreeEntry* pFirstFree = nullptr;
    for (Item* pItem = pFirstItem();
         pItem != nullptr;
         pItem = pNextItem(pItem)) {
    
      eCN eClass = pItem->eClass();
      
      if (CN_FREEENTRY == eClass) {
        pFirstFree = static_cast<FreeEntry*>(pItem);
        break;
      }
    }
    
    // return early if no free entry because compaction is not possible
    if (! pFirstFree )
      return;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // find the next entry in buffer after the first free entry
    
    FreeEntry* pNextEntry = nullptr;
    for (Item* pItem = pNextItem(pFirstFree);
         pItem != nullptr;
         pItem = pNextItem(pItem)) {
      
      eCN eClass = pItem->eClass();
      
      if (CN_ENTRY == eClass) {
        pFirstFree = static_cast<FreeEntry*>(pItem);
        break;
      }
    }
    
    // return early if no entry because compaction is not possible
    if (! pNextEntry )
      return;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // now copy left
    
    // calc the number of bytes
    // i8 nBytesReclaimed;
    
    //MemLib::vCopy(pDst, pSrc, nBytes, CN_ENTRYBUFFER);
    
    // adjust free bytes in item buffer
    // vGiveSpace(nBytesReclaimed);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move scan
    
  } // for
  
}

/*----------------------------------------------------------------------------*/
/* get decile full of the buffer                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete the given entry                                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vDeleteEntry(Entry* pEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // currently we only delete from non-hidden indexes
  ASSERT(! m_pIndex->bIsHidden());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // remove entry from next offset chains
  
  Entry* xPriorVec[3];

  Key* aKey = pEntry->aKey_v2();
  vPositionPrior_v3(aKey, xPriorVec);
  vDumpPriorVec(xPriorVec);
  
  i8 nOffsets = pEntry->nOffsets();
  for (i8 i_Offset = 0; i_Offset < nOffsets; i_Offset++) {
    i4 iNEO = pEntry->iNEO(i_Offset);
    xPriorVec[i_Offset]->vNEO(i_Offset,iNEO);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // if we are deleting the first entry adjust m_pFirstEntry
  
  if (pEntry == m_pFirstEntry) {
    Entry* pNextEntry = this->pNextEntry(pEntry,0);
    m_pFirstEntry = pNextEntry;
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
}

/*----------------------------------------------------------------------------*/
/* get first item after head (could be entry, free entry or wilderness)       */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer_v3::pFirstItem(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Item* pFirstItem = pNextItem(m_pHeadEntry);
  
  return pFirstItem;
}

/*----------------------------------------------------------------------------*/
/* set up the initial offset free chains                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
bool EntryBuffer_v3::bHasSpace(i8 nBytes)
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
/* attempt to insert a new key entry into this buffer                         */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v3::pInsertKey_v3(KeyWrk_v2& xKeyWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  vTrace("Inserting key:\n"_Stz);
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  xKeyWrk.vShow(pIdxColVec);
  
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
  Key* aKey = xKeyWrk.aKey();
  vPositionPrior_v3(aKey, xPriorVec);
  vDumpPriorVec(xPriorVec);
  
  // set no of offsets in working key
  xKeyWrk.vOffsets(nOffsets); // 1,2 or 3 for the moment
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if this buffer has enough space
  i8 nEntryBytes = Entry::nBytesRequired(xKeyWrk);
  
  i8 nOffsetBytes = xKeyWrk.nOffsetSize();
 
  i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;
  if ( ! bHasSpace(nEntryPlusOffsets) )
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the rowId
  
  bn nBufferNo = this->nBufferNo();
  i8 nRowNo = iGetFreeEntry(); //nItems();
  Id xRowId(*nBufferNo, nRowNo);
  
  xKeyWrk.vId(xRowId);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate space in this buffer
  
  Entry* pKeyEntry = pAllotEntryBytes(nEntryBytes);
  ASSERT(pKeyEntry);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the key entry
  
  pKeyEntry->vSetEntry(xKeyWrk);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set prior offsets
  
  i8 iKeyEntryOffset = iToOffsetAbs(pKeyEntry);
  
  NEO* aNEO = aAllotNEO(nOffsetBytes);
  
  for (i8 iNextOffset = 0; iNextOffset < nOffsets; iNextOffset++) {
    
    i8 iPriorOffset = xPriorVec[iNextOffset]->iNEO(iNextOffset);

    pKeyEntry->vNEO(iNextOffset,iPriorOffset);
    
    xPriorVec[iNextOffset]->vNEO(iNextOffset,iKeyEntryOffset);
  }
  
  return pKeyEntry;
}

/*----------------------------------------------------------------------------*/
/* attempt to insert a new key entry into this buffer                         */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v3::pInsertKey_v3a(i8 iTxId, Key* aKey, i8 iKeyLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  vTrace("Inserting key:\n"_Stz);
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  //xKeyWrk.vShow(pIdxColVec);
  
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
  vPositionPrior_v3(aKey, xPriorVec);
  vDumpPriorVec(xPriorVec);
  
  // set no of offsets in working key
  //xKeyWrk.vOffsets(nOffsets); // 1,2 or 3 for the moment
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if this buffer has enough space
  i8 nEntryBytes = Entry::nBytesRequired_v3(nOffsets, iKeyLength);
  
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
  
  Entry* pKeyEntry = pAllotEntryBytes(nEntryBytes);
  ASSERT(pKeyEntry);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the key entry
  
  //pKeyEntry->vSetEntry(xKeyWrk);
  pKeyEntry->vSetEntry_v3(iTxId, xRowId, nOffsets, (Stz*)aKey, iKeyLength);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set prior offsets
  
  i8 iKeyEntryOffset = iToOffsetAbs(pKeyEntry);
  
  NEO* aNEO = aAllotNEO(nOffsetBytes);
  
  for (i8 iNextOffset = 0; iNextOffset < nOffsets; iNextOffset++) {
    
    i8 iPriorOffset = xPriorVec[iNextOffset]->iNEO(iNextOffset);
    
    pKeyEntry->vNEO(iNextOffset,iPriorOffset);
    
    xPriorVec[iNextOffset]->vNEO(iNextOffset,iKeyEntryOffset);
  }
  
  return pKeyEntry;
}

/*----------------------------------------------------------------------------*/
/* create a new next buffer                                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get next entry                                                             */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v3::pNextEntry(Entry* pCurrent, i8 nOffsetNo)
{
  
  i4 iOffset = pCurrent->iNEO(nOffsetNo);
  
  if (-1 == iOffset)
    return nullptr;
  
  char* aAddress = pAdd<char*>(aDataArea(), iOffset);
  Entry* pEntry = reinterpret_cast<Entry*>(aAddress);

  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* get next item in buffer (could be entry, free entry, wilderness)           */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer_v3::pNextItem(Item* pCurrent)
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
      ASSERT(0);
  }
  
  return pItemNext;
}

/*----------------------------------------------------------------------------*/
/* scan thru skip list and positition on entry prior to key                   */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vPositionPrior_v3(Key* aFindKey, Entry* pPriorVec[])
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
    
      Key* aNextKey = pNextEntry_sl->aKey_v2();
      
      aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
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
//void EntryBuffer_v3::vPositionPrior_v3(Key* aFindKey, Entry* pPriorVec[])
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
//  
//  for (ln nLevel = 0_ln; *nLevel < gpAttachMap->m_nIndexLevels; nLevel++)
//    pPriorVec[*nLevel] = m_pHeadEntry;
//  
//  if (nItems() == 0)
//    return;
//  
//  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
//  i8 nKeyCols = pIdxColVec->nItems();
//  
//  bool bIsJuxta;
//  bool bIsEqual;
//  
//  ln nScanLevel;
//  for (nScanLevel = 2; *nScanLevel >= 0; nScanLevel--) {
//    
//    Entry* pNextEntry_sl = pNextEntry(m_pHeadEntry,*nScanLevel);
//    while (pNextEntry_sl) {
//      
//      Key* aNextKey = pNextEntry_sl->aKey_v2();
//      
//      aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
//      aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
//      
//      bIsJuxta =
//      aNextKey->bJuxtaCompare(JC_AFTER,aFindKey,pIdxColVec,nKeyCols,bIsEqual);
//      
//      if (bIsJuxta)
//        break;
//      
//      pPriorVec[*nScanLevel] = pNextEntry_sl;
//      
//      pNextEntry_sl = pNextEntry(pNextEntry_sl,*nScanLevel);
//    }
//  }
//  
//}

/*----------------------------------------------------------------------------*/
/* remove given entry                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* remove entry from entry offset arrays                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vShow(void)
{
  s256 sHigherBuffer;
  if (m_pHigherBuffer)
    StrLib::wToString(((EntryBuffer_v3*)m_pHigherBuffer)->m_nBufferLevel,
                      sHigherBuffer.aChar());
  else
    sHigherBuffer = "..."_Stz;
  
  vTrace("\n"_Stz);
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  vTrace("EntryBuffer_v3: %p (buffer %lld): items: %lld used: %lld freespace %lld\n"_Stz,
         this,*nBufferNo(),nItems(),nUsedBytes(),nFreeBytes());
  
  vTrace("  durability: %s last: %s\n"_Stz,
         aBD[m_eDurability],m_bIsLast ? "true" : "false");
  
  vTrace("  higherBuffer %p (L%s) prevBuffer %p nextBuffer %p\n"_Stz,
         m_pHigherBuffer,sHigherBuffer.aStr(),m_pPrevBuffer,m_pNextBuffer);

  vTrace("  index: %s partition: %lld level: %lld\n"_Stz,
         m_pIndex->aIndexName(),m_nPartitionNo,m_nBufferLevel);
  
  vTrace("  entryOffset0: %p m_nFreeOffsets: %lld freeOffsetHead: %lld\n"_Stz,
         m_pEntryOffset0,m_nFreeOffsets,m_iFreeOffsetHead);

  vTrace("  headEntry: %p[%lld,%lld,%lld] firstEntry: %p lastEntry: %p firstFree: %p\n"_Stz,
         m_pHeadEntry,m_pHeadEntry ? m_pHeadEntry->iNEO(0) : 0,
                      m_pHeadEntry ? m_pHeadEntry->iNEO(1) : 0,
                      m_pHeadEntry ? m_pHeadEntry->iNEO(2) : 0,
         m_pFirstEntry,m_pLastEntry,m_pFirstFree);
  
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  if (m_pHeadEntry) {
    
    vTrace("\n"_Stz);
    vTrace("Entries (by headEntry vector):\n"_Stz);
   
    i8 iOffset = m_pHeadEntry->iNEO(0);
    while (iOffset > -1) {
      Entry* pEntry = pGetEntry(iOffset);
      pEntry->vShow_v3(iOffset,m_pIndex,true);
      iOffset = pEntry->iNEO(0);
    }
  }
  else {
    vTrace("\n"_Stz);
    vTrace("Entries (by EntryOffset array):\n"_Stz);
    for (i8 i_Entry = 0; i_Entry != C_ENTRIES_0_V3; i_Entry++) {

      if (m_pEntryOffset0[i_Entry] <= 0)
        continue;

      Entry* pEntry = pGetEntry(i_Entry, 0_ln);
      vTrace("%4lld %8lld: "_Stz,i_Entry, nSub(pEntry,m_xWsp.aArea()));
      i8 iOffset = iToOffsetAbs(pEntry);
      pEntry->vShow_v3(iOffset,m_pIndex,true);
    }
    
    vTrace("\n"_Stz);
    vTrace("Entries (by next offset):\n"_Stz);
    
    for (Entry* pEntry = m_pFirstEntry; pEntry; ) {
      
      i8 iOffset = iToOffsetAbs(pEntry);
      vTrace("%8lld: "_Stz,iOffset);
      pEntry->vShow_v3(iOffset,m_pIndex,true);
      
      pEntry = pNextEntry(pEntry,0);
    }
  }
  
  if (gpAttachMap->m_nIndexEntries) {
    vTrace("\n"_Stz);
    vTrace("Free Entry Chain:\n"_Stz);
    for (i4 i_Entry = m_iFreeOffsetHead;
         i_Entry != C_ENTRY_NONE_V3;
         i_Entry = - m_pEntryOffset0[i_Entry]) {
      vTrace("  %lld: %lld\n"_Stz,
             i_Entry,m_pEntryOffset0[i_Entry]);
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

/*----------------------------------------------------------------------------*/
/* truncate this buffer by removing all entries                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* verify the structure of this buffer                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compact this buffer                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy entries from fromBuffer starting at fromEntry to this buffer          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump the given prior vector                                                */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vDumpPriorVec(Entry* pPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  vTrace("Prior Vector\n"_Stz);
  
    vTrace("  0:%p 1:%p 2:%p\n"_Stz,
           pPriorVec[0],pPriorVec[1],pPriorVec[2]);
}


/*----------------------------------------------------------------------------*/
/* dump the prior vector                                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* fix the higher level buffers                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set up the initial offset free chains                                      */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vFreeChains(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 L_ENTRIES_0 = gpAttachMap->m_nIndexEntries ?
  gpAttachMap->m_nIndexEntries : C_ENTRIES_0_V3;
  
  // L0 free chain
  for (i4 i_Entry = 0; i_Entry < (L_ENTRIES_0 - 1); i_Entry++)
    m_pEntryOffset0[i_Entry] = - (i_Entry+1);
  m_pEntryOffset0[L_ENTRIES_0 - 1] = - C_ENTRY_NONE;
  
  m_iFreeOffsetHead = 0;
  m_nFreeOffsets = L_ENTRIES_0;
}

/*----------------------------------------------------------------------------*/
/* get an entry by offset                                                     */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v3::pGetEntry(i8 iOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  char* aAddress = pAdd<char*>(aDataArea(), iOffset);
  Entry* pEntry = reinterpret_cast<Entry*>(aAddress);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* get an entry by number                                                     */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v3::pGetEntry(i8 iEntryNo, ln nLevelNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (iEntryNo == C_ENTRY_NONE_V3)
    return nullptr;
  
  Entry* pEntry = nullptr;
  char* aAddress;
  i4 iOffset;
  
  switch (nLevelNo) {
    case 0:
      iOffset = m_pEntryOffset0[iEntryNo];
      aAddress = pAdd<char*>(aDataArea(), iOffset);
      pEntry = reinterpret_cast<Entry*>(aAddress);
      break;
//    case 1:
//      iOffset = m_pEntryOffset1[iEntryNo].m_iOffset;
//      aAddress = pAdd<char*>(aDataArea(), iOffset);
//      pEntry = reinterpret_cast<Entry*>(aAddress);
//      break;
//    case 2:
//      iOffset = m_pEntryOffset2[iEntryNo].m_iOffset;
//      aAddress = pAdd<char*>(aDataArea(), iOffset);
//      pEntry = reinterpret_cast<Entry*>(aAddress);
//      break;
    default:
      ASSERT(0);
  }
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* get an entry by row number                                                 */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v3::pGetEntryRow(i8 iRowNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 iOffset = m_pEntryOffset0[iRowNo];
  
  if (iOffset < 0)
    return nullptr;
  
  Entry* pEntry = pGetEntry(iOffset);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* get split entry by number                                                  */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v3::pGetSplitEntry(i8 iSplitEntryNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 iEntryNo = 0;
  for (Entry* pSplitEntry = pFirstEntry();
       pSplitEntry;
       pSplitEntry = this->pNextEntry(pSplitEntry,0)) {
    
    // found split point?
    if (iEntryNo++ == iSplitEntryNo)
      return pSplitEntry;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a free entry (or C_ENTRY_NONE if none)                                 */
/*----------------------------------------------------------------------------*/
i4 EntryBuffer_v3::iGetFreeEntry(void)
{
  if (C_ENTRY_NONE_V3 == m_iFreeOffsetHead)
    return C_ENTRY_NONE_V3;
  
  i4 iFreeEntry = m_iFreeOffsetHead;
  
  m_iFreeOffsetHead = - m_pEntryOffset0[iFreeEntry];
  
  return iFreeEntry;
}

/*----------------------------------------------------------------------------*/
/* create a head entry                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vHeadEntry(void)
{
  RowWrk_v2 xHeadWrk;
  
  xHeadWrk.vOffsets(3);
  
  i8 nEntryBytes = Entry::nBytesRequired_v3(xHeadWrk);
  
  i8 nOffsetBytes = xHeadWrk.nOffsetSize();
  
  i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;
  ASSERT(bHasSpace(nEntryPlusOffsets));
  
  m_pHeadEntry = reinterpret_cast<Entry*>(aAllotSpace(nEntryBytes));
  ASSERT(m_pHeadEntry);
  new(m_pHeadEntry) Entry(CN_ENTRY,nEntryBytes,0);
  
  m_pHeadEntry->vSetEntry(xHeadWrk);

  NEO* aNEO = aAllotNEO(nOffsetBytes);
  m_pHeadEntry->vNEO(0,-1);
  m_pHeadEntry->vNEO(1,-1);
  m_pHeadEntry->vNEO(2,-1);
}

/*----------------------------------------------------------------------------*/
/* create a new next buffer                                                   */
/*----------------------------------------------------------------------------*/
EntryBuffer_v3* EntryBuffer_v3::pNewNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // create new buffer at the same index level and link the two buffers
  EntryBuffer_v3* pNewBuffer = new EntryBuffer_v3();
  
  pNewBuffer->vIndex(m_pIndex);
  pNewBuffer->vBufferLevel(m_nBufferLevel);
  pNewBuffer->vPartitionNo(m_nPartitionNo);
  
  pNewBuffer->vPrevBuffer(this);
  vNextBuffer(pNewBuffer);
  
  return pNewBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the prior vector of given entry                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* replace old key with new key in the index buffer                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get right entry at given level (returns null if no right)                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate the absolute offset within workspace for given entry             */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer_v3::iToOffsetAbs(Entry* pEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 iOffset = nSub(pEntry,m_xWsp.aArea());
  
  ASSERT(iOffset >= 0 && iOffset < m_xWsp.nSize());
  
  return iOffset;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* EntryBuffer_v3::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Unit Test ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

void DBobjects::vEntryBuffer_v3_UnitTest(Table* pTable)
{
  
  ColumnVec* pColVec = pTable->pColumnVec();
  Index* pHiddenIndex = pTable->pHiddenIndex();
  
  IdxVec* pIdxVec = pTable->pIdxVec();
  Index* pIndex1 = pIdxVec->pGet(1);
  IdxColVec* pIdxColVec1 = pIndex1->pIdxColVec();
  
  Formatters* pFormatters = &gpAttachMap->m_xFormatters;
  
  EntryBuffer_v3* pFirstBuffer = new EntryBuffer_v3;
  EntryBuffer_v3* pCurrentBuffer = pFirstBuffer;
  pCurrentBuffer->vIndex(pHiddenIndex);
  pCurrentBuffer->vPartitionNo(0);
  //pCurrentBuffer->vShow();
  
  EntryBuffer_v3* pFirstIndexBuffer = new EntryBuffer_v3;
  EntryBuffer_v3* pCurrentIndexBuffer = pFirstIndexBuffer;
  pCurrentIndexBuffer->vIndex(pIndex1);
  pCurrentIndexBuffer->vPartitionNo(0);
  pCurrentIndexBuffer->vShow();
  
  s256 sRecData;
  i8 iRecLength;
  RowWrk_v2 xRowEntWrk;
  i8 iRowLength;
  bool bFormatted;
  bool bBlankLine;
  KeyWrk_v2 xKeyWrk;
  
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
  
    char* aRowData = xRowEntWrk.aData_v2();
    iRecLength = sRecData.iLength();
    
    bFormatted = Row::bFormatRow(pColVec,pFormatters,
                                 sRecData,iRecLength,
                                 (Stz*)aRowData,iRowLength,
                                 bBlankLine);

    xRowEntWrk.vDataSize(iRowLength);
    ((Row*)aRowData)->vDump(pColVec);

    //Entry* pRowEntry = pCurrentBuffer->pAppendRow_v3(xRowEntWrk);
    
    Entry* pRowEntry = pCurrentBuffer->pAppendRow_v3a(0, (Stz*)aRowData, iRowLength);
    
    if ( ! pRowEntry ) {
      pCurrentBuffer = pCurrentBuffer->pNewNextBuffer();
      pRowEntry = pCurrentBuffer->pAppendRow_v3a(0, (Stz*)aRowData, iRowLength);
      ASSERT(pRowEntry);
    }
    
    xKeyWrk.vReset();
    pRowEntry->vExtractIndexKey_v2(xKeyWrk,pIdxColVec1);
    //xKeyWrk.vShow(pIdxColVec1);
    
    //Entry* pKeyEntry = pCurrentIndexBuffer->pInsertKey_v3(xKeyWrk);
    
    Entry* pKeyEntry = pCurrentIndexBuffer->pInsertKey_v3a(0, xKeyWrk.aKey(), xKeyWrk.nKeySize());
    
    xKeyEntries[iKey] = pKeyEntry;
    
//    if ( ! pKeyEntry ) {
//      pCurrentIndexBuffer = pCurrentIndexBuffer->pNewNextBuffer();
//      pKeyEntry = pCurrentIndexBuffer->pInsertKey_v3(xKeyWrk);
//      ASSERT(pKeyEntry);
//    }
    pCurrentIndexBuffer->vShow();
    
  }
  
  pCurrentBuffer->vShow();
  
  // delete some entries
  pCurrentIndexBuffer->vShow();
  Entry* pEntry = xKeyEntries[0];
  pCurrentIndexBuffer->vDeleteEntry(pEntry);
  pCurrentIndexBuffer->vShow();
  
  pEntry = xKeyEntries[2];
  pCurrentIndexBuffer->vDeleteEntry(pEntry);
  pCurrentIndexBuffer->vShow();
  
  pEntry = xKeyEntries[4];
  pCurrentIndexBuffer->vDeleteEntry(pEntry);
  pCurrentIndexBuffer->vShow();
  
  pCurrentIndexBuffer->vCompactBuffer();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
