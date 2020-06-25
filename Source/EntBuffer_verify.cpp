/*============================================================================*/
/* EntBuffer_verify.cpp -- verify related functions for Entry Buffer          */
/*============================================================================*/

#include "Indexing.h"


/*----------------------------------------------------------------------------*/
/*-- Verify ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* check prior vec ordering                                                   */
/*----------------------------------------------------------------------------*/
bool EntBuffer::bCheckPriorVec(Ent* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  vNumberEntries();
  
  Ent* aPriorEntry_0 = aPriorVec[0];
  Ent* aPriorEntry_1 = aPriorVec[1];
  Ent* aPriorEntry_2 = aPriorVec[2];
  
  Ent* aNextEntry_0  = aNextEntry(aPriorEntry_0,0);
  Ent* aNextEntry_1  = aNextEntry(aPriorEntry_1,1);
  Ent* aNextEntry_2  = aNextEntry(aPriorEntry_2,2);
 
  
  if (aNextEntry_0 && aNextEntry_1) {
    if (aNextEntry_0->iEntryNo() > aNextEntry_1->iEntryNo()) {
      vVerifyFault();
      return false;
    }
  }

  if (aNextEntry_1 && aNextEntry_2) {
    if (aNextEntry_1->iEntryNo() > aNextEntry_2->iEntryNo()) {
      vVerifyFault();
      return false;
    }
  }

  if (aNextEntry_0 && aNextEntry_2) {
    if (aNextEntry_0->iEntryNo() > aNextEntry_2->iEntryNo()) {
      vVerifyFault();
      return false;
    }
  }

  
  return true;
}

/*----------------------------------------------------------------------------*/
/* number entries                                                             */
/*----------------------------------------------------------------------------*/
void EntBuffer::vNumberEntries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  // set entry no
  
  i8 iEntryNo = 0;
  
  for (Ent* aTestEntry_0 = aNextEntry(aSkipRoots(),0);
       aTestEntry_0;
       aTestEntry_0 = aNextEntry(aTestEntry_0,0))

    aTestEntry_0->vEntryNo(iEntryNo++);
  
}

/*----------------------------------------------------------------------------*/
/* quick verify                                                               */
/*----------------------------------------------------------------------------*/
void EntBuffer::vQuickVerify(Ent* aSeeEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  for (Ent* aEnt = aFirstItem();
       aEnt != nullptr;
       aEnt = aAdjacentItem(aEnt))
    {;}
  
  for (Ent* aTestEntry = aNextEntry(aSkipRoots(),0);
       aTestEntry;
       aTestEntry = aNextEntry(aTestEntry,0)) {
    
    if (aSeeEntry)
      vShowEntry(aTestEntry);
    
    if (aTestEntry == aSeeEntry)
      int iDebug = 1;
    
    for (i8 i_ScanLevel = aTestEntry->nSkipLevels()-1; i_ScanLevel >= 0; i_ScanLevel--) {
      Ent* aCheckEntry = aNextEntry(aTestEntry,i_ScanLevel);
      if (! aCheckEntry)
        continue;
      
      if (CN_ENT != aCheckEntry->eClass())
        vShow();
      ASSERT(CN_ENT == aCheckEntry->eClass());
    }
    
  }
  
  //  ln i_ScanLevel;
  //  for (i_ScanLevel = 2; *i_ScanLevel >= 0; i_ScanLevel--) {
  //
  //    for (Ent* aTestEntry = aNextEntry(aSkipRoots(),*i_ScanLevel);
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
  //      if (CN_ENT != pTestEntry->eClass())
  //        vShow();
  //      ASSERT(CN_ENT == pTestEntry->eClass());
  //    }
  //  }
  
}

