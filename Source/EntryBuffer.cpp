/*============================================================================*/
/* EntryBuffer.cpp -- Entry Buffer                                            */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

bool mbWatch = false;
//#define MB_WATCH

#define EB_LAZY_CHAIN
#define EB_SPLIT2
#define EB_SINGLE_COLIDX

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
EntryBuffer* DBobjects::_pEntryBuffer(void* aAddress)
{
  EntryBuffer* pEntryBuffer = reinterpret_cast<EntryBuffer*>(aAddress);
  
  RASSERT_TYPE(pEntryBuffer,CN_ENTRYBUFFER);
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* EntryBuffer::operator new(size_t)
{
  EntryBuffer* pEntryBuffer =
    static_cast<EntryBuffer*>(HeapMem::pAllocate(CN_ENTRYBUFFER));
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* EntryBuffer::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void EntryBuffer::operator delete(void* aAddress)
{
  EntryBuffer* pEntryBuffer = static_cast<EntryBuffer*>(aAddress);
  
  HeapMem::vFree(pEntryBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer::EntryBuffer(void)
: ItemBuffer(CN_ENTRYBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_oLock.vInitLock(LK_ENTRYBUFFER);
  
  m_bIsLast       = false;
  
  m_pHigherBuffer = nullptr;
  m_pNextBuffer   = nullptr;
  m_pPrevBuffer   = nullptr;

  m_pIndex        = nullptr;
  m_nPartitionNo  = -1;
  m_iBufferNo     = *nBufferNo();
  m_iBufferLevel  = 0;
  
  m_aEntryOffsets = nullptr;
  m_i_FreeOffsetChain = C_ENTRY_NONE;
  
  m_aHeadEntry  = nullptr;
  m_aLastEntry  = nullptr;
  m_aFreeEntryList  = nullptr;
  
  m_aLazyChain  = nullptr;
  
  m_oWhenNewed.vNow();
  
  vHeadEntryNew();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_ENTRYBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntryBuffer::~EntryBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assert valid entry buffer                                                  */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vAssertEntryBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* test for valid entry buffer                                                */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bValidEntryBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (! bValidMagic()) {
    vTrace("Not valid magic\n"_Stz);
    return false;
  }
  
  if (CN_ENTRYBUFFER != eClass()) {
    vTrace("Not valid class\n"_Stz);
    return false;
  }
  
  if (! aFooter()->bValidFooter()) {
    vTrace("Not valid footer\n"_Stz);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump entry buffer                                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  EntryBuffer* pEntryBuffer = _pEntryBuffer(pObject);

  pEntryBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get age of buffer in seconds                                               */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::iAgeInSeconds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Since oSince;
  oSince.vNow();
  
  Ivl wDiff;
  
  wDiff = oSince.wSubtract(m_oWhenNewed);

  i8 iAge = wDiff.iSecond();

  return iAge;
}

/*----------------------------------------------------------------------------*/
/* set buffer level                                                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vBufferLevel(i8 iBufferLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_iBufferLevel = iBufferLevel;
}

/*----------------------------------------------------------------------------*/
/* get the buffer level                                                       */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::iBufferLevel(void)
{
  return m_iBufferLevel;
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDropWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_oLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vGrabWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_oLock.vGrabWrite();
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
  
  m_pHigherBuffer = pHigherBuffer;
}

/*----------------------------------------------------------------------------*/
/* set the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pIndex = pIndex;

  if (! m_pIndex->bIsHidden())
    return;
  
  m_aEntryOffsets  =
    reinterpret_cast<i4*>(aAllotSpace(sizeof(i4) * C_ENTRIES_L0));
  
  m_i_FreeOffsetChain = C_ENTRY_NONE;
  
  vInitFreeChain();
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
void EntryBuffer::vAnalyze(s48K& sAnalysis, bool bDetailed,
                           i8& iBuffers, i8& iEntries)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (m_pIndex->bIsHidden()) {
    
    ASSERT(m_iBufferLevel == 0);
    EntryBuffer* pNextBuffer = this;
    
    do {
      ASSERT(pNextBuffer->iBufferLevel() == 0);
      
      iBuffers++;
      iEntries += (pNextBuffer->m_nItems);
      
      if (bDetailed)
        sAnalysis.vCatSprint("row: (%s, %lld, %5lld, 'L%lld', %lld )\n"_Stz,
                             gpAttachMap->m_pGrid->pThisNode()->aNodeName(),
                             pNextBuffer->m_nPartitionNo,
                             iBuffers,
                             pNextBuffer->m_iBufferLevel,
                             pNextBuffer->m_nItems);
      
      pNextBuffer = pNextBuffer->pNextBuffer();
    } while (pNextBuffer);
    
    return;
  }
  
  iBuffers++;
  iEntries += m_nItems;
  
  if (bDetailed)
    sAnalysis.vCatSprint(" row: (%s, %lld, %5lld,' L%lld', %lld)\n"_Stz,
                         gpAttachMap->m_pGrid->pThisNode()->aNodeName(),
                         m_nPartitionNo,
                         iBuffers,
                         m_iBufferLevel,
                         m_nItems);
  
  // recursively dump the lower level buffers
  if (m_iBufferLevel != 0) {
    
    for (Entry* aEntry = aFirstEntry(); aEntry != nullptr;
         aEntry = aNextEntry(aEntry, 0)) {
      
      // get the index buffer from the entry
      EntryBuffer* pEntryBuffer = aEntry->pReferencedEntryBuffer();
      
      ASSERT(pEntryBuffer);
      ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
      
      pEntryBuffer->vAnalyze(sAnalysis, bDetailed, iBuffers, iEntries);
      
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* attempt to append a new row entry into this buffer                         */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aAppendRow(RowWrk& xRowWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(iBufferLevel() == 0);
  
  Entry* aRowEntry;

  // rows only ever have one next offset
  xRowWrk.vOffsets(1);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute required entry space for row and offset
  
  i8 nEntryBytes = xRowWrk.nEntrySize();
  i8 nOffsetBytes = xRowWrk.nOffsetSize();
  i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if this buffer has enough space
  
  if ( ! bHasSpace(nEntryPlusOffsets)) {
    //if (nItems() > 7260)
    //vTrace("Row entries %lld\n"_Stz,nItems());
    return nullptr;
  }
  
//  if (nItems() >= C_ENTRIES_L0 - 16) {
//    vTrace("Too many row entries %lld\n"_Stz,nItems());
//    return nullptr;
//  }
  
  RASSERT(nItems() < C_ENTRIES_L0 - 16);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the rowId
  
  bn nBufferNo = this->nBufferNo();
  
  i8 i_RowNo = iGetFreeRowEntry(); //nItems();
  if (C_ENTRY_NONE == i_RowNo) {
    vTrace("No free row entries\n"_Stz);
    return nullptr;
  }
  
  RASSERT(i_RowNo >=0 and i_RowNo < C_ENTRIES_L0);
  
  Id xRowId(*nBufferNo, i_RowNo);
  
  xRowWrk.vId(xRowId);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate space in this buffer for the row and offsets
  
  aRowEntry = aAllotEntry(nEntryBytes);
  ASSERT(aRowEntry);
  
  NEO* aNEO = aAllotNEO(nOffsetBytes);
  ASSERT(aNEO);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the buffer entry
  aRowEntry->vSetEntry(xRowWrk);
  
  //pRowEntry->vShow(0, m_pIndex, false);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set entry offsets
  
  i4 iEntryOffset = iToOffset(aRowEntry);
  RASSERT(i_RowNo >= 0 && i_RowNo < C_ENTRIES_L0);
  m_aEntryOffsets[i_RowNo] = iEntryOffset;
  
  if (m_aLastEntry)
    m_aLastEntry->vNEO(0,iEntryOffset);
  aRowEntry->vNEO(0,-1);
  
  m_aLastEntry = aRowEntry;
  
  // set head entry offset
  if (m_aHeadEntry->iNEO(0) == -1)
    m_aHeadEntry->vNEO(0, iEntryOffset);
  
  RASSERT(aRowEntry->bValidEntry());
  
  return aRowEntry;
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
    ASSERT(m_iBufferLevel == 0);
    pNextBuffer = this->pNextBuffer();
    nBuffers++;
    
    while (pNextBuffer) {
      ASSERT(pNextBuffer->iBufferLevel() == 0);
      
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
  if (m_iBufferLevel != 0) {
    
    for (Entry* aEntry = aFirstEntry();
         aEntry != nullptr;
         aEntry = aNextEntry(aEntry, 0)) {
      
      // get the index buffer from the entry
      pNextBuffer = aEntry->pReferencedEntryBuffer();
      
      ASSERT(pNextBuffer);
      ASSERT(pNextBuffer->eClass() == CN_ENTRYBUFFER);
      
      pNextBuffer->vBufferCount(nBuffers);
      
    }
  }
}

/*----------------------------------------------------------------------------*/
/* build skip list based on existing L0 offset chain being in sorted order    */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vBuildSkipListSorted(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //
  //  H:
  //  [p]..[ ].................[ ]
  //  [p]..[ ]..[ ].......[ ]..[ ]
  //  [p]--[ ]--[ ]--[ ]--[ ]--[ ]  (L0 is a-priori in sorted order)
  //
  
  /*
   // ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
   // temp: clear out all l1 and l2 offsets for testing purpoes
   
   for (Entry* aEntry = m_aHeadEntry;
   aEntry != nullptr;
   aEntry = aNextEntry(aEntry, 0)) {
   
   // get the number of offsets for this entry
   i8 nOffsets = aEntry->nOffsets();
   
   // connect the dots
   for (i8 i_Offset = 1; i_Offset < nOffsets; i_Offset++)
   aEntry->vNEO(i_Offset,-1);
   }
   vShow();
   */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Entry* xPrevVec[gpAttachMap->m_nIndexOffsets];
  
  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexOffsets; i_Offset++)
    xPrevVec[i_Offset] = m_aHeadEntry;
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // go through each entry in L0 order and
  //   correct the prior offset to that entry for each level
  
  for (Entry* aEntry = aFirstEntry();
       aEntry != nullptr;
       aEntry = aNextEntry(aEntry, 0)) {
    
    // where is this entry in the buffer (offset-wise)?
    i4 iEntryOffset = iToOffset(aEntry);
    
    // get the number of offsets for this entry
    i8 nOffsets = aEntry->nOffsets();
    
    // connect the dots
    for (i8 i_Offset = 1; i_Offset < nOffsets; i_Offset++) {
      
      // set previous entry at this level to refer to this entry
      xPrevVec[i_Offset]->vNEO(i_Offset,iEntryOffset);
      
      // the previous entry at this level now becomes this entry
      xPrevVec[i_Offset] = aEntry;
    }
  }
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
void EntryBuffer::vDeleteEntry(Entry** aaEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // currently we only delete from non-hidden indexes
  ASSERT(! m_pIndex->bIsHidden());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Entry* aEntry = *aaEntry;
  
  Entry* xPriorVec[gpAttachMap->m_nIndexOffsets];

  Key* aKey = aEntry->aKey();
  
  vPositionPriorVecEntry(aKey, xPriorVec, aEntry);
  
  vCutEntry(aEntry,xPriorVec);
  
  vAddFreeList(aEntry);
  
  // entry no longer exists, set the pointer to be null
  *aaEntry = nullptr;
}

/*----------------------------------------------------------------------------*/
/* delete key                                                                 */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bDeleteKey(Key* aDeleteKey, EntryBuffer* pToBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Entry* xPriorVec[gpAttachMap->m_nIndexOffsets];
  
  vPositionPriorVecBuffer3(aDeleteKey,xPriorVec,pToBuffer);
  
  Entry* aPriorEntry = xPriorVec[0];
  
  Entry* aEntryToBeDeleted = aNextEntry(aPriorEntry,0);
  if (!aEntryToBeDeleted)
    return false;
  
  //vTrace("[%lld] pDeleteEntry key %lld\n"_Stz,
  //       *nBufferNo(),
  //       pEntryToBeDeleted->aKey()->iGetInt());
  
  Key* aDeleteEntryKey = aEntryToBeDeleted->aKey();
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  //if (! aDeleteEntryKey->bEqualTo(aDeleteKey,pIdxColVec))
  //  return;
  RASSERT(aDeleteEntryKey->bEqualTo(aDeleteKey,pIdxColVec));
  RASSERT(pToBuffer == aEntryToBeDeleted->pReferencedEntryBuffer());
  
  vDeleteEntry(&aEntryToBeDeleted);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // show this buffer
  vShow();
  
  // recursively dump the lower level buffers
  if (m_iBufferLevel != 0) {
    
    for (Entry* aEntry = aFirstEntry();
         aEntry != nullptr;
         aEntry = aNextEntry(aEntry, 0)) {
      
      // get the index buffer from the entry
      EntryBuffer* pEntryBuffer = aEntry->pReferencedEntryBuffer();
      
      ASSERT(pEntryBuffer);
      ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
      
      pEntryBuffer->vDump();
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get first child buffer                                                     */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pFirstChildBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
  
  //if (m_aLazyChain)
  //  vBuildSkipList();
#endif
  
  Entry* aFirstEntry = this->aFirstEntry();
  EntryBuffer* pChildEntryBuffer = aFirstEntry->pReferencedEntryBuffer();
  
  return pChildEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* get first entry                                                            */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aFirstEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
  
  //if (m_aLazyChain)
  //  vBuildSkipList();
#endif
  
  Entry* aFirst = aNextEntry(m_aHeadEntry,0);
  
  return aFirst;
}

/*----------------------------------------------------------------------------*/
/* get an entry by number                                                     */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aGetEntryRowNo(i8 i_RowNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (i_RowNo == C_ENTRY_NONE)
    return nullptr;
  
  RASSERT(i_RowNo >= 0 && i_RowNo < C_ENTRIES_L0);
  i4 iOffset = m_aEntryOffsets[i_RowNo];
  
  char* aAddress = aAdd<char*>(aDataArea(), iOffset);
  
  Entry* aEntry = reinterpret_cast<Entry*>(aAddress);
  
  return aEntry;
}

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bHasSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nFree = m_xWsp.nFree() - sizeof(Wilderness);
  
  // we allow for only C_ENTRIES_L0 - 16 entries
  if (nItems() >= C_ENTRIES_L0 - 16) {
    return false;
  }

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
/* insert a key entry specifically into this buffer (should be enough space)  */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aInsertKeyAbs(KeyWrk& xKeyWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Entry* aKeyEntry;
  
#ifdef EB_LAZY_CHAIN
  if (m_pIndex->pTable()->bIsLoading()) {
    if (! m_iBufferLevel) {
      aKeyEntry = aInsertKeyAbsLazy(xKeyWrk);
      return aKeyEntry;
    }
  }
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // trace
  
  //vTrace("Inserting key: "_Stz);
  //IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  //xKeyWrk.aKey()->vShow(pIdxColVec);
  //vTrace("\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute required entry space
  
  RASSERT(xKeyWrk.nOffsets());
  
  i8 nEntryBytes = xKeyWrk.nEntrySize();
  i8 nOffsetBytes = xKeyWrk.nOffsetSize();
  i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;
  
  RASSERT(bHasSpace(nEntryPlusOffsets));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate space for the key and next offsets in this buffer
  
  aKeyEntry = aAllotEntry(nEntryBytes);
  ASSERT(aKeyEntry);
  
  NEO* aNEO = aAllotNEO(nOffsetBytes);
  ASSERT(aNEO);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the key entry
  
  aKeyEntry->vSetEntry(xKeyWrk);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the key entry into the skip list
  
  vInsertKeySkip(aKeyEntry);
  
  //if (! m_pIndex->bIsHidden() && m_iBufferLevel)
  //  bVerifyAdjacent();
  
  //vShow();
  
  return aKeyEntry;
}

/*----------------------------------------------------------------------------*/
/* insert a key entry specifically into this buffer (L0 chain only)           */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aInsertKeyAbsL0(KeyWrk& xKeyWrk, Entry** aaPriorEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // trace
  
  //vTrace("Inserting key:\n"_Stz);
  //IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  //aKey->vShow(pIdxColVec);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute required entry space
  
  RASSERT(xKeyWrk.nOffsets());
  
  i8 nEntryBytes = xKeyWrk.nEntrySize();
  i8 nOffsetBytes = xKeyWrk.nOffsetSize();
  i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;
  
  RASSERT(bHasSpace(nEntryPlusOffsets));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate space for the key and next offsets in this buffer
  
  Entry* aKeyEntry = aAllotEntry(nEntryBytes);
  ASSERT(aKeyEntry);
  
  NEO* aNEO = aAllotNEO(nOffsetBytes);
  ASSERT(aNEO);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the key entry
  
  aKeyEntry->vSetEntry(xKeyWrk);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append to prior entry at L0 only
  
  i4 iOffset = iToOffset(aKeyEntry);
  
  (*aaPriorEntry)->vNEO(0,iOffset);
  
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  // for the moment we set L1 & L2 offsets in prior entry to -1
  
  i8 nPriorOffsets = aKeyEntry->nOffsets();
  for (i8 i_Offset = 0; i_Offset < nPriorOffsets; i_Offset++)
    aKeyEntry->vNEO(i_Offset,-1);
  
  // ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
  
  *aaPriorEntry = aKeyEntry;
  
  return aKeyEntry;
}

/*----------------------------------------------------------------------------*/
/* insert a new key entry into this key tree                                  */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aInsertKeyTree(KeyWrk& xKeyWrk,
                                  EntryBuffer** ppRootKeyBuffer)
{
  // ** note: this is a static function **
  
  
  EntryBuffer* pRootKeyBuffer = *ppRootKeyBuffer;
  
#ifdef MB_WATCH
  static i8 nInserts = 0;
  nInserts++;
  
  if (2947 == nInserts)
    mbWatch = true;
  
  if (mbWatch) {
    IdxColVec* pIdxColVec = pRootKeyBuffer->pIndex()->pIdxColVec();
    vTrace("\n[ ] pInsertKeyTree: "_Stz);
    xKeyWrk.vShow(pIdxColVec);
    pRootKeyBuffer->vShowTree();
    int iDebug = 1;
  }
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set a random number offsets
  
  xKeyWrk.vRandomOffsets();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // position on the initial buffer we will try to insert into
  
  Key* aInsKey = xKeyWrk.aKey();
  i8 iInsKeyLen = xKeyWrk.nKeySize();
  
  EntryBuffer* pPositionedBuffer =
    pRootKeyBuffer->pPositionPriorBuffer(aInsKey);

  ASSERT(0 == pPositionedBuffer->iBufferLevel());
  //pPositionedBuffer->vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert into the tree
  

  Entry* aInsertedKeyEntry =
    pPositionedBuffer->aInsertKeyUpwards(xKeyWrk,ppRootKeyBuffer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef MB_WATCH
  if (mbWatch) {
    IdxColVec* pIdxColVec = pRootKeyBuffer->pIndex()->pIdxColVec();
    vTrace("\n[ ] pInsertKeyTree: "_Stz);
    xKeyWrk.vShow(pIdxColVec);
    pRootKeyBuffer->vShowTree();
    int iDebug = 1;
  }
#endif
  
  return aInsertedKeyEntry;
}

/*----------------------------------------------------------------------------*/
/* get last entry                                                             */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aLastEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
  
  //if (m_aLazyChain)
  //  vBuildSkipList();
#endif
  
  Entry* aLast = m_aHeadEntry;
  
  for (;;) {
    Entry* aNext = aNextEntry(aLast,0);
    if (! aNext)
      break;
    aLast = aNext;
  }
  
  return aLast;
}

/*----------------------------------------------------------------------------*/
/* get length of list of buffers                                               */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::iLengthList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 iLength = 0;
  for (EntryBuffer* pLn = this; pLn; pLn = pLn->pNextBuffer())
    iLength++;
  
  return iLength;
}

/*----------------------------------------------------------------------------*/
/* get length of sublist of buffers                                           */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::iLengthSubList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 iLength = 0;
  
  EntryBuffer* pThisL0 = this;
  EntryBuffer* pThisL1 = pThisL0->pHigherBuffer();
  
  EntryBuffer* pL0;
  for (pL0 = pThisL0;
       pL0 && pL0->pHigherBuffer() == pThisL1;
       pL0 = pL0->pNextBuffer()) {
  
    iLength++;
  }

  return iLength;
}

/*----------------------------------------------------------------------------*/
/* get next entry                                                             */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aNextEntry(Entry* aCurrent, i8 i_Offset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //#ifdef EB_LAZY_CHAIN
  //RASSERT( ! m_aLazyChain );
  //#endif
  
#ifdef _DEBUG
  aCurrent->vAssertEntry();
#endif
  
  i4 iOffset = aCurrent->iNEO(i_Offset);
  
  if (-1 == iOffset)
    return nullptr;
  
  char* aAddress = aAdd<char*>(aDataArea(), iOffset);
  //Item* aItem = _pItem(aAddress);
  
  //if (CN_ENTRY != pItem->eClass())
  //  vShow();
  
  Entry* aEntry = _aEntry(aAddress);

  return aEntry;
}

/*----------------------------------------------------------------------------*/
/* position at an index buffer at given level starting from this buffer       */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pPositionLevel(Key* aGivenKey, i8 nKeyCols,
                                         eJC eJuxtaCompare, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
  
  //if (m_aLazyChain)
  //  vBuildSkipList();
#endif

  EntryBuffer* pEntryBuffer = this;
  
  i8 iBufferLevel;
  Entry* aEntry;
  
  while (true) {
    iBufferLevel = pEntryBuffer->iBufferLevel();
    
    if (iBufferLevel == iLevel)
      break;
    
    // get the entry
    aEntry = pEntryBuffer->aPositionQuery(aGivenKey, nKeyCols, eJuxtaCompare);
    ASSERT(aEntry);
    
    // get the index buffer of the entry
    pEntryBuffer = aEntry->pReferencedEntryBuffer();
    ASSERT(pEntryBuffer);
    ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
  }
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* position at key as per relational operator                                 */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aPositionQuery(Key* aGivenKey, eRO eRelnOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
  
  //if (m_aLazyChain)
  //  vBuildSkipList();
#endif
  
  bool bIsEqual;
  Entry* aIndexEntry = aPositionQuery(aGivenKey, 1, JC_AFTER);
  
  bool bAfter;
  if (aIndexEntry) {
    do {
      bool bIsEqual = false;
      bAfter = aIndexEntry->aKey()->bJuxtaCompare(JC_AFTER, aGivenKey,
                                                  m_pIndex->pIdxColVec(), 1,
                                                  bIsEqual, eRelnOp);
      if (! bAfter) {
        aIndexEntry = aNextEntry(aIndexEntry, iBufferLevel());
      }
    } while (aIndexEntry && ! bAfter);
  }
  
  return aIndexEntry;
}

/*----------------------------------------------------------------------------*/
/* position at given key                                                      */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aPositionQuery(Key* aGivenKey,i8 nKeyCols, eJC eJuxtaCompare)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
  
  //if (m_aLazyChain)
  //  vBuildSkipList();
#endif
  
  /*
   * positioning is based on eJuxtaCompare
   *   JC_BEFORE: never called
   *   JC_SAME:   scan for entry which is the same as given key
   *   JC_AFTER:  scan for entry which the given key would be inserted after
   */
  
  ASSERT(JC_SAME == eJuxtaCompare || JC_AFTER == eJuxtaCompare);
  
  bool bIsJuxta;
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  Entry* aScanEntry_sl = m_aHeadEntry;
  bool bIsEqual;
  
  for (i8 i_ScanOffset = 2; i_ScanOffset >= 0; i_ScanOffset--) {
    
    Entry* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanOffset);
    while (aNextEntry_sl) {
      
      Key* aNextKey = aNextEntry_sl->aKey();
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aGivenKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
      bIsJuxta =
        aGivenKey->bJuxtaCompare(JC_AFTER,aNextKey,pIdxColVec,nKeyCols,bIsEqual);
      
      // have we found to key during a query?
      if (bIsEqual) {
        if (0 == i_ScanOffset)
          return aNextEntry_sl;
        else
          break;
      }
      
      // if given key is not same/after next key then break to outer loop
      if (! bIsJuxta)
        break;
      
      aScanEntry_sl = aNextEntry_sl;
      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanOffset);
    }
    //pPriorVec[*i_ScanOffset] = pScanEntry_sl;
  }
  
  if (aScanEntry_sl == aHeadEntry()) {
    aScanEntry_sl = aNextEntry(aScanEntry_sl, 0);
    // we currently think it should be null
    // RASSERT( ! pScanEntry_sl );
  }
  
  // JC_SAME : return the entry which is same as given key
  // JC_AFTER: return the entry which the given key would be inserted after
  return aScanEntry_sl;
}

/*----------------------------------------------------------------------------*/
/* reset/empty the buffer                                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT(nullptr == m_aEntryOffsets);
  RASSERT(! m_pIndex->bIsHidden());
  RASSERT(1 == m_iBufferLevel);
  
  vResetUsed();
  
  m_i_FreeOffsetChain = C_ENTRY_NONE;
  
  m_aHeadEntry  = nullptr;
  m_aLastEntry  = nullptr;
  m_aFreeEntryList  = nullptr;
  
  m_aLazyChain  = nullptr;
  
  vHeadEntryNew();
}

/*----------------------------------------------------------------------------*/
/* restore the buffer after being received over a socket                      */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vRestore(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT( ! m_pHigherBuffer );
  RASSERT( ! m_pNextBuffer );
  RASSERT( ! m_pPrevBuffer );
  
  // set the index to nullptr (this pointer is not valid any more)
  m_pIndex = nullptr;
  
  // get the original buffer address
  Buffer* pOriginalBuffer = pBitAnd<Buffer*>(m_aHeadEntry,C_OBJECT_TO_BUFFER_MASK);
  
  // get this buffer address
  Buffer* pThisBuffer = pBitAnd<Buffer*>(this,C_OBJECT_TO_BUFFER_MASK);
  
  // calculate offset for m_aEntryOffsets and set it
  u8 uOffset = nSub(m_aEntryOffsets, pOriginalBuffer);
  m_aEntryOffsets = aBitOr<i4*>(pThisBuffer,uOffset);
  
  // calculate offset for m_aHeadEntry and set it
  uOffset = nSub(m_aHeadEntry, pOriginalBuffer);
  m_aHeadEntry = pBitOr<Entry*>(pThisBuffer,uOffset);
  
  // calculate offset for m_aLastEntry and set it
  if (m_aLastEntry) {
    uOffset = nSub(m_aLastEntry, pOriginalBuffer);
    m_aLastEntry = pBitOr<Entry*>(pThisBuffer,uOffset);
  }
  
  // calculate offset for m_aFreeEntryList and set it
  if (m_aFreeEntryList) {
    uOffset = nSub(m_aFreeEntryList, pOriginalBuffer);
    m_aFreeEntryList = pBitOr<FreeEntry*>(pThisBuffer,uOffset);
  }
  
  // calculate offset for m_aLazyChain and set it
  if (m_aLazyChain) {
    uOffset = nSub(m_aLazyChain, pOriginalBuffer);
    m_aLazyChain = pBitOr<Entry*>(pThisBuffer,uOffset);
  }
  
  // get the buffer no
  bn nBuffer = nBufferNo();
  
  // now restore the row id's to point to this buffer no
  for (Entry* aEntry = aFirstEntry();
       aEntry != nullptr;
       aEntry = aNextEntry(aEntry, 0)) {
    
    Id xId(*nBuffer, aEntry->iRow());
    aEntry->vId(xId);
  }
  
}

/*----------------------------------------------------------------------------*/
/* restore the index of the index buffer                                      */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vRestoreIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT( ! m_pIndex );
  m_pIndex = pIndex;
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vShow(Entry* aMarkEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  s256 sHigherBuffer;
  if (m_pHigherBuffer)
    StrLib::wToString((i8)((EntryBuffer*)m_pHigherBuffer)->m_iBufferLevel,
                      sHigherBuffer.aChar());
  else
    sHigherBuffer = "..."_Stz;
  
  vTrace("\n"_Stz);
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  vTrace("EntryBuffer: %p (buffer %lld): items: %lld used: %lld freespace %lld\n"_Stz,
         this,*nBufferNo(),nItems(),nUsedBytes(),nFreeBytes());
  
  vTrace("  higherBuffer %p (L%s) prevBuffer %p nextBuffer %p\n"_Stz,
         m_pHigherBuffer,sHigherBuffer.aStz(),m_pPrevBuffer,m_pNextBuffer);

  vTrace("  index: %s partition: %lld level: %lld\n"_Stz,
         m_pIndex ? m_pIndex->aIndexName() : "NULL"_Stz,m_nPartitionNo,m_iBufferLevel);
  
  vTrace("  entryOffset: %p freeOffsetHead: %lld\n"_Stz,
         m_aEntryOffsets,m_i_FreeOffsetChain);

  vTrace("  headEntry: %p[%d,%d,%d] lastEntry: %p firstFree: %p\n"_Stz,
         m_aHeadEntry,m_aHeadEntry ? m_aHeadEntry->iNEO(0) : 0,
                      m_aHeadEntry ? m_aHeadEntry->iNEO(1) : 0,
                      m_aHeadEntry ? m_aHeadEntry->iNEO(2) : 0,
         m_aLastEntry,m_aFreeEntryList);
  
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  ASSERT(m_aHeadEntry);
  
  vTrace("\n"_Stz);
  vTrace("Entries (by headEntry vector[0]):\n"_Stz);
 
  i8 iOffset = m_aHeadEntry->iNEO(0);
  while (iOffset > -1) {
    Entry* aEntry = aGetEntryOffset(iOffset);
    aEntry->vShow(iOffset,m_pIndex,true);
    if (aMarkEntry == aEntry)
      vTrace("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
             "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n"_Stz);
    
    iOffset = aEntry->iNEO(0);
  }
    
  vTrace("\n"_Stz);
  vTrace("=================================================="
         "==================================================\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* show short form of buffer                                                  */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vShowShort(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  vTrace("[t%lld h%lld: "_Stz,
         *nBufferNo(),
         pHigherBuffer() ? *pHigherBuffer()->nBufferNo() : 0);
  
  i8 iOffset = aHeadEntry()->iNEO(0);
  while (iOffset > -1) {
    Entry* aEntry = aGetEntryOffset(iOffset);
    
    Key* aKey = aEntry->aKey();
    i8 iVal = aKey->iGetInt();
    
    if (m_iBufferLevel)
      vTrace("%lld(%lld)"_Stz,iVal,*aEntry->pReferencedEntryBuffer()->nBufferNo());
    else
      vTrace("%lld|%lld|"_Stz,iVal,*aEntry->pReferencedEntryBuffer()->nBufferNo());
    
    iOffset = aEntry->iNEO(0);
    if (iOffset > -1)
      vTrace(","_Stz);
  }
  vTrace("]"_Stz);
}

/*----------------------------------------------------------------------------*/
/* show sublist of buffers                                                    */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vShowSubList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);

  EntryBuffer* pThisL0 = this;
  EntryBuffer* pThisL1 = pThisL0->pHigherBuffer();
  
  vTrace("{"_Stz);
  
  if (pThisL0->pPrevBuffer())
    vTrace("<-"_Stz);
  
  EntryBuffer* pL0;
  for (pL0 = pThisL0;
       pL0 && pL0->pHigherBuffer() == pThisL1;
       pL0 = pL0->pNextBuffer()) {
    
    if (pL0 != pThisL0)
      vTrace(","_Stz);
    
    vTrace("%lld"_Stz,pL0->iBufferNo());
  }
  
  if (pL0)
    vTrace("->"_Stz);
  
  vTrace("}\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* show contents of all buffers in index tree                                 */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vShowTree(bool bDumpBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (! m_pHigherBuffer )
    vTrace("\nIndex Tree:\n"_Stz,m_iBufferLevel);
  
  vTrace("L%lld"_Stz,m_iBufferLevel);
  
  for (EntryBuffer* pBuffer = this;
       pBuffer;
       pBuffer = pBuffer->pNextBuffer()) {
    
    pBuffer->vShowShort();
    
    ASSERT(! pBuffer->pPrevBuffer() || pBuffer == pBuffer->pPrevBuffer()->pNextBuffer());
    ASSERT(! pBuffer->pNextBuffer() || pBuffer == pBuffer->pNextBuffer()->pPrevBuffer());
  }
  
  vTrace("\n"_Stz);
  
  if (bDumpBuffer) {
    for (EntryBuffer* pBuffer = this;
         pBuffer;
         pBuffer = pBuffer->pNextBuffer())
      pBuffer->vShow();
  }
  
  if ( ! m_iBufferLevel )
    return;
  
  Entry* aFirstEntry = this->aFirstEntry();
  if (! aFirstEntry)
    return;
  
  Id xId;
  xId = aFirstEntry->xId();
  bn nBuffer = xId.iBuffer();
  
  HeapBuffer* pHeapBuffer =
    reinterpret_cast<HeapBuffer*>(gpBufferPool->pToBuffer(nBuffer));
  
  EntryBuffer* pChildBuffer =
    reinterpret_cast<EntryBuffer*>(pHeapBuffer->aFirstChunk());
  
  pChildBuffer->vShowTree(bDumpBuffer);
}

/*----------------------------------------------------------------------------*/
/* splice the given buffer after this buffer                                  */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vSpliceAfter(EntryBuffer* pSplicedBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  EntryBuffer* pThisBuffer = this;
  EntryBuffer* pNextBuffer = pThisBuffer->pNextBuffer();
  
  // the spliced buffer is always inserted after this buffer
  pThisBuffer->vNextBuffer(pSplicedBuffer);
  pSplicedBuffer->vNextBuffer(pNextBuffer);
  pSplicedBuffer->vPrevBuffer(pThisBuffer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // circumstance (a), no next buffer
  
  if (! pNextBuffer)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // circumstance (b), next buffer
  
  pNextBuffer->vPrevBuffer(pSplicedBuffer);
}

/*----------------------------------------------------------------------------*/
/* truncate this buffer by removing all entries                               */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  EntryBuffer* pNextBuffer;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // hidden index buffers are just one level
  
  if (m_pIndex->bIsHidden()) {
    ASSERT(m_iBufferLevel == 0);
    pNextBuffer = this->pNextBuffer();
    
    while (pNextBuffer) {
      ASSERT(pNextBuffer->iBufferLevel() == 0);
      
      EntryBuffer* pThisBuffer = pNextBuffer;
      pNextBuffer = pNextBuffer->pNextBuffer();
      delete pThisBuffer;
    }
    delete this;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // other index
  if (m_iBufferLevel != 0) {
    
    // recursively delete the lower level buffers
    for (Entry* aEntry = aFirstEntry();
         aEntry != nullptr;
         aEntry = aNextEntry(aEntry, 0)) {
      
      // get the index buffer from the entry
      pNextBuffer = aEntry->pReferencedEntryBuffer();
      
      ASSERT(pNextBuffer);
      ASSERT(pNextBuffer->eClass() == CN_ENTRYBUFFER);
      pNextBuffer->vTruncate();
    }
  }
  
  delete this;
}

/*----------------------------------------------------------------------------*/
/* unlink the sublist of EB's following this L0 EB within this L1             */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pUnlinkSubList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(0 == m_iBufferLevel);

  EntryBuffer* pThisL0 = this;
  EntryBuffer* pThisL1 = pThisL0->pHigherBuffer();
  EntryBuffer* pNextL0 = pThisL0->pNextBuffer();
  EntryBuffer* pNextL1 = pThisL1->pNextBuffer();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // circumstance (a) no more L0

  if (! pNextL0)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // circumstance (b) only one L0 under L1

  if (pNextL0->pHigherBuffer() != pHigherBuffer())
    return nullptr;

  //----------------------------------------------------------------------------
  //
  // - there is a sublist
  //
  //----------------------------------------------------------------------------
  
  EntryBuffer* pThisL0SubList = pNextL0;
  
  // the sublist no longer points back to this L0
  pThisL0SubList->vPrevBuffer(nullptr);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // circumstance (c) last L1, there is a sublist

  if (! pNextL1) {

    // last L1, therefore the next chain terminates here
    pThisL0->vNextBuffer(nullptr);

    return pThisL0SubList;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // circumstance (d) middle L1, there is a sublist
  
  EntryBuffer* pFirstL0ofNextL1 = pNextL1->pFirstChildBuffer();
  EntryBuffer* pLastL0ofThisL1 = pFirstL0ofNextL1->pPrevBuffer();
  
  // the next chain points to first L0 buffer of next L1
  pThisL0->vNextBuffer(pFirstL0ofNextL1);
  
  // the prev chain of first L0 of next L1 is this L0
  pFirstL0ofNextL1->vPrevBuffer(pThisL0);
  
  // the prev chain of last L0 of sublist terminates
  pLastL0ofThisL1->vNextBuffer(nullptr);
  
  return pThisL0SubList;
}

/*----------------------------------------------------------------------------*/
/* verify the structure of this buffer (and its children)                     */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  bool bValid = false;
  
  if (m_iBufferLevel) {
    bValid = bVerifyLnAny();
    return bValid;
  }
  
  if (! m_pIndex->bIsHidden()) {
    bValid = bVerifyL0Other();
    return bValid;
  }

  for (EntryBuffer* EBhiddenL0 = this;
       EBhiddenL0;
       EBhiddenL0 = EBhiddenL0->pNextBuffer()) {
    bValid = EBhiddenL0->bVerifyL0Hidden();
    if (! bValid)
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* when was this buffer newed?                                                */
/*----------------------------------------------------------------------------*/
Since& EntryBuffer::oWhenNewed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_oWhenNewed;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot an entry in this buffer                                              */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aAllotEntry(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Entry* aEntry = reinterpret_cast<Entry*>(aAllotItem(nBytes));
  if ( ! aEntry )
    return nullptr;
  
  new(aEntry) Entry(CN_ENTRY,nBytes,0);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return aEntry;
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
/* add given entry to free list                                               */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vAddFreeList(Entry* aEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add entry to head of free entry list
  
  /*
  i8 nSizeEntry = aEntry->nSizeEntry();
  i8 nSizeNEO   = aEntry->iNEOsize();
  i8 nSizeFree  = nSizeEntry + nSizeNEO;
  
  FreeEntry* aFreeEntry = new(aEntry) FreeEntry(nSizeFree);
  */
  
  FreeEntry* aFreeEntry = aMakeFreeEntry(aEntry);
  
  //  if (! bQuickVerify(aEntry)) {
  //    vDumpPriorVec(xPriorVec);
  //    vQuickVerify();
  //  }
  
  if (m_aFreeEntryList) {
    m_aFreeEntryList->vPrevFree(aFreeEntry);
    aFreeEntry->vNextFree(m_aFreeEntryList);
  }
  
  m_aFreeEntryList = aFreeEntry;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // decement the number of items in buffer
  vDecItems();
  
  //vQuickVerify();
}

/*----------------------------------------------------------------------------*/
/* build L0 of the skip list from the lazy chain                              */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vBuildOffsetL0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(! aFirstEntry());
  
  Entry* aTailEntry = m_aHeadEntry;
  while (Entry* aPoppedEntry = aPopLazyChain())
    {;}
}

/*----------------------------------------------------------------------------*/
/* build skip list from the lazy chain                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vBuildSkipList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (! m_aLazyChain )
    return;
  
  Entry* aOriginalFirstEntry = aNextEntry(m_aHeadEntry,0);
  
  static i8 nBuild = 0;
  //vTrace("vBuildSkipList %lld (%lld items)\n"_Stz,++nBuild,nItems());
  
  Entry* aLazyChain = m_aLazyChain;
  m_aLazyChain = nullptr;
  
  while (aLazyChain) {
    
    Entry* aKeyEntry = aLazyChain;
    
    i8 iNextOffset = aKeyEntry->iNEO(0);
    if (-1 == iNextOffset)
      aLazyChain = nullptr;
    else
      aLazyChain = aGetEntryOffset(iNextOffset);
    
    vInsertKeySkip(aKeyEntry);
  }
  
  Entry* aInsertedKeyEntry = aNextEntry(m_aHeadEntry,0);
  
  vHandleFirstInsert(aInsertedKeyEntry,aOriginalFirstEntry,nullptr);
}

/*----------------------------------------------------------------------------*/
/* compact this buffer                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vCompact(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  //RASSERT( ! m_aLazyChain );
  
  if (m_aLazyChain)
    vBuildSkipList();
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  FreeEntry* aFreeEntry;
  Entry*     aMoveEntry;
  Entry*     xPriorVec[gpAttachMap->m_nIndexOffsets];
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  // we can only compact if there are free entries
  if (! m_aFreeEntryList)
    return;
  
  // loose the free list so vShow() works correctly
  m_aFreeEntryList = nullptr;
  
  // scan item is the working pointer
  Item* aScanItem = aFirstItem();
  
  // loop while we can still reclaim more space
  for (;;) {
    
    if (m_iBufferLevel) {
      //vShow();
      int iDebug = 1;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // find the next free entry in buffer
    
    aFreeEntry = nullptr;
    for (;
         aScanItem != nullptr;
         aScanItem = aNextItem(aScanItem)) {
      
      eCN eClass = aScanItem->eClass();
      
      if (CN_FREEENTRY == eClass) {
        aFreeEntry = _aFreeEntry(aScanItem);
        break;
      }
    }
    
    // return if no more free entries
    if (! aFreeEntry)
      break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // find the next entry in buffer after the free entry
    
    aMoveEntry = nullptr;
    for (Item* aItem = aNextItem(aFreeEntry);
         aItem != nullptr;
         aItem = aNextItem(aItem)) {
      
      eCN eClass = aItem->eClass();
      
      if (CN_ENTRY == eClass) {
        aMoveEntry = static_cast<Entry*>(aItem);
        break;
      }
    }
    
    // return if no more entries after the free entry
    if (! aMoveEntry)
      break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the prior vector to the entry being moved
    
    Key* aMoveKey = aMoveEntry->aKey();
    //aKey->vShow(pIdxColVec);
    //vTrace("\n"_Stz);
    
    vPositionPriorVecEntry(aMoveKey, xPriorVec, aMoveEntry);
    
    if (m_iBufferLevel) {
      //vDumpPriorVec(xPriorVec);
      int iDebug = 1;
    }
    
    i4 iWasOffset = iToOffset(aMoveEntry);
    i4 iNowOffset = iToOffset(aFreeEntry);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // now copy move entry left onto free entry
    
    // calculate the number of bytes to copy
    i8 nCopyBytes = aMoveEntry->nSizeEntry() + aMoveEntry->iNEOsize();
    
    // copy the move entry over the free entry
    MemLib::vMove(aFreeEntry, aMoveEntry, nCopyBytes, CN_ENTRYBUFFER);
    
    // give the reclaimed space back to the buffer
    i8 nReclaimedBytes = nSub(aMoveEntry,aFreeEntry);
    //vGiveSpace(nReclaimedBytes);
    
    // convert the moved entry to a free entry
    FreeEntry* aOpenedFreeEntry = aAdd<FreeEntry*>(aFreeEntry,nCopyBytes);
    new(aOpenedFreeEntry) FreeEntry(nReclaimedBytes);
    
    Entry* aCheckEntry = reinterpret_cast<Entry*>(aFreeEntry);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the prior vector to the entry being moved
    
    //vDumpPriorVec(xPriorVec);
    for (i8 i_Offset = 0; i_Offset < 3; i_Offset++) {
      Entry* aPriorEntry = xPriorVec[i_Offset];
      i4 iPriorOffset = aPriorEntry->iNEO(i_Offset);
      if (iWasOffset == iPriorOffset)
        aPriorEntry->vNEO(i_Offset,iNowOffset);
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move scan
    
    aScanItem = aFreeEntry;
    
  } // for
  
  ASSERT(CN_FREEENTRY == aScanItem->eClass());
  
  vResetUsed(aScanItem);
  
  vSetWilderness();
  
  //vShow();
}

/*----------------------------------------------------------------------------*/
/* compact this buffer                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vCompactLeft(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  //RASSERT( ! m_aLazyChain );
  
  if (m_aLazyChain)
    vBuildSkipList();
#endif
  
  //vShow();
  
  //
  // this is contiguous in memory:
  //
  // example:
  //
  // before:  [head](free)[entry][entry](free)(free)[entry](wilderness)
  //
  // after:   [head][entry][entry][entry](wilderness)
  //
  
  // go through each entry in L0 order
  Entry* aLastEntry = nullptr;
  
  for (Entry* aThisEntry = m_aHeadEntry;
       aThisEntry != nullptr;
       aThisEntry = aNextEntry(aThisEntry, 0)) {
    
    aLastEntry = aThisEntry;
    
    // get the address after this entry
    char* aAfterAddr = aThisEntry->aAddressAfter();
    
    // get the address of the next entry
    Entry* aNextEntry = this->aNextEntry(aThisEntry,0);
    
    if (! aNextEntry)
      break;
    
    // if next entry is contiguous with this entry then skip this
    if ((char*)aNextEntry == aAfterAddr)
      continue;
    
    // - there is a gap between this entry and next entry
    
    // calculate the number of bytes to copy
    i8 nCopyBytes = aNextEntry->nSizeEntry() + aNextEntry->iNEOsize();
    
    vTrace("ThisEntry %p AfterAddr %p NextEntry %p CopyBytes %lld\n"_Stz,
           aThisEntry,aAfterAddr,aNextEntry,nCopyBytes);
           
    // copy the next entry so its adjacent to this entry
    MemLib::vMove(aAfterAddr, aNextEntry, nCopyBytes, CN_ENTRYBUFFER);
    
    // reset the L0 offset in this entry
    i4 iRevisedNextOffset = iToOffset((Entry*)aAfterAddr);
    aThisEntry->vNEO(0,iRevisedNextOffset);
    
    //vResetUsed(pAdd<Item*>(pLastEntry,nCopyBytes));
  }
  
  // reset the wilderness
  
  Item* aNewWilderness = aAdd<Item*>(aLastEntry,aLastEntry->nSizeEntry() + aLastEntry->iNEOsize());
  
  vResetUsed(aNewWilderness);
  
  vSetWilderness();
  
  //vShow();
  
  // rebuild the full skiplist
  vBuildSkipListSorted();
}

/*----------------------------------------------------------------------------*/
/* compact this buffer                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vCompactLeft2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  RASSERT(! m_aLazyChain);
#endif

  EntryBuffer* pThisBuffer = this;
  EntryBuffer* pTempBuffer = new EntryBuffer();
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();

  KeyWrk xKeyWrk;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Entry* aPriorEntry = pTempBuffer->aHeadEntry();
  
  for (Entry* aThisEntry = pThisBuffer->aFirstEntry();
       aThisEntry != nullptr;
       aThisEntry = pThisBuffer->aNextEntry(aThisEntry, 0)) {
    
    i8 iTxId = aThisEntry->iTxId();
    Id xId = aThisEntry->xId();
    Key* aKey = aThisEntry->aKey();
    i8 nKeySize = aKey->nKeySize(pIdxColVec);
    i8 nOffsets = aThisEntry->nOffsets();
    
    xKeyWrk.vTxId(iTxId);
    xKeyWrk.vId(xId);
    xKeyWrk.vEmbedded((char*)aKey);
    xKeyWrk.vKeySize(nKeySize);
    xKeyWrk.vOffsets(nOffsets);
    
    pTempBuffer->aInsertKeyAbsL0(xKeyWrk,&aPriorEntry);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Item* aThisWilderness = aAdd<Item*>(m_aHeadEntry,
                                      m_aHeadEntry->nSizeEntry() +
                                      m_aHeadEntry->iNEOsize());
  vResetUsed(aThisWilderness);
  vSetWilderness();
  vItems(0);

  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexOffsets; i_Offset++)
    m_aHeadEntry->vNEO(i_Offset,-1);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  aPriorEntry = pThisBuffer->aHeadEntry();
  
  for (Entry* aTempEntry = pTempBuffer->aFirstEntry();
       aTempEntry != nullptr;
       aTempEntry = pTempBuffer->aNextEntry(aTempEntry, 0)) {
    
    i8 iTxId = aTempEntry->iTxId();
    Id xId = aTempEntry->xId();
    Key* aKey = aTempEntry->aKey();
    i8 nKeySize = aKey->nKeySize(pIdxColVec);
    i8 nOffsets = aTempEntry->nOffsets();
    
    xKeyWrk.vTxId(iTxId);
    xKeyWrk.vId(xId);
    xKeyWrk.vEmbedded((char*)aKey);
    xKeyWrk.vKeySize(nKeySize);
    xKeyWrk.vOffsets(nOffsets);
    
    pThisBuffer->aInsertKeyAbsL0(xKeyWrk,&aPriorEntry);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pThisBuffer->vBuildSkipListSorted();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pTempBuffer;
}

/*----------------------------------------------------------------------------*/
/* cut entry from skip list                                                   */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vCutEntry(Entry* aEntry, Entry* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nOffsets = aEntry->nOffsets();
  
  for (i8 i_Offset = 0; i_Offset < nOffsets; i_Offset++) {
    i4 iNEO = aEntry->iNEO(i_Offset);
    aPriorVec[i_Offset]->vNEO(i_Offset,iNEO);
  }
}

/*----------------------------------------------------------------------------*/
/* delete upwards                                                             */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDeleteUpwards(Key* aDeleteKey,
                                 EntryBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef MB_WATCH
  if (mbWatch) {
    EntryBuffer* pRootKeyBuffer = *ppRootKeyBuffer;
    IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
    vTrace("\n[%lld] vDeleteUpwards: "_Stz,m_iBufferNo);
    aDeleteKey->vShow(pIdxColVec);
    vTrace("\n"_Stz);
    pRootKeyBuffer->vShowTree();
    int iDebug = 1;
  }
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Entry* aPriorEntry = pPositionPriorEntry(aDeleteKey);
  if (!aPriorEntry)
    return;
  
  Entry* aEntryToBeDeleted = aNextEntry(aPriorEntry,0);
  if (!aEntryToBeDeleted)
    return;

//  vTrace("[%lld] pDeleteEntry key %lld\n"_Stz,
//         *nBufferNo(),
//         pEntryToBeDeleted->aKey()->iGetInt());
  
  Key* aDeleteEntryKey = aEntryToBeDeleted->aKey();
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  if ( ! aDeleteEntryKey->bEqualTo(aDeleteKey,pIdxColVec) ) {
    
    //vShow();
    
    //vShowTree();
    
    //vTrace("Delete key: "_Stz);
    //aDeleteKey->vShow(pIdxColVec);
    //vTrace("\n"_Stz);
    
    //vTrace("Delete entry key found: "_Stz);
    //aDeleteEntryKey->vShow(pIdxColVec);
    //vTrace("\n"_Stz);
    
    return;
  }
  //RASSERT(aDeleteEntryKey->bEqualTo(aDeleteKey,pIdxColVec));
  
  Entry* aFirstEntry = this->aFirstEntry();
  if (aEntryToBeDeleted == aFirstEntry && m_pHigherBuffer)
    m_pHigherBuffer->vDeleteUpwards(aDeleteKey,ppRootKeyBuffer);

  vDeleteEntry(&aEntryToBeDeleted);
}

/*----------------------------------------------------------------------------*/
/* delete upwards                                                             */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDeleteUpwards(Key* aDeleteKey, EntryBuffer* pToBuffer,
                                 EntryBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Entry* xPriorVec[gpAttachMap->m_nIndexOffsets];

  vPositionPriorVecBuffer(aDeleteKey,xPriorVec,pToBuffer);
  
  Entry* aPriorEntry = xPriorVec[0];

  Entry* aEntryToBeDeleted = aNextEntry(aPriorEntry,0);
  if (! aEntryToBeDeleted)
    return;
  
  //  vTrace("[%lld] pDeleteEntry key %lld\n"_Stz,
  //         *nBufferNo(),
  //         pEntryToBeDeleted->aKey()->iGetInt());
  
  Key* aDeleteEntryKey = aEntryToBeDeleted->aKey();
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  //if (! aDeleteEntryKey->bEqualTo(aDeleteKey,pIdxColVec))
  //  return;
  RASSERT(aDeleteEntryKey->bEqualTo(aDeleteKey,pIdxColVec));
  RASSERT(pToBuffer == aEntryToBeDeleted->pReferencedEntryBuffer());
  
  Entry* aFirstEntry = this->aFirstEntry();
  if (aEntryToBeDeleted == aFirstEntry && m_pHigherBuffer)
    m_pHigherBuffer->vDeleteUpwards(aDeleteKey,this,ppRootKeyBuffer);
  
  vDeleteEntry(&aEntryToBeDeleted);
}

/*----------------------------------------------------------------------------*/
/* dump the given prior vector                                                */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDumpPriorVec(Entry* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  vTrace("Prior Vector\n"_Stz);
  
  vTrace("  0:%p 1:%p 2:%p\n"_Stz,
         aPriorVec[0],aPriorVec[1],aPriorVec[2]);
  
  for (i8 i = 0; i < 3; i++) {
    Entry* aEntry = aPriorVec[i];
    for (i8 i_Offset = 0; i_Offset < aEntry->nOffsets(); i_Offset++) {
      vTrace("  i : %lld Entry : %p i_Offset : %lld offset : %lld pointsTo : %p\n"_Stz,
             i,
             aEntry,
             i_Offset,
             aEntry->iNEO(i_Offset),
             aGetEntryOffset(aEntry->iNEO(i_Offset)));
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get first free entry                                                       */
/*----------------------------------------------------------------------------*/
FreeEntry* EntryBuffer::aFirstFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_aFreeEntryList;
}

/*----------------------------------------------------------------------------*/
/* get first item after head (could be entry, free entry or wilderness)       */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aFirstItem(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Item* aFirstItem = aNextItem(m_aHeadEntry);
  
  return aFirstItem;
}

/*----------------------------------------------------------------------------*/
/* get an entry by offset                                                     */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aGetEntryOffset(i8 iOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  char* aAddress = aAdd<char*>(aDataArea(), iOffset);
  Entry* aEntry = reinterpret_cast<Entry*>(aAddress);
  
  return aEntry;
}

/*----------------------------------------------------------------------------*/
/* get split entry by split number                                            */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aGetEntrySplit(i8 iSplitEntryNo,Entry** aaPriorEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(m_aHeadEntry);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  *aaPriorEntry = nullptr;
  
  i8 iEntryNo = 0;
  for (Entry* aSplitEntry = aNextEntry(m_aHeadEntry,0);
       aSplitEntry;
       aSplitEntry = aNextEntry(aSplitEntry,0)) {
    
    // found split point?
    if (iEntryNo++ == iSplitEntryNo)
      return aSplitEntry;
    
    *aaPriorEntry = aSplitEntry;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a free entry (or C_ENTRY_NONE if none)                                 */
/*----------------------------------------------------------------------------*/
i4 EntryBuffer::iGetFreeRowEntry(void)
{
  RASSERT( (m_i_FreeOffsetChain >= 0 && m_i_FreeOffsetChain < C_ENTRIES_L0) ||
           (C_ENTRY_NONE == m_i_FreeOffsetChain) );
  
  if (C_ENTRY_NONE == m_i_FreeOffsetChain)
    return C_ENTRY_NONE;
  
  i4 i_FreeEntry = m_i_FreeOffsetChain;
  
  RASSERT(i_FreeEntry < C_ENTRIES_L0);
  m_i_FreeOffsetChain = - m_aEntryOffsets[i_FreeEntry];
  
  return i_FreeEntry;
}

/*----------------------------------------------------------------------------*/
/* handle the insert of a first key                                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vHandleFirstInsert(Entry* aInsertedKeyEntry,
                                     Entry* aOriginalFirstEntry,
                                     EntryBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (aFirstEntry() != aInsertedKeyEntry)
    return;
    
  if (aOriginalFirstEntry) {
    Key* aOriginalFirstKey = aOriginalFirstEntry->aKey();
    Key* aInsertedFirstKey = aInsertedKeyEntry->aKey();
    
    IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
    
    if (aInsertedFirstKey->bNotEqual(aOriginalFirstKey,pIdxColVec))
      vHandleLeftInsert(aInsertedKeyEntry,
                        aOriginalFirstEntry,
                        ppRootKeyBuffer);
  }
  else {
    vHandleLeftInsert(aInsertedKeyEntry,
                      aOriginalFirstEntry,
                      ppRootKeyBuffer);
  }
}

/*----------------------------------------------------------------------------*/
/* handle the insert of a left most key                                       */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vHandleLeftInsert(Entry* aInsertedKeyEntry,
                                    Entry* aOriginalFirstEntry,
                                    EntryBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (! m_pHigherBuffer)
    return;
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef MB_WATCH
  if (mbWatch) {
    EntryBuffer* pRootKeyBuffer = *ppRootKeyBuffer;
    IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
    vTrace("\n[%lld] vHandleLeftInsert: Inserted "_Stz,m_iBufferNo);
    pInsertedKeyEntry->aKey()->vShow(pIdxColVec);
    vTrace(" Orginial "_Stz);
    pOriginalFirstEntry->aKey()->vShow(pIdxColVec);
    vTrace("\n"_Stz);
    pRootKeyBuffer->vShowTree();
    int iDebug = 1;
  }
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (aOriginalFirstEntry) {
    Key* aDeleteKey = aOriginalFirstEntry->aKey();
    m_pHigherBuffer->vDeleteUpwards(aDeleteKey,ppRootKeyBuffer);
    //(*ppRootKeyBuffer)->vShowTree();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Key* aInsertKey = aInsertedKeyEntry->aKey();
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  Id xInsertId(*nBufferNo(),0);
  i8 iInsertKeyLen = aInsertKey->nKeySize(pIdxColVec);
  i8 nInsertOffsets = aInsertedKeyEntry->nOffsets();
  
  i8 iTxId = aInsertedKeyEntry->iTxId();
  
  KeyWrk xKeyWrkIns;
  xKeyWrkIns.vTxId(iTxId);
  xKeyWrkIns.vId(xInsertId);
  xKeyWrkIns.vEmbedded((char*)aInsertKey);
  xKeyWrkIns.vKeySize(iInsertKeyLen);
  xKeyWrkIns.vOffsets(nInsertOffsets);
  
  m_pHigherBuffer->aInsertKeyBuffer(xKeyWrkIns,ppRootKeyBuffer);
}

/*----------------------------------------------------------------------------*/
/* get head entry                                                             */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aHeadEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_aHeadEntry;
}

/*----------------------------------------------------------------------------*/
/* create a head entry                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vHeadEntryNew(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
 
  RowWrk xHeadWrk;
  
  ASSERT( ! m_aHeadEntry );
  
  // the head entry always has 3 offsets
  xHeadWrk.vOffsets(3);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute required entry space
  
  i8 nEntryBytes = xHeadWrk.nEntrySize();
  i8 nOffsetBytes = xHeadWrk.nOffsetSize();
  i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;
  
  ASSERT(bHasSpace(nEntryPlusOffsets));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  xHeadWrk.xId().vBuffer(*nBufferNo());
  
  m_aHeadEntry = reinterpret_cast<Entry*>(aAllotSpace(nEntryBytes));
  ASSERT(m_aHeadEntry);
  //char* aData = aDataArea();                                //! RAY to look at
  //char* aHead = pAdd<char *>(aData,sizeof(i4) * C_ENTRIES_0);
  
  //ASSERT((char*)m_aHeadEntry == aHead);
  
  new(m_aHeadEntry) Entry(CN_ENTRY,nEntryBytes,0);
  
  m_aHeadEntry->vSetEntry(0,xHeadWrk.xId(),3,nullptr,0);
  //m_aHeadEntry->vSetEntry(xHeadWrk);
  
  NEO* aNEO = aAllotNEO(nOffsetBytes);
  m_aHeadEntry->vNEO(0,-1);
  m_aHeadEntry->vNEO(1,-1);
  m_aHeadEntry->vNEO(2,-1);
  
  //m_aHeadEntry->vNEOdump();
  
}

/*----------------------------------------------------------------------------*/
/* restore the head entry                                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vHeadEntryRestore(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_aHeadEntry = aAdd<Entry*>(aDataArea(),sizeof(i4) * C_ENTRIES_L0);
}

/*----------------------------------------------------------------------------*/
/* set up the initial entry offset free chain                                 */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vInitFreeChain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 L_ENTRIES_0 = gpAttachMap->m_nIndexEntries ?
                     gpAttachMap->m_nIndexEntries : C_ENTRIES_L0;
  
  // L0 free chain
  for (i4 i_RowNo = 0; i_RowNo < (L_ENTRIES_0 - 1); i_RowNo++) {
    RASSERT(i_RowNo >= 0 && i_RowNo < C_ENTRIES_L0);
    m_aEntryOffsets[i_RowNo] = - (i_RowNo+1);
  }
  
  RASSERT((L_ENTRIES_0 - 1) >= 0 && (L_ENTRIES_0 - 1) < C_ENTRIES_L0);
  m_aEntryOffsets[L_ENTRIES_0 - 1] = - C_ENTRY_NONE;
  
  m_i_FreeOffsetChain = 0;
}

/*----------------------------------------------------------------------------*/
/* get an insertable buffer                                                   */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pInsertableBuffer(KeyWrk& xKeyWrk,
                                            EntryBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute required entry space

  RASSERT(xKeyWrk.nOffsets());
  
  i8 nEntryBytes = xKeyWrk.nEntrySize();
  i8 nOffsetBytes = xKeyWrk.nOffsetSize();
  i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert key into this EntryBuffer if it has space or get a new one
  
  Key* aInsKey = xKeyWrk.aKey();
  
  EntryBuffer* pInsertIntoBuffer;
  if (bHasSpace(nEntryPlusOffsets))
    pInsertIntoBuffer = this;
  else {
    pInsertIntoBuffer =
      pNewInsertBuffer(aInsKey,nEntryPlusOffsets,ppRootKeyBuffer);
    //(*ppRootKeyBuffer)->vShow();
  }
 
  return pInsertIntoBuffer;
}

/*----------------------------------------------------------------------------*/
/* insert a key entry specifically into this buffer (should be enough space)  */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aInsertKeyAbsLazy(KeyWrk& xKeyWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // trace
  
  //vTrace("Inserting key:\n"_Stz);
  //IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  //aKey->vShow(pIdxColVec);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute required entry space
  
  RASSERT(xKeyWrk.nOffsets());
  
  i8 nEntryBytes = xKeyWrk.nEntrySize();
  i8 nOffsetBytes = xKeyWrk.nOffsetSize();
  i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;
  
  RASSERT(bHasSpace(nEntryPlusOffsets));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate space for the key and next offsets in this buffer
  
  Entry* aKeyEntry = aAllotEntry(nEntryBytes);
  ASSERT(aKeyEntry);
  
  NEO* aNEO = aAllotNEO(nOffsetBytes);
  ASSERT(aNEO);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the key entry
  
  aKeyEntry->vSetEntry(xKeyWrk);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form lazy chain
  
  if (m_aLazyChain) {
    i4 iLazyOffset = iToOffset(m_aLazyChain);
    aKeyEntry->vNEO(0,iLazyOffset);
  }
  else {
    aKeyEntry->vNEO(0,-1);
  }

  m_aLazyChain = aKeyEntry;
  
  return aKeyEntry;
}

/*----------------------------------------------------------------------------*/
/* insert key entry into skip list of this buffer                             */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vInsertKeySkip(Entry* aKeyEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the entry after which the new entry will be inserted
  
  Entry* xPriorVec[gpAttachMap->m_nIndexOffsets];
  
  Key* aKey = aKeyEntry->aKey();
  
  if (0 == m_iBufferLevel)
    vPositionPriorVecEntry(aKey, xPriorVec, nullptr);
  else {
    EntryBuffer* pToBuffer = aKeyEntry->pReferencedEntryBuffer();
    vPositionPriorVecBuffer(aKey, xPriorVec, pToBuffer);
  }
  
  //vDumpPriorVec(xPriorVec);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set prior offsets
  
  i4 iKeyEntryOffset = iToOffset(aKeyEntry);
  
  i8 nOffsets = aKeyEntry->nOffsets();
  for (i8 iNextOffset = 0; iNextOffset < nOffsets; iNextOffset++) {
    i4 iPriorOffset = xPriorVec[iNextOffset]->iNEO(iNextOffset);
    aKeyEntry->vNEO(iNextOffset,iPriorOffset);
    xPriorVec[iNextOffset]->vNEO(iNextOffset,iKeyEntryOffset);
  }
  
  //vShow();
}

/*----------------------------------------------------------------------------*/
/* insert a new key entry into this buffer                                    */
/*----------------------------------------------------------------------------*/
/*
Entry* EntryBuffer::pInsertKeyBuffer(KeyWrk& xKeyWrk,
                                     EntryBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef MB_WATCH
  if (mbWatch) {
    EntryBuffer* pRootKeyBuffer = *ppRootKeyBuffer;
    IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
    vTrace("\n[%lld] pInsertKeyBuffer: "_Stz,m_iBufferNo);
    xKeyWrk.vShow(pIdxColVec);
    pRootKeyBuffer->vShowTree();
    int iDebug = 1;
  }
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // record orginal first entry incase we insert a new first entry
  
  Entry* aOriginalFirstEntry = aFirstEntry();
  if (!pOriginalFirstEntry)
    pOriginalFirstEntry = aFirstEntry();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the key
  
  Entry* aInsertedKeyEntry = pInsertKeyAbs(xKeyWrk);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle the insert of a left most key
 
#ifdef EB_LAZY_CHAIN
  if (m_iBufferLevel)
    vHandleFirstInsert(pInsertedKeyEntry,pOriginalFirstEntry,ppRootKeyBuffer);
#else
  vHandleFirstInsert(pInsertedKeyEntry,pOriginalFirstEntry,ppRootKeyBuffer);
#endif
  
  return pInsertedKeyEntry;
}
*/

/*----------------------------------------------------------------------------*/

Entry* EntryBuffer::aInsertKeyBuffer(KeyWrk& xKeyWrk,
                                     EntryBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef MB_WATCH
  if (mbWatch) {
    EntryBuffer* pRootKeyBuffer = *ppRootKeyBuffer;
    IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
    vTrace("\n[%lld] pInsertKeyBuffer: "_Stz,m_iBufferNo);
    xKeyWrk.vShow(pIdxColVec);
    pRootKeyBuffer->vShowTree();
    int iDebug = 1;
  }
#endif
  
  Entry* aInsertedKeyEntry;

#ifdef EB_LAZY_CHAIN
  if (m_pIndex->pTable()->bIsLoading()) {
    if (m_iBufferLevel) {
      Entry* aOriginalFirstEntry = aFirstEntry();
      aInsertedKeyEntry = aInsertKeyAbs(xKeyWrk);
      vHandleFirstInsert(aInsertedKeyEntry,aOriginalFirstEntry,ppRootKeyBuffer);
    }
    else {
      aInsertedKeyEntry = aInsertKeyAbs(xKeyWrk);
    }
  }
  else {
    Entry* aOriginalFirstEntry = aFirstEntry();
    aInsertedKeyEntry = aInsertKeyAbs(xKeyWrk);
    vHandleFirstInsert(aInsertedKeyEntry,aOriginalFirstEntry,ppRootKeyBuffer);
  }
#else
  Entry* aOriginalFirstEntry = aFirstEntry();
  pInsertedKeyEntry = pInsertKeyAbs(xKeyWrk);
  vHandleFirstInsert(pInsertedKeyEntry,pOriginalFirstEntry,ppRootKeyBuffer);
#endif
  
  return aInsertedKeyEntry;
}

/*----------------------------------------------------------------------------*/
/* insert a new key entry into this key tree (handling ripple upwards)        */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aInsertKeyUpwards(KeyWrk& xKeyWrk,
                                      EntryBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef MB_WATCH
  if (mbWatch) {
    EntryBuffer* pRootKeyBuffer = *ppRootKeyBuffer;
    IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
    vTrace("\n[%lld] pInsertKeyUpwards: "_Stz,m_iBufferNo);
    xKeyWrk.vShow(pIdxColVec);
    pRootKeyBuffer->vShowTree();
    int iDebug = 1;
  }
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  EntryBuffer* pInsertIntoBuffer =
    pInsertableBuffer(xKeyWrk,ppRootKeyBuffer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Entry* aInsertedKeyEntry =
    pInsertIntoBuffer->aInsertKeyBuffer(xKeyWrk,ppRootKeyBuffer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pInsertIntoBuffer->m_iBufferLevel) {
    Id xId = aInsertedKeyEntry->xId();
    i8 nBuffer = xId.iBuffer();
    
    HeapBuffer* pHeapBuffer =
    reinterpret_cast<HeapBuffer*>(gpBufferPool->pToBuffer(nBuffer));
    
    EntryBuffer* pChildBuffer =
    reinterpret_cast<EntryBuffer*>(pHeapBuffer->aFirstChunk());
    
    if (pChildBuffer->m_pHigherBuffer != pInsertIntoBuffer) {
      //(*ppRootKeyBuffer)->vShowTree();
      pChildBuffer->m_pHigherBuffer = pInsertIntoBuffer;
      //(*ppRootKeyBuffer)->vShowTree();
    }
  }
  
  return aInsertedKeyEntry;
}

/*----------------------------------------------------------------------------*/
/* restore item count                                                         */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vItemCountRestore(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_nItems = 0;
  for (Entry* aEntry = aFirstEntry(); aEntry; aEntry = aNextEntry(aEntry, 0))
    m_nItems++;
}

/*----------------------------------------------------------------------------*/
/* is given key before the first key of this buffer                           */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bKeyBeforeBuffer(Key* aKeyTest)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Entry* aFirstEntry = this->aFirstEntry();
  Key* aFirstKey = aFirstEntry->aKey();
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  i8 nKeyCols = pIdxColVec->nItems();
  
  bool bIsEqual;
  bool bIsJuxta =
  //aKeyTest->bJuxtaCompare(JC_BEFORE,aFirstKey,pIdxColVec,nKeyCols,bIsEqual);
    aKeyTest->bLessThan(aFirstKey,pIdxColVec);
  
  return bIsJuxta;
}

/*----------------------------------------------------------------------------*/
/* build skip lists from the lazy chain of all L0 buffers                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vLazyBuild(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  EntryBuffer* pEntryBuffer = this;
  
  while (pEntryBuffer) {
    
    RASSERT(pEntryBuffer->iBufferLevel() == 0);
    
    //vTrace("Building skip list %p\n"_Stz, pEntryBuffer);
    
    pEntryBuffer->vBuildSkipList();
    
    pEntryBuffer = pEntryBuffer->pNextBuffer();
  }
}

/*----------------------------------------------------------------------------*/
/* convert given entry into free entry                                        */
/*----------------------------------------------------------------------------*/
FreeEntry* EntryBuffer::aMakeFreeEntry(Entry* aEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nSizeEntry = aEntry->nSizeEntry();
  i8 nSizeNEO   = aEntry->iNEOsize();
  i8 nSizeFree  = nSizeEntry + nSizeNEO;
  
  FreeEntry* aFreeEntry = new(aEntry) FreeEntry(nSizeFree);

  return aFreeEntry;
}

/*----------------------------------------------------------------------------*/
/* create a new higher buffer                                                 */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pNewHigherBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // create new buffer at one higher index level
  EntryBuffer* pNewBuffer = new EntryBuffer();
  
  pNewBuffer->vIndex(m_pIndex);
  pNewBuffer->vBufferLevel(m_iBufferLevel+1);
  pNewBuffer->vPartitionNo(m_nPartitionNo);
  
  vHigherBuffer(pNewBuffer);
  
  return pNewBuffer;
}

/*----------------------------------------------------------------------------*/
/* create a new insert buffer (adjusting higher buffers accordingly)          */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pNewInsertBuffer(Key* aInsKey, i8 nEntryPlusOffsets,
                                           EntryBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compact this buffer and check if it has enough space
  
  this->vCompact();
  
  if (this->bHasSpace(nEntryPlusOffsets) )
    return this;
  
  EntryBuffer* pLeftBuffer = this;

  
  // ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
  // experiment to see if building the skip list (sorted) works
  
  /*
  vShow();
  vBuildSkipListSorted();
  vShow();
  */
  
  // ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the higher buffer, creating one if it doesnt exist
  
  EntryBuffer* pHigherBuffer = this->pHigherBuffer();
  if (! pHigherBuffer) {
    pHigherBuffer = this->pNewHigherBuffer();
    *ppRootKeyBuffer = pHigherBuffer;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // split the left buffer to yield a right buffer

#ifdef EB_SPLIT2
  EntryBuffer* pRightBuffer = pLeftBuffer->pSplit2();
#else
  EntryBuffer* pRightBuffer = pLeftBuffer->pSplit();
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the first key of left buffer into higher buffer
  
  if ( ! pHigherBuffer->nItems() ) {
    Entry* aFirstKeyLeft  = pLeftBuffer->aFirstEntry();
    
    Id xIdLeft(*pLeftBuffer->nBufferNo(),0);
    i8 iTxIdLeft = aFirstKeyLeft->iTxId();
    Key* aKeyLeft = aFirstKeyLeft->aKey();
    i8 iKeyLenLeft = aKeyLeft->nKeySize(m_pIndex->pIdxColVec());
    i8 nOffsetsLeft = aFirstKeyLeft->nOffsets();
    
    
    KeyWrk xKeyWrkLeft;
    xKeyWrkLeft.vTxId(iTxIdLeft);
    xKeyWrkLeft.vId(xIdLeft);
    xKeyWrkLeft.vEmbedded((char*)aKeyLeft);
    xKeyWrkLeft.vKeySize(iKeyLenLeft);
    xKeyWrkLeft.vOffsets(nOffsetsLeft);
    
    
    Entry* aNewKeyLeft =
    pHigherBuffer->aInsertKeyUpwards(xKeyWrkLeft,ppRootKeyBuffer);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the first key of right buffer into higher buffer
  
  Entry* aFirstKeyRight = pRightBuffer->aFirstEntry();
  
  Id xIdRight(*pRightBuffer->nBufferNo(),0);
  i8 iTxIdRight = aFirstKeyRight->iTxId();
  Key* aKeyRight = aFirstKeyRight->aKey();
  i8 iKeyLenRight = aKeyRight->nKeySize(m_pIndex->pIdxColVec());
  i8 nOffsetsRight = aFirstKeyRight->nOffsets();
  
  
  KeyWrk xKeyWrkRight;
  xKeyWrkRight.vTxId(iTxIdRight);
  xKeyWrkRight.vId(xIdRight);
  xKeyWrkRight.vEmbedded((char*)aKeyRight);
  xKeyWrkRight.vKeySize(iKeyLenRight);
  xKeyWrkRight.vOffsets(nOffsetsRight);
  
  
  Entry* aNewKeyRight =
  pHigherBuffer->aInsertKeyUpwards(xKeyWrkRight,ppRootKeyBuffer);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //ASSERT( (* ppRootKeyBuffer)->bVerify() );
  
  if ( pRightBuffer->bKeyBeforeBuffer(aInsKey) )
    return pLeftBuffer;
  else
    return pRightBuffer;
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
  pNewBuffer->vBufferLevel(m_iBufferLevel);
  pNewBuffer->vPartitionNo(m_nPartitionNo);
  
  pNewBuffer->vHigherBuffer(m_pHigherBuffer);
  pNewBuffer->vPrevBuffer(this);
  pNewBuffer->vNextBuffer(m_pNextBuffer);
  if (m_pNextBuffer)
    m_pNextBuffer->vPrevBuffer(pNewBuffer);
  
  m_pNextBuffer = pNewBuffer;
  
  ASSERT(this == pNewBuffer->pPrevBuffer());
  
  return pNewBuffer;
}

/*----------------------------------------------------------------------------*/
/* get next item in buffer (could be entry, free entry, wilderness)           */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aNextItem(Item* aCurrent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Item* aItemNext = nullptr;
  
  eCN eClass = aCurrent->eClass();
  
  switch (eClass) {
    case CN_ENTRY: {
      Entry* aEntryCurr = static_cast<Entry*>(aCurrent);
      aItemNext = reinterpret_cast<Item*>(aEntryCurr->aAddressAfter());
      break;
    }
      
    case CN_FREEENTRY: {
      FreeEntry* aFreeEntryCurr = static_cast<FreeEntry*>(aCurrent);
      aItemNext = reinterpret_cast<Item*>(aFreeEntryCurr->aAddressAfter());
      break;
    }
      
    case CN_WILDERNESS:
      break;
      
    default:
      vShow();
      ASSERT(0);
      break;
  }
  
  return aItemNext;
}

/*----------------------------------------------------------------------------*/
/* pop entry from lazy chain                                                  */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aPopLazyChain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (! m_aLazyChain )
    return nullptr;
  
  Entry* aPoppedLazyEntry = m_aLazyChain;
  
  i8 iNextLazyOffset = aPoppedLazyEntry->iNEO(0);
  
  if (-1 == iNextLazyOffset)
    m_aLazyChain = nullptr;
  else
    m_aLazyChain = aGetEntryOffset(iNextLazyOffset);
  
  return aPoppedLazyEntry;
}

/*----------------------------------------------------------------------------*/
/* position at the L0 buffer into which key should be inserted                */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pPositionPriorBuffer(Key* aFindKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if ( ! nItems() ) {
    ASSERT( 0 == iBufferLevel());
    return this;
  }
  
  EntryBuffer* pEntryBuffer = this;
  while (true) {
    
    // have we arrived at L0 ?
    i8 iBufferLevel = pEntryBuffer->iBufferLevel();
    if (iBufferLevel == 0) {
      break;
    }
    else {
#ifdef EB_LAZY_CHAIN
      RASSERT( ! m_aLazyChain );
#endif
    }
    
    // get the entry before the given insert key
    Entry* aEntryPrior = pEntryBuffer->pPositionPriorEntry(aFindKey);
    ASSERT(aEntryPrior);
    if (aEntryPrior == pEntryBuffer->aHeadEntry())
      aEntryPrior = pEntryBuffer->aNextEntry(aEntryPrior,0);
    ASSERT(aEntryPrior);
    
    // get the index buffer of the entry
    //pEntryBuffer->vShow();
    ASSERT(pEntryBuffer != aEntryPrior->pReferencedEntryBuffer());
    pEntryBuffer = aEntryPrior->pReferencedEntryBuffer();
    ASSERT(pEntryBuffer);
    pEntryBuffer->vAssertEntryBuffer();
  }
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* scan thru skip list and return entry prior to key                          */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pPositionPriorEntry(Key* aFindKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (nItems() == 0)
    return m_aHeadEntry;
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  i8 nKeyCols = pIdxColVec->nItems();
  
  Entry* aScanEntry_sl = m_aHeadEntry;
  
  bool bKeyIsGreaterEqual;
  
  for (i8 i_ScanOffset = gpAttachMap->m_nIndexOffsets - 1;
       i_ScanOffset >= 0;
       i_ScanOffset--) {
    
    Entry* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanOffset);
    while (aNextEntry_sl) {
      
      Key* aNextKey = aNextEntry_sl->aKey();
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
#ifdef EB_SINGLE_COLIDX
      if (m_pIndex->bSingleColumn()) {
        bKeyIsGreaterEqual = aNextKey->bGreaterEqualSingle(aFindKey,m_pIndex);
        //RASSERT(bKeyIsGreaterEqual == aNextKey->bGreaterEqual(aFindKey,pIdxColVec));
      }
      else
        bKeyIsGreaterEqual = aNextKey->bGreaterEqual(aFindKey,pIdxColVec);
#else
      bKeyIsGreaterEqual = aNextKey->bGreaterEqual(aFindKey,pIdxColVec);
#endif
      
      if (bKeyIsGreaterEqual)
        break;
      
      aScanEntry_sl = aNextEntry_sl;
      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanOffset);
    }
  }
  
  return aScanEntry_sl;
}

/*----------------------------------------------------------------------------*/
/* compute prior vector for find key, such that next of priorVec[0] is        */
/* specifically the given find entry                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vPositionPriorVecEntry(Key* aFindKey, Entry* aPriorVec[],
                                         Entry* aFindEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //ASSERT(0 == m_iBufferLevel);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexOffsets; i_Offset++)
    aPriorVec[i_Offset] = m_aHeadEntry;
  
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  i8 iFindLevels = aFindEntry ? aFindEntry->nOffsets() - 1 : 2;
  
  bool bIsJuxta;
  Entry* aScanEntry_sl = m_aHeadEntry;
  for (i8 i_ScanOffset = gpAttachMap->m_nIndexOffsets - 1;
       i_ScanOffset >= 0;
       i_ScanOffset--) {
    
    Entry* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanOffset);
    while (aNextEntry_sl) {
      
      if (aFindEntry == aNextEntry_sl)
        break;
      
      Key* aNextKey = aNextEntry_sl->aKey();
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
#ifdef EB_SINGLE_COLIDX
      if (m_pIndex->bSingleColumn()) {
        bIsJuxta = aNextKey->bGreaterEqualSingle(aFindKey,m_pIndex);
//        if (bIsJuxta != aNextKey->bGreaterEqual(aFindKey,pIdxColVec)) {
//          aNextKey->bGreaterEqualSingle(aFindKey,m_pIndex);
//          aNextKey->bGreaterEqual(aFindKey,pIdxColVec);
//        }
      }
      else
        bIsJuxta = aNextKey->bGreaterEqual(aFindKey,pIdxColVec);
#else
      bIsJuxta = aNextKey->bGreaterEqual(aFindKey,pIdxColVec);
#endif
      
      if (bIsJuxta) {
        if (! aFindEntry || aNextEntry_sl == aFindEntry)
          break;
        
        if (i_ScanOffset > iFindLevels)
          break;
      }
      
      aScanEntry_sl = aNextEntry_sl;
      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanOffset);
    }
    aPriorVec[i_ScanOffset] = aScanEntry_sl;
  }
}

/*----------------------------------------------------------------------------*/
/* compute prior vec for find key, such that next of priorVec[0] is the next  */
/* buffer after find keys toBuffer (ie nextBuffer->prevBuffer == toBuffer)    */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vPositionPriorVecBuffer(Key* aFindKey, Entry* aPriorVec[],
                                          EntryBuffer* pToBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(0 != m_iBufferLevel);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexOffsets; i_Offset++)
    aPriorVec[i_Offset] = m_aHeadEntry;
  
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  bool bNextKeyIsGreater;
  Entry* aScanEntry_sl = m_aHeadEntry;
  
  for (i8 i_ScanOffset = gpAttachMap->m_nIndexOffsets - 1;
       i_ScanOffset >= 0;
       i_ScanOffset--) {
    
    Entry* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanOffset);
    while (aNextEntry_sl) {
      
      Key* aNextKey = aNextEntry_sl->aKey();
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
#ifdef EB_SINGLE_COLIDX
      if (m_pIndex->bSingleColumn()) {
        bNextKeyIsGreater = aNextKey->bGreaterThanSingle(aFindKey,m_pIndex);
        RASSERT(bNextKeyIsGreater == aNextKey->bGreaterThan(aFindKey,pIdxColVec));
      }
      else
        bNextKeyIsGreater = aNextKey->bGreaterThan(aFindKey,pIdxColVec);
#else
      bNextKeyIsGreater = aNextKey->bGreaterThan(aFindKey,pIdxColVec);
#endif
      
      if (bNextKeyIsGreater)
        break;
      
      if (0 == i_ScanOffset) {
        bool bKeysAreEqual = aNextKey->bEqualTo(aFindKey,pIdxColVec);
        if (bKeysAreEqual &&
            (pToBuffer == aNextEntry_sl->pReferencedEntryBuffer()->pPrevBuffer()))
          break;
      }
      
      aScanEntry_sl = aNextEntry_sl;
      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanOffset);
    }
    aPriorVec[i_ScanOffset] = aScanEntry_sl;
  }
}

/*----------------------------------------------------------------------------*/
/* compute prior vec for find key, such that next of priorVec[0] is the next  */
/* buffer after find keys toBuffer (ie nextBuffer->prevBuffer == toBuffer)    */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vPositionPriorVecBuffer2(Key* aFindKey, Entry* aPriorVec[],
                                           EntryBuffer* pToBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(0 != m_iBufferLevel);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexOffsets; i_Offset++)
    aPriorVec[i_Offset] = m_aHeadEntry;
  
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  bool bNextKeyIsGreaterEqual;
  Entry* aScanEntry_sl = m_aHeadEntry;
  
  for (i8 i_ScanOffset = gpAttachMap->m_nIndexOffsets - 1;
       i_ScanOffset >= 0;
       i_ScanOffset--) {
    
    Entry* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanOffset);
    while (aNextEntry_sl) {
      
      Key* aNextKey = aNextEntry_sl->aKey();
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
      bNextKeyIsGreaterEqual = aNextKey->bGreaterEqual(aFindKey,pIdxColVec);
      
      if (bNextKeyIsGreaterEqual)
        break;
      
      if (0 == i_ScanOffset) {
        bool bKeysAreEqual = aNextKey->bEqualTo(aFindKey,pIdxColVec);
        if (bKeysAreEqual &&
            (pToBuffer == aNextEntry_sl->pReferencedEntryBuffer()->pPrevBuffer()))
          break;
      }
      
      aScanEntry_sl = aNextEntry_sl;
      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanOffset);
    }
    aPriorVec[i_ScanOffset] = aScanEntry_sl;
  }
}

/*----------------------------------------------------------------------------*/
/* compute prior vec for find key, such that next of priorVec[0] is the next  */
/* buffer after find keys toBuffer (ie nextBuffer->prevBuffer == toBuffer)    */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vPositionPriorVecBuffer3(Key* aFindKey, Entry* aPriorVec[],
                                           EntryBuffer* pToBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(0 != m_iBufferLevel);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexOffsets; i_Offset++)
    aPriorVec[i_Offset] = m_aHeadEntry;
  
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  bool bNextKeyIsGreaterEqual;
  Entry* aScanEntry_sl = m_aHeadEntry;
  
  for (i8 i_ScanOffset = gpAttachMap->m_nIndexOffsets - 1;
       i_ScanOffset >= 0;
       i_ScanOffset--) {
    
    Entry* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanOffset);
    while (aNextEntry_sl) {
      
      Key* aNextKey = aNextEntry_sl->aKey();
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
      bNextKeyIsGreaterEqual = aNextKey->bGreaterEqual(aFindKey,pIdxColVec);
      
      if (0 == i_ScanOffset) {
        bool bKeysAreEqual = aNextKey->bEqualTo(aFindKey,pIdxColVec);
        if (bKeysAreEqual &&
            (pToBuffer == aNextEntry_sl->pReferencedEntryBuffer()))
          break;
      }
      
      if (bNextKeyIsGreaterEqual)
        break;
      
      aScanEntry_sl = aNextEntry_sl;
      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanOffset);
    }
    aPriorVec[i_ScanOffset] = aScanEntry_sl;
  }
  
  Entry* aToBeDelete = aNextEntry(aPriorVec[0],0);
  
  RASSERT(pToBuffer == aToBeDelete->pReferencedEntryBuffer());
}

