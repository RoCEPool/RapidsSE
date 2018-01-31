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
EntryBuffer_v2* DBobjects::_pEntryBuffer_v2(void* aAddr)
{
  EntryBuffer_v2* pEntryBuffer = reinterpret_cast<EntryBuffer_v2*>(aAddr);
  
  RASSERT( !pEntryBuffer || CN_ENTRYBUFFER == pEntryBuffer->eClass());
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* EntryBuffer_v2::operator new(size_t iSize)
{
  RASSERT(iSize);
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_ENTRYBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* EntryBuffer_v2::operator new(size_t iSize, void* aAddr)
{
  RASSERT(iSize);
  
  return aAddr;
}

/*----------------------------------------------------------------------------*/
/* return a row buffer to the buffer pool                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::operator delete(void* pRowBufferAddress)
{
  Buffer* pBuffer = reinterpret_cast<Buffer*>(pRowBufferAddress);
  
  Heap::vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer_v2::EntryBuffer_v2(void)
: ItemBuffer(CN_ENTRYBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_eDurability   = BD_PERMANENT;
  m_bIsLast       = false;
  
  m_pHigherBuffer = nullptr;
  m_pNextBuffer   = nullptr;
  m_pPrevBuffer   = nullptr;

  m_pIndex        = nullptr;
  m_nBufferLevel  = 0;
  m_nPartitionNo  = -1;
  
  m_pEntryOffset2 =
    reinterpret_cast<Offset*>(aAllotSpace(sizeof(Offset) * C_ENTRIES_2));
  m_pEntryOffset1 =
    reinterpret_cast<Offset*>(aAllotSpace(sizeof(Offset) * C_ENTRIES_1));
  m_pEntryOffset0 =
    reinterpret_cast<i4*>(aAllotSpace(sizeof(i4) * C_ENTRIES_0));
  
  m_nEntryOffset2 = 0;
  m_nEntryOffset1 = 0;
  
  m_iHeadEntry2  = C_ENTRY_NONE;
  m_iHeadEntry1  = C_ENTRY_NONE;
  m_iHeadEntry0  = C_ENTRY_NONE;
  vFreeChains();
  
  m_pEntries = _pEntry(m_xWsp.aLatter());
  
  m_nReclaimSpace = 0;
  m_pFirstFree    = nullptr;
  m_pLastFree     = nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_ENTRYBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntryBuffer_v2::~EntryBuffer_v2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump entry buffer                                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get durability                                                             */
/*----------------------------------------------------------------------------*/
eBD EntryBuffer_v2::eDurability(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_eDurability;
}

/*----------------------------------------------------------------------------*/
/* set durability                                                             */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vDurability(eBD eDurability)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_eDurability = eDurability;
}

/*----------------------------------------------------------------------------*/
/* set buffer is last                                                         */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vIsLast(bool bIsLast)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_bIsLast = bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get if buffer is last                                                      */
/*----------------------------------------------------------------------------*/
bool EntryBuffer_v2::bIsLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get the higher level index buffer                                          */
/*----------------------------------------------------------------------------*/
EntryBuffer_v2* EntryBuffer_v2::pHigherBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pHigherBuffer;
}

/*----------------------------------------------------------------------------*/
/* set the higher level index buffer                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vHigherBuffer(EntryBuffer_v2* pHigherBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //ASSERT( ! m_pHigherBuffer);
  
  m_pHigherBuffer = pHigherBuffer;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer_v2* EntryBuffer_v2::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vNextBuffer(EntryBuffer_v2* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer_v2* EntryBuffer_v2::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vPrevBuffer(EntryBuffer_v2* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set buffer level                                                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vBufferLevel(i8 nBufferLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_nBufferLevel = nBufferLevel;
}

/*----------------------------------------------------------------------------*/
/* get the buffer level                                                       */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer_v2::nBufferLevel(void)
{
  return m_nBufferLevel;
}

/*----------------------------------------------------------------------------*/
/* set the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pIndex = pIndex;
}

/*----------------------------------------------------------------------------*/
/* get the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
Index* EntryBuffer_v2::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pIndex;
}

/*----------------------------------------------------------------------------*/
/* set partition no                                                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vPartitionNo(i8 nPartitionNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_nPartitionNo = nPartitionNo;
}

/*----------------------------------------------------------------------------*/
/* get partition no                                                           */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer_v2::nPartitionNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_nPartitionNo;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
FreeEntry* EntryBuffer_v2::pFirstFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pFirstFree;
}

