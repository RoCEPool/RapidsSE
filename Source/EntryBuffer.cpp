/*============================================================================*/
/* EntryBuffer.cpp -- Entry Buffer                                            */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

bool mbWatch = false;
//#define MB_WATCH

//#define EB_LAZY_CHAIN
//#define EB_SPLIT2

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
EntryBuffer* DBobjects::_pEntryBuffer(void* aAddress)
{
  EntryBuffer* pEntryBuffer = reinterpret_cast<EntryBuffer*>(aAddress);
  
  RASSERT( !pEntryBuffer || CN_ENTRYBUFFER == pEntryBuffer->eClass());
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* EntryBuffer::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  EntryBuffer* pEntryBuffer =
    static_cast<EntryBuffer*>(Heap::pAllocate(CN_ENTRYBUFFER));
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* EntryBuffer::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void EntryBuffer::operator delete(void* aAddress)
{
  EntryBuffer* pEntryBuffer = static_cast<EntryBuffer*>(aAddress);
  
  Heap::vFree(pEntryBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer::EntryBuffer(void)
: ItemBuffer(CN_ENTRYBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_bIsLast       = false;
  
  m_pHigherBuffer = nullptr;
  m_pNextBuffer   = nullptr;
  m_pPrevBuffer   = nullptr;

  m_pIndex        = nullptr;
  m_nPartitionNo  = -1;
  m_iBufferNo     = *nBufferNo();
  m_iBufferLevel  = 0;
  
  m_pEntryOffset    =
    reinterpret_cast<i4*>(aAllotSpace(sizeof(i4) * C_ENTRIES_0));
  m_i_FreeOffsetHead = C_ENTRY_NONE;
  vFreeChains();
  
  m_pHeadEntry  = nullptr;
  m_pLastEntry  = nullptr;
  m_pFirstFree  = nullptr;
  
  m_pLazyChain  = nullptr;
  
  vHeadEntryNew();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_ENTRYBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntryBuffer::~EntryBuffer(void)
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
        sAnalysis.vCatSprint("     buffer %5lld (L%lld) : %lld entries\n"_Stz,
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
    sAnalysis.vCatSprint("     buffer %5lld (L%lld) : %lld entries\n"_Stz,
                         iBuffers,
                         m_iBufferLevel,
                         m_nItems);
  
  // recursively dump the lower level buffers
  if (m_iBufferLevel != 0) {
    
    for (Entry* pEntry = pFirstEntry(); pEntry != nullptr;
         pEntry = pNextEntry(pEntry, 0)) {
      
      // get the index buffer from the entry
      EntryBuffer* pEntryBuffer = pEntry->pReferencedEntryBuffer();
      
      ASSERT(pEntryBuffer);
      ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
      
      pEntryBuffer->vAnalyze(sAnalysis, bDetailed, iBuffers, iEntries);
      
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* attempt to append a new row entry into this buffer                         */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pAppendRow(RowWrk& xRowWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(iBufferLevel() == 0);
  
  Entry* pRowEntry;

  // rows only ever have one next offset
  xRowWrk.vOffsets(1);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute required entry space for row and offset
  
  i8 nEntryBytes = xRowWrk.nEntrySize();
  i8 nOffsetBytes = xRowWrk.nOffsetSize();
  i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if this buffer has enough space
  
  if ( ! bHasSpace(nEntryPlusOffsets) )
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the rowId
  
  bn nBufferNo = this->nBufferNo();
  i8 i_RowNo = iGetFreeEntry(); //nItems();
  Id xRowId(*nBufferNo, i_RowNo);
  
  xRowWrk.vId(xRowId);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate space in this buffer for the row and offsets
  
  pRowEntry = pAllotEntry(nEntryBytes);
  ASSERT(pRowEntry);
  
  NEO* aNEO = aAllotNEO(nOffsetBytes);
  ASSERT(aNEO);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the buffer entry
  pRowEntry->vSetEntry(xRowWrk);
  
  //pRowEntry->vShow(0, m_pIndex, false);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set entry offsets
  
  i8 iEntryOffset = iToOffset(pRowEntry);
  m_pEntryOffset[i_RowNo] = iEntryOffset;
  
  if (m_pLastEntry)
    m_pLastEntry->vNEO(0,iEntryOffset);
  pRowEntry->vNEO(0,-1);
  
  m_pLastEntry = pRowEntry;
  
  // set head entry offset
  if (m_pHeadEntry->iNEO(0) == -1)
    m_pHeadEntry->vNEO(0, iEntryOffset);
  
  RASSERT(pRowEntry->bValidEntry());
  
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
    
    for (Entry* pEntry = pFirstEntry(); pEntry != nullptr;
         pEntry = pNextEntry(pEntry, 0)) {
      
      // get the index buffer from the entry
      pNextBuffer = pEntry->pReferencedEntryBuffer();
      
      ASSERT(pNextBuffer);
      ASSERT(pNextBuffer->eClass() == CN_ENTRYBUFFER);
      
      pNextBuffer->vBufferCount(nBuffers);
      
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
void EntryBuffer::vDeleteEntry(Entry** ppEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // currently we only delete from non-hidden indexes
  ASSERT(! m_pIndex->bIsHidden());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Entry* pEntry = *ppEntry;
  
  Entry* xPriorVec[gpAttachMap->m_nIndexOffsets];

  Key* aKey = pEntry->aKey();
  
  vPositionPriorVecEntry(aKey, xPriorVec, pEntry);
  
  vCutEntry(pEntry,xPriorVec);
  
  vAddFreeList(pEntry);
  
  // entry no longer exists, set the pointer to be null
  *ppEntry = nullptr;
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
    
    for (Entry* pEntry = pFirstEntry();
         pEntry != nullptr;
         pEntry = pNextEntry(pEntry, 0)) {
      
      // get the index buffer from the entry
      EntryBuffer* pEntryBuffer = pEntry->pReferencedEntryBuffer();
      
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
  
#ifdef EB_LAZY_CHAIN
  //RASSERT( ! m_pLazyChain );
  
  if (m_pLazyChain)
    vBuildSkipList();
#endif
  
  Entry* pFirst = pNextEntry(m_pHeadEntry,0);
  
  return pFirst;
}

/*----------------------------------------------------------------------------*/
/* get an entry by number                                                     */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pGetEntryNo(i8 i_Entry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_pLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (i_Entry == C_ENTRY_NONE)
    return nullptr;
  
  i4 iOffset = m_pEntryOffset[i_Entry];
  char* aAddress = pAdd<char*>(aDataArea(), iOffset);
  
  Entry* pEntry = reinterpret_cast<Entry*>(aAddress);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* insert a new key entry into this key tree                                  */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pInsertKeyTree(KeyWrk& xKeyWrk,
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
  

  Entry* pInsertedKeyEntry =
    pPositionedBuffer->pInsertKeyUpwards(xKeyWrk,ppRootKeyBuffer);
  
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
  
  return pInsertedKeyEntry;
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
Entry* EntryBuffer::pNextEntry(Entry* pCurrent, i8 i_Offset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //#ifdef EB_LAZY_CHAIN
  //RASSERT( ! m_pLazyChain );
  //#endif
  
  pCurrent->vAssertEntry();
  
  i4 iOffset = pCurrent->iNEO(i_Offset);
  
  if (-1 == iOffset)
    return nullptr;
  
  char* aAddress = pAdd<char*>(aDataArea(), iOffset);
  Item* pItem = _pItem(aAddress);
  
  //if (CN_ENTRY != pItem->eClass())
  //  vShow();
  
  Entry* pEntry = _pEntry(aAddress);

  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* position at an index buffer at given level starting from this buffer       */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pPositionLevel(Key* aGivenKey, i8 nKeyCols,
                                         eJC eJuxtaCompare, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  //RASSERT( ! m_pLazyChain );
  
  if (m_pLazyChain)
    vBuildSkipList();
#endif

  EntryBuffer* pEntryBuffer = this;
  
  i8 iBufferLevel;
  Entry* pEntry;
  
  while (true) {
    iBufferLevel = pEntryBuffer->iBufferLevel();
    
    if (iBufferLevel == iLevel)
      break;
    
    // get the entry
    pEntry = pEntryBuffer->pPositionQuery(aGivenKey, nKeyCols, eJuxtaCompare);
    ASSERT(pEntry);
    
    // get the index buffer of the entry
    pEntryBuffer = pEntry->pReferencedEntryBuffer();
    ASSERT(pEntryBuffer);
    ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
  }
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* position at key as per relational operator                                 */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pPositionQuery(Key* aGivenKey, eRO eRelnOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  //RASSERT( ! m_pLazyChain );
  
  if (m_pLazyChain)
    vBuildSkipList();
#endif
  
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
        pIndexEntry = pNextEntry(pIndexEntry, iBufferLevel());
      }
    } while (pIndexEntry && ! bAfter);
  }
  
  return pIndexEntry;
}

/*----------------------------------------------------------------------------*/
/* position at given key                                                      */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pPositionQuery(Key* aGivenKey,i8 nKeyCols, eJC eJuxtaCompare)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  //RASSERT( ! m_pLazyChain );
  
  if (m_pLazyChain)
    vBuildSkipList();
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
  
  Entry* pScanEntry_sl = m_pHeadEntry;
  bool bIsEqual;
  
  for (i8 i_ScanOffset = 2; i_ScanOffset >= 0; i_ScanOffset--) {
    
    Entry* pNextEntry_sl = pNextEntry(pScanEntry_sl,i_ScanOffset);
    while (pNextEntry_sl) {
      
      Key* aNextKey = pNextEntry_sl->aKey();
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aGivenKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
      bIsJuxta =
        aGivenKey->bJuxtaCompare(JC_AFTER,aNextKey,pIdxColVec,nKeyCols,bIsEqual);
      
      // have we found to key during a query?
      if (bIsEqual) {
        if (0 == i_ScanOffset)
          return pNextEntry_sl;
        else
          break;
      }
      
      // if given key is not same/after next key then break to outer loop
      if (! bIsJuxta)
        break;
      
      pScanEntry_sl = pNextEntry_sl;
      pNextEntry_sl = pNextEntry(pNextEntry_sl,i_ScanOffset);
    }
    //pPriorVec[*i_ScanOffset] = pScanEntry_sl;
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
/* restore the buffer after being received over a socket                      */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vRestore(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  vHeadEntryRestore();
  vItemCountRestore();
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  s256 sHigherBuffer;
  if (m_pHigherBuffer)
    StrLib::wToString(((EntryBuffer*)m_pHigherBuffer)->m_iBufferLevel,
                      sHigherBuffer.aChar());
  else
    sHigherBuffer = "..."_Stz;
  
  vTrace("\n"_Stz);
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  vTrace("EntryBuffer: %p (buffer %lld): items: %lld used: %lld freespace %lld\n"_Stz,
         this,*nBufferNo(),nItems(),nUsedBytes(),nFreeBytes());
  
  vTrace("  higherBuffer %p (L%s) prevBuffer %p nextBuffer %p\n"_Stz,
         m_pHigherBuffer,sHigherBuffer.aStr(),m_pPrevBuffer,m_pNextBuffer);

  vTrace("  index: %s partition: %lld level: %lld\n"_Stz,
         m_pIndex ? m_pIndex->aIndexName() : "NULL"_Stz,m_nPartitionNo,m_iBufferLevel);
  
  vTrace("  entryOffset: %p freeOffsetHead: %lld\n"_Stz,
         m_pEntryOffset,m_i_FreeOffsetHead);

  vTrace("  headEntry: %p[%lld,%lld,%lld] lastEntry: %p firstFree: %p\n"_Stz,
         m_pHeadEntry,m_pHeadEntry ? m_pHeadEntry->iNEO(0) : 0,
                      m_pHeadEntry ? m_pHeadEntry->iNEO(1) : 0,
                      m_pHeadEntry ? m_pHeadEntry->iNEO(2) : 0,
         m_pLastEntry,m_pFirstFree);
  
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  ASSERT(m_pHeadEntry);
  
  vTrace("\n"_Stz);
  vTrace("Entries (by headEntry vector[0]):\n"_Stz);
 
  i8 iOffset = m_pHeadEntry->iNEO(0);
  while (iOffset > -1) {
    Entry* pEntry = pGetEntryOffset(iOffset);
    pEntry->vShow(iOffset,m_pIndex,true);
    iOffset = pEntry->iNEO(0);
  }
  
  if (gpAttachMap->m_nIndexEntries) {
    vTrace("\n"_Stz);
    vTrace("Free Entry Chain:\n"_Stz);
    for (i4 i_Entry = m_i_FreeOffsetHead;
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
      vTrace("  %p bytes: %lld next: %p prev: %p offset %lld\n"_Stz,
             pFreeEntry,pFreeEntry->nSizeItem(),
             pFreeEntry->pNextFree(),pFreeEntry->pPrevFree(),
             iToOffset(pFreeEntry));
    }
  }
  
  vTrace("\n"_Stz);
  vTrace("By item:\n"_Stz);
  for (Item* pItem = pFirstItem();
       pItem != nullptr;
       pItem = pNextItem(pItem)) {
    
    i8 iItemOffset = iToOffset(pItem);
    
    vTrace(" %lld: %p: "_Stz,iItemOffset,pItem);
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
        vTrace("Unknown (class is %lld)"_Stz,eClass);
        break;
    }
    vTrace("\n"_Stz);
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
  
  i8 iOffset = pHeadEntry()->iNEO(0);
  while (iOffset > -1) {
    Entry* pEntry = pGetEntryOffset(iOffset);
    
    Key* aKey = pEntry->aKey();
    i8 iVal = aKey->iGetInt();
    
    if (m_iBufferLevel)
      vTrace("%lld(%lld)"_Stz,iVal,*pEntry->pReferencedEntryBuffer()->nBufferNo());
    else
      vTrace("%lld|%lld|"_Stz,iVal,*pEntry->pReferencedEntryBuffer()->nBufferNo());
    
    iOffset = pEntry->iNEO(0);
    if (iOffset > -1)
      vTrace(","_Stz);
  }
  vTrace("]"_Stz);
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
  
  Entry* pFirstEntry = this->pFirstEntry();
  if (! pFirstEntry)
    return;
  
  Id xId;
  xId = pFirstEntry->xId();
  bn nBuffer = xId.iBuffer();
  
  HeapBuffer* pHeapBuffer =
    reinterpret_cast<HeapBuffer*>(gpBufferPool->pToBuffer(nBuffer));
  
  EntryBuffer* pChildBuffer =
    reinterpret_cast<EntryBuffer*>(pHeapBuffer->pFirstChunk());
  
  pChildBuffer->vShowTree(bDumpBuffer);
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
    for (Entry* pEntry = pFirstEntry(); pEntry != nullptr;
         pEntry = pNextEntry(pEntry, 0)) {
      
      // get the index buffer from the entry
      pNextBuffer = pEntry->pReferencedEntryBuffer();
      
      ASSERT(pNextBuffer);
      ASSERT(pNextBuffer->eClass() == CN_ENTRYBUFFER);
      pNextBuffer->vTruncate();
    }
  }
  
  delete this;
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
  
  if (!m_pIndex->bIsHidden()) {
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
/* add given entry to free list                                               */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vAddFreeList(Entry* pEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add entry to head of free entry list
  
  /*
  i8 nSizeEntry = pEntry->nSizeEntry();
  i8 nSizeNEO   = pEntry->iNEOsize();
  i8 nSizeFree  = nSizeEntry + nSizeNEO;
  
  FreeEntry* pFreeEntry = new(pEntry) FreeEntry(nSizeFree);
  */
  
  FreeEntry* pFreeEntry = pMakeFreeEntry(pEntry);
  
  //  if (! bQuickVerify(pEntry)) {
  //    vDumpPriorVec(xPriorVec);
  //    vQuickVerify();
  //  }
  
  if (m_pFirstFree) {
    m_pFirstFree->vPrevFree(pFreeEntry);
    pFreeEntry->vNextFree(m_pFirstFree);
  }
  
  m_pFirstFree = pFreeEntry;
  
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
  
  ASSERT(! pFirstEntry());
  
  Entry* pTailEntry = m_pHeadEntry;
  while (Entry* pPoppedEntry = pPopLazyChain()) {

  }
}

/*----------------------------------------------------------------------------*/
/* build skip list from the lazy chain                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vBuildSkipList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (! m_pLazyChain )
    return;
  
  Entry* pOriginalFirstEntry = pNextEntry(m_pHeadEntry,0);
  
  static i8 nBuild = 0;
  //vTrace("vBuildSkipList %lld (%lld items)\n"_Stz,++nBuild,nItems());
  
  Entry* pLazyChain = m_pLazyChain;
  m_pLazyChain = nullptr;
  
  while (pLazyChain) {
    
    Entry* pKeyEntry = pLazyChain;
    
    i8 iNextOffset = pKeyEntry->iNEO(0);
    if (-1 == iNextOffset)
      pLazyChain = nullptr;
    else
      pLazyChain = pGetEntryOffset(iNextOffset);
    
    vInsertKeySkip(pKeyEntry);
  }
  
  Entry* pInsertedKeyEntry = pNextEntry(m_pHeadEntry,0);
  
  vHandleFirstInsert(pInsertedKeyEntry,pOriginalFirstEntry,nullptr);
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
  
  Entry* xPrevVec[gpAttachMap->m_nIndexOffsets];
  
  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexOffsets; i_Offset++)
    xPrevVec[i_Offset] = m_pHeadEntry;

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // go through each entry in L0 order and
  //   correct the prior offset to that entry for each level
  
  for (Entry* pEntry = pFirstEntry();
       pEntry != nullptr;
       pEntry = pNextEntry(pEntry, 0)) {
  
    // where is this entry in the buffer (offset-wise)?
    i8 iEntryOffset = iToOffset(pEntry);
    
    // get the number of offsets for this entry
    i8 nOffsets = pEntry->nOffsets();
    
    // connect the dots
    for (i8 i_Offset = 1; i_Offset < nOffsets; i_Offset++) {
      
      // set previous entry at this level to refer to this entry
      xPrevVec[i_Offset]->vNEO(i_Offset,iEntryOffset);
      
      // the previous entry at this level now becomes this entry
      xPrevVec[i_Offset] = pEntry;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* compact this buffer                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vCompact(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  //RASSERT( ! m_pLazyChain );
  
  if (m_pLazyChain)
    vBuildSkipList();
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  FreeEntry* pFreeEntry;
  Entry*     pMoveEntry;
  Entry*     xPriorVec[gpAttachMap->m_nIndexOffsets];
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
    
    if (m_iBufferLevel) {
      //vShow();
      int iDebug = 1;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // find the next free entry in buffer
    
    pFreeEntry = nullptr;
    for (;
         pScanItem != nullptr;
         pScanItem = pNextItem(pScanItem)) {
      
      eCN eClass = pScanItem->eClass();
      
      if (CN_FREEENTRY == eClass) {
        pFreeEntry = _pFreeEntry(pScanItem);
        break;
      }
    }
    
    // return if no more free entries
    if (! pFreeEntry)
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
    if (! pMoveEntry)
      break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the prior vector to the entry being moved
    
    Key* aMoveKey = pMoveEntry->aKey();
    //aKey->vShow(pIdxColVec);
    //vTrace("\n"_Stz);
    
    vPositionPriorVecEntry(aMoveKey, xPriorVec, pMoveEntry);
    
    if (m_iBufferLevel) {
      //vDumpPriorVec(xPriorVec);
      int iDebug = 1;
    }
    
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
    
    //vDumpPriorVec(xPriorVec);
    for (i8 i_Offset = 0; i_Offset < 3; i_Offset++) {
      Entry* pPriorEntry = xPriorVec[i_Offset];
      i8 iPriorOffset = pPriorEntry->iNEO(i_Offset);
      if (iWasOffset == iPriorOffset)
        pPriorEntry->vNEO(i_Offset,iNowOffset);
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move scan
    
    pScanItem = pFreeEntry;
    
  } // for
  
  ASSERT(CN_FREEENTRY == pScanItem->eClass());
  
  vResetUsed(pScanItem);
  
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
  //RASSERT( ! m_pLazyChain );
  
  if (m_pLazyChain)
    vBuildSkipList();
#endif
  
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
  for (Entry* pThisEntry = m_pHeadEntry;
       pThisEntry != nullptr;
       pThisEntry = pNextEntry(pThisEntry, 0)) {
    
    // get the address after this entry
    char* aAfterAddr = pThisEntry->aAddressAfter();
    
    // get the address of the next entry
    Entry* pNextEntry = this->pNextEntry(pThisEntry,0);
    
    // if next entry is contiguous with this entry then skip this
    if ((char*)pNextEntry == aAfterAddr)
      continue;
    
    // - there is a gap between this entry and next entry
    
    // calculate the number of bytes to copy
    i8 nCopyBytes = pNextEntry->nSizeEntry() + pNextEntry->iNEOsize();
    
    // copy the next entry so its adjacent to this entry
    MemLib::vMove(aAfterAddr, pNextEntry, nCopyBytes, CN_ENTRYBUFFER);
    
    // reset the L0 offset in this entry
    i8 iRevisedNextOffset = iToOffset((Entry*)aAfterAddr);
    pThisEntry->vNEO(0,iRevisedNextOffset);
  }
  
  // reset the wilderness
  vSetWilderness();
  
  // rebuild the full skiplist
  vBuildSkipListSorted();
}

/*----------------------------------------------------------------------------*/
/* cut entry from skip list                                                   */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vCutEntry(Entry* pEntry, Entry* pPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nOffsets = pEntry->nOffsets();
  
  for (i8 i_Offset = 0; i_Offset < nOffsets; i_Offset++) {
    i4 iNEO = pEntry->iNEO(i_Offset);
    pPriorVec[i_Offset]->vNEO(i_Offset,iNEO);
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
  RASSERT( ! m_pLazyChain );
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
  
  Entry* pPriorEntry = pPositionPriorEntry(aDeleteKey);
  if (!pPriorEntry)
    return;
  
  Entry* pEntryToBeDeleted = pNextEntry(pPriorEntry,0);
  if (!pEntryToBeDeleted)
    return;

//  vTrace("[%lld] pDeleteEntry key %lld\n"_Stz,
//         *nBufferNo(),
//         pEntryToBeDeleted->aKey()->iGetInt());
  
  Key* aDeleteEntryKey = pEntryToBeDeleted->aKey();
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
  
  Entry* pFirstEntry = this->pFirstEntry();
  if (pEntryToBeDeleted == pFirstEntry && m_pHigherBuffer)
    m_pHigherBuffer->vDeleteUpwards(aDeleteKey,ppRootKeyBuffer);

  vDeleteEntry(&pEntryToBeDeleted);
}

/*----------------------------------------------------------------------------*/
/* delete upwards                                                             */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDeleteUpwards(Key* aDeleteKey, EntryBuffer* pToBuffer,
                                 EntryBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_pLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Entry* xPriorVec[gpAttachMap->m_nIndexOffsets];

  vPositionPriorVecBuffer(aDeleteKey,xPriorVec,pToBuffer);
  
  Entry* pPriorEntry = xPriorVec[0];

  Entry* pEntryToBeDeleted = pNextEntry(pPriorEntry,0);
  if (!pEntryToBeDeleted)
    return;
  
  //  vTrace("[%lld] pDeleteEntry key %lld\n"_Stz,
  //         *nBufferNo(),
  //         pEntryToBeDeleted->aKey()->iGetInt());
  
  Key* aDeleteEntryKey = pEntryToBeDeleted->aKey();
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  //if (! aDeleteEntryKey->bEqualTo(aDeleteKey,pIdxColVec))
  //  return;
  RASSERT(aDeleteEntryKey->bEqualTo(aDeleteKey,pIdxColVec));
  RASSERT(pToBuffer == pEntryToBeDeleted->pReferencedEntryBuffer());
  
  Entry* pFirstEntry = this->pFirstEntry();
  if (pEntryToBeDeleted == pFirstEntry && m_pHigherBuffer)
    m_pHigherBuffer->vDeleteUpwards(aDeleteKey,this,ppRootKeyBuffer);
  
  vDeleteEntry(&pEntryToBeDeleted);
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
  
  for (i8 i = 0; i < 3; i++) {
    Entry* pEntry = pPriorVec[i];
    for (i8 i_Offset = 0; i_Offset < pEntry->nOffsets(); i_Offset++) {
      vTrace("  i : %lld Entry : %p i_Offset : %lld offset : %lld pointsTo : %p\n"_Stz,
             i,
             pEntry,
             i_Offset,
             pEntry->iNEO(i_Offset),
             pGetEntryOffset(pEntry->iNEO(i_Offset)));
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get first free entry                                                       */
/*----------------------------------------------------------------------------*/
FreeEntry* EntryBuffer::pFirstFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pFirstFree;
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
  
  m_i_FreeOffsetHead = 0;
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
Entry* EntryBuffer::pGetEntrySplit(i8 iSplitEntryNo,Entry** ppPriorEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(m_pHeadEntry);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_pLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  *ppPriorEntry = nullptr;
  
  i8 iEntryNo = 0;
  for (Entry* pSplitEntry = pNextEntry(m_pHeadEntry,0);
       pSplitEntry;
       pSplitEntry = pNextEntry(pSplitEntry,0)) {
    
    // found split point?
    if (iEntryNo++ == iSplitEntryNo)
      return pSplitEntry;
    
    *ppPriorEntry = pSplitEntry;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a free entry (or C_ENTRY_NONE if none)                                 */
/*----------------------------------------------------------------------------*/
i4 EntryBuffer::iGetFreeEntry(void)
{
  if (C_ENTRY_NONE == m_i_FreeOffsetHead)
    return C_ENTRY_NONE;
  
  i4 i_FreeEntry = m_i_FreeOffsetHead;
  
  m_i_FreeOffsetHead = - m_pEntryOffset[i_FreeEntry];
  
  return i_FreeEntry;
}

/*----------------------------------------------------------------------------*/
/* handle the insert of a first key                                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vHandleFirstInsert(Entry* pInsertedKeyEntry,
                                     Entry* pOriginalFirstEntry,
                                     EntryBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (pFirstEntry() != pInsertedKeyEntry)
    return;
    
  if (pOriginalFirstEntry) {
    Key* aOriginalFirstKey = pOriginalFirstEntry->aKey();
    Key* aInsertedFirstKey = pInsertedKeyEntry->aKey();
    
    IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
    
    if (aInsertedFirstKey->bNotEqual(aOriginalFirstKey,pIdxColVec))
      vHandleLeftInsert(pInsertedKeyEntry,
                        pOriginalFirstEntry,
                        ppRootKeyBuffer);
  }
  else {
    vHandleLeftInsert(pInsertedKeyEntry,
                      pOriginalFirstEntry,
                      ppRootKeyBuffer);
  }
}

/*----------------------------------------------------------------------------*/
/* handle the insert of a left most key                                       */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vHandleLeftInsert(Entry* pInsertedKeyEntry,
                                    Entry* pOriginalFirstEntry,
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
  
  if (pOriginalFirstEntry) {
    Key* aDeleteKey = pOriginalFirstEntry->aKey();
    m_pHigherBuffer->vDeleteUpwards(aDeleteKey,ppRootKeyBuffer);
    //(*ppRootKeyBuffer)->vShowTree();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Key* aInsertKey = pInsertedKeyEntry->aKey();
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  Id xInsertId(*nBufferNo(),0);
  i8 iInsertKeyLen = aInsertKey->nKeySize(pIdxColVec);
  i8 nInsertOffsets = pInsertedKeyEntry->nOffsets();
  
  i8 iTxId = pInsertedKeyEntry->iTxId();
  
  KeyWrk xKeyWrkIns;
  xKeyWrkIns.vTxId(iTxId);
  xKeyWrkIns.vId(xInsertId);
  xKeyWrkIns.vEmbedded((char*)aInsertKey);
  xKeyWrkIns.vKeySize(iInsertKeyLen);
  xKeyWrkIns.vOffsets(nInsertOffsets);
  
  m_pHigherBuffer->pInsertKeyBuffer(xKeyWrkIns,ppRootKeyBuffer);
}

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bHasSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nFree = m_xWsp.nFree() - sizeof(Wilderness);
  
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
/* get head entry                                                             */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pHeadEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pHeadEntry;
}

/*----------------------------------------------------------------------------*/
/* create a head entry                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vHeadEntryNew(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
 
  RowWrk xHeadWrk;
  
  ASSERT( ! m_pHeadEntry );
  
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
  
  m_pHeadEntry = reinterpret_cast<Entry*>(aAllotSpace(nEntryBytes));
  ASSERT(m_pHeadEntry);
  char* aData = aDataArea();
  char* aHead = pAdd<char *>(aData,sizeof(i4) * C_ENTRIES_0);
  
  ASSERT((char*)m_pHeadEntry == aHead);
  
  new(m_pHeadEntry) Entry(CN_ENTRY,nEntryBytes,0);
  
  m_pHeadEntry->vSetEntry(0,xHeadWrk.xId(),3,nullptr,0);
  //m_pHeadEntry->vSetEntry(xHeadWrk);
  
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
Entry* EntryBuffer::pInsertKeyAbs(KeyWrk& xKeyWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Entry* pKeyEntry;
  
#ifdef EB_LAZY_CHAIN
  if (! m_iBufferLevel) {
    pKeyEntry = pInsertKeyAbsLazy(xKeyWrk);
    return pKeyEntry;
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
  
  pKeyEntry = pAllotEntry(nEntryBytes);
  ASSERT(pKeyEntry);
  
  NEO* aNEO = aAllotNEO(nOffsetBytes);
  ASSERT(aNEO);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the key entry
  
  pKeyEntry->vSetEntry(xKeyWrk);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the key entry into the skip list
  
  vInsertKeySkip(pKeyEntry);
  
  //if (! m_pIndex->bIsHidden() && m_iBufferLevel)
  //  bVerifyAdjacent();
  
  //vShow();
  
  return pKeyEntry;
}

/*----------------------------------------------------------------------------*/
/* insert a key entry specifically into this buffer (should be enough space)  */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pInsertKeyAbsLazy(KeyWrk& xKeyWrk)
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
  
  Entry* pKeyEntry = pAllotEntry(nEntryBytes);
  ASSERT(pKeyEntry);
  
  NEO* aNEO = aAllotNEO(nOffsetBytes);
  ASSERT(aNEO);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the key entry
  
  pKeyEntry->vSetEntry(xKeyWrk);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form lazy chain
  
  if (m_pLazyChain) {
    i8 iLazyOffset = iToOffset(m_pLazyChain);
    pKeyEntry->vNEO(0,iLazyOffset);
  }
  else {
    pKeyEntry->vNEO(0,-1);
  }

  m_pLazyChain = pKeyEntry;
  
  return pKeyEntry;
}

/*----------------------------------------------------------------------------*/
/* insert key entry into skip list of this buffer                             */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vInsertKeySkip(Entry* pKeyEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the entry after which the new entry will be inserted
  
  Entry* xPriorVec[gpAttachMap->m_nIndexOffsets];
  
  Key* aKey = pKeyEntry->aKey();
  
  if (0 == m_iBufferLevel)
    vPositionPriorVecEntry(aKey, xPriorVec, nullptr);
  else {
    EntryBuffer* pToBuffer = pKeyEntry->pReferencedEntryBuffer();
    vPositionPriorVecBuffer(aKey, xPriorVec, pToBuffer);
  }
  
  //vDumpPriorVec(xPriorVec);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set prior offsets
  
  i8 iKeyEntryOffset = iToOffset(pKeyEntry);
  
  i8 nOffsets = pKeyEntry->nOffsets();
  for (i8 iNextOffset = 0; iNextOffset < nOffsets; iNextOffset++) {
    i8 iPriorOffset = xPriorVec[iNextOffset]->iNEO(iNextOffset);
    pKeyEntry->vNEO(iNextOffset,iPriorOffset);
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
  
  Entry* pOriginalFirstEntry = pFirstEntry();
  if (!pOriginalFirstEntry)
    pOriginalFirstEntry = pFirstEntry();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the key
  
  Entry* pInsertedKeyEntry = pInsertKeyAbs(xKeyWrk);
  
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
  
  Entry* pInsertedKeyEntry;

#ifdef EB_LAZY_CHAIN
  if (m_iBufferLevel) {
    Entry* pOriginalFirstEntry = pFirstEntry();
    pInsertedKeyEntry = pInsertKeyAbs(xKeyWrk);
    vHandleFirstInsert(pInsertedKeyEntry,pOriginalFirstEntry,ppRootKeyBuffer);
  }
  else {
    pInsertedKeyEntry = pInsertKeyAbs(xKeyWrk);
  }
#else
  Entry* pOriginalFirstEntry = pFirstEntry();
  pInsertedKeyEntry = pInsertKeyAbs(xKeyWrk);
  vHandleFirstInsert(pInsertedKeyEntry,pOriginalFirstEntry,ppRootKeyBuffer);
#endif
  
  return pInsertedKeyEntry;
}

/*----------------------------------------------------------------------------*/
/* insert a new key entry into this key tree (handling ripple upwards)        */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pInsertKeyUpwards(KeyWrk& xKeyWrk,
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

  Entry* pInsertedKeyEntry =
    pInsertIntoBuffer->pInsertKeyBuffer(xKeyWrk,ppRootKeyBuffer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pInsertIntoBuffer->m_iBufferLevel) {
    Id xId = pInsertedKeyEntry->xId();
    i8 nBuffer = xId.iBuffer();
    
    HeapBuffer* pHeapBuffer =
    reinterpret_cast<HeapBuffer*>(gpBufferPool->pToBuffer(nBuffer));
    
    EntryBuffer* pChildBuffer =
    reinterpret_cast<EntryBuffer*>(pHeapBuffer->pFirstChunk());
    
    if (pChildBuffer->m_pHigherBuffer != pInsertIntoBuffer) {
      //(*ppRootKeyBuffer)->vShowTree();
      pChildBuffer->m_pHigherBuffer = pInsertIntoBuffer;
      //(*ppRootKeyBuffer)->vShowTree();
    }
  }
  
  return pInsertedKeyEntry;
}

/*----------------------------------------------------------------------------*/
/* restore item count                                                         */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vItemCountRestore(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_nItems = 0;
  for (Entry* pEntry = pFirstEntry(); pEntry; pEntry = pNextEntry(pEntry, 0))
    m_nItems++;
}

/*----------------------------------------------------------------------------*/
/* is given key before the first key of this buffer                           */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bKeyBeforeBuffer(Key* aKeyTest)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Entry* pFirstEntry = this->pFirstEntry();
  Key* aFirstKey = pFirstEntry->aKey();
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  i8 nKeyCols = pIdxColVec->nItems();
  
  bool bIsEqual;
  bool bIsJuxta =
  //aKeyTest->bJuxtaCompare(JC_BEFORE,aFirstKey,pIdxColVec,nKeyCols,bIsEqual);
    aKeyTest->bLessThan(aFirstKey,pIdxColVec);
  
  return bIsJuxta;
}

/*----------------------------------------------------------------------------*/
/* convert given entry into free entry                                        */
/*----------------------------------------------------------------------------*/
FreeEntry* EntryBuffer::pMakeFreeEntry(Entry* pEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nSizeEntry = pEntry->nSizeEntry();
  i8 nSizeNEO   = pEntry->iNEOsize();
  i8 nSizeFree  = nSizeEntry + nSizeNEO;
  
  FreeEntry* pFreeEntry = new(pEntry) FreeEntry(nSizeFree);

  return pFreeEntry;
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
  EntryBuffer* pRightBuffer = pLeftBuffer->pSplit2(ppRootKeyBuffer);
#else
  EntryBuffer* pRightBuffer = pLeftBuffer->pSplit(ppRootKeyBuffer);
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the first key of left buffer into higher buffer
  
  if ( ! pHigherBuffer->nItems() ) {
    Entry* pFirstKeyLeft  = pLeftBuffer->pFirstEntry();
    
    Id xIdLeft(*pLeftBuffer->nBufferNo(),0);
    i8 iTxIdLeft = pFirstKeyLeft->iTxId();
    Key* aKeyLeft = pFirstKeyLeft->aKey();
    i8 iKeyLenLeft = aKeyLeft->nKeySize(m_pIndex->pIdxColVec());
    i8 nOffsetsLeft = pFirstKeyLeft->nOffsets();
    
    
    KeyWrk xKeyWrkLeft;
    xKeyWrkLeft.vTxId(iTxIdLeft);
    xKeyWrkLeft.vId(xIdLeft);
    xKeyWrkLeft.vEmbedded((char*)aKeyLeft);
    xKeyWrkLeft.vKeySize(iKeyLenLeft);
    xKeyWrkLeft.vOffsets(nOffsetsLeft);
    
    
    Entry* pNewKeyLeft =
    pHigherBuffer->pInsertKeyUpwards(xKeyWrkLeft,ppRootKeyBuffer);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the first key of right buffer into higher buffer
  
  Entry* pFirstKeyRight = pRightBuffer->pFirstEntry();
  
  Id xIdRight(*pRightBuffer->nBufferNo(),0);
  i8 iTxIdRight = pFirstKeyRight->iTxId();
  Key* aKeyRight = pFirstKeyRight->aKey();
  i8 iKeyLenRight = aKeyRight->nKeySize(m_pIndex->pIdxColVec());
  i8 nOffsetsRight = pFirstKeyRight->nOffsets();
  
  
  KeyWrk xKeyWrkRight;
  xKeyWrkRight.vTxId(iTxIdRight);
  xKeyWrkRight.vId(xIdRight);
  xKeyWrkRight.vEmbedded((char*)aKeyRight);
  xKeyWrkRight.vKeySize(iKeyLenRight);
  xKeyWrkRight.vOffsets(nOffsetsRight);
  
  
  Entry* pNewKeyRight =
  pHigherBuffer->pInsertKeyUpwards(xKeyWrkRight,ppRootKeyBuffer);
  
  
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
      vShow();
      ASSERT(0);
      break;
  }
  
  return pItemNext;
}

/*----------------------------------------------------------------------------*/
/* pop entry from lazy chain                                                  */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pPopLazyChain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (! m_pLazyChain )
    return nullptr;
  
  Entry* pPoppedLazyEntry = m_pLazyChain;
  
  i8 iNextLazyOffset = pPoppedLazyEntry->iNEO(0);
  
  if (-1 == iNextLazyOffset)
    m_pLazyChain = nullptr;
  else
    m_pLazyChain = pGetEntryOffset(iNextLazyOffset);
  
  return pPoppedLazyEntry;
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
      RASSERT( ! m_pLazyChain );
#endif
    }
    
    // get the entry before the given insert key
    Entry* pEntryPrior = pEntryBuffer->pPositionPriorEntry(aFindKey);
    ASSERT(pEntryPrior);
    if (pEntryPrior == pEntryBuffer->pHeadEntry())
      pEntryPrior = pEntryBuffer->pNextEntry(pEntryPrior,0);
    ASSERT(pEntryPrior);
    
    // get the index buffer of the entry
    //pEntryBuffer->vShow();
    ASSERT(pEntryBuffer != pEntryPrior->pReferencedEntryBuffer());
    pEntryBuffer = pEntryPrior->pReferencedEntryBuffer();
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
  RASSERT( ! m_pLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (nItems() == 0)
    return m_pHeadEntry;
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  i8 nKeyCols = pIdxColVec->nItems();
  
  Entry* pScanEntry_sl = m_pHeadEntry;
  
  for (i8 i_ScanOffset = gpAttachMap->m_nIndexOffsets - 1;
       i_ScanOffset >= 0;
       i_ScanOffset--) {
    
    Entry* pNextEntry_sl = pNextEntry(pScanEntry_sl,i_ScanOffset);
    while (pNextEntry_sl) {
      
      Key* aNextKey = pNextEntry_sl->aKey();
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
      bool bKeyIsGreaterEqual = aNextKey->bGreaterEqual(aFindKey,pIdxColVec);
      if (bKeyIsGreaterEqual)
        break;
      
      pScanEntry_sl = pNextEntry_sl;
      pNextEntry_sl = pNextEntry(pNextEntry_sl,i_ScanOffset);
    }
  }
  
  return pScanEntry_sl;
}

/*----------------------------------------------------------------------------*/
/* compute prior vector for find key, such that next of priorVec[0] is        */
/* specifically the given find entry                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vPositionPriorVecEntry(Key* aFindKey, Entry* pPriorVec[],
                                         Entry* pFindEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //ASSERT(0 == m_iBufferLevel);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_pLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexOffsets; i_Offset++)
    pPriorVec[i_Offset] = m_pHeadEntry;
  
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  i8 iFindLevels = pFindEntry ? pFindEntry->nOffsets() - 1 : 2;
  
  Entry* pScanEntry_sl = m_pHeadEntry;
  for (i8 i_ScanOffset = gpAttachMap->m_nIndexOffsets - 1;
       i_ScanOffset >= 0;
       i_ScanOffset--) {
    
    Entry* pNextEntry_sl = pNextEntry(pScanEntry_sl,i_ScanOffset);
    while (pNextEntry_sl) {
      
      if (pFindEntry == pNextEntry_sl)
        break;
      
      Key* aNextKey = pNextEntry_sl->aKey();
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
      bool bIsJuxta = aNextKey->bGreaterEqual(aFindKey,pIdxColVec);
      
      if (bIsJuxta) {
        if (! pFindEntry || pNextEntry_sl == pFindEntry)
          break;
        
        if (i_ScanOffset > iFindLevels)
          break;
      }
      
      pScanEntry_sl = pNextEntry_sl;
      pNextEntry_sl = pNextEntry(pNextEntry_sl,i_ScanOffset);
    }
    pPriorVec[i_ScanOffset] = pScanEntry_sl;
  }
}

/*----------------------------------------------------------------------------*/
/* compute prior vec for find key, such that next of priorVec[0] is the next  */
/* buffer after find keys toBuffer (ie nextBuffer->prevBuffer == toBuffer)    */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vPositionPriorVecBuffer(Key* aFindKey, Entry* pPriorVec[],
                                          EntryBuffer* pToBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(0 != m_iBufferLevel);
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_pLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexOffsets; i_Offset++)
    pPriorVec[i_Offset] = m_pHeadEntry;
  
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  Entry* pScanEntry_sl = m_pHeadEntry;
  for (i8 i_ScanOffset = gpAttachMap->m_nIndexOffsets - 1;
       i_ScanOffset >= 0;
       i_ScanOffset--) {
    
    Entry* pNextEntry_sl = pNextEntry(pScanEntry_sl,i_ScanOffset);
    while (pNextEntry_sl) {
      
      Key* aNextKey = pNextEntry_sl->aKey();
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
      bool bNextKeyIsGreater = aNextKey->bGreaterThan(aFindKey,pIdxColVec);
      if (bNextKeyIsGreater)
        break;
      
      if (0 == i_ScanOffset) {
        bool bKeysAreEqual = aNextKey->bEqualTo(aFindKey,pIdxColVec);
        if (bKeysAreEqual &&
            (pToBuffer == pNextEntry_sl->pReferencedEntryBuffer()->pPrevBuffer()))
          break;
      }
      
      pScanEntry_sl = pNextEntry_sl;
      pNextEntry_sl = pNextEntry(pNextEntry_sl,i_ScanOffset);
    }
    pPriorVec[i_ScanOffset] = pScanEntry_sl;
  }
}

/*----------------------------------------------------------------------------*/
/* split this buffer into two: this original buffer and a new right buffer    */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pSplit(EntryBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);

  static i8 nSplit = 0;
  //vTrace("pSplit %lld (%lld items)\n"_Stz,++nSplit,nItems());

  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_pLazyChain );
#endif
  
  EntryBuffer* pThisLeftBuffer = this;
  
  //pThisLeftBuffer->vShow();
  
  //ASSERT( (* ppRootKeyBuffer)->bVerify() );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find split entry. every entry from split entry and to the right of it
  // will be moved to the new right buffer
  
  Entry* pPriorEntry;
  i8 iSplitAtEntry = m_nItems/2;
  Entry* pSplitEntry = pGetEntrySplit(iSplitAtEntry,&pPriorEntry);
  
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
  
  Entry* pNextEntry;
  for (Entry* pMoveEntry = pSplitEntry;
       pMoveEntry;
       pMoveEntry = pNextEntry) {
  
    ASSERT(! m_iBufferLevel ||
           pMoveEntry->pReferencedEntryBuffer()->m_pHigherBuffer == pThisLeftBuffer);

    pNextEntry = this->pNextEntry(pMoveEntry,0);

    i8 iTxId = pMoveEntry->iTxId();
    Id xId = pMoveEntry->xId();
    Key* aKey = pMoveEntry->aKey();
    i8 nKeySize = aKey->nKeySize(pIdxColVec);
    i8 nOffsets = pMoveEntry->nOffsets();
    
    xKeyWrk.vTxId(iTxId);
    xKeyWrk.vId(xId);
    xKeyWrk.vEmbedded((char*)aKey);
    xKeyWrk.vKeySize(nKeySize);
    xKeyWrk.vOffsets(nOffsets);
    
    Entry* pNewEntry = pNewRightBuffer->pInsertKeyAbs(xKeyWrk);
    
    ASSERT(! m_iBufferLevel ||
           pNewEntry->pReferencedEntryBuffer()->m_pHigherBuffer == pThisLeftBuffer);
    
    i8 nBuffer = xId.iBuffer();
    HeapBuffer* pHeapBuffer =
      reinterpret_cast<HeapBuffer*>(gpBufferPool->pToBuffer(nBuffer));
    
    EntryBuffer* pChildBuffer =
      reinterpret_cast<EntryBuffer*>(pHeapBuffer->pFirstChunk());
    
    ASSERT(pChildBuffer->m_pHigherBuffer ==
           pNewEntry->pReferencedEntryBuffer()->m_pHigherBuffer);
    
    vDeleteEntry(&pMoveEntry);
  }
  
  //pThisLeftBuffer->vShow();
  //pNewRightBuffer->vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // the child buffer of any moved entry needs to have its higher buffer
  // corrected to the new right buffer
  
  Id xId;
  if (m_iBufferLevel != 0)  {
    for (Entry* pMovedEntry = pNewRightBuffer->pFirstEntry();
         pMovedEntry;
         pMovedEntry = pNewRightBuffer->pNextEntry(pMovedEntry, 0)) {
      
      xId = pMovedEntry->xId();
      i8 nBuffer = xId.iBuffer();
      
      HeapBuffer* pHeapBuffer =
        reinterpret_cast<HeapBuffer*>(gpBufferPool->pToBuffer(nBuffer));
      
      EntryBuffer* pChildBuffer =
        reinterpret_cast<EntryBuffer*>(pHeapBuffer->pFirstChunk());
      
      ASSERT(pChildBuffer->m_pHigherBuffer ==
             pMovedEntry->pReferencedEntryBuffer()->m_pHigherBuffer);
      
      
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
EntryBuffer* EntryBuffer::pSplit2(EntryBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  static i8 nSplit = 0;
  //vTrace("pSplit2 %lld (%lld items)\n"_Stz,++nSplit,nItems());
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
#ifdef EB_LAZY_CHAIN
  RASSERT( ! m_pLazyChain );
#endif
  
  EntryBuffer* pThisLeftBuffer = this;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a new right buffer
  
  EntryBuffer* pNewRightBuffer = pNewNextBuffer();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find split entry. every entry from split entry and to the right of it
  // will be moved to the new right buffer
  
  Entry* pPriorEntry;
  i8 iSplitAtEntry = m_nItems/2;
  Entry* pSplitEntry = pGetEntrySplit(iSplitAtEntry,&pPriorEntry);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy split right entries to a new right buffer
  
  KeyWrk xKeyWrk;
  Entry* pCopyEntry;
  Entry* pNextEntry;
  
  for (pCopyEntry = pSplitEntry;
       pCopyEntry;
       pCopyEntry = pNextEntry) {
    
    ASSERT(! m_iBufferLevel ||
           pCopyEntry->pReferencedEntryBuffer()->m_pHigherBuffer == pThisLeftBuffer);
    
    pNextEntry = this->pNextEntry(pCopyEntry,0);
    
    i8 iTxId = pCopyEntry->iTxId();
    Id xId = pCopyEntry->xId();
    Key* aKey = pCopyEntry->aKey();
    i8 nKeySize = aKey->nKeySize(pIdxColVec);
    i8 nOffsets = pCopyEntry->nOffsets();
    
    xKeyWrk.vTxId(iTxId);
    xKeyWrk.vId(xId);
    xKeyWrk.vEmbedded((char*)aKey);
    xKeyWrk.vKeySize(nKeySize);
    xKeyWrk.vOffsets(nOffsets);
    
    Entry* pNewEntry = pNewRightBuffer->pInsertKeyAbsLazy(xKeyWrk);
    
    // make the moved entry in the left buffer to be a free entry
    FreeEntry* pFreeEntry = pMakeFreeEntry(pCopyEntry);
    
    // these can be commented
    ASSERT(! m_iBufferLevel ||
           pNewEntry->pReferencedEntryBuffer()->m_pHigherBuffer == pThisLeftBuffer);
    
    i8 nBuffer = xId.iBuffer();
    HeapBuffer* pHeapBuffer =
      reinterpret_cast<HeapBuffer*>(gpBufferPool->pToBuffer(nBuffer));
    
    EntryBuffer* pChildBuffer =
      reinterpret_cast<EntryBuffer*>(pHeapBuffer->pFirstChunk());
    
    ASSERT(pChildBuffer->m_pHigherBuffer ==
           pNewEntry->pReferencedEntryBuffer()->m_pHigherBuffer);
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cut off right entries at L0
  
  ASSERT(this->pNextEntry(pPriorEntry,0) == pSplitEntry);
  pPriorEntry->vNEO(0,-1);
  
  // - at this point the left buffer no longer has the right latter entries
  //   and the SkipList is not valid (other than L0)
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // build the skip list of the right buffer
  
  pNewRightBuffer->vBuildSkipListSorted();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compact the left buffer and rebuild SkipList
  
  pThisLeftBuffer->vCompactLeft();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // the child buffer of any moved entry needs to have its higher buffer
  // corrected to the new right buffer
  
  Id xId;
  if (m_iBufferLevel != 0)  {
    for (Entry* pMovedEntry = pNewRightBuffer->pFirstEntry();
         pMovedEntry;
         pMovedEntry = pNewRightBuffer->pNextEntry(pMovedEntry, 0)) {
      
      xId = pMovedEntry->xId();
      i8 nBuffer = xId.iBuffer();
      
      HeapBuffer* pHeapBuffer =
      reinterpret_cast<HeapBuffer*>(gpBufferPool->pToBuffer(nBuffer));
      
      EntryBuffer* pChildBuffer =
      reinterpret_cast<EntryBuffer*>(pHeapBuffer->pFirstChunk());
      
      ASSERT(pChildBuffer->m_pHigherBuffer ==
             pMovedEntry->pReferencedEntryBuffer()->m_pHigherBuffer);
      
      
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
/* quick verify                                                               */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vQuickVerify(Entry* pSeeEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);

  for (Item* pItem = pFirstItem();
       pItem != nullptr;
       pItem = pNextItem(pItem))
    ;
  
  for (Entry* pTestEntry = pNextEntry(m_pHeadEntry,0);
       pTestEntry;
       pTestEntry = pNextEntry(pTestEntry,0)) {
  
    if (pSeeEntry)
      pTestEntry->vShow(iToOffset(pTestEntry),m_pIndex,true);
    
    if (pTestEntry == pSeeEntry)
      int iDebug = 1;
    
    for (i8 i_ScanOffset = pTestEntry->nOffsets()-1; i_ScanOffset >= 0; i_ScanOffset--) {
      Entry* pCheckEntry = pNextEntry(pTestEntry,i_ScanOffset);
      if (!pCheckEntry)
        continue;
      
      if (CN_ENTRY != pCheckEntry->eClass())
        vShow();
      ASSERT(CN_ENTRY == pCheckEntry->eClass());
    }
 
  }

//  ln i_ScanOffset;
//  for (i_ScanOffset = 2; *i_ScanOffset >= 0; i_ScanOffset--) {
//
//    for (Entry* pTestEntry = pNextEntry(m_pHeadEntry,*i_ScanOffset);
//         pTestEntry;
//         pTestEntry = pNextEntry(pTestEntry,*i_ScanOffset)) {
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
bool EntryBuffer::bQuickVerify(Entry* pNotSeeEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  for (Entry* pTestEntry = pNextEntry(m_pHeadEntry,0);
       pTestEntry;
       pTestEntry = pNextEntry(pTestEntry,0)) {
    
    for (i8 i_ScanOffset = pTestEntry->nOffsets()-1; i_ScanOffset >= 0; i_ScanOffset--) {
      
      Entry* pCheckEntry = pNextEntry(pTestEntry,i_ScanOffset);
      if (!pCheckEntry)
        continue;
      
      if (pCheckEntry == pNotSeeEntry) {
        pTestEntry->vShow(iToOffset(pTestEntry),m_pIndex,true);
        pCheckEntry->vShow(iToOffset(pCheckEntry),m_pIndex,true);
        return false;
      }
      
      if (CN_ENTRY != pCheckEntry->eClass())
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
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify the contents of child buffers
  
  for (Entry* pEntry = pFirstEntry(); pEntry; pEntry = pNextEntry(pEntry,0)) {
    
    EntryBuffer* pRefEntBuf = pEntry->pReferencedEntryBuffer();
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
  
  RASSERT(m_iBufferLevel >= 1);
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  for (Entry* pEntry = pFirstEntry(); pEntry; pEntry = pNextEntry(pEntry,0)) {
    
    // get the buffer that this entry references
    EntryBuffer* pChildEntryBuffer = pEntry->pReferencedEntryBuffer();
    
    // get the adjacent entry
    Entry* pAdjacentEntry = pNextEntry(pEntry,0);
    
    if (pAdjacentEntry) {
      // get the buffer that the adjacent entry references
      EntryBuffer* pAdjacentChildEntryBuffer = pAdjacentEntry->pReferencedEntryBuffer();
      
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
  
  RASSERT(m_iBufferLevel >= 1);
  
  for (Entry* pEntry = pFirstEntry(); pEntry; pEntry = pNextEntry(pEntry,0)) {
    
    EntryBuffer* pChildEntBuf = pEntry->pReferencedEntryBuffer();
    
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
/* verify the entry offset array of this buffer                               */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyEntryOffset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT(0 == m_iBufferLevel);
  RASSERT(m_pIndex->bIsHidden());
  
  //vShow();
  
  for (i8 i_Offset = 0; i_Offset < C_ENTRIES_0; i_Offset++) {

    i8 i_EntryOffset = m_pEntryOffset[i_Offset];
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // validate free offset element
    
    if (i_EntryOffset < 0) {
      
      // last offset in free chain
      if (C_ENTRY_NONE == - i_EntryOffset)
        continue;
      
      // get the next offset in free chain
      i8 i_NextEntryOffset = m_pEntryOffset[ - i_EntryOffset ];
      
      // next offset should be free (or end of free chain)
      if (i_NextEntryOffset >= 0) {
        vVerifyFault();
        return false;
      }
      
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // validate offset element which is the offset to an entry
    
    Entry* pEntry = pGetEntryOffset(i_EntryOffset);
    if (! pEntry->bValidEntry())
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
  
  RASSERT(m_iBufferLevel >= 1);
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  for (Entry* pEntry = pFirstEntry(); pEntry; pEntry = pNextEntry(pEntry,0)) {
    
    // get the entry buffer that this entry references
    EntryBuffer* pChildEntryBuffer = pEntry->pReferencedEntryBuffer();
    
    // get the first entry of the child buffer
    Entry* pChildFirstEntry = pChildEntryBuffer->pFirstEntry();

    // get keys of the two entries
    Key* pKeyParent = pEntry->aKey();
    Key* pKeyChild  = pChildFirstEntry->aKey();
   
    // the two keys should match
    bool bEqual = pKeyChild->bEqualTo(pKeyParent,pIdxColVec);
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
  
  for (FreeEntry* pFreeEntry = pFirstFree();
       pFreeEntry != nullptr;
       pFreeEntry = pFreeEntry->pNextFree()) {
    
    if (! pFreeEntry->bValidFreeEntry()) {
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
  
  i8 nEntryItems = 0;
  
  for (Item* pItem = pFirstItem();
       pItem != nullptr;
       pItem = pNextItem(pItem)) {
    
    //vTrace("Verify item %p\n"_Stz,pItem);
    
    eCN eClass = pItem->eClass();
    
    switch (eClass) {
      
      case CN_ENTRY: {
        Entry* pEntry = _pEntry(pItem);
        if (! pEntry->bValidEntry()) {
          vShow();
          vVerifyFault();
          return false;
        }
        nEntryItems++;
        break;
      }
      
      case CN_FREEENTRY: {
        FreeEntry* pFreeEntry = _pFreeEntry(pItem);
        if (! pFreeEntry->bValidFreeEntry()) {
          vShow();
          vVerifyFault();
          return false;
        }
        break;
      }
        
      case CN_WILDERNESS: {
        Wilderness* pWilderness = _pWilderness(pItem);
        if (! pWilderness->bValidWilderness()) {
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
  
  RASSERT(m_iBufferLevel >= 1);
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  for (Entry* pEntry = pFirstEntry(); pEntry; pEntry = pNextEntry(pEntry,0)) {
    
    // get next entry
    Entry* pNextEntry = this->pNextEntry(pEntry,0);
    if (! pNextEntry)
      break;
    
    // get the child entry buffers
    EntryBuffer* pChildBufferA = pEntry->pReferencedEntryBuffer();
    EntryBuffer* pChildBufferB = pNextEntry->pReferencedEntryBuffer();
    
    // child A should point to child B
    if (pChildBufferA->pNextBuffer() != pChildBufferB) {
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
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  for (Entry* pEntry = pFirstEntry(); pEntry; pEntry = pNextEntry(pEntry,0)) {
    
    // get next entry
    Entry* pNextEntry = this->pNextEntry(pEntry,0);
    if (! pNextEntry)
      break;
    
    // get keys of the two entries
    Key* pKeyA = pEntry->aKey();
    Key* pKeyB = pNextEntry->aKey();

    // next keyB should be greater than or equal to keyA
    bool bGreaterEqual = pKeyB->bGreaterEqual(pKeyA,pIdxColVec);
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
  
  for (Entry* pEntry = pFirstEntry(); pEntry; pEntry = pNextEntry(pEntry,0)) {
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the row references in this L0 non-hidden index buffer               */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyRowRefs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT(0 == m_iBufferLevel);
  RASSERT(! m_pIndex->bIsHidden());
  
  for (Entry* pEntry = pFirstEntry(); pEntry; pEntry = pNextEntry(pEntry,0)) {
    
    // get the entry buffer that this L0 key leaf references
    EntryBuffer* pEntryBuffer = pEntry->pReferencedEntryBuffer();
    
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
    i8 i_Row = pEntry->iRow();
    Entry* pRowEntry = pEntryBuffer->pGetEntryNo(i_Row);
    
    // validate the row entry                                          //# more?
    if (1 != pRowEntry->nOffsets()) {
      vVerifyFault();
      return false;
    }
  }
  
  return true;
}

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
  
  Formatters* pFormatters = &gpAttachMap->m_xFormatters;
  
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
    //i8 iInt = iRndNo[iCount]; //Random::iRandom(10);
    //i8 iInt = Random::iRandom(1000);
  
  // duplicate keys
  for (i8 iInt = 1; iInt <= 10; iInt++)
  for (i8 iCount = 0; iCount < iInt; iCount++) {
    
    i8 iKey = iInt;
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    sprintf(sRecData.aChar(),"%lld,%s,1900-01-01 00:00:%lld",
            iKey,IntLib::sAsText(iKey),iKey);
    
    char* aRowData = (char*)xRowWrk.aRow();
    iRecLength = sRecData.iLength();
    
    bFormatted = Row::bFormatRow(pColVec,pFormatters,
                                 sRecData,iRecLength,
                                 (Stz*)aRowData,iRowLength,
                                 bBlankLine,
                                 false /* allowNonPrintable */);
    
    xRowWrk.vRowSize(iRowLength);
    xRowWrk.vTxId(0);
    
    //((Row*)aRowData)->vDump(pColVec);
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    Entry* pRowEntry = pLastRowBuffer->pAppendRow(xRowWrk);
    
    if ( ! pRowEntry ) {
      pLastRowBuffer = pLastRowBuffer->pNewNextBuffer();
      pRowEntry = pLastRowBuffer->pAppendRow(xRowWrk);
    }
  
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    xKeyWrk.vReset();
    pRowEntry->vExtractIndexKey(xKeyWrk,pIdxColVec1);
    //xKeyWrk.vShow(pIdxColVec1);
    
    xKeyWrk.vTxId(0);
    xKeyWrk.vId(pRowEntry->xId());
    
    Entry* pKeyEntry =
      EntryBuffer::pInsertKeyTree(xKeyWrk,&pRootKeyBuffer);
    
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