/*----------------------------------------------------------------------------*/
/* split this buffer into two: this original buffer and a new right buffer    */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pSplit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);

  static i8 nSplit = 0;
  //vTrace("pSplit %lld (%lld items)\n"_Stz,++nSplit,nItems());

  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  EntryBuffer* pThisLeftBuffer = this;
  
  //pThisLeftBuffer->vShow();
  
  //ASSERT( (* ppRootKeyBuffer)->bVerify() );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find split entry. every entry from split entry and to the right of it
  // will be moved to the new right buffer
  
  Entry* aPriorEntry;
  i8 iSplitAtEntry = m_nItems/2;
  Entry* aSplitEntry = aGetEntrySplit(iSplitAtEntry,&aPriorEntry);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the prior vector to the split entry
  
  //Key* aKey = pSplitEntry->aKey();
  //i8 iKeyLength = aKey->nKeySize(pIdxColVec);
  
  //Entry* xPriorVec[gpAttachMap->m_nIndexOffsets];
  /*
  if (0 == m_iBufferLevel) {
    vPositionPriorL0(aKey, xPriorVec, pSplitEntry);
  }
  else {
    //EntryBuffer* pToBuffer = pSplitEntry->pReferencedEntryBuffer();
    //vPositionPriorLnAfter(aKey, xPriorVec, pToBuffer);
    vPositionPriorL0(aKey, xPriorVec, pSplitEntry);
  }
  */
  //vPositionPriorVecEntry(aKey, xPriorVec, pSplitEntry);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a new right buffer
  
  EntryBuffer* pNewRightBuffer = pNewNextBuffer();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // move split entries to a new right buffer
  
  KeyWrk xKeyWrk;
  
  Entry* aNextEntry;
  for (Entry* aMoveEntry = aSplitEntry;
       aMoveEntry;
       aMoveEntry = aNextEntry) {
  
    ASSERT(! m_iBufferLevel ||
           aMoveEntry->pReferencedEntryBuffer()->m_pHigherBuffer == pThisLeftBuffer);

    aNextEntry = this->aNextEntry(aMoveEntry,0);

    i8 iTxId = aMoveEntry->iTxId();
    Id xId = aMoveEntry->xId();
    Key* aKey = aMoveEntry->aKey();
    i8 nKeySize = aKey->nKeySize(pIdxColVec);
    i8 nOffsets = aMoveEntry->nOffsets();
    
    xKeyWrk.vTxId(iTxId);
    xKeyWrk.vId(xId);
    xKeyWrk.vEmbedded((char*)aKey);
    xKeyWrk.vKeySize(nKeySize);
    xKeyWrk.vOffsets(nOffsets);
    
    Entry* aNewEntry = pNewRightBuffer->aInsertKeyAbs(xKeyWrk);
    
    ASSERT(! m_iBufferLevel ||
           aNewEntry->pReferencedEntryBuffer()->m_pHigherBuffer == pThisLeftBuffer);
    
    i8 nBuffer = xId.iBuffer();
    HeapBuffer* pHeapBuffer =
      reinterpret_cast<HeapBuffer*>(gpBufferPool->pToBuffer(nBuffer));
    
    EntryBuffer* pChildBuffer =
      reinterpret_cast<EntryBuffer*>(pHeapBuffer->aFirstChunk());
    
    ASSERT(pChildBuffer->m_pHigherBuffer ==
           aNewEntry->pReferencedEntryBuffer()->m_pHigherBuffer);
    
    vDeleteEntry(&aMoveEntry);
  }
  
  //pThisLeftBuffer->vShow();
  //pNewRightBuffer->vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // the child buffer of any moved entry needs to have its higher buffer
  // corrected to the new right buffer
  
  Id xId;
  if (m_iBufferLevel != 0)  {
    for (Entry* aMovedEntry = pNewRightBuffer->aFirstEntry();
         aMovedEntry;
         aMovedEntry = pNewRightBuffer->aNextEntry(aMovedEntry, 0)) {
      
      xId = aMovedEntry->xId();
      i8 nBuffer = xId.iBuffer();
      
      HeapBuffer* pHeapBuffer =
        reinterpret_cast<HeapBuffer*>(gpBufferPool->pToBuffer(nBuffer));
      
      EntryBuffer* pChildBuffer =
        reinterpret_cast<EntryBuffer*>(pHeapBuffer->aFirstChunk());
      
      ASSERT(pChildBuffer->m_pHigherBuffer ==
             aMovedEntry->pReferencedEntryBuffer()->m_pHigherBuffer);
      
      
      if (pChildBuffer->m_pHigherBuffer != pThisLeftBuffer) {
        //pThisLeftBuffer->vShow();
        //pNewRightBuffer->vShow();
        //pChildBuffer->vShow();
      }
      
      ASSERT(pChildBuffer->m_pHigherBuffer == pThisLeftBuffer);
      pChildBuffer->m_pHigherBuffer = pNewRightBuffer;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compact the buffer
  
  vCompact();
  
  //ASSERT( (* ppRootKeyBuffer)->bVerify() );
  
  return pNewRightBuffer;
}

/*----------------------------------------------------------------------------*/
/* split this buffer into two: this original buffer and a new right buffer    */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pSplit2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 iSplitRatio = 50;
  
  static i8 nSplit = 0;
  //vTrace("pSplit2 %lld (%lld items)\n"_Stz,++nSplit,nItems());
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  EntryBuffer* pThisLeftBuffer = this;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a new right buffer
  
  EntryBuffer* pNewRightBuffer = pNewNextBuffer();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find split entry. every entry from split entry and to the right of it
  // will be moved to the new right buffer
  
  Entry* aPriorEntry;
  //i8 iSplitAtEntry = m_nItems/2;
  i8 iSplitAtEntry = (m_nItems * iSplitRatio) / 100;
  Entry* aSplitEntry = aGetEntrySplit(iSplitAtEntry,&aPriorEntry);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy split right entries to a new right buffer
  
  KeyWrk xKeyWrk;
  Entry* aCopyEntry;
  Entry* aNextEntry;
  
  Entry* aLastEntryRight = pNewRightBuffer->aHeadEntry();
 
  i8 nEntriesCopied = 0;
  for (aCopyEntry = aSplitEntry;
       aCopyEntry;
       aCopyEntry = aNextEntry) {
    
    ASSERT(! m_iBufferLevel ||
           aCopyEntry->pReferencedEntryBuffer()->m_pHigherBuffer == pThisLeftBuffer);
    
    nEntriesCopied++;
    
    aNextEntry = this->aNextEntry(aCopyEntry,0);
    
    i8 iTxId = aCopyEntry->iTxId();
    Id xId = aCopyEntry->xId();
    Key* aKey = aCopyEntry->aKey();
    i8 nKeySize = aKey->nKeySize(pIdxColVec);
    i8 nOffsets = aCopyEntry->nOffsets();
    
    xKeyWrk.vTxId(iTxId);
    xKeyWrk.vId(xId);
    xKeyWrk.vEmbedded((char*)aKey);
    xKeyWrk.vKeySize(nKeySize);
    xKeyWrk.vOffsets(nOffsets);
    
    Entry* aNewEntry = pNewRightBuffer->aInsertKeyAbsL0(xKeyWrk,&aLastEntryRight);
    
    //.pNewRightBuffer->vShow();
    
    // make the moved entry in the left buffer to be a free entry
    FreeEntry* aFreeEntry = aMakeFreeEntry(aCopyEntry);
    
    // these can be commented
    ASSERT(! m_iBufferLevel ||
           aNewEntry->pReferencedEntryBuffer()->m_pHigherBuffer == pThisLeftBuffer);
    
    i8 nBuffer = xId.iBuffer();
    HeapBuffer* pHeapBuffer =
      reinterpret_cast<HeapBuffer*>(gpBufferPool->pToBuffer(nBuffer));
    
    EntryBuffer* pChildBuffer =
      reinterpret_cast<EntryBuffer*>(pHeapBuffer->aFirstChunk());
    
    ASSERT(pChildBuffer->m_pHigherBuffer ==
           aNewEntry->pReferencedEntryBuffer()->m_pHigherBuffer);
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cut off right entries at L0
  
  ASSERT(this->aNextEntry(aPriorEntry,0) == aSplitEntry);
  aPriorEntry->vNEO(0,-1);
  
  // adjust the number of items in the buffer
  i8 nItemsOriginally = pThisLeftBuffer->nItems();
  i8 nRevisedItems = nItemsOriginally - nEntriesCopied;
  pThisLeftBuffer->vItems(nRevisedItems);
  
  // - at this point the left buffer no longer has the right latter entries
  //   and the SkipListNL is not valid (other than L0)
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // build the skip list of the right buffer
  
  pNewRightBuffer->vBuildSkipListSorted();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compact the left buffer and rebuild SkipListNL
  
  pThisLeftBuffer->vCompactLeft2();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // the child buffer of any moved entry needs to have its higher buffer
  // corrected to the new right buffer
  
  Id xId;
  if (m_iBufferLevel != 0)  {
    for (Entry* aMovedEntry = pNewRightBuffer->aFirstEntry();
         aMovedEntry;
         aMovedEntry = pNewRightBuffer->aNextEntry(aMovedEntry, 0)) {
      
      xId = aMovedEntry->xId();
      i8 nBuffer = xId.iBuffer();
      
      HeapBuffer* pHeapBuffer =
      reinterpret_cast<HeapBuffer*>(gpBufferPool->pToBuffer(nBuffer));
      
      EntryBuffer* pChildBuffer =
      reinterpret_cast<EntryBuffer*>(pHeapBuffer->aFirstChunk());
      
      ASSERT(pChildBuffer->m_pHigherBuffer ==
             aMovedEntry->pReferencedEntryBuffer()->m_pHigherBuffer);
      
      
      if (pChildBuffer->m_pHigherBuffer != pThisLeftBuffer) {
        //pThisLeftBuffer->vShow();
        //pNewRightBuffer->vShow();
        //pChildBuffer->vShow();
      }
      
      ASSERT(pChildBuffer->m_pHigherBuffer == pThisLeftBuffer);
      pChildBuffer->m_pHigherBuffer = pNewRightBuffer;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  return pNewRightBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Verify ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* check prior vec ordering                                                   */
/*----------------------------------------------------------------------------*/
#ifdef _DEBUG

bool EntryBuffer::bCheckPriorVec(Entry* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  vNumberEntries();
  
  Entry* aPriorEntry_0 = aPriorVec[0];
  Entry* aPriorEntry_1 = aPriorVec[1];
  Entry* aPriorEntry_2 = aPriorVec[2];
  
  Entry* aNextEntry_0  = aNextEntry(aPriorEntry_0,0);
  Entry* aNextEntry_1  = aNextEntry(aPriorEntry_1,1);
  Entry* aNextEntry_2  = aNextEntry(aPriorEntry_2,2);
  
  if (aNextEntry_0 && aNextEntry_1) {
    if (aNextEntry_0->m_iEntryNo > aNextEntry_1->m_iEntryNo) {
      vVerifyFault();
      return false;
    }
  }
  
  if (aNextEntry_1 && aNextEntry_2) {
    if (aNextEntry_1->m_iEntryNo > aNextEntry_2->m_iEntryNo) {
      vVerifyFault();
      return false;
    }
  }
  
  if (aNextEntry_0 && aNextEntry_2) {
    if (aNextEntry_0->m_iEntryNo > aNextEntry_2->m_iEntryNo) {
      vVerifyFault();
      return false;
    }
  }
  
  return true;
}
#endif

/*----------------------------------------------------------------------------*/
/* number entries                                                             */
/*----------------------------------------------------------------------------*/
#ifdef _DEBUG
void EntryBuffer::vNumberEntries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // set entry no
  
  i8 iEntryNo = 0;
  
  for (Entry* aTestEntry_0 = aNextEntry(m_aHeadEntry,0);
       aTestEntry_0;
       aTestEntry_0 = aNextEntry(aTestEntry_0,0))
    
    aTestEntry_0->m_iEntryNo = iEntryNo++;
  
}
#endif

/*----------------------------------------------------------------------------*/
/* quick verify                                                               */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vQuickVerify(Entry* aSeeEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);

  for (Item* aItem = aFirstItem();
       aItem != nullptr;
       aItem = aNextItem(aItem))
    {;}
  
  for (Entry* aTestEntry = aNextEntry(m_aHeadEntry,0);
       aTestEntry;
       aTestEntry = aNextEntry(aTestEntry,0)) {
  
    if (aSeeEntry)
      aTestEntry->vShow(iToOffset(aTestEntry),m_pIndex,true);
    
    if (aTestEntry == aSeeEntry)
      int iDebug = 1;
    
    for (i8 i_ScanOffset = aTestEntry->nOffsets()-1; i_ScanOffset >= 0; i_ScanOffset--) {
      Entry* aCheckEntry = aNextEntry(aTestEntry,i_ScanOffset);
      if (! aCheckEntry)
        continue;
      
      if (CN_ENTRY != aCheckEntry->eClass())
        vShow();
      ASSERT(CN_ENTRY == aCheckEntry->eClass());
    }
 
  }

//  ln i_ScanOffset;
//  for (i_ScanOffset = 2; *i_ScanOffset >= 0; i_ScanOffset--) {
//
//    for (Entry* aTestEntry = aNextEntry(m_aHeadEntry,*i_ScanOffset);
//         pTestEntry;
//         pTestEntry = aNextEntry(pTestEntry,*i_ScanOffset)) {
//      
//      if (pSeeEntry) {
//        vTrace("|scan: %lld| "_Stz,*i_ScanOffset);
//        pTestEntry->vShow(iToOffset(pTestEntry),m_pIndex,true);
//      }
//      
//      if (pTestEntry == pSeeEntry)
//        int iDebug = 1;
//      
//      if (CN_ENTRY != pTestEntry->eClass())
//        vShow();
//      ASSERT(CN_ENTRY == pTestEntry->eClass());
//    }
//  }
  
}

/*----------------------------------------------------------------------------*/
/* quick verify                                                               */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bQuickVerify(Entry* aNotSeeEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  for (Entry* aTestEntry = aNextEntry(m_aHeadEntry,0);
       aTestEntry;
       aTestEntry = aNextEntry(aTestEntry,0)) {
    
    for (i8 i_ScanOffset = aTestEntry->nOffsets()-1; i_ScanOffset >= 0; i_ScanOffset--) {
      
      Entry* aCheckEntry = aNextEntry(aTestEntry,i_ScanOffset);
      if (! aCheckEntry)
        continue;
      
      if (aCheckEntry == aNotSeeEntry) {
        aTestEntry->vShow(iToOffset(aTestEntry),m_pIndex,true);
        aCheckEntry->vShow(iToOffset(aCheckEntry),m_pIndex,true);
        return false;
      }
      
      if (CN_ENTRY != aCheckEntry->eClass())
        return false;
    }
  }
   
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the structure of this L0 buffer (hidden index)                      */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyL0Hidden(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(! m_aLazyChain);
    
  RASSERT(0 == m_iBufferLevel);
  RASSERT(m_pIndex->bIsHidden());
  
//  vTrace("VerifyL0 %s: bufno: %lld level %lld\n"_Stz,
//         m_pIndex->aIndexName(),m_iBufferNo,m_iBufferLevel);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify the contents of this L0 hidden index buffer buffer
  
  bool bValid;
  
  bValid = bVerifyNextPrev();
  if (! bValid)
    return false;
  
  bValid = bVerifyItems();
  if (! bValid) {
    return false;
  }
  
  bValid = bVerifyFreeList();
  if (! bValid) {
    return false;
  }
  
  bValid = bVerifyEntryOffset();
  if (! bValid) {
    return false;
  }
  
  bValid = bVerifyRows();
  if (! bValid) {
    return false;
  }
  
  bValid = bVerifySkipListEntry();
  if (! bValid)
    return false;
  
#ifdef _DEBUG
  bValid = bVerifySkipListLevels();
  if (! bValid)
    return false;
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the structure of this L0 buffer (non-hidden index)                  */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyL0Other(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT(0 == m_iBufferLevel);
  
//  vTrace("VerifyL0 %s: bufno: %lld level %lld\n"_Stz,
//         m_pIndex->aIndexName(),m_iBufferNo,m_iBufferLevel);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify the contents of this L0 non-hidden index buffer
  
  bool bValid;
  
  bValid = bVerifyNextPrev();
  if (! bValid)
    return false;
  
  bValid = bVerifyItems();
  if (! bValid)
    return false;
  
  bValid = bVerifyFreeList();
  if (! bValid)
    return false;
    
  bValid = bVerifyKeyOrder();
  if (! bValid)
    return false;
  
  bValid = bVerifyRowRefs();
  if (! bValid)
    return false;
  
  bValid = bVerifySkipListEntry();
  if (! bValid)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the structure of this Ln buffer (any index)                         */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyLnAny(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT(0 != m_iBufferLevel);

//  vTrace("VerifyLn %s: bufno: %lld level %lld\n"_Stz,
//         m_pIndex->aIndexName(),m_iBufferNo,m_iBufferLevel);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify the contents of this buffer
  
  bool bValid;
  
  bValid = bVerifyNextPrev();
  if (! bValid)
    return false;
  
  bValid = bVerifyItems();
  if (! bValid)
    return false;
  
  bValid = bVerifyFreeList();
  if (! bValid)
    return false;
  
  bValid = bVerifyKeyOrder();
  if (! bValid)
    return false;
  
  bValid = bVerifyBufferRefs();
  if (! bValid)
    return false;
  
  bValid = bVerifyAdjacent();
  if (!bValid)
    return false;
  
  bValid = bVerifyKeyBufferOrder();
  if (! bValid)
    return false;
  
  bValid = bVerifyFirstKeys();
  if (! bValid)
    return false;
  
  bValid = bVerifySkipListEntry();
  if (! bValid)
    return false;
  
#ifdef _DEBUG
  bValid = bVerifySkipListLevels();
  if (! bValid)
    return false;
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify the contents of child buffers
  
  for (Entry* aEntry = aFirstEntry(); aEntry; aEntry = aNextEntry(aEntry,0)) {
    
    EntryBuffer* pRefEntBuf = aEntry->pReferencedEntryBuffer();
    if (pRefEntBuf->m_iBufferLevel) {
      bValid = pRefEntBuf->bVerifyLnAny();
    }
    else {
      if (m_pIndex->bIsHidden())
        bValid = pRefEntBuf->bVerifyL0Hidden();
      else
        bValid = pRefEntBuf->bVerifyL0Other();
    }
    if (! bValid)
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the adjacent entries not pointing to the same buffer                */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyAdjacent(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
      Every entry in an Ln buffer would point to a different buffer.  This
      routine performs a simple whereby two adjacent entries can't point to
      the same buffer.
   
      A more complete test would be to mark all buffers with some reference
      count.
   */
  
  RASSERT(m_iBufferLevel >= 1);
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  for (Entry* aEntry = aFirstEntry(); aEntry; aEntry = aNextEntry(aEntry,0)) {
    
    // get the buffer that this entry references
    EntryBuffer* pChildEntryBuffer = aEntry->pReferencedEntryBuffer();
    
    // get the adjacent entry
    Entry* aAdjacentEntry = aNextEntry(aEntry,0);
    
    if (aAdjacentEntry) {
      // get the buffer that the adjacent entry references
      EntryBuffer* pAdjacentChildEntryBuffer = aAdjacentEntry->pReferencedEntryBuffer();
      
      // the two child buffers should not be the same
     if ( pChildEntryBuffer == pAdjacentChildEntryBuffer) {
        vShow();
        vVerifyFault();
        return false;
      }
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the buffer references in this Ln index buffer                       */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyBufferRefs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
      Every entry in an Ln buffer should reference an Ln-1 buffer. And
      the Ln-1 buffer should have its parent buffer be that Ln buffer
   */
  
  RASSERT(m_iBufferLevel >= 1);
  
  for (Entry* aEntry = aFirstEntry(); aEntry; aEntry = aNextEntry(aEntry,0)) {
    
    EntryBuffer* pChildEntBuf = aEntry->pReferencedEntryBuffer();
    
    if ( ! pChildEntBuf->bValidEntryBuffer() ) {
      vVerifyFault();
      return false;
    }
    
    if (pChildEntBuf->iBufferLevel() != m_iBufferLevel - 1) {
      vVerifyFault();
      return false;
    }
    
    if (pChildEntBuf->pHigherBuffer() != this) {
      vVerifyFault();
      return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the entry offset array of this L0 hidden index buffer               */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyEntryOffset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
      For the hidden index each buffer has an offset array which indicates
      where the row is located in the buffer.
   
      Entries are only maintained for entries 0..gpAttachMap->m_nIndexEntries-1.
   
      In all cases the hidden index is a list of L0 buffers.
   */
  
  RASSERT(0 == m_iBufferLevel);
  RASSERT(m_pIndex->bIsHidden());
  
  //vShow();
  
  for (i8 i_RowNo = 0; i_RowNo < gpAttachMap->m_nIndexEntries; i_RowNo++) {

    RASSERT(i_RowNo < C_ENTRIES_L0);
    i8 i_RowEntryOffset = m_aEntryOffsets[i_RowNo];
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // validate free offset element
    
    if (i_RowEntryOffset < 0) {
      
      // last offset in free chain
      if (C_ENTRY_NONE == - i_RowEntryOffset)
        continue;
      
      // get the next offset in free chain
      i8 i_NextRowEntryOffset = m_aEntryOffsets[ - i_RowEntryOffset ];
      
      // next offset should be free (or end of free chain)
      if (i_NextRowEntryOffset >= 0) {
        vVerifyFault();
        return false;
      }
      
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // validate offset element which is the offset to an entry
    
    Entry* aEntry = aGetEntryOffset(i_RowEntryOffset);
    if (! aEntry->bValidEntry())
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the first key order of child buffers                                */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyFirstKeys(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
      The for each entry in an Ln buffer will match the first key in an
      Ln-1 buffer
   */
  
  RASSERT(m_iBufferLevel >= 1);
  
  //vShow();
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  for (Entry* aEntry = aFirstEntry(); aEntry; aEntry = aNextEntry(aEntry,0)) {
    
    // get the entry buffer that this entry references
    EntryBuffer* pChildEntryBuffer = aEntry->pReferencedEntryBuffer();
    
    //pChildEntryBuffer->vShow();

    
    // get the first entry of the child buffer
    Entry* aChildFirstEntry = pChildEntryBuffer->aFirstEntry();

    // get keys of the two entries
    Key* aKeyParent = aEntry->aKey();
    Key* aKeyChild  = aChildFirstEntry->aKey();
   
    // the two keys should match
    bool bEqual = aKeyChild->bEqualTo(aKeyParent,pIdxColVec);
    if (! bEqual) {
      vVerifyFault();
      return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the free list of this buffer                                        */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyFreeList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
      The chain of free entries in any buffer must consist of solely
      of FreeEntry's.  No other class is permited.
   */
  
  for (FreeEntry* aFreeEntry = aFirstFree();
       aFreeEntry != nullptr;
       aFreeEntry = aFreeEntry->aNextFree()) {
    
    if (! aFreeEntry->bValidFreeEntry()) {
      vVerifyFault();
      return false;
    }
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the items of this buffer                                            */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
      The items in and entry buffer are some sequence of:
   
        {[Entry]|[FreeEntry]...}[Wilderness]
   
      The number of Entries must match the number of items in the buffer
   */
  
  i8 nEntryItems = 0;
  
  for (Item* aItem = aFirstItem();
       aItem != nullptr;
       aItem = aNextItem(aItem)) {
    
    //vTrace("Verify item %p\n"_Stz,pItem);
    
    eCN eClass = aItem->eClass();
    
    switch (eClass) {
      
      case CN_ENTRY: {
        Entry* aEntry = _aEntry(aItem);
        if (! aEntry->bValidEntry()) {
          vShow();
          vVerifyFault();
          return false;
        }
        nEntryItems++;
        break;
      }
      
      case CN_FREEENTRY: {
        FreeEntry* aFreeEntry = _aFreeEntry(aItem);
        if (! aFreeEntry->bValidFreeEntry()) {
          vShow();
          vVerifyFault();
          return false;
        }
        break;
      }
        
      case CN_WILDERNESS: {
        Wilderness* aWilderness = _aWilderness(aItem);
        if (! aWilderness->bValidWilderness()) {
          vShow();
          vVerifyFault();
          return false;
        }
        break;
      }
    
      default:
        vVerifyFault();
        return false;
    }
  }
  
  // check the number of entries match
  if (nEntryItems != nItems()) {
    vShow();
    vVerifyFault();
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify that keys are in buffer order of the child buffers                  */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyKeyBufferOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
      The entries in an Ln buffer should be in order of the child buffer list
   */
  
  RASSERT(m_iBufferLevel >= 1);
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  for (Entry* aEntry = aFirstEntry(); aEntry; aEntry = aNextEntry(aEntry,0)) {
    
    // get next entry
    Entry* aNextEntry = this->aNextEntry(aEntry,0);
    if (! aNextEntry)
      break;
    
    // get the child entry buffers
    EntryBuffer* pChildBufferA = aEntry->pReferencedEntryBuffer();
    EntryBuffer* pChildBufferB = aNextEntry->pReferencedEntryBuffer();
    
    // child A should point to child B
    if (pChildBufferA->pNextBuffer() != pChildBufferB) {
      vShow(aEntry);
      vVerifyFault();
      return false;
    }

    // child B should point to child A
    if (pChildBufferB->pPrevBuffer() != pChildBufferA) {
      vVerifyFault();
      return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the key order of this buffer                                        */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyKeyOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
      Any Ln buffer or non-hidden L0 buffer should have its keys in ascending 
      order.
   */
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  for (Entry* aEntry = aFirstEntry(); aEntry; aEntry = aNextEntry(aEntry,0)) {
    
    // get next entry
    Entry* aNextEntry = this->aNextEntry(aEntry,0);
    if (! aNextEntry)
      break;
    
    // get keys of the two entries
    Key* aKeyA = aEntry->aKey();
    Key* aKeyB = aNextEntry->aKey();

    // next keyB should be greater than or equal to keyA
    bool bGreaterEqual = aKeyB->bGreaterEqual(aKeyA,pIdxColVec);
    if (! bGreaterEqual) {
      vVerifyFault();
      return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the next, previous and higher pointers                              */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyNextPrev(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
      Any buffer should have a next whose prev point back to it; any prev should
      point forward to it; a child buffer should point back to its parent
   */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_pPrevBuffer) {
    if (m_pPrevBuffer->pNextBuffer() != this) {
      vVerifyFault();
      return false;
    }
    if (m_pPrevBuffer->iBufferLevel() != m_iBufferLevel) {
      vVerifyFault();
      return false;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_pNextBuffer) {
    if (m_pNextBuffer->pPrevBuffer() != this) {
      vVerifyFault();
      return false;
    }
    if (m_pNextBuffer->iBufferLevel() != m_iBufferLevel) {
      vVerifyFault();
      return false;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_pHigherBuffer) {
    if (m_pHigherBuffer->iBufferLevel() != m_iBufferLevel + 1) {
      vVerifyFault();
      return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the rows of this buffer                                             */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyRows(void)                        //# discuss with Alka
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT(0 == m_iBufferLevel);
  RASSERT(m_pIndex->bIsHidden());
  
  for (Entry* aEntry = aFirstEntry(); aEntry; aEntry = aNextEntry(aEntry,0)) {
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the row references in this L0 non-hidden index buffer               */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyRowRefs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
      For this L0 non-hidden index verify that its key entries referen a
      valid row in an L0 hidden index buffer
   */
  
  RASSERT(0 == m_iBufferLevel);
  RASSERT(! m_pIndex->bIsHidden());
  
  for (Entry* aEntry = aFirstEntry(); aEntry; aEntry = aNextEntry(aEntry,0)) {
    
    // get the entry buffer that this L0 key leaf references
    EntryBuffer* pEntryBuffer = aEntry->pReferencedEntryBuffer();
    
    // that referenced entry buffer should be an L0 buffer
    if (0 != pEntryBuffer->iBufferLevel()) {
      vVerifyFault();
      return false;
    }
    
    // that referenced entry buffer should be for a hidden index
    if (! pEntryBuffer->pIndex()->bIsHidden()) {
      vVerifyFault();
      return false;
    }
    
    // get the row entry from referenced entry buffer
    i8 i_Row = aEntry->iRow();
    Entry* aRowEntry = pEntryBuffer->aGetEntryRowNo(i_Row);
    
    // validate the row entry                                          //# more?
    if (1 != aRowEntry->nOffsets()) {
      vVerifyFault();
      return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the skip list only contains only entries                            */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifySkipListEntry(bool bShowKeys)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
      Run thru each scan list level and verify we discover entries with
      the same number of scan entries
   */
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  Entry* aNextEntry2;
  Entry* aNextEntry1;
  Entry* aNextEntry0;
  
  for (i8 i_ScanLevel = gpAttachMap->m_nIndexOffsets - 1;
       i_ScanLevel >= 0;
       i_ScanLevel--) {
    
    if (bShowKeys)
      vTrace("i_ScanLevel %lld\n"_Stz,i_ScanLevel);
    
    Entry* aTestEntry_sl = aNextEntry(m_aHeadEntry,i_ScanLevel);
    while (aTestEntry_sl) {
      
      if (CN_ENTRY != aTestEntry_sl->eClass()) {
        vVerifyFault();
        return false;
      }
      
      switch (i_ScanLevel) {
        case 2:
          aNextEntry2 = aNextEntry(aTestEntry_sl,2);
          if (aNextEntry2 && CN_ENTRY != aNextEntry2->eClass()) {
            vVerifyFault();
            return false;
          }
        case 1:
          aNextEntry1 = aNextEntry(aTestEntry_sl,1);
          if (aNextEntry1 && CN_ENTRY != aNextEntry1->eClass()) {
            vVerifyFault();
            return false;
          }
        case 0:
          aNextEntry0 = aNextEntry(aTestEntry_sl,0);
          if (aNextEntry0 && CN_ENTRY != aNextEntry0->eClass()) {
            vVerifyFault();
            return false;
          }
          break;
      }
      
      if (bShowKeys) {
        Key* aTestKey = aTestEntry_sl->aKey();
        vTrace("Test %p: "_Stz,aTestEntry_sl);
        aTestKey->vShow(pIdxColVec);
        vTrace("\n"_Stz);
      }
      
      aTestEntry_sl = aNextEntry(aTestEntry_sl,i_ScanLevel);
    }
  }
  
#ifdef _DEBUG
  if (! bVerifySkipListLevels())
    return false;
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the skip list levels                                                */
/*----------------------------------------------------------------------------*/
#ifdef _DEBUG

bool EntryBuffer::bVerifySkipListLevels(bool bShowKeys)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
      Verify entries with skip offsets greater than one that they
      can be found by following the  skip chain at that level, and
      that the entries all move forward (not backward).
   */
  
  // set entry no's
  vNumberEntries();
  
  // run through all entries at level 0
  Entry* aTestEntry_0 = aNextEntry(m_aHeadEntry,0);
  while (aTestEntry_0) {
    
    // check if scan entry has more than one offset
    i8 nOffsets = aTestEntry_0->nOffsets();
    if (1 == nOffsets) {
      aTestEntry_0 = aNextEntry(aTestEntry_0,0);
      continue;
    }
    
    // for each level see if we can find the test entry
    for (i8 i_SkipLevel = nOffsets - 1;
         i_SkipLevel > 0;
         i_SkipLevel--) {
      
      Entry* aScanEntry_sl = aNextEntry(m_aHeadEntry,i_SkipLevel);
      while (aScanEntry_sl) {
        
        if (aScanEntry_sl == aTestEntry_0)
          break;
        
        Entry* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_SkipLevel);
        
        // check we dont go backwwards
        if (aNextEntry_sl->m_iEntryNo < aScanEntry_sl->m_iEntryNo) {
          vShow(aScanEntry_sl);
          vVerifyFault();
          return false;
        }
        
        aScanEntry_sl = aNextEntry_sl;
      }
      
      if (aScanEntry_sl != aTestEntry_0) {
        vShow(aTestEntry_0);
        vVerifyFault();
        return false;
      }
      
    }
    
    aTestEntry_0 = aNextEntry(aTestEntry_0,0);
  }
  
  return true;
}
#endif

/*----------------------------------------------------------------------------*/
/* verify has detected a fault                                                */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vVerifyFault(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  vTrace("VerifyFault\n"_Stz);
  
  return;
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
/*-- Unit Tests --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* unit test (b) -> (c)                                                       */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vUnitTest_c(Table* pTable)
{
  vTrace("\n"_Stz);
  vTrace("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"_Stz);
  vTrace("EntryBuffer::vUnitTest_c\n"_Stz);
  //vTrace("\n"_Stz);
  
  s256 sRecData;
  i8 iRecLength;
  RowWrk xRowWrk;
  i8 iRowLength;
  bool bFormatted;
  bool bBlankLine;
  KeyWrk xKeyWrk;
  
  ColumnVec* pColVec = pTable->pColumnVec();
  Index* pHiddenIndex = pTable->pHiddenIndex();
  
  IdxVec* pIdxVec = pTable->pIdxVec();
  Index* pIndex1 = pIdxVec->pGet(1);
  IdxColVec* pIdxColVec1 = pIndex1->pIdxColVec();
  
  Formatters* pFormatters = &gpAttachMap->m_oFormatters;
  
  EntryBuffer* pFirstRowBuffer = new EntryBuffer;
  EntryBuffer* pLastRowBuffer = pFirstRowBuffer;
  pLastRowBuffer->vIndex(pHiddenIndex);
  pLastRowBuffer->vPartitionNo(0);
  
  EntryBuffer* pRootKeyBuffer = new EntryBuffer;
  pRootKeyBuffer->vIndex(pIndex1);
  pRootKeyBuffer->vPartitionNo(0);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // keys in ascending order
  //for (i8 iInt = 0; iInt <= 100; iInt++) {

  // keys in descending order
  //for (i8 iInt = 10; iInt >= 0; iInt--) {
  
  // keys in random order
  //i8 iRndNo[] = { 4,0,1,7,8,2,5,9,6,3,10 };
  //for (i8 iCount = 0; iCount <= 100; iCount++) {
    //i8 iInt = iRndNo[iCount]; //Random::iInteger(10);
    //i8 iInt = Random::iInteger(1000);
  
  // duplicate keys
  for (i8 iInt = 1; iInt <= 10; iInt++)
  for (i8 iCount = 0; iCount < iInt; iCount++) {
    
    i8 iKey = iInt;
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    sprintf(sRecData.aChar(),"%lld,%s,1900-01-01 00:00:%lld",
            iKey,IntLib::sAsText(iKey),iKey);
    
    char* aRowData = (char*)xRowWrk.aRow();
    iRecLength = sRecData.iLength();
    
//    bFormatted = Row::bFormatRow(pColVec,pFormatters,
//                                 sRecData,iRecLength,
//                                 (Stz*)aRowData,iRowLength,
//                                 bBlankLine,
//                                 false /* allowNonPrintable */);
    
    xRowWrk.vRowSize(iRowLength);
    xRowWrk.vTxId(0);
    
    //((Row*)aRowData)->vDump(pColVec);
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    Entry* aRowEntry = pLastRowBuffer->aAppendRow(xRowWrk);
    
    if ( ! aRowEntry ) {
      pLastRowBuffer = pLastRowBuffer->pNewNextBuffer();
      aRowEntry = pLastRowBuffer->aAppendRow(xRowWrk);
    }
  
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    xKeyWrk.vReset();
    aRowEntry->vExtractIndexKey(xKeyWrk,pIdxColVec1);
    //xKeyWrk.vShow(pIdxColVec1);
    
    xKeyWrk.vTxId(0);
    xKeyWrk.vId(aRowEntry->xId());
    
    Entry* aKeyEntry =
      EntryBuffer::aInsertKeyTree(xKeyWrk,&pRootKeyBuffer);
    
    vTrace("\nInserted record: %s\n"_Stz,sRecData.aChar());
    pRootKeyBuffer->vShowTree(false);

    int iDebug = 1;
  }
  pRootKeyBuffer->vShowTree();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  for (EntryBuffer* pRowBuffer = pFirstRowBuffer;
//       pRowBuffer;
//       pRowBuffer = pRowBuffer->pNextBuffer())
//    pRowBuffer->vShow();
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
