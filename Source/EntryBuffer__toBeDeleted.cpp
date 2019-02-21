/*============================================================================*/
/* EntryBuffer__toBeDeleted.cpp -- functions which are going to be deleted    */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
