/*============================================================================*/
/* EntryBuffer_all.cpp -- working file to sort out the fun's of Item PoolBuffer  */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
    
    ASSERT(m_iLevelNo == 0);
    EntryBuffer* pNextBuffer = this;
    
    do {
      ASSERT(pNextBuffer->iLevelNo() == 0);
      
      iBuffers++;
      iEntries += (pNextBuffer->m_nItems);
      
      if (bDetailed)
        sAnalysis.vCatSprint("row: (%s, %lld, %5lld, 'L%lld', %lld )\n"_Stz,
                             gpAttachMap->m_pGrid->pThisNode()->aNodeName(),
                             pNextBuffer->m_iPartitionNo,
                             iBuffers,
                             pNextBuffer->m_iLevelNo,
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
                         m_iPartitionNo,
                         iBuffers,
                         m_iLevelNo,
                         m_nItems);
  
  // recursively dump the lower level buffers
  if (m_iLevelNo != 0) {
    
    for (Item* aItem = aLeadEntry();
         aItem != nullptr;
         aItem = aNextEntry(aItem, 0)) {
      
      // get the index buffer from the entry
      EntryBuffer* pEntryBuffer = EntryBuffer::pRefdEntryBuffer(aItem);
      
      ASSERT(pEntryBuffer);
      ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
      
      pEntryBuffer->vAnalyze(sAnalysis, bDetailed, iBuffers, iEntries);
      
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
   
   for (Item* aItem = aSkipRoots();
   aItem != nullptr;
   aItem = aNextEntry(aItem, 0)) {
   
   // get the number of offsets for this entry
   i8 nOffsets = aItem->nOffsets();
   
   // connect the dots
   for (i8 i_Offset = 1; i_Offset < nOffsets; i_Offset++)
   aItem->vNEO(i_Offset,-1);
   }
   vShow();
   */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Item* xPrevVec[gpAttachMap->m_nIndexOffsets];
  
  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexOffsets; i_Offset++)
    xPrevVec[i_Offset] = aSkipRoots();
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // go through each entry in L0 order and
  //   correct the prior offset to that entry for each level
  
  for (Item* aItem = aLeadEntry();
       aItem != nullptr;
       aItem = aNextEntry(aItem, 0)) {
    
    // where is this entry in the buffer (offset-wise)?
    i4 iEntryOffset = iToOffset(aItem);
    
    // get the number of offsets for this entry
    i8 nOffsets = aItem->nOffsets();
    
    // connect the dots
    for (i8 i_Offset = 1; i_Offset < nOffsets; i_Offset++) {
      
      // set previous entry at this level to refer to this entry
      xPrevVec[i_Offset]->vNEO(i_Offset,iEntryOffset);
      
      // the previous entry at this level now becomes this entry
      xPrevVec[i_Offset] = aItem;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* delete the given entry                                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDeleteEntry(Item** aaItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // currently we only delete from non-hidden indexes
  ASSERT(! m_pIndex->bIsHidden());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Item* aItem = *aaItem;
  
  Item* xPriorVec[gpAttachMap->m_nIndexOffsets];
  
  //KeyData* aKey = _aKeyData(aItem->aDataArea());
  
  vPositionPriorVecEntry(aItem, xPriorVec, aItem);
  
  vCutEntry(aItem,xPriorVec);
  
  vFreeItem(aItem);
  
  // entry no longer exists, set the pointer to be null
  *aaItem = nullptr;
}

/*----------------------------------------------------------------------------*/
/* delete key                                                                 */
/*----------------------------------------------------------------------------*/
//bool EntryBuffer::bDeleteKey(KeyData* aDeleteKey, EntryBuffer* pToBuffer)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
//  
//#ifdef USE_EB_LAZY_CHAIN
//  RASSERT( ! m_aLazyChain );
//#endif
//  
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  
//  Item* xPriorVec[gpAttachMap->m_nIndexOffsets];
//  
//  vPositionPriorVecBuffer3(aDeleteKey,xPriorVec,pToBuffer);
//  
//  Item* aPriorEntry = xPriorVec[0];
//  
//  Item* aEntryToBeDeleted = aNextEntry(aPriorEntry,0);
//  if (!aEntryToBeDeleted)
//    return false;
//  
//  //vTrace("[%lld] pDeleteEntry key %lld\n"_Stz,
//  //       *nPoolBufferNo(),
//  //       pEntryToBeDeleted->aKey()->iGetInt());
//  
//  KeyData* aDeleteEntryKey = _aKeyData(aEntryToBeDeleted->aDataArea());
//  
//  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
//  //if (! aDeleteEntryKey->bEqualTo(aDeleteKey,pIdxColVec))
//  //  return;
//  RASSERT(aDeleteEntryKey->bEqualTo(aDeleteKey,pIdxColVec));
//  RASSERT(pToBuffer == EntryBuffer::pRefdEntryBuffer(aEntryToBeDeleted));
//  
//  vDeleteEntry(&aEntryToBeDeleted);
//  
//  return true;
//}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // show this buffer
  vShow();
  
  // recursively dump the lower level buffers
  if (m_iLevelNo != 0) {
    
    for (Item* aItem = aLeadEntry();
         aItem != nullptr;
         aItem = aNextEntry(aItem, 0)) {
      
      // get the index buffer from the entry
      EntryBuffer* pEntryBuffer = EntryBuffer::pRefdEntryBuffer(aItem);
      
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
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
  
  //if (m_aLazyChain)
  //  vBuildSkipList();
#endif
  
  Item* aFirstEntry = this->aLeadEntry();
  EntryBuffer* pChildEntryBuffer = EntryBuffer::pRefdEntryBuffer(aFirstEntry);
  
  return pChildEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* insert a key entry specifically into this buffer (should be enough space)  */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aInsertKeyAbs(KeyWrk& xKeyWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Item* aKeyEntry;
  
#ifdef USE_EB_LAZY_CHAIN
  if (m_pIndex->pTable()->bIsLoading()) {
    if (! m_iLevelNo) {
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
  
  xKeyWrk.vSetEntry(aKeyEntry);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the key entry into the skip list
  
  vInsertKeySkip(aKeyEntry);
  
  //if (! m_pIndex->bIsHidden() && m_iLevelNo)
  //  bVerifyAdjacent();
  
  //vShow();
  
  return aKeyEntry;
}

/*----------------------------------------------------------------------------*/
/* insert a key entry specifically into this buffer (L0 chain only)           */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aInsertKeyAbsL0(KeyWrk& xKeyWrk, Item** aaPriorEntry)
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
  
  Item* aKeyEntry = aAllotEntry(nEntryBytes);
  ASSERT(aKeyEntry);
  
  NEO* aNEO = aAllotNEO(nOffsetBytes);
  ASSERT(aNEO);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the key entry
  
  xKeyWrk.vSetEntry(aKeyEntry);
  
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
Item* EntryBuffer::aInsertKeyTree(KeyWrk& xKeyWrk,
                                   EntryBuffer** ppRootKeyBuffer)
{
  // ** note: this is a static function **
  
  
  EntryBuffer* pRootKeyBuffer = *ppRootKeyBuffer;
  
#ifdef USE_EB_WATCH
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
  
  //KeyData* aInsKey = xKeyWrk.aKey();
  //i8 iInsKeyLen = xKeyWrk.nKeySize();
  
  //EntryBuffer* pPositionedBuffer =
  //pRootKeyBuffer->pPositionPriorBuffer(aInsKey);
  
  EntryBuffer* pPositionedBuffer =
  pRootKeyBuffer->pPositionPriorBuffer(xKeyWrk);
  
  ASSERT(0 == pPositionedBuffer->iLevelNo());
  //pPositionedBuffer->vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert into the tree
  
  
  Item* aInsertedKeyEntry =
  pPositionedBuffer->aInsertKeyUpwards(xKeyWrk,ppRootKeyBuffer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef USE_EB_WATCH
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
Item* EntryBuffer::aNextEntry(Item* aCurrent, i8 i_Offset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT(aCurrent->bValidItem());
  
  i4 iOffset = aCurrent->iNEO(i_Offset);
  
  if (-1 == iOffset)
    return nullptr;
  
  Item* aItem = aAdd<Item*>(aItemArea(), iOffset);
  
  RASSERT(! aItem || aItem->bValidItem());

  return aItem;
}

/*----------------------------------------------------------------------------*/
/* position at an index buffer at given level starting from this buffer       */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pPositionLevel(KeyData* aGivenKey, i8 nKeyCols,
                                         eJC eJuxtaCompare, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
  
  //if (m_aLazyChain)
  //  vBuildSkipList();
#endif
  
  EntryBuffer* pEntryBuffer = this;
  
  i8 iBufferLevel;
  Item* aItem;
  
  while (true) {
    iBufferLevel = pEntryBuffer->iLevelNo();
    
    if (iBufferLevel == iLevel)
      break;
    
    // get the entry
    aItem = pEntryBuffer->aPositionQuery(aGivenKey, nKeyCols, eJuxtaCompare);
    ASSERT(aItem);
    
    // get the index buffer of the entry
    pEntryBuffer = EntryBuffer::pRefdEntryBuffer(aItem);
    ASSERT(pEntryBuffer);
    ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
  }
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* position at key as per relational operator                                 */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aPositionQuery(KeyData* aGivenKey, eRO eRelnOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
  
  //if (m_aLazyChain)
  //  vBuildSkipList();
#endif
  
  bool bIsEqual;
  Item* aIndexEntry = aPositionQuery(aGivenKey, 1, JC_AFTER);
  
  bool bAfter;
  if (aIndexEntry) {
    do {
      bool bIsEqual = false;
      bAfter = _aKeyData(aIndexEntry->aDataArea())
                    ->bJuxtaCompare(JC_AFTER, aGivenKey,
                                    m_pIndex->pIdxColVec(), 1,
                                    bIsEqual, eRelnOp);
      if (! bAfter) {
        aIndexEntry = aNextEntry(aIndexEntry, iLevelNo());
      }
    } while (aIndexEntry && ! bAfter);
  }
  
  return aIndexEntry;
}

/*----------------------------------------------------------------------------*/
/* position at given key                                                      */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aPositionQuery(KeyData* aGivenKey,i8 nKeyCols, eJC eJuxtaCompare)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
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
  
  Item* aScanEntry_sl = aSkipRoots();
  bool bIsEqual;
  
  for (i8 i_ScanOffset = 2; i_ScanOffset >= 0; i_ScanOffset--) {
    
    Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanOffset);
    while (aNextEntry_sl) {
      
      KeyData* aNextKey = _aKeyData(aNextEntry_sl->aDataArea());
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aGivenKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
      bIsJuxta = aGivenKey->bJuxtaCompare(JC_AFTER,aNextKey,pIdxColVec,
                                          nKeyCols,bIsEqual);
      
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
  
  if (aScanEntry_sl == aSkipRoots()) {
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
  
  RASSERT(nullptr == m_i4aItemOffsets);
  RASSERT(! m_pIndex->bIsHidden());
  RASSERT(1 == m_iLevelNo);
  
  vResetUsed();
  
  m_xSkipRoots.m_iaNextOffset[0] = -1;
  m_xSkipRoots.m_iaNextOffset[1] = -1;
  m_xSkipRoots.m_iaNextOffset[2] = -1;

  m_i_FreeOffsetChain = C_NO_MORE_ITEMS;
  
  //m_aHeadEntry  = nullptr;
  m_aLastEntry  = nullptr;
  m_aFreeItemChain  = nullptr;
  
  m_aLazyChain  = nullptr;
  
  //vHeadEntryNew();
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
  //  nb: the given address has to be an address in the orginal buffer
  PoolBuffer* pOriginalBuffer =
    pBitAnd<PoolBuffer*>(m_i4aItemOffsets,C_OBJECT_TO_BUFFER_MASK);
  
  
  
  // get this buffer address
  PoolBuffer* pThisBuffer =
    pBitAnd<PoolBuffer*>(this,C_OBJECT_TO_BUFFER_MASK);
  
  // calculate offset for m_i4aItemOffsets and set it
  if (m_i4aItemOffsets) {
    PoolBuffer* pOriginalBuffer =
      pBitAnd<PoolBuffer*>(m_i4aItemOffsets,C_OBJECT_TO_BUFFER_MASK);
    u8 uOffset = nSub(m_i4aItemOffsets, pOriginalBuffer);
    m_i4aItemOffsets = aBitOr<i4*>(pThisBuffer,uOffset);
  }
  
  // calculate offset for m_aHeadEntry and set it
  //uOffset = nSub(m_aHeadEntry, pOriginalBuffer);
  //m_aHeadEntry = pBitOr<Item*>(pThisBuffer,uOffset);
  
  // calculate offset for m_aLastEntry and set it
  if (m_aLastEntry) {
    PoolBuffer* pOriginalBuffer =
      pBitAnd<PoolBuffer*>(m_aLastEntry,C_OBJECT_TO_BUFFER_MASK);
    u8 uOffset = nSub(m_aLastEntry, pOriginalBuffer);
    m_aLastEntry = pBitOr<Item*>(pThisBuffer,uOffset);
  }
  
  // calculate offset for m_aFreeItemChain and set it
  if (m_aFreeItemChain) {
    PoolBuffer* pOriginalBuffer =
      pBitAnd<PoolBuffer*>(m_aFreeItemChain,C_OBJECT_TO_BUFFER_MASK);
    u8 uOffset = nSub(m_aFreeItemChain, pOriginalBuffer);
    m_aFreeItemChain = pBitOr<FreeItem*>(pThisBuffer,uOffset);
  }
  
  // calculate offset for m_aLazyChain and set it
  if (m_aLazyChain) {
    PoolBuffer* pOriginalBuffer =
      pBitAnd<PoolBuffer*>(m_aLazyChain,C_OBJECT_TO_BUFFER_MASK);
    u8 uOffset = nSub(m_aLazyChain, pOriginalBuffer);
    m_aLazyChain = pBitOr<Item*>(pThisBuffer,uOffset);
  }
  
  // get the buffer no
  bn nBuffer = nPoolBufferNo();
  
  // restore the bufferNo
  m_iBufferNo = *nBuffer;
  
  // now restore the row id's to point to this buffer no
  for (Item* aItem = aLeadEntry();
       aItem != nullptr;
       aItem = aNextEntry(aItem, 0)) {
    
    Id xId(*nBuffer, aItem->iItemNo(), aItem->nOffsets());
    aItem->vId(xId);
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
void EntryBuffer::vShow(Item* aMarkEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  s256 sHigherBuffer;
  if (m_pHigherBuffer)
    StrLib::wToString((i8)((EntryBuffer*)m_pHigherBuffer)->m_iLevelNo,
                      sHigherBuffer.aChar());
  else
    sHigherBuffer = "..."_Stz;
  
  vTrace("\n"_Stz);
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  vTrace("EntryBuffer: %p (buffer %lld): items: %lld used: %lld freespace %lld\n"_Stz,
         this,*nPoolBufferNo(),nItems(),nUsedBytes(),nFreeBytes());
  
  vTrace("  higherBuffer %p (L%s) prevBuffer %p nextBuffer %p\n"_Stz,
         m_pHigherBuffer,sHigherBuffer.aStz(),m_pPrevBuffer,m_pNextBuffer);
  
  vTrace("  index: %s partition: %lld level: %lld\n"_Stz,
         m_pIndex ? m_pIndex->aIndexName() : "NULL"_Stz,m_iPartitionNo,m_iLevelNo);
  
  vTrace("  entryOffset: %p freeOffsetHead: %lld\n"_Stz,
         m_i4aItemOffsets,m_i_FreeOffsetChain);
  
  vTrace("  headEntry: %p[%d,%d,%d] lastEntry: %p firstFree: %p\n"_Stz,
         aSkipRoots(),aSkipRoots() ? aSkipRoots()->iNEO(0) : 0,
         aSkipRoots() ? aSkipRoots()->iNEO(1) : 0,
         aSkipRoots() ? aSkipRoots()->iNEO(2) : 0,
         m_aLastEntry,m_aFreeItemChain);
  
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  ASSERT(aSkipRoots());
  
  vTrace("\n"_Stz);
  vTrace("Entries (by headEntry vector[0]):\n"_Stz);
  
  i8 iOffset = aSkipRoots()->iNEO(0);
  while (iOffset > -1) {
    Item* aItem = aGetEntryOffset(iOffset);
    vShowEntry(aItem);
    if (aMarkEntry == aItem)
      vTrace("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
             "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n"_Stz);
    
    iOffset = aItem->iNEO(0);
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
         *nPoolBufferNo(),
         pHigherBuffer() ? *pHigherBuffer()->nPoolBufferNo() : 0);
  
  i8 iOffset = aSkipRoots()->iNEO(0);
  while (iOffset > -1) {
    Item* aItem = aGetEntryOffset(iOffset);
    
    KeyData* aKey = _aKeyData(aItem->aDataArea())
    ;
    i8 iVal = aKey->iGetInt();
    
    if (m_iLevelNo)
      vTrace("%lld(%lld)"_Stz,iVal,*EntryBuffer::pRefdEntryBuffer(aItem)->nPoolBufferNo());
    else
      vTrace("%lld|%lld|"_Stz,iVal,*EntryBuffer::pRefdEntryBuffer(aItem)->nPoolBufferNo());
    
    iOffset = aItem->iNEO(0);
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
    
    vTrace("%lld"_Stz,pL0->iPoolBufferNo());
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
    vTrace("\nIndex Tree:\n"_Stz,m_iLevelNo);
  
  vTrace("L%lld"_Stz,m_iLevelNo);
  
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
  
  if ( ! m_iLevelNo )
    return;
  
  Item* aFirstEntry = this->aLeadEntry();
  if (! aFirstEntry)
    return;
  
  Id xId;
  xId = aFirstEntry->xId();
  bn nBuffer = xId.iBuffer();
  
  HeapBuffer* pHeapBuffer =
  reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(nBuffer));
  
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
/* unlink the sublist of EB's following this L0 EB within this L1             */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pUnlinkSubList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(0 == m_iLevelNo);
  
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
  
  if (m_iLevelNo) {
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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* build skip list from the lazy chain                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vBuildSkipList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (! m_aLazyChain )
    return;
  
  Item* aOriginalFirstEntry = aNextEntry(aSkipRoots(),0);
  
  static i8 nBuild = 0;
  //vTrace("vBuildSkipList %lld (%lld items)\n"_Stz,++nBuild,nItems());
  
  Item* aLazyChain = m_aLazyChain;
  m_aLazyChain = nullptr;
  
  while (aLazyChain) {
    
    Item* aKeyEntry = aLazyChain;
    
    i8 iNextOffset = aKeyEntry->iNEO(0);
    if (-1 == iNextOffset)
      aLazyChain = nullptr;
    else
      aLazyChain = aGetEntryOffset(iNextOffset);
    
    vInsertKeySkip(aKeyEntry);
  }
  
  Item* aInsertedKeyEntry = aNextEntry(aSkipRoots(),0);
  
  vHandleFirstInsert(aInsertedKeyEntry,aOriginalFirstEntry,nullptr);
}

/*----------------------------------------------------------------------------*/
/* compact this buffer                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vCompact(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  //RASSERT( ! m_aLazyChain );
  
  if (m_aLazyChain)
    vBuildSkipList();
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  FreeItem* aFreeEntry;
  Item*     aMoveEntry;
  Item*     xPriorVec[gpAttachMap->m_nIndexOffsets];
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  // we can only compact if there are free entries
  if (! m_aFreeItemChain)
    return;
  
  // loose the free list so vShow() works correctly
  m_aFreeItemChain = nullptr;
  
  // scan item is the working pointer
  Item* aScanItem = aFirstItem();
  //ASSERT(aScanItem->eClass() == CN_ENTRY);
  
  // loop while we can still reclaim more space
  for (;;) {
    
    if (m_iLevelNo) {
      //vShow();
      int iDebug = 1;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // find the next free entry in buffer
    
    aFreeEntry = nullptr;
    for (;
         aScanItem != nullptr;
         aScanItem = aAdjacentItem(aScanItem)) {
      
      eCN eClass = aScanItem->eClass();
      
      if (CN_FREEENTRY == eClass) {
        aFreeEntry = _aFreeItem(aScanItem);
        break;
      }
    }
    
    // return if no more free entries
    if (! aFreeEntry)
      break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // find the next entry in buffer after the free entry
    
    aMoveEntry = nullptr;
    for (Item* aItem = aAdjacentItem(aFreeEntry);
         aItem != nullptr;
         aItem = aAdjacentItem(aItem)) {
      
      eCN eClass = aItem->eClass();
      
      RASSERT ( (eClass == CN_ENTRY) ||
               (eClass == CN_FREEENTRY) ||
               (eClass == CN_WILDERNESS));
      
      
      if (CN_ENTRY == eClass) {
        aMoveEntry = static_cast<Item*>(aItem);
        break;
      }
    }
    
    // return if no more entries after the free entry
    if (! aMoveEntry)
      break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the prior vector to the entry being moved
    
    //KeyData* aMoveKey = _aKeyData(aMoveEntry->aDataArea());
    
    //aKey->vShow(pIdxColVec);
    //vTrace("\n"_Stz);
    
    vPositionPriorVecEntry(aMoveEntry, xPriorVec, aMoveEntry);
    
    if (m_iLevelNo) {
      //vDumpPriorVec(xPriorVec);
      int iDebug = 1;
    }
    
    i4 iWasOffset = iToOffset(aMoveEntry);
    i4 iNowOffset = iToOffset(aFreeEntry);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // now copy move entry left onto free entry
    
    // calculate the number of bytes to copy
    i8 nCopyBytes = aMoveEntry->nSizeItem_WithoutOffsets()
                      + aMoveEntry->iNEOsize();
    
    // copy the move entry over the free entry
    MemLib::vMove(aFreeEntry, aMoveEntry, nCopyBytes, CN_ENTRYBUFFER);
    
    // give the reclaimed space back to the buffer
    i8 nReclaimedBytes = nSub(aMoveEntry,aFreeEntry);
    //vGiveSpace(nReclaimedBytes);
    
    // convert the moved entry to a free entry
    FreeItem* aOpenedFreeEntry = aAdd<FreeItem*>(aFreeEntry,nCopyBytes);
    new(aOpenedFreeEntry) FreeItem(nReclaimedBytes);
    
    Item* aCheckEntry = reinterpret_cast<Item*>(aFreeEntry);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the prior vector to the entry being moved
    
    //vDumpPriorVec(xPriorVec);
    for (i8 i_Offset = 0; i_Offset < 3; i_Offset++) {
      Item* aPriorEntry = xPriorVec[i_Offset];
      i4 iPriorOffset = aPriorEntry->iNEO(i_Offset);
      if (iWasOffset == iPriorOffset)
        aPriorEntry->vNEO(i_Offset,iNowOffset);
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move scan
    
    aScanItem = aFreeEntry;
    
  } // for
  
  ASSERT( ! aScanItem || CN_FREEENTRY == aScanItem->eClass() );
  
  if (aScanItem)
    vSetUsed(aScanItem);
  
  //vShow();
}

/*----------------------------------------------------------------------------*/
/* compact this buffer                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vCompactLeft2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT(! m_aLazyChain);
#endif
  
  EntryBuffer* pThisBuffer = this;
  EntryBuffer* pTempBuffer = new EntryBuffer();
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  KeyWrk xKeyWrk;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Item* aPriorEntry = pTempBuffer->aSkipRoots();
  
  for (Item* aThisEntry = pThisBuffer->aLeadEntry();
       aThisEntry != nullptr;
       aThisEntry = pThisBuffer->aNextEntry(aThisEntry, 0)) {
    
    Id xId = aThisEntry->xId();
    KeyData* aKey = _aKeyData(aThisEntry->aDataArea());
    
    i8 nKeySize = aKey->nKeySize(pIdxColVec);
    i8 nOffsets = aThisEntry->nOffsets();
    
    xKeyWrk.vId(xId);
    xKeyWrk.vEmbedded((char*)aKey);
    xKeyWrk.vKeySize(nKeySize);
    xKeyWrk.vOffsets(nOffsets);
    
    pTempBuffer->aInsertKeyAbsL0(xKeyWrk,&aPriorEntry);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Item* aThisWilderness = aItemArea();
//    = aAdd<Item*>(aSkipRoots(),
//                  aSkipRoots()->nSizeItem_WithoutOffsets() +
//                    aSkipRoots()->iNEOsize());
  
  vSetUsed(aThisWilderness);
  
  vItems(0);
  
  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexOffsets; i_Offset++)
    aSkipRoots()->vNEO(i_Offset,-1);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  aPriorEntry = pThisBuffer->aSkipRoots();
  
  for (Item* aTempEntry = pTempBuffer->aLeadEntry();
       aTempEntry != nullptr;
       aTempEntry = pTempBuffer->aNextEntry(aTempEntry, 0)) {
    
    Id xId = aTempEntry->xId();
    KeyData* aKey = _aKeyData(aTempEntry->aDataArea());
    i8 nKeySize = aKey->nKeySize(pIdxColVec);
    i8 nOffsets = aTempEntry->nOffsets();
    
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
void EntryBuffer::vCutEntry(Item* aItem, Item* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nOffsets = aItem->nOffsets();
  
  for (i8 i_Offset = 0; i_Offset < nOffsets; i_Offset++) {
    i4 iNEO = aItem->iNEO(i_Offset);
    aPriorVec[i_Offset]->vNEO(i_Offset,iNEO);
  }
}

/*----------------------------------------------------------------------------*/
/* delete upwards                                                             */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDeleteUpwards(KeyData* aDeleteKey,
                                 EntryBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef USE_EB_WATCH
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
  
  Item* aPriorEntry = pPositionPriorEntry(aDeleteKey);
  if (!aPriorEntry)
    return;
  
  Item* aEntryToBeDeleted = aNextEntry(aPriorEntry,0);
  if (!aEntryToBeDeleted)
    return;
  
  //  vTrace("[%lld] pDeleteEntry key %lld\n"_Stz,
  //         *nPoolBufferNo(),
  //         pEntryToBeDeleted->aKey()->iGetInt());
  
  KeyData* aDeleteEntryKey = _aKeyData(aEntryToBeDeleted->aDataArea());
  
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
  
  Item* aFirstEntry = this->aLeadEntry();
  if (aEntryToBeDeleted == aFirstEntry && m_pHigherBuffer) {
    EntryBuffer* pHigherBuffer = _pEntryBuffer(m_pHigherBuffer);
    pHigherBuffer->vDeleteUpwards(aDeleteKey,ppRootKeyBuffer);
  }

  vDeleteEntry(&aEntryToBeDeleted);
}

/*----------------------------------------------------------------------------*/
/* delete upwards                                                             */
/*----------------------------------------------------------------------------*/
//void EntryBuffer::vDeleteUpwards(KeyData* aDeleteKey, EntryBuffer* pToBuffer,
//                                 EntryBuffer** ppRootKeyBuffer)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
//
//#ifdef USE_EB_LAZY_CHAIN
//  RASSERT( ! m_aLazyChain );
//#endif
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  Item* xPriorVec[gpAttachMap->m_nIndexOffsets];
//
//  vPositionPriorVecBuffer(aDeleteKey,xPriorVec,pToBuffer);
//
//  Item* aPriorEntry = xPriorVec[0];
//
//  Item* aEntryToBeDeleted = aNextEntry(aPriorEntry,0);
//  if (! aEntryToBeDeleted)
//    return;
//
//  //  vTrace("[%lld] pDeleteEntry key %lld\n"_Stz,
//  //         *nPoolBufferNo(),
//  //         pEntryToBeDeleted->aKey()->iGetInt());
//
//  KeyData* aDeleteEntryKey = _aKeyData(aEntryToBeDeleted->aDataArea());
//  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
//  //if (! aDeleteEntryKey->bEqualTo(aDeleteKey,pIdxColVec))
//  //  return;
//  RASSERT(aDeleteEntryKey->bEqualTo(aDeleteKey,pIdxColVec));
//  RASSERT(pToBuffer == EntryBuffer::pRefdEntryBuffer(aEntryToBeDeleted));
//
//  Item* aFirstEntry = this->aLeadEntry();
//  if (aEntryToBeDeleted == aFirstEntry && m_pHigherBuffer) {
//    EntryBuffer* pHigherBuffer = _pEntryBuffer(m_pHigherBuffer);
//    pHigherBuffer->vDeleteUpwards(aDeleteKey,this,ppRootKeyBuffer);
//  }
//
//  vDeleteEntry(&aEntryToBeDeleted);
//}

/*----------------------------------------------------------------------------*/
/* dump the given prior vector                                                */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDumpPriorVec(Item* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  vTrace("Prior Vector\n"_Stz);
  
  vTrace("  0:%p 1:%p 2:%p\n"_Stz,
         aPriorVec[0],aPriorVec[1],aPriorVec[2]);
  
  for (i8 i = 0; i < 3; i++) {
    Item* aItem = aPriorVec[i];
    for (i8 i_Offset = 0; i_Offset < aItem->nOffsets(); i_Offset++) {
      vTrace("  i : %lld Item : %p i_Offset : %lld offset : %lld pointsTo : %p\n"_Stz,
             i,
             aItem,
             i_Offset,
             aItem->iNEO(i_Offset),
             aGetEntryOffset(aItem->iNEO(i_Offset)));
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get an entry by offset                                                     */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aGetEntryOffset(i8 iOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Item* aItem = aAdd<Item*>(aItemArea(), iOffset);
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* get split entry by split number                                            */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aGetEntrySplit(i8 iSplitEntryNo,Item** aaPriorEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(aSkipRoots());
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  *aaPriorEntry = nullptr;
  
  i8 iEntryNo = 0;
  for (Item* aSplitEntry = aNextEntry(aSkipRoots(),0);
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
/* handle the insert of a first key                                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vHandleFirstInsert(Item* aInsertedKeyEntry,
                                     Item* aOriginalFirstEntry,
                                     EntryBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (aLeadEntry() != aInsertedKeyEntry)
    return;
  
  if (aOriginalFirstEntry) {
    KeyData* aOriginalFirstKey = _aKeyData(aOriginalFirstEntry->aDataArea());
    
    KeyData* aInsertedFirstKey = _aKeyData(aInsertedKeyEntry->aDataArea());
    
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
void EntryBuffer::vHandleLeftInsert(Item* aInsertedKeyEntry,
                                    Item* aOriginalFirstEntry,
                                    EntryBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (! m_pHigherBuffer)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef USE_EB_WATCH
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
    KeyData* aDeleteKey = _aKeyData(aOriginalFirstEntry->aDataArea());
    EntryBuffer* pHigherBuffer = _pEntryBuffer(m_pHigherBuffer);
    pHigherBuffer->vDeleteUpwards(aDeleteKey,ppRootKeyBuffer);
    //(*ppRootKeyBuffer)->vShowTree();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  KeyData* aInsertKey = _aKeyData(aInsertedKeyEntry->aDataArea());
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  i8 iInsertKeyLen  = aInsertKey->nKeySize(pIdxColVec);
  i8 nInsertOffsets = aInsertedKeyEntry->nOffsets();
  
  Id xInsertId(*nPoolBufferNo(),0,nInsertOffsets);
  
  KeyWrk xKeyWrkIns;
  xKeyWrkIns.vId(xInsertId);
  xKeyWrkIns.vEmbedded((char*)aInsertKey);
  xKeyWrkIns.vKeySize(iInsertKeyLen);
  xKeyWrkIns.vOffsets(nInsertOffsets);
  
  EntryBuffer* pHigherBuffer = _pEntryBuffer(m_pHigherBuffer);
  pHigherBuffer->aInsertKeyBuffer(xKeyWrkIns,ppRootKeyBuffer);
}

/*----------------------------------------------------------------------------*/
/* create a head entry                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vHeadEntryNew(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT(0);
  
//  RowWrk xHeadWrk;
//
//  ASSERT(! m_aHeadEntry);
//
//  // the head entry always has 3 offsets
//  xHeadWrk.vOffsets(3);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // compute required entry space
//
//  i8 nEntryBytes = xHeadWrk.nEntrySize();
//  i8 nOffsetBytes = xHeadWrk.nOffsetSize();
//  i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;
//
//  ASSERT(bHasSpace(nEntryPlusOffsets));
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  xHeadWrk.xId().vBuffer(*nPoolBufferNo());
//
//  //m_aHeadEntry = reinterpret_cast<Item*>(aAllotItem(CN_ENTRY,nEntryBytes));
//  m_aHeadEntry = reinterpret_cast<Item*>(aAllotSpace(nEntryBytes));
//  new(m_aHeadEntry) Item(CN_ENTRY,nEntryBytes);
//
// // m_aHeadEntry->vSetClass(CN_ENTRY);
//
//  ASSERT(m_aHeadEntry);
//
//  m_aHeadEntry->vId(xHeadWrk.xId());
//
//  NEO* aNEO = aAllotNEO(nOffsetBytes);
//  m_aHeadEntry->vNEO(0,-1);
//  m_aHeadEntry->vNEO(1,-1);
//  m_aHeadEntry->vNEO(2,-1);
}

/*----------------------------------------------------------------------------*/
/* restore the head entry                                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vHeadEntryRestore(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT(0);
  
  //m_aHeadEntry = aAdd<Item*>(aDataArea(),sizeof(i4) * C_MAX_ITEMS);
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
  
  //KeyData* aInsKey = xKeyWrk.aKey();
  
  EntryBuffer* pInsertIntoBuffer;
  if (bHasSpace(nEntryPlusOffsets))
    pInsertIntoBuffer = this;
  else {
    pInsertIntoBuffer =
    pNewInsertBuffer(xKeyWrk,nEntryPlusOffsets,ppRootKeyBuffer);
    //(*ppRootKeyBuffer)->vShow();
  }
  
  return pInsertIntoBuffer;
}

/*----------------------------------------------------------------------------*/
/* insert a key entry specifically into this buffer (should be enough space)  */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aInsertKeyAbsLazy(KeyWrk& xKeyWrk)
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
  
  Item* aKeyEntry = aAllotEntry(nEntryBytes);
  ASSERT(aKeyEntry);
  
  NEO* aNEO = aAllotNEO(nOffsetBytes);
  ASSERT(aNEO);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the key entry
  
  xKeyWrk.vSetEntry(aKeyEntry);
  
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
void EntryBuffer::vInsertKeySkip(Item* aKeyEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the entry after which the new entry will be inserted
  
  Item* xPriorVec[gpAttachMap->m_nIndexOffsets];
  
  //KeyData* aKey = _aKeyData(aKeyEntry->aDataArea());
  
  if (0 == m_iLevelNo)
    vPositionPriorVecEntry(aKeyEntry, xPriorVec, nullptr);
  else {
    EntryBuffer* pToBuffer = EntryBuffer::pRefdEntryBuffer(aKeyEntry);
    vPositionPriorVecBuffer(aKeyEntry, xPriorVec, pToBuffer);
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
 Item* EntryBuffer::pInsertKeyBuffer(KeyWrk& xKeyWrk,
 EntryBuffer** ppRootKeyBuffer)
 {
 _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
 
 // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 #ifdef USE_EB_WATCH
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
 
 Item* aOriginalFirstEntry = aFirstEntry();
 if (!pOriginalFirstEntry)
 pOriginalFirstEntry = aFirstEntry();
 
 // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 // insert the key
 
 Item* aInsertedKeyEntry = pInsertKeyAbs(xKeyWrk);
 
 // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 // handle the insert of a left most key
 
 #ifdef USE_EB_LAZY_CHAIN
 if (m_iLevelNo)
 vHandleFirstInsert(pInsertedKeyEntry,pOriginalFirstEntry,ppRootKeyBuffer);
 #else
 vHandleFirstInsert(pInsertedKeyEntry,pOriginalFirstEntry,ppRootKeyBuffer);
 #endif
 
 return pInsertedKeyEntry;
 }
 */

/*----------------------------------------------------------------------------*/

Item* EntryBuffer::aInsertKeyBuffer(KeyWrk& xKeyWrk,
                                     EntryBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef USE_EB_WATCH
  if (mbWatch) {
    EntryBuffer* pRootKeyBuffer = *ppRootKeyBuffer;
    IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
    vTrace("\n[%lld] pInsertKeyBuffer: "_Stz,m_iBufferNo);
    xKeyWrk.vShow(pIdxColVec);
    pRootKeyBuffer->vShowTree();
    int iDebug = 1;
  }
#endif
  
  Item* aInsertedKeyEntry;
  
#ifdef USE_EB_LAZY_CHAIN
  if (m_pIndex->pTable()->bIsLoading()) {
    if (m_iLevelNo) {
      Item* aOriginalFirstEntry = aLeadEntry();
      aInsertedKeyEntry = aInsertKeyAbs(xKeyWrk);
      vHandleFirstInsert(aInsertedKeyEntry,aOriginalFirstEntry,ppRootKeyBuffer);
    }
    else {
      aInsertedKeyEntry = aInsertKeyAbs(xKeyWrk);
    }
  }
  else {
    Item* aOriginalFirstEntry = aLeadEntry();
    aInsertedKeyEntry = aInsertKeyAbs(xKeyWrk);
    vHandleFirstInsert(aInsertedKeyEntry,aOriginalFirstEntry,ppRootKeyBuffer);
  }
#else
  Item* aOriginalFirstEntry = aFirstEntry();
  pInsertedKeyEntry = pInsertKeyAbs(xKeyWrk);
  vHandleFirstInsert(pInsertedKeyEntry,pOriginalFirstEntry,ppRootKeyBuffer);
#endif
  
  return aInsertedKeyEntry;
}

/*----------------------------------------------------------------------------*/
/* insert a new key entry into this key tree (handling ripple upwards)        */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aInsertKeyUpwards(KeyWrk& xKeyWrk,
                                      EntryBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef USE_EB_WATCH
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
  
  Item* aInsertedKeyEntry =
  pInsertIntoBuffer->aInsertKeyBuffer(xKeyWrk,ppRootKeyBuffer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pInsertIntoBuffer->m_iLevelNo) {
    Id xId = aInsertedKeyEntry->xId();
    i8 nBuffer = xId.iBuffer();
    
    HeapBuffer* pHeapBuffer =
    reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(nBuffer));
    
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
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem, 0))
    m_nItems++;
}

/*----------------------------------------------------------------------------*/
/* is given key before the first key of this buffer                           */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bKeyBeforeBuffer(KeyWrk& xKeyTest)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Item* aFirstEntry = this->aLeadEntry();
  //KeyData* aFirstKey = _aKeyData(aFirstEntry->aDataArea());

  KeyData* aKeyTest = xKeyTest.aKey();

//  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
//  bool bIsJuxta =
//  aKeyTest->bLessThan(aFirstKey,pIdxColVec);
  
  bool bIsJuxta = KeyData::bLessThan(xKeyTest,aFirstEntry);
  
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
    
    RASSERT(pEntryBuffer->iLevelNo() == 0);
    
    //vTrace("Building skip list %p\n"_Stz, pEntryBuffer);
    
    pEntryBuffer->vBuildSkipList();
    
    pEntryBuffer = pEntryBuffer->pNextBuffer();
  }
}

/*----------------------------------------------------------------------------*/
/* convert given entry into free entry                                        */
/*----------------------------------------------------------------------------*/
FreeItem* EntryBuffer::aMakeFreeEntry(Item* aItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nSizeItem = aItem->nSizeItem_WithoutOffsets();
  i8 nSizeNEO  = aItem->iNEOsize();
  i8 nSizeFree = nSizeItem + nSizeNEO;
  
  FreeItem* aFreeItem = new(aItem) FreeItem(nSizeFree);
  
  return aFreeItem;
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
  pNewBuffer->vLevelNo(m_iLevelNo+1);
  pNewBuffer->vPartitionNo(m_iPartitionNo);
  
  vHigherBuffer(pNewBuffer);
  
  return pNewBuffer;
}

/*----------------------------------------------------------------------------*/
/* create a new insert buffer (adjusting higher buffers accordingly)          */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pNewInsertBuffer(KeyWrk& xInsKey, i8 nEntryPlusOffsets,
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
  
  EntryBuffer* pRightBuffer = pLeftBuffer->pSplit2();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the first key of left buffer into higher buffer
  
  if ( ! pHigherBuffer->nItems() ) {
    Item* aFirstKeyLeft  = pLeftBuffer->aLeadEntry();
    
    i8 nOffsetsLeft = aFirstKeyLeft->nOffsets();
    Id xIdLeft(*pLeftBuffer->nPoolBufferNo(),0,nOffsetsLeft);
    KeyData* aKeyLeft = _aKeyData(aFirstKeyLeft->aDataArea());
    i8 iKeyLenLeft = aKeyLeft->nKeySize(m_pIndex->pIdxColVec());
    
    
    KeyWrk xKeyWrkLeft;
    xKeyWrkLeft.vId(xIdLeft);
    xKeyWrkLeft.vEmbedded((char*)aKeyLeft);
    xKeyWrkLeft.vKeySize(iKeyLenLeft);
    xKeyWrkLeft.vOffsets(nOffsetsLeft);
    
    
    Item* aNewKeyLeft =
    pHigherBuffer->aInsertKeyUpwards(xKeyWrkLeft,ppRootKeyBuffer);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the first key of right buffer into higher buffer
  
  Item* aFirstKeyRight = pRightBuffer->aLeadEntry();
  
  KeyData* aKeyRight = _aKeyData(aFirstKeyRight->aDataArea());
  
  i8 iKeyLenRight = aKeyRight->nKeySize(m_pIndex->pIdxColVec());
  i8 nOffsetsRight = aFirstKeyRight->nOffsets();
  
  Id xIdRight(*pRightBuffer->nPoolBufferNo(),0,nOffsetsRight);
  
  KeyWrk xKeyWrkRight;
  xKeyWrkRight.vId(xIdRight);
  xKeyWrkRight.vEmbedded((char*)aKeyRight);
  xKeyWrkRight.vKeySize(iKeyLenRight);
  xKeyWrkRight.vOffsets(nOffsetsRight);
  
  
  Item* aNewKeyRight =
  pHigherBuffer->aInsertKeyUpwards(xKeyWrkRight,ppRootKeyBuffer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //ASSERT( (* ppRootKeyBuffer)->bVerify() );
  
  if ( pRightBuffer->bKeyBeforeBuffer(xInsKey) )
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
  pNewBuffer->vLevelNo(m_iLevelNo);
  pNewBuffer->vPartitionNo(m_iPartitionNo);
  
  pNewBuffer->vHigherBuffer(_pEntryBuffer(m_pHigherBuffer));
  pNewBuffer->vPrevBuffer(this);
  pNewBuffer->vNextBuffer(_pEntryBuffer(m_pNextBuffer));
  if (m_pNextBuffer) {
    EntryBuffer* pNextBuffer = _pEntryBuffer(m_pNextBuffer);
    pNextBuffer->vPrevBuffer(pNewBuffer);
  }
  
  m_pNextBuffer = pNewBuffer;
  
  ASSERT(this == pNewBuffer->pPrevBuffer());
  
  return pNewBuffer;
}

/*----------------------------------------------------------------------------*/
/* pop entry from lazy chain                                                  */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aPopLazyChain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (! m_aLazyChain )
    return nullptr;
  
  Item* aPoppedLazyEntry = m_aLazyChain;
  
  i8 iNextLazyOffset = aPoppedLazyEntry->iNEO(0);
  
  if (-1 == iNextLazyOffset)
    m_aLazyChain = nullptr;
  else
    m_aLazyChain = aGetEntryOffset(iNextLazyOffset);
  
  return aPoppedLazyEntry;
}

///*----------------------------------------------------------------------------*/
///* position at the L0 buffer into which key should be inserted                */
///*----------------------------------------------------------------------------*/
//EntryBuffer* EntryBuffer::pPositionPriorBuffer(KeyData* aFindKey)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
//  
//  if ( ! nItems() ) {
//    ASSERT( 0 == iLevelNo());
//    return this;
//  }
//  
//  EntryBuffer* pEntryBuffer = this;
//  while (true) {
//    
//    // have we arrived at L0 ?
//    i8 iBufferLevel = pEntryBuffer->iLevelNo();
//    if (iBufferLevel == 0) {
//      break;
//    }
//    else {
//#ifdef USE_EB_LAZY_CHAIN
//      RASSERT( ! m_aLazyChain );
//#endif
//    }
//    
//    // get the entry before the given insert key
//    Item* aEntryPrior = pEntryBuffer->pPositionPriorEntry(aFindKey);
//    ASSERT(aEntryPrior);
//    if (aEntryPrior == pEntryBuffer->aSkipRoots())
//      aEntryPrior = pEntryBuffer->aNextEntry(aEntryPrior,0);
//    ASSERT(aEntryPrior);
//    
//    // get the index buffer of the entry
//    //pEntryBuffer->vShow();
//    ASSERT(pEntryBuffer != EntryBuffer::pRefdEntryBuffer(aEntryPrior));
//    pEntryBuffer = EntryBuffer::pRefdEntryBuffer(aEntryPrior);
//    ASSERT(pEntryBuffer);
//    pEntryBuffer->vAssertEntryBuffer();
//  }
//  
//  return pEntryBuffer;
//}

/*----------------------------------------------------------------------------*/
/* position at the L0 buffer into which key should be inserted                */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pPositionPriorBuffer(KeyWrk& xFindKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if ( ! nItems() ) {
    ASSERT( 0 == iLevelNo());
    return this;
  }
  
  EntryBuffer* pEntryBuffer = this;
  while (true) {
    
    // have we arrived at L0 ?
    i8 iBufferLevel = pEntryBuffer->iLevelNo();
    if (iBufferLevel == 0) {
      break;
    }
    else {
#ifdef USE_EB_LAZY_CHAIN
      RASSERT( ! m_aLazyChain );
#endif
    }
    
    // get the entry before the given insert key
    Item* aEntryPrior = pEntryBuffer->pPositionPriorEntry(xFindKey);
    ASSERT(aEntryPrior);
    if (aEntryPrior == pEntryBuffer->aSkipRoots())
      aEntryPrior = pEntryBuffer->aNextEntry(aEntryPrior,0);
    ASSERT(aEntryPrior);
    
    // get the index buffer of the entry
    //pEntryBuffer->vShow();
    ASSERT(pEntryBuffer != EntryBuffer::pRefdEntryBuffer(aEntryPrior));
    pEntryBuffer = EntryBuffer::pRefdEntryBuffer(aEntryPrior);
    ASSERT(pEntryBuffer);
    pEntryBuffer->vAssertEntryBuffer();
  }
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* scan thru skip list and return entry prior to key                          */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::pPositionPriorEntry(KeyData* aFindKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (nItems() == 0)
    return aSkipRoots();
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  i8 nKeyCols = pIdxColVec->nItems();
  
  Item* aScanEntry_sl = aSkipRoots();
  
  bool bKeyIsGreaterEqual;
  
  for (i8 i_ScanOffset = gpAttachMap->m_nIndexOffsets - 1;
       i_ScanOffset >= 0;
       i_ScanOffset--) {
    
    Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanOffset);
    while (aNextEntry_sl) {
      
      KeyData* aNextKey = _aKeyData(aNextEntry_sl->aDataArea());
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
#ifdef USE_EB_SINGLE_COLIDX
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
/* scan thru skip list and return entry prior to key                          */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::pPositionPriorEntry(KeyWrk& xFindKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (nItems() == 0)
    return aSkipRoots();
  
  //IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  //i8 nKeyCols = pIdxColVec->nItems();
  
  Item* aScanEntry_sl = aSkipRoots();
  
  bool bKeyIsGreaterEqual;
  
  for (i8 i_ScanOffset = gpAttachMap->m_nIndexOffsets - 1;
       i_ScanOffset >= 0;
       i_ScanOffset--) {
    
    Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanOffset);
    while (aNextEntry_sl) {
      
      KeyData* aNextKey = _aKeyData(aNextEntry_sl->aDataArea());
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
      //#ifdef USE_EB_SINGLE_COLIDX
      //      if (m_pIndex->bSingleColumn()) {
      //        bKeyIsGreaterEqual = aNextKey->bGreaterEqualSingle(aFindKey,m_pIndex);
      //        //RASSERT(bKeyIsGreaterEqual == aNextKey->bGreaterEqual(aFindKey,pIdxColVec));
      //      }
      //      else
      //        bKeyIsGreaterEqual = aNextKey->bGreaterEqual(aFindKey,pIdxColVec);
      //#else
      //      bKeyIsGreaterEqual = aNextKey->bGreaterEqual(aFindKey,pIdxColVec);
      //#endif
      
      bKeyIsGreaterEqual = KeyData::bGreaterEqual(aNextEntry_sl, xFindKey);
      
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
void EntryBuffer::vPositionPriorVecEntry(KeyData* aFindKey, Item* aPriorVec[],
                                         Item* aFindEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //ASSERT(0 == m_iLevelNo);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexOffsets; i_Offset++)
    aPriorVec[i_Offset] = aSkipRoots();
  
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  i8 iFindLevels = aFindEntry ? aFindEntry->nOffsets() - 1 : 2;
  
  bool bIsJuxta;
  Item* aScanEntry_sl = aSkipRoots();
  for (i8 i_ScanOffset = gpAttachMap->m_nIndexOffsets - 1;
       i_ScanOffset >= 0;
       i_ScanOffset--) {
    
    Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanOffset);
    while (aNextEntry_sl) {
      
      if (aFindEntry == aNextEntry_sl)
        break;
      
      KeyData* aNextKey = _aKeyData(aNextEntry_sl->aDataArea());
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
#ifdef USE_EB_SINGLE_COLIDX
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
/* compute prior vector for find key, such that next of priorVec[0] is        */
/* specifically the given find entry                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vPositionPriorVecEntry(Item* aFindKey, Item* aPriorVec[],
                                         Item* aFindEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //ASSERT(0 == m_iLevelNo);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexOffsets; i_Offset++)
    aPriorVec[i_Offset] = aSkipRoots();
  
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  i8 iFindLevels = aFindEntry ? aFindEntry->nOffsets() - 1 : 2;
  
  bool bIsJuxta;
  Item* aScanEntry_sl = aSkipRoots();
  for (i8 i_ScanOffset = gpAttachMap->m_nIndexOffsets - 1;
       i_ScanOffset >= 0;
       i_ScanOffset--) {
    
    Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanOffset);
    while (aNextEntry_sl) {
      
      if (aFindEntry == aNextEntry_sl)
        break;
      
      KeyData* aNextKey = _aKeyData(aNextEntry_sl->aDataArea());
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
//#ifdef USE_EB_SINGLE_COLIDX
//      if (m_pIndex->bSingleColumn()) {
//        bIsJuxta = aNextKey->bGreaterEqualSingle(aFindKey,m_pIndex);
//        //        if (bIsJuxta != aNextKey->bGreaterEqual(aFindKey,pIdxColVec)) {
//        //          aNextKey->bGreaterEqualSingle(aFindKey,m_pIndex);
//        //          aNextKey->bGreaterEqual(aFindKey,pIdxColVec);
//        //        }
//      }
//      else
//        bIsJuxta = aNextKey->bGreaterEqual(aFindKey,pIdxColVec);
//#else
//      bIsJuxta = aNextKey->bGreaterEqual(aFindKey,pIdxColVec);
//#endif
      
      bIsJuxta = KeyData::bGreaterEqual(aNextEntry_sl, aFindKey);
      
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
void EntryBuffer::vPositionPriorVecBuffer(Item* aFindKey, Item* aPriorVec[],
                                          EntryBuffer* pToBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(0 != m_iLevelNo);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexOffsets; i_Offset++)
    aPriorVec[i_Offset] = aSkipRoots();
  
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  bool bNextKeyIsGreater;
  Item* aScanEntry_sl = aSkipRoots();
  
  for (i8 i_ScanOffset = gpAttachMap->m_nIndexOffsets - 1;
       i_ScanOffset >= 0;
       i_ScanOffset--) {
    
    Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanOffset);
    while (aNextEntry_sl) {
      
      //KeyData* aNextKey = _aKeyData(aNextEntry_sl->aDataArea());
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
//#ifdef USE_EB_SINGLE_COLIDX
//      if (m_pIndex->bSingleColumn()) {
//        bNextKeyIsGreater = aNextKey->bGreaterThanSingle(aFindKey,m_pIndex);
//        RASSERT(bNextKeyIsGreater == aNextKey->bGreaterThan(aFindKey,pIdxColVec));
//      }
//      else
//        bNextKeyIsGreater = aNextKey->bGreaterThan(aFindKey,pIdxColVec);
//#else
//      bNextKeyIsGreater = aNextKey->bGreaterThan(aFindKey,pIdxColVec);
//#endif
      
      bNextKeyIsGreater = KeyData::bGreaterThan(aNextEntry_sl, aFindKey);
      
      if (bNextKeyIsGreater)
        break;
      
      if (0 == i_ScanOffset) {
        //bool bKeysAreEqual = aNextKey->bEqualTo(aFindKey,pIdxColVec);
        bool bKeysAreEqual = KeyData::bEqualTo(aNextEntry_sl, aFindKey);
        if (bKeysAreEqual &&
            (pToBuffer == EntryBuffer::pRefdEntryBuffer(aNextEntry_sl)->pPrevBuffer()))
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
//void EntryBuffer::vPositionPriorVecBuffer2(KeyData* aFindKey, Item* aPriorVec[],
//                                           EntryBuffer* pToBuffer)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
//  
//  ASSERT(0 != m_iLevelNo);
//  
//#ifdef USE_EB_LAZY_CHAIN
//  RASSERT( ! m_aLazyChain );
//#endif
//  
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  
//  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexOffsets; i_Offset++)
//    aPriorVec[i_Offset] = aSkipRoots();
//  
//  if (nItems() == 0)
//    return;
//  
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  
//  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
//  bool bNextKeyIsGreaterEqual;
//  Item* aScanEntry_sl = aSkipRoots();
//  
//  for (i8 i_ScanOffset = gpAttachMap->m_nIndexOffsets - 1;
//       i_ScanOffset >= 0;
//       i_ScanOffset--) {
//    
//    Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanOffset);
//    while (aNextEntry_sl) {
//      
//      KeyData* aNextKey = _aKeyData(aNextEntry_sl->aDataArea());
//      
//      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
//      //aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
//      
//      bNextKeyIsGreaterEqual = aNextKey->bGreaterEqual(aFindKey,pIdxColVec);
//      
//      if (bNextKeyIsGreaterEqual)
//        break;
//      
//      if (0 == i_ScanOffset) {
//        bool bKeysAreEqual = aNextKey->bEqualTo(aFindKey,pIdxColVec);
//        if (bKeysAreEqual &&
//            (pToBuffer == EntryBuffer::pRefdEntryBuffer(aNextEntry_sl)->pPrevBuffer()))
//          break;
//      }
//      
//      aScanEntry_sl = aNextEntry_sl;
//      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanOffset);
//    }
//    aPriorVec[i_ScanOffset] = aScanEntry_sl;
//  }
//}

/*----------------------------------------------------------------------------*/
/* compute prior vec for find key, such that next of priorVec[0] is the next  */
/* buffer after find keys toBuffer (ie nextBuffer->prevBuffer == toBuffer)    */
/*----------------------------------------------------------------------------*/
//void EntryBuffer::vPositionPriorVecBuffer3(KeyData* aFindKey, Item* aPriorVec[],
//                                           EntryBuffer* pToBuffer)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
//  
//  ASSERT(0 != m_iLevelNo);
//  
//#ifdef USE_EB_LAZY_CHAIN
//  RASSERT( ! m_aLazyChain );
//#endif
//  
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  
//  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexOffsets; i_Offset++)
//    aPriorVec[i_Offset] = aSkipRoots();
//  
//  if (nItems() == 0)
//    return;
//  
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  
//  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
//  bool bNextKeyIsGreaterEqual;
//  Item* aScanEntry_sl = aSkipRoots();
//  
//  for (i8 i_ScanOffset = gpAttachMap->m_nIndexOffsets - 1;
//       i_ScanOffset >= 0;
//       i_ScanOffset--) {
//    
//    Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanOffset);
//    while (aNextEntry_sl) {
//      
//      KeyData* aNextKey = _aKeyData(aNextEntry_sl->aDataArea());
//      
//      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
//      //aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
//      
//      bNextKeyIsGreaterEqual = aNextKey->bGreaterEqual(aFindKey,pIdxColVec);
//      
//      if (0 == i_ScanOffset) {
//        bool bKeysAreEqual = aNextKey->bEqualTo(aFindKey,pIdxColVec);
//        if ( bKeysAreEqual &&
//            (pToBuffer == EntryBuffer::pRefdEntryBuffer(aNextEntry_sl)) )
//          break;
//      }
//      
//      if (bNextKeyIsGreaterEqual)
//        break;
//      
//      aScanEntry_sl = aNextEntry_sl;
//      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanOffset);
//    }
//    aPriorVec[i_ScanOffset] = aScanEntry_sl;
//  }
//  
//  Item* aToBeDelete = aNextEntry(aPriorVec[0],0);
//  
//  RASSERT(pToBuffer == EntryBuffer::pRefdEntryBuffer(aToBeDelete));
//}

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
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  EntryBuffer* pThisLeftBuffer = this;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a new right buffer
  
  EntryBuffer* pNewRightBuffer = pNewNextBuffer();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find split entry. every entry from split entry and to the right of it
  // will be moved to the new right buffer
  
  Item* aPriorEntry;
  //i8 iSplitAtEntry = m_nItems/2;
  i8 iSplitAtEntry = (m_nItems * iSplitRatio) / 100;
  Item* aSplitEntry = aGetEntrySplit(iSplitAtEntry,&aPriorEntry);
  ASSERT(this->aNextEntry(aPriorEntry,0) == aSplitEntry);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy split right entries to a new right buffer
  
  KeyWrk xKeyWrk;
  Item* aCopyEntry;
  Item* aNextEntry;
  
  Item* aLastEntryRight = pNewRightBuffer->aSkipRoots();
  
  i8 nEntriesCopied = 0;
  for (aCopyEntry = aSplitEntry;
       aCopyEntry;
       aCopyEntry = aNextEntry) {

    ASSERT(! m_iLevelNo ||
           EntryBuffer::pRefdEntryBuffer(aCopyEntry)->m_pHigherBuffer == pThisLeftBuffer);
    
    nEntriesCopied++;
    
    aNextEntry = this->aNextEntry(aCopyEntry,0);
    
    Id xId = aCopyEntry->xId();
    KeyData* aKey = _aKeyData(aCopyEntry->aDataArea());
    
    i8 nKeySize = aKey->nKeySize(pIdxColVec);
    i8 nOffsets = aCopyEntry->nOffsets();
    
    xKeyWrk.vId(xId);
    xKeyWrk.vEmbedded((char*)aKey);
    xKeyWrk.vKeySize(nKeySize);
    xKeyWrk.vOffsets(nOffsets);
    
    Item* aNewEntry = pNewRightBuffer->aInsertKeyAbsL0(xKeyWrk,&aLastEntryRight);
    
    //.pNewRightBuffer->vShow();
    
    // make the moved entry in the left buffer to be a free entry
    FreeItem* aFreeEntry = aMakeFreeEntry(aCopyEntry);
    
    // these can be commented
    ASSERT(! m_iLevelNo ||
           EntryBuffer::pRefdEntryBuffer(aNewEntry)->m_pHigherBuffer == pThisLeftBuffer);
    
    i8 nBuffer = xId.iBuffer();
    HeapBuffer* pHeapBuffer =
      reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(nBuffer));
    
    EntryBuffer* pChildBuffer =
      reinterpret_cast<EntryBuffer*>(pHeapBuffer->aFirstChunk());
    
    ASSERT(pChildBuffer->m_pHigherBuffer ==
           EntryBuffer::pRefdEntryBuffer(aNewEntry)->m_pHigherBuffer);
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
  if (m_iLevelNo != 0)  {
    for (Item* aMovedEntry = pNewRightBuffer->aLeadEntry();
         aMovedEntry;
         aMovedEntry = pNewRightBuffer->aNextEntry(aMovedEntry, 0)) {
      
      xId = aMovedEntry->xId();
      i8 nBuffer = xId.iBuffer();
      
      HeapBuffer* pHeapBuffer =
      reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(nBuffer));
      
      EntryBuffer* pChildBuffer =
      reinterpret_cast<EntryBuffer*>(pHeapBuffer->aFirstChunk());
      
      ASSERT(pChildBuffer->m_pHigherBuffer ==
             EntryBuffer::pRefdEntryBuffer(aMovedEntry)->m_pHigherBuffer);
      
      
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
#ifdef WITH_DEBUG_VERSION

bool EntryBuffer::bCheckPriorVec(Item* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  vNumberEntries();
  
  Item* aPriorEntry_0 = aPriorVec[0];
  Item* aPriorEntry_1 = aPriorVec[1];
  Item* aPriorEntry_2 = aPriorVec[2];
  
  Item* aNextEntry_0  = aNextEntry(aPriorEntry_0,0);
  Item* aNextEntry_1  = aNextEntry(aPriorEntry_1,1);
  Item* aNextEntry_2  = aNextEntry(aPriorEntry_2,2);
  
  if (aNextEntry_0 && aNextEntry_1) {
    if (aNextEntry_0->m_iSkipNo > aNextEntry_1->m_iSkipNo) {
      vVerifyFault();
      return false;
    }
  }
  
  if (aNextEntry_1 && aNextEntry_2) {
    if (aNextEntry_1->m_iSkipNo > aNextEntry_2->m_iSkipNo) {
      vVerifyFault();
      return false;
    }
  }
  
  if (aNextEntry_0 && aNextEntry_2) {
    if (aNextEntry_0->m_iSkipNo > aNextEntry_2->m_iSkipNo) {
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
#ifdef WITH_DEBUG_VERSION
void EntryBuffer::vNumberEntries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // set entry no
  
  i8 iEntryNo = 0;
  
  for (Item* aTestEntry_0 = aNextEntry(aSkipRoots(),0);
       aTestEntry_0;
       aTestEntry_0 = aNextEntry(aTestEntry_0,0))
    
    aTestEntry_0->m_iSkipNo = iEntryNo++;
  
}
#endif

/*----------------------------------------------------------------------------*/
/* quick verify                                                               */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vQuickVerify(Item* aSeeEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  for (Item* aItem = aFirstItem();
       aItem != nullptr;
       aItem = aAdjacentItem(aItem))
    {;}
  
  for (Item* aTestEntry = aNextEntry(aSkipRoots(),0);
       aTestEntry;
       aTestEntry = aNextEntry(aTestEntry,0)) {
    
    if (aSeeEntry)
      vShowEntry(aTestEntry);
    
    if (aTestEntry == aSeeEntry)
      int iDebug = 1;
    
    for (i8 i_ScanOffset = aTestEntry->nOffsets()-1; i_ScanOffset >= 0; i_ScanOffset--) {
      Item* aCheckEntry = aNextEntry(aTestEntry,i_ScanOffset);
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
  //    for (Item* aTestEntry = aNextEntry(aSkipRoots(),*i_ScanOffset);
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
bool EntryBuffer::bQuickVerify(Item* aNotSeeEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  for (Item* aTestEntry = aNextEntry(aSkipRoots(),0);
       aTestEntry;
       aTestEntry = aNextEntry(aTestEntry,0)) {
    
    for (i8 i_ScanOffset = aTestEntry->nOffsets()-1; i_ScanOffset >= 0; i_ScanOffset--) {
      
      Item* aCheckEntry = aNextEntry(aTestEntry,i_ScanOffset);
      if (! aCheckEntry)
        continue;
      
      if (aCheckEntry == aNotSeeEntry) {
        vShowEntry(aTestEntry);
        vShowEntry(aCheckEntry);
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
  
  RASSERT(0 == m_iLevelNo);
  RASSERT(m_pIndex->bIsHidden());
  
  //  vTrace("VerifyL0 %s: bufno: %lld level %lld\n"_Stz,
  //         m_pIndex->aIndexName(),m_iBufferNo,m_iLevelNo);
  
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
  
#ifdef WITH_DEBUG_VERSION
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
  
  RASSERT(0 == m_iLevelNo);
  
  //  vTrace("VerifyL0 %s: bufno: %lld level %lld\n"_Stz,
  //         m_pIndex->aIndexName(),m_iBufferNo,m_iLevelNo);
  
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
  
  RASSERT(0 != m_iLevelNo);
  
  //  vTrace("VerifyLn %s: bufno: %lld level %lld\n"_Stz,
  //         m_pIndex->aIndexName(),m_iBufferNo,m_iLevelNo);
  
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
  
#ifdef WITH_DEBUG_VERSION
  bValid = bVerifySkipListLevels();
  if (! bValid)
    return false;
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify the contents of child buffers
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
    
    EntryBuffer* pRefEntBuf = EntryBuffer::pRefdEntryBuffer(aItem);
    if (pRefEntBuf->m_iLevelNo) {
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
  
  RASSERT(m_iLevelNo >= 1);
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
    
    // get the buffer that this entry references
    EntryBuffer* pChildEntryBuffer = EntryBuffer::pRefdEntryBuffer(aItem);
    
    // get the adjacent entry
    Item* aAdjacentEntry = aNextEntry(aItem,0);
    
    if (aAdjacentEntry) {
      // get the buffer that the adjacent entry references
      EntryBuffer* pAdjacentChildEntryBuffer =
        EntryBuffer::pRefdEntryBuffer(aAdjacentEntry);
      
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
  
  RASSERT(m_iLevelNo >= 1);
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
    
    EntryBuffer* pChildEntBuf = EntryBuffer::pRefdEntryBuffer(aItem);
    
    if ( ! pChildEntBuf->bValidEntryBuffer() ) {
      vVerifyFault();
      return false;
    }
    
    if (pChildEntBuf->iLevelNo() != m_iLevelNo - 1) {
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
  
  RASSERT(0 == m_iLevelNo);
  RASSERT(m_pIndex->bIsHidden());
  
  //vShow();
  
  for (i8 i_RowNo = 0; i_RowNo < gpAttachMap->m_nIndexEntries; i_RowNo++) {
    
    RASSERT(i_RowNo < C_MAX_ITEMS);
    i8 i_RowOffset = m_i4aItemOffsets[i_RowNo];
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // validate free offset element
    
    if (i_RowOffset < 0) {
      
      // last offset in free chain
      if (C_NO_MORE_ITEMS == - i_RowOffset)
        continue;
      
      // get the next offset in free chain
      i8 i_NextRowEntryOffset = m_i4aItemOffsets[ - i_RowOffset ];
      
      // next offset should be free (or end of free chain)
      if (i_NextRowEntryOffset >= 0) {
        vVerifyFault();
        return false;
      }
      
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // validate offset element which is the offset to an entry
    
    Item* aItem = aGetEntryOffset(i_RowOffset);
    if (! aItem->bValidItem())
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
  
  RASSERT(m_iLevelNo >= 1);
  
  //vShow();
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
    
    // get the entry buffer that this entry references
    EntryBuffer* pChildEntryBuffer = EntryBuffer::pRefdEntryBuffer(aItem);
    
    //pChildEntryBuffer->vShow();
    
    
    // get the first entry of the child buffer
    Item* aChildFirstEntry = pChildEntryBuffer->aLeadEntry();
    
    // get keys of the two entries
    KeyData* aKeyParent = _aKeyData(aItem->aDataArea());
    KeyData* aKeyChild  = _aKeyData(aChildFirstEntry->aDataArea());
    
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
  
  for (FreeItem* aFreeEntry = m_aFreeItemChain;
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
   
   {[Item]|[FreeItem]...}[Wilderness]
   
   The number of Entries must match the number of items in the buffer
   */
  
  i8 nEntryItems = 0;
  
  for (Item* aItem = aFirstItem();
       aItem != nullptr;
       aItem = aAdjacentItem(aItem)) {
    
    //vTrace("Verify item %p\n"_Stz,pItem);
    
    eCN eClass = aItem->eClass();
    
    switch (eClass) {
        
      case CN_ENTRY: {
        Item* aItem = (Item*)aItem;
        if (! aItem->bValidItem()) {
          vShow();
          vVerifyFault();
          return false;
        }
        nEntryItems++;
        break;
      }
        
      case CN_FREEENTRY: {
        FreeItem* aFreeEntry = _aFreeItem(aItem);
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
  
  RASSERT(m_iLevelNo >= 1);
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
    
    // get next entry
    Item* aNextEntry = this->aNextEntry(aItem,0);
    if (! aNextEntry)
      break;
    
    // get the child entry buffers
    EntryBuffer* pChildBufferA = EntryBuffer::pRefdEntryBuffer(aItem);
    EntryBuffer* pChildBufferB = EntryBuffer::pRefdEntryBuffer(aNextEntry);
    
    // child A should point to child B
    if (pChildBufferA->pNextBuffer() != pChildBufferB) {
      vShow(aItem);
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
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
    
    // get next entry
    Item* aNextEntry = this->aNextEntry(aItem,0);
    if (! aNextEntry)
      break;
    
    // get keys of the two entries
    KeyData* aKeyA = _aKeyData(aItem->aDataArea());
    KeyData* aKeyB = _aKeyData(aNextEntry->aDataArea());
    
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
    EntryBuffer* pPrevBuffer = _pEntryBuffer(m_pPrevBuffer);
    if (pPrevBuffer->pNextBuffer() != this) {
      vVerifyFault();
      return false;
    }
    if (pPrevBuffer->iLevelNo() != m_iLevelNo) {
      vVerifyFault();
      return false;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_pNextBuffer) {
    EntryBuffer* pNextBuffer = _pEntryBuffer(m_pNextBuffer);
    if (pNextBuffer->pPrevBuffer() != this) {
      vVerifyFault();
      return false;
    }
    if (pNextBuffer->iLevelNo() != m_iLevelNo) {
      vVerifyFault();
      return false;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_pHigherBuffer) {
    EntryBuffer* pHigherBuffer = _pEntryBuffer(m_pHigherBuffer);
    if (pHigherBuffer->iLevelNo() != m_iLevelNo + 1) {
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
  
  RASSERT(0 == m_iLevelNo);
  RASSERT(m_pIndex->bIsHidden());
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
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
  
  RASSERT(0 == m_iLevelNo);
  RASSERT(! m_pIndex->bIsHidden());
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
    
    // get the entry buffer that this L0 key leaf references
    EntryBuffer* pEntryBuffer = EntryBuffer::pRefdEntryBuffer(aItem);
    
    // that referenced entry buffer should be an L0 buffer
    if (0 != pEntryBuffer->iLevelNo()) {
      vVerifyFault();
      return false;
    }
    
    // that referenced entry buffer should be for a hidden index
    if (! pEntryBuffer->pIndex()->bIsHidden()) {
      vVerifyFault();
      return false;
    }
    
    // get the row entry from referenced entry buffer
    i8 i_Item = aItem->iItemNo();
    Item* aRowEntry = pEntryBuffer->aGetEntry_Item(i_Item);
    
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
  
  Item* aNextEntry2;
  Item* aNextEntry1;
  Item* aNextEntry0;
  
  for (i8 i_ScanLevel = gpAttachMap->m_nIndexOffsets - 1;
       i_ScanLevel >= 0;
       i_ScanLevel--) {
    
    if (bShowKeys)
      vTrace("i_ScanLevel %lld\n"_Stz,i_ScanLevel);
    
    Item* aTestEntry_sl = aNextEntry(aSkipRoots(),i_ScanLevel);
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
        KeyData* aTestKey = _aKeyData(aTestEntry_sl->aDataArea());
        vTrace("Test %p: "_Stz,aTestEntry_sl);
        aTestKey->vShow(pIdxColVec, m_pIndex->bIsHidden() ? true : false);
        vTrace("\n"_Stz);
      }
      
      aTestEntry_sl = aNextEntry(aTestEntry_sl,i_ScanLevel);
    }
  }
  
#ifdef WITH_DEBUG_VERSION
  if (! bVerifySkipListLevels())
    return false;
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the skip list levels                                                */
/*----------------------------------------------------------------------------*/
#ifdef WITH_DEBUG_VERSION

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
  Item* aTestEntry_0 = aNextEntry(aSkipRoots(),0);
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
      
      Item* aScanEntry_sl = aNextEntry(aSkipRoots(),i_SkipLevel);
      while (aScanEntry_sl) {
        
        if (aScanEntry_sl == aTestEntry_0)
          break;
        
        Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_SkipLevel);
        
        // check we dont go backwwards
        if (aNextEntry_sl->m_iSkipNo < aScanEntry_sl->m_iSkipNo) {
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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
