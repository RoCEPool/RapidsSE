/*============================================================================*/
/* EntryBuffer__toBeDeleted.cpp -- functions which are going to be deleted    */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;


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

///*----------------------------------------------------------------------------*/
///* position at the L0 buffer into which key should be inserted                */
///*----------------------------------------------------------------------------*/
//EntryBuffer* EntryBuffer::pPositionPriorBuffer(Item* aFindKey)
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
//    i8 iBufferHeight = pEntryBuffer->m_iBufferHeight();
//    if (iBufferHeight == 0) {
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
/* position at key as per relational operator                                 */
/*----------------------------------------------------------------------------*/
//Item* EntryBuffer::aPositionQuery(KeyWrk& xGivenKey, eRO eRelnOp)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
//
//#ifdef USE_EB_LAZY_CHAIN
//  RASSERT( ! m_aLazyChain );
//
//  //if (m_aLazyChain)
//  //  vBuildSkipList();
//#endif
//
//  Item* aIndexEntry = aPositionQuery(xGivenKey);
//
//  eKC eCompare;
//  if (aIndexEntry) {
//
//    if (eRelnOp == RO_GREATER_THAN) {
//
//      do {
//
//        eCompare = KeyData::eCompare(aIndexEntry, xGivenKey);
//
//        if (eCompare != KC_AFTER) {
//          aIndexEntry = aNextEntry(aIndexEntry, 0);
//        }
//      } while (aIndexEntry && eCompare != KC_AFTER);
//
//    }
//    else {
//      do {
//        eCompare = KeyData::eCompare(aIndexEntry, xGivenKey);
//
//        if (eCompare == KC_BEFORE) {
//          aIndexEntry = aNextEntry(aIndexEntry, 0);
//        }
//      } while (aIndexEntry && eCompare == KC_BEFORE);
//
//    }
//  }
//
//  return aIndexEntry;
//}

/*----------------------------------------------------------------------------*/
/* position at given key                                                      */
/*----------------------------------------------------------------------------*/
//Item* EntryBuffer::aPositionQuery(KeyWrk& xGivenKey)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
//
//#ifdef USE_EB_LAZY_CHAIN
//  RASSERT( ! m_aLazyChain );
//
//  //if (m_aLazyChain)
//  //  vBuildSkipList();
//#endif
//
//  Item* aScanEntry_sl = aSkipRoots();
//
//  for (i8 i_ScanLevel = 2; i_ScanLevel >= 0; i_ScanLevel--) {
//
//    Item* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
//    while (aNextEntry_sl) {
//
//      //KeyData* aNextKey = _aKeyData(aNextEntry_sl->aDataArea());
//      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
//      //aGivenKey->vShow(pIdxColVec); vTrace("\n"_Stz);
//
//      eKC eCompare = KeyData::eCompare(xGivenKey, aNextEntry_sl);
//
//      // have we found the key during a query?
//      if (eCompare == KC_SAME) {
//        if (0 == i_ScanLevel)
//          return aNextEntry_sl;
//        else
//          break;
//      }
//
//      // if given key is not same/after next key then break to outer loop
//      if (eCompare != KC_AFTER)
//        break;
//
//      aScanEntry_sl = aNextEntry_sl;
//      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanLevel);
//    }
//    //pPriorVec[*i_ScanLevel] = pScanEntry_sl;
//  }
//
//  if (aScanEntry_sl == aSkipRoots()) {
//    aScanEntry_sl = aNextEntry(aScanEntry_sl, 0);
//    // we currently think it should be null
//    // RASSERT( ! pScanEntry_sl );
//  }
//
//  return aScanEntry_sl;
//}

/*----------------------------------------------------------------------------*/
/* position at the L0 buffer into which key should be inserted                */
/*----------------------------------------------------------------------------*/
//EntryBuffer* EntryBuffer::pPositionPriorBuffer(KeyWrk& xFindKey)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
//
//  if ( ! nItems() ) {
//    ASSERT( 0 == iBufferHeight());
//    return this;
//  }
//
//  EntryBuffer* pEntryBuffer = this;
//  while (true) {
//
//    // have we arrived at L0 ?
//    i8 iBufferHeight = pEntryBuffer->iBufferHeight();
//    if (iBufferHeight == 0) {
//      break;
//    }
//    else {
//#ifdef USE_EB_LAZY_CHAIN
//      RASSERT( ! m_aLazyChain );
//#endif
//    }
//
//    // get the entry before the given insert key
//    Item* aEntryPrior = pEntryBuffer->pPositionPriorEntry(xFindKey);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
