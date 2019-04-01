/*============================================================================*/
/* EntryBuffer_verify.cpp -- verify related functions for Entry Buffer        */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
    
    for (i8 i_ScanLevel = aTestEntry->nLevels()-1; i_ScanLevel >= 0; i_ScanLevel--) {
      Item* aCheckEntry = aNextEntry(aTestEntry,i_ScanLevel);
      if (! aCheckEntry)
        continue;
      
      if (CN_ENTRY != aCheckEntry->eClass())
        vShow();
      ASSERT(CN_ENTRY == aCheckEntry->eClass());
    }
    
  }
  
  //  ln i_ScanLevel;
  //  for (i_ScanLevel = 2; *i_ScanLevel >= 0; i_ScanLevel--) {
  //
  //    for (Item* aTestEntry = aNextEntry(aSkipRoots(),*i_ScanLevel);
  //         pTestEntry;
  //         pTestEntry = aNextEntry(pTestEntry,*i_ScanLevel)) {
  //
  //      if (pSeeEntry) {
  //        vTrace("|scan: %lld| "_Stz,*i_ScanLevel);
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
    
    for (i8 i_ScanLevel = aTestEntry->nLevels()-1; i_ScanLevel >= 0; i_ScanLevel--) {
      
      Item* aCheckEntry = aNextEntry(aTestEntry,i_ScanLevel);
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
/* verify the structure of this buffer (and its children)                     */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  bool bValid = false;
  
  if (m_iBufferHeight) {
    bValid = bVerifyLnAny();
    return bValid;
  }
  
  bValid = bVerifyL0Other();
  return bValid;
}

/*----------------------------------------------------------------------------*/
/* verify the structure of this L0 buffer                                     */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyL0Other(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  RASSERT(0 == m_iBufferHeight);
  
  //  vTrace("VerifyL0 %s: bufno: %lld level %lld\n"_Stz,
  //         m_pIndex->aIndexName(),m_iBufferNo,m_iBufferHeight);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify the contents of this L0 index buffer
  
  bool bValid;
  
  bValid = bVerifyNextPrev();
  if (! bValid)
    return false;
  
  bValid = bVerifyItems();
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
  
  RASSERT(0 != m_iBufferHeight);
  
  //  vTrace("VerifyLn %s: bufno: %lld level %lld\n"_Stz,
  //         m_pIndex->aIndexName(),m_iBufferNo,m_iBufferHeight);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify the contents of this buffer
  
  bool bValid;
  
  bValid = bVerifyNextPrev();
  if (! bValid)
    return false;
  
  bValid = bVerifyItems();
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
    if (pRefEntBuf->m_iBufferHeight) {
      bValid = pRefEntBuf->bVerifyLnAny();
    }
    else {
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
  
  RASSERT(m_iBufferHeight >= 1);
  
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
  
  RASSERT(m_iBufferHeight >= 1);
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
    
    EntryBuffer* pChildEntBuf = EntryBuffer::pRefdEntryBuffer(aItem);
    
    if ( ! pChildEntBuf->bValidEntryBuffer() ) {
      vVerifyFault();
      return false;
    }
    
    if (pChildEntBuf->iBufferHeight() != m_iBufferHeight - 1) {
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
/* verify the first key order of child buffers                                */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyFirstKeys(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   The for each entry in an Ln buffer will match the first key in an
   Ln-1 buffer
   */
  
  RASSERT(m_iBufferHeight >= 1);
  
  //vShow();
  
  //IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
    
    // get the entry buffer that this entry references
    EntryBuffer* pChildEntryBuffer = EntryBuffer::pRefdEntryBuffer(aItem);
    
    //pChildEntryBuffer->vShow();
    
    
    // get the first entry of the child buffer
    Item* aChildFirstEntry = pChildEntryBuffer->aLeadEntry();
    
    // get keys of the two entries
    //KeyData* aKeyParent = _aKeyData(aItem->aDataArea());
    //KeyData* aKeyChild  = _aKeyData(aChildFirstEntry->aDataArea());
    
    // the two keys should match
    //bool bEqual = aKeyChild->bEqualTo(aKeyParent,pIdxColVec);
    bool bEqual = KeyData::bEqualTo(aChildFirstEntry, aItem);
    if (! bEqual) {
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
  
  RASSERT(m_iBufferHeight >= 1);
  
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
      vDump();
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
   Any Ln buffer or L0 buffer should have its keys in ascending
   order.
   */
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
    
    // get next entry
    Item* aNextEntry = this->aNextEntry(aItem,0);
    if (! aNextEntry)
      break;
    
    // get keys of the two entries
    //KeyData* aKeyA = _aKeyData(aItem->aDataArea());
    //KeyData* aKeyB = _aKeyData(aNextEntry->aDataArea());
    
    // next keyB should be greater than or equal to keyA
    //bool bGreaterEqual = aKeyB->bGreaterEqual(aKeyA,pIdxColVec);
    bool bGreaterEqual = KeyData::bGreaterEqual(aNextEntry, aItem);
    if (! bGreaterEqual) {
      vDump();
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
    if (pPrevBuffer->iBufferHeight() != m_iBufferHeight) {
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
    if (pNextBuffer->iBufferHeight() != m_iBufferHeight) {
      vVerifyFault();
      return false;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_pHigherBuffer) {
    EntryBuffer* pHigherBuffer = _pEntryBuffer(m_pHigherBuffer);
    if (pHigherBuffer->iBufferHeight() != m_iBufferHeight + 1) {
      vVerifyFault();
      return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the row references in this L0 index buffer                          */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerifyRowRefs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   For this L0 index verify that its key entries referen a
   valid row in a row buffer
   */
  
  RASSERT(0 == m_iBufferHeight);
  
  for (Item* aItem = aLeadEntry(); aItem; aItem = aNextEntry(aItem,0)) {
    
    // get the row buffer that this L0 key leaf references
    Id xId = aItem->xId();
    RowBuffer* pRowBuffer = RowBuffer::pRefdRowBuffer(xId);
    
    // get the row entry from referenced entry buffer
    i8 i_Item = aItem->iItemNo();
    CacheLine* aRowData = pRowBuffer->pRow(i_Item);
    
    if (! aRowData ) {
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
  
  for (i8 i_ScanLevel = gpAttachMap->m_nIndexLevels - 1;
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
        aTestKey->vShow(pIdxColVec);
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
    i8 nOffsets = aTestEntry_0->nLevels();
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
  RASSERT(0);
  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
