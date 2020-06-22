/*============================================================================*/
/* EntBuffer_insert.cpp -- insert related functions for Entry Buffer          */
/*============================================================================*/

#include "Indexing.h"



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
    
    Ent* aEntry = aLazyChain;
    
    i8 iNextOffset = aEntry->iNEO(0);
    if (-1 == iNextOffset)
      aLazyChain = nullptr;
    else
      aLazyChain = aGetEntryOffset(iNextOffset);
    
    vInsertEntSkipList(aEntry);
  }
  
  Ent* aRevisedEntry = aNextEntry(aSkipRoots(),0);
  
  vHandleFirstInsert(aRevisedEntry,aOriginalFirstEntry,nullptr);
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
   for (i8 i_Level = 1; i_Level < nOffsets; i_Level++)
     aEnt->vNEO(i_Level,-1);
   }
   vShow();
   */
  
  i8 nLvlLevels = AM::nLvlLevels();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent* xPrevVec[C_MAX_LVL_LEVELS];
  
  for (i8 i_Offset = 0; i_Offset < nLvlLevels; i_Offset++)
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
    i8 nLevels = aEnt->nOffsets();
    
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
  
#ifdef WITH_EB_LAZY_CHAIN
  //RASSERT( ! m_aLazyChain );
  
  if (m_aLazyChain)
    vBuildSkipList();