/*----------------------------------------------------------------------------*/
/* quick verify                                                               */
/*----------------------------------------------------------------------------*/
bool EntBuffer::bQuickVerify(Ent* aNotSeeEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  for (Ent* aTestEntry = aNextEntry(aSkipRoots(),0);
       aTestEntry;
       aTestEntry = aNextEntry(aTestEntry,0)) {
    
    for (i8 i_ScanLevel = aTestEntry->nSkipLevels()-1; i_ScanLevel >= 0; i_ScanLevel--) {
      
      Ent* aCheckEntry = aNextEntry(aTestEntry,i_ScanLevel);
      if (! aCheckEntry)
        continue;
      
      if (aCheckEntry == aNotSeeEntry) {
        vShowEntry(aTestEntry);
        vShowEntry(aCheckEntry);
        return false;
      }
      
      if (CN_ENT != aCheckEntry->eClass())
        return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for valid entry buffer                                                */
/*----------------------------------------------------------------------------*/
bool EntBuffer::bValidEntBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  if (! bValidMagic()) {
    vTrace("Not valid magic\n"_Stz);
    return false;
  }
  
  if (CN_ENTBUFFER != eClass()) {
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
bool EntBuffer::bVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  bool bValid = false;
  
  if (m_iBufferHeight) {
    bValid = bVerifyLnAny();
    return bValid;
  }
  
  bValid = bVerifyH0Other();
  return bValid;
}

/*----------------------------------------------------------------------------*/
/* verify the structure of this H0 buffer                                     */
/*----------------------------------------------------------------------------*/
bool EntBuffer::bVerifyH0Other(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  ASSERT(0 == m_iBufferHeight);
  
  //  vTrace("VerifyH0 %s: bufno: %lld level %lld\n"_Stz,
  //         m_pIndex->aIndexName(),m_iBufferNo,m_iBufferHeight);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify the contents of this H0 index buffer
  
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
bool EntBuffer::bVerifyLnAny(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  ASSERT(0 != m_iBufferHeight);
  
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
  
  bValid = bVerifySkipListLevels();
  if (! bValid)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify the contents of child buffers
  
  for (Ent* aEnt = aLeadEntry(); aEnt; aEnt = aNextEntry(aEnt,0)) {
    
    EntBuffer* pRefEntBuf = EntBuffer::pRefdEntBuffer(aEnt);
    if (pRefEntBuf->m_iBufferHeight) {
      bValid = pRefEntBuf->bVerifyLnAny();
    }
    else {
      bValid = pRefEntBuf->bVerifyH0Other();
    }
    if (! bValid)
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the adjacent entries not pointing to the same buffer                */
/*----------------------------------------------------------------------------*/
bool EntBuffer::bVerifyAdjacent(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  /*
   Every entry in an Ln buffer would point to a different buffer.  This
   routine performs a simple whereby two adjacent entries can't point to
   the same buffer.
   
   A more complete test would be to mark all buffers with some reference
   count.
   */
  
  ASSERT(m_iBufferHeight >= 1);
  
  IndexDef* pIndexDef = m_pIndex->pIndexDef();
  
  for (Ent* aEnt = aLeadEntry(); aEnt; aEnt = aNextEntry(aEnt,0)) {
    
    // get the buffer that this entry references
    EntBuffer* pChildEntBuffer = EntBuffer::pRefdEntBuffer(aEnt);
    
    // get the adjacent entry
    Ent* aAdjacentEntry = aNextEntry(aEnt,0);
    
    if (aAdjacentEntry) {
      // get the buffer that the adjacent entry references
      EntBuffer* pAdjacentChildEntBuffer =
      EntBuffer::pRefdEntBuffer(aAdjacentEntry);
      
      // the two child buffers should not be the same
      if ( pChildEntBuffer == pAdjacentChildEntBuffer) {
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
bool EntBuffer::bVerifyBufferRefs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  /*
   Every entry in an Ln buffer should reference an Ln-1 buffer. And
   the Ln-1 buffer should have its parent buffer be that Ln buffer
   */
  
  ASSERT(m_iBufferHeight >= 1);
  
  for (Ent* aEnt = aLeadEntry(); aEnt; aEnt = aNextEntry(aEnt,0)) {
    
    EntBuffer* pChildEntBuf = EntBuffer::pRefdEntBuffer(aEnt);
    
    if ( ! pChildEntBuf->bValidEntBuffer() ) {
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
bool EntBuffer::bVerifyFirstKeys(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  /*
   The for each entry in an Ln buffer will match the first key in an
   Ln-1 buffer
   */
  
  ASSERT(m_iBufferHeight >= 1);
  
  //vShow();
  
  for (Ent* aEnt = aLeadEntry(); aEnt; aEnt = aNextEntry(aEnt,0)) {
    
    // get the entry buffer that this entry references
    EntBuffer* pChildEntBuffer = EntBuffer::pRefdEntBuffer(aEnt);
    
    //pChildEntBuffer->vShow();
    
    
    // get the first entry of the child buffer
    Ent* aChildFirstEntry = pChildEntBuffer->aLeadEntry();
    
    // get keys of the two entries
    //Key* aKeyParent = _aKey(aEnt->aDataArea());
    //Key* aKeyChild  = _aKey(aChildFirstEntry->aDataArea());
    
    // the two keys should match
    //bool bEqual = aKeyChild->bEqualTo(aKeyParent,pIndexDef);
    bool bEqual = Key::bEqualTo(aChildFirstEntry->aKey(), aEnt->aKey());
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
bool EntBuffer::bVerifyItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  /*
   The items in and entry buffer are some sequence of:
   
   {[Ent]|[FreeItem]...}[Wilderness]
   
   The number of Entries must match the number of items in the buffer
   */
  
  i8 nEntryItems = 0;
  
  for (Ent* aEnt = aFirstItem();
       aEnt != nullptr;
       aEnt = aAdjacentItem(aEnt)) {
    
    //vTrace("Verify item %p\n"_Stz,pItem);
    
    eCN eClass = aEnt->eClass();
    
    switch (eClass) {
        
      case CN_ENT: {
        if (! aEnt->bValidEnt()) {
          vShow();
          vVerifyFault();
          return false;
        }
        nEntryItems++;
        break;
      }
        
      case CN_FREEITEM: {
        FreeItem* aFreeItem = _aFreeItem(aEnt);
        if (! aFreeItem->bValidFreeItem()) {
          vShow();
          vVerifyFault();
          return false;
        }
        break;
      }
        
      case CN_WILDERNESS: {
        Wilderness* aWilderness = _aWilderness(aEnt);
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
bool EntBuffer::bVerifyKeyBufferOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  /*
   The entries in an Ln buffer should be in order of the child buffer list
   */
  
  ASSERT(m_iBufferHeight >= 1);
  
  IndexDef* pIndexDef = m_pIndex->pIndexDef();
  
  for (Ent* aEnt = aLeadEntry(); aEnt; aEnt = aNextEntry(aEnt,0)) {
    
    // get next entry
    Ent* aNextEntry = this->aNextEntry(aEnt,0);
    if (! aNextEntry)
      break;
    
    // get the child entry buffers
    EntBuffer* pChildBufferA = EntBuffer::pRefdEntBuffer(aEnt);
    EntBuffer* pChildBufferB = EntBuffer::pRefdEntBuffer(aNextEntry);
    
    // child A should point to child B
    if (pChildBufferA->pNextBuffer() != pChildBufferB) {
      vDump();
      vShow(aEnt);
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
bool EntBuffer::bVerifyKeyOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  /*
   Any Hn buffer or H0 buffer should have its keys in ascending
   order.
   */
  
  for (Ent* aEnt = aLeadEntry(); aEnt; aEnt = aNextEntry(aEnt,0)) {
    
    // get next entry
    Ent* aNextEntry = this->aNextEntry(aEnt,0);
    if (! aNextEntry)
      break;
    
    // get keys of the two entries
    //Key* aKeyA = _aKey(aEnt->aDataArea());
    //Key* aKeyB = _aKey(aNextEntry->aDataArea());
    
    // next keyB should be greater than or equal to keyA
    //bool bGreaterEqual = aKeyB->bGreaterEqual(aKeyA,pIndexDef);
    bool bGreaterEqual = Key::bGreaterEqual(aNextEntry->aKey(), aEnt->aKey());
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
bool EntBuffer::bVerifyNextPrev(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  /*
   Any buffer should have a next whose prev point back to it; any prev should
   point forward to it; a child buffer should point back to its parent
   */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_pPrevBuffer) {
    EntBuffer* pPrevBuffer = _pEntBuffer(m_pPrevBuffer);
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
    EntBuffer* pNextBuffer = _pEntBuffer(m_pNextBuffer);
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
    EntBuffer* pHigherBuffer = _pEntBuffer(m_pHigherBuffer);
    if (pHigherBuffer->iBufferHeight() != m_iBufferHeight + 1) {
      vVerifyFault();
      return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the row references in this H0 index buffer                          */
/*----------------------------------------------------------------------------*/
bool EntBuffer::bVerifyRowRefs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  /*
   For this H0 index verify that its key entries referen a
   valid row in a row buffer
   */
  
  ASSERT(0 == m_iBufferHeight);
  
  for (Ent* aEnt = aLeadEntry(); aEnt; aEnt = aNextEntry(aEnt,0)) {
    
    // get the row buffer that this H0 key leaf references
    Ref xRef = aEnt->rxRef();
    //CrwBuffer* pCrwBuffer = CrwBuffer::pRefdCrwBuffer(xRef);
    
    DataBuffer* pDataBuffer = DataBuffer::pRefdDataBuffer(xRef);
    
    // get the row entry from referenced entry buffer
    i8 i_Item = aEnt->rxRef().iItem();
    
    if (pDataBuffer->eClass() == CN_CRWBUFFER) {
      CrwBuffer* pCrwBuffer = _pCrwBuffer(pDataBuffer);
      Crw* aCrw = pCrwBuffer->aCrw(i_Item);
      if (! aCrw ) {
        vVerifyFault();
        return false;
      }
    }
    else {
      ASSERT(pDataBuffer->eClass() == CN_CRVBUFFER);
      Crv* aCrv = AM::aCrv(pDataBuffer,i_Item);
      if (! aCrv ) {
        vVerifyFault();
        return false;
      }
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the skip list only contains only entries                            */
/*----------------------------------------------------------------------------*/
bool EntBuffer::bVerifySkipListEntry(bool bShowKeys)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  i8 nLvlLevels = AM::nLvlLevels();
  
  /*
   Run thru each scan list level and verify we discover entries with
   the same number of scan entries
   */
  
  IndexDef* pIndexDef = m_pIndex->pIndexDef();
  
  Ent* aNextEntry2;
  Ent* aNextEntry1;
  Ent* aNextEntry0;
  
  for (i8 i_ScanLevel = nLvlLevels - 1; i_ScanLevel >= 0; i_ScanLevel--) {
    
    if (bShowKeys)
      vTrace("i_ScanLevel %lld\n"_Stz,i_ScanLevel);
    
    Ent* aTestEntry_sl = aNextEntry(aSkipRoots(),i_ScanLevel);
    while (aTestEntry_sl) {
      
      if (CN_ENT != aTestEntry_sl->eClass()) {
        vVerifyFault();
        return false;
      }
      
      switch (i_ScanLevel) {
        case 2:
          aNextEntry2 = aNextEntry(aTestEntry_sl,2);
          if (aNextEntry2 && CN_ENT != aNextEntry2->eClass()) {
            vVerifyFault();
            return false;
          }
        case 1:
          aNextEntry1 = aNextEntry(aTestEntry_sl,1);
          if (aNextEntry1 && CN_ENT != aNextEntry1->eClass()) {
            vVerifyFault();
            return false;
          }
        case 0:
          aNextEntry0 = aNextEntry(aTestEntry_sl,0);
          if (aNextEntry0 && CN_ENT != aNextEntry0->eClass()) {
            vVerifyFault();
            return false;
          }
          break;
      }
      
      if (bShowKeys) {
        Key* aTestKey = _aKey(aTestEntry_sl->ayElmSeq());
        vTrace("Test %p: "_Stz,aTestEntry_sl);
        aTestKey->vShow(pIndexDef);
        vTrace("\n"_Stz);
      }
      
      aTestEntry_sl = aNextEntry(aTestEntry_sl,i_ScanLevel);
    }
  }
  
  if (! bVerifySkipListLevels())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the skip list levels                                                */
/*----------------------------------------------------------------------------*/
bool EntBuffer::bVerifySkipListLevels(bool bShowKeys)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  /*
   Verify entries with skip offsets greater than one that they
   can be found by following the  skip chain at that level, and
   that the entries all move forward (not backward).
   */
  
  // set entry no's
  vNumberEntries();
  
  // run through all entries at level 0
  Ent* aTestEntry_0 = aNextEntry(aSkipRoots(),0);
  while (aTestEntry_0) {
    
    // check if scan entry has more than one offset
    i8 nOffsets = aTestEntry_0->nSkipLevels();
    if (1 == nOffsets) {
      aTestEntry_0 = aNextEntry(aTestEntry_0,0);
      continue;
    }
    
    // for each level see if we can find the test entry
    for (i8 i_SkipLevel = nOffsets - 1;
         i_SkipLevel > 0;
         i_SkipLevel--) {
      
      Ent* aScanEntry_sl = aNextEntry(aSkipRoots(),i_SkipLevel);
      while (aScanEntry_sl) {
        
        if (aScanEntry_sl == aTestEntry_0)
          break;
        
        Ent* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_SkipLevel);
        
        // check we dont go backwwards
        if (aNextEntry_sl->iEntryNo() < aScanEntry_sl->iEntryNo()) {
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

/*----------------------------------------------------------------------------*/
/* verify has detected a fault                                                */
/*----------------------------------------------------------------------------*/
void EntBuffer::vVerifyFault(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  vTrace("VerifyFault\n"_Stz);
  ASSERT(0);
  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