/*----------------------------------------------------------------------------*/
/*-- Complex Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add given entry to free list                                               */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vAddFree(Entry *pEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nSizeEntry = pEntry->nSizeItem();
  
  FreeEntry* pFreeEntry = new(pEntry) FreeEntry(nSizeEntry);
  
  if ( ! m_pFirstFree ) {
    m_pFirstFree = pFreeEntry;
    m_pLastFree = pFreeEntry;
    return;
  }
  
  if (pFreeEntry < m_pFirstFree) {
    pFreeEntry->vNextFree(m_pFirstFree);
    m_pFirstFree->vPrevFree(pFreeEntry);
    m_pFirstFree = pFreeEntry;
  }
  
  if (pFreeEntry > m_pLastFree) {
    pFreeEntry->vPrevFree(m_pLastFree);
    m_pLastFree->vNextFree(pFreeEntry);
    m_pLastFree = pFreeEntry;
  }
  
  FreeEntry* pNextFree;
  for (FreeEntry* pScanFree = m_pFirstFree;
       pScanFree;
       pScanFree = pScanFree->pNextFree()) {
    
    pNextFree = pScanFree->pNextFree();
    ASSERT(pNextFree);
    
    if (pFreeEntry > pScanFree && pFreeEntry < pNextFree) {
      
      pScanFree->vNextFree(pFreeEntry);
      pFreeEntry->vPrevFree(pScanFree);
      
      pNextFree->vPrevFree(pFreeEntry);
      pFreeEntry->vNextFree(pScanFree);
      
      return;
    }
  }
  
  ASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* allot an entry in this buffer                                              */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v2::pAllotEntryBytes(i8 nBytes)
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
/* analyze this buffer                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* attempt to append a new row entry into this buffer                         */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v2::pAppendRow_v2(EntWrk_v2& xRowWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(nBufferLevel() == 0);
  
  i8 nOffsets =
    Random::iLogLevel(gpAttachMap->m_nIndexOffsets+1,C_FLAT_FREQ) + 1;
  xRowWrk.vOffsets(nOffsets);
  
//  if (3 == nOffsets)
//    int iDebug = 1;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if this buffer has enough space
  
  i8 nEntryBytes = Entry::nBytesRequired(xRowWrk);
  
  if ( ! bHasSpace(nEntryBytes) )
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the rowId
  
  bn nBufferNo = this->nBufferNo();
  i8 nRowNo = nItems();
  Id xRowId(*nBufferNo, nRowNo);
  
  xRowWrk.vId(xRowId);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate space in this buffer
  
  Entry* pBufferEntry = pAllotEntryBytes(nEntryBytes);
  ASSERT(pBufferEntry)
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the buffer entry
  
  pBufferEntry->vSetEntry(xRowWrk);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set entry offsets
  
  i8 iEntryOffset = iToOffsetAbs(pBufferEntry);
  m_pEntryOffset0[nRowNo] = iEntryOffset;
  
  if (nOffsets >= 2) {
    Offset* pOffset1 = &m_pEntryOffset1[m_nEntryOffset1];
    pOffset1->m_iEntryNo = nRowNo;
    pOffset1->m_iOffset = iEntryOffset;
    m_nEntryOffset1++;
  }
  
  if (nOffsets == 3) {
    Offset* pOffset2 = &m_pEntryOffset2[m_nEntryOffset2];
    pOffset2->m_iEntryNo = nRowNo;
    pOffset2->m_iOffset = iEntryOffset;
    m_nEntryOffset2++;
  }
  
  return pBufferEntry;
}

/*----------------------------------------------------------------------------*/
/* return number of index buffers under this buffer                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get decile full of the buffer                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get first entry                                                            */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v2::pFirstEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // the location of the possible first entry set in constructor
  ASSERT(m_pEntries);
  
  // first possible first entry
  Entry* pFirst = m_pEntries;
  
  // check for valid entry
  Entry* pLatter = _pEntry(m_xWsp.aLatter());
  if (pFirst < pLatter)
    return pFirst;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* set up the initial offset free chains                                      */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vFreeChains(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // L2 free chain
  for (i4 i_Entry = 0; i_Entry < (C_ENTRIES_2 - 1); i_Entry++) {
    m_pEntryOffset2[i_Entry].m_iEntryNo = i_Entry;
    m_pEntryOffset2[i_Entry].m_iNextEntry = - (i_Entry+1);
  }
  m_pEntryOffset2[C_ENTRIES_2 - 1].m_iEntryNo = C_ENTRIES_2 - 1;
  m_pEntryOffset2[C_ENTRIES_2 - 1].m_iNextEntry = - C_ENTRY_NONE;
  m_iHeadEntry2 = 0;
  
  // L1 free chain
  for (i4 i_Entry = 0; i_Entry < (C_ENTRIES_1 - 1); i_Entry++) {
    m_pEntryOffset1[i_Entry].m_iEntryNo = i_Entry;
    m_pEntryOffset1[i_Entry].m_iNextEntry = - (i_Entry+1);
  }
  m_pEntryOffset1[C_ENTRIES_1 - 1].m_iEntryNo = C_ENTRIES_1 - 1;
  m_pEntryOffset1[C_ENTRIES_1 - 1].m_iNextEntry  = - C_ENTRY_NONE;
  m_iHeadEntry1 = 0;
  
  // L0 free chain
  for (i4 i_Entry = 0; i_Entry < (C_ENTRIES_0 - 1); i_Entry++) {
    m_pEntryOffset0[i_Entry] = - (i_Entry+1);
  }
  m_pEntryOffset0[C_ENTRIES_0 - 1] = - C_ENTRY_NONE;
  m_iHeadEntry0 = 0;
}

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
bool EntryBuffer_v2::bHasSpace(i8 nBytes)
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
Entry* EntryBuffer_v2::pInsertKey_v2(EntWrk_v2& xRowWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* create a new next buffer                                                   */
/*----------------------------------------------------------------------------*/
EntryBuffer_v2* EntryBuffer_v2::pNewNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // create new buffer at the same index level and link the two buffers
  EntryBuffer_v2* pNewBuffer = new EntryBuffer_v2();
  
  pNewBuffer->vIndex(m_pIndex);
  pNewBuffer->vBufferLevel(m_nBufferLevel);
  pNewBuffer->vPartitionNo(m_nPartitionNo);
  
  pNewBuffer->vPrevBuffer(this);
  vNextBuffer(pNewBuffer);
  
  return pNewBuffer;
}

/*----------------------------------------------------------------------------*/
/* get next entry                                                             */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v2::pNextEntry(Entry* pCurrent)
{
  // get the size of given entry
  i8 nBytes = pCurrent->nSizeEntry();
  
  // compute the address of possible next entry
  Entry* pNext = pAdd<Entry*>(pCurrent,nBytes);
  
  // check for valid entry
  Entry* pLatter = _pEntry(m_xWsp.aLatter());
  if (pNext < pLatter)
    return pNext;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* scan thru skip list and positition on entry prior to key                   */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vPositionPrior_v2(Key* aFindKey, EntryVec* pPriorVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* remove given entry                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* remove entry from entry offset arrays                                      */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vRemoveFree(Entry *pEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 i_RowNo = pEntry->iRow();
  i8 nOffsets = pEntry->nOffsets();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // remove from entry offset 0
  
  if (C_ENTRY_NONE == m_iHeadEntry0) {
    m_pEntryOffset0[i_RowNo] = - C_ENTRY_NONE;
    m_iHeadEntry0 = i_RowNo;
  }
  else if (i_RowNo < m_iHeadEntry0) {
    m_pEntryOffset0[i_RowNo] = - m_iHeadEntry0;
    m_iHeadEntry0 = i_RowNo;
  }
  else {
    for (i4 i_Entry = m_iHeadEntry0;
         i_Entry != C_ENTRY_NONE;
         i_Entry = - m_pEntryOffset0[i_Entry]) {
      
      i4 iNextEntry = - m_pEntryOffset0[i_Entry];
      if (i_Entry < i_RowNo &&
            (iNextEntry > i_RowNo || iNextEntry == C_ENTRY_NONE)) {
        m_pEntryOffset0[i_Entry] = - i_RowNo;
        m_pEntryOffset0[i_RowNo] = - iNextEntry;
        break;
      }
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // remove from entry offset 1
  
  if (nOffsets == 1)
    return;

  if (C_ENTRY_NONE == m_iHeadEntry1) {
    m_pEntryOffset1[i_RowNo].m_iEntryNo   = i_RowNo;
    m_pEntryOffset1[i_RowNo].m_iNextEntry = - C_ENTRY_NONE;
    m_iHeadEntry1 = i_RowNo;
  }
  else if (i_RowNo < m_iHeadEntry1) {
    m_pEntryOffset1[i_RowNo].m_iEntryNo   = i_RowNo;
    m_pEntryOffset1[i_RowNo].m_iNextEntry = - m_iHeadEntry1;
    m_iHeadEntry1 = i_RowNo;
  }
  else {
    for (i4 i_Entry = m_iHeadEntry1;
         i_Entry != C_ENTRY_NONE;
         i_Entry = - m_pEntryOffset1[i_Entry].m_iNextEntry) {
      
      i4 iNextEntry = - m_pEntryOffset1[i_Entry].m_iNextEntry;
      if (i_Entry < i_RowNo &&
          (iNextEntry > i_RowNo || iNextEntry == C_ENTRY_NONE)) {
        m_pEntryOffset1[i_Entry].m_iNextEntry = - i_RowNo;
        m_pEntryOffset1[i_RowNo].m_iEntryNo   = i_RowNo;
        m_pEntryOffset1[i_RowNo].m_iNextEntry = - iNextEntry;
        break;
      }
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // remove from entry offset 2
  
  if (nOffsets == 2)
    return;
  
  if (C_ENTRY_NONE == m_iHeadEntry2) {
    m_pEntryOffset2[i_RowNo].m_iEntryNo   = i_RowNo;
    m_pEntryOffset2[i_RowNo].m_iNextEntry = - C_ENTRY_NONE;
    m_iHeadEntry2 = i_RowNo;
  }
  else if (i_RowNo < m_iHeadEntry2) {
    m_pEntryOffset2[i_RowNo].m_iEntryNo   = i_RowNo;
    m_pEntryOffset2[i_RowNo].m_iNextEntry = - m_iHeadEntry2;
    m_iHeadEntry2 = i_RowNo;
  }
  else {
    for (i4 i_Entry = m_iHeadEntry2;
         i_Entry != C_ENTRY_NONE;
         i_Entry = - m_pEntryOffset2[i_Entry].m_iNextEntry) {
      
      i4 iNextEntry = - m_pEntryOffset2[i_Entry].m_iNextEntry;
      if (i_Entry < i_RowNo &&
          (iNextEntry > i_RowNo || iNextEntry == C_ENTRY_NONE)) {
        m_pEntryOffset2[i_Entry].m_iNextEntry = - i_RowNo;
        m_pEntryOffset2[i_RowNo].m_iEntryNo   = i_RowNo;
        m_pEntryOffset2[i_RowNo].m_iNextEntry = - iNextEntry;
        break;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vShow(void)
{
  s256 sHigherBuffer;
  if (m_pHigherBuffer)
    StrLib::wToString(((EntryBuffer_v2*)m_pHigherBuffer)->m_nBufferLevel,
                      sHigherBuffer.aChar());
  else
    sHigherBuffer = "..."_Stz;
  
  vTrace("EntryBuffer %p (buffer %lld): %s index %s partition %lld level L%lld\n"
         "  HigherBuffer %p (L%s) entries %lld prevBuffer %p nextBuffer %p\n"
         "  ReclaimSpace: %lld FirstFree: %p LastFree %p\n"_Stz,
         this,
         *nBufferNo(),
         aBD[m_eDurability],
         m_pIndex->aIndexName(),
         m_nPartitionNo,
         m_nBufferLevel,
         m_pHigherBuffer,
         sHigherBuffer.aStr(),
         m_nItems,
         m_pPrevBuffer,
         m_pNextBuffer,
         m_nReclaimSpace,
         m_pFirstFree,
         m_pLastFree);
  
  vTrace("Entry Offsets 0:\n"_Stz);
  for (i8 i_Entry = 0; i_Entry < m_nItems; i_Entry++)
    vTrace("  [%2lld]: %lld\n"_Stz,i_Entry,m_pEntryOffset0[i_Entry]);

  vTrace("Entry Offsets 1:\n"_Stz);
  for (i8 i_Entry = 0; i_Entry < m_nEntryOffset1; i_Entry++)
    vTrace("  [%2lld]: %lld %lld\n"_Stz,
           i_Entry,m_pEntryOffset1[i_Entry].m_iEntryNo,
                  m_pEntryOffset1[i_Entry].m_iOffset);
  
  vTrace("Entry Offsets 2:\n"_Stz);
  for (i8 i_Entry = 0; i_Entry < m_nEntryOffset2; i_Entry++)
    vTrace("  [%2lld]: %lld %lld\n"_Stz,
           i_Entry,m_pEntryOffset2[i_Entry].m_iEntryNo,
                  m_pEntryOffset2[i_Entry].m_iOffset);
  
  vTrace("\n"_Stz);
  vTrace("Entries:\n"_Stz);
  for (Entry* pEntry = pFirstEntry();
       pEntry != nullptr;
       pEntry = pNextEntry(pEntry)) {
    vTrace("%8lld: "_Stz,nSub(pEntry,m_xWsp.aArea()));
    i8 iOffset = iToOffsetAbs(pEntry);
    pEntry->vShow_v2(iOffset,m_pIndex,true);
  }

  vTrace("\n"_Stz);
  vTrace("FreeEntries:\n"_Stz);
  for (FreeEntry* pFreeEntry = pFirstFree();
       pFreeEntry != nullptr;
       pFreeEntry = pFreeEntry->pNextFree()) {
    vTrace("  %p bytes: %lld next: %p prev: %p\n"_Stz,
           pFreeEntry,pFreeEntry->nSizeItem(),
           pFreeEntry->pNextFree(),pFreeEntry->pPrevFree());
  }
  
  vTrace("\n"_Stz);
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

/*----------------------------------------------------------------------------*/
/* fix the higher level buffers                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get an entry by number                                                     */
/*----------------------------------------------------------------------------*/

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
i8 EntryBuffer_v2::iToOffsetAbs(Entry* pEntry)
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
Footer* EntryBuffer_v2::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Unit Test ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

void DBobjects::vEntryBuffer_v2_UnitTest(Table* pTable)
{
  
//  for (i8 iInt = 0; iInt < 1000; iInt++) {
//    i8 nOffsets = Random::iLogLevel(gpAttachMap->m_nIndexOffsets+1, C_FLAT_FREQ);
//    vTrace("nOffsets %lld\n"_Stz,nOffsets);
//  }
  
  ColumnVec* pColVec = pTable->pColumnVec();
  Index* pHiddenIndex = pTable->pHiddenIndex();
  
  IdxVec* pIdxVec = pTable->pIdxVec();
  Index* pIndex1 = pIdxVec->pGet(1);
  IdxColVec* pIdxColVec1 = pIndex1->pIdxColVec();
  
  Formatters* pFormatters = &gpAttachMap->m_xFormatters;
 
  EntryBuffer_v2* pFirstBuffer = new EntryBuffer_v2;
  EntryBuffer_v2* pCurrentBuffer = pFirstBuffer;
  pCurrentBuffer->vIndex(pHiddenIndex);
  pCurrentBuffer->vPartitionNo(0);

  EntryBuffer_v2* pIndexBuffer = new EntryBuffer_v2;
  pIndexBuffer->vIndex(pHiddenIndex);
  pIndexBuffer->vPartitionNo(0);
  
  s256 sRecData;
  i8 iRecLength;
  
  EntWrk_v2 xRowEntWrk;
  i8 iRowLength;
  
  bool bBlankLine;
  bool bFormatted;
  bool bAppended;
  
  KeyWrk xKeyWrk;
  
  for (i8 iInt = 0; iInt < 10; iInt++) {
    sprintf(sRecData.aChar(),"%lld,%s,1900-01-01 00:00:%lld",
            iInt,IntLib::sAsText(iInt),iInt);
    //vTrace("Record: %s\n"_Stz,sRawData.aChar());
    
    char* aRowData = xRowEntWrk.aData_v2();
    iRecLength = sRecData.iLength();
    bFormatted = Row::bFormatRow(pColVec,pFormatters,
                                 sRecData,iRecLength,
                                 (Stz*)aRowData,iRowLength,
                                 bBlankLine);
    xRowEntWrk.vDataSize(iRowLength);
    ((Row*)aRowData)->vDump(pColVec);
    
    xRowEntWrk.vTxId(0);
    xRowEntWrk.vHasData(true);
    Entry* pRowEntry = pCurrentBuffer->pAppendRow_v2(xRowEntWrk);
    if ( ! pRowEntry ) {
      pCurrentBuffer = pCurrentBuffer->pNewNextBuffer();
      pRowEntry = pCurrentBuffer->pAppendRow_v2(xRowEntWrk);
      ASSERT(pRowEntry);
    }
    
    //pCurrentBuffer->vAddFree(pRowEntry);
    
    xKeyWrk.vReset();
    pRowEntry->vExtractIndexKey_v2(xKeyWrk,pIdxColVec1);
    xKeyWrk.vShow();
  }

  for (pCurrentBuffer = pFirstBuffer;
       pCurrentBuffer;
       pCurrentBuffer = pCurrentBuffer->pNextBuffer())
    pCurrentBuffer->vShow();

  //pIndexBuffer->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
