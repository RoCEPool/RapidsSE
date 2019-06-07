/*============================================================================*/
/* EntBuffer_insert.cpp -- insert related functions for Entry Buffer        */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* build skip list from the lazy chain                                        */
/*----------------------------------------------------------------------------*/
void EntBuffer::vBuildSkipList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  if (! m_aLazyChain )
    return;
  
  Ent* aOriginalFirstEntry = aNextEntry(aSkipRoots(),0);
  
  static i8 nBuild = 0;
  //vTrace("vBuildSkipList %lld (%lld items)\n"_Stz,++nBuild,nItems());
  
  Ent* aLazyChain = m_aLazyChain;
  m_aLazyChain = nullptr;
  
  while (aLazyChain) {
    
    Ent* aKeyEntry = aLazyChain;
    
    i8 iNextOffset = aKeyEntry->iNEO(0);
    if (-1 == iNextOffset)
      aLazyChain = nullptr;
    else
      aLazyChain = aGetEntryOffset(iNextOffset);
    
    vInsertKeySkip(aKeyEntry);
  }
  
  Ent* aInsertedKeyEntry = aNextEntry(aSkipRoots(),0);
  
  vHandleFirstInsert(aInsertedKeyEntry,aOriginalFirstEntry,nullptr);
}

/*----------------------------------------------------------------------------*/
/* build skip list based on existing L0 offset chain being in sorted order    */
/*----------------------------------------------------------------------------*/
void EntBuffer::vBuildSkipListSorted(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  //
  //  H:
  //  [p]..[ ].................[ ]
  //  [p]..[ ]..[ ].......[ ]..[ ]
  //  [p]--[ ]--[ ]--[ ]--[ ]--[ ]  (L0 is a-priori in sorted order)
  //
  
  /*
   // ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
   // temp: clear out all l1 and l2 offsets for testing purpoes
   
   for (Ent* aEnt = aSkipRoots();
   aEnt != nullptr;
   aEnt = aNextEntry(aEnt, 0)) {
   
   // get the number of offsets for this entry
   i8 nOffsets = aEnt->nOffsets();
   
   // connect the dots
   for (i8 i_Offset = 1; i_Offset < nOffsets; i_Offset++)
   aEnt->vNEO(i_Offset,-1);
   }
   vShow();
   */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent* xPrevVec[gpAttachMap->m_nIndexLevels];
  
  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexLevels; i_Offset++)
    xPrevVec[i_Offset] = aSkipRoots();
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // go through each entry in L0 order and
  //   correct the prior offset to that entry for each level
  
  for (Ent* aEnt = aLeadEntry();
       aEnt != nullptr;
       aEnt = aNextEntry(aEnt, 0)) {
    
    // where is this entry in the buffer (offset-wise)?
    i4 iEntryOffset = iToOffset(aEnt);
    
    // get the number of offsets for this entry
    i8 nLevels = aEnt->nLevels();
    
    // connect the dots
    for (i8 i_Level = 1; i_Level < nLevels; i_Level++) {
      
      // set previous entry at this level to refer to this entry
      xPrevVec[i_Level]->vNEO(i_Level,iEntryOffset);
      
      // the previous entry at this level now becomes this entry
      xPrevVec[i_Level] = aEnt;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* compact this buffer                                                        */
/*----------------------------------------------------------------------------*/
void EntBuffer::vCompact(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  //RASSERT( ! m_aLazyChain );
  
  if (m_aLazyChain)
    vBuildSkipList();
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  FreeItem* aFreeItem;
  Ent*     aMoveEntry;
  Ent*     xPriorVec[gpAttachMap->m_nIndexLevels];
  
  // scan item is the working pointer
  Ent* aScanItem = aFirstItem();
  //ASSERT(aScanItem->eClass() == CN_ENT);
  
  // loop while we can still reclaim more space
  for (;;) {
    
    if (m_iBufferHeight) {
      //vShow();
      int iDebug = 1;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // find the next free entry in buffer
    
    aFreeItem = nullptr;
    for (;
         aScanItem != nullptr;
         aScanItem = aAdjacentItem(aScanItem)) {
      
      eCN eClass = aScanItem->eClass();
      
      if (CN_FREEITEM == eClass) {
        aFreeItem = _aFreeItem(aScanItem);
        break;
      }
    }
    
    // return if no more free entries
    if (! aFreeItem)
      break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // find the next entry in buffer after the free entry
    
    aMoveEntry = nullptr;
    for (Ent* aEnt = aAdjacentItem((Ent*)aFreeItem);
         aEnt != nullptr;
         aEnt = aAdjacentItem(aEnt)) {
      
      eCN eClass = aEnt->eClass();
      
      RASSERT ((eClass == CN_ENT) ||
               (eClass == CN_FREEITEM) ||
               (eClass == CN_WILDERNESS));
      
      
      if (CN_ENT == eClass) {
        aMoveEntry = static_cast<Ent*>(aEnt);
        break;
      }
    }
    
    // return if no more entries after the free entry
    if (! aMoveEntry)
      break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the prior vector to the entry being moved
    
    //Key* aMoveKey = _aKey(aMoveEntry->aDataArea());
    
    //aKey->vShow(pIdxColVec);
    //vTrace("\n"_Stz);
    
    vPositionPriorVecEntry(aMoveEntry, xPriorVec, aMoveEntry);
    
    if (m_iBufferHeight) {
      //vDumpPriorVec(xPriorVec);
      int iDebug = 1;
    }
    
    i4 iWasOffset = iToOffset(aMoveEntry);
    i4 iNowOffset = iToOffset((Ent*)aFreeItem);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // now copy move entry left onto free entry
    
    // calculate the number of bytes to copy
    i8 nCopyBytes = aMoveEntry->nSizeItem_WithoutOffsets()
    + aMoveEntry->iNEOsize();
    
    // copy the move entry over the free entry
    MemLib::vMove(aFreeItem, aMoveEntry, nCopyBytes, CN_ENTBUFFER);
    
    // give the reclaimed space back to the buffer
    i8 nReclaimedBytes = nSub(aMoveEntry,aFreeItem);
    //vGiveSpace(nReclaimedBytes);
    
    // convert the moved entry to a free entry
    FreeItem* aOpenedFreeEntry = aAdd<FreeItem*>(aFreeItem,nCopyBytes);
    new(aOpenedFreeEntry) FreeItem(nReclaimedBytes);
    
    Ent* aCheckEntry = reinterpret_cast<Ent*>(aFreeItem);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the prior vector to the entry being moved
    
    //vDumpPriorVec(xPriorVec);
    for (i8 i_Offset = 0; i_Offset < 3; i_Offset++) {
      Ent* aPriorEntry = xPriorVec[i_Offset];
      i4 iPriorOffset = aPriorEntry->iNEO(i_Offset);
      if (iWasOffset == iPriorOffset)
        aPriorEntry->vNEO(i_Offset,iNowOffset);
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move scan
    
    aScanItem = (Ent*)aFreeItem;
    
  } // for
  
  ASSERT( ! aScanItem || CN_FREEITEM == aScanItem->eClass() );
  
  if (aScanItem)
    vSetUsed(aScanItem);
  
  //vShow();
}

/*----------------------------------------------------------------------------*/
/* compact this buffer                                                        */
/*----------------------------------------------------------------------------*/
void EntBuffer::vCompactLeft(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT(! m_aLazyChain);
#endif
  
  EntBuffer* pThisBuffer = this;
  EntBuffer* pTempBuffer = new EntBuffer();
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  KeyWrk xKeyWrk;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent* aPriorEntry = pTempBuffer->aSkipRoots();
  
  for (Ent* aThisEntry = pThisBuffer->aLeadEntry();
       aThisEntry != nullptr;
       aThisEntry = pThisBuffer->aNextEntry(aThisEntry, 0)) {
    
    Id xId = aThisEntry->xId();
    
    xKeyWrk.vSetKey(aThisEntry, xId.iBuffer(), xId.iItem(), pIdxColVec);
    pTempBuffer->aInsertKeyAbsL0(xKeyWrk,&aPriorEntry);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent* aThisWilderness = aItemArea();
  vSetUsed(aThisWilderness);
  
  vItems(0);
  
  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexLevels; i_Offset++)
    aSkipRoots()->vNEO(i_Offset,-1);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  aPriorEntry = pThisBuffer->aSkipRoots();
  
  for (Ent* aTempEntry = pTempBuffer->aLeadEntry();
       aTempEntry != nullptr;
       aTempEntry = pTempBuffer->aNextEntry(aTempEntry, 0)) {
    
    Id xId = aTempEntry->xId();
    
    xKeyWrk.vSetKey(aTempEntry, xId.iBuffer(), xId.iItem(), pIdxColVec);
    pThisBuffer->aInsertKeyAbsL0(xKeyWrk,&aPriorEntry);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pThisBuffer->vBuildSkipListSorted();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete pTempBuffer;
}

/*----------------------------------------------------------------------------*/
/* delete upwards                                                             */
/*----------------------------------------------------------------------------*/
void EntBuffer::vDeleteUpwards(Ent* aDeleteKey,
                                 EntBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent* aPriorEntry = pPositionPriorEntry(aDeleteKey);
  if (!aPriorEntry)
    return;
  
  Ent* aEntryToBeDeleted = aNextEntry(aPriorEntry,0);
  if (!aEntryToBeDeleted)
    return;
  
  //  vTrace("[%lld] pDeleteEntry key %lld\n"_Stz,
  //         *nPoolBufferNo(),
  //         pEntryToBeDeleted->aKey()->iGetInt());
  
  Key* aDeleteEntryKey = _aKey(aEntryToBeDeleted->aDataArea());
  
  //IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  //if ( ! aDeleteEntryKey->bEqualTo(aDeleteKey,pIdxColVec) ) {
  if ( ! Key::bEqualTo(aEntryToBeDeleted, aDeleteKey) ) {
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
  
  Ent* aFirstEntry = this->aLeadEntry();
  if (aEntryToBeDeleted == aFirstEntry && m_pHigherBuffer) {
    EntBuffer* pHigherBuffer = _pEntBuffer(m_pHigherBuffer);
    pHigherBuffer->vDeleteUpwards(aDeleteKey,ppRootKeyBuffer);
  }
  
  vDeleteEntry(&aEntryToBeDeleted);
}

/*----------------------------------------------------------------------------*/
/* handle the insert of a first key                                           */
/*----------------------------------------------------------------------------*/
void EntBuffer::vHandleFirstInsert(Ent* aInsertedKeyEntry,
                                     Ent* aOriginalFirstEntry,
                                     EntBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  if (aLeadEntry() != aInsertedKeyEntry)
    return;
  
  if (aOriginalFirstEntry) {
    //Key* aOriginalFirstKey = _aKey(aOriginalFirstEntry->aDataArea());
    
    //Key* aInsertedFirstKey = _aKey(aInsertedKeyEntry->aDataArea());
    
    IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
    
    //if (aInsertedFirstKey->bNotEqual(aOriginalFirstKey,pIdxColVec))
    if (Key::bNotEqual(aInsertedKeyEntry, aOriginalFirstEntry))
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
void EntBuffer::vHandleLeftInsert(Ent* aInsertedKeyEntry,
                                    Ent* aOriginalFirstEntry,
                                    EntBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  if (! m_pHigherBuffer)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (aOriginalFirstEntry) {
    //Key* aDeleteKey = _aKey(aOriginalFirstEntry->aDataArea());
    EntBuffer* pHigherBuffer = _pEntBuffer(m_pHigherBuffer);
    //pHigherBuffer->vDeleteUpwards(aDeleteKey,ppRootKeyBuffer);
    pHigherBuffer->vDeleteUpwards(aOriginalFirstEntry,ppRootKeyBuffer);
    //(*ppRootKeyBuffer)->vShowTree();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //Key* aInsertKey = _aKey(aInsertedKeyEntry->aDataArea());
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  KeyWrk xKeyWrkIns;
  xKeyWrkIns.vSetKey(aInsertedKeyEntry, *nPoolBufferNo(), 0, pIdxColVec);
  
  EntBuffer* pHigherBuffer = _pEntBuffer(m_pHigherBuffer);
  pHigherBuffer->aInsertKeyBuffer(xKeyWrkIns,ppRootKeyBuffer);
}

/*----------------------------------------------------------------------------*/
/* get an insertable buffer                                                   */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pInsertableBuffer(KeyWrk& xKeyWrk,
                                            EntBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute required entry space
  
  RASSERT(xKeyWrk.nLevels());
  
  i8 nEntryBytes = xKeyWrk.nEntrySize();
  i8 nOffsetBytes = xKeyWrk.nOffsetSize();
  i8 nEntryPlusOffsetsBytes = nEntryBytes+nOffsetBytes;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert key into this EntBuffer if it has space or get a new one
  
  EntBuffer* pInsertIntoBuffer;
  if (bHasSpace(nEntryPlusOffsetsBytes))
    pInsertIntoBuffer = this;
  else {
    pInsertIntoBuffer =
      pNewInsertBuffer(xKeyWrk,nEntryPlusOffsetsBytes,ppRootKeyBuffer);
    //(*ppRootKeyBuffer)->vShow();
  }
  
  return pInsertIntoBuffer;
}

/*----------------------------------------------------------------------------*/
/* insert a key entry specifically into this buffer (should be enough space)  */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aInsertKeyAbs(KeyWrk& xKeyWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  Ent* aKeyEntry;
  
#ifdef USE_EB_LAZY_CHAIN
  if (m_pIndex->pTable()->bIsLoading()) {
    if (! m_iBufferHeight) {
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
  
  //vShow();
  
  return aKeyEntry;
}

/*----------------------------------------------------------------------------*/
/* insert a key entry specifically into this buffer (L0 chain only)           */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aInsertKeyAbsL0(KeyWrk& xKeyWrk, Ent** aaPriorEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
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
  
  Ent* aKeyEntry = aAllotEntry(nEntryBytes);
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
Ent* EntBuffer::aInsertKeyTree(KeyWrk& xKeyWrk,
                                  EntBuffer** ppRootKeyBuffer)
{
  // ** note: this is a static function **

  EntBuffer* pRootKeyBuffer = *ppRootKeyBuffer;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set a random number offsets
  xKeyWrk.vRandomLevels();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // position on an H0 buffer which we will try and insert into
  
  EntBuffer* pPositionedBuffer =
    pRootKeyBuffer->pPositionH0Buffer(xKeyWrk);
  
  ASSERT(0 == pPositionedBuffer->iBufferHeight());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert into the tree

  Ent* aInsertedKeyEntry =
    pPositionedBuffer->aInsertKeyUpwards(xKeyWrk,ppRootKeyBuffer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  return aInsertedKeyEntry;
}

/*----------------------------------------------------------------------------*/
/* insert a key entry specifically into this buffer (should be enough space)  */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aInsertKeyAbsLazy(KeyWrk& xKeyWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
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
  
  Ent* aKeyEntry = aAllotEntry(nEntryBytes);
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
void EntBuffer::vInsertKeySkip(Ent* aKeyEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the entry after which the new entry will be inserted
  
  Ent* xPriorVec[gpAttachMap->m_nIndexLevels];
  
  //Key* aKey = _aKey(aKeyEntry->aDataArea());
  
  if (0 == m_iBufferHeight)
    vPositionPriorVecEntry(aKeyEntry, xPriorVec, nullptr);
  else {
    EntBuffer* pToBuffer = EntBuffer::pRefdEntBuffer(aKeyEntry);
    vPositionPriorVecBuffer(aKeyEntry, xPriorVec, pToBuffer);
  }
  
  //vDumpPriorVec(xPriorVec);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set prior offsets
  
  i4 iKeyEntryOffset = iToOffset(aKeyEntry);
  
  i8 nLevels = aKeyEntry->nLevels();
  for (i8 i_Level = 0; i_Level < nLevels; i_Level++) {
    i4 iNextOffset = xPriorVec[i_Level]->iNEO(i_Level);
    aKeyEntry->vNEO(i_Level,iNextOffset);
    xPriorVec[i_Level]->vNEO(i_Level,iKeyEntryOffset);
  }
  
  //vShow();
}

/*----------------------------------------------------------------------------*/
/* insert a new key entry into this buffer                                    */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aInsertKeyBuffer(KeyWrk& xKeyWrk,
                                    EntBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent* aInsertedKeyEntry;
  
#ifdef USE_EB_LAZY_CHAIN
  if (m_pIndex->pTable()->bIsLoading()) {
    if (m_iBufferHeight) {
      Ent* aOriginalFirstEntry = aLeadEntry();
      aInsertedKeyEntry = aInsertKeyAbs(xKeyWrk);
      vHandleFirstInsert(aInsertedKeyEntry,aOriginalFirstEntry,ppRootKeyBuffer);
    }
    else {
      aInsertedKeyEntry = aInsertKeyAbs(xKeyWrk);
    }
  }
  else {
    Ent* aOriginalFirstEntry = aLeadEntry();
    aInsertedKeyEntry = aInsertKeyAbs(xKeyWrk);
    vHandleFirstInsert(aInsertedKeyEntry,aOriginalFirstEntry,ppRootKeyBuffer);
  }
#else
  Ent* aOriginalFirstEntry = aFirstEntry();
  pInsertedKeyEntry = pInsertKeyAbs(xKeyWrk);
  vHandleFirstInsert(pInsertedKeyEntry,pOriginalFirstEntry,ppRootKeyBuffer);
#endif
  
  return aInsertedKeyEntry;
}

/*----------------------------------------------------------------------------*/
/* insert a new key entry into this key tree (handling ripple upwards)        */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aInsertKeyUpwards(KeyWrk& xKeyWrk,
                                     EntBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  EntBuffer* pInsertIntoBuffer =
    pInsertableBuffer(xKeyWrk,ppRootKeyBuffer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent* aInsertedKeyEntry =
    pInsertIntoBuffer->aInsertKeyBuffer(xKeyWrk,ppRootKeyBuffer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pInsertIntoBuffer->m_iBufferHeight) {
    Id xId = aInsertedKeyEntry->xId();
    i8 nBuffer = xId.iBuffer();
    
    HeapBuffer* pHeapBuffer =
    reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(nBuffer));
    
    EntBuffer* pChildBuffer =
    reinterpret_cast<EntBuffer*>(pHeapBuffer->aFirstChunk());
    
    if (pChildBuffer->m_pHigherBuffer != pInsertIntoBuffer) {
      
      //(*ppRootKeyBuffer)->vShowTree();
      pChildBuffer->m_pHigherBuffer = pInsertIntoBuffer;
      //(*ppRootKeyBuffer)->vShowTree();
    }
  }
  
  return aInsertedKeyEntry;
}

/*----------------------------------------------------------------------------*/
/* is given key before the first key of this buffer                           */
/*----------------------------------------------------------------------------*/
bool EntBuffer::bKeyBeforeBuffer(KeyWrk& xKeyTest)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  Ent* aFirstEntry = this->aLeadEntry();
  
  bool bLessEqual = Key::bLessEqual(xKeyTest,aFirstEntry);
  
  return bLessEqual;
}

/*----------------------------------------------------------------------------*/
/* build skip lists from the lazy chain of all L0 buffers                     */
/*----------------------------------------------------------------------------*/
void EntBuffer::vLazyBuild(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  EntBuffer* pEntBuffer = this;
  
  while (pEntBuffer) {
    
    RASSERT(pEntBuffer->iBufferHeight() == 0);
    
    //vTrace("Building skip list %p\n"_Stz, pEntBuffer);
    
    pEntBuffer->vBuildSkipList();
    
    pEntBuffer = pEntBuffer->pNextBuffer();
  }
}

/*----------------------------------------------------------------------------*/
/* create a new higher buffer                                                 */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pNewHigherBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  // create new buffer at one higher index level
  EntBuffer* pNewBuffer = new EntBuffer();
  
  pNewBuffer->vIndex(m_pIndex);
  pNewBuffer->vBufferHeight(m_iBufferHeight+1);
  pNewBuffer->vPartitionNo(m_iPartitionNo);
  
  vHigherBuffer(pNewBuffer);
  
  return pNewBuffer;
}

/*----------------------------------------------------------------------------*/
/* create a new insert buffer (adjusting higher buffers accordingly)          */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pNewInsertBuffer(KeyWrk& xInsKey, i8 nEntryPlusOffsets,
                                           EntBuffer** ppRootKeyBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compact this buffer
  
  this->vCompact();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now check if it has enough space
  
  if (this->bHasSpace(nEntryPlusOffsets) )
    return this;
  
  EntBuffer* pLeftBuffer = this;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the higher buffer, creating one if it doesnt exist
  
  EntBuffer* pHigherBuffer = this->pHigherBuffer();
  if (! pHigherBuffer) {
    pHigherBuffer = this->pNewHigherBuffer();
    *ppRootKeyBuffer = pHigherBuffer;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // split the left buffer to yield a right buffer
  
  EntBuffer* pRightBuffer = pLeftBuffer->pSplitBuffer();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the first key of left buffer into higher buffer if higher buffer
  // was just created
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  if ( ! pHigherBuffer->nItems() ) {
    
    KeyWrk xKeyWrkLeft;
    Ent* aFirstKeyLeft  = pLeftBuffer->aLeadEntry();

    // initialize the keyWrk from the first left key
    xKeyWrkLeft.vSetKey(aFirstKeyLeft, *pLeftBuffer->nPoolBufferNo(), 0, pIdxColVec);
    
    Ent* aNewKeyLeft =
      pHigherBuffer->aInsertKeyUpwards(xKeyWrkLeft,ppRootKeyBuffer);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the first key of right buffer into higher buffer
  
  KeyWrk xKeyWrkRight;
  Ent* aFirstKeyRight = pRightBuffer->aLeadEntry();
  
  // initialize the keyWrk from the first left key
  xKeyWrkRight.vSetKey(aFirstKeyRight, *pRightBuffer->nPoolBufferNo(), 0, pIdxColVec);
  
  Ent* aNewKeyRight =
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
EntBuffer* EntBuffer::pNewNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  // create new buffer at the same index level and link the two buffers
  EntBuffer* pNewBuffer = new EntBuffer();
  
  pNewBuffer->vIndex(m_pIndex);
  pNewBuffer->vBufferHeight(m_iBufferHeight);
  pNewBuffer->vPartitionNo(m_iPartitionNo);
  
  pNewBuffer->vHigherBuffer(_pEntBuffer(m_pHigherBuffer));
  pNewBuffer->vPrevBuffer(this);
  pNewBuffer->vNextBuffer(_pEntBuffer(m_pNextBuffer));
  if (m_pNextBuffer) {
    EntBuffer* pNextBuffer = _pEntBuffer(m_pNextBuffer);
    pNextBuffer->vPrevBuffer(pNewBuffer);
  }
  
  m_pNextBuffer = pNewBuffer;
  
  ASSERT(this == pNewBuffer->pPrevBuffer());
  
  return pNewBuffer;
}


/*----------------------------------------------------------------------------*/
/* split this buffer into two: this original buffer and a new right buffer    */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pSplitBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  i8 iSplitRatio = 50;
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  EntBuffer* pThisLeftBuffer = this;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a new right buffer
  
  EntBuffer* pNewRightBuffer = pNewNextBuffer();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find split entry. every entry from split entry and to the right of it
  // will be moved to the new right buffer
  
  Ent* aPriorEntry;
  //i8 iSplitAtEntry = m_nItems/2;
  i8 iSplitAtEntry = (m_nItems * iSplitRatio) / 100;
  Ent* aSplitEntry = aGetEntrySplit(iSplitAtEntry,&aPriorEntry);
  ASSERT(this->aNextEntry(aPriorEntry,0) == aSplitEntry);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy split right entries to a new right buffer
  
  KeyWrk xKeyWrk;
  Ent* aCopyEntry;
  Ent* aNextEntry;
  
  Ent* aLastEntryRight = pNewRightBuffer->aSkipRoots();
  
  i8 nEntriesCopied = 0;
  for (aCopyEntry = aSplitEntry;
       aCopyEntry;
       aCopyEntry = aNextEntry) {
    
    ASSERT(! m_iBufferHeight ||
           EntBuffer::pRefdEntBuffer(aCopyEntry)->m_pHigherBuffer == pThisLeftBuffer);
    
    nEntriesCopied++;
    
    aNextEntry = this->aNextEntry(aCopyEntry,0);
    
    // form the key
    Id xId = aCopyEntry->xId();
    xKeyWrk.vSetKey(aCopyEntry, xId.iBuffer(), xId.iItem(), pIdxColVec);
    
    Ent* aNewEntry = pNewRightBuffer->aInsertKeyAbsL0(xKeyWrk,&aLastEntryRight);
    
    //.pNewRightBuffer->vShow();
    
    // make the moved entry in the left buffer to be a free entry
    FreeItem* aFreeEntry = aMakeFreeEntry(aCopyEntry);
    
    // these can be commented
    ASSERT(! m_iBufferHeight ||
           EntBuffer::pRefdEntBuffer(aNewEntry)->m_pHigherBuffer == pThisLeftBuffer);
    
    i8 nBuffer = xId.iBuffer();
    HeapBuffer* pHeapBuffer =
    reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(nBuffer));
    
    if (m_iBufferHeight) {
      EntBuffer* pChildBuffer =
      reinterpret_cast<EntBuffer*>(pHeapBuffer->aFirstChunk());
      
      ASSERT(pChildBuffer->m_pHigherBuffer ==
             EntBuffer::pRefdEntBuffer(aNewEntry)->m_pHigherBuffer);
    }
    else {
      RowBuffer* pChildBuffer =
      reinterpret_cast<RowBuffer*>(pHeapBuffer->aFirstChunk());
      RASSERT(pChildBuffer);
    }
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
  
  pThisLeftBuffer->vCompactLeft();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // the child buffer of any moved entry needs to have its higher buffer
  // corrected to the new right buffer
  
  Id xId;
  if (m_iBufferHeight != 0)  {
    for (Ent* aMovedEntry = pNewRightBuffer->aLeadEntry();
         aMovedEntry;
         aMovedEntry = pNewRightBuffer->aNextEntry(aMovedEntry, 0)) {
      
      xId = aMovedEntry->xId();
      i8 nBuffer = xId.iBuffer();
      
      HeapBuffer* pHeapBuffer =
      reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(nBuffer));
      
      EntBuffer* pChildBuffer =
      reinterpret_cast<EntBuffer*>(pHeapBuffer->aFirstChunk());
      
      ASSERT(pChildBuffer->m_pHigherBuffer ==
             EntBuffer::pRefdEntBuffer(aMovedEntry)->m_pHigherBuffer);
      
      
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
/* allot an offset array in this buffer                                       */
/*----------------------------------------------------------------------------*/
NEO* EntBuffer::aAllotNEO(i8 nBytes)
{
  NEO* aNEO = reinterpret_cast<NEO*>(ayAllotBytes(nBytes));
  
  return aNEO;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