#endif
  
  i8 nLvlLevels = AM::nLvlLevels();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  FreeItem* aFreeItem;
  Ent*     aMoveEntry;
  Ent*     xPriorVec[C_MAX_LVL_LEVELS];
  
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
    
    //aKey->vShow(pIndexDef);
    //vTrace("\n"_Stz);
    
    vPositionPriorVecEntry(aMoveEntry->aKey(), xPriorVec, aMoveEntry);
    
    if (m_iBufferHeight) {
      //vDumpPriorVec(xPriorVec);
      int iDebug = 1;
    }
    
    i4 iWasOffset = iToOffset(aMoveEntry);
    i4 iNowOffset = iToOffset((Ent*)aFreeItem);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // now copy move entry left onto free entry
    
    // calculate the number of bytes to copy
    i8 nCopyBytes = aMoveEntry->nSizeBytes();
    
    // copy the move entry over the free entry
    MemLib::vMove(aFreeItem, aMoveEntry, nCopyBytes, CN_ENTBUFFER);
    
    // give the reclaimed space back to the buffer
    i8 nReclaimedBytes = nDifference(aMoveEntry,aFreeItem);
    //vGiveSpace(nReclaimedBytes);
    
    // convert the moved entry to a free entry
    FreeItem* aOpenedFreeEntry = aAdd<FreeItem*>(aFreeItem,nCopyBytes);
    new(aOpenedFreeEntry) FreeItem(nReclaimedBytes);
    
    Ent* aCheckEntry = reinterpret_cast<Ent*>(aFreeItem);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the prior vector to the entry being moved
    
    //vDumpPriorVec(xPriorVec);
    for (i8 i_Level = 0; i_Level < nLvlLevels; i_Level++) {
      Ent* aPriorEntry = xPriorVec[i_Level];
      i4 iPriorOffset = aPriorEntry->iNEO(i_Level);
      if (iWasOffset == iPriorOffset)
        aPriorEntry->vNEO(i_Level,iNowOffset);
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
  
#ifdef WITH_EB_LAZY_CHAIN
  RASSERT(! m_aLazyChain);
#endif
  
  EntBuffer* pThisBuffer = this;
  EntBuffer* pTempBuffer = new EntBuffer();
  
  IndexDef* pIndexDef = m_pIndex->pIndexDef();
  
  EntWrk xEntWrk;
  
  i8 nLvlLevels = AM::nLvlLevels();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent* aPriorEntry = pTempBuffer->aSkipRoots();
  
  for (Ent* aThisEntry = pThisBuffer->aLeadEntry();
       aThisEntry != nullptr;
       aThisEntry = pThisBuffer->aNextEntry(aThisEntry, 0)) {
    
    Ref xRef = aThisEntry->xRef();
    
    xEntWrk.vSetEnt(xRef.iBuffer(), xRef.iItem(), aThisEntry);
    pTempBuffer->aInsertEntAbsL0(xEntWrk.aEnt(),&aPriorEntry);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent* aThisWilderness = aItemArea();
  vSetUsed(aThisWilderness);
  
  vItems(0);
  
  for (i8 i_Level = 0; i_Level < nLvlLevels; i_Level++)
    aSkipRoots()->vNEO(i_Level,-1);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  aPriorEntry = pThisBuffer->aSkipRoots();
  
  for (Ent* aTempEntry = pTempBuffer->aLeadEntry();
       aTempEntry != nullptr;
       aTempEntry = pTempBuffer->aNextEntry(aTempEntry, 0)) {
    
    Ref xRef = aTempEntry->xRef();
    
    xEntWrk.vSetEnt(xRef.iBuffer(), xRef.iItem(), aTempEntry);
    pThisBuffer->aInsertEntAbsL0(xEntWrk.aEnt(),&aPriorEntry);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pThisBuffer->vBuildSkipListSorted();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete pTempBuffer;
}

/*----------------------------------------------------------------------------*/
/* delete upwards                                                             */
/*----------------------------------------------------------------------------*/
void EntBuffer::vDeleteUpwards(Ent* aDeleteEntry, EntBuffer** ppRootBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
#ifdef WITH_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent* aPriorEntry = aPositionPriorEntry(aDeleteEntry->aKey());
  if (!aPriorEntry)
    return;
  
  Ent* aEntryToBeDeleted = aNextEntry(aPriorEntry,0);
  if (!aEntryToBeDeleted)
    return;
  
  //  vTrace("[%lld] pDeleteEntry key %lld\n"_Stz,
  //         *nPoolBufferNo(),
  //         pEntryToBeDeleted->aKey()->iGetInt());
  
  //Key* aDeleteEntryKey = _aKey(aEntryToBeDeleted->ayElmSeq());
  
  //IndexDef* pIndexDef = m_pIndex->pIndexDef();
  
  //if ( ! aDeleteEntryKey->bEqualTo(aDeleteKey,pIndexDef) ) {
  if ( ! Key::bEqualTo(aEntryToBeDeleted->aKey(), aDeleteEntry->aKey()) ) {
    //vShow();
    
    //vShowTree();
    
    //vTrace("Delete key: "_Stz);
    //aDeleteKey->vShow(pIndexDef);
    //vTrace("\n"_Stz);
    
    //vTrace("Delete entry key found: "_Stz);
    //aDeleteEntryKey->vShow(pIndexDef);
    //vTrace("\n"_Stz);
    
    return;
  }
  //RASSERT(aDeleteEntryKey->bEqualTo(aDeleteKey,pIndexDef));
  
  Ent* aFirstEntry = this->aLeadEntry();
  if (aEntryToBeDeleted == aFirstEntry && m_pHigherBuffer) {
    EntBuffer* pHigherBuffer = _pEntBuffer(m_pHigherBuffer);
    pHigherBuffer->vDeleteUpwards(aDeleteEntry,ppRootBuffer);
  }
  
  vDeleteEntry(&aEntryToBeDeleted);
}

/*----------------------------------------------------------------------------*/
/* handle the insert of a first key                                           */
/*----------------------------------------------------------------------------*/
void EntBuffer::vHandleFirstInsert(Ent* aInsertedKeyEntry,
                                   Ent* aOriginalFirstEntry,
                                   EntBuffer** ppRootBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  if (aLeadEntry() != aInsertedKeyEntry)
    return;
  
  if (aOriginalFirstEntry) {
    //Key* aOriginalFirstKey = _aKey(aOriginalFirstEntry->aDataArea());
    
    //Key* aInsertedFirstKey = _aKey(aInsertedKeyEntry->aDataArea());
    
    IndexDef* pIndexDef = m_pIndex->pIndexDef();
    
    //if (aInsertedFirstKey->bNotEqual(aOriginalFirstKey,pIndexDef))
    if (Key::bNotEqual(aInsertedKeyEntry->aKey(), aOriginalFirstEntry->aKey()))
      vHandleLeftInsert(aInsertedKeyEntry,
                        aOriginalFirstEntry,
                        ppRootBuffer);
  }
  else {
    vHandleLeftInsert(aInsertedKeyEntry,
                      aOriginalFirstEntry,
                      ppRootBuffer);
  }
}

/*----------------------------------------------------------------------------*/
/* handle the insert of a left most key                                       */
/*----------------------------------------------------------------------------*/
void EntBuffer::vHandleLeftInsert(Ent* aInsertedKeyEntry,
                                  Ent* aOriginalFirstEntry,
                                  EntBuffer** ppRootBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  if (! m_pHigherBuffer)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (aOriginalFirstEntry) {
    EntBuffer* pHigherBuffer = _pEntBuffer(m_pHigherBuffer);
    pHigherBuffer->vDeleteUpwards(aOriginalFirstEntry,ppRootBuffer);
    //(*ppRootBuffer)->vShowTree();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //Key* aInsertEnt = _aKey(aInsertedKeyEntry->aDataArea());
  IndexDef* pIndexDef = m_pIndex->pIndexDef();
  EntWrk xEntWrkIns;
  xEntWrkIns.vSetEnt(iBufferNo(), 0, aInsertedKeyEntry);
  
  EntBuffer* pHigherBuffer = _pEntBuffer(m_pHigherBuffer);
  pHigherBuffer->aInsertEntBuffer(xEntWrkIns.aEnt(),ppRootBuffer);
}

/*----------------------------------------------------------------------------*/
/* get an insertable buffer                                                   */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pInsertableBuffer(Ent* aEnt, EntBuffer** ppRootBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute required entry space
  
  RASSERT(aEnt->nOffsets());
  
  i8 nEntryBytes  = aEnt->nEntKeySize();
  i8 nOffsetBytes = aEnt->nNEOsize();
  i8 nEntryPlusOffsets = nEntryBytes + nOffsetBytes;
  RASSERT(nEntryPlusOffsets == aEnt->nSizeBytes());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert key into this EntBuffer if it has space or get a new one
  
  EntBuffer* pInsertIntoBuffer;
  if (bHasSpace(nEntryPlusOffsets))
    pInsertIntoBuffer = this;
  else {
    pInsertIntoBuffer =
      pNewInsertBuffer(aEnt,nEntryPlusOffsets,ppRootBuffer);
    //(*ppRootBuffer)->vShow();
  }
  
  return pInsertIntoBuffer;
}

/*----------------------------------------------------------------------------*/
/* insert a key entry specifically into this buffer (should be enough space)  */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aInsertEntAbs(Ent* aEnt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
      
#ifdef WITH_EB_LAZY_CHAIN
  if (m_pIndex->pTable() && m_pIndex->pTable()->bIsLoading()) {
    if (! m_iBufferHeight) {
      aInsertedEntry = aInsertEntAbsLazy(aEntryWork);
      return aInsertedEntry;
    }
  }
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // trace
  
  //vTrace("Inserting key: "_Stz);
  //IndexDef* pIndexDef = m_pIndex->pIndexDef();
  //xEntWrk.aKey()->vShow(pIndexDef);
  //vTrace("\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute required entry space
  
  ASSERT(aEnt->nOffsets());
  
  i8 nEntryPlusOffsets = aEnt->nSizeBytes();
  
  ASSERT(bHasSpace(nEntryPlusOffsets));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate space for the key and next offsets in this buffer
  
  Ent* aInsertedEntry = aAllotEntry(nEntryPlusOffsets);
  ASSERT(aInsertedEntry);

//  aInsertedEntry = aAllotEntry(nEntryBytes);
//  ASSERT(aInsertedEntry);
//
//  NEO* aNEO = aAllotNEO(nOffsetBytes);
//  ASSERT(aNEO);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the new entry
  
  aInsertedEntry->vSetEntry(aEnt);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the new entry into the skip list
  
  vInsertEntSkipList(aInsertedEntry);
  
  //vShow();
  
  return aInsertedEntry;
}

/*----------------------------------------------------------------------------*/
/* insert a key entry specifically into this buffer (L0 chain only)           */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aInsertEntAbsL0(Ent* aEnt, Ent** aaPriorEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // trace
  
  //vTrace("Inserting key:\n"_Stz);
  //IndexDef* pIndexDef = m_pIndex->pIndexDef();
  //aKey->vShow(pIndexDef);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute required entry space
  
  ASSERT(aEnt->nOffsets());
  
  i8 nEntryPlusOffsets = aEnt->nSizeBytes();

  ASSERT(bHasSpace(nEntryPlusOffsets));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate space for the key and next offsets in this buffer
  
  Ent* aInsertedEntry = aAllotEntry(nEntryPlusOffsets);
  ASSERT(aInsertedEntry);
  
//  Ent* aInsertedEntry = aAllotEntry(nEntryBytes);
//  ASSERT(aInsertedEntry);
//
//  NEO* aNEO = aAllotNEO(nOffsetBytes);
//  ASSERT(aNEO);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the new entry
  
  aInsertedEntry->vSetEntry(aEnt);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append to prior entry at L0 only
  
  i4 iOffset = iToOffset(aInsertedEntry);
  
  (*aaPriorEntry)->vNEO(0,iOffset);
  
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  // for the moment we set offsets in prior entry to -1
  
  i8 nPriorLevels = aInsertedEntry->nOffsets();
  for (i8 i_Level = 0; i_Level < nPriorLevels; i_Level++)
    aInsertedEntry->vNEO(i_Level,-1);
  
  // ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
  
  *aaPriorEntry = aInsertedEntry;
  
  return aInsertedEntry;
}

/*----------------------------------------------------------------------------*/
/* insert a new key entry into this key tree                                  */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aInsertEntTree(Ent* aEnt, EntBuffer** ppRootEntBuffer)
{
  _static_

  EntBuffer* pRootEntBuffer = *ppRootEntBuffer;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set a random number offsets
  i8 nOffsets = AM::nRndSkipLevel();

  aEnt->vOffsets(nOffsets);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // position on an H0 buffer which we will try and insert into
  
  EntBuffer* pPositionedH0EntBuffer =
    pRootEntBuffer->pPositionH0Buffer(aEnt->aKey());
  
  ASSERT(0 == pPositionedH0EntBuffer->iBufferHeight());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert into the tree

  Ent* aInsertedEnt =
    pPositionedH0EntBuffer->aInsertEntUpwards(aEnt,ppRootEntBuffer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  return aInsertedEnt;
}

/*----------------------------------------------------------------------------*/
/* insert a key entry specifically into this buffer (should be enough space)  */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aInsertEntAbsLazy(Ent* aEnt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // trace
  
  //vTrace("Inserting key:\n"_Stz);
  //IndexDef* pIndexDef = m_pIndex->pIndexDef();
  //aKey->vShow(pIndexDef);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute required entry space
    
  ASSERT(aEnt->nOffsets());
  
  i8 nEntryPlusOffsets = aEnt->nSizeBytes();

  ASSERT(bHasSpace(nEntryPlusOffsets));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate space for the key and next offsets in this buffer
  
  Ent* aInsertedEntry = aAllotEntry(nEntryPlusOffsets);
  ASSERT(aInsertedEntry);
  
//  Ent* aInsertedEntry = aAllotEntry(nEntryBytes);
//  ASSERT(aInsertedEntry);
//
//  NEO* aNEO = aAllotNEO(nOffsetBytes);
//  ASSERT(aNEO);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the new entry
  
  aInsertedEntry->vSetEntry(aEnt);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form lazy chain
  
  if (m_aLazyChain) {
    i4 iLazyOffset = iToOffset(m_aLazyChain);
    aInsertedEntry->vNEO(0,iLazyOffset);
  }
  else {
    aInsertedEntry->vNEO(0,-1);
  }
  
  m_aLazyChain = aInsertedEntry;
  
  return aInsertedEntry;
}

/*----------------------------------------------------------------------------*/
/* insert a new key entry into this buffer                                    */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aInsertEntBuffer(Ent* aEntryWork, EntBuffer** ppRootBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent* aInsertedEntry;
  
#ifdef WITH_EB_LAZY_CHAIN
  if (m_pIndex->pTable()->bIsLoading()) {
    if (m_iBufferHeight) {
      Ent* aOriginalFirstEntry = aLeadEntry();
      aInsertedEntry = aInsertEntAbs(aEntryWork);
      vHandleFirstInsert(aInsertedEntry,aOriginalFirstEntry,ppRootBuffer);
    }
    else {
      aInsertedEntry = aInsertEntAbs(aEntryWork);
    }
  }
  else {
    Ent* aOriginalFirstEntry = aLeadEntry();
    aInsertedEntry = aInsertEntAbs(aEntryWork);
    vHandleFirstInsert(aInsertedEntry,aOriginalFirstEntry,ppRootBuffer);
  }
#else
  Ent* aOriginalFirstEntry = aLeadEntry();
  aInsertedEntry = aInsertEntAbs(aEntryWork);
  vHandleFirstInsert(aInsertedEntry,aOriginalFirstEntry,ppRootBuffer);
#endif
  
  return aInsertedEntry;
}

/*----------------------------------------------------------------------------*/
/* insert given entry into skip list of this buffer                           */
/*----------------------------------------------------------------------------*/
void EntBuffer::vInsertEntSkipList(Ent* aEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the entry after which the new entry will be inserted
  
  Ent* xPriorVec[C_MAX_LVL_LEVELS];
  
  //Key* aKey = _aKey(aKeyEntry->aDataArea());
  
  if (0 == m_iBufferHeight)
    vPositionPriorVecEntry(aEntry->aKey(), xPriorVec, nullptr);
    //vPositionPriorVecEntry_Alka01(aEntry->aKey(), xPriorVec, nullptr);
    //vPositionPriorVecEntry_Ray02(aEntry->aKey(), xPriorVec);
    //vPositionPriorVecEntry_Ray03(aEntry->aKey(), xPriorVec);
  else {
    EntBuffer* pToBuffer = EntBuffer::pRefdEntBuffer(aEntry);
    vPositionPriorVecBuffer(aEntry->aKey(), xPriorVec, pToBuffer);
  }
  
  //vDumpPriorVec(xPriorVec);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set prior offsets
  
  i4 iKeyEntryOffset = iToOffset(aEntry);
  
  i8 nLevels = aEntry->nOffsets();
  for (i8 i_Level = 0; i_Level < nLevels; i_Level++) {
    i4 iNextOffset = xPriorVec[i_Level]->iNEO(i_Level);
    aEntry->vNEO(i_Level,iNextOffset);
    xPriorVec[i_Level]->vNEO(i_Level,iKeyEntryOffset);
  }
  
  //vShow();
}

/*----------------------------------------------------------------------------*/
/* insert a new key entry into this key tree (handling ripple upwards)        */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aInsertEntUpwards(Ent* aEntryWork, EntBuffer** ppRootBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  EntBuffer* pInsertIntoBuffer =
    pInsertableBuffer(aEntryWork,ppRootBuffer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent* aInsertedEntry =
    pInsertIntoBuffer->aInsertEntBuffer(aEntryWork,ppRootBuffer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pInsertIntoBuffer->m_iBufferHeight) {
    Ref xRef = aInsertedEntry->xRef();
    i8 i_Buffer = xRef.iBuffer();
    
    HeapBuffer* pHeapBuffer =
    reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(i_Buffer));
    
    EntBuffer* pChildBuffer =
    reinterpret_cast<EntBuffer*>(pHeapBuffer->aFirstChunk());
    
    if (pChildBuffer->m_pHigherBuffer != pInsertIntoBuffer) {
      
      //(*ppRootBuffer)->vShowTree();
      pChildBuffer->m_pHigherBuffer = pInsertIntoBuffer;
      //(*ppRootBuffer)->vShowTree();
    }
  }
  
  return aInsertedEntry;
}

/*----------------------------------------------------------------------------*/
/* is given key before the first key of this buffer                           */
/*----------------------------------------------------------------------------*/
bool EntBuffer::bKeyBeforeBuffer(Key* aKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  Ent* aFirstEntry = this->aLeadEntry();
  
  bool bLessEqual = Key::bLessEqual(aKey,aFirstEntry->aKey());
  
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
EntBuffer* EntBuffer::pNewInsertBuffer(Ent* aEntryWork,
                                       i8 nEntryPlusOffsets,
                                       EntBuffer** ppRootBuffer)
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
    *ppRootBuffer = pHigherBuffer;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // split the left buffer to yield a right buffer
  
  EntBuffer* pRightBuffer = pLeftBuffer->pSplitBuffer();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the first key of left buffer into higher buffer if higher buffer
  // was just created
  
  if ( ! pHigherBuffer->nItems() ) {
    
    EntWrk xEntWrkLeft;
    Ent* aFirstEntryLeft  = pLeftBuffer->aLeadEntry();

    // initialize the keyWrk from the first left key
    xEntWrkLeft.vSetEnt(pLeftBuffer->iBufferNo(), 0, aFirstEntryLeft);
    
    Ent* aInsertedEntryLeft =
      pHigherBuffer->aInsertEntUpwards(xEntWrkLeft.aEnt(),ppRootBuffer);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the first key of right buffer into higher buffer
  
  EntWrk xEntWrkRight;
  Ent* aFirstEntryRight = pRightBuffer->aLeadEntry();
  
  // initialize the keyWrk from the first left key
  xEntWrkRight.vSetEnt(pRightBuffer->iBufferNo(), 0, aFirstEntryRight);
  
  Ent* aInsertedEntryRight =
    pHigherBuffer->aInsertEntUpwards(xEntWrkRight.aEnt(),ppRootBuffer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //ASSERT( (* ppRootBuffer)->bVerify() );
  
  if ( pRightBuffer->bKeyBeforeBuffer(aEntryWork->aKey()) )
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
/* purge upwards                                                              */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPurgeUpwards(Ent* aDeleteEntry, EntBuffer** ppRootBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
#ifdef WITH_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // position to entry prior to the entry to be deleted
  Ent* aPriorEntry = aPositionPriorEntry(aDeleteEntry->aKey());
  if (!aPriorEntry)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now get the entry to be deleted
  Ent* aEntryToBeDeleted = this->aNextEntry(aPriorEntry,0);
  if (!aEntryToBeDeleted)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // does the aEntryToBeDeleted match the aDeleteEntry ?
  if ( ! Key::bEqualTo(aEntryToBeDeleted->aKey(), aDeleteEntry->aKey()) ) {
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the firstEntry and the next entry
  Ent* aFirstEntry = this->aLeadEntry();
  Ent* aEntNext = aNextEntry(aEntryToBeDeleted, 0);
  EntBuffer* pHigherBuffer = _pEntBuffer(m_pHigherBuffer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is the entry to be deleted the first entry and we have a higher buffer ?
  if (aEntryToBeDeleted == aFirstEntry && pHigherBuffer) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // if there is a next entry then insert the next entry to the higher buffers
    if ( aEntNext ) {
      EntWrk xEntWrkIns;
      xEntWrkIns.vSetEnt(m_iBufferNo, 0, aEntNext);
      pHigherBuffer->aInsertEntUpwards(xEntWrkIns.aEnt(),ppRootBuffer);
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // now purge the deleteEntry upwards
    pHigherBuffer->vPurgeUpwards(aDeleteEntry,ppRootBuffer);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete the entry to be deleted from this buffer
  vDeleteEntry(&aEntryToBeDeleted);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is this buffer empty ?
  if (nItems() == 0) {
    
    // unlink this buffer
    vUnLink();
    
    // delete this buffer
    delete this;
    
    // ws this biuffer the root ?
    if (*ppRootBuffer == this)
      *ppRootBuffer = nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* split this buffer into two: this original buffer and a new right buffer    */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pSplitBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  i8 iSplitRatio = 50;
  
#ifdef WITH_EB_LAZY_CHAIN
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
  
  EntWrk xEntWrk;
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
    Ref xRef = aCopyEntry->xRef();
    xEntWrk.vSetEnt(xRef.iBuffer(), xRef.iItem(), aCopyEntry);
    
    Ent* aNewEntry = pNewRightBuffer->aInsertEntAbsL0(xEntWrk.aEnt(),&aLastEntryRight);
    
    //.pNewRightBuffer->vShow();
    
    // make the moved entry in the left buffer to be a free entry
    FreeItem* aFreeEntry = aMakeFreeEntry(aCopyEntry);
    
    // these can be commented
    ASSERT(! m_iBufferHeight ||
           EntBuffer::pRefdEntBuffer(aNewEntry)->m_pHigherBuffer == pThisLeftBuffer);
    
    i8 i_Buffer = xRef.iBuffer();
    HeapBuffer* pHeapBuffer =
    reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(i_Buffer));
    
    if (m_iBufferHeight) {
      EntBuffer* pChildBuffer =
      reinterpret_cast<EntBuffer*>(pHeapBuffer->aFirstChunk());
      
      ASSERT(pChildBuffer->m_pHigherBuffer ==
             EntBuffer::pRefdEntBuffer(aNewEntry)->m_pHigherBuffer);
    }
    else {
      CrwBuffer* pChildBuffer =
      reinterpret_cast<CrwBuffer*>(pHeapBuffer->aFirstChunk());
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
  
  Ref xRef;
  if (m_iBufferHeight != 0)  {
    for (Ent* aMovedEntry = pNewRightBuffer->aLeadEntry();
         aMovedEntry;
         aMovedEntry = pNewRightBuffer->aNextEntry(aMovedEntry, 0)) {
      
      xRef = aMovedEntry->xRef();
      i8 i_Buffer = xRef.iBuffer();
      
      HeapBuffer* pHeapBuffer =
      reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(i_Buffer));
      
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
//NEO* EntBuffer::aAllotNEO(i8 nBytes)
//{
//  NEO* aNEO = reinterpret_cast<NEO*>(ayAllotBytes(nBytes));
//
//  return aNEO;
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
