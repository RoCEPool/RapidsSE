/*============================================================================*/
/* EntryBuffer_position.cpp -- position related functions for EntryBuffer     */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* position at an index buffer at given height starting from this buffer      */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pPositionHeight(KeyWrk& xGivenKey, i8 iHeight)
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
  Item* aItem1;
  
  while (true) {
    iBufferHeight = pEntryBuffer->iBufferHeight();
    
    if (iBufferHeight == iHeight)
      break;
    
    // get the entry
    aItem1 = pEntryBuffer->aPositionQuery(xGivenKey);
    ASSERT(aItem1);
    
    aItem = pEntryBuffer->pPositionPriorEntry(xGivenKey);
    ASSERT(aItem);
    aItem = pEntryBuffer->pMoveToLessEqualEntry(aItem, xGivenKey, &pEntryBuffer);
    ASSERT(aItem);
//    if (aItem != aItem1) {
//      int debug = 1;
//      aItem1 = pEntryBuffer->aPositionQuery(xGivenKey);
//      ASSERT(aItem1);
//
//      aItem = pEntryBuffer->pPositionPriorEntry(xGivenKey);
//      ASSERT(aItem);
//      aItem = pEntryBuffer->pMoveToLessEqualEntry(aItem, xGivenKey, &pEntryBuffer);
//      ASSERT(aItem);
//    }
    
    // get the index buffer of the entry
    pEntryBuffer = EntryBuffer::pRefdEntryBuffer(aItem);
    ASSERT(pEntryBuffer);
    ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
    
    
  }
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* position at the L0 buffer into which key should be inserted                */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pPositionPriorBuffer(KeyWrk& xFindKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if ( ! nItems() ) {
    ASSERT( 0 == iBufferHeight());
    return this;
  }
  
  EntryBuffer* pEntryBuffer = this;
  while (true) {
    
    // have we arrived at L0 ?
    i8 iBufferHeight = pEntryBuffer->iBufferHeight();
    if (iBufferHeight == 0) {
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
Item* EntryBuffer::pPositionPriorEntry(Item* aFindKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (nItems() == 0)
    return aSkipRoots();
  
  Item* aScanEntry_sl = aSkipRoots();
  
  bool bKeyIsGreaterEqual;
  
  for (i8 i_ScanLevel = gpAttachMap->m_nIndexLevels - 1;
       i_ScanLevel >= 0;
       i_ScanLevel--) {
    
    Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
    while (aNextEntry_sl) {
      
      bKeyIsGreaterEqual = KeyData::bGreaterEqual(aNextEntry_sl, aFindKey);
      
      if (bKeyIsGreaterEqual)
        break;
      
      aScanEntry_sl = aNextEntry_sl;
      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanLevel);
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
  
  Item* aScanEntry_sl = aSkipRoots();
  
  bool bKeyIsGreaterEqual;
  
  for (i8 i_ScanLevel = gpAttachMap->m_nIndexLevels - 1;
       i_ScanLevel >= 0;
       i_ScanLevel--) {
    
    Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
    while (aNextEntry_sl) {
      
      bKeyIsGreaterEqual = KeyData::bGreaterEqual(aNextEntry_sl, xFindKey);
      
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
void EntryBuffer::vPositionPriorVecEntry(Item* aFindKey, Item* aPriorVec[],
                                         Item* aFindEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //ASSERT(0 == m_iBufferHeight);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexLevels; i_Offset++)
    aPriorVec[i_Offset] = aSkipRoots();
  
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
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
void EntryBuffer::vPositionPriorVecBuffer(Item* aFindKey, Item* aPriorVec[],
                                          EntryBuffer* pToBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(0 != m_iBufferHeight);
  
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
      
      if (0 == i_ScanLevel) {
        //bool bKeysAreEqual = aNextKey->bEqualTo(aFindKey,pIdxColVec);
        bool bKeysAreEqual = KeyData::bEqualTo(aNextEntry_sl, aFindKey);
        
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
/* position at key as per relational operator                                 */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aPositionQuery(KeyWrk& xGivenKey, eRO eRelnOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
  
  //if (m_aLazyChain)
  //  vBuildSkipList();
#endif
  
  Item* aIndexEntry = aPositionQuery(xGivenKey);
  
  eKC eCompare;
  if (aIndexEntry) {
    
    if (eRelnOp == RO_GREATER_THAN) {
      
      do {
        
        eCompare = KeyData::eCompare(aIndexEntry, xGivenKey);
        
        if (eCompare != KC_AFTER) {
          aIndexEntry = aNextEntry(aIndexEntry, 0);
        }
      } while (aIndexEntry && eCompare != KC_AFTER);
      
    }
    else {
      do {
        eCompare = KeyData::eCompare(aIndexEntry, xGivenKey);
        
        if (eCompare == KC_BEFORE) {
          aIndexEntry = aNextEntry(aIndexEntry, 0);
        }
      } while (aIndexEntry && eCompare == KC_BEFORE);
      
    }
  }
  
  return aIndexEntry;
}

/*----------------------------------------------------------------------------*/
/* position at given key                                                      */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aPositionQuery(KeyWrk& xGivenKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
  
  //if (m_aLazyChain)
  //  vBuildSkipList();
#endif
  
  Item* aScanEntry_sl = aSkipRoots();
  
  for (i8 i_ScanLevel = 2; i_ScanLevel >= 0; i_ScanLevel--) {
    
    Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
    while (aNextEntry_sl) {
      
      //KeyData* aNextKey = _aKeyData(aNextEntry_sl->aDataArea());
      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      //aGivenKey->vShow(pIdxColVec); vTrace("\n"_Stz);
      
      eKC eCompare = KeyData::eCompare(xGivenKey, aNextEntry_sl);
      
      // have we found the key during a query?
      if (eCompare == KC_SAME) {
        if (0 == i_ScanLevel)
          return aNextEntry_sl;
        else
          break;
      }
      
      // if given key is not same/after next key then break to outer loop
      if (eCompare != KC_AFTER)
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
  
  return aScanEntry_sl;
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
