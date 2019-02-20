/*============================================================================*/
/* EntryBuffer_all.cpp -- working file to sort out the fun's of Item PoolBuffer  */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Complex Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
  
  Item* xPrevVec[gpAttachMap->m_nIndexLevels];
  
  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexLevels; i_Offset++)
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
    i8 nLevels = aItem->nLevels();
    
    // connect the dots
    for (i8 i_Level = 1; i_Level < nLevels; i_Level++) {
      
      // set previous entry at this level to refer to this entry
      xPrevVec[i_Level]->vNEO(i_Level,iEntryOffset);
      
      // the previous entry at this level now becomes this entry
      xPrevVec[i_Level] = aItem;
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
  
  Item* xPriorVec[gpAttachMap->m_nIndexLevels];
  
  KeyData* aKey = _aKeyData(aItem->aDataArea());
  
  vPositionPriorVecEntry(aKey, xPriorVec, aItem);
  
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
//  Item* xPriorVec[gpAttachMap->m_nIndexLevels];
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
    if (! m_iHeightNo) {
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
  
  RASSERT(xKeyWrk.nLevels());
  
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
  
  //if (! m_pIndex->bIsHidden() && m_iHeightNo)
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
  
  RASSERT(xKeyWrk.nLevels());
  
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
  
  i8 nPriorLevels = aKeyEntry->nLevels();
  for (i8 i_Level = 0; i_Level < nPriorLevels; i_Level++)
    aKeyEntry->vNEO(i_Level,-1);
  
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
  
  xKeyWrk.vRandomLevels();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // position on the initial buffer we will try to insert into
  
  KeyData* aInsKey = xKeyWrk.aKey();
  i8 iInsKeyLen = xKeyWrk.nKeySize();
  
  EntryBuffer* pPositionedBuffer =
  pRootKeyBuffer->pPositionPriorBuffer(aInsKey);
  
  ASSERT(0 == pPositionedBuffer->iHeightNo());
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
/* get next entry                                                             */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aNextEntry(Item* aCurrent, i8 i_Level)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT(aCurrent->bValidItem());
  
  i4 iOffset = aCurrent->iNEO(i_Level);
  
  if (-1 == iOffset)
    return nullptr;
  
  Item* aItem = aAdd<Item*>(aItemArea(), iOffset);
  
  RASSERT(! aItem || aItem->bValidItem());

  return aItem;
}

/*----------------------------------------------------------------------------*/
/* position at an index buffer at given height starting from this buffer      */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pPositionHeight(KeyData* aGivenKey, i8 nKeyCols,
                                         eJC eJuxtaCompare, i8 iHeight)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
  
  //if (m_aLazyChain)
  //  vBuildSkipList();
#endif
  
  EntryBuffer* pEntryBuffer = this;
  
  i8 iBufferHeight;
  Item* aItem;
  
  while (true) {
    iBufferHeight = pEntryBuffer->iHeightNo();
    
    if (iBufferHeight == iHeight)
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
        aIndexEntry = aNextEntry(aIndexEntry, iHeightNo());
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
  
  for (i8 i_ScanLevel = 2; i_ScanLevel >= 0; i_ScanLevel--) {
    
    Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
    while (aNextEntry_sl) {
      
      KeyData* aNextKey = _aKeyData(aNextEntry_sl->aDataArea());
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aGivenKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
      bIsJuxta = aGivenKey->bJuxtaCompare(JC_AFTER,aNextKey,pIdxColVec,
                                          nKeyCols,bIsEqual);
      
      // have we found to key during a query?
      if (bIsEqual) {
        if (0 == i_ScanLevel)
          return aNextEntry_sl;
        else
          break;
      }
      
      // if given key is not same/after next key then break to outer loop
      if (! bIsJuxta)
        break;
      
      aScanEntry_sl = aNextEntry_sl;
      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanLevel);
    }
    //pPriorVec[*i_ScanLevel] = pScanEntry_sl;
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
  
  ASSERT(0 == m_iHeightNo);
  
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
  Item*     xPriorVec[gpAttachMap->m_nIndexLevels];
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
    
    if (m_iHeightNo) {
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
    
    KeyData* aMoveKey = _aKeyData(aMoveEntry->aDataArea());
    
    //aKey->vShow(pIdxColVec);
    //vTrace("\n"_Stz);
    
    vPositionPriorVecEntry(aMoveKey, xPriorVec, aMoveEntry);
    
    if (m_iHeightNo) {
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
    i8 nLevels = aThisEntry->nLevels();
    
    xKeyWrk.vId(xId);
    xKeyWrk.vEmbedded((char*)aKey);
    xKeyWrk.vKeySize(nKeySize);
    xKeyWrk.vLevels(nLevels);
    
    pTempBuffer->aInsertKeyAbsL0(xKeyWrk,&aPriorEntry);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Item* aThisWilderness = aItemArea();
//    = aAdd<Item*>(aSkipRoots(),
//                  aSkipRoots()->nSizeItem_WithoutOffsets() +
//                    aSkipRoots()->iNEOsize());
  
  vSetUsed(aThisWilderness);
  
  vItems(0);
  
  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexLevels; i_Offset++)
    aSkipRoots()->vNEO(i_Offset,-1);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  aPriorEntry = pThisBuffer->aSkipRoots();
  
  for (Item* aTempEntry = pTempBuffer->aLeadEntry();
       aTempEntry != nullptr;
       aTempEntry = pTempBuffer->aNextEntry(aTempEntry, 0)) {
    
    Id xId = aTempEntry->xId();
    KeyData* aKey = _aKeyData(aTempEntry->aDataArea());
    i8 nKeySize = aKey->nKeySize(pIdxColVec);
    i8 nLevels = aTempEntry->nLevels();
    
    xKeyWrk.vId(xId);
    xKeyWrk.vEmbedded((char*)aKey);
    xKeyWrk.vKeySize(nKeySize);
    xKeyWrk.vLevels(nLevels);
    
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
  
  i8 nLevels = aItem->nLevels();
  
  for (i8 i_Level = 0; i_Level < nLevels; i_Level++) {
    i4 iNEO = aItem->iNEO(i_Level);
    aPriorVec[i_Level]->vNEO(i_Level,iNEO);
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
//  Item* xPriorVec[gpAttachMap->m_nIndexLevels];
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
    for (i8 i_Level = 0; i_Level < aItem->nLevels(); i_Level++) {
      vTrace("  i : %lld Item : %p i_Offset : %lld offset : %lld pointsTo : %p\n"_Stz,
             i,
             aItem,
             i_Level,
             aItem->iNEO(i_Level),
             aGetEntryOffset(aItem->iNEO(i_Level)));
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
  i8 nInsertLevels = aInsertedKeyEntry->nLevels();
  
  Id xInsertId(*nPoolBufferNo(),0,nInsertLevels);
  
  KeyWrk xKeyWrkIns;
  xKeyWrkIns.vId(xInsertId);
  xKeyWrkIns.vEmbedded((char*)aInsertKey);
  xKeyWrkIns.vKeySize(iInsertKeyLen);
  xKeyWrkIns.vLevels(nInsertLevels);
  
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
  
  RASSERT(xKeyWrk.nLevels());
  
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
  
  RASSERT(xKeyWrk.nLevels());
  
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
  
  Item* xPriorVec[gpAttachMap->m_nIndexLevels];
  
  KeyData* aKey = _aKeyData(aKeyEntry->aDataArea());
  
  if (0 == m_iHeightNo)
    vPositionPriorVecEntry(aKey, xPriorVec, nullptr);
  else {
    EntryBuffer* pToBuffer = EntryBuffer::pRefdEntryBuffer(aKeyEntry);
    vPositionPriorVecBuffer(aKey, xPriorVec, pToBuffer);
  }
  
  //vDumpPriorVec(xPriorVec);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set prior offsets
  
  i4 iKeyEntryOffset = iToOffset(aKeyEntry);
  
  i8 nLevels = aKeyEntry->nLevels();
  for (i8 iNextLevel = 0; iNextLevel < nLevels; iNextLevel++) {
    i4 iPriorOffset = xPriorVec[iNextLevel]->iNEO(iNextLevel);
    aKeyEntry->vNEO(iNextLevel,iPriorOffset);
    xPriorVec[iNextLevel]->vNEO(iNextLevel,iKeyEntryOffset);
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
 if (m_iHeightNo)
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
    if (m_iHeightNo) {
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
  
  if (pInsertIntoBuffer->m_iHeightNo) {
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
  KeyData* aFirstKey = _aKeyData(aFirstEntry->aDataArea());

  KeyData* aKeyTest = xKeyTest.aKey();

  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  i8 nKeyCols = pIdxColVec->nItems();
  
  bool bIsJuxta =
  aKeyTest->bLessThan(aFirstKey,pIdxColVec);
  //KeyData::bLessThan(xKeyTest,aFirstEntry);
  
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
    
    RASSERT(pEntryBuffer->iHeightNo() == 0);
    
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
  pNewBuffer->vHeightNo(m_iHeightNo+1);
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
    
    i8 nLevelsLeft = aFirstKeyLeft->nLevels();
    Id xIdLeft(*pLeftBuffer->nPoolBufferNo(),0,nLevelsLeft);
    KeyData* aKeyLeft = _aKeyData(aFirstKeyLeft->aDataArea());
    i8 iKeyLenLeft = aKeyLeft->nKeySize(m_pIndex->pIdxColVec());
    
    
    KeyWrk xKeyWrkLeft;
    xKeyWrkLeft.vId(xIdLeft);
    xKeyWrkLeft.vEmbedded((char*)aKeyLeft);
    xKeyWrkLeft.vKeySize(iKeyLenLeft);
    xKeyWrkLeft.vLevels(nLevelsLeft);
    
    
    Item* aNewKeyLeft =
    pHigherBuffer->aInsertKeyUpwards(xKeyWrkLeft,ppRootKeyBuffer);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the first key of right buffer into higher buffer
  
  Item* aFirstKeyRight = pRightBuffer->aLeadEntry();
  
  KeyData* aKeyRight = _aKeyData(aFirstKeyRight->aDataArea());
  
  i8 iKeyLenRight = aKeyRight->nKeySize(m_pIndex->pIdxColVec());
  i8 nLevelsRight = aFirstKeyRight->nLevels();
  
  Id xIdRight(*pRightBuffer->nPoolBufferNo(),0,nLevelsRight);
  
  KeyWrk xKeyWrkRight;
  xKeyWrkRight.vId(xIdRight);
  xKeyWrkRight.vEmbedded((char*)aKeyRight);
  xKeyWrkRight.vKeySize(iKeyLenRight);
  xKeyWrkRight.vLevels(nLevelsRight);
  
  
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
  pNewBuffer->vHeightNo(m_iHeightNo);
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

/*----------------------------------------------------------------------------*/
/* position at the L0 buffer into which key should be inserted                */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pPositionPriorBuffer(KeyData* aFindKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if ( ! nItems() ) {
    ASSERT( 0 == iHeightNo());
    return this;
  }
  
  EntryBuffer* pEntryBuffer = this;
  while (true) {
    
    // have we arrived at L0 ?
    i8 iBufferiHeight = pEntryBuffer->iHeightNo();
    if (iBufferiHeight == 0) {
      break;
    }
    else {
#ifdef USE_EB_LAZY_CHAIN
      RASSERT( ! m_aLazyChain );
#endif
    }
    
    // get the entry before the given insert key
    Item* aEntryPrior = pEntryBuffer->pPositionPriorEntry(aFindKey);
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
  
  for (i8 i_ScanLevel = gpAttachMap->m_nIndexLevels - 1;
       i_ScanLevel >= 0;
       i_ScanLevel--) {
    
    Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
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
      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanLevel);
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
  
  //ASSERT(0 == m_iHeightNo);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexLevels; i_Offset++)
    aPriorVec[i_Offset] = aSkipRoots();
  
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  i8 iFindLevels = aFindEntry ? aFindEntry->nLevels() - 1 : 2;
  
  bool bIsJuxta;
  Item* aScanEntry_sl = aSkipRoots();
  for (i8 i_ScanLevel = gpAttachMap->m_nIndexLevels - 1;
       i_ScanLevel >= 0;
       i_ScanLevel--) {
    
    Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
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
        
        if (i_ScanLevel > iFindLevels)
          break;
      }
      
      aScanEntry_sl = aNextEntry_sl;
      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanLevel);
    }
    aPriorVec[i_ScanLevel] = aScanEntry_sl;
  }
}

/*----------------------------------------------------------------------------*/
/* compute prior vec for find key, such that next of priorVec[0] is the next  */
/* buffer after find keys toBuffer (ie nextBuffer->prevBuffer == toBuffer)    */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vPositionPriorVecBuffer(KeyData* aFindKey, Item* aPriorVec[],
                                          EntryBuffer* pToBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(0 != m_iHeightNo);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexLevels; i_Offset++)
    aPriorVec[i_Offset] = aSkipRoots();
  
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  bool bNextKeyIsGreater;
  Item* aScanEntry_sl = aSkipRoots();
  
  for (i8 i_ScanLevel = gpAttachMap->m_nIndexLevels - 1;
       i_ScanLevel >= 0;
       i_ScanLevel--) {
    
    Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
    while (aNextEntry_sl) {
      
      KeyData* aNextKey = _aKeyData(aNextEntry_sl->aDataArea());
      
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
#ifdef USE_EB_SINGLE_COLIDX
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
      
      if (0 == i_ScanLevel) {
        bool bKeysAreEqual = aNextKey->bEqualTo(aFindKey,pIdxColVec);
        if (bKeysAreEqual &&
            (pToBuffer == EntryBuffer::pRefdEntryBuffer(aNextEntry_sl)->pPrevBuffer()))
          break;
      }
      
      aScanEntry_sl = aNextEntry_sl;
      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanLevel);
    }
    aPriorVec[i_ScanLevel] = aScanEntry_sl;
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
//  ASSERT(0 != m_iHeightNo);
//  
//#ifdef USE_EB_LAZY_CHAIN
//  RASSERT( ! m_aLazyChain );
//#endif
//  
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  
//  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexLevels; i_Offset++)
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
//  for (i8 i_ScanLevel = gpAttachMap->m_nIndexLevels - 1;
//       i_ScanLevel >= 0;
//       i_ScanLevel--) {
//    
//    Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
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
//      if (0 == i_ScanLevel) {
//        bool bKeysAreEqual = aNextKey->bEqualTo(aFindKey,pIdxColVec);
//        if (bKeysAreEqual &&
//            (pToBuffer == EntryBuffer::pRefdEntryBuffer(aNextEntry_sl)->pPrevBuffer()))
//          break;
//      }
//      
//      aScanEntry_sl = aNextEntry_sl;
//      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanLevel);
//    }
//    aPriorVec[i_ScanLevel] = aScanEntry_sl;
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
//  ASSERT(0 != m_iHeightNo);
//  
//#ifdef USE_EB_LAZY_CHAIN
//  RASSERT( ! m_aLazyChain );
//#endif
//  
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  
//  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexLevels; i_Offset++)
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
//  for (i8 i_ScanLevel = gpAttachMap->m_nIndexLevels - 1;
//       i_ScanLevel >= 0;
//       i_ScanLevel--) {
//    
//    Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
//    while (aNextEntry_sl) {
//      
//      KeyData* aNextKey = _aKeyData(aNextEntry_sl->aDataArea());
//      
//      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
//      //aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
//      
//      bNextKeyIsGreaterEqual = aNextKey->bGreaterEqual(aFindKey,pIdxColVec);
//      
//      if (0 == i_ScanLevel) {
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
//      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanLevel);
//    }
//    aPriorVec[i_ScanLevel] = aScanEntry_sl;
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

    ASSERT(! m_iHeightNo ||
           EntryBuffer::pRefdEntryBuffer(aCopyEntry)->m_pHigherBuffer == pThisLeftBuffer);
    
    nEntriesCopied++;
    
    aNextEntry = this->aNextEntry(aCopyEntry,0);
    
    Id xId = aCopyEntry->xId();
    KeyData* aKey = _aKeyData(aCopyEntry->aDataArea());
    
    i8 nKeySize = aKey->nKeySize(pIdxColVec);
    i8 nLevels = aCopyEntry->nLevels();
    
    xKeyWrk.vId(xId);
    xKeyWrk.vEmbedded((char*)aKey);
    xKeyWrk.vKeySize(nKeySize);
    xKeyWrk.vLevels(nLevels);
    
    Item* aNewEntry = pNewRightBuffer->aInsertKeyAbsL0(xKeyWrk,&aLastEntryRight);
    
    //.pNewRightBuffer->vShow();
    
    // make the moved entry in the left buffer to be a free entry
    FreeItem* aFreeEntry = aMakeFreeEntry(aCopyEntry);
    
    // these can be commented
    ASSERT(! m_iHeightNo ||
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
  if (m_iHeightNo != 0)  {
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
